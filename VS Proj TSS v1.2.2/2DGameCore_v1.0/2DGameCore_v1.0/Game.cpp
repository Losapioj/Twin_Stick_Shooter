#include "LibDec.h"

/////////////////////////////////////////////////
cGame g_game;

/////////////////////////////////////////////////
bool cGame::Init(ifstream& fin)
{
	if(!fin.is_open())  
		return false;
	if(!g_graphics.Init(fin))  
		return false;
	if(!g_input.Init())  
		return false;
	if(!g_entities.Init(fin))  
		return false;

	const string& scoreFile = "HighScores.dat";
	ifstream scoreIn (scoreFile.c_str());
	if (!g_scores.Init(scoreIn))
		return false;

	srand((unsigned int)time(NULL));

	//spawn player and walls
	g_entities.SpawnPlayer(PLAYER_TYPE);
	
	//WALLS NOT WORKING, SEE "to change.txt in V1.2.2"
//	g_entities.AddToList(WALL, sVector2d(), TOP_WALL, PICK_ALL_LISTS);
//	g_entities.AddToList(WALL, sVector2d(), BOTTOM_WALL, PICK_ALL_LISTS);
//	g_entities.AddToList(WALL, sVector2d(), RIGHT_WALL, PICK_ALL_LISTS);
//	g_entities.AddToList(WALL, sVector2d(), LEFT_WALL, PICK_ALL_LISTS);

	return true;
}

/////////////////////////////////////////////////
bool cGame::Run()
{
	g_input.PollInput();
	if(g_input.KeyUp(DIK_ESCAPE))
	{
		PostMessage(g_app.GetWindowHandle(), 
			WM_DESTROY, 0, 0);
	}

	g_app.WindowResizedCheck();
//	RECT rc = {0, 0, g_app.GetWindowWidth(), 
//		g_app.GetWindowHeight()};

	g_graphics.ClearScreen(
		(char)255,(char)255,(char)255);

	float dt = m_timer.GetTime();
	m_gameTime.updateTime(dt);
//	if (!g_entities.SpawnEnemyByTime("Bat"))
//		debug << "ENTITY NOT SPAWNED";

	g_entities.UpdateAll(dt);
	g_scores.UpdateScore();
	g_entities.CollisionTest();

//	g_entities.RemoveDead();

	if(!g_graphics.Lock())  return false;

	g_graphics.DrawSurface(
		"Background", NULL, NULL);

	g_entities.DrawAll(); 
	g_scores.DisplayScores();
	g_graphics.Unlock();
	g_graphics.Display();

	return true;
}