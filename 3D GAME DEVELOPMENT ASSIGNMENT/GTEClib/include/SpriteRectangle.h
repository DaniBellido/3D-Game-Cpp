/***************************************************************************
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
*****************************************************************************/
#pragma once

#include "Sprite.h"

class CSpriteRectangle : public CSprite
{
public:
	
	// Constructors & Destructor
	CSpriteRectangle() : CSprite() {}
	
	
	CSpriteRectangle(float x, float y, float width, float height, bool fill=true) : CSprite()
    {
        Xpos=x; Ypos=y; Width=width; Height=height; Color.Set( CColor::Gray()); filled=fill;
    }


	CSpriteRectangle(float x, float y, float width, float height, const CColor& color, bool fill=true) : CSprite()
    {
        Xpos=x; Ypos=y; Width=width; Height=height; Color.Set( color); filled=fill;
    }
	
	virtual ~CSpriteRectangle() {}




    // draw the rectangle as an OpenGL Quad 
	virtual void Draw(CGraphics* g)
    {
		
        if (!filled) glPolygonMode( GL_FRONT, GL_LINE);
        else glPolygonMode( GL_FRONT, GL_FILL);
		
    	glColor4f( Color.R, Color.G, Color.B, Color.A);
	
    	glPushMatrix();
		  glTranslatef( Xpos, Ypos, 0);  	// transformation to world coordinates
		  glRotatef( Rotation, 0, 0, 1 );   // rotation in degrees
		  glScalef(Width, Height, 0);       // scale box to size

		  glBegin(GL_QUADS);
		   glVertex3f(-0.5f, -0.5f, 0);
		   glVertex3f(0.5f, -0.5f, 0);
		   glVertex3f(0.5f, 0.5f, 0);
		   glVertex3f(-0.5f, 0.5f, 0);
		  glEnd();
		  
		glPopMatrix();
		glPolygonMode( GL_FRONT, GL_FILL);
		
		
		if (debug)
		{
			glColor3f(1.0f, 0, 0); // red

			glPolygonMode(GL_FRONT, GL_LINE);

			glPushMatrix();
			glTranslatef(Xpos, Ypos, 0);  	// transformation to world coordinates
			glRotatef(Rotation, 0, 0, 1);   // rotation in degrees
			glScalef(ScaleBB * Width, ScaleBB * Height, 0); // scale bounding box to size

			glBegin(GL_QUADS);
			 glVertex3f(-0.5f, -0.5f, 0);
			 glVertex3f(0.5f, -0.5f, 0);
			 glVertex3f(0.5f, 0.5f, 0);
			 glVertex3f(-0.5f, 0.5f, 0);
			glEnd();
			glPopMatrix();
			glPolygonMode(GL_FRONT, GL_FILL);
		}
	}
	
};
