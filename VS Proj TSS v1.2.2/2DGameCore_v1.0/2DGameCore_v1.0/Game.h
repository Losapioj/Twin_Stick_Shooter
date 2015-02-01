#ifndef __GAME_H__
#define __GAME_H__

/////////////////////////////////////////////////
class cGame
{
protected:
	cTimer m_timer;

public:
	cTotalGameTime m_gameTime;

	cGame()  {}
	~cGame()  {if (DEBUG_MODE) debug << "GAME DESTRUCTOR" << endl;}
	bool Init (ifstream& fin);
	bool Run();
};

/////////////////////////////////////////////////
#endif //__GAME_H__