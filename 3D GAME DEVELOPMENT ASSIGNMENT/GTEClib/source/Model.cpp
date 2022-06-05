#include "Model.h"

#include <iostream>
#include <fstream>


void CModel::Init()
{
	Position.x = Position.y = Position.z = 0.0f;

	minx = miny = minz = -0.5f;
	maxx = maxy = maxz = 0.5f;

	Scale = 1.0f;   // model scale
	ScaleBB = 1.0f; // bounding box scale 

	// appearance
	Color.Set(0.8f, 0.8f, 0.8f, 1.0f); // light grey
	Color2.Set(0.5f, 0.5f, 0.5f, 1.0f); // grey
	Brightness = 1.0f;
	filled = true; border = false;
	visible = true;
	// setup model vertex, texture and normal data 
	numTris = 0;
	vert=NULL;
	tex=NULL;
	norm=NULL;
	TextureID = 0;

	// material groups
	numG = 0;
	groups = new group[256];
	groups[0].startIndex = 0;
	groups[0].endIndex = 0;
	groups[0].mat.Kd[0] = 0.8f; groups[0].mat.Kd[1] = 0.8f; groups[0].mat.Kd[2] = 0.8f; groups[0].mat.Kd[3] = 1.0f;
	groups[0].mat.Ks[0] = 0; groups[0].mat.Ks[1] = 0; groups[0].mat.Ks[2] = 0; groups[0].mat.Ks[3] = 1.0f;
	groups[0].mat.Ka[0] = 0.2f; groups[0].mat.Ka[1] = 0.2f; groups[0].mat.Ka[2] = 0.2f; groups[0].mat.Ka[3] = 1.0f;
	groups[0].format = 0;
	groups[0].mat.texture_filename = NULL;
	groups[0].mat.texture_ID = 0;

	// Time
	SpriteTime = 0;

	// Properties
	Health = 100;	Status = 0; Id = 0;

	// Linear Motion { Veclocity and Direction }
	Direction.x = 1.0f; // facing in the direction of the positive x-axis
	Direction.y = Direction.z = 0.0f;
	Speed = 0.0f;		 	 // speed

	// Rotation
	Rotation.x = Rotation.y = Rotation.z = 0.0f;	// rotation angle
	Omega.x = Omega.y = Omega.z = 0.0f; // rotation speed

	marked_for_removal = false;
	dying = 0;
	automove = false;
	DistanceToTarget = 0.0f;

	childNode = NULL;

	isCloned = false;

	// setting up animation variables
	numFrames = 1; currentFrame = 1.0f; period = 0.0f; looping = false;
	startFrame = stopFrame = 1;

	framedata = NULL;
	numAnims = 0;
	selectedAnimation = 0; // starting at 1

}
// Update function
void CModel::Update(Uint32 GameTime)
{
	if (SpriteTime == 0) { SpriteTime = GameTime; return; }
	
	if (childNode!=NULL) CNodeUpdate();

	int deltaTime = GameTime - SpriteTime;
	if ( deltaTime < 0) deltaTime = 0;
	
	if (dying > 0) dying=dying-deltaTime;
	
	// linear motion update
    float dx = float(Speed*Direction.x*deltaTime/1000.0f);
	float dy = float(Speed*Direction.y*deltaTime/1000.0f);
	float dz = float(Speed*Direction.z*deltaTime/1000.0f);

    Position.x += dx; Position.y += dy; Position.z += dz;
    
    // auto movement
    if (automove)
    {
	  DistanceToTarget=DistanceToTarget-(float)sqrt(dx*dx+dy*dy+dz*dz);
	  if (DistanceToTarget <= 0) Stop();
	}
	
	// rotation update
	Rotate( Omega.x*deltaTime/1000.0f, Omega.y*deltaTime/1000.0f, Omega.z*deltaTime/1000.0f);
	
	// animation update forward
	if (period > 0)
	{
	  currentFrame=currentFrame+deltaTime*period/1000.0f;
	  if (currentFrame >= stopFrame+1) 
	  { 
		if (looping) currentFrame=(float)startFrame;
		else SetFrame( stopFrame); 
	  }
    }
    
    SpriteTime = GameTime;
    
    if (childNode!=NULL) 
    {
		CNodeUpdate();
		childNode->Update( GameTime); 
		
	}
		 		
}

void CModel::PlayAnimation( int start, int stop, float speed, bool loop)
{
	  if (currentFrame <= start || currentFrame >= stop+1 || period == 0)
	  {
	    startFrame=start; stopFrame=stop; period=speed; looping=loop;
	    currentFrame = (float)start; selectedAnimation = 0;
	  }
}

void CModel::PlayAnimation(string name, float speed, bool loop)
{
	if (framedata == NULL) return;

	// have we already selected this animation sequence
	if (selectedAnimation > 0)
	{
		if (strcmp(name.c_str(), ((anim_frame*)&framedata[selectedAnimation - 1])->name) == 0) return;
	}

	// find a matching sequence name in framedata list
	for (unsigned n = 0; n < numAnims; n++)
	{
		anim_frame* animPtr = &framedata[n];
		// found animation in list
		if (strcmp(name.c_str(), animPtr->name) == 0)
		{
			startFrame = animPtr->start; stopFrame = animPtr->end; period = speed; looping = loop;
			currentFrame = (float)animPtr->start;
			selectedAnimation = n + 1;
			break;
		}
	}
}

void CModel::AddAnimation(string name, int start, int stop)
{
	if (framedata != NULL && start >= 0 && stop < numFrames && numAnims < 31 && name.length() < 16)
	{
		framedata[numAnims].start = start;
		framedata[numAnims].end = stop;
		memcpy((char*)&framedata[numAnims].name, name.c_str(), name.length() + 1);
		numAnims++;
	}
}

void CModel::CNodeUpdate()
{
	 // todo: check all parameters
	 if (childNode!=NULL)
	 {
	    childNode->SetRotationV(GetRotationV());
		childNode->SetPositionV( GetPositionV());
		childNode->SetDirectionV( GetDirectionV());
		childNode->Speed=Speed;
		childNode->Omega.x=Omega.x; childNode->Omega.y=Omega.y; childNode->Omega.z=Omega.z;
		
		// animation handling
		childNode->currentFrame=currentFrame;
		childNode->startFrame=startFrame;
		childNode->stopFrame=stopFrame;
		childNode->period=period;
		childNode->looping=looping;
	 }
}

bool CModel::LoadModel(string obj_filename)
{
	if (obj_filename.find(".md2") < 1 || obj_filename.find(".MD2") < 1)
	{
		cout << "ERROR: Your are trying to load an md2 file. Make sure to define your model as CModelMd2" << endl;
	}
	if (obj_filename.find(".md3") < 1 || obj_filename.find(".MD3") < 1)
	{
		cout << "ERROR: Your are trying to load an md3 file. Make sure to define your model as CModelMd3" << endl;
	}
	if (obj_filename.find(".obj") < 1 && obj_filename.find(".OBJ") < 1)
	{
		cout << "ERROR: " << obj_filename << " is not an OBJ file" << endl;
		return false;
	}
	string filename = "models/" + obj_filename;
	return LoadObj(filename);
}

bool CModel::LoadModel(string obj_filename, string bmp_filename)
{
	if (obj_filename.find(".md2") < 1 || obj_filename.find(".MD2") < 1)
	{
		cout << "ERROR: Your are trying to load an md2 file. Make sure to define your model as CModelMd2" << endl;
	}
	if (obj_filename.find(".md3") < 1 || obj_filename.find(".MD3") < 1)
	{
		cout << "ERROR: Your are trying to load an md3 file. Make sure to define your model as CModelMd3" << endl;
	}
	if (obj_filename.find(".obj") < 1 && obj_filename.find(".OBJ") < 1)
	{
		cout << "ERROR: " << obj_filename << " is not an OBJ file" << endl;
		return false;
	}
	string filename = "models/" + obj_filename;
	if (LoadObj(filename,false)) { return LoadTexture(bmp_filename); }
	return false;
}

// load a single texture and assign to all groups
bool CModel::LoadTexture(string bmp_filename)
{
	string filename = "models/" + bmp_filename;
	TextureID=LoadTexture(filename,CColor::Black(),false); 
	// for backward compatability
	Color.Set(CColor::White());
	for (unsigned char n = 0; n < numG; n++)
	{
		groups[n].mat.texture_ID = TextureID;
		groups[n].mat.Kd[0] = 1.0f;
		groups[n].mat.Kd[1] = 1.0f;
		groups[n].mat.Kd[2] = 1.0f;
		groups[n].mat.Kd[3] = 1.0f;
	}
	return TextureID;
}

void CModel::CalculateNormals()
{
   vector3f a, b, c;
   float length;

   for (unsigned int n = 0; n < numTris; n = n + 3)
   {
	   a.x = vert[n].x - vert[n + 1].x;
	   a.y = vert[n].y - vert[n + 1].y;
	   a.z = vert[n].z - vert[n + 1].z;

	   b.x = vert[n].x - vert[n + 2].x;
	   b.y = vert[n].y - vert[n + 2].y;
	   b.z = vert[n].z - vert[n + 2].z;

	   c.x = a.y * b.z - b.y * a.z;
	   c.y = b.x * a.z - a.x * b.z;
	   c.z = a.x * b.y - b.x * a.y;

	   // calculate the length of the normal
	   length = (float)sqrt(c.x * c.x + c.y * c.y + c.z * c.z);

	   norm[n].x = c.x / length;   norm[n].y = c.y / length;    norm[n].z = c.z / length;
	   norm[n + 1].x = c.x / length;   norm[n + 1].y = c.y / length;    norm[n + 1].z = c.z / length;
	   norm[n + 2].x = c.x / length;   norm[n + 2].y = c.y / length;    norm[n + 2].z = c.z / length;
   }
}


void CModel::Clear()
{
	  if (!isCloned)
	  {
	    if (vert != NULL) delete [] vert;
	    if (norm != NULL) delete [] norm;
	    if (tex != NULL) delete [] tex;
		if (framedata != NULL) delete[] framedata;
		for (unsigned n = 0; n < numG; n++) glDeleteTextures(1, (GLuint*)&groups[n].mat.texture_ID);
		//if (groups != NULL) delete[] groups;
		glDeleteTextures(1, &TextureID);
	  }	

	  if (groups != NULL) delete[] groups;
	  numTris = 0; vert = NULL; norm = NULL; tex = NULL;
	  numG = 0;  groups = NULL; framedata = NULL;

	  if (childNode != NULL)
	  {
		  //delete childNode;
		  childNode = NULL;
	  }

}

char* CModel::decodeString(char* memblock)
{
	char* txt = new char[256];
	unsigned char i = 0;
	for (int n = 0; n < 256; n++)
	{
		if (memblock[n] != 0x20 && i == 0) continue;
		if (memblock[n] == 0x20) { i = 1; continue; }
		txt[i - 1] = memblock[n];
		if (txt[i - 1] == '\n' || txt[i - 1] == '\r') { txt[i - 1] = '\0'; break; }
		i++;
	}
	return txt;
}

void CModel::Draw(CGraphics* g)
{
	// any geometry data in vertex array?
	if (numTris>0)
	{
	  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	  for (unsigned char n = 0; n < numG; n++) // draw groups
	  {
		      glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			  //glColorMaterial(GL_FRONT, GL_DIFFUSE);
		      glColor4f(Brightness*Color.R*groups[n].mat.Kd[0], Brightness * Color.G*groups[n].mat.Kd[1], Brightness * Color.B*groups[n].mat.Kd[2], groups[n].mat.Kd[3]);
			  //glColorMaterial(GL_FRONT, GL_AMBIENT);
			  //glColor4f(Brightness * Color.R*groups[n].mat.Ka[0], Brightness * Color.G*groups[n].mat.Ka[1], Brightness * Color.B*groups[n].mat.Ka[2], groups[n].mat.Ka[3]);
			  //glColorMaterial(GL_FRONT, GL_SPECULAR);
			  //glColor4f(groups[n].mat.Ks[0], groups[n].mat.Ks[1], groups[n].mat.Ks[2], groups[n].mat.Ks[3]);
			  
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
			  if (tex != NULL)
			  {
				  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				  glTexCoordPointer(2, GL_FLOAT, 0, tex);
			  }
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
	else // just draw a box
	{
	  glLineWidth(2.0);
	  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	  glColor4f(Brightness * Color.R, Brightness * Color.G, Brightness * Color.B, Color.A);
	  if (!filled) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	  else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	  DrawBox(false);
	  
	  if (border) 
	  { 
		  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		  glColor4f(Brightness * Color2.R, Brightness * Color2.G, Brightness * Color2.B, Color2.A);
		  DrawBox(false);
	  }
    }
	
	if (g->IsDebug()) // if debug draw red bounding box
	{
		 glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		 glLineWidth(2.0);
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	}
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	
	
	if (childNode!=NULL && visible) 
	{
		childNode->Draw( g);
	}
		
}

void CModel::DrawBox(bool bbox)
{
    	glPushMatrix();
		  glTranslatef( Position.x, Position.y, Position.z);  	// transformation to world coordinates
		  glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
		  glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
		  glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
		  //glTranslatef(localPosition.x, localPosition.y, localPosition.z);
          
          if (bbox) glScalef( ScaleBB*Scale, ScaleBB*Scale, ScaleBB*Scale);
          else glScalef( Scale, Scale, Scale);
          
		  glBegin(GL_QUADS);
		   // front
		   glNormal3f(0,0,1);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(minx, maxy,  maxz);
		   // back
		   glNormal3f(0,0,-1);
		   glVertex3f(minx, miny,  minz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(maxx, maxy,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // top
		   glNormal3f(0,1,0);
		   glVertex3f(minx, maxy,  maxz);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(maxx, maxy,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // bottom
		   glNormal3f(0,-1,0);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(minx, miny,  minz);
		   // left
		   glNormal3f(-1,0,0);
		   glVertex3f(minx, maxy,  maxz);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(minx, miny,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // right
		    glNormal3f(1,0,0);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(maxx, maxy,  minz);
		   
		  glEnd();
		glPopMatrix();	
}


// move distance in xz plane in direction of rotation angle
void CModel::MoveXZRotation( float distance) 
{  
  Position.x += float(distance*cos( DEG2RAD (Rotation.y))); Position.z -= float(distance*sin( DEG2RAD (Rotation.y))); 
}

// move in xyz plane according to specified direction
void CModel::Move( float distance) 
{  
	Position.x += distance*Direction.x;
	Position.y += distance*Direction.y;
	Position.z += distance*Direction.z; 
}

void CModel::MoveTo(float x, float z, float speed) 
{
	DistanceToTarget=CVector::Distance( CVector(x,Position.y, z), Position); 
	SetDirectionAndRotationToPoint(x,z);
	SetSpeed( speed);
	automove=true;
}

void CModel::SetToFloor( float y) 
{
	Position.y = 1 + y - Scale*miny;
	//Position.y = y + 1 + (Scale * (maxy - miny)) / 2.0f; 
} 

// facing towards the vector specified through dx,dy,dz
void CModel::SetRotationV(float dX, float dY, float dZ)
{ 
	float angle_y = (float)RAD2DEG (atan2(-dZ, dX));
	Rotation.y = angle_y;
	float hyp_y=hypot(-dZ,dX);
	
	float angle_z=(float)RAD2DEG (atan2(dY, hyp_y));
	
	Rotation.z=angle_z;
}

CVector CModel::GetRotationV()
{
	float x=1.0f,y=0,z=0;  // default along positive x-axis
	CVector v; 
	
	GLfloat view[16];
	
	glPushMatrix();
	 glLoadIdentity();
	 glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view);
	 
    glPopMatrix();
    
    v.x=x*view[0]+y*view[4]+z*view[8]+view[12];
    v.y=x*view[1]+y*view[5]+z*view[9]+view[13];
    v.z=x*view[2]+y*view[6]+z*view[10]+view[14];
	
	return v;
}




void CModel::DrawVertex( float x, float y, float z)
{
	glColor3f(1.0,1.0,0);
	glPointSize( 7); 
	
	glBegin(GL_POINTS);
	 glVertex3f( x, y, z);
	glEnd();
		
}
// attached model shares the same origin at (0,0,0)
void CModel::AttachModel(CModel* model) { childNode = model->Clone(); }


CModel* CModel::Clone()
{
	// create new model
	CModel* m = new CModel();
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

	m->numFrames = numFrames;
	m->startFrame = startFrame;
	m->stopFrame = stopFrame;
	m->currentFrame = currentFrame;
	m->period = period;

	m->framedata = framedata;
	m->numAnims = numAnims;
	m->selectedAnimation = selectedAnimation;

	// assign texture ID
	m->TextureID=TextureID;

	// copy vertex data pointers
	m->numTris = numTris;
	m->vert = vert;
	m->norm = norm;
	m->tex = tex;
	

	m->numG = numG;
	memcpy(m->groups, groups, numG*(sizeof(group)));
	//m->groups = groups;
	
	m->ResetTime();

	if (childNode != NULL)
	{
		m->childNode = childNode->Clone();
	}

	return m;
	
}

float CModel::GetRadius()
{
	float xmax, ymax, zmax;
	
	if (abs(minx) > maxx) xmax=abs(minx);
	else xmax=maxx;
	
	if (abs(miny) > maxy) ymax=abs(miny);
	else ymax=maxy;
	
	if (abs(minz) > maxz) zmax=abs(minz);
	else zmax=maxz;
	
	return Scale*(float)sqrt( xmax*xmax+ymax*ymax+zmax*zmax);
}

CVector CModel::GetLocalPositionV(CVector pos)
{
	CVector p;
	float x,y,z;
	
	GLfloat view[16];
	
	x=pos.x; y=pos.y;  z=pos.z; 
	
	glPushMatrix();
	 glLoadIdentity();
	 glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view);
    glPopMatrix();
    
    p.x=x*view[0]+y*view[4]+z*view[8]+view[12];
    p.y=x*view[1]+y*view[5]+z*view[9]+view[13];
    p.z=x*view[2]+y*view[6]+z*view[10]+view[14];
    

	pos.x=p.x+Position.x; pos.y=p.y+Position.y; pos.z=p.z+Position.z; 
	
	return pos;
	
}

bool CModel::HitTest(CVector pos, float distance)
{
	float dist;
	
	dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
	
	if (dist < distance) return true;
	
	return false;
}

bool CModel::HitTest(CVector pos)
{
	float xp,yp,zp;
	float x,y,z;
	
	GLfloat view[16];
	
	x=xp=pos.x-Position.x; y=yp=pos.y-Position.y;  z=zp=pos.z-Position.z; 
  
    glPushMatrix();
    
	 glLoadIdentity();
	 glRotatef( -Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glRotatef( -Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( -Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view); 
	 
    glPopMatrix();
    
    xp=x*view[0]+y*view[4]+z*view[8]+view[12];
    yp=x*view[1]+y*view[5]+z*view[9]+view[13];
    zp=x*view[2]+y*view[6]+z*view[10]+view[14];
  
  
    if (xp >= Scale*ScaleBB*minx && xp <= Scale*ScaleBB*maxx && yp >= Scale*ScaleBB*miny && yp <= Scale*ScaleBB*maxy && zp >= Scale*ScaleBB*minz && zp <= Scale*ScaleBB*maxz) 
    {
		return true;
    }
	return false;
}

void CModel::DoRotation( CVector& pos, float yoffset)
{
	float xp,yp,zp;
	float x,y,z;
	
	GLfloat view[16];
	
	x=xp=pos.x-Position.x; y=yp=pos.y-Position.y;  z=zp=pos.z-Position.z; 
	
	glPushMatrix();
	 glLoadIdentity();
	 glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	 glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	 glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
	 glGetFloatv(GL_MODELVIEW_MATRIX, view);
	 
    glPopMatrix();
    
    xp=x*view[0]+y*view[4]+z*view[8]+view[12];
    yp=x*view[1]+y*view[5]+z*view[9]+view[13];
    zp=x*view[2]+y*view[6]+z*view[10]+view[14];
	
	pos.x=float(xp+Position.x); pos.y=float(yp+Position.y);  pos.z=float(zp+Position.z); 
	
	// ---------------------------------------------------------------------------------
	//DrawVertex(pos.x, pos.y, pos.z);
}

bool CModel::HitTest(CModel *pModel)
{
   CVector pos;
   
   pos = pModel->GetPositionV();
   
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
  
  
   // --- inside ? -----
   if (pModel->HitTest( Position)) return true;
   
   // --- inside ? -----
   if (HitTest( pModel->GetPositionV())) return true;
  
   // ----  test 8 vertex positions from this model----  
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos); if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   
   
   // ----  test 8 vertex positions from pModel----  
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   
   return false;
}

bool CModel::HitTestFront(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test front 9 vertex positions from this model---- 
   
   // --- front middle  
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- front right
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- front left
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}

bool CModel::HitTestBack(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test back 9 vertex positions from this model---- 
   
   // --- back middle  
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- back right
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- back left
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
	
}

bool CModel::HitTestLeft(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test left 9 vertex positions from this model---- 
   
   // --- left middle  
   pos.x=Position.x; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- left front
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- left back
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}

bool CModel::HitTestRight(CModel *pModel)
{
   CVector pos;
     
   pos = pModel->GetPositionV();
    
   float dist;
	
   dist=(float)sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   float radius1=GetRadius(); 
   float radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test left 9 vertex positions from this model---- 
   
   // --- right middle  
   pos.x=Position.x; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   // --- right front
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;

   //---- right back
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
   DoRotation( pos);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}

GLuint CModel::LoadTexture(string filename, CColor colorkey, bool usecolorkey)
{
	GLuint textureID = 0;

	SDL_Surface* image1 = IMG_Load(filename.c_str());
	SDL_Surface* image2 = NULL;
	if (image1 == NULL)
	{
		cout << "ERROR: " << filename << " could not be loaded" << endl;
		return false;
	}

	// create a new OpenGL texture handle
	glGenTextures(1, &textureID);

	// binding texture to handle
	glBindTexture(GL_TEXTURE_2D, textureID);

	// formatting and copying raw image data to OpenGL texture
	cout << "loading.. " << filename << ": " << image1->w << "x" << image1->h << "-" << (int)image1->format->BitsPerPixel << " ..ok" << endl;

	if (image1->format->BitsPerPixel < 32)
	{
		image2 = SDL_CreateRGBSurface(SDL_SWSURFACE, image1->w, image1->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000); // rgba
		if (usecolorkey)
			SDL_SetColorKey(image1, SDL_SRCCOLORKEY, SDL_MapRGB(image1->format, Uint8(255 * colorkey.R), Uint8(255 * colorkey.G), Uint8(255 * colorkey.B)));
		SDL_BlitSurface(image1, NULL, image2, NULL);
	}
	//flip image data using memcpy
	unsigned int* target = new unsigned int[image1->w * image1->h];
	unsigned int* source = NULL;

	if (image1->format->BitsPerPixel < 32) source = (unsigned int*)image2->pixels;
	else source = (unsigned int*)image1->pixels;

	for (int n = 0; n < image1->h; n++)
	{
		memcpy(&target[(image1->h - n - 1) * image1->w], &source[n * image1->w], 4 * image1->w);
	}

	// generate midmaps

	unsigned int x = (unsigned int)image1->w;
	unsigned int y = (unsigned int)image1->h;
	bool midmap = false;
	if ((x & (x - 1)) == 0 && (y & (y - 1)) == 0)
	{
		midmap = true;
	}
	if (midmap)
	{
		// generate midmap texture in the GL_RGBA8 format in graphics memory		 
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, image1->w, image1->h, GL_RGBA, GL_UNSIGNED_BYTE, target);
	}
	else
	{
	  // generate texture in the GL_RGBA8 format in graphics memory
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image1->w, image1->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, target);
    }
	if (image1->format->BitsPerPixel < 32) SDL_FreeSurface(image2);
	SDL_FreeSurface(image1);
	delete[] target;


	// linear filtering of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (midmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //GL_LINEAR_MIPMAP_LINEAR 
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// unbinding texture
	glBindTexture(GL_TEXTURE_2D, 0);

	filled = true;

	return textureID;
}

bool CModel::LoadObj(string filename, bool loadmaterial)
{
	streampos size;
	char* memblock;

	ifstream file(filename.c_str(), ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		// reading in data
		size = file.tellg();
		memblock = new char[(unsigned int)size];
		file.seekg(0, ios::beg);
		file.read(memblock, size);
		file.close();

		//cout << "the entire file is " << size << " bytes is in memory" << endl;

		// parsing data
		unsigned short numV, numT, numN;
		unsigned short numF;
		int positionlastline = 0;

		// setup data for reading in
		vector3f* vertices = new vector3f[65535];
		vector2f* texCoords = new vector2f[65535];
		vector3f* normals = new vector3f[65535];
		numV = numT = numN = numF = 0;

		// setup model group format
		numG = 0;
		groups[numG].format = 0; // 1=v,vt=3,vn=5,vtn=7;
		groups[numG].startIndex = 0;
		groups[numG].endIndex = 0;
		groups[numG].mat.name = "generic";
		groups[numG].mat.texture_filename = NULL;
		groups[numG].mat.texture_ID = 0;

		// setup model vertex, texture and normal data 
		// max 64k triangles/quads per model
		numTris = 0;
		vert = new vector3f[4 * 65535];
		tex = new vector2f[4 * 65535];
		norm = new vector3f[4 * 65535];


		for (int n = 0; n < size; n++)
		{
			// newline detected
			if (memblock[n] == '\n')
			{
				if (memblock[n + 1] == 'u')  // use new material create new group
				{
					//cout << "material " << decodeString(&memblock[n + 1]) << endl;
					char* matname = decodeString(&memblock[n + 1]);
					
						groups[numG].endIndex = numTris;
						numG++;
						groups[numG].format = 0; // 1=v,vt=3,vn=5,vtn=7;
						groups[numG].startIndex = numTris;
						groups[numG].endIndex = numTris;
						groups[numG].mat.name = matname;
						groups[numG].mat.texture_filename = NULL;
						groups[numG].mat.texture_ID = 0;
					
				}
				else if (memblock[n + 1] == 'm')  // material file
				{
					//cout << "material file " << decodeString(&memblock[n + 1]) << endl;
					materialFile.assign(decodeString(&memblock[n + 1]));
				}
				else if (memblock[n + 1] == 'o')  // new object (ignore)
				{
					//cout << "object " << decodeString(&memblock[n + 1]) << endl;
				}
				else if (memblock[n + 1] == 'g')  // new group (ignore)
				{
					//cout << "group " << decodeString(&memblock[n + 1]) << endl;
				}
				else if (memblock[n + 1] == 'v')
				{

					if (memblock[n + 2] == 0x20)  // space for new vertex coordinate v
					{
						numV++;
						char* pStart = &memblock[n + 2];
						char* pEnd;
						float f1, f2, f3;
						f1 = strtof(pStart, &pEnd);
						f2 = strtof(pEnd, &pEnd);
						f3 = strtof(pEnd, &pEnd);
						int steps = pEnd - &memblock[n + 2];
						n = n + steps;
						vertices[numV].x = f1; vertices[numV].y = f2; vertices[numV].z = f3;

					}
					else if (memblock[n + 2] == 't') // new texture coordinate vt
					{
						numT++;
						char* pStart = &memblock[n + 3];
						char* pEnd;
						float f1, f2;
						f1 = strtof(pStart, &pEnd);
						f2 = strtof(pEnd, &pEnd);
						int steps = pEnd - &memblock[n + 2];
						n = n + steps;
						texCoords[numT].x = f1; texCoords[numT].y = f2;
					}
					else if (memblock[n + 2] == 'n') // new normal coordinate vn
					{
						numN++;
						char* pStart = &memblock[n + 3];
						char* pEnd;
						float f1, f2, f3;
						f1 = strtof(pStart, &pEnd);
						f2 = strtof(pEnd, &pEnd);
						f3 = strtof(pEnd, &pEnd);
						int steps = pEnd - &memblock[n + 2];
						n = n + steps;
						normals[numN].x = f1; normals[numN].y = f2; normals[numN].z = f3;
					}
				}

				else if (memblock[n + 1] == 'f')  // new face
				{
					if (numF == 65535) continue;
					numF++;
					unsigned short values[64]; // format triplets v/t/n
					int steps = 0;
					unsigned short nofvalues = 1;
					memset(values, 0, 64 * sizeof(unsigned short));

					char* pStart = &memblock[n + 2];
					char* pEnd = &memblock[n + 2];
					// reading triplets
					for (unsigned short i = 0; i < 16; i++)
					{
						values[nofvalues] = (unsigned short)strtol(pStart, &pEnd, 10); //v
						pStart = &pEnd[1];
						if (values[nofvalues] == 0) { break; }
						else nofvalues++;
						if (pEnd[0] == '\r' || pEnd[0] == '\n')
						{
							values[nofvalues++] = 0; values[nofvalues++] = 0;
							break;
						}
						if (pEnd[0] == 0x20) // end of triplet
						{
							values[nofvalues++] = 0; values[nofvalues++] = 0;
							continue;
						}
						if (pEnd[0] == '/' && pEnd[1] == '/') // no texture value
						{
							values[nofvalues++] = 0; pStart = &pEnd[2];
							values[nofvalues++] = (unsigned short)strtol(pStart, &pEnd, 10); //n
							pStart = &pEnd[1];
							if (pEnd[0] == '\r' || pEnd[0] == '\n') break;
							else continue;
						}
						else  // read texture and/or normal
						{
							values[nofvalues++] = (unsigned short)strtol(pStart, &pEnd, 10); //t
							pStart = &pEnd[1];
							if (pEnd[0] == '\r' || pEnd[0] == '\n')
							{
								values[nofvalues++] = 0;
								break;
							}
							if (pEnd[0] == 0x20) // end of triplet
							{
								values[nofvalues++] = 0;
								continue;
							}
							else
							{
								values[nofvalues++] = (unsigned short)strtol(pStart, &pEnd, 10); //n
								pStart = &pEnd[1];
								if (pEnd[0] == '\r' || pEnd[0] == '\n') break;
								else continue;
							}
						}
					}
					values[0] = nofvalues - 1;
					steps = pEnd - &memblock[n + 2];
					n = n + steps - 1;

					if (values[0] == 0) continue; // no face values
					// --- decode face values ------

					if (groups[numG].format == 0)  // determine format for this group
					{
						// 1=v,vt=3,vn=5,vtn=7;
						groups[numG].format = 1;
						if (values[2] != 0) groups[numG].format = groups[numG].format | 2; //t
						if (values[3] != 0) groups[numG].format = groups[numG].format | 4; //n
					}

					// --- converting first triangle ----
					unsigned int i = numTris;
					vert[i++] = vertices[values[1]];
					vert[i++] = vertices[values[4]];
					vert[i++] = vertices[values[7]];
					numTris = i;

					if (groups[numG].format & 2) //t
					{
						i = i - 3;
						tex[i++] = texCoords[values[2]];
						tex[i++] = texCoords[values[5]];
						tex[i++] = texCoords[values[8]];
					}
					if (groups[numG].format & 4) //n
					{
						i = i - 3;
						norm[i++] = normals[values[3]];
						norm[i++] = normals[values[6]];
						norm[i++] = normals[values[9]];
					}

					if (values[0] > 9) // do triangulation
					{
						i = numTris;
						unsigned short triplets = (values[0] / 3) - 1;
						for (unsigned short triplet = 2; triplet < triplets; triplet++) // start with the second triplet
						{
							// --- copy triangle triplets ----
							vert[i++] = vertices[values[1]];
							vert[i++] = vertices[values[(3 * triplet) + 1]];
							vert[i++] = vertices[values[(3 * (triplet + 1)) + 1]];
							numTris = i;

							if (groups[numG].format & 2) //t
							{
								i = i - 3;
								tex[i++] = texCoords[values[2]];
								tex[i++] = texCoords[values[(3 * triplet) + 2]];
								tex[i++] = texCoords[values[(3 * (triplet + 1)) + 2]];
							}
							if (groups[numG].format & 4) //n
							{
								i = i - 3;
								norm[i++] = normals[values[3]];
								norm[i++] = normals[values[(3 * triplet) + 3]];
								norm[i++] = normals[values[(3 * (triplet + 1)) + 3]];
							}
						}

					}
				}

			}
		}

		// --- determine size of the model ---

		minx = 100000;
		maxx = -100000;
		miny = 100000;
		maxy = -100000;
		minz = 100000;
		maxz = -100000;

		for (unsigned short n = 1; n <= numV; n++)
		{
			minx = (vertices[n].x < minx) ? vertices[n].x : minx;
			miny = (vertices[n].y < miny) ? vertices[n].y : miny;
			minz = (vertices[n].z < minz) ? vertices[n].z : minz;
			maxx = (vertices[n].x > maxx) ? vertices[n].x : maxx;
			maxy = (vertices[n].y > maxy) ? vertices[n].y : maxy;
			maxz = (vertices[n].z > maxz) ? vertices[n].z : maxz;
		}

		cout << "loading.. " << filename << "-> vert: " << numV << " tex: " << numT << " norm: " << numN << " faces: " << numF << " tris: " << numTris / 3; 
		//cout << minx << " " << maxx << "," << miny << " " << maxy << "," << minz << " " << maxz << endl;

		delete[] memblock;
		delete[] vertices;
		delete[] texCoords;
		delete[] normals;

		// todo: trim group data arrays
		vector3f* tempV = vert;
		vector2f* tempT = tex;
		vector3f* tempN = norm;

		vert = new vector3f[numTris];
		memcpy(vert, tempV, numTris * sizeof(vector3f));

		norm = new vector3f[numTris];
		memcpy(norm, tempN, numTris * sizeof(vector3f));

		groups[numG].endIndex = numTris;

		// need to calculate new normals if not provided
		if (numN == 0) CalculateNormals();

		if (numT > 0) // has t?
		{
			tex = new vector2f[numTris];
			memcpy(tex, tempT, numTris * sizeof(vector2f));
		}
		else tex = NULL;

		numG++;

		cout << " groups: " << (int)numG << endl;

		delete[] tempV;
		delete[] tempT;
		delete[] tempN;

		// we use tris not vertices
		numTris = numTris / 3;

		if (materialFile.size() > 0)
		{
			material* materials = new material[256];
			unsigned char numMat = 0;
			// extract directory name
			int diroffset = (int)filename.find_last_of("/");
			if (diroffset > 0) materialFile.insert(0, filename.substr(0, diroffset + 1));
			else materialFile.insert(0, "models/");

			if (loadmaterial) numMat = LoadMtl(materialFile, materials);
			else numMat = LoadMtl("", materials); // just use default material
			// assign materials to groups
			AssignMtl(materials, numMat, groups, numG);
			// clean up materials
			delete[] materials;
		}
		//else we just use the default colour and material
      return true;
	}
	cout << "ERROR: Unable to open file: " << filename << endl;
	return false;
}

void CModel::AssignMtl(material* materials, unsigned char numMat, group* groups, unsigned char numG)
{
	// assign materials to groups
	for (unsigned char g = 0; g < numG; g++)
	{
		bool unassigned = true;
		// find matching materials and assign those
		for (unsigned char m = 0; m < numMat; m++)
		{
			if (strcmp((groups[g].mat).name, materials[m].name) == 0)
			{
				//cout << "match " << materials[m].name << endl;

				memcpy(&groups[g], &materials[m], sizeof(material));
				unassigned = false;
			}
		}
		if (unassigned)
		{
			//cout << "unassigned " << groups[g].mat.name << endl;
			memcpy(&groups[g], &materials[0], sizeof(material));

		}
	}
}

unsigned char CModel::LoadMtl(string filename, material* materials)
{
	unsigned char numMat = 0;
	//material = new material*[256];
	materials[0].name = "generic";
	materials[0].texture_filename = NULL;
	materials[0].texture_ID = 0;
	materials[0].Kd[0] = 0.8f; materials[0].Kd[1] = 0.8f; materials[0].Kd[2] = 0.8f; materials[0].Kd[3] = 1.0f;
	materials[0].Ks[0] = 0; materials[0].Ks[1] = 0; materials[0].Ks[2] = 0; materials[0].Ks[3] = 1.0f;
	materials[0].Ka[0] = 0.2f; materials[0].Ka[1] = 0.2f; materials[0].Ka[2] = 0.2f; materials[0].Ka[3] = 1.0f;

	if (filename.size() < 3) return numMat;

	char str1[256];
	float f1, f2, f3;

	ifstream inFile;
	string str;

	inFile.open(filename.c_str(), ios::in);

	// ---- determining array sizes ------
	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			getline(inFile, str); // read a line of text into string str

			if (strstr(str.c_str(), "newmtl") != NULL) // new material found
			{
				numMat++;
				materials[numMat].texture_filename = NULL;
				materials[numMat].texture_ID = 0;
				materials[numMat].Kd[0] = 0.8f; materials[numMat].Kd[1] = 0.8f; materials[numMat].Kd[2] = 0.8f; materials[numMat].Kd[3] = 1.0f;
				materials[numMat].Ks[0] = 0; materials[numMat].Ks[1] = 0; materials[numMat].Ks[2] = 0; materials[numMat].Ks[3] = 1.0f;
				materials[numMat].Ka[0] = 0.2f; materials[numMat].Ka[1] = 0.2f; materials[numMat].Ka[2] = 0.2f; materials[numMat].Ka[3] = 1.0f;
				materials[numMat].name = new char[256];
				sscanf_s(str.c_str(), "%s %s", str1, 256, materials[numMat].name, 256);
				//cout << materials[numMat].name << endl;
			}
			else if (strstr(str.c_str(), "map_Kd") != NULL) // new material found
			{
				materials[numMat].texture_filename = new char[256];
				
				
				// remove directories from filename
				const char* txt = str.c_str();
				int n = (int)str.length();
				for (n = (int)str.length(); n >= 0; n--)
				{
					if (txt[n] == '\\' || txt[n] == '/' || txt[n] == ' ') break;
				}
				memcpy(materials[numMat].texture_filename, &txt[n + 1], str.length() - n);
				
				sscanf_s(str.c_str(), "%s %s", str1, 256, materials[numMat].texture_filename, 256);
				//cout << materials[numMat].texture_filename << endl;
			}
			else if (strstr(str.c_str(), "Kd") != NULL)
			{
				sscanf_s(str.c_str(), " %s %f %f %f", str1, 256, &f1, &f2, &f3);
				f1 = (f1 < 0.0f) ? 0.0f : f1;
				f1 = (f1 > 1.0f) ? 1.0f : f1;
				f2 = (f2 < 0.0f) ? 0.0f : f2;
				f2 = (f2 > 1.0f) ? 1.0f : f2;
				f3 = (f3 < 0.0f) ? 0.0f : f3;
				f3 = (f3 > 1.0f) ? 1.0f : f3;
				materials[numMat].Kd[0] = f1; materials[numMat].Kd[1] = f2; materials[numMat].Kd[2] = f3; materials[numMat].Kd[3] = 1.0f;
				//cout << "Kd: " << f1 << " " << f2 << " " << f3 << endl;
			}
			else if (strstr(str.c_str(), "Ks") != NULL)
			{
				sscanf_s(str.c_str(), " %s %f %f %f", str1, 256, &f1, &f2, &f3);
				f1 = (f1 < 0.0f) ? 0.0f : f1;
				f1 = (f1 > 1.0f) ? 1.0f : f1;
				f2 = (f2 < 0.0f) ? 0.0f : f2;
				f2 = (f2 > 1.0f) ? 1.0f : f2;
				f3 = (f3 < 0.0f) ? 0.0f : f3;
				f3 = (f3 > 1.0f) ? 1.0f : f3;
				materials[numMat].Ks[0] = f1; materials[numMat].Ks[1] = f2; materials[numMat].Ks[2] = f3; materials[numMat].Ks[3] = 1.0f;
				//cout << "Ks: " << f1 << " " << f2 << " " << f3 << endl;
			}
			else if (strstr(str.c_str(), "Ka") != NULL)
			{
				sscanf_s(str.c_str(), " %s %f %f %f", str1, 256, &f1, &f2, &f3);
				f1 = (f1 < 0.0f) ? 0.0f : f1;
				f1 = (f1 > 1.0f) ? 1.0f : f1;
				f2 = (f2 < 0.0f) ? 0.0f : f2;
				f2 = (f2 > 1.0f) ? 1.0f : f2;
				f3 = (f3 < 0.0f) ? 0.0f : f3;
				f3 = (f3 > 1.0f) ? 1.0f : f3;
				materials[numMat].Ka[0] = f1; materials[numMat].Ka[1] = f2; materials[numMat].Ka[2] = f3; materials[numMat].Ka[3] = 1.0f;
				//cout << "Ka: " << f1 << " " << f2 << " " << f3 << endl;
			}
			//cout << str << endl;
		}
		inFile.close();

		// loading textures
		int diroffset = (int)filename.find_last_of("/");

		for (unsigned char n = 0; n <= numMat; n++)
		{
			if (materials[n].texture_filename != NULL)
			{
				string texname(materials[n].texture_filename);
				if (diroffset > 0) texname.insert(0, filename.substr(0, diroffset + 1));
				else texname.insert(0, "models/");

				materials[n].texture_ID=LoadTexture(texname, CColor::Black(), false);
				//cout << "loading " << materials[n].texture_filename << endl;
			}
		}
	}
	else cout << "ERROR: material file " << filename << " not found" << endl;
	return ++numMat;
}









