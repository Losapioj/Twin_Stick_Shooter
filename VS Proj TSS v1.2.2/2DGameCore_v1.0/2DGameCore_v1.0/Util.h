#ifndef __UTIL_H__
#define __UTIL_H__

//#include <iostream>
//#include <cmath>
//using std::ostream;
//using std::istream;

/////////////////////////////////////////////////
const float PI = 3.141592654f;
const float TWO_PI = 2 * PI;

/////////////////////////////////////////////////
struct sVector2d
{
	float m_x, m_y;

	sVector2d();
	sVector2d(float x, float y);
	sVector2d(const sVector2d& v);

	void Set(float x, float y);
	const sVector2d& operator=(const sVector2d& v);

	bool operator==(const sVector2d& v);
	bool operator!=(const sVector2d& v);

	sVector2d operator+(const sVector2d& v);
	sVector2d operator-(const sVector2d& v);
	const sVector2d& operator+=(const sVector2d& v);
	const sVector2d& operator-=(const sVector2d& v);

	sVector2d operator*(float f);
	sVector2d operator/(float f);
	const sVector2d& operator*=(float f);
	const sVector2d& operator/=(float f);
	float Dot(const sVector2d& v);
  
	float Length();
	float NormSquared();

	sVector2d Normalize();
};

sVector2d operator*(float f, const sVector2d& v);
sVector2d operator/(float f, const sVector2d& v);

/////////////////////////////////////////////////
struct sVector3d
{
	float m_x, m_y, m_z;

	sVector3d ();
	sVector3d (float x, float y, float z);
	sVector3d (const sVector3d& v);

	void Set (float x, float y, float z);
	const sVector3d& operator= (const sVector3d& v);

	bool operator== (const sVector3d& v);
	bool operator!= (const sVector3d& v);

	sVector3d operator+ (const sVector3d& v);
	sVector3d operator- (const sVector3d& v);
	const sVector3d& operator+= (const sVector3d& v);
	const sVector3d& operator-= (const sVector3d& v);

	sVector3d operator* (float f);
	sVector3d operator/ (float f);
	const sVector3d& operator*= (float f);
	const sVector3d& operator/= (float f);
	float Dot (const sVector3d& v);
	sVector3d Cross (const sVector3d& v);
  
	float Norm ();
	float NormSquared ();

	sVector3d Normalize ();

};

sVector3d operator* (float f, const sVector3d& v);

/////////////////////////////////////////////////
float CalcAngle (float x, float y);

/////////////////////////////////////////////////
#endif // __UTIL_H__