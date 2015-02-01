// LibDecl.h - Library declarations and definitions

//////////////////////////////
//Forward class declarations//
//////////////////////////////
class cTimer;
class cInput;
class cGraphics;
class cEntity;
class cEntityMngr;
class cScores;
class cGame;
class cApplication;

/////////////////////////////////
//Links to global class objects//
/////////////////////////////////
extern cScores g_scores;
extern cEntityMngr g_entities;
extern cInput g_input;
extern cGraphics g_graphics;
extern cGame g_game;
extern cApplication g_app;

//////////////////////
//Include statements//
//////////////////////

// Identify the version of DirectInput 
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <time.h>
#include <windows.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <map>
#include <d3d9.h>
#include <d3dx9.h>
#include <sstream>			/* used to make unique ID's for entities spawned */
using namespace std;


#include "Debug.h"
#include "Util.h"
#include "Timer.h"
#include "Input.h"
#include "Entity.h"
#include "EntityMngr.h"
#include "Scores.h"
#include "Graphics.h"
#include "Game.h"
#include "Application.h"
#include "CONSTANTS.h"

//////////////////////////////////////////
//Macros for safely deleting & releasing//
//////////////////////////////////////////
#define SAFE_DELETE(p) if(p){delete p; p=NULL;}
#define SAFE_RELEASE(p) if(p){p->Release(); p=NULL;}