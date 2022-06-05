/*********************************************************************************
GTEC - Games Technology Game Engine
Copyright (C) 2012-2020  Andreas Hoppe

This program is free software; you can redistribute it and /or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; If not, see < http://www.gnu.org/licenses/>.
*********************************************************************************/
#pragma once
#include"GTEC.h"


// a single vertex/vector structure
typedef struct
{
   float x;
   float y;
   float z;
} vector3f;

typedef struct
{
	float x;
	float y;
} vector2f;

// vector normalisation
void normalizeVector3f(vector3f* v);


class CVector  
{
public:
	float x;
	float y;
	float z;

	// Constructor
	CVector(float x, float y, float z)	  { (*this).x=x; (*this).y=y; (*this).z=z; }
	CVector(const CVector& v)				  { (*this).x=v.x; (*this).y=v.y; (*this).z=v.z; }
	CVector()								  { x=0; y=0; z=0; }

	// Destructor
	~CVector()	{ }

	// Getters & Setters
	float GetX() const					{ return x; }
	float GetY() const					{ return y; }
	float GetZ() const					{ return z; }
	
	void SetX(float x)					{ (*this).x = x; }
	void SetY(float y)					{ (*this).y = y; }
	void SetZ(float z)					{ (*this).z = z; }
	
	void Set(float x, float y, float z)	{ (*this).x=x; (*this).y=y; (*this).z=z;  }
	void Set(const CVector& v)			{ (*this).x=v.x; (*this).y=v.y; (*this).z=v.z; }
	
	// Distance to vector v
	float Distance(const CVector& v)
	{
		float dx = x - v.x, dy = y - v.y, dz = z - v.z;
		return (float)sqrt(dx*dx + dy*dy + dz*dz);
	}
	
	// Distance between vector v1 and vector v2
	static float Distance(const CVector& v1, const CVector& v2)
	{
	    float dx = v1.x - v2.x, dy = v1.y - v2.y ,dz = v1.z - v2.z;
		return (float)sqrt(dx*dx + dy*dy + dz*dz);
	}
	
	// Distance to vector v in the XZ plane
	float DistanceXZ(const CVector& v)
	{
		float dx = x - v.x, dz = z - v.z;
		return (float)sqrt(dx*dx + dz*dz);
	}
	
	// Distance between vector v1 and vector v2 in the XZ plane
	static float DistanceXZ(const CVector& v1, const CVector& v2)
	{
	    float dx = v1.x - v2.x, dz = v1.z - v2.z;
		return (float)sqrt(dx*dx + dz*dz);
	}
	
	// Magnitude of this vector
	float Magnitude()
	{
	  return (float)sqrt(x*x+y*y+z*z);
	}
	
	// Length of this vector
	float Length() { return Magnitude(); }
	
	// Normalise this vector
	void Normalize()
	{
	   float mag=Magnitude();
	
		if (mag != 0.0f)
		{
		  x = x / mag;
		  y = y / mag;
		  z = z / mag;
		}
	}
	
	// returns normalized version of this vector
	CVector Normalized()
	{
	   CVector v;
	   float mag=Magnitude();
	
		if (mag != 0.0f)
		{
		  v.x = x / mag;
		  v.y = y / mag;
		  v.z = z / mag;
		}
		return v;
	}
	
	// Dot-product between this vector and vector v
	float Dot(const CVector& v)
	{
		return x*v.x + y*v.y + z*v.z;
	}
	
	// Dot-product between vector v1 and vector v2
	static float Dot(const CVector& v1, const CVector& v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}
	
	// Cross-product between this vector and vector v
	CVector Cross(const CVector& v)
	{
		CVector r;
		r.x = y*v.z - z*v.y;
		r.y = z*v.x - x*v.z;
		r.z = x*v.y - y*v.x;
		
		return r;	
	}
	
	// Cross-product between vector v1 and vector v2
	static CVector Cross(const CVector& v1, const CVector& v2)
	{
		CVector r;
		r.x = v1.y*v2.z - v1.z*v2.y;
		r.y = v1.z*v2.x - v1.x*v2.z;
		r.z = v1.x*v2.y - v1.y*v2.x;
		
		return r;	
	}
	
	void Print() 
	{
		cout << "x = " << x << " , y = " << y << " , z = " << z << endl;
	}
	

    // -----------  Operators ---------------	

	// assignment operators
	CVector& operator =(const CVector& v)	{  x = v.x; y = v.y; z = v.z; return(*this); }
	CVector& operator +=(const CVector& v)	{ x+=v.x; y+=v.y; z+=v.z; return (*this); }
	CVector& operator -=(const CVector& v)	{ x-=v.x; y-=v.y; z-=v.z; return (*this); }
	CVector& operator *=(float scalar)	{ x*=scalar; y*=scalar; z*=scalar; return (*this); }
	CVector& operator /=(float scalar)	{ x/=scalar; y/=scalar; z/=scalar; return (*this); }

	// unary operators
	CVector operator -() const				{ return CVector(-x, -y, -z); } 
	CVector operator +() const				{ return CVector(x, y, z); } 

	// binary operators
	CVector operator +(const CVector& v)		{ CVector r = *this; r += v; return r; }
	CVector operator -(const CVector& v)		{ CVector r = *this; r -= v; return r; }
	CVector operator *(float scalar)		    { CVector r = *this; r *= scalar; return r; }
	CVector operator /(float scalar)	        { CVector r = *this; r /= scalar; return r; }

	// comparisons
	bool operator ==(const CVector& v)		{ return (x == v.x && y == v.y && z == v.z); }
	bool operator !=(const CVector& v)		{ return (x != v.x || y != v.y  || z != v.z); }
};
