
#include "Game.h"


CGame::CGame()
{
	GameTime = 0; Width=0; Height=0;
	GameMode = MODE_MENU; 
	view = NULL;
	kbarray = NULL;
	graphics = NULL;
}

CGame::~CGame()
{
	ShowMouse();
}

void CGame::ShowCoordinateSystem(bool b)
{
	if (!b) return;
	glLineWidth(4.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glBegin(GL_LINES);
	glNormal3f(0,1,0);
	// x - dimension - red
	glColor3f( 1.0, 0,0); glVertex3f(0, 5, 0);
	glColor3f( 1.0, 0,0); glVertex3f(300, 5, 0);
	// y - dimension - green
	glColor3f( 0, 1.0,0); glVertex3f(0, 5, 0);
	glColor3f( 0, 1.0,0); glVertex3f(0, 300, 0);
	//z - dimension - blue
	glColor3f( 0, 0,1.0); glVertex3f(0, 5, 0);
	glColor3f( 0, 0,1.0); glVertex3f(0, 5, 300);
	
	glEnd();	
}



void CGame::DispatchEvents(SDL_Event* pEvent)
{
    kbarray=SDL_GetKeyState(NULL);   // get keyboard states
	
	// event mapping routine
	switch(pEvent->type)
	{
		case SDL_ACTIVEEVENT:		// activation event
			if(pEvent->active.state==SDL_APPACTIVE)
			{
				if(pEvent->active.gain) OnRestore();
				else OnMinimize();
			}
			break;
		case SDL_KEYDOWN:			// key press event mapping
			OnKeyDown(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.unicode);
			break;
		case SDL_KEYUP:				// key release event mapping
			OnKeyUp(pEvent->key.keysym.sym,pEvent->key.keysym.mod,pEvent->key.keysym.unicode);
			break;
		case SDL_MOUSEMOTION:		// mouse motion event mapping
			OnMouseMove(pEvent->motion.x,Height-pEvent->motion.y,pEvent->motion.xrel,pEvent->motion.yrel,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT))!=0,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT))!=0,(pEvent->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE))!=0);
			break;
		case SDL_MOUSEBUTTONDOWN:	// mouse button press event mapping
			// which button has been pressed?
			switch(pEvent->button.button)
			{
				case SDL_BUTTON_LEFT:	OnLButtonDown(pEvent->button.x,Height-pEvent->button.y); break;
				case SDL_BUTTON_RIGHT:	OnRButtonDown(pEvent->button.x,Height-pEvent->button.y); break;
				case SDL_BUTTON_MIDDLE:	OnMButtonDown(pEvent->button.x,Height-pEvent->button.y); break;
				case SDL_BUTTON_WHEELUP: OnWheelUp(pEvent->button.x,Height-pEvent->button.y); break;
				case SDL_BUTTON_WHEELDOWN: OnWheelDown(pEvent->button.x,Height-pEvent->button.y); break;
			}
			break;
		case SDL_MOUSEBUTTONUP:		//mouse button release event mapping
			//which button has been released?
			switch(pEvent->button.button)
			{
				case SDL_BUTTON_LEFT:	OnLButtonUp(pEvent->button.x,Height-pEvent->button.y); break;
				case SDL_BUTTON_RIGHT:	OnRButtonUp(pEvent->button.x,Height-pEvent->button.y); break;
				case SDL_BUTTON_MIDDLE:	OnMButtonUp(pEvent->button.x,Height-pEvent->button.y); break;
			}
			break;
		case SDL_VIDEORESIZE:		// resize video
			OnResize(pEvent->resize.w,pEvent->resize.h);
			break;
		case SDL_VIDEOEXPOSE:		// video expose
			OnExpose();
			break;
	
	}
}

// -----------  Generic Implementation of Events Handlers -----------

// Keyboard events
void CGame::OnKeyDown(SDLKey sym,SDLMod mod,Uint16 unicode)				{ }
void CGame::OnKeyUp(SDLKey sym,SDLMod mod,Uint16 unicode)				{ }

// Mouse events
void CGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)	{ }
void CGame::OnLButtonDown(Uint16 x,Uint16 y)							{ }
void CGame::OnLButtonUp(Uint16 x,Uint16 y)								{ }
void CGame::OnRButtonDown(Uint16 x,Uint16 y)							{ }
void CGame::OnRButtonUp(Uint16 x,Uint16 y)								{ }
void CGame::OnMButtonDown(Uint16 x,Uint16 y)							{ }
void CGame::OnMButtonUp(Uint16 x,Uint16 y)								{ }
void CGame::OnWheelUp(Uint16 x,Uint16 y)								{ }
void CGame::OnWheelDown(Uint16 x,Uint16 y)								{ }

// Active Events (application)
void CGame::OnMinimize()												{ PauseGame(); }
void CGame::OnRestore()												    { ResumeGame(); }

// Video Event
void CGame::OnResize(int w,int h)										{ }
void CGame::OnExpose()													{ }


// ----- 3D world coordinate to 2D projection plane coordinate -----
CVector CGame::WorldToScreenCoordinate(CVector pos)
{
	return graphics->WorldToScreenCoordinate(pos);
}


// ----- 2D Screen Coordinate on 3D plane (floor) -----
CVector CGame::ScreenToFloorCoordinate( int x, int y)
{
	return graphics->ScreenToFloorCoordinate( x, y);
}



