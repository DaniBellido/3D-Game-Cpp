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


typedef struct
{
   float x;
   float y;
   float z;
   float w;
} vector4f;


class CLight
{
	private:

        vector4f Ambient, Diffuse, Direction;
        int Light;

	public:

	CLight()
	{
       Light=0;
	   Ambient.x=Ambient.y=Ambient.z=0.7f; Ambient.w=1.0f; //0.7
       Diffuse.x=Diffuse.y=Diffuse.z=0.8f; Diffuse.w=1.0f; //1.0
	   // 2.0, 5.0, 1.0
	   Direction.x=1.5f; Direction.y=2.0f; Direction.z=1.0f; Direction.w=0;
	}

        void SetAmbientLight( float r, float g, float b) {  Ambient.x=r; Ambient.y=g; Ambient.z=b; Ambient.w=1.0f; }

        void SetDiffuseLight( float r, float g, float b) {  Diffuse.x=r; Diffuse.y=g; Diffuse.z=b; Diffuse.w=1.0f; }

        void SetLightDirection( float x, float y, float z) { Direction.x=x; Direction.y=y; Direction.z=z; Direction.w=0; }

		void SetDefaults()
		{
			Ambient.x = Ambient.y = Ambient.z = 0.65f; Ambient.w = 1.0f;
			Diffuse.x = Diffuse.y = Diffuse.z = 1.0f; Diffuse.w = 1.0f;
			Direction.x = 2.0f; Direction.y = 5.0f; Direction.z = 1.0f; Direction.w = 0;
			Apply();
		}

        void SelectLight( int light=0)
		{
            if (light > 2 || light < 0) { cout << "Three lights max" << endl; return; }
            Light=light;
		}

        void Enable()
		{
            glEnable(GL_LIGHTING);
           
            // normalize normal vectors
			glEnable(GL_NORMALIZE);
    
			// enable color tracking
			glEnable(GL_COLOR_MATERIAL);
			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

			// smooth shading
            glShadeModel(GL_SMOOTH); 

			// Apply the lights
            Apply();

		}
		
		// apply the light
		void Apply()
		{
	     if (Light==0)
	    {
 	      glEnable(GL_LIGHT0); 
             
          glLightfv(GL_LIGHT0, GL_AMBIENT, (float*)&Ambient);  // add lighting. (ambient)
	      glLightfv(GL_LIGHT0, GL_DIFFUSE, (float*)&Diffuse);  // add lighting. (diffuse).
	      glLightfv(GL_LIGHT0, GL_POSITION,(float*)&Direction); // set light position.
            }
 	    else if (Light==1)
	    {
 	      glEnable(GL_LIGHT1); 
          glLightfv(GL_LIGHT1, GL_AMBIENT, (float*)&Ambient);  // add lighting. (ambient)
	      glLightfv(GL_LIGHT1, GL_DIFFUSE, (float*)&Diffuse);  // add lighting. (diffuse).
	      glLightfv(GL_LIGHT1, GL_POSITION,(float*)&Direction); // set light position.
            }
	    else if (Light==2)
	    {
 	      glEnable(GL_LIGHT2); 
          glLightfv(GL_LIGHT2, GL_AMBIENT, (float*)&Ambient);  // add lighting. (ambient)
	      glLightfv(GL_LIGHT2, GL_DIFFUSE, (float*)&Diffuse);  // add lighting. (diffuse).
	      glLightfv(GL_LIGHT2, GL_POSITION,(float*)&Direction); // set light position.
            }
			
			
		}

        void Disable() 
        {
            if (Light==0) glDisable(GL_LIGHT0); 
            if (Light==1) glDisable(GL_LIGHT1); 
			if (Light==2) glDisable(GL_LIGHT2); 

	} 



};
