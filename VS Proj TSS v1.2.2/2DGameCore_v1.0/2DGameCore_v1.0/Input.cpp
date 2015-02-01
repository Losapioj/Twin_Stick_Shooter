#include "LibDec.h"

/////////////////////////////////////////////////
// Needed library files
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

/////////////////////////////////////////////////
cInput g_input;

/////////////////////////////////////////////////
cInput::cInput ()
: m_dinput(NULL), m_keyboard(NULL), 
  m_mouse(NULL), m_curIndex(0), m_oldIndex(1)
{
	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 256; ++j)
			m_keys[i][j] = 0;
}

/////////////////////////////////////////////////
cInput::~cInput ()
{
	if (DEBUG_MODE) 
		debug << "INPUT DESTRUCTOR" << endl;
	if (m_keyboard != NULL)
	{
		m_keyboard->Unacquire ();
		m_keyboard->Release ();
		m_keyboard = NULL;
	}
  
	if (m_mouse != NULL)
	{
		m_mouse->Unacquire ();
		m_mouse->Release ();
		m_mouse = NULL;
	}

	if (m_dinput != NULL)
		m_dinput->Release ();
	m_dinput = NULL;
}

/////////////////////////////////////////////////
bool cInput::Init ()
{  
	//initialize DirectInput object
	DirectInput8Create(GetModuleHandle (NULL), 
					   DIRECTINPUT_VERSION, IID_IDirectInput8, 
					   (void**)&m_dinput, NULL);  

	//initialize the keyboard
	m_dinput->CreateDevice (GUID_SysKeyboard, &m_keyboard, NULL);

	//set the data format for keyboard input
	m_keyboard->SetDataFormat(&c_dfDIKeyboard);

	//set the cooperative level
	m_keyboard->SetCooperativeLevel (g_app.GetWindowHandle(), 
									 DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);

	//initialize the mouse
	m_dinput->CreateDevice (GUID_SysMouse, &m_mouse, NULL);

	//set the data format for mouse input
	m_mouse->SetDataFormat (&c_dfDIMouse);

	//set the cooperative level
	m_mouse->SetCooperativeLevel (g_app.GetWindowHandle(), 
								  DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	AcquireInputDevices();

	return true;
}

/////////////////////////////////////////////////
void cInput::PollInput ()
{
/*	m_keyboard->Poll();

	if (FAILED(m_keyboard->GetDeviceState)
	{
		

	}*/

	m_oldIndex = m_curIndex;
	m_curIndex = (m_curIndex + 1) % 2;

	if (m_keyboard != NULL)
	{
		m_keyboard->GetDeviceState (256, (LPVOID) &m_keys[m_curIndex]);
	}

	if (m_mouse != NULL)
	{
		m_mouse->GetDeviceState(sizeof(m_mouseState[m_curIndex]), 
							    (LPVOID) &m_mouseState[m_curIndex]);
	}
} 

/////////////////////////////////////////////////  
int cInput::MouseX ()
{ 
	return m_mouseState[m_curIndex].lX;  
}

/////////////////////////////////////////////////
int cInput::MouseY ()
{  
	return m_mouseState[m_curIndex].lY;  
}

/////////////////////////////////////////////////
int cInput::MouseZ ()
{  
	return m_mouseState[m_curIndex].lZ;  
}

/////////////////////////////////////////////////
// return true if the key was not pressed last 
// frame but is pressed this frame
bool cInput::KeyDown (int i)
{
	return( (((m_keys[m_oldIndex][i]) & 0x80) == 0) &&
		    (((m_keys[m_curIndex][i]) & 0x80) != 0) );
}

/////////////////////////////////////////////////
// if the key was down last frame and is still
// down this frame
bool cInput::KeyPress (int i)
{
	return( (((m_keys[m_oldIndex][i]) & 0x80) != 0) && 
            (((m_keys[m_curIndex][i]) & 0x80) != 0) );
}

/////////////////////////////////////////////////
// if the key was down  last frame but is up 
// this frame
bool cInput::KeyUp (int i)
{
	return( (((m_keys[m_oldIndex][i]) & 0x80) != 0) && 
            (((m_keys[m_curIndex][i]) & 0x80) == 0) );
}

/////////////////////////////////////////////////
bool cInput::ButtonDown (int i)
{
	return( (((m_mouseState[m_oldIndex].rgbButtons[i]) & 0x80) == 0) &&
			(((m_mouseState[m_curIndex].rgbButtons[i]) & 0x80) != 0) );
}

/////////////////////////////////////////////////
bool cInput::ButtonPress (int i)
{
	return( (((m_mouseState[m_oldIndex].rgbButtons[i]) & 0x80) != 0) &&
			(((m_mouseState[m_curIndex].rgbButtons[i]) & 0x80) != 0) );
}

/////////////////////////////////////////////////
bool cInput::ButtonUp (int i)
{
	return( (((m_mouseState[m_oldIndex].rgbButtons[i]) & 0x80) != 0) &&
			(((m_mouseState[m_curIndex].rgbButtons[i]) & 0x80) == 0) );
}

void cInput::AcquireInputDevices()
{
	//acquire the keyboard
	m_keyboard->Acquire ();	
  
	//acquire the mouse 
	m_mouse->Acquire ();
}