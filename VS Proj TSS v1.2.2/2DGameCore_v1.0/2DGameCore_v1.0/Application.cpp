// Application.cpp - 

#include "LibDec.h"

/////////////////////////////////////////////////
cApplication g_app;

/////////////////////////////////////////////////
LRESULT CALLBACK WinProc (HWND hwnd, UINT msg, 
						  WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		//check if escape key (or any other exit) was called
		case WM_DESTROY:
			PostQuitMessage (0);
			return 0;
		
		//check if the screen loses or gains focus
		//if lose focus, pause game
		//if gain focus, reacquire keyboard and mouse and unpause
		case WM_ACTIVATE:
			if (DEBUG_MODE)
				debug << "WM_ACTIVATE PASSED" << endl;

			if ((HWND)lParam == hwnd)		//FOCUS LOST
				; //PAUSE GAME HERE
			else if ((HWND)lParam != hwnd)	//FOCUS GAINED
			{
				g_input.AcquireInputDevices(); 
			}

			return 0;
	}

	return DefWindowProc (hwnd, msg, 
                          wParam, lParam);
}

/////////////////////////////////////////////////
cApplication::cApplication()
: m_width (0), m_height (0), m_hWnd (0),  
  m_hInst (0), m_fullscreen (false)
{}

/////////////////////////////////////////////////
bool cApplication::Init(HINSTANCE hInst, 
                        const string& file)
{
	ifstream fin (file.c_str());
	if(!fin)
		return false;

	m_hInst = hInst;
	getline(fin, m_appName);
	fin >> m_width >> m_height 
		>> boolalpha >> m_fullscreen;

	WNDCLASSEX wc = {sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW, WinProc, 0, 0, 
		m_hInst, NULL, LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH), NULL, 
		m_appName.c_str(), NULL};

	if (!RegisterClassEx (&wc))
		return false;

	// Define window style
	// Style differs for windowed vs. full screen
	int style = WS_OVERLAPPEDWINDOW;
	if(m_fullscreen)
		style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;

	m_hWnd = CreateWindow (
		m_appName.c_str(),      // Window Class
		m_appName.c_str(),      // Titlebar text
		style,					// Window Style
		CW_USEDEFAULT,          // X-position
		CW_USEDEFAULT,          // Y-position
		m_width, m_height,      // Dimensions
		NULL,                   // Parent Window
		NULL,                   // Menu
		m_hInst,                // Handle to instance
		NULL);                  // Creation data

	ShowWindow (m_hWnd, SW_SHOWNORMAL);
	UpdateWindow (m_hWnd);

	//set window to fit screen.
	if (m_width <= 0 || m_height <= 0)
	{
		GetDesktopResolution(&(m_width), &(m_height));
	}
	SetWindowPos(m_hWnd, HWND_TOP, 0, 0, m_width, m_height, 
		         SWP_NOMOVE|SWP_NOACTIVATE);

	//set game window in memory
	//reuses m_width and m_height to hold the client rect from here on
	GetClientRect(m_hWnd, &m_windowRect);
	m_height = m_windowRect.bottom - m_windowRect.top;
	m_width = m_windowRect.right - m_windowRect.left;
	m_windowResized = false;

	if(!g_game.Init(fin))  
		return false;

	fin.close();
  
	return true;
}

/////////////////////////////////////////////////
int cApplication::Run()
{
	MSG msg;
	bool done = false;
	while (!done)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				done = true;
/*			else if (msg.message == WM_ACTIVATE)
			{
				if (DEBUG_MODE)
					debug << "WM_ACTIVATE PASSED" << endl;
				g_input.AcquireInputDevices();
			}*/
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			g_game.Run();
	}

	return msg.wParam;
}

/////////////////////////////////////////////////
bool cApplication::WindowResizedCheck()
{
	RECT window;
	GetClientRect(m_hWnd, &window);
	if (window.top == m_windowRect.top &&
		window.left == m_windowRect.left &&
		window.right == m_windowRect.right &&
		window.bottom == m_windowRect.bottom)
	{
		m_windowResized = false;
		return m_windowResized;
	}

	if(DEBUG_MODE)
		debug << "resize check true: window resized\n";

	m_height = window.bottom - window.top;
	m_width = window.right - window.left;
	m_windowRect = window;

	m_windowResized = true;
	return m_windowResized;
}

/////////////////////////////////////////////////
void cApplication::GetDesktopResolution(int* horizontal, int* vertical)
{
   RECT desktop;
   // Get the size of screen to the variable desktop
   GetWindowRect(GetDesktopWindow(), &desktop);
   // The top left corner will have coordinates (0,0)
   // and the bottom right corner will have coordinates
   // (horizontal, vertical)
   *(horizontal) = desktop.right;
   *(vertical) = desktop.bottom;
}