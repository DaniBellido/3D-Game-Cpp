#include "MD2model.h"

#include <iostream>
using namespace std;

vector3f anorms_table[162] = {
#include "anorms.h"
};

// ModelMd2 constructor
CModelMd2::CModelMd2() : CModel()
{
    md2Header = NULL;
    modeldata = NULL;
    Color.Set(1.0f, 1.0f, 1.0f, 1.0f); // white
    
}    

// CModelMd2 destructor
CModelMd2::~CModelMd2()
{
	Clear();
}

bool CModelMd2::LoadModel(string md2_filename)
{ 
    if (md2_filename.find(".md2") < 1 && md2_filename.find(".MD2") < 1)
    {
        cout << "ERROR: " << md2_filename << " is not an MD2 file" << endl;
        return false;
    }

    return LoadMd2(md2_filename);
}

bool CModelMd2::LoadModel(string md2_filename, string bmp_filename)
{
    if (md2_filename.find(".md2") < 1 && md2_filename.find(".MD2") < 1)
    {
        cout << "ERROR: " << md2_filename << " is not an MD2 file" << endl;
        return false;
    }
    numG = 1; // one group and texture
    if (LoadMd2(md2_filename)) { return LoadTexture(bmp_filename);}
    return false;
}

// Load MD2 model
bool CModelMd2::LoadMd2(string filename)
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
        md2Header = (md2_header*)modeldata;


        // --- calculate texture coordinates for all triangles---
        tex = new vector2f[3 * md2Header->numTris];
        md2_texCoord* stPtr = (md2_texCoord*)&modeldata[md2Header->offsetST];
        md2_triangle* trisPtr = (md2_triangle*)&modeldata[md2Header->offsetTris];
        unsigned short i = 0; // index parameter
        for (unsigned short n = 0; n < md2Header->numTris; n++)
        {
            tex[i].x = (float)stPtr[trisPtr[n].st[0]].s / md2Header->skinwidth;
            tex[i++].y = 1.0f - (float)stPtr[trisPtr[n].st[0]].t / md2Header->skinheight;
            tex[i].x = (float)stPtr[trisPtr[n].st[1]].s / md2Header->skinwidth;
            tex[i++].y = 1.0f - (float)stPtr[trisPtr[n].st[1]].t / md2Header->skinheight;
            tex[i].x = (float)stPtr[trisPtr[n].st[2]].s / md2Header->skinwidth;
            tex[i++].y = 1.0f - (float)stPtr[trisPtr[n].st[2]].t / md2Header->skinheight;
            //cout << tex[i - 1].x << " " << tex[i - 1].y << endl;
        }

        minx = 100000;
        maxx = -100000;
        miny = 100000;
        maxy = -100000;
        minz = 100000;
        maxz = -100000;

        // --- calculate triangles and normals and dimensions ----
        i = 0;
        unsigned short frame = 0;
        numTris = md2Header->numTris;
        numFrames = md2Header->numFrames;
        vert = new vector3f[3 * md2Header->numTris];
        norm = new vector3f[3 * md2Header->numTris];

        // non-animated model using a single frame
        md2_frame* framePtr = (md2_frame*)&modeldata[md2Header->offsetFrames + md2Header->framesize * frame];
        md2_vertex* vertPtr = (md2_vertex*)&framePtr->verts;
        trisPtr = (md2_triangle*)&modeldata[md2Header->offsetTris];

        for (unsigned short n = 0; n < md2Header->numTris; n++)
        {
            for (unsigned short t = 0; t < 3; t++)  // for each vertex of the triangle
            {
                unsigned short index = trisPtr[n].vertex[t];
                md2_vertex md2v = vertPtr[index];
                vector3f v, vn;
                // swap y and z
                v.x = (framePtr->scale.x * md2v.v[0]) + framePtr->translate.x;
                v.z = (framePtr->scale.y * md2v.v[1]) + framePtr->translate.y;
                v.y = (framePtr->scale.z * md2v.v[2]) + framePtr->translate.z;
                vert[i] = v;

                minx = (v.x < minx) ? v.x : minx;
                miny = (v.y < miny) ? v.y : miny;
                minz = (v.z < minz) ? v.z : minz;
                maxx = (v.x > maxx) ? v.x : maxx;
                maxy = (v.y > maxy) ? v.y : maxy;
                maxz = (v.z > maxz) ? v.z : maxz;

                vn = anorms_table[md2v.normalIndex];
                norm[i].x = vn.x;
                norm[i].z = vn.y;
                norm[i].y = vn.z;
                i++;
            }
        }
        cout << "loading.. " << model_filename << " Tris:" << md2Header->numTris << " Frames:" << md2Header->numFrames << " W:" << int(maxx - minx) << " H:" << int(maxy - miny) << " D:" << int(maxz - minz) << endl;

        // --- read in names of frames and assign frame numbers max 32 frame animations
        framedata = new anim_frame[32];
        char currentframename[16];  // current frame name
        numAnims = 0;

        for (unsigned short f = 0; f < md2Header->numFrames; f++)
        {
            md2_frame* framePtr = (md2_frame*)&modeldata[md2Header->offsetFrames + md2Header->framesize * f];

            if (f == 0)
            {
                for (unsigned char n = 3; n < 16; n++)
                {
                    if (framePtr->name[n] == '\0' || framePtr->name[n] == '\r')
                    {
                        framePtr->name[n - 3] = '\0';
                        memcpy(framedata->name, framePtr->name, 16);
                        memcpy(&currentframename, framePtr->name, 16);
                        framedata->start = f; framedata->end = f;
                        numAnims = 1;
                        //cout << framedata->name << " " << f << endl;
                        break;
                    }
                }
            }
            else
            {
                for (unsigned char n = 3; n < 16; n++)
                {
                    if (framePtr->name[n] == '\0' || framePtr->name[n] == '\r')
                    {
                        framePtr->name[n - 3] = '\0';
                        if (strcmp(currentframename, framePtr->name) == 0)
                        {
                            anim_frame* animPtr = (anim_frame*)&framedata[numAnims - 1];
                            animPtr->end = f;
                            //cout << framePtr->name << " " << f << endl;
                        }
                        else
                        {
                            anim_frame* animPtr = (anim_frame*)&framedata[numAnims];
                            animPtr->start = f; animPtr->end = f;
                            memcpy(animPtr->name, framePtr->name, 16);
                            memcpy(&currentframename, framePtr->name, 16);
                            if (numAnims < 30) numAnims++;
                            //cout << animPtr->name << " " << f << endl;
                        }
                        break;
                    }
                }
            }
        }

        // display animation names
        for (unsigned short n = 0; n < numAnims; n++)
        {
            anim_frame* animPtr = (anim_frame*)&framedata[n];
            cout << animPtr->name << ":" << animPtr->start << "-" << animPtr->end << " ";

        }
        cout << endl;


        // --- read texture file name (if not already done) ---
        if (md2Header->numSkins == 1 && numG == 0)
        {
            string skin;
            skin.assign(&modeldata[md2Header->offsetSkins]);
            // sometimes the texture images are stored with pathnames
            // we want to ignore those
            int diroffset = (int)skin.find_last_of("/");

            if (diroffset > 0)
            {
                string tex = skin.substr(diroffset + 1);
                diroffset = (int)filename.find_last_of("/");
                if (diroffset > 0)
                {
                    tex.insert(0, filename.substr(0, diroffset + 1));
                }
                //cout << "Texture file: " << tex << endl;
                numG = 1; LoadTexture(tex);
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
int CModelMd2::AnimateModel(CGraphics* g)
{
     float interpol;				// percent through current frame
     int nextFrame;					 // next frame in animation sequence

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

     // determining dimensions
     minx = 100000;
     maxx = -100000;
     miny = 100000;
     maxy = -100000;
     minz = 100000;
     maxz = -100000;
	 
     md2_triangle* trisPtr = (md2_triangle*)&modeldata[md2Header->offsetTris];
     md2_frame* framePtr1 = (md2_frame*)&modeldata[md2Header->offsetFrames + md2Header->framesize * (int)currentFrame];
     md2_vertex* vertPtr1 = (md2_vertex*)&framePtr1->verts;
     md2_frame* framePtr2 = (md2_frame*)&modeldata[md2Header->offsetFrames + md2Header->framesize * nextFrame];
     md2_vertex* vertPtr2 = (md2_vertex*)&framePtr2->verts;
             
     // calculate vertex and normal array
     int i = 0;                       // index counter
     for (unsigned short n = 0; n < md2Header->numTris; n++)
     {
         for (unsigned t = 0; t < 3; t++)
         {
             unsigned short index = trisPtr[n].vertex[t];
             md2_vertex md2v = vertPtr1[index];
             vector3f v1, v2, vn1, vn2;

             // first frame swap y and z
             v1.x = (framePtr1->scale.x * md2v.v[0]) + framePtr1->translate.x;
             v1.z = (framePtr1->scale.y * md2v.v[1]) + framePtr1->translate.y;
             v1.y = (framePtr1->scale.z * md2v.v[2]) + framePtr1->translate.z;
             vn1 = anorms_table[md2v.normalIndex];

             // second frame swap y and z
             md2v = vertPtr2[index];
             v2.x = (framePtr2->scale.x * md2v.v[0]) + framePtr2->translate.x;
             v2.z = (framePtr2->scale.y * md2v.v[1]) + framePtr2->translate.y;
             v2.y = (framePtr2->scale.z * md2v.v[2]) + framePtr2->translate.z;
             vn2 = anorms_table[md2v.normalIndex];

             // interpolation vertex positions
             vert[i].x = v1.x + interpol * (v2.x - v1.x);
             vert[i].y = v1.y + interpol * (v2.y - v1.y);
             vert[i].z = v1.z + interpol * (v2.z - v1.z);


             minx = (vert[i].x < minx) ? vert[i].x : minx;
             miny = (vert[i].y < miny) ? vert[i].y : miny;
             minz = (vert[i].z < minz) ? vert[i].z : minz;
             maxx = (vert[i].x > maxx) ? vert[i].x : maxx;
             maxy = (vert[i].y > maxy) ? vert[i].y : maxy;
             maxz = (vert[i].z > maxz) ? vert[i].z : maxz;


             // interpolation of normal vector
             norm[i].x = vn1.x + interpol * (vn2.x - vn1.x);
             norm[i].z = vn1.y + interpol * (vn2.y - vn1.y);
             norm[i].y = vn1.z + interpol * (vn2.z - vn1.z);

             i++;
         }
     }

     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

     glBindTexture(GL_TEXTURE_2D, TextureID);

     glPushMatrix();

     glTranslatef(Position.x, Position.y, Position.z);
     glRotatef(Rotation.x, 1, 0, 0);   // rotation around x-axis
     glRotatef(Rotation.y, 0, 1, 0);   // rotation around y-axis
     glRotatef(Rotation.z, 0, 0, 1);   // rotation around z-axis
     glScalef(Scale, Scale, Scale);

     Brightness = 0.8f;
     glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
     glColor3f(Brightness * Color.R, Brightness * Color.G, Brightness * Color.B);

     // draw vertex array data
     glEnableClientState(GL_VERTEX_ARRAY);
     glEnableClientState(GL_NORMAL_ARRAY);
     if (tex != NULL)
     {
         glEnableClientState(GL_TEXTURE_COORD_ARRAY);
         glTexCoordPointer(2, GL_FLOAT, 0, tex);
     }

     glVertexPointer(3, GL_FLOAT, 0, vert);
     glNormalPointer(GL_FLOAT, 0, norm);

     glDrawArrays(GL_TRIANGLES, 0, 3 * numTris);
     glDisableClientState(GL_VERTEX_ARRAY);
     glDisableClientState(GL_NORMAL_ARRAY);
     glDisableClientState(GL_TEXTURE_COORD_ARRAY);

     glPopMatrix();

     glBindTexture( GL_TEXTURE_2D, 0); 
     
     if (g->IsDebug()) // if debug draw bounding box
     {
         glLineWidth(2.0);
         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         glColor3f(1.0, 0, 0);
         DrawBox(true);
     }
    
     
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

     if (childNode!=NULL && visible) ((CModelMd2*)childNode)->AnimateModel(g);
     
     return 0;
}

void CModelMd2::Draw(CGraphics* g)
{ 
     
     //if (currentFrame >= numFrames) return;
     if (vert == NULL) return;
     
     //if (visible)
     {
     if (numFrames > 1)  // draw animated model
     {
		 AnimateModel( g);
		 return;
	 }
         
     // display the textured model with lighting normals
     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	 glBindTexture( GL_TEXTURE_2D, TextureID); 
	  	
	  glPushMatrix();
	 
       glTranslatef(Position.x, Position.y, Position.z);
        glRotatef( Rotation.x, 1, 0, 0 );   // rotation around x-axis
	    glRotatef( Rotation.y, 0, 1, 0 );   // rotation around y-axis
	    glRotatef( Rotation.z, 0, 0, 1 );   // rotation around z-axis
        glScalef( Scale, Scale, Scale);
	   
	   //Brightness = 0.8f;
       //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	   //glColor3f(Brightness * Color.R, Brightness * Color.G, Brightness * Color.B);

       // only one group in md2 file
       glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
       glColor4f(Brightness * Color.R * groups[0].mat.Kd[0], Brightness * Color.G * groups[0].mat.Kd[1], Brightness * Color.B * groups[0].mat.Kd[2], groups[0].mat.Kd[3]);

       // draw vertex array data
       glEnableClientState(GL_VERTEX_ARRAY);
       glEnableClientState(GL_NORMAL_ARRAY);
       glEnableClientState(GL_TEXTURE_COORD_ARRAY);

       glTexCoordPointer(2, GL_FLOAT, 0, tex);
       glVertexPointer(3, GL_FLOAT, 0, vert);
       glNormalPointer(GL_FLOAT, 0, norm);

       glDrawArrays(GL_TRIANGLES, 0, 3*numTris);
       glDisableClientState(GL_VERTEX_ARRAY);
       glDisableClientState(GL_NORMAL_ARRAY);
       glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	   
  
	  glPopMatrix();
     
     glBindTexture( GL_TEXTURE_2D, 0);
     } 
     
     if (g->IsDebug())  // if debug draw bounding box
     {
		 glLineWidth(2.0);
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	 }
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); 
     
     if (childNode!=NULL && visible) ((CModelMd2*)childNode)->Draw(g);     	
     
}



CModel* CModelMd2::Clone()
{
	// create new model
	CModelMd2* m = new CModelMd2();
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

	
    m->currentFrame = currentFrame;
    m->numFrames = numFrames;
    m->stopFrame = stopFrame;
    m->startFrame = startFrame;
    m->period = period;

    m->framedata = framedata;
    m->numAnims = numAnims;
    m->selectedAnimation = selectedAnimation;
	
	// assign texture ID
	m->TextureID=TextureID;

    // copy references to model data
    m->md2Header = md2Header;
    m->modeldata = modeldata;
    m->numTris = numTris;
    m->vert = vert;
    m->tex = tex;
    m->norm = norm;

    // copy group data
    m->numG = numG;
    memcpy(m->groups, groups, numG * (sizeof(group)));
    //m->groups = groups;

	
    if (childNode != NULL)
    {
        m->childNode = childNode->Clone();
    }
	
	return m;
}


void CModelMd2::Clear()
{
	if (!isCloned)
	{
     if (modeldata != NULL) delete[] modeldata;
     if (vert != NULL) delete[] vert;
     if (norm != NULL) delete[] norm;
     if (tex != NULL) delete[] tex;
     //if (groups != NULL) delete[] groups;
     if (framedata != NULL) delete[] framedata;
     glDeleteTextures(1, &TextureID);
    }
    
    if (groups != NULL) delete[] groups;
    numTris = 0; vert = NULL; norm = NULL; tex = NULL;
    numG = 0;  groups = NULL; modeldata = NULL; md2Header = NULL;
    framedata = NULL;
    numAnims = 0; selectedAnimation = 0;

    if (childNode != NULL)
    {
        //delete childNode;
        childNode = NULL;
    }

}


