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
#include "Model.h"


class CPoint3 : public CModel
{
	public:
	float Size;
	
	CPoint3(float x, float y, float z, float size=3) : CModel()
	{
		Position.x=x; Position.y=y; Position.z=z;
		Size=size; Color.Set( 1.0, 1.0, 0.0, 1.0); // yellow	
	}


	
	CPoint3(vector3f& pos, float size=3) : CModel()
	{
		Position.x=pos.x; Position.y=pos.y; Position.z=pos.z;
		Size=size; Color.Set( 1.0, 1.0, 0.0, 1.0); // yellow	
		
		//cout << pos.x << " " << pos.y << " " << pos.z << endl;
	}

	virtual void Draw(CGraphics* g)
	{
	   glColor4f( Color.R, Color.G, Color.B, Color.A);
	   glPointSize( Size); 

		glBegin(GL_POINTS);
	  		glVertex3fv( (float*)&Position );
		glEnd();
  	}
};
