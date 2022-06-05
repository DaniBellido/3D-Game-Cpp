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
***************************************************************************/
#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Texture.h"
#include "Model.h"

/* model frame */
typedef struct
{
	vector3f min;    // bonding box bottom left
	vector3f max;	 // bounding box top right
	vector3f origin; // model origin
	float radius;    // radius of bounding sphere
	char name[16];   // name of animation
} md3_frame;

/* vertex position and normal: Position multiply by 1/64 */
typedef struct
{
	short x;		// x coordinate
	short y;		// y coordinate
	short z;		// z coordinate
	short n;		// normal encoded as a 16 bit short
} md3_vertex;

typedef struct
{
	float s;
	float t;
} md3_texCoord;

/* triangle made of three verices*/
typedef struct
{
	int v[3];		// index of vertices of triangle
} md3_triangle;

/* tags */
typedef  struct
{
	char name[64];
	vector3f origin;
	vector3f axis1;
	vector3f axis2;
	vector3f axis3;
} md3_tag;

/* skin textures and shaders */
typedef struct
{
	char name[64];
	int index;
} md3_skin;

/* mesh surface object */
typedef struct
{
	char		ID[4];			//	ID of the file is always "IDP3"	
	char		name[64];		// name of the mesh group
	int			flags;
	int			numFrames;
	int			numSkins;		// typically 1 per mesh group
	int			numVertices;
	int			numTriangles;
	int			offsetTriangles;
	int			offsetSkins;
	int			offsetST;
	int			offsetVertices;
	int			offsetEnd;
} md3_mesh;


/* md3 header structure */
typedef struct
{
	char		ID[4];				//	ID of the file is always "IDP3"
	int			version;			//	version number, usually 15
	char		filename[64];		//  filename, sometimes left blank
	int			numFlags;			//  number of flags (typically zero)
	int			numFrames;			//  number of frames
	int			numTags;			//  number of 'tags' for objects
	int			numMeshSurfaces;	//  number of surface mesh objects
	int			numSkins;			//  number of unique skins
	int			offsetFrames;		//  offset to frame data
	int			offsetTags;			//  offset to tag data
	int			offsetMeshSurfaces;	//  offset to mesh surface data
	int			ofEndOfFile;		
} md3_header;

class CModelMd3 : public CModel
{
	
private:

    md3_header* md3Header;  // model format structure
    char* modeldata;		// model data

public:

     CModelMd3();        // constructor
     ~CModelMd3();       // destructor

     // load model only
     bool LoadModel(string md3_filename);
     
     bool LoadModel(string md3_filename, string bmp_filename);

     // render a single frame
     void Draw(CGraphics* g);
     
     CModel* Clone();

protected:
     // render model with interpolation to get animation
     int AnimateModel(CGraphics* g);
     
private:
	   // free memory of model
     void Clear();

     bool LoadMd3(string model_filename);

};
