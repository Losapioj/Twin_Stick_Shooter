#ifndef __APPLICATION_H__
#define __APPLICATION_H__

/////////////////////////////////////////
//Represents application and its window//
/////////////////////////////////////////
class cApplication
{
private:
	RECT m_windowRect;
	int m_width, m_height;
	string m_appName;
	HWND m_hWnd;
	HINSTANCE m_hInst;
	bool m_fullscreen;
	bool m_windowResized;
  
public:
	cApplication();
	~cApplication() {}
	bool Init(HINSTANCE hInst, const string& file);
	int Run();

	void GetDesktopResolution(int* horizontal, int* vertical);
	bool WindowResizedCheck();

	int GetWindowWidth()  {  return m_width;  }
	int GetWindowHeight()  {  return m_height;  }
	string GetAppName()  {  return m_appName;  }
	HWND GetWindowHandle()  {  return m_hWnd;  }
	HINSTANCE GetAppHandle()  {  return m_hInst;  }
	bool IsFullscreen()  {  return m_fullscreen;  }
	
};

/////////////////////////////////////////////////
#endif // __APPLICATION_H__