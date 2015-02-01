#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "Util.h"
#include "Application.h"

const static bool DEBUG_MODE = false;

//////////////////////
// entity constants //
//////////////////////

//Entity Manager
//const static short PICK_ALL_LISTS = -1;
const static short PICK_ENEMY_LIST = 0;
const static short PICK_ALLY_LIST = 1;
const static short PICK_NEUTRAL_LIST = 2;

/////////////////////////////////////////////////////
////////////////////TO ADD ENTITY////////////////////
/////////////////CHANGE ENTITY_TOTAL/////////////////
/////////////ADD ENTITY IDENTIFIER string////////////
/////////////////////////////////////////////////////

// entity struct creation
const static string COL_TYPE_BOX = "box";
const static string COL_TYPE_CIRCLE = "circle";

// enemy spawning
const static int ENTITY_TOTAL = 5; //avatar, enemy1, bullet, walls, explosion
// Entity type constants
// used to find structs in entity creation
const static string PLAYER_TYPE = "Avatar";
const static string PLAYER_BULLET = "Bullet";
const static string ENEMY_ONE = "BasicEnemy1";		/* used in timer when calling spawner */
const static string WALL = "Wall";
const static string EXPLOSION = "Explosion";
//collision detection
const static bool IS_KILLED = true;
const static bool IS_NOT_KILLED = false;

// avatar
const static float PLAYER_ACCEL = 200.0;
const static float PLAYER_DECEL = 50.0;
const static float DEG_TO_RAD = D3DX_PI / 180;
const static float PLAYER_ROT_ADJUST = 90.0;	/* degree adjust for player model to face right */
const static float SHOT_DELAY = 0.125f;
//explosion
const static float FRAME_DELAY = 0.05f;
const static int EXPLOSION_DEGRADE_RATE = 25;
const static int EXPLOSIONS_PER_KILL = 20;
const static int EARLY_DEATH_PCT = 20;

// spawners OR spawn at walls
const static int SEC_BTWN_SPAWNS = 2;
const static int SEC_BTWN_LEVELS = 15;
const static int ENEMIES_PER_LEVEL = 3;
//const static int NUM_OF_SPAWNERS = 2;
//const static sVector2d SPAWN_ONE_POS = sVector2d(50, 50);
//const static sVector2d SPAWN_TWO_POS = sVector2d(g_app.GetWindowWidth() - 50.0f, g_app.GetWindowHeight() - 50.0f); /*!!top left corner!!*/
//sVector2d[NUM_OF_SPAWNERS] SPAWN_LOCATIONS = {SPAWN_ONE_POS, SPAWN_TWO_POS};

	//OR
const static int TOP_WALL = 0;
const static int RIGHT_WALL = 1;
const static int BOTTOM_WALL = 2;
const static int LEFT_WALL = 3;
const static int SPAWN_DIST_FROM_WALL = 20;



const static int ASTEROID_ROT_SPD = 10;


// timer
const static int MILLI_IN_SEC = 1000;
const static int SEC_IN_MIN = 60;
const static int MIN_IN_HR = 60;
const static int HR_IN_DAY = 24;

#endif //__CONSTANTS_H__