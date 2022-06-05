
#include "Texture.h"

	
bool CTexture::smooth=false;	
	
bool CTexture::LoadTexture( string filename, const CColor& color, int frames)
{

  unsigned char* TexData=NULL;  // raw texture data
  
  // SDL_Surface* TexData IMG_Load(filename)
  
  
  TranspColor=color;  // assign transparency colour
  
  // --- identify image type from file extension ----
  
  int length=filename.length();
  
  if (length < 5) 
  {
	  cout << "filename requires extension " << endl;
	  return false;
  }

  // bmp file type
  if ((filename[length-3]=='b' && filename[length-2]=='m' && filename[length-1]=='p') || (filename[length-3]=='B' && filename[length-2]=='M' && filename[length-1]=='P'))
  {
	  cout << "loading.. " << filename;
	  if ( (TexData=LoadFromBMP( filename.c_str())) == NULL ) return false;
	  else cout << "..ok" << endl;
  }
  
   // tga file type
  else if ((filename[length-3]=='t' && filename[length-2]=='g' && filename[length-1]=='a') || (filename[length-3]=='T' && filename[length-2]=='G' && filename[length-1]=='A'))
  {
	  cout << "loading.. " << filename;
	  if ( (TexData=LoadFromTGA( filename.c_str())) == NULL ) return false;
	  else cout << "..ok" << endl;
  }
  else 
  {
	cout << "ERROR: Wrong file format. Only 24-bit bmp and 24,32-bit tga files are supported " << filename << endl;
	return false;   
  }
  
 
 
	// delete old texture from graphics memory to replace it
	if (TextureID > 0) { glDeleteTextures( 1, &TextureID); TextureID=0; }
 

    // create a new OpenGL texture handle
    glGenTextures( 1, &TextureID );

    // binding texture to handle
    glBindTexture( GL_TEXTURE_2D, TextureID);
    
    unsigned int x=(unsigned int)Width;
    unsigned int y=(unsigned int)Height;
    bool midmap=false;
    if ((x & (x - 1)) == 0 && (y & (y - 1))==0)
    {
      if (smooth) midmap=true;  
    }
    
     if (midmap)
     {
		// generate midmap texture in the GL_RGBA4 format in graphics memory		 
	    gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA8, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, TexData );  
	 }
	 else
	 {
		// generate texture in the GL_RGBA4 format in graphics memory
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexData ); 
	 } 
    
    // linear filtering of texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    if (midmap) glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); //GL_LINEAR_MIPMAP_LINEAR 
    else glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	
	// unbinding current texture
	glBindTexture( GL_TEXTURE_2D, 0);
   
 
    delete TexData; // delete raw texture data
  
    Width=Width/frames;
	
	Frames=frames; frame=1;

  return true;

}

// position inside this texture?
bool CTexture::HitTest(int x, int y)
{
	if (x > Xpos-Width/2 && x < Xpos+Width/2 && y > Ypos-Height/2 && y < Ypos+Height/2) return true;
	return false;
}

void CTexture::Draw(int x, int y)
{
   Xpos=x; Ypos=y;
   // binding texture
   glBindTexture( GL_TEXTURE_2D, TextureID);  

   glColor4f( 1.0, 1.0, 1.0, 1.0);

   glPushMatrix();
   glTranslatef( (float)x, (float)y, 0);    // transformation to world coordinates
	
	if (Frames > 1)
	{
	  float dx=1.0f/Frames;
	  
	  // todo: select correct texture coordinates
	  glBegin(GL_QUADS);
	    glTexCoord2f((frame-1)*dx, 0.0); glVertex3f(float(-Width/2), float(-Height/2),   0);
	    glTexCoord2f(frame*dx, 0.0); glVertex3f( float(Width/2), float(-Height/2),   0);
	    glTexCoord2f(frame*dx, 1.0); glVertex3f( float(Width/2),  float(Height/2),   0);
	    glTexCoord2f((frame-1)*dx, 1.0); glVertex3f(float(-Width/2),  float(Height/2),   0);
	  glEnd();	
	}
	else
	{
	  glBegin(GL_QUADS);
	    glTexCoord2f(0.0, 0.0); glVertex3f(float(-Width/2), float(-Height/2),   0);
	    glTexCoord2f(1.0, 0.0); glVertex3f( float(Width/2), float(-Height/2),   0);
	    glTexCoord2f(1.0, 1.0); glVertex3f( float(Width/2),  float(Height/2),   0);
	    glTexCoord2f(0.0, 1.0); glVertex3f(float(-Width/2),  float(Height/2),   0);
	  glEnd();
    }
    glPopMatrix();	

  glBindTexture( GL_TEXTURE_2D, 0);
  
}


unsigned char* CTexture::LoadFromBMP(const char* filename)
{

  FILE* stream;
  unsigned char* TexData=NULL;
  unsigned char* row=NULL;
  unsigned char bft[3];  
  int biSize;               
  int biBitCount;           
  int biCompression;
  int padding = 0;
  int x,y,index=0;
  int overage;

  if ((stream = fopen( filename, "rb")) != NULL)
  {
   // extract file header
   fread( &bft, 1, 2, stream);
   if ( bft[0] != 'B' || bft[1] != 'M' ) { fclose(stream); return NULL; }
   readWord( stream, 12);
   // extract bitmap header
	  biSize = readWord( stream, 4);
    if ( biSize == 12 ) {   // OS/2 style bitmap
    Width = readWord( stream, 2);
    Height = readWord( stream, 2);
    readWord( stream, 2);
    biBitCount = readWord( stream, 2);
	if (biBitCount < 24 ) 
	{ 
		cout << " ..ERROR:" << endl;
        cout << "Texture must be a 24-bit or 32-bit bitmap file" << endl;
		fclose(stream); 
		return NULL; 
	}
    }
	else
    {                      // Windows style bitmap
    Width = readWord( stream, 4);
    Height = readWord( stream, 4);
    readWord( stream, 2);
    biBitCount = readWord( stream, 2);
    biCompression = readWord( stream, 4);
    readWord( stream, 12);
    readWord( stream, 4);
    readWord( stream, 4);

	// we don't support compressed or 8 bit images
     if (biBitCount < 24 || biCompression != 0) 
     { 
		cout << " ..ERROR:" << endl;
        cout << "Texture must be a 24-bit or 32-bit uncompressed bitmap file" << endl;
		fclose(stream); 
		return NULL; 
	 }
    }
	
    // ---- extract image data ----

    if (biBitCount == 24)
	{
     overage = (3*Width) % 4;
     if ( overage != 0 ) padding = 4 - overage;

     // read data row by row and convert to RGBA 
     row = new unsigned char[(3*Width)+padding];

     // RGBA texture data buffer
     TexData = new unsigned char[ 4*Width*Height]; 
      
    unsigned char R=(unsigned char)TranspColor.R*255, G=(unsigned char)TranspColor.G*255, B=(unsigned char)TranspColor.B*255; 
     /* read pixel data in rows
	and convert them into RGBA texture format with colour 0 being the transparent value
     */
  
    index=0;
	// 24 bit RGB bitmap
    for ( y = 0; y < Height; y++ )
    {
      index=y*4*Width;
      fread(row,1,(3*Width)+padding,stream);
      for ( x = 0; x < 3*Width; x=x+3 ) 
	  {
		  TexData[index++]=row[x+2];
		  TexData[index++]=row[x+1];
		  TexData[index++]=row[x];
		  if (TranspColor.A < 0) TexData[index]=255;
		  else if (row[x+2]==R && row[x+1]==G && row[x]==B) TexData[index]=0;
		  else TexData[index]=255;
		  index++;
	  }
    }
   }
   else if (biBitCount == 32) // 32 bit RGBA bitmap
   {
     // read data row by row and convert to RGBA
     row = new unsigned char[(4*Width)];

     // RGBA texture data buffer
     TexData = new unsigned char[ 4*Width*Height]; 

	 index=0;
     for ( y = 0; y < Height; y++ )
     {
      index=y*4*Width;
      fread(row,1,4*Width,stream);
      for ( x = 0; x < 4*Width; x=x+4 ) 
	  {
		  TexData[index++]=row[x+2];
		  TexData[index++]=row[x+1];
		  TexData[index++]=row[x];
		  TexData[index++]=row[x+3];
	  }

	 }
   }
	   
    fclose( stream);
    delete row; return TexData;
   }
    cout << " ..ERROR: Can't open image file" << endl;
   
    return NULL;
}


int CTexture::readWord(FILE* in, int len)
{
   unsigned char* v;
   int x,temp = 0;


   v = new unsigned char[len*sizeof(int)];
   fread( v, len, 1, in);
   for ( x = 0; x < len; x++ )
   {
     temp += ( ((int)v[x]) << (x * 8));
   }
   delete v;
   return temp;
}	


unsigned char* CTexture::LoadFromTGA(const char* filename)
{
  FILE* stream;
  unsigned char* TexData=NULL;
  unsigned char* row=NULL;
  unsigned char bft[3];                
  int biBitCount;           
  int x,y,index=0;

  if ((stream = fopen( filename, "rb")) != NULL)
  {
   // extract file header
   fread( &bft, 1, 3, stream);
   if (bft[2] > 2) cout << "RLE encoding not supported";
   if (bft[2] != 2) 
   {
	   cout << "ERROR: Only type 2 uncompressed RGB(A) supported" << endl;
	   return NULL;  
   }
	
   // read unused header parameters
   fread( &bft, 1, 2, stream); fread( &bft, 1, 2, stream); fread( &bft, 1, 1, stream);
   fread( &bft, 1, 2, stream); fread( &bft, 1, 2, stream); 
	
   Width = readWord( stream, 2);
   Height = readWord( stream, 2);
   
   
   fread( &bft, 1, 1, stream); biBitCount=bft[0]; 
   fread( &bft, 1, 1, stream);
   
   //cout << "W: " << Width << " H: " << Height << " BitCount: " << biBitCount << endl;
   
   
   
   if (biBitCount == 24)
   {
	 index=0; 
	 unsigned char R=(unsigned char)TranspColor.R*255, G=(unsigned char)TranspColor.G*255, B=(unsigned char)TranspColor.B*255;
	 
	 // RGBA texture data buffer
     TexData = new unsigned char[ 4*Width*Height]; 
	 
	 // read data row by row and convert to RGBA 
     row = new unsigned char[3*Width];
     for ( y = 0; y < Height; y++ )
     {
      index=y*4*Width;
      fread(row,1,3*Width,stream);
      for ( x = 0; x < 3*Width; x=x+3 ) 
	  {
		  TexData[index++]=row[x+2];
		  TexData[index++]=row[x+1];
		  TexData[index++]=row[x];
		  if (TranspColor.A < 0) TexData[index]=255;
		  else if (row[x]==B && row[x+1]==G && row[x+2]==R) TexData[index]=0;
		  else TexData[index]=255;
		  index++;
	  }
     }
    delete row;
   }
   else if (biBitCount == 32)
   {
	 index=0; 
	 
	 // RGBA texture data buffer
     TexData = new unsigned char[ 4*Width*Height]; 
	 
	 // read data row by row and convert to RGBA 
     row = new unsigned char[4*Width];
     for ( y = 0; y < Height; y++ )
     {
      index=y*4*Width;
      fread(row,1,4*Width,stream);
      for ( x = 0; x < 4*Width; x=x+4 ) 
	  {
		  TexData[index++]=row[x+2];
		  TexData[index++]=row[x+1];
		  TexData[index++]=row[x];
		  TexData[index++]=row[x+3];
	  }
     }
    delete row;
   }
   else
   {
	 cout << " ..ERROR:" << endl;
     cout << "Texture must be a 24-bit or 32-bit uncompressed bitmap file" << endl;    
   }
    
    fclose( stream);
  }
  else
  {
	 cout << " ..ERROR: Can't open image file" << endl;
	 return NULL; 
  }
 return TexData;
}






