
#include "Graphics.h"



void CGraphics::InitializeGL()
{
    glClearColor( Color.R, Color.G, Color.B, Color.A);

	glLineWidth( 2);
	
	/* line anti-aliasing */
	glEnable( GL_LINE_SMOOTH);
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    
	glEnable( GL_POINT_SMOOTH);
	glShadeModel(GL_SMOOTH);

	/* enable 2D texturing */
	glEnable(GL_TEXTURE_2D);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_GREATER, 0);
   
  
    /* nice perspective correction */
    //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    /* anable alpha blending */
    //glEnable( GL_BLEND );
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
	/* enable alpha blending */
    glEnable( GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /* normalize normal vectors */
	glEnable(GL_NORMALIZE);
	
	/* enable color tracking */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
}

void CGraphics::SetProjection2D()
{
	glViewport(0, 0, Width, Height);		// reset the viewport to new dimensions
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix

	gluOrtho2D(0, Width, 0, Height);        // 2D projection
    glMatrixMode( GL_MODELVIEW );       	// set modelview matrix
	glLoadIdentity();	
	
	glDisable(GL_DEPTH_TEST);	
}

void CGraphics::SetProjection3D()
{
	//glViewport(0, 0, Width, Height);		// reset the viewport to new dimensions
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix
	
	nearPlane=1.0f;
	// calculate perspective
	gluPerspective(viewAngle, (float)Width/(float)Height, nearPlane, farPlane);
	glViewport(0, 0, Width, Height);		// reset the viewport to new dimensions
	glMatrixMode(GL_MODELVIEW);				// set modelview matrix
	glLoadIdentity();						// reset modelview matrix
	
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	
	// ----- set camera view --------
	gluLookAt(	eyeX, eyeY, eyeZ,
			centerX, centerY,  centerZ,
			0.0f, 1.0f,  0.0f);
			
}


// ----- 3D world coordinate to 2D projection plane coordinate -----
CVector CGraphics::WorldToScreenCoordinate(CVector pos)
{
	CVector v;
	
	 //nearPlane=-1000.0f;
	
	//GLfloat view[16];
	//glGetFloatv(GL_MODELVIEW_MATRIX, view);
	
    v.x=pos.x*view[0]+pos.y*view[4]+pos.z*view[8]+view[12];
    v.y=pos.x*view[1]+pos.y*view[5]+pos.z*view[9]+view[13];
    v.z=pos.x*view[2]+pos.y*view[6]+pos.z*view[10]+view[14];
    
    v.x=Width/2*(1-v.x/v.z/(Width*(float)tan(M_PI*viewAngle/360.0f)/Height));
    v.y=Height/2*(1-v.y/v.z/(float)tan(M_PI*viewAngle/360.0f));
   
    v.z=0;
	
	return v;
}

// ----- 2D Screen Coordinate on 3D plane (floor) -----
CVector CGraphics::ScreenToFloorCoordinate( int x, int y)
{
	CVector point;
	
	float l=0;
	
	GLfloat ray_clip[4]={1,0,-1,0};
	
	ray_clip[0]=(2.0f*x)/Width-1.0f;
	ray_clip[1]=(2.0f*y)/Height-1.0f;
	
	//cout << "ray_clip" << endl;
	//PrintMatrix(ray_clip,4);
	
	GLfloat ray_eye[4]={0,0,-1,0};
	GLfloat ray_line[4]={0,0,0,0};
	GLfloat lo[4]={0,0,0,1};
	
	GLfloat proj[16];
	GLfloat projInv[16];
	//GLfloat view[16];
	GLfloat viewInv[16];
	
	
	glGetFloatv(GL_PROJECTION_MATRIX, proj);
	//glGetFloatv(GL_MODELVIEW_MATRIX, view);
	
	//cout << "Projection" << endl;
	//PrintMatrix(proj);
	
	//cout << "Inverse Projection" << endl;
	if (!gluInvertMatrix(proj, projInv)) return point;
	//PrintMatrix(projInv);
	
	ray_eye[0]=ray_clip[0]*projInv[0]+ray_clip[1]*projInv[4]+ray_clip[2]*projInv[8];
	ray_eye[1]=ray_clip[0]*projInv[1]+ray_clip[1]*projInv[5]+ray_clip[2]*projInv[9];
	
	//cout << "ray_eye" << endl;
	//PrintMatrix(ray_eye,4);
	
	//cout << "Modelview" << endl;
    //PrintMatrix(view);
	
	//cout << "Inverse View" << endl;
	if (!gluInvertMatrix(view, viewInv)) return point;
	//PrintMatrix(viewInv);
	
	ray_line[0]=ray_eye[0]*viewInv[0]+ray_eye[1]*viewInv[4]+ray_eye[2]*viewInv[8];
	ray_line[1]=ray_eye[0]*viewInv[1]+ray_eye[1]*viewInv[5]+ray_eye[2]*viewInv[9];
	ray_line[2]=ray_eye[0]*viewInv[2]+ray_eye[1]*viewInv[6]+ray_eye[2]*viewInv[10];
	
	//cout << "ray_line" << endl;
	//PrintMatrix(ray_line,4);
	
	lo[0]=viewInv[12]; lo[1]=viewInv[13];  lo[2]=viewInv[14]; lo[3]=viewInv[15]; 
	
	//cout << "start_line" << endl;
	//PrintMatrix(lo,4);
	
	if (ray_line[1] != 0)
	{
		
		l=-lo[1]/ray_line[1];
		
		point.x=l*ray_line[0]+lo[0];
		point.y=l*ray_line[1]+lo[1];
		point.z=l*ray_line[2]+lo[2];
	
		//cout << "x: " << (int)point.x << " y: " << (int)point.y << " z: " << (int)point.z << endl;
		
	}
	return point;
}

CVector CGraphics::GetViewingDirection(int x, int y)
{
	GLfloat ray_clip[4]={1,0,-1,0};
	
	ray_clip[0]=(2.0f*x)/Width-1.0f;
	ray_clip[1]=(2.0f*y)/Height-1.0f;
	
	GLfloat ray_eye[4]={0,0,-1,0};
	
	
	GLfloat proj[16];
	GLfloat projInv[16];
	//GLfloat view[16];
	//GLfloat viewInv[16];
	
	
	glGetFloatv(GL_PROJECTION_MATRIX, proj);
		
	//cout << "Inverse Projection" << endl;
	if (!gluInvertMatrix(proj, projInv)) return CVector(0,0,0);
	//PrintMatrix(projInv);
	
	ray_eye[0]=ray_clip[0]*projInv[0]+ray_clip[1]*projInv[4]+ray_clip[2]*projInv[8];
	ray_eye[1]=ray_clip[0]*projInv[1]+ray_clip[1]*projInv[5]+ray_clip[2]*projInv[9];
	
	return CVector(ray_eye[0],ray_eye[1],ray_eye[2]);
	
}



/********************************************************
 * Source: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
 ********************************************************/
bool CGraphics::gluInvertMatrix(const float m[16], float invOut[16])
{
    float inv[16], det;
    int i;

    inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15]
			+ m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
	inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15]
			- m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
	inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15]
			+ m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
	inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14]
			- m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
	inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15]
			- m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
	inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15]
			+ m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
	inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15]
			- m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
	inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14]
			+ m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
	inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15]
			+ m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
	inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15]
			- m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
	inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15]
			+ m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
	inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14]
			- m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
	inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11]
			- m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
	inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11]
			+ m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
	inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11]
			- m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
	inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10]
			+ m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

    if (det == 0)
        return false;

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return true;
}

void CGraphics::PrintMatrix( float* m, int size)
{
	if (size==4)
	{
		cout << m[0] << " " << m[1] << " " << m[2] << " " << m[3] << endl;
	}
	else if (size==16)
	{
	   cout << m[0] << " " << m[4] << " " << m[8] << " " << m[12] << endl;
	   cout << m[1] << " " << m[5] << " " << m[9] << " " << m[13] << endl;
	   cout << m[2] << " " << m[6] << " " << m[10] << " " << m[14] << endl;
	   cout << m[3] << " " << m[7] << " " << m[11] << " " << m[15] << endl;
    }
}





