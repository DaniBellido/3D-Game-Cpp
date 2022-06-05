
#include "Sprite.h"


CSprite::CSprite()
{
	Init();
}


CSprite::CSprite( float x, float y, CSprite &sprite, Uint32 time)
{
	Init();
	
	Xpos = x; Ypos = y;	
	
	// cloning appearance of existing sprite
	Width = sprite.Width;
	Height = sprite.Height;	
    ScaleBB = sprite.ScaleBB;

    TextureID=sprite.GetTextureID();
	filled=sprite.filled;
	
	// animation
	numFrames = sprite.numFrames;
	currentFrame = sprite.currentFrame;
	period = sprite.currentFrame; 
	
	looping = sprite.looping;
	startFrame = sprite.startFrame;
	stopFrame = sprite.stopFrame;
	
	// motion 
	Speed = sprite.Speed;
	Direction = sprite.Direction;

	// Rotation
	Rotation = sprite.Rotation;
	Omega = sprite.Omega;

	SetColor( sprite.Color);

	Status = sprite.Status;
	Health = sprite.Health;
	
	// Time
	SpriteTime=time;
	isCloned = true;
}

void CSprite::Init()
{
	Xpos = Ypos = 0.0f;
	Width = Height = 1.0f;
	ScaleBB = 1.0f;

	numFrames = 1; currentFrame = 1.0f; period = 0.0f; looping = false;
	startFrame = stopFrame = 1;

	Color.Set(1.0, 0.0, 0.0, 1.0); // red
	filled = true;
	TextureID = 0;

	// Time
	SpriteTime = 0;

	Health = 100;	Status = 0;

	// Linear Motion
	Speed = 0;
	Direction = 0;

	// Rotation
	Rotation = 0;
	Omega = 0;

	marked_for_removal = false;
	dying = 0;
	isCloned = false;

}

void CSprite::Clear()
{
	if (!isCloned)
	{
	  glDeleteTextures(1, &TextureID);
    }


}

void CSprite::SetSize(float w, float h)
{
	Width = w; Height = h;
}

void CSprite::SetScale( float scale)
{
	if (scale > 0)
	{
	 // size changes
	  Width= Width*scale;
	  Height=Height*scale;
	}	
} 

bool CSprite::LoadImage(string filename, CColor colorkey, bool usecolorkey, int frames)
{
	string fname = "images/" + filename;
	
	SDL_Surface* image1 = IMG_Load(fname.c_str());
	SDL_Surface* image2 = NULL;
	if (image1 == NULL)
	{
		cout << "ERROR: " << filename << "is of an unsupported file format" << endl;
		return false;
	}

	Width = float(image1->w/frames); Height = float(image1->h);
	numFrames = frames;
	currentFrame = 1.0f; period = 0.0f; looping = false;
	startFrame = stopFrame = 1;

	// create a new OpenGL texture handle
	glGenTextures(1, &TextureID);

	// binding texture to handle
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// formatting and copying raw image data to OpenGL texture
	cout << "loading.. "<< filename << ": " << image1->w << "x" << image1->h << "-" << (int)image1->format->BitsPerPixel << " ..ok" << endl;

	if (image1->format->BitsPerPixel < 32)
	{
		image2 = SDL_CreateRGBSurface(SDL_SWSURFACE, image1->w, image1->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000); // rgba
		if (usecolorkey)
		   SDL_SetColorKey(image1, SDL_SRCCOLORKEY, SDL_MapRGB(image1->format, Uint8(255*colorkey.R), Uint8(255 * colorkey.G), Uint8(255 * colorkey.B)));
		SDL_BlitSurface(image1, NULL, image2, NULL);
	}
	  //flip image data using memcpy
	  unsigned int* target = new unsigned int[image1->w * image1->h];
	  unsigned int* source = NULL;
	  
	  if (image1->format->BitsPerPixel < 32) source = (unsigned int*)image2->pixels;
	  else source = (unsigned int*)image1->pixels;

	  for (int n = 0; n < image1->h; n++)
	  {
		memcpy(&target[(image1->h-n-1)*image1->w], &source[n*image1->w], 4 * image1->w);
	  }

	// generate texture in the GL_RGBA8 format in graphics memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image1->w, image1->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, target);
	if (image1->format->BitsPerPixel < 32) SDL_FreeSurface(image2);
	SDL_FreeSurface(image1);
	delete[] target;
		
	// linear filtering of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// unbinding texture
	glBindTexture(GL_TEXTURE_2D, 0);

	Color.Set( 1.0, 1.0, 1.0, 1.0); // White
	
	filled=true;

	return true;

}



float CSprite::GetRadius()
{
   return (float)sqrt( (Width/2.0)*(Width/2.0)+(Height/2.0)*(Height/2.0));
}


// ----------  Collision detection -----------


// true if the distance between this sprite and the position (x,y) is less than the specified minDistance 
bool CSprite::HitTest(float x, float y, float minDistance)
{
	float distance=(float)sqrt((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos));
	
	if (distance < minDistance) return true;
	return false;
}


// true if the position (x,y) is inside the bounding box of this sprite
bool CSprite::HitTest(float x, float y)
{
    float x1=-ScaleBB*Width/2.0f;
    float x2=ScaleBB*Width/2.0f;
	float y1=-ScaleBB*Height/2.0f;
	float y2=ScaleBB*Height/2.0f;

	float xp,yp;
	   
	// correcting for bounding box rotation and position
    xp = (x-Xpos)*(float)cos(DEG2RAD (-Rotation)) - (y-Ypos)*(float)sin(DEG2RAD (-Rotation));
	yp = (x-Xpos)*(float)sin(DEG2RAD (-Rotation)) + (y-Ypos)*(float)cos(DEG2RAD (-Rotation));

    if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2) return true;
	return false;
}


// are bounding rectangles overlapping?
	
	bool CSprite::HitTest(CSprite *pSprite) 
    { 
       float Xpos2=pSprite->GetX();
	   float Ypos2=pSprite->GetY();
		
       float distance=(float)sqrt((Xpos2-Xpos)*(Xpos2-Xpos)+(Ypos2-Ypos)*(Ypos2-Ypos));
	   float radius1=ScaleBB*GetRadius(); 
	   float radius2=pSprite->ScaleBB*pSprite->GetRadius();

       // sprites are too far apart and could not possibly overlap
	   if (distance > radius1+radius2) return false;

	   // centre of pSprite inside this sprite?
       if ( HitTest( Xpos2, Ypos2)) return true;
       
       // centre of pSprite inside this sprite?
       if ( pSprite->HitTest( Xpos, Ypos)) return true;
       
       // ----- 4 bounding points of this sprite inside pSprite? ----
         
       float x1=-ScaleBB*Width/2.0f;
	   float x2=ScaleBB*Width/2.0f;
	   float y1=-ScaleBB*Height/2.0f;
	   float y2=ScaleBB*Height/2.0f;
     
       // top left hand corner    
       Xpos2 = Xpos + x1*(float)cos(DEG2RAD (Rotation)) - y2*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x1*(float)sin(DEG2RAD (Rotation)) + y2*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // top right hand corner 
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y2*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y2*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y1*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y1*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // bottom left hand corner   
       Xpos2 = Xpos + x1*(float)cos(DEG2RAD (Rotation)) - y1*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x1*(float)sin(DEG2RAD (Rotation)) + y1*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;

       
       // ----- 4 bounding points of pSprite inside this sprite? ----
         
       x1=-pSprite->ScaleBB*pSprite->Width/2.0f;
	   x2=pSprite->ScaleBB*pSprite->Width/2.0f;
	   y1=-pSprite->ScaleBB*pSprite->Height/2.0f;
	   y2=pSprite->ScaleBB*pSprite->Height/2.0f;
         
         
       // top left hand corner    
       Xpos2 = pSprite->Xpos + x1*(float)cos(DEG2RAD (pSprite->Rotation)) - y2*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x1*(float)sin(DEG2RAD (pSprite->Rotation)) + y2*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
       // top right hand corner 
       Xpos2 = pSprite->Xpos + x2*(float)cos(DEG2RAD (pSprite->Rotation)) - y2*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x2*(float)sin(DEG2RAD (pSprite->Rotation)) + y2*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = pSprite->Xpos + x2*(float)cos(DEG2RAD (pSprite->Rotation)) - y1*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x2*(float)sin(DEG2RAD (pSprite->Rotation)) + y1*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
       // bottom left hand corner   
       Xpos2 = pSprite->Xpos + x1*(float)cos(DEG2RAD (pSprite->Rotation)) - y1*(float)sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + x1*(float)sin(DEG2RAD (pSprite->Rotation)) + y1*(float)cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( Xpos2, Ypos2)) return true;
      
       return false;
    }

	bool CSprite::HitTestFront(CSprite* pSprite)
	{
	   float Xpos2=pSprite->GetX();
	   float Ypos2=pSprite->GetY();
		
       float distance=(float)sqrt((Xpos2-Xpos)*(Xpos2-Xpos)+(Ypos2-Ypos)*(Ypos2-Ypos));
	   float radius1=ScaleBB*GetRadius(); 
	   float radius2=pSprite->ScaleBB*pSprite->GetRadius();

       // sprites are too far apart and could not possibly overlap
	   if (distance > radius1+radius2) return false;

	   // centre of pSprite inside this sprite?
       if ( HitTest( Xpos2, Ypos2)) return true;
       
       // centre of pSprite inside this sprite?
       if ( pSprite->HitTest( Xpos, Ypos)) return true;
       
       // ----- 3 forward in x-direction (right) bounding points of this sprite inside pSprite? ----
         
	   float x2=ScaleBB*Width/2.0f;
	   
	   float y1=-ScaleBB*Height/2.0f;
	   float y2=ScaleBB*Height/2.0f;
	   float y3=0.0f;
	  
	   
	   // top right hand corner 
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y2*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y2*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y1*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y1*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       // middle right had side
       Xpos2 = Xpos + x2*(float)cos(DEG2RAD (Rotation)) - y3*(float)sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + x2*(float)sin(DEG2RAD (Rotation)) + y3*(float)cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( Xpos2, Ypos2)) return true;
       
       return false;

	}



// -----------    Update -----------------

void CSprite::Update(Uint32 GameTime)
{
	if (SpriteTime == 0) { SpriteTime = GameTime; return; }

	int deltaTime = GameTime - SpriteTime;
	if ( deltaTime < 0) deltaTime = 0;
	
	if (dying > 0) dying=dying-deltaTime;

	// linear motion update
    float dx = Speed*(float)cos( DEG2RAD (Direction))*deltaTime/1000.0f;
	float dy = Speed*(float)sin( DEG2RAD (Direction))*deltaTime/1000.0f;

    Xpos += dx; Ypos += dy;
	
	// rotation update
	Rotate( Omega*deltaTime/1000.0f);
	
	// animation update
	if (period > 0)
	{
	  currentFrame=currentFrame+deltaTime*period/1000.0f;
	  if (currentFrame >= stopFrame+1) 
	  { 
		if (looping) currentFrame=(float)startFrame;
		else SetFrame( stopFrame); 
	  }
    }
    
    if (dying)
    {
		if (IsAnimationFinished()) Delete();
	}
    
    SpriteTime = GameTime;
}


void CSprite::Move( float distance)
{
      Xpos += distance*(float)cos( DEG2RAD (Direction));
	  Ypos += distance*(float)sin( DEG2RAD (Direction));
}

void CSprite::PlayAnimation( int start, int stop, float speed, bool loop)
{
	  if (currentFrame <= start || currentFrame >= stop+1 || period == 0)
	  {
	    startFrame=start; stopFrame=stop; period=speed;
	    currentFrame = (float)start; looping=loop;
	  }
	
}


void CSprite::Draw(CGraphics* g)  
{
    if (TextureID  <  1)
	{
    // ----------  If no texture assigned draw a green bounding with red cross --------
    glPolygonMode( GL_FRONT, GL_LINE);
     glColor3f( 0 , 1.0f, 0);
     glPushMatrix();
		glTranslatef( Xpos, Ypos, 0);  // transformation to world coordinates
		glBegin(GL_QUADS);
	  		glVertex3f(-10.0f, -10.0f, 0);
	  		glVertex3f( 10.0f, -10.0f, 0);
	  		glVertex3f( 10.0f,  10.0f, 0);
	  		glVertex3f( -10.0f,  10.0f, 0);
		glEnd();
		glColor3f(1.0f , 0, 0);
		glBegin(GL_LINES);
	  	  glVertex3f( -10.0f, 10.0f, 0);
	      glVertex3f( 10.0f, -10.0f, 0);
	      glVertex3f( -10.0f, -10.0f, 0);
	      glVertex3f( 10.0f, 10.0f, 0);
		glEnd();
		
	 glPopMatrix();
	 glPolygonMode( GL_FRONT, GL_FILL);
	 
	}
	else  // draw texture as a rectangular sprite
	{
        glColor4f( Color.R, Color.G, Color.B, Color.A);
		// binding texture
   		glBindTexture( GL_TEXTURE_2D, TextureID);  

   		glPushMatrix();
   		glTranslatef( Xpos, Ypos, 0);    // transformation to world coordinates
		glRotatef( Rotation, 0, 0, 1 );   // rotation in degrees
		glScalef(Width, Height, 0); // scale sprite rectangle to size
	
	   if (numFrames > 1)  // animated texture: select the correct frame
	   {
		float dx=1.0f/numFrames;
		glBegin(GL_QUADS);
			glTexCoord2f(float(((int)currentFrame-1)*dx), 0.0f); glVertex3f( -0.5f, -0.5f,   0);
			glTexCoord2f(float((int)currentFrame*dx), 0.0f);     glVertex3f(  0.5f, -0.5f,   0);
			glTexCoord2f(float((int)currentFrame*dx), 1.0f);     glVertex3f(  0.5f,  0.5f,   0);
			glTexCoord2f(float(((int)currentFrame-1)*dx), 1.0f); glVertex3f( -0.5f,  0.5f,   0);
		glEnd();	
	   }
	   else  // no anmimated texture
	   {
		glBegin(GL_QUADS);
	  		glTexCoord2f(0.0f, 0.0f); glVertex3f( -0.5f, -0.5f,   0);
	  		glTexCoord2f(1.0f, 0.0f); glVertex3f(  0.5f, -0.5f,   0);
	  		glTexCoord2f(1.0f, 1.0f); glVertex3f(  0.5f,  0.5f,   0);
	  		glTexCoord2f(0.0f, 1.0f); glVertex3f( -0.5f,  0.5f,   0);
		glEnd();
		
	   }
    	glPopMatrix();	

  		glBindTexture( GL_TEXTURE_2D, 0);
	
	}
	
	// ---- draw red bounding box -----
	if (g->IsDebug())
	{
		glColor3f(1.0f,0,0); // red
		
		glPolygonMode( GL_FRONT, GL_LINE);
	
    	glPushMatrix();
		  glTranslatef( Xpos, Ypos, 0);  	// transformation to world coordinates
		  glRotatef( Rotation, 0, 0, 1 );   // rotation in degrees
		  glScalef(ScaleBB*Width, ScaleBB*Height,0); // scale bounding box to size
		  
		  glBegin(GL_QUADS);
		   glVertex3f(-0.5f, -0.5f, 0);
		   glVertex3f( 0.5f, -0.5f, 0);
		   glVertex3f( 0.5f,  0.5f, 0);
		   glVertex3f(-0.5f,  0.5f, 0);
		  glEnd();
		  
		glPopMatrix();
		glPolygonMode( GL_FRONT, GL_FILL);
	}
}

CSprite* CSprite::Clone()
{
	CSprite* s = new CSprite();

	Xpos = s->Xpos; Ypos = s->Ypos;

	// cloning appearance of existing sprite
	Width = s->Width;
	Height = s->Height;
	ScaleBB = s->ScaleBB;

	TextureID = s->GetTextureID();
	filled = s->filled;

	// animation
	numFrames = s->numFrames;
	currentFrame = s->currentFrame;
	period = s->currentFrame;

	looping = s->looping;
	startFrame = s->startFrame;
	stopFrame = s->stopFrame;

	// motion 
	Speed = s->Speed;
	Direction = s->Direction;

	// Rotation
	Rotation = s->Rotation;
	Omega = s->Omega;

	SetColor(s->Color);

	Status = s->Status;
	Health = s->Health;

	// Time
	SpriteTime = s->SpriteTime;
	isCloned = true;

	return s;
}
