/**************************************************************************
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
***************************************************************************/
#pragma once

#include "Sprite.h"

class CSpritePoint : public CSprite
{
public:
	// Constructor
	CSpritePoint() : CSprite() { Radius = 0.0f; }
	
	float Radius;
	
	CSpritePoint(float x, float y, float r=3.0f) : CSprite()
    {
	  Xpos=x; Ypos=y;
	  Radius = r;
      Color.Set( CColor::Yellow());    
    }
	virtual ~CSpritePoint() {}

	virtual void Draw(CGraphics* g)
    {
       glColor4f( Color.R, Color.G, Color.B, Color.A);
	   glPointSize( float(2*Radius)); 

		glBegin(GL_POINTS);
	  		glVertex3f( Xpos, Ypos, 0);
		glEnd();
  	}
	
	
};
