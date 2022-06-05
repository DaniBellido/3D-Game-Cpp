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


class CLine : public CModel
{
	public:

    float Length; // length from starting point along the x-axis	
    float Width;
    
    CLine() : CModel() { Color.Set( CColor::Green()); Length=0; Width=2.0f; }
	
	CLine(float x, float y, float z, float length, const CColor& color=CColor::Green()) : CModel()
	{
		Position.x=x; Position.y=y; Position.z=z;
		
		Length=length;
		
		minx=0; maxx=length;
		miny=maxy=0;
		minz=maxz=0;
		
		Width=2.0f;
		
		Color.Set( color);
	}
	
	CLine(CVector pos, float length, const CColor& color=CColor::Green()) : CModel()
	{
		Position=pos;
		
		Length=length;
		
		minx=0; maxx=length;
		miny=maxy=0;
		minz=maxz=0;
		
		Width=2.0f;
		
		Color.Set( color);
	}
	
	CLine(CVector pos1, CVector pos2, const CColor& color=CColor::Green()) : CModel()
	{
		Position=pos1;
		
		CVector v=pos2-pos1;
		Length=v.Length();
		SetRotationV(v);
		
		minx=0; maxx=Length;
		miny=maxy=0;
		minz=maxz=0;
		
		Width=2.0f;
		
		Color.Set( color);
	}
	
	void SetPositionV(CVector pos1, CVector pos2) 
	{ 
		Position=pos1;
		CVector v=pos2-pos1;
		Length=v.Length();
		SetRotationV(v);
		
		minx=0; maxx=Length;
		miny=maxy=0;
		minz=maxz=0;
		
	}
	
	void SetWidth( float w) { Width=w; }
	
	void SetLength(float length) { Length=length; }
	
	virtual void Draw(CGraphics* g)
    {
       glColor4f( Color.R, Color.G, Color.B, Color.A);
	   glLineWidth( Width); 
       glPushMatrix();
		  glTranslatef( Position.x, Position.y, Position.z);  	// transformation to world coordinates
		  glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
		  glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
		  glRotatef( Rotation.z, 0, 0, 1 );
		  //glTranslatef(localPosition.x, localPosition.y, localPosition.z);
		  glScalef( Scale, Scale, Scale); 
		glBegin(GL_LINES);
	  		glVertex3f( 0, 0, 0);
	  		glVertex3f( Length, 0,0);
		glEnd();
	glPopMatrix();
  	}
  	

virtual CModel* Clone()
{
	// create new model
	CLine* m = new CLine();
	m->isCloned=true;
	m->Position=Position;
	m->Color.Set( Color);
	m->visible=visible;
	//m->localPosition=localPosition;
	m->Width=Width;
	m->Length=Length;
	
	if (childNode != NULL) m->childNode=childNode->Clone();
	
	return m;
}
  	
virtual bool HitTest(CModel *pModel)
{
   CVector v=GetRotationV();
   v.Normalized();
   
   // --- inside start or end point? -----
   if (pModel->HitTest( Position)) return true;
   if (pModel->HitTest( Position+v*Length)) return true;
   
   // check points along the line for every other units
   for (float n=0.0f; n < Length; n+=2.0f)
   {
	 if (pModel->HitTest( Position+(v*n))) return true;  
   }
   
   return false;
   
}
  	

};
