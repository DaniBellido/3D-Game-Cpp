
#include "Floor.h"

void CFloor::LoadTexture(string filename)
{
	string fname = "images/" + filename;
	
	TextureID = 0;

	SDL_Surface* image1 = IMG_Load(fname.c_str());
	SDL_Surface* image2 = NULL;
	if (image1 == NULL)
	{
		cout << "ERROR: " << fname << " could not be loaded from the images folder" << endl;
		return;
	}

	// create a new OpenGL texture handle
	glGenTextures(1, &TextureID);

	// binding texture to handle
	glBindTexture(GL_TEXTURE_2D, TextureID);

	// formatting and copying raw image data to OpenGL texture
	cout << "loading.. " << fname << ": " << image1->w << "x" << image1->h << "-" << (int)image1->format->BitsPerPixel << " ..ok" << endl;

	if (image1->format->BitsPerPixel < 32)
	{
		image2 = SDL_CreateRGBSurface(SDL_SWSURFACE, image1->w, image1->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000); // rgba
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

	// TODO: generate midmaps
	width = image1->w; height = image1->h;
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

	Color.Set( 1.0, 1.0, 1.0, 1.0);
}


void CFloor::Draw(CGraphics* g)
{
  float texXR=1.0f, texYR=1.0f;
    
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glPushMatrix();
      glTranslatef( Position.x-Width/2.0f, Position.y-1, Position.z-Depth/2.0f);  	// transformation to world coordinates
      
    if (TextureID)
    {
	 glColor4f(1.0,1.0,1.0, 1.0);
	
	 if (tiling)
	 {
	  texXR=float(Width)/width;
	  texYR=float(Depth)/height;
     }
     
	 glBindTexture( GL_TEXTURE_2D, TextureID);  // binding texture
     
	 glBegin(GL_QUADS);
      glNormal3f(0,0,0);

	  //glTexCoord2f(0.0, texYR); glVertex3f(-Width/2, Position.y, -Depth/2);
	  //glTexCoord2f(texXR, texYR); glVertex3f( Width/2, Position.y, -Depth/2);
	  //glTexCoord2f(texXR, 0.0); glVertex3f( Width/2, Position.y,  Depth/2);
	  //glTexCoord2f(0.0, 0.0); glVertex3f(-Width/2, Position.y,  Depth/2);
	  
	  // hard coding size of a square
	  int nx = Width / 100;
	  int nz = Depth / 100;

	  float dx = 1.0f/nx;
	  float dy = 1.0f/nz;

	  float tx =0, ty = 0;
	  float vx = 0, vz = 0;

	  for (int  x = 0; x < nx; x++)
	  {
		  vx = x * 100;
		  for (int z = 0; z < nz; z++)
		  {
			  vz = z * 100;
			  glTexCoord2f(tx*texXR, ty*texYR); glVertex3f(vx, Position.y, vz);
			  glTexCoord2f(tx * texXR, (ty+dy) * texYR); glVertex3f(vx, Position.y, vz+100);
			  glTexCoord2f((tx+dx) * texXR, (ty+dy) * texYR); glVertex3f(vx+100, Position.y, vz+100);
			  glTexCoord2f((tx+dx) * texXR, ty * texYR); glVertex3f(vx+100, Position.y, vz);
			  ty += dy;
		  }
		  tx += dx;
		  ty = 0;
	  }
     glEnd();
	 glBindTexture( GL_TEXTURE_2D, 0);
	}
	else
	{

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(Color.R, Color.G, Color.B);
     glBegin(GL_QUADS);
     glNormal3f(0,0,0);
	  glVertex3f(0, Position.y, 0);
	  glVertex3f( Width, Position.y, 0);
	  glVertex3f( Width, Position.y,  Depth);
	  glVertex3f(0, Position.y,  Depth);
     glEnd();
     
    }
  glPopMatrix();
  
  if (drawgrid)
  {
  glColor3f( 0.7f, 0.7f, 0.7f);  // the grid is grey
  glLineWidth(1.0f);

  glPushMatrix();
    glTranslatef( Position.x, Position.y+3, Position.z);  	// transformation to world coordinates	
	
	 glBegin(GL_LINES);
	 glNormal3f(0,0,0);
     for (float n=-Width/2; n <= Width/2; n=n+100)
     {
	   glVertex3f( n, Position.y, -Depth/2); 
	   glVertex3f( n, Position.y, Depth/2); 
	 }
	 
	 for (float n=-Depth/2; n <= Depth/2; n=n+100)
     {
	   glVertex3f( -Width/2, Position.y, n); 
	   glVertex3f( Width/2, Position.y, n); 
	 }
	glEnd();
	glPopMatrix();
   }
}



