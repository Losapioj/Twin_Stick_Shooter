#ifndef __INPUT_H__
#define __INPUT_H__


/////////////////////////////////////////////////
class cInput
{
private:
	// Pointer to the DirectInput Interface object
	LPDIRECTINPUT8 m_dinput;

	//keyboard input 
	LPDIRECTINPUTDEVICE8 m_keyboard;
	char m_keys [2] [256];

	//mouse input
	LPDIRECTINPUTDEVICE8 m_mouse;
	DIMOUSESTATE m_mouseState [2];

	// Current index
	int m_oldIndex, m_curIndex;

public:
	// Default constructor
	cInput ();

	// Destructor
	~cInput ();  

	// Initialization
	bool Init ();

	void AcquireInputDevices();
	void PollInput ();  
  
	int MouseX ();
	int MouseY ();
	int MouseZ ();

	bool ButtonDown (int i);
	bool ButtonPress (int i);
	bool ButtonUp (int i);

	bool KeyDown (int i);
	bool KeyPress (int i);
	bool KeyUp (int i);

};

/////////////////////////////////////////////////
#endif // __INPUT_H__