/*************************************************************************
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
**************************************************************************/
#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Texture.h"
#include "Model.h"

/* md2 vertex */
typedef struct
{
    unsigned char v[3];         // position in chars
    unsigned char normalIndex;  // normal vector index
} md2_vertex;

/* triangle */
typedef struct
{
    unsigned short vertex[3];   // vertex indices of the triangle
    unsigned short st[3];       // tex. coord. indices
} md2_triangle;

/* texture coords */
typedef struct
{
    short s;
    short t;
}  md2_texCoord;


/* model frame */
typedef struct
{
    vector3f scale;               // scaling factor
    vector3f translate;          // translation vector
    char name[16];                // frame name
    md2_vertex* verts;  // list of frame's vertices
} md2_frame;

/* md2 header structure */
typedef struct
{
    int ident;		    // identifies as MD2 file "IDP2"
    int version;	   
    int skinwidth;      // width of texture
    int skinheight;     // height of texture
    int framesize;      // number of bytes per frame
    int numSkins;       // number of textures
    int numVertices;    // number of vertex coordinates
    int numTex;         // number of texture coordinates
    int numTris;        // number of triangles
    int numGLcmds;      // number of OpenGL commands
    int numFrames;      // total number of frames
    int offsetSkins;    // offset to skin names (64 bytes each)
    int offsetST;       // offset of texture s-t values
    int offsetTris;     // offset of triangle mesh
    int offsetFrames;   // offset of frame data 
    int offsetGLcmds; 
    int offsetEnd;    // end of file
} md2_header;


class CModelMd2 : public CModel
{
	
private:

    md2_header* md2Header;  // model format structure
    char* modeldata;        // model data
    
public:

     CModelMd2();        // constructor
     ~CModelMd2();       // destructor

     // load model only
     bool LoadModel(string md2_filename);
     
     bool LoadModel(string md2_filename, string bmp_filename);

     // render a single frame
     void Draw(CGraphics* g);
     
     CModel* Clone();

protected:

    // render model with interpolation to get animation
    int AnimateModel(CGraphics* g);
     
private:
	   // free memory of model
     void Clear();

     bool LoadMd2(string model_filename);

};
