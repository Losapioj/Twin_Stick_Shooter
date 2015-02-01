//Jeffrey Losapio
//win main for game core
//starts up the application and passes control to it


#include "LibDec.h"


int WINAPI WinMain (HINSTANCE hInst, 
                    HINSTANCE hPrev,
                    LPSTR szArgs, int nShow)
{
	// Set up check for memory leaks
	int tmpDbgFlag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
	// Always perform a leak check just before app exits
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag (tmpDbgFlag);

	//cApplication app;
	g_app.Init(hInst, "game.dat");
	return g_app.Run();
}