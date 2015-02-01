#include "LibDec.h"

cEntityMngr g_entities;

/////////////////////////////////////////////////
bool sEntitySpawnStats::Init(ifstream & fin)
{
	// Verify file is opened
	if (!fin)  
	{
		ErrorMsg("SpawnStruct.INIT: no fin found");
		return false;
	}

	if (DEBUG_MODE)
		debug << "ENTSPWN.init" << (char)fin.peek() << endl;
	// Read data
	fin >> m_entityID 
		>> m_maxSpeed >> m_minSpeed
        >> m_width >> m_height
        >> m_scale.m_x >> m_scale.m_y
		>> m_collisionCode >> m_imageId;

	if (DEBUG_MODE)
		debug << "ENTSPWN.init: " << m_entityID << " imageID: " << m_imageId << endl;

	m_entityUIDMod = 0;
	if (m_collisionCode == COL_TYPE_CIRCLE)
	{
		if (DEBUG_MODE)
			debug << "STRUCT FOR " << m_entityID << " CIRCLE COLLISION" << endl;
		m_collision = CIRCLE;
	}
	else if (m_collisionCode == COL_TYPE_BOX)
	{
		if (DEBUG_MODE)
			debug << "STRUCT FOR " << m_entityID << " BOX COLLISION" << endl;
		m_collision = AABOX;
	}
	m_color = 0xffffffff;

	return true;
}

/////////////////////////////////////////////////
cEntityMngr::~cEntityMngr() 
{
	if (DEBUG_MODE)
		debug << "ENTITY MANAGER DESTRUCTOR" << endl;
	while(!m_enemyList.empty())
	{
		ENEMY_ITER it = m_enemyList.begin();
		if(it->second != NULL)
			delete it->second;
		m_enemyList.erase(it);
	}
	while(!m_allyList.empty())
	{
		ALLY_ITER it = m_allyList.begin();
		if(it->second != NULL)
			delete it->second;
		m_allyList.erase(it);
	}
	while(!m_neutralList.empty())
	{
		NEUTRAL_ITER it = m_neutralList.begin();
		if(it->second != NULL)
			delete it->second;
		m_neutralList.erase(it);
	}

	if (entityStructArray != NULL)
	{
		for (int i = 0; i < ENTITY_TOTAL; i++)
		{
			if (DEBUG_MODE)
				debug << entityStructArray[i] << endl;
			if (entityStructArray[i] != NULL)
				delete entityStructArray[i];
		}
		delete [] entityStructArray;
		entityStructArray = NULL;
	}
	if (entityIDArray != NULL)
	{ 
		delete[] entityIDArray;
		entityIDArray = NULL;
	}
	if (DEBUG_MODE)
		debug << "ENTITY MANAGER DESTRUCTOR" << endl;
}

/////////////////////////////////////////////////
bool cEntityMngr::Init(ifstream& fin)
{
	entityIDArray = new string[ENTITY_TOTAL];
	entityStructArray = new sEntitySpawnStats*[ENTITY_TOTAL];

	string type;
	fin >> type;
	if(type != "[BeginEntityList]")
	{
		debug << "Error reading data file" << endl;
		return false;
	}

	fin >> type;
	sEntitySpawnStats* spawnStats = NULL;
	int currentEntityIter = 0;
	while(type != "[EndEntityList]")
	{
		if (DEBUG_MODE)
			debug << "created struct" << endl;
		spawnStats = new sEntitySpawnStats;

		if(spawnStats == NULL) 
		{
			ErrorMsg("Creating entity: " + type);
			fin >> type;
			continue;
		}

		if(!spawnStats->Init(fin))
		{
			ErrorMsg("Initializing struct");
			return false;
		}
		
		if (spawnStats != NULL)
		{
			entityIDArray[currentEntityIter] = spawnStats->m_entityID;
			spawnStats->numOfEntityAlive = 0;
			entityStructArray[currentEntityIter] = spawnStats;
			currentEntityIter++;
			if (DEBUG_MODE) 
				debug << "ENTMNG.init " << currentEntityIter << endl;
		}
		
		if (DEBUG_MODE) 
			debug << "ENTMNG.init " << spawnStats->m_entityID << endl;
		spawnStats = NULL;
		fin >> type;
	}

	return true;
}

/////////////////////////////////////////////////
bool cEntityMngr::AddToList(const string& entityID, 
							sVector2d parentPOS,
							float parentROT,
							short LIST_PICK_CONST)
{
	cEntity* entityToAdd = NULL;
	//check entity type to create
	if (entityID == PLAYER_TYPE)
		entityToAdd = new cAvatar;
	else if (entityID == PLAYER_BULLET)
		entityToAdd = new cPlayerBullet;
	else if (entityID == ENEMY_ONE)
		entityToAdd = new cBasicEnemy;
	else if (entityID == WALL)
		entityToAdd = new cWall;
	else if (entityID == EXPLOSION)
		entityToAdd = new cExplosion;

	//find struct for entity
	int entityPOS = 0;
	
	while (entityPOS < ENTITY_TOTAL)
	{
		if (g_entities.entityIDArray[entityPOS] == entityID)
		{
			if (DEBUG_MODE)
				debug << "ENT_MNGR.ADDTOLIST: entity struct found at: " << entityPOS << endl;
			break;
		}
		entityPOS++;
	}
	if (entityPOS >= ENTITY_TOTAL)
	{
		ErrorMsg("Entity not in array: " + entityID);
		return false;
	}

	//call entity's init
	if(!entityToAdd->Init(entityStructArray[entityPOS], parentPOS, parentROT))
	{
		ErrorMsg("Spawning entity" + entityID);
		return false;
	}

	//add to proper list(s)
	string entID = entityToAdd->GetID();

	if (DEBUG_MODE)
		debug << "ADD_TO_LIST EID: " << entID << endl;

	if (LIST_PICK_CONST == PICK_ENEMY_LIST)
		m_enemyList[entID] = entityToAdd;
	else if (LIST_PICK_CONST == PICK_ALLY_LIST)
		m_allyList[entID] = entityToAdd;
	else if (LIST_PICK_CONST == PICK_NEUTRAL_LIST)
		m_neutralList[entID] = entityToAdd;
/*	else if (LIST_PICK_CONST == PICK_ALL_LISTS)
	{
		m_allyList[entID] = entityToAdd;
		m_enemyList[entID] = entityToAdd;
	}*/
	else
	{
		ErrorMsg("INVALID LIST PICKED FOR" + entID);
	}

	return true;
}

/////////////////////////////////////////////////
bool cEntityMngr::SpawnPlayer(const string& playerID)
{
	if (playerID != PLAYER_TYPE)
	{
		ErrorMsg("Spawn Player: invalid player type \"" + playerID + "\"");
		return false;
	}
	cEntity* entity = new cAvatar;

	int avatarPOS = 0;
	while (avatarPOS < ENTITY_TOTAL)
	{
		if (g_entities.entityIDArray[avatarPOS] == playerID)
		{
			if (DEBUG_MODE)
				debug << "SPWN BY TIME: entity struct found at: " << avatarPOS << endl;
			break;
		}
		avatarPOS++;
	}
	if (avatarPOS >= ENTITY_TOTAL)
	{
		ErrorMsg("Entity not in array: " + playerID);
		return false;
	}

	if(!entity->Init(entityStructArray[avatarPOS]))
	{
		ErrorMsg("Spawning entity" + entity->GetID());
		return false;
	}

	string entID = entity->GetID();
	m_allyList[entID] = entity;

	return true;
}

/////////////////////////////////////////////////
void cEntityMngr::KillPlayer()
{
	while(!m_enemyList.empty())
		{
			ENEMY_ITER it = m_enemyList.begin();
			if(it->second != NULL)
				delete it->second;
			m_enemyList.erase(it);
		}
		while(!m_allyList.empty())
		{
			ALLY_ITER it = m_allyList.begin();
			if(it->second != NULL)
				delete it->second;
			m_allyList.erase(it);
		}

		if (DEBUG_MODE)
		{
			if (m_allyList.begin() == m_allyList.end())
				debug << "PLAYER KILLED: ALLY LIST EMPTY" << endl;
		}
		g_entities.SpawnPlayer(PLAYER_TYPE);
		if (DEBUG_MODE)
		{
			if (m_allyList.begin() == m_allyList.end())
				debug << "PLAYER KILLED: PLAYER DID NOT RESPAWN" << endl;
		}
}

/////////////////////////////////////////////////
// uses game time to determine when to spawn enemies
// searches entityIDArray for appropriate entity to spawn
// hands pointer to struct (from entityStructArray) to entity.
bool cEntityMngr::SpawnEnemyByTime(const string& enemyID)
{
	if (g_game.m_gameTime.getSec() % SEC_BTWN_SPAWNS == 0)
	{
		int totalEnemies = ENEMIES_PER_LEVEL + 
						   (ENEMIES_PER_LEVEL * (g_game.m_gameTime.getTotalSec() / SEC_BTWN_LEVELS));
		if (DEBUG_MODE)
		{
			debug << endl << "Total to spawn: " << totalEnemies 
				  << " Total time: " << g_game.m_gameTime.getTotalSec() 
				  << endl;
		}
		for (int index = 0; index < totalEnemies; index++)
		{

			if (DEBUG_MODE)
				debug << "SPAWNBYTIME start" << endl;
			cEntity* entity = new cBasicEnemy;
		
			// loop through entity list to find enemy's position in array
			int enemyPOS = 0;
			while (enemyPOS < ENTITY_TOTAL)
			{
				if (entityIDArray[enemyPOS] == enemyID)
				{
					if (DEBUG_MODE)
						debug << "SPWN BY TIME: entity struct found at: " << enemyPOS << endl;
					break;
				}
				enemyPOS++;
			}
			if (enemyPOS >= ENTITY_TOTAL)
			{
				ErrorMsg("Entity not in array: " + enemyID);
				return false;
			}

			if(!entity->Init(entityStructArray[enemyPOS]))
			{
				ErrorMsg("Spawning entity" + entity->GetID());
				return false;
			}

			string entID = entity->GetID();
			m_enemyList[entID] = entity;

			if (DEBUG_MODE)
			{
				debug << "\tTOTAL TO SPAWN: " << totalEnemies << endl;
				debug << "\tINDEX: " << index << endl;
				debug << "\tspawned ENTID: " << entID << endl;
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////
bool cEntityMngr::VerifyID(const string& id, short LIST_PICK_CONST)
{
	if (LIST_PICK_CONST == PICK_ENEMY_LIST)
	{
		ENEMY_ITER it = m_enemyList.find(id);
		if (it == m_enemyList.end())
		{
			debug << "Entity ID not found" << endl;
			return false;
		}
	}
	else if (LIST_PICK_CONST == PICK_ALLY_LIST)
	{
		ALLY_ITER it = m_allyList.find(id);
		if (it == m_allyList.end())
		{
			debug << "Entity ID not found" << endl;
			return false;
		}
	}

	return true;
}

/////////////////////////////////////////////////
void cEntityMngr::UpdateAll(float dt)
{
	ENEMY_ITER itEnemyCurrent, itEnemyNext;
	itEnemyCurrent = itEnemyNext = m_enemyList.begin();
	while (itEnemyCurrent != m_enemyList.end())
	{
		itEnemyNext ++;
		itEnemyCurrent->second->Update(dt);
		itEnemyCurrent = itEnemyNext;
	}
	ALLY_ITER itAllyCurrent, itAllyNext;
	itAllyCurrent = itAllyNext = m_allyList.begin();
	while (itAllyCurrent != m_allyList.end())
	{
		itAllyNext ++;
		itAllyCurrent->second->Update(dt);
		itAllyCurrent = itAllyNext;
	}
	
/*	for(ENEMY_ITER it = m_enemyList.begin(); it != m_enemyList.end(); it++)
	{
		if (it != m_enemyList.begin() && itLastEnemy != NULL)
			itLastEnemy = m_enemyList.begin();
		it->second->Update(dt);
		if (it != ++itLastEnemy)
			it = itLastEnemy;
	}*/
/*	for(ALLY_ITER it = m_allyList.begin(); it != m_allyList.end(); it++)
	{
		if (it != m_allyList.begin())
			itLastAlly = it - 1;
		it->second->Update(dt);
		if (it - 1 != itLastAlly)
			it = itLastAlly;
	}*/

	NEUTRAL_ITER itNeutralCurrent, itNeutralNext;
	itNeutralCurrent = itNeutralNext = m_neutralList.begin();
	while (itNeutralCurrent != m_neutralList.end())
	{
		itNeutralNext ++;
		itNeutralCurrent->second->Update(dt);
		itNeutralCurrent = itNeutralNext;
	}
}

/////////////////////////////////////////////////
void cEntityMngr::DrawAll()
{
	for(NEUTRAL_ITER it = m_neutralList.begin(); it != m_neutralList.end(); ++it)
	{
		it->second->Draw();
	}
	for(ENEMY_ITER it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		it->second->Draw();
	}
	for(ALLY_ITER it = m_allyList.begin(); it != m_allyList.end(); ++it)
	{
		it->second->Draw();
	}
}

/////////////////////////////////////////////////
bool cEntityMngr::BoxBoxTest(cEntity *e1, cEntity *e2)
{
	RECT r1;
	sVector2d p = e1->GetPosition();
	sVector2d s = e1->GetScale();
	int w = e1->GetWidth();
	int h = e1->GetHeight();

	r1.left = (long) p.m_x;
	r1.top = (long) p.m_y;
	r1.right = (long) (r1.left + w * s.m_x);
	r1.bottom = (long) (r1.top + h * s.m_y);

	RECT r2;
	p = e2->GetPosition();
	s = e2->GetScale();
	w = e2->GetWidth();
	h = e2->GetHeight();

	r2.left = (long) p.m_x;
	r2.top = (long) p.m_y;
	r2.right = (long) (r2.left + w * s.m_x);
	r2.bottom = (long) (r2.top + h * s.m_y);

	RECT dest;
	return IntersectRect(&dest, &r1, &r2) == TRUE;
}

/////////////////////////////////////////////////
bool cEntityMngr::CircleCircleTest(cEntity *e1, cEntity *e2)
{
	sVector2d p = e1->GetPosition();
	sVector2d s = e1->GetScale();
	int w = e1->GetWidth();
	int h = e1->GetHeight();

	sVector2d c1 (p.m_x + (w * s.m_x) / 2,
				  p.m_y + (h * s.m_y) / 2);

	float r1;
	if (w > h)
		r1 = (w*s.m_x) / 2;
	else
		r1 = (h*s.m_y) / 2;

	p = e2->GetPosition();
	s = e2->GetScale();
	w = e2->GetWidth();
	h = e2->GetHeight();

	sVector2d c2 (p.m_x + (w * s.m_x) / 2,
				  p.m_y + (h * s.m_y) / 2);

	float r2;
	if (w > h)
		r2 = (w*s.m_x) / 2;
	else
		r2 = (h*s.m_y) / 2;

	sVector2d d = c2 - c1;
	float dist = d.Length();

	return (dist < (r1 + r2) );
}

/////////////////////////////////////////////////
bool cEntityMngr::CircleBoxTest(cEntity *c, cEntity *b)
{
	//get center and radius of circle
	sVector2d p = c->GetPosition();
	sVector2d s = c->GetScale();
	int w = c->GetWidth();
	int h = c->GetHeight();

	sVector2d center (p.m_x + (w * s.m_x) / 2,
				      p.m_y + (h * s.m_y) / 2);

	float radius;
	if (w > h)
		radius = (w*s.m_x) / 2;
	else
		radius = (h*s.m_y) / 2;

	//get rect for box
	RECT rc;
	p = b->GetPosition();
	s = b->GetScale();
	w = b->GetWidth();
	h = b->GetHeight();

	rc.left = (long) p.m_x;
	rc.top = (long) p.m_y;
	rc.right = (long) (rc.left + w * s.m_x);
	rc.bottom = (long) (rc.top + h * s.m_y);

	bool centerIsRightOfLeft = rc.left <= center.m_x;
	bool centerIsLeftOfRight = rc.right >= center.m_x;
	bool centerIsBelowTop = rc.top <= center.m_y;
	bool centerIsAboveBottom = rc.bottom >= center.m_y;

	//test where circle center is in relation to rect
	//return collision test
	if (centerIsRightOfLeft)
	{
		if (centerIsLeftOfRight)
		{
			if (centerIsBelowTop)
			{
				if (centerIsAboveBottom) // center is inside rect
					return true;
				else // center is below rect
					return ( rc.bottom >= (center.m_y - radius) );
			}
			else // center is above rect
				return (rc.top <= (center.m_y + radius) );
		}
		//center is to the right of rect... somewhere
		else if (centerIsBelowTop) 
		{
			if (centerIsAboveBottom) // center right
				return (rc.right >= (center.m_x - radius) );
			else // center is right AND below
			{
				sVector2d corner ( (float)rc.right, (float)rc.bottom);
				sVector2d dist = corner - center;
				return (dist.Length() <= radius);
			}
		}
		else // center is right AND above
		{
			sVector2d corner ( (float)rc.right, (float)rc.top);
			sVector2d dist = corner - center;
			return (dist.Length() <= radius);
		}
	}
	//center is to left of rect... somewhere
	else if (centerIsBelowTop) 
	{
		if (centerIsAboveBottom) // center left
			return (rc.left <= (center.m_x + radius) );
		else // center is left AND below
		{
			sVector2d corner ( (float)rc.left, (float)rc.bottom);
			sVector2d dist = corner - center;
			return (dist.Length() <= radius);
		}
	}
	else // center is left AND above
	{
		sVector2d corner ( (float)rc.left, (float)rc.top);
		sVector2d dist = corner - center;
		return (dist.Length() <= radius);
	}
}

/////////////////////////////////////////////////
void cEntityMngr::CollisionTest()
{
	ENEMY_ITER itEnemyNext, itEnemyCurrent;
	ALLY_ITER itAllyNext, itAllyCurrent;

	bool endLevel = false;
	bool collision = false;
	itEnemyCurrent = itEnemyNext = m_enemyList.begin();

	while (itEnemyCurrent != m_enemyList.end())
	{
		itEnemyNext ++;

		itAllyCurrent = itAllyNext = m_allyList.begin();
		while (itAllyCurrent != m_allyList.end())
		{
			itAllyNext ++;
			
			CollisionType col1 = itEnemyCurrent->second->GetCollisionType();
			CollisionType col2 = itAllyCurrent->second->GetCollisionType();

			if (col1 == AABOX && col2 == AABOX)
				collision = BoxBoxTest(itEnemyCurrent ->second, itAllyCurrent ->second);
			else if (col1 == CIRCLE && col2 == CIRCLE)
				collision = CircleCircleTest(itEnemyCurrent ->second, itAllyCurrent ->second);
			else if (col1 == CIRCLE && col2 == AABOX)
				collision = CircleBoxTest(itEnemyCurrent ->second, itAllyCurrent ->second);
			else if (col1 == AABOX && col2 == CIRCLE)
				collision = CircleBoxTest(itEnemyCurrent ->second, itAllyCurrent ->second);

			if(collision)
			{
				string allyID = itAllyCurrent->second->GetID();
				string enBaseID = itEnemyCurrent->second->GetBaseID();
				string alBaseID = itAllyCurrent->second->GetBaseID();
				
				//do ally collision
				itAllyCurrent->second->OnCollision(enBaseID);	

				//check that player is still alive
				if (alBaseID == PLAYER_TYPE && !VerifyID(allyID, PICK_ALLY_LIST))
				{
					if (DEBUG_MODE)
					{
						debug << "**PLAYER NO LONGER IN MAP** ID: " << allyID << endl;
					}
					endLevel = true;
					break;
				}

				//do enemy collision, go pick new enemy if dead
				if (itEnemyCurrent->second->OnCollision(alBaseID) == IS_KILLED)
				{
					if(DEBUG_MODE)
						debug << "Breaking ally loop" << endl;
					break;
				}
			}
			collision = false;

			itAllyCurrent = itAllyNext;
		} //END INNER WHILE

		//check if we're ending the loop
		if (endLevel)
		{
			if (DEBUG_MODE)
				debug << "**PLAYER NO LONGER IN MAP** ENDING COL CHECKS " << endl;
			break;
		}

		itEnemyCurrent = itEnemyNext;
	} //END OUTER WHILE
}

/////////////////////////////////////////////////
void cEntityMngr::RemoveFromList(cEntity* entToDel, short LIST_PICK_CONST)
{
	string entID = entToDel->GetID();

	if (DEBUG_MODE)
		debug << "REMOVE FROM LIST: " << entID << endl;

	if (LIST_PICK_CONST == PICK_ENEMY_LIST)
		m_enemyList.erase(entID);
	else if (LIST_PICK_CONST == PICK_ALLY_LIST)
		m_allyList.erase(entID);
	else if (LIST_PICK_CONST == PICK_NEUTRAL_LIST)
		m_neutralList.erase(entID);
/*	else if (LIST_PICK_CONST == PICK_ALL_LISTS)
	{
		m_allyList.erase(entID);
		m_enemyList.erase(entID);
	}*/
	delete entToDel;
}
////////////////////////////////////////////////////////////////

