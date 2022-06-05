/****************************************************************************
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

#include "GTEC.h"
#include "Color.h"

using namespace std;



class CTexture
{
public:
	
	// ----- Constructors & Destructor -----
	
	CTexture() { Width=Height=Xpos=Ypos=0; TextureID=0; Frames=frame=1; iscloned=false;}
	
	~CTexture() { if (!iscloned) glDeleteTextures( 1, &TextureID); }
	
	static bool smooth;

	// ----- Attributes ------

private:
	
	// Position & Size
	int Xpos, Ypos;			// position of the texture
	int Width, Height;		// width and height of texture in pixels
    GLuint TextureID;		// OpenGL texture ID
	int frame, Frames;		// current frame, total number of frames
	CColor TranspColor;		// transparent colour, only used when creating texture
	
	bool iscloned;

	
public:
	

	// Size of the Texture
	int GetWidth()			    	    { return Width; }
	int GetHeight()			    		{ return Height; }
	void SetSize(int w, int h)         { Width=w; Height=h; }
	void SetPosition(int x, int y)		{ Xpos=x; Ypos=y; }

		
	int GetFrames()						{ return Frames; }
	void SetFrame (int f) 				{ frame=f; }
	void NextFrame()					{ frame++; if (frame > Frames) frame=1; }

	GLuint GetTextureID()				{ return TextureID; }
	void SetTextureID( GLuint tid)		{ TextureID=tid; iscloned=true;} 
	
    bool LoadTexture( string filename, const CColor& color, int frames=1);
    bool LoadTexture( string filename, int frames=1) { return LoadTexture( filename, CColor::None(), frames); } 
    
	void Draw(int x, int y);
	void Draw() { Draw( Xpos, Ypos); }
	
	bool HitTest(int x, int y);

	

private:

	unsigned char* LoadFromBMP(const char* filename);
	unsigned char* LoadFromTGA(const char* filename);
	
    int readWord(FILE* in, int len);

        

	
};



