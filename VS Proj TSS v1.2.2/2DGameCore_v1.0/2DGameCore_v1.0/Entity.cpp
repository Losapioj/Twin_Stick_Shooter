// Entity.cpp - class cEntity implementation

#include "LibDec.h"
#include <stdlib.h>




/////////////////////////////////////////////////
// Entity defaults (called by others)
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Initializes all data members from data file
bool cEntity::Init (sEntitySpawnStats* myStruct, 
					sVector2d parentPos, 
					float parentRot)
{
	m_baseID = myStruct->m_entityID;
	// Entity ID
	ostringstream entityUIDSS;
	entityUIDSS << m_baseID << myStruct->m_entityUIDMod;
	m_id = entityUIDSS.str();
	myStruct->m_entityUIDMod++;

	// Speed (in 2 dimensions)
	m_maxSpd = myStruct->m_maxSpeed;
	m_minSpd = myStruct->m_minSpeed;

	// Scale (in 2 dimensions)
	m_scl = myStruct->m_scale;

	// Dimensions of each tile
	m_width = myStruct->m_width;
	m_height = myStruct->m_height;

	m_collision = myStruct->m_collision;
	m_color = myStruct->m_color;
	m_imageId = myStruct->m_imageId;

	return true;
}

/////////////////////////////////////////////////
// Draw current tile at current position
void cEntity::Draw ()
{
	sVector2d center(m_width/2.0f,
                     m_height/2.0f);
	g_graphics.DrawTexture(m_imageId, NULL, &m_pos, 
						   &center, &m_scl, m_rotate, 
						   m_color);
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////Avatar ENTITIY//////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

/////////////////////////////////////////////////
// Default constructor sets all data members to 
// zero or NULL
cAvatar::cAvatar ()
{
	m_rotate = 0.0f;
}

/////////////////////////////////////////////////
// Initializes all data members from data file
bool cAvatar::Init (sEntitySpawnStats* myStruct, 
					sVector2d parentPos, 
					float parentRot)
{
	if (!cEntity::Init(myStruct))
	{
		ErrorMsg("cAvatar::Init call");
		return false;
	}

	// Current screen position
	m_pos.m_x = g_app.GetWindowWidth()/2.0f;
	m_pos.m_y = g_app.GetWindowHeight()/2.0f; 

	//set defaults for shot vars
	m_shotDelay = SHOT_DELAY;
	m_timeSinceLastShot = SHOT_DELAY;
	return true;
	
}

/////////////////////////////////////////////////
// Update position and possibly tile and clipping 
void cAvatar::Update (float dt)
{
	// Process user input here
	bool left1 = g_input.KeyPress(DIK_A);
	bool right1 = g_input.KeyPress(DIK_D);
	bool up1 = g_input.KeyPress(DIK_W);
	bool down1 = g_input.KeyPress(DIK_S);
	bool left2 = g_input.KeyPress(DIK_LEFT);
	bool right2 = g_input.KeyPress(DIK_RIGHT);
	bool up2 = g_input.KeyPress(DIK_UP);
	bool down2 = g_input.KeyPress(DIK_DOWN);

	bool shoot1 = g_input.KeyPress(DIK_SPACE);
	bool shoot2 = g_input.ButtonPress(0);
//	g_input.ButtonDown(0);
//	g_input.ButtonUp(0);

	//combine both input methods to one control for code
	bool left = false, 
		 right = false, 
		 up = false, 
		 down = false,
		 shoot = false;
	if (left1 || left2)
		left = true;
	if (right1 || right2)
		right = true;
	if (up1 || up2)
		up = true;
	if (down1 || down2)
		down = true;
	if (shoot1 || shoot2)
		shoot = true;

	//control movement
	if(left && right)
		left = right = 0;
	if(up && down)
		up = down = 0;

	if (left)
		m_spd.m_x -= (float)(PLAYER_ACCEL * dt);
	else if (right)
		m_spd.m_x += (float)(PLAYER_ACCEL * dt);
	else if (m_spd.m_x != 0.0f)
	{
		if (m_spd.m_x > 0.0f)
		{
			m_spd.m_x -= PLAYER_DECEL * dt;
			if (m_spd.m_x < 0.0f)
				m_spd.m_x = 0.0f;
		}
		else if (m_spd.m_x < 0.0f)
		{
			m_spd.m_x += PLAYER_DECEL * dt;
			if (m_spd.m_x > 0.0f)
				m_spd.m_x = 0.0f;
		}
	}

	if (up)
		m_spd.m_y -= (float)(PLAYER_ACCEL * dt);
	else if (down)
		m_spd.m_y += (float)(PLAYER_ACCEL * dt);
	else if (m_spd.m_y != 0.0f)
	{
		if (m_spd.m_y > 0.0f)
		{
			m_spd.m_y -= PLAYER_DECEL * dt;
			if (m_spd.m_y < 0.0f)
				m_spd.m_y = 0.0f;
		}
		else if (m_spd.m_y < 0.0f)
		{
			m_spd.m_y += PLAYER_DECEL * dt;
			if (m_spd.m_y > 0.0f)
				m_spd.m_y = 0.0f;
		}
	}

	//control max speed
	if (m_spd.m_x > m_maxSpd)
		m_spd.m_x = (float)m_maxSpd;
	else if (m_spd.m_x < -m_maxSpd)
		m_spd.m_x = (float)-m_maxSpd;
	if (m_spd.m_y > m_maxSpd)
		m_spd.m_y = (float)m_maxSpd;
	else if (m_spd.m_y < -m_maxSpd)
		m_spd.m_y = (float)-m_maxSpd;

	//rotation controls
	//face mouse = atan (dy/dx)
	//	be sure to not divide by 0
	//	check for quadrant
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(g_app.GetWindowHandle(), &cursorPos);
	
	float dy = (m_pos.m_y + m_height/2.0f) - cursorPos.y;
	float dx = (m_pos.m_x + m_width/2.0f) - cursorPos.x;
	float arcTan;

	//catch case so you don't divide by 0 for atan()
	if (dx == 0)
	{
		if (dy > 0)
			m_rotate = -PLAYER_ROT_ADJUST * DEG_TO_RAD;
		else
			m_rotate = PLAYER_ROT_ADJUST * DEG_TO_RAD;
	}
	else
	{
		arcTan = atan(dy / dx);
		if (dx > 0)
			arcTan += D3DX_PI;
		m_rotate = arcTan;
	}
	m_rotate += D3DX_PI/2;
	

	// Compute new screen position
	m_pos.m_x += m_spd.m_x * dt;
	m_pos.m_y += m_spd.m_y * dt;

	//collision with border correction
	int windowWidth = g_app.GetWindowWidth();
	int windowHeight = g_app.GetWindowHeight();
	if(m_pos.m_x > windowWidth - m_width)
	{
		m_spd.m_x = 0.0f;
		m_pos.m_x = (float)windowWidth - m_width;
	}
	else if(m_pos.m_x < 0)
	{
		m_spd.m_x = 0.0f;
		m_pos.m_x = 0;
	}

	if(m_pos.m_y < 0.0f)
	{
		m_spd.m_y = 0.0f;
		m_pos.m_y = 0.0f;
	}
	else if(m_pos.m_y > windowHeight - m_height)
	{
		m_spd.m_y = 0.0f;
		m_pos.m_y = (float)windowHeight - m_height;
	}

	//fire bullet check once you are repositioned
	if (shoot)
	{
		// Increment time since last shot 
		m_timeSinceLastShot += dt;
		if (m_timeSinceLastShot >= m_shotDelay)
		{
			sVector2d spawnPoint = sVector2d(m_pos.m_x + (m_width /2.0f), 
				                             m_pos.m_y + (m_height /2.0f));
			g_entities.AddToList(PLAYER_BULLET, spawnPoint, m_rotate, PICK_ALLY_LIST);
			if (DEBUG_MODE)
			{
				debug << "SPAWNING SHOT, SHIP POS: (" << m_pos.m_x << ", " << m_pos.m_y << ")\n";
				debug << "SPAWNING SHOT, SHIP DIMENSION: (" << m_width << ", " << m_height << ")\n";
			}
			m_timeSinceLastShot = 0.0f;
		}
	}
}

/////////////////////////////////////////////////
bool cAvatar::OnCollision(const string& baseID)
{
	if (DEBUG_MODE)
		debug << "AVATAR ON_COLLISION\n";

	if (baseID == ENEMY_ONE)
	{
		g_scores.SaveNewHighScore();
		g_scores.ResetScore();
		g_entities.KillPlayer();
		if (DEBUG_MODE)
			debug << "ALL ENTITIES KILLED, PLAYER RESPAWNED" << endl;
		g_game.m_gameTime.setGameTime();
		return IS_KILLED;
	}
	
//	m_color = 0xffff0000;
	return IS_NOT_KILLED;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//////////////BASIC ENEMY ENTITIY////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
cBasicEnemy::cBasicEnemy ()
{
	m_rotate = 0.0f;
}

/////////////////////////////////////////////////
bool cBasicEnemy::Init (sEntitySpawnStats* myStruct, 
						sVector2d parentPos, 
						float parentRot)
{
	

	if (!cEntity::Init(myStruct))
	{
		ErrorMsg("cAvatar::Init call");
		return false;
	}


	//set spawn position along a random wall, spaced away from it by set amount
	RECT client;
	GetClientRect(g_app.GetWindowHandle(), &client);

	int wallToSpawnAt = rand() % 4;
	if (wallToSpawnAt == TOP_WALL)
	{
		m_pos.m_x = (float)(rand() % ( (int)( client.right - client.left - 
									    (SPAWN_DIST_FROM_WALL * 2) ))); //from 0 to width minus 2x spawn dist
		m_pos.m_x += SPAWN_DIST_FROM_WALL; //center spawn dist
		m_pos.m_y = (float)client.top + SPAWN_DIST_FROM_WALL;
	}
	else if (wallToSpawnAt == BOTTOM_WALL)
	{
		m_pos.m_x = (float)(rand() % ( (int)( client.right - client.left - 
									    (SPAWN_DIST_FROM_WALL * 2) ))); //from 0 to width minus 2x spawn dist
		m_pos.m_x += SPAWN_DIST_FROM_WALL; //center spawn dist
		m_pos.m_y = (float)client.bottom - m_height - SPAWN_DIST_FROM_WALL;
	}
	else if (wallToSpawnAt == LEFT_WALL)
	{
		m_pos.m_x = (float)client.left + SPAWN_DIST_FROM_WALL;
		m_pos.m_y = (float)(rand() % ( (int)( client.bottom - client.top - 
									    (SPAWN_DIST_FROM_WALL * 2) ))); //from 0 to width minus 2x spawn dist
		m_pos.m_y += SPAWN_DIST_FROM_WALL; //center spawn dist
	}
	else if (wallToSpawnAt == RIGHT_WALL)
	{
		m_pos.m_x = (float)client.right - m_width - SPAWN_DIST_FROM_WALL;
		m_pos.m_y = (float)(rand() % ( (int)( client.bottom - client.top - 
									    (SPAWN_DIST_FROM_WALL * 2) ))); //from 0 to width minus 2x spawn dist
		m_pos.m_y += SPAWN_DIST_FROM_WALL; //center spawn dist
	}

/*	// Current screen position
	
	int spawnLoc = rand() % NUM_OF_SPAWNERS;
	if (spawnLoc == 0)
		m_pos = SPAWN_ONE_POS;
	else
		m_pos = SPAWN_TWO_POS;
	
	m_pos = SPAWN_LOCATIONS[spawnLoc];  */

	do
	{
		m_spd.m_x = (float)(rand() % (m_maxSpd + 1));
		m_spd.m_y = (float)(rand() % (m_maxSpd + 1));
	} while(m_spd.m_x < m_minSpd && m_spd.m_y < m_minSpd);
	
	//direction of movement
	int direction = rand() % 4;
	if (direction == 1 || direction == 3)
		m_spd.m_x *= -1;
	if (direction == 3 || direction == 4)
		m_spd.m_y *= -1;

	return true;
}

/////////////////////////////////////////////////
void cBasicEnemy::Update (float dt)
{
	// Compute new screen position
	m_pos.m_x += m_spd.m_x * dt;
	m_pos.m_y += m_spd.m_y * dt;
	m_rotate -= ASTEROID_ROT_SPD * dt;

	//collision with border correction
	//bounces off edges for now
	float windowHeight = (float)g_app.GetWindowHeight();
	float windowWidth = (float)g_app.GetWindowWidth();
	if(m_pos.m_x > windowWidth - m_width)
	{
		m_spd.m_x *= -1;
		m_pos.m_x = windowWidth - m_width;
	}
	else if(m_pos.m_x < 0)
	{
		m_spd.m_x *= -1;
		m_pos.m_x = 0;
	}

	if(m_pos.m_y < 0.0f)
	{
		m_spd.m_y *= -1;
		m_pos.m_y = 0.0f;
	}
	else if(m_pos.m_y > windowHeight - m_height)
	{
		m_spd.m_y *= -1;
		m_pos.m_y = windowHeight - m_height;
	}


}

/////////////////////////////////////////////////
bool cBasicEnemy::OnCollision(const string& baseID)
{
	if (DEBUG_MODE)
	{
		debug << "BASIC ENEMY ON_COLLISION\n";
		debug << "\tID passed: " << baseID << endl;
	}
	if (baseID == PLAYER_BULLET)
	{
		//add to score
		g_scores.AddToScore(100);
		//create explosion
		sVector2d spawnPoint = sVector2d(m_pos.m_x + (m_width /2.0f), 
			                             m_pos.m_y + (m_height /2.0f));
		for (int i = 0; i < EXPLOSIONS_PER_KILL; i++)
			g_entities.AddToList(EXPLOSION, spawnPoint, 0, PICK_NEUTRAL_LIST);
		
		//delete entity
		g_entities.RemoveFromList(this, PICK_ENEMY_LIST);
		return IS_KILLED;
	}

//	m_color = 0xff00ff00;
	return IS_NOT_KILLED;
}

/////////////////////////////////////////////////
/////////////////////////////////////////////////
//////////////PLAYER BULLET ENTITIY//////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

cPlayerBullet::cPlayerBullet ()
{
	m_rotate = 0.0f;
}

/////////////////////////////////////////////////
bool cPlayerBullet::Init(sEntitySpawnStats* myStruct, 
						 sVector2d parentPos, 
						 float parentRot)
{
	if (!cEntity::Init(myStruct))
	{
		ErrorMsg("cPlayerBullet::Init call");
		return false;
	}

	//set bullet pos and rot to match avatar
	m_pos = parentPos - sVector2d(m_width/2.0f, m_height/2.0f);
	m_rotate = parentRot - (PLAYER_ROT_ADJUST * DEG_TO_RAD);

	//set speed in init since it is unchanging until collision
	m_spd.m_x = cos(m_rotate) * m_maxSpd;
	m_spd.m_y = sin(m_rotate) * m_maxSpd;

	if (DEBUG_MODE)
	{
		debug << "SPAWNING SHOT, BULLET POS: (" << m_pos.m_x << ", " << m_pos.m_y << ")\n";
		debug << "SPAWNING SHOT, BULLET DIMENSION: (" << m_width << ", " << m_height << ")\n";
	}

	return true;
}

/////////////////////////////////////////////////
void cPlayerBullet::Update (float dt)
{
	// Compute new screen position
	m_pos.m_x += m_spd.m_x * dt;
	m_pos.m_y += m_spd.m_y * dt;

	//collision with border correction
	//delete if hit edge
	float windowHeight = (float)g_app.GetWindowHeight();
	float windowWidth = (float)g_app.GetWindowWidth();
	if (m_pos.m_x > windowWidth - m_width ||
		m_pos.m_x < 0 ||
		m_pos.m_y < 0.0f ||
		m_pos.m_y > windowHeight - m_height)
	{
		g_entities.RemoveFromList(this, PICK_ALLY_LIST);
	}
}

/////////////////////////////////////////////////
bool cPlayerBullet::OnCollision(const string& baseID)
{
	if (DEBUG_MODE)
		debug << "BULLET ON_COLLISION\n";
	g_entities.RemoveFromList(this, PICK_ALLY_LIST);
	return IS_KILLED;
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
//////////////////Wall ENTITIY///////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////

cWall::cWall ()
{
	m_rotate = 0.0f;
}

/////////////////////////////////////////////////
bool cWall::Init(sEntitySpawnStats* myStruct, 
							  sVector2d parentPos, 
							  float parentRot)
{
	if (!cEntity::Init(myStruct))
	{
		ErrorMsg("cWall::Init call");
		return false;
	}

	//use parent rot to determine which side the wall is on
	if (parentRot == TOP_WALL || parentRot == BOTTOM_WALL)
	{
		m_scl.m_x = (float)g_app.GetWindowWidth() / m_width;
		m_pos.m_x = g_app.GetWindowWidth() / 2.0f;
		if (parentRot == TOP_WALL)
			m_pos.m_y = 0.0f;
		else
			m_pos.m_y = (float)g_app.GetWindowHeight() - m_height;
	}
	else if (parentRot == LEFT_WALL || parentRot == RIGHT_WALL)
	{
		m_scl.m_y = (float)g_app.GetWindowHeight() / m_height;
		m_pos.m_y = g_app.GetWindowHeight() / 2.0f;
		if (parentRot == LEFT_WALL)
			m_pos.m_x = 0.0f;
		else
			m_pos.m_x = (float)g_app.GetWindowWidth() - m_width;
	}

	return true;
}


/////////////////////////////////////////////////
/////////////////////////////////////////////////
///////////////Explosion ENTITIY/////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
bool cExplosion::Init(sEntitySpawnStats* myStruct, 
					  sVector2d parentPos, 
					  float parentRot)
{
	if (!cEntity::Init(myStruct))
	{
		ErrorMsg("cWall::Init call");
		return false;
	}

	//set explosion pos and rot to match dead enemy
	m_pos = parentPos - sVector2d(m_width/2.0f, m_height/2.0f);
	m_rotate = parentRot;

	//calc max scale for variance, change current scale to 0.0
	do
	{
		m_maxScl.m_x = (float)(rand() % 1000 * (m_scl.m_x + 1));
		m_maxScl.m_y = (float)(rand() % 1000 * (m_scl.m_x + 1));
		m_maxScl /= 1000.0f;
	} while(m_maxScl.m_x < m_scl.m_y || m_maxScl.m_y < m_scl.m_y);
	m_scl.m_x = m_scl.m_y = 0.1f;

	//set random color from 0 to 255, be sure at least one is above 100
	opacity = 255;
	do
	{
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
	} while(r < 100 && g < 100 && b < 100);

	//set random rotation
	m_rotate = (rand() % 360) * DEG_TO_RAD;

	m_color = D3DCOLOR_ARGB(opacity, r, g, b);
	m_frameDelay = FRAME_DELAY;
	m_timeSinceLastFrame = 0.0f;

	return true;
}

/////////////////////////////////////////////////
void cExplosion::Update (float dt)
{
	// Increment time since last tile change 
	m_timeSinceLastFrame += dt;

	// if time since last tile change exceeds the 
	// time allotted for each tile...
	if (m_timeSinceLastFrame > m_frameDelay)
	{
		if (DEBUG_MODE)
			debug << "\tscale update. CURRENT: (" << m_scl.m_x 
				  << ", " << m_scl.m_y << ")" << endl;

		m_scl += m_maxScl * m_frameDelay;
		m_timeSinceLastFrame -= m_frameDelay;
		//fade explosions out
		if (m_scl.m_x >= m_maxScl.m_x/2)
		{
			//check early death for each explosion
			if (rand() % 100 < EARLY_DEATH_PCT)
			{
				g_entities.RemoveFromList(this, PICK_NEUTRAL_LIST);
				return;
			}
			opacity -= EXPLOSION_DEGRADE_RATE;

			//move colors towards white as explosion dies
			if (r < 255 - EXPLOSION_DEGRADE_RATE)
				r += EXPLOSION_DEGRADE_RATE;
			if (g < 255 - EXPLOSION_DEGRADE_RATE)
				g += EXPLOSION_DEGRADE_RATE;
			if (b < 255 - EXPLOSION_DEGRADE_RATE)
				b += EXPLOSION_DEGRADE_RATE;
		}
		//set new color
		m_color = (DWORD)D3DCOLOR_ARGB(opacity, r, g, b);

		if (DEBUG_MODE)
			debug << "\tscale update. NEW: (" << m_scl.m_x 
				  << ", " << m_scl.m_y << ")" << endl;
	}

	if (m_scl.m_x > m_maxScl.m_x)
		g_entities.RemoveFromList(this, PICK_NEUTRAL_LIST);
}