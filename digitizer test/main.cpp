// compilation control #defines:
#if defined(_DEBUG) && !defined(DEBUG)
#define DEBUG
#endif

// #includes:
#include <iostream>
using std::cout;
using std::wcout;
using std::cin;
using std::endl;
using std::cerr;
using std::clog;
using std::left;

#include <iomanip>
using std::setw;
using std::setprecision;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::wofstream;
using std::fstream;
using std::ios;

#include <cstdlib>
using std::system;
using std::exit;

#include <exception>
using std::exception;

#include <stdexcept>
using std::runtime_error;
using std::logic_error;
using std::invalid_argument;
using std::bad_alloc;


#include <gl/glut.h>

#include <Direct Input/DirectInput Wrapper.h>
#include <Direct Input/Digitizer Wrapper.h>
#include <Direct Input/Keyboard Wrapper.h>
#include <windows/common.h>
#include <StopWatch.h>



template<typename StoredType>
class DisplayList
{
public:

	/***************************
	* contructors & destructor *
	***************************/

	inline DisplayList()															// DisplayList default constructor
	{
		// do nothing
	} // end DisplayList default constructor


	inline DisplayList(const DisplayList &original)										// DisplayList copy constructor
		:iList(original.iList)
	{
		// empty body
	} // end DisplayList copy constructor


	inline DisplayList(StoredType List)												// DisplayList constructor
		:iList(List)
	{
		// empty body
	} // end DisplayList constructor


	inline ~DisplayList()															// DisplayList destructor
	{
		// empty body
	} // end DisplayList destructor




	/************
	* operators *
	************/

	inline DisplayList &operator = (const DisplayList &original)							// operator =
	{
		iList = original.iList;
		return *this;
	} // end function operator =


	/*******************
	* member functions *
	*******************/

	inline DisplayList &setList(StoredType List)												// setList
	{
		iList = List;
		return *this;
	} // end function setList


private:

	/***************
	* data members *
	***************/

	StoredType iList;

}; // end class DisplayList









void intTrower(){throw 0;}										// intTrower

// main
int main(int argc , char **argv)
{
	wofstream properties("c:/output/properties.txt");
	properties<<std::boolalpha;
	ofstream output("c:/output/digitizer_log.txt");
	ofstream mean("c:/output/mean.txt");
	StopWatch timer("ms");


	glutInit(&argc,argv);
	glutInitWindowPosition(640,640);
	glutInitWindowSize(32,32);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Digitizer Test");

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(-1,1,-1,1);
	glutDisplayFunc(intTrower);
	try
	{
		glutMainLoop();
	}
	catch(int)
	{
		// do nothing
	} // end catch
	WindowHandle mainWindow = FindWindow(0,"Digitizer Test");

	DigitizerState oldState;
	double sum = 0;
	unsigned int n = 0;
	unsigned int c = 0;

	DirectInput directInput(GetModuleHandle(NULL));
	DIKeyboard keyboard(directInput,mainWindow);
	DIDigitizer digitizer(directInput,mainWindow);

	digitizer.acquire();
	digitizer.getProperties();
	properties<<static_cast<DigitizerProperties &>(digitizer)<<endl;

	keyboard.acquire();
	while(1)
	{
		keyboard.getState();
		if(keyboard.key[DIK_ESCAPE] & 0x80)
			break;

		digitizer.getState();


		if((digitizer.InRange & 0x80) && !(oldState.InRange & 0x80))
		{
			sum = 0;
			n = 0;
			timer.push();
		} // end if

		timer.push();
		if(digitizer != oldState)
		{
			++c;
			if(digitizer.InRange & 0x80)
			{
				++n;
				sum += timer.pop();
				timer.push();
			} // end if

			system("CLS");
			cout<<digitizer<<endl;
			output<<digitizer<<endl;

			if(!(digitizer.InRange & 0x80) && (oldState.InRange & 0x80))
			{
				mean << sum/n << timer.getUnit() << endl;
				timer.pop();
			} // end if

			oldState = digitizer;
		} // end if 
	} // end while
	cout<<timer.pop()/c<<timer.getUnit()<<endl;

	output.close();
	mean.close();
	system("start c:/output/mean.txt");
	system("start c:/output/digitizer_log.txt");
	system("PAUSE");
	return 0;
} // end function main