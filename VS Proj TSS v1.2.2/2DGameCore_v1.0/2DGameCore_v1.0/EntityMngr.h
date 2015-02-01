#ifndef __ENTITYMNGR_H__
#define __ENTITYMNGR_H__

#include "CONSTANTS.h"

extern enum CollisionType;
struct sEntitySpawnStats
{
public:
	sEntitySpawnStats() {}
	bool Init(ifstream& fin);

	string m_entityID;
	unsigned long m_entityUIDMod;
	int numOfEntityAlive;

	int m_maxSpeed, m_minSpeed;
	sVector2d m_scale;
	int m_width, m_height;

	string m_collisionCode;
	CollisionType m_collision;
	DWORD m_color;
	string m_imageId;
};

/////////////////////////////////////////////////
class cEntityMngr
{
private:
	//2 arrays holding entityID's and pointers to entity structs
	string* entityIDArray;
	sEntitySpawnStats** entityStructArray;

	/*enemies can't collide with enemies
	  allies can't collide with allies
	  saves on processing with many enemies on screen
	  */

	//map of enemies and iterators to go through
	typedef map<string, cEntity*> ENEMY_LIST;
	typedef ENEMY_LIST::iterator ENEMY_ITER;
	ENEMY_LIST m_enemyList;

	//map of player/bullets and iterators to go through
	typedef map<string, cEntity*> ALLY_LIST;
	typedef ALLY_LIST::iterator ALLY_ITER;
	ALLY_LIST m_allyList;

	//map of non-interacting objects, like temporary explosions.
	//no collision detection on these
	typedef map<string, cEntity*> NEUTRAL_LIST;
	typedef NEUTRAL_LIST::iterator NEUTRAL_ITER;
	NEUTRAL_LIST m_neutralList;

	bool VerifyID(const string& id, short LIST_PICK_CONST);

	bool BoxBoxTest(cEntity* e1, cEntity* e2);
	bool CircleCircleTest(cEntity* e1, cEntity* e2);
	bool CircleBoxTest(cEntity* e1, cEntity* e2);

public:
	cEntityMngr() {}
	~cEntityMngr();
	bool Init(ifstream& fin);
	bool SpawnEnemyByTime(const string& enemyID);
	bool SpawnPlayer(const string& playerID);
	void KillPlayer();

	bool AddToList(const string& entityID, 
				   sVector2d parentPOS,
				   float parentROT, 
				   short LIST_PICK_CONST);
	void RemoveFromList(cEntity* entToDel, short LIST_PICK_CONST);

	void UpdateAll(float dt);
	void DrawAll();
	void CollisionTest();
//	void RemoveDead(); // g_entities.RemoveFromList(this, PICK_ALLY_LIST);
};


/////////////////////////////////////////////////
#endif // __ENTITYMNGR_H__