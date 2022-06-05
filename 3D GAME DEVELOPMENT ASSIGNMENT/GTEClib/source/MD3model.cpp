#include "MD3model.h"

#include <iostream>
using namespace std;



// ModelMd3 constructor
CModelMd3::CModelMd3() : CModel()
{
    md3Header = NULL;
    modeldata = NULL;
    Color.Set(1.0f, 1.0f, 1.0f, 1.0f); // white
}    

// CModelMd2 destructor
CModelMd3::~CModelMd3()
{
	Clear();
}

bool CModelMd3::LoadModel(string md3_filename)
{ 
    if (md3_filename.find(".md3") < 1 && md3_filename.find(".MD3") < 1)
    {
        cout << "ERROR: " << md3_filename << " is not an MD3 file" << endl;
        return false;
    }
    return LoadMd3(md3_filename);
}

bool CModelMd3::LoadModel(string md3_filename, string bmp_filename)
{
    if (md3_filename.find(".md3") < 1 && md3_filename.find(".MD3") < 1)
    {
        cout << "ERROR: " << md3_filename << " is not an MD3 file" << endl;
        return false;
    }
    numG = 1; // one group and texture
    if (LoadMd3(md3_filename)) { return LoadTexture(bmp_filename);}
    return false;
}

// Load MD3 model
bool CModelMd3::LoadMd3(string filename)
{
    streampos size;
    string model_filename = "models/" + filename;

    ifstream file(model_filename.c_str(), ios::in | ios::binary | ios::ate);
    if (file.is_open())
    {
        // reading in data
        size = file.tellg();
        modeldata = new char[(unsigned int)size];
        file.seekg(0, ios::beg);
        file.read(modeldata, size);
        file.close();

        // extract model file header from buffer
        md3Header = (md3_header*)modeldata;
        cout << "loading.. " << filename << " Frames: " << md3Header->numFrames << " Objects: " << md3Header->numMeshSurfaces << endl;
        
        // --- TODO: read in names of frame objects and assign frame numbers max 32 frame animations
        framedata = new anim_frame[32];
        numAnims = 0;

        // ------------------ Read texture coordinates and surface normals --------------

        //groups = new group[256];
        numG = (unsigned char)md3Header->numMeshSurfaces;
        numFrames = md3Header->numFrames;
        numTris = 0;
       
        // first read of surface meshes to setup group data
        int offset = 0;
        for (unsigned short n = 0; n < md3Header->numMeshSurfaces; n++)
        {
            md3_mesh* meshPtr = (md3_mesh*)&modeldata[md3Header->offsetMeshSurfaces + offset];
            //cout << meshPtr->name << " V: " << meshPtr->numVertices << " Tris: " << meshPtr->numTriangles << " -------------------------------- " << endl;

            groups[n].startIndex = 3 * numTris;
            numTris = numTris + meshPtr->numTriangles;
            groups[n].endIndex = 3 * (numTris);

            // print out texture filename
            char* texturename = (char*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetSkins];
            //cout << texturename << endl;
            groups[n].mat.name = texturename;
            groups[n].mat.texture_filename = texturename;
            groups[n].format = 7;
            groups[n].mat.texture_ID = 0;
            groups[n].mat.Kd[0] = 0.8f;
            groups[n].mat.Kd[1] = 0.8f;
            groups[n].mat.Kd[2] = 0.8f;
            groups[n].mat.Kd[3] = 1.0f;

            offset = offset + meshPtr->offsetEnd;
        }

        cout << "number of tris: " << numTris << endl;

        // ----------   obtain texture coordinates and normals for all mesh objects------------
        float nvec[32] = {-1,-0.93f,-0.87f,-0.8f,-0.73f,-0.67f,-0.6f,-0.53f,-0.47f,-0.4f,-0.33f,-0.27f,-0.2f,-0.13f,-0.07f,0,0.07f,0.13f,0.2f,0.27f,0.33f,0.4f,0.47f,0.53f,0.6f,0.67f,0.73f,0.8f,0.87f,0.93f,1,1 };
        vert = new vector3f[3 * numTris];
        norm = new vector3f[3 * numTris];
        tex = new vector2f[3 * numTris];

        offset = 0;		// offset to next surface mesh object
        int i = 0;		// index parameter
        for (unsigned short n = 0; n < md3Header->numMeshSurfaces; n++)
        {
            // mesh surface data start
            md3_mesh* meshPtr = (md3_mesh*)&modeldata[md3Header->offsetMeshSurfaces + offset];
            // texture coordinates start
            md3_texCoord* texPtr = (md3_texCoord*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetST];
            // vertex coordinates start
            md3_vertex* vertPtr = (md3_vertex*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetVertices];

            int sizef = meshPtr->offsetEnd - meshPtr->offsetVertices;
            // convert vertex data normals to new format
            for (int v = 0; v < meshPtr->numVertices * meshPtr->numFrames; v++)
            {
                short snorm, newnorm = 0;
                float lat, lng;

                // decode normals
                snorm = vertPtr[v].n;
                lng = (snorm & short(255)) * 2 * 3.1416f / 255.0f;
                lat = ((snorm >> 8)& (short)255) * 2 * 3.1416f / 255.0f;
                short x = short(15 * (1.01f + float(cos(lat) * sin(lng))));
                short y = short(15 * (1.01f + float(cos(lng))));
                short z = short(15 * (1.01f + float(sin(lat) * sin(lng))));
                // encode new normal format
                newnorm = x + 32 * y + 32 * 32 * z;
                vertPtr[v].n = newnorm; // assign new normal format
            }

            // read triangle index data for texture coordinates
            for (unsigned short t = 0; t < meshPtr->numTriangles; t++)
            {
                md3_triangle* vertTrias = (md3_triangle*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetTriangles];
                int i1 = vertTrias[t].v[0];
                int i2 = vertTrias[t].v[1];
                int i3 = vertTrias[t].v[2];

                tex[i].x = texPtr[i1].s; tex[i++].y = 1.0f - texPtr[i1].t;
                //cout << "s: " << tex[i - 1].x << " t: " << tex[i - 1].y << endl;
                tex[i].x = texPtr[i2].s; tex[i++].y = 1.0f - texPtr[i2].t;
                //cout << "s: " << tex[i - 1].x << " t: " << tex[i - 1].y << endl;
                tex[i].x = texPtr[i3].s; tex[i++].y = 1.0f - texPtr[i3].t;
                //cout << "s: " << tex[i - 1].x << " t: " << tex[i - 1].y << endl;
            }

            offset = offset + meshPtr->offsetEnd;
        }

        // ---- decode first frame ----

        unsigned short frame = 0;
        md3_frame* framePtr = (md3_frame*)&modeldata[md3Header->offsetFrames + sizeof(md3_frame) * frame];
        //cout << framePtr->name << endl;
        // get dimensions for first frame
        minx = framePtr->min.x; maxx = framePtr->max.x;
        miny = framePtr->min.z; maxy = framePtr->max.z;
        minz = framePtr->min.y; maxz = framePtr->max.y;

        offset = 0;		// offset to next surface mesh object
        i = 0;			// index parameter
        for (unsigned short n = 0; n < md3Header->numMeshSurfaces; n++)
        {
            // start of mesh surfaces
            md3_mesh* meshPtr = (md3_mesh*)&modeldata[md3Header->offsetMeshSurfaces + offset];

            // vertex pointers array start
            md3_vertex* vertPtr = (md3_vertex*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetVertices + frame * sizeof(md3_vertex) * meshPtr->numVertices];

            // process triangle data
            for (unsigned short t = 0; t < meshPtr->numTriangles; t++)
            {
                md3_triangle* vertTrias = (md3_triangle*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetTriangles];

                for (unsigned short v = 0; v < 3; v++)
                {
                    int i1 = vertTrias[t].v[v];
                    vert[i].x = vertPtr[i1].x * 0.015625f;
                    vert[i].z = vertPtr[i1].y * 0.015625f;
                    vert[i].y = vertPtr[i1].z * 0.015625f;
                    short newnorm = vertPtr[i1].n;
                    norm[i].x = nvec[newnorm & 31];
                    norm[i].y = nvec[newnorm >> 5 & 31];
                    norm[i].z = nvec[newnorm >> 10 & 31];
                    //cout << "xn: " << norm[i].x << " yn: " << norm[i].y << " zn: " << norm[i].z << endl;
                    i++;
                }

            }

            offset = offset + meshPtr->offsetEnd;
        }

        // --------------- Loading Textures ------------------
        int diroffset = (int)model_filename.find_last_of("/");

        for (unsigned char g = 0; g < numG; g++)
        {
            string skin;
            skin.assign(groups[g].mat.texture_filename);
            if (diroffset > 0) skin.insert(0,model_filename.substr(0, diroffset+1));

            if (groups[g].mat.texture_ID == 0)
            {
                // load texture and assign same texture ID to other groups of the same name
                //cout << "load " << skin << endl;
                unsigned short texID = LoadTexture(skin, CColor::Black(), false);
                groups[g].mat.texture_ID = texID;
                if (texID == 0) break;
                // make sure to load the same texture only once
                for (unsigned char n = 0; n < numG; n++)
                {
                    if (strcmp(groups[g].mat.texture_filename, groups[n].mat.texture_filename) == 0) groups[n].mat.texture_ID = texID;
                }
            }

        }


         
    }
    else cout << "ERROR: Could not load " << model_filename << endl;
    currentFrame = 0;
    
     
     // set y position
	 Position.y=(maxy-miny)/2.0f;
     
     
     return true;
}


// int (startFrame, int endFrame, float percent)
int CModelMd3::AnimateModel(CGraphics* g)
{
     float interpol;				// percent through current frame
     int nextFrame;					 // next frame in animation sequence
     float nvec[32] = {-1,-0.93f,-0.87f,-0.8f,-0.73f,-0.67f,-0.6f,-0.53f,-0.47f,-0.4f,-0.33f,-0.27f,-0.2f,-0.13f,-0.07f,0,0.07f,0.13f,0.2f,0.27f,0.33f,0.4f,0.47f,0.53f,0.6f,0.67f,0.73f,0.8f,0.87f,0.93f,1,1 };

     if ((startFrame > currentFrame))
          currentFrame = (float)startFrame;
     
     if ((startFrame < 0) || (stopFrame < 0)) return -1;
     
     if ((startFrame >= numFrames) || (stopFrame >= numFrames))
          return 0;
     
     interpol=(float)currentFrame-(int)currentFrame;
     nextFrame=(int)currentFrame+1;
     
     if (nextFrame > stopFrame) 
     {
		 nextFrame = startFrame;
	 }
     
     // display the textured model with lighting normals
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
     for (unsigned char n = 0; n < numG; n++) // draw groups
     {
         glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
         glColor4f(Brightness * Color.R * groups[n].mat.Kd[0], Brightness * Color.G * groups[n].mat.Kd[1], Brightness * Color.B * groups[n].mat.Kd[2], groups[n].mat.Kd[3]);

         glBindTexture(GL_TEXTURE_2D, groups[n].mat.texture_ID);

         glPushMatrix();
         glTranslatef(Position.x, Position.y, Position.z);  	// transformation to world coordinates
         glRotatef(Rotation.x, 1, 0, 0);       // rotation around x-axis
         glRotatef(Rotation.y, 0, 1, 0);       // rotation around y-axis
         glRotatef(Rotation.z, 0, 0, 1);       // rotation around z-axis
         glScalef(Scale, Scale, Scale);

         // draw vertex array data
         glEnableClientState(GL_VERTEX_ARRAY);
         glEnableClientState(GL_NORMAL_ARRAY);
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);

         md3_frame* framePtr1 = (md3_frame*)&modeldata[md3Header->offsetFrames + sizeof(md3_frame) * (int)currentFrame];
         md3_frame* framePtr2 = (md3_frame*)&modeldata[md3Header->offsetFrames + sizeof(md3_frame) * (int)nextFrame];

         // get dimensions for interpolated frame
         minx = framePtr1->min.x + interpol * (framePtr2->min.x - framePtr1->min.x);
         maxx = framePtr1->max.x + interpol * (framePtr2->max.x - framePtr1->max.x);
         minz = framePtr1->min.y + interpol * (framePtr2->min.y - framePtr1->min.y);
         maxz = framePtr1->max.y + interpol * (framePtr2->max.y - framePtr1->max.y);
         miny = framePtr1->min.z + interpol * (framePtr2->min.z - framePtr1->min.z);
         maxy = framePtr1->max.z + interpol * (framePtr2->max.z - framePtr1->max.z);

         int offset = 0;		// offset to next surface mesh object
         int i = 0;			    // index parameter
         for (unsigned short n = 0; n < md3Header->numMeshSurfaces; n++)
         {
             // start of mesh surfaces
             md3_mesh* meshPtr = (md3_mesh*)&modeldata[md3Header->offsetMeshSurfaces + offset];

             // vertex pointers array start
             md3_vertex* vertPtr1 = (md3_vertex*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetVertices + (int)currentFrame * sizeof(md3_vertex) * meshPtr->numVertices];
             md3_vertex* vertPtr2 = (md3_vertex*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetVertices + (int)nextFrame * sizeof(md3_vertex) * meshPtr->numVertices];

             // process triangle data
             for (unsigned short t = 0; t < meshPtr->numTriangles; t++)
             {
                 md3_triangle* vertTrias = (md3_triangle*)&modeldata[md3Header->offsetMeshSurfaces + offset + meshPtr->offsetTriangles];

                 for (unsigned short v = 0; v < 3; v++)
                 {
                     vector3f n1, n2;
                     int i1 = vertTrias[t].v[v];
                     // interpolated vertex coordinate
                     vert[i].x = 0.015625f * (vertPtr1[i1].x + interpol * (vertPtr2[i1].x - vertPtr1[i1].x));
                     vert[i].z = 0.015625f * (vertPtr1[i1].y + interpol * (vertPtr2[i1].y - vertPtr1[i1].y));
                     vert[i].y = 0.015625f * (vertPtr1[i1].z + interpol * (vertPtr2[i1].z - vertPtr1[i1].z));

                     short newnorm = vertPtr1[i1].n;
                     n1.x = nvec[newnorm & 31];
                     n1.y = nvec[newnorm >> 5 & 31];
                     n1.z = nvec[newnorm >> 10 & 31];
                     newnorm = vertPtr2[i1].n;
                     n2.x = nvec[newnorm & 31];
                     n2.y = nvec[newnorm >> 5 & 31];
                     n2.z = nvec[newnorm >> 10 & 31];

                     norm[i].x = n1.x + interpol * (n2.x - n1.x);
                     norm[i].y = n1.y + interpol * (n2.y - n1.y);
                     norm[i].z = n1.z + interpol * (n2.z - n1.z);
                     i++;
                 }
             }

             offset = offset + meshPtr->offsetEnd;
         }

         glTexCoordPointer(2, GL_FLOAT, 0, tex);
         glVertexPointer(3, GL_FLOAT, 0, vert);
         glNormalPointer(GL_FLOAT, 0, norm);

         unsigned int start = groups[n].startIndex;
         unsigned int numTrias = groups[n].endIndex - start;
         glDrawArrays(GL_TRIANGLES, start, numTrias);

         glDisableClientState(GL_VERTEX_ARRAY);
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);

         glPopMatrix();

         glBindTexture(GL_TEXTURE_2D, 0);
     }
     
     if (g->IsDebug()) // if debug draw bounding box
     {
         glLineWidth(2.0);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glColor3f(1.0, 0, 0);
         DrawBox(true);
     }
    
     
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

     if (childNode!=NULL && visible) ((CModelMd3*)childNode)->AnimateModel(g);
     
     return 0;
}

void CModelMd3::Draw(CGraphics* g)
{ 
     
     //if (currentFrame >= numFrames) return;
     if (vert == NULL) return;
     
     if (visible)
     {
      if (numFrames > 1)  // draw animated model
      {
		 AnimateModel( g);
		 return;
	  }
         
      // display the textured model with lighting normals
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      for (unsigned char n = 0; n < numG; n++) // draw groups
      {
         glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
         //glColorMaterial(GL_FRONT, GL_AMBIENT);
         glColor4f(Brightness * Color.R * groups[n].mat.Kd[0], Brightness * Color.G * groups[n].mat.Kd[1], Brightness * Color.B * groups[n].mat.Kd[2], groups[n].mat.Kd[3]);

         glBindTexture(GL_TEXTURE_2D, groups[n].mat.texture_ID);

         glPushMatrix();
         glTranslatef(Position.x, Position.y, Position.z);  	// transformation to world coordinates
         glRotatef(Rotation.x, 1, 0, 0);       // rotation around x-axis
         glRotatef(Rotation.y, 0, 1, 0);       // rotation around y-axis
         glRotatef(Rotation.z, 0, 0, 1);       // rotation around z-axis
         glScalef(Scale, Scale, Scale);

         // draw vertex array data
         glEnableClientState(GL_VERTEX_ARRAY);
         glEnableClientState(GL_NORMAL_ARRAY);
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
             
         glTexCoordPointer(2, GL_FLOAT, 0, tex);
         glVertexPointer(3, GL_FLOAT, 0, vert);
         glNormalPointer(GL_FLOAT, 0, norm);

         unsigned int start = groups[n].startIndex;
         unsigned int numTrias = groups[n].endIndex - start;
         glDrawArrays(GL_TRIANGLES, start, numTrias);

         glDisableClientState(GL_VERTEX_ARRAY);
         glDisableClientState(GL_NORMAL_ARRAY);
         glDisableClientState(GL_TEXTURE_COORD_ARRAY);

         glPopMatrix();

         glBindTexture(GL_TEXTURE_2D, 0);
      }
     } 
     
     if (g->IsDebug())  // if debug draw bounding box
     {
		 glLineWidth(2.0);
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	 }
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); 
     
     if (childNode!=NULL && visible) ((CModelMd3*)childNode)->Draw(g);     	
     
}



CModel* CModelMd3::Clone()
{
	// create new model
	CModelMd3* m = new CModelMd3();
    m->isCloned = true;

    m->minx = minx; m->maxx = maxx; m->miny = miny; m->maxy = maxy; m->minz = minz; m->maxz = maxz;
    m->Scale = Scale;
    m->ScaleBB = ScaleBB;
    m->Color.Set(Color);
    m->Color2.Set(Color2);
    m->Brightness = Brightness;
    m->filled = filled;
    m->border = border;
    m->visible = visible;

    m->Position = Position;
    m->Status = Status;
    m->Health = Health;
    m->Id = Id;

    m->Direction = Direction;
    m->Speed = Speed;
    m->Rotation = Rotation;
    m->Omega = Omega;

	
	m->currentFrame=currentFrame;
	m->numFrames=numFrames;
	m->stopFrame=stopFrame;
	m->startFrame=startFrame;
    m->period = period;

    m->framedata = framedata;
    m->numAnims = numAnims;
    m->selectedAnimation = selectedAnimation;
	
	// assign texture ID
	m->TextureID=TextureID;

    // copy references to model data
    m->md3Header = md3Header;
    m->modeldata = modeldata;
    m->numTris = numTris;
    m->vert = vert;
    m->tex = tex;
    m->norm = norm;

    m->numG = numG;
    memcpy(m->groups, groups, numG * (sizeof(group)));
    //m->groups = groups;

   
	
    if (childNode != NULL)
    {
        m->childNode = childNode->Clone();
    }
	
	return m;
}


void CModelMd3::Clear()
{
	if (!isCloned)
	{
     if (modeldata != NULL) delete[] modeldata;
     if (vert != NULL) delete[] vert;
     if (norm != NULL) delete[] norm;
     if (tex != NULL) delete[] tex;
     for (unsigned n = 0; n < numG; n++) glDeleteTextures(1, (GLuint*)&groups[n].mat.texture_ID);
     //if (groups != NULL) delete[] groups;
     if (framedata != NULL) delete[] framedata;
     glDeleteTextures(1, &TextureID);
    }
    
    if (groups != NULL) delete[] groups;
    numTris = 0; vert = NULL; norm = NULL; tex = NULL;
    numG = 0;  groups = NULL; modeldata = NULL; md3Header = NULL;
    framedata = NULL;
    numAnims = 0; selectedAnimation = 0;

    if (childNode != NULL)
    {
        //delete childNode;
        childNode = NULL;
    }

}


