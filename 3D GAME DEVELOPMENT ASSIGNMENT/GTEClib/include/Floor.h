/*********************************************************************
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
*************************************************************************/
#pragma once

#include "GTEC.h"
#include "Model.h"


using namespace std;

class CFloor
{
public:
	
	// ----- Constructors & Destructor -----
	
	CFloor() 
	{ 
		Width=Depth=0; 
		width = height = 0;
		Position.x=Position.y=Position.z=0; 
		tiling=false; 
		drawgrid=false; 
		Color.Set(0, 0.5f, 0, 1.0);
		TextureID = 0;
	}
	
	~CFloor() 
	{ 
		glDeleteTextures(1, &TextureID);
	}

	// ----- Attributes ------

private:
	
	// Position & Size
	float  Width, Depth; // width, depth
	int width, height;   // texture size
	CVector Position;    // centre position
	GLuint TextureID;  		// texture, zero if no texture assigned
	CColor Color;			// colour
	bool tiling;			// should the texture be tiled?
	bool drawgrid;
	
public:
	

	// Size of the Texture
	float GetWidth()			    { return Width; }
	float GetDepth()		        { return Depth; }
    float GetHeight( float x, float z)	    { return 0; }
	// multiple of hundreds
	void SetSize(float x, float z)         { Width=x; Depth=z; }
	void SetPosition( float x, float z)    { Position.x=x; Position.z=z; }
	void SetPosition( float x, float y, float z)    { Position.x=x; Position.y=y; Position.z=z; }
	void SetPositionV(CVector pos) { Position=pos; }
	void Move( float dx, float dy, float dz) { Position.x+=dx; Position.y+=dy; Position.z+=dz; }
	
	
	
	void LoadTexture(string filename);
	
	void SetColor( const CColor& c) { Color.Set( c);  }
	void SetTiling( bool tile) { tiling=tile;}
	void ShowGrid(bool grid=true) { drawgrid=grid; }
	
    void Draw(CGraphics* g);
    


};



