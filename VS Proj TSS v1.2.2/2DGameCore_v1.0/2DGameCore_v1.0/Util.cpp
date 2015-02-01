#include "LibDec.h"

/////////////////////////////////////////////////
float CalcAngle (float x, float y)
{
	float speed = sqrt((x * x) + (y * y)),
		  angle = 0.0f;

	angle = (float)acos(y/speed);
	if ((float)asin(x/speed) <= 0.0f)
		angle = (D3DX_PI * 2 - angle);

	return angle;
}

/////////////////////////////////////////////////
sVector2d::sVector2d ()
{
	m_x = m_y = 0.0f;
}

/////////////////////////////////////////////////
sVector2d::sVector2d (float x, float y)
{
	m_x = x;
	m_y = y;
}

/////////////////////////////////////////////////
sVector2d::sVector2d (const sVector2d& v)
{
	m_x = v.m_x;
	m_y = v.m_y;
}

/////////////////////////////////////////////////
void sVector2d::Set (float x, float y)
{
	m_x = x;
	m_y = y;
}

/////////////////////////////////////////////////
const sVector2d& sVector2d::operator= (const sVector2d& v)
{
	m_x = v.m_x;
	m_y = v.m_y;
	return *this;
}

/////////////////////////////////////////////////
bool sVector2d::operator== (const sVector2d& v)
{
	return ((m_x == v.m_x) && (m_y == v.m_y));
}

/////////////////////////////////////////////////
bool sVector2d::operator!= (const sVector2d& v)
{
	return ((m_x != v.m_x) || (m_y != v.m_y));
}

/////////////////////////////////////////////////
sVector2d sVector2d::operator+ (const sVector2d& v)
{
	return sVector2d(m_x + v.m_x, m_y + v.m_y);
}

/////////////////////////////////////////////////
sVector2d sVector2d::operator- (const sVector2d& v)
{
	return sVector2d(m_x - v.m_x, m_y - v.m_y);
}

/////////////////////////////////////////////////
const sVector2d& sVector2d::operator+= (const sVector2d& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	return *this;
}

/////////////////////////////////////////////////
const sVector2d& sVector2d::operator-= (const sVector2d& v)
{
	m_x -= v.m_x;
	m_y -= v.m_y;
	return *this;
}

/////////////////////////////////////////////////
sVector2d sVector2d::operator* (float f)
{
	return sVector2d(m_x * f, m_y * f);
}

/////////////////////////////////////////////////
sVector2d sVector2d::operator/ (float f)
{
	return sVector2d(m_x / f, m_y / f);
}

/////////////////////////////////////////////////
const sVector2d& sVector2d::operator*= (float f)
{
	m_x *= f;
	m_y *= f;
	return *this;
}

/////////////////////////////////////////////////
const sVector2d& sVector2d::operator/= (float f)
{
	m_x /= f;
	m_y /= f;
	return *this;
}

/////////////////////////////////////////////////
float sVector2d::Dot (const sVector2d& v)
{
	return (m_x * v.m_x + m_y * v.m_y);
}

/////////////////////////////////////////////////
float sVector2d::Length ()
{
	return sqrt(m_x * m_x + m_y * m_y);
}

/////////////////////////////////////////////////
float sVector2d::NormSquared ()
{
	return m_x * m_x + m_y * m_y;
}

/////////////////////////////////////////////////
sVector2d sVector2d::Normalize ()
{
	sVector2d result;
	float len = Length ();
	if (len > 0.001)
	{
		result.m_x = m_x / len;
		result.m_y = m_y / len;
	}
	return result;
}

/////////////////////////////////////////////////
sVector2d operator* (float f, const sVector2d& v)
{
	return sVector2d(v.m_x * f, v.m_y * f);
}

/////////////////////////////////////////////////
sVector2d operator/ (float f, const sVector2d& v)
{
	return sVector2d(v.m_x / f, v.m_y / f);
}

/////////////////////////////////////////////////
sVector3d::sVector3d ()
{
	m_x = m_y = m_z = 0.0f;
}

/////////////////////////////////////////////////
sVector3d::sVector3d (float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

/////////////////////////////////////////////////
sVector3d::sVector3d (const sVector3d& v)
{
	m_x = v.m_x;
	m_y = v.m_y;
	m_z = v.m_z;
}

/////////////////////////////////////////////////
void sVector3d::Set (float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

/////////////////////////////////////////////////
const sVector3d& sVector3d::operator= (const sVector3d& v)
{
	m_x = v.m_x;
	m_y = v.m_y;
	m_z = v.m_z;
	return *this;
}

/////////////////////////////////////////////////
bool sVector3d::operator== (const sVector3d& v)
{
	return ((m_x == v.m_x) && (m_y == v.m_y) && (m_z == v.m_z));
}

/////////////////////////////////////////////////
bool sVector3d::operator!= (const sVector3d& v)
{
	return ((m_x != v.m_x) || (m_y != v.m_y) || (m_z != v.m_z));
}

/////////////////////////////////////////////////
sVector3d sVector3d::operator+ (const sVector3d& v)
{
	return sVector3d(m_x+v.m_x, m_y+v.m_y, m_z+v.m_z);
}

/////////////////////////////////////////////////
sVector3d sVector3d::operator- (const sVector3d& v)
{
	return sVector3d(m_x-v.m_x, m_y-v.m_y, m_z-v.m_z);
}

/////////////////////////////////////////////////
const sVector3d& sVector3d::operator+= (const sVector3d& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	m_z += v.m_z;
	return *this;
}

/////////////////////////////////////////////////
const sVector3d& sVector3d::operator-= (const sVector3d& v)
{
	m_x -= v.m_x;
	m_y -= v.m_y;
	m_z -= v.m_z;
	return *this;
}

/////////////////////////////////////////////////
sVector3d sVector3d::operator* (float f)
{
	return sVector3d(m_x * f, m_y * f,  m_z * f);
}

/////////////////////////////////////////////////
sVector3d sVector3d::operator/ (float f)
{
	return sVector3d(m_x / f, m_y / f,  m_z / f);
}

/////////////////////////////////////////////////
const sVector3d& sVector3d::operator*= (float f)
{
	m_x *= f;
	m_y *= f;
	m_z *= f;
	return *this;
}

/////////////////////////////////////////////////
const sVector3d& sVector3d::operator/= (float f)
{
	m_x /= f;
	m_y /= f;
	m_z /= f;
	return *this;
}

/////////////////////////////////////////////////
float sVector3d::Dot (const sVector3d& v)
{
	return m_x*v.m_x + m_y*v.m_y + m_z*v.m_z;
}

/////////////////////////////////////////////////
sVector3d sVector3d::Cross (const sVector3d& v)
{
	return sVector3d(m_y*v.m_z - m_z*v.m_y,
                     m_z*v.m_x - m_x*v.m_z,
                     m_x*v.m_y - m_y*v.m_x);
}

/////////////////////////////////////////////////
float sVector3d::Norm ()
{
	return sqrt(m_x*m_x+m_y*m_y+m_z*m_z);
}

/////////////////////////////////////////////////
float sVector3d::NormSquared ()
{
	return m_x*m_x + m_y*m_y + m_z*m_z;
}

/////////////////////////////////////////////////
sVector3d sVector3d::Normalize ()
{
	sVector3d result;
	float len = Norm ();
	if (len >= 0.001f)
	{
		result.m_x = m_x / len;
		result.m_y = m_y / len;
		result.m_z = m_z / len;
	}
	return result;
}

/////////////////////////////////////////////////
sVector3d operator* (float f, const sVector3d& v)
{
	return sVector3d(v.m_x * f, v.m_y * f,  v.m_z * f);
}

/////////////////////////////////////////////////
sVector3d operator/ (float f, const sVector3d& v)
{
	return sVector3d(v.m_x / f, v.m_y / f,  v.m_z / f);
}

/////////////////////////////////////////////////