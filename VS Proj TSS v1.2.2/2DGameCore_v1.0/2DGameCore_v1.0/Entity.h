// Entity.h - class cEntity definition
#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Util.h"
#include "EntityMngr.h"

enum CollisionType {AABOX, CIRCLE};

/////////////////////////////////////////////////
class cEntity
{
protected:
	// Entity ID for map
	string m_id;
	// Entity ID for collision detection
	string m_baseID;

	// Current screen position and rotation
	sVector2d m_pos;  
	float m_rotate;

	// Speed (in 2 dimensions)
	sVector2d m_spd;
	int m_maxSpd, m_minSpd;

	// Scale (in 2 dimensions)
	sVector2d m_scl;

	// Dimensions of each tile
	int m_width, m_height;

	CollisionType m_collision;

	DWORD m_color;

	string m_imageId;

public:
	virtual ~cEntity()  {if (DEBUG_MODE) debug<<"ENTITY DESTRUCTOR"<< endl;}

	virtual bool Init(sEntitySpawnStats* myStruct, 
					  sVector2d parentPos = sVector2d(0, 0), 
					  float parentRot = 0) = 0;

	virtual void Update (float dt) = 0;

	virtual void Draw () = 0; 

	virtual bool OnCollision(const string& baseID) = 0;

	virtual void SetID (const string& id)
	{  m_id = id;  }
	virtual const string& GetID()
	{  return m_id;  }
	virtual const string& GetBaseID()
	{  return m_baseID;  }

	void SetPosition(const sVector2d& p)
	{  m_pos = p;  }
	void SetPositionX(float x)
	{  m_pos.m_x = x;  }
	void SetPositionY(float y)
	{  m_pos.m_y = y;  }

	const sVector2d& GetPosition()
	{  return m_pos;  }
	int GetWidth()
	{  return m_width;  }
	int GetHeight()
	{  return m_height;  }
	float GetRotation()
	{  return m_rotate;  }

	void SetSpeed(const sVector2d& s)
	{  m_spd = s;  }
	void SetSpeedX(float x)
	{  m_spd.m_x = x;  }
	void SetSpeedY(float y)
	{  m_spd.m_y = y;  }
	const sVector2d& GetSpeed()
	{  return m_spd;  }

	void SetScale(const sVector2d& s)
	{  m_scl = s;  }
	void SetScaleX(float x)
	{  m_scl.m_x = x;  }
	void SetScaleY(float y)
	{  m_scl.m_y = y;  }
	const sVector2d& GetScale()
	{  return m_scl;  }

	CollisionType GetCollisionType()
	{	 return m_collision;	}

};

/////////////////////////////////////////////////
class cAvatar : public cEntity
{
protected:
	float m_shotDelay;
	float m_timeSinceLastShot;

public:
	cAvatar ();
	virtual ~cAvatar () {if (DEBUG_MODE) debug<<"AVATAR DESTRUCTOR"<< endl;}
	// Initialization from data file (parameter)
	virtual bool Init(sEntitySpawnStats* myStruct, 
					  sVector2d parentPos = sVector2d(0, 0), 
					  float parentRot = 0);

	// Computes new position & tile for next frame
	virtual void Update (float dt);

	// Draws current tile 
	virtual void Draw ()
	{  cEntity::Draw();  }

	virtual bool OnCollision(const string& baseID);
};

/////////////////////////////////////////////////
class cBasicEnemy : public cEntity
{
protected:

public:
	cBasicEnemy ();
	virtual ~cBasicEnemy () {}

	virtual bool Init(sEntitySpawnStats* myStruct, 
					  sVector2d parentPos = sVector2d(0, 0), 
					  float parentRot = 0);

	virtual void Update (float dt);
	virtual void Draw ()
	{  cEntity::Draw();  }
	virtual bool OnCollision(const string& baseID);
};

/////////////////////////////////////////////////
class cPlayerBullet : public cEntity
{
protected:

public:
	cPlayerBullet ();
	virtual ~cPlayerBullet () {}

	//spawnBullet called when an object shoots a bullet.
	virtual bool Init(sEntitySpawnStats* myStruct, 
					  sVector2d parentPos = sVector2d(0, 0), 
					  float parentRot = 0);

	virtual void Update (float dt);
	virtual void Draw ()
	{  cEntity::Draw();  }
	virtual bool OnCollision(const string& baseID);
};

/////////////////////////////////////////////////
class cWall : public cEntity
{
protected:

public:
	cWall ();
	virtual ~cWall () {if (DEBUG_MODE) debug<<"WALL DESTRUCTOR"<< endl;}
	// Initialization from data file (parameter)
	virtual bool Init(sEntitySpawnStats* myStruct, 
							  sVector2d parentPos = sVector2d(0, 0), 
							  float parentRot = 0);

	// Computes new position & tile for next frame
	virtual void Update (float dt) {}

	// Draws current tile 
	virtual void Draw ()
	{  cEntity::Draw();  }

	virtual bool OnCollision(const string& baseID) { return IS_NOT_KILLED;}
};

/////////////////////////////////////////////////
class cExplosion : public cEntity
{
protected:
	float m_frameDelay;
	float m_timeSinceLastFrame;
	sVector2d m_maxScl;

	int r, g, b, opacity;

public:
	cExplosion () {}
	virtual ~cExplosion () {if (DEBUG_MODE) debug<<"EXPLOSION DESTRUCTOR"<< endl;}
	// Initialization from data file (parameter)
	virtual bool Init(sEntitySpawnStats* myStruct, 
							  sVector2d parentPos = sVector2d(0, 0), 
							  float parentRot = 0);

	// Computes new position & tile for next frame
	virtual void Update (float dt);

	// Draws current tile 
	virtual void Draw ()
	{  cEntity::Draw();  }

	virtual bool OnCollision(const string& baseID) { return IS_NOT_KILLED;}
};
/////////////////////////////////////////////////
#endif // __ENTITY_H__