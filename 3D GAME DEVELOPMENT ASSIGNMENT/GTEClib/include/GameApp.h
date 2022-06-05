#pragma once

#include "Game.h"
#include "Color.h"


class CGameApp
{

private:
	CGame *pGame;				// Reference to the Game
	SDL_Surface *pSurface;		// Reference to screen surface
	CGraphics* graphics;			// OpenGL graphics
	
	Uint16 Height,Width;    	// Width and height of the game
	Uint16 FPS;					// Frames per Second rate
	Uint32 GameTime;			// Game Time since the game was started


public:
	
	// --- Constructor & Destructor ----------
	CGameApp();
	~CGameApp();

	// ----- Setter and Getter Functions -----
	
    int GetWidth()						{ return Width; }
	int GetHeight()						{ return Height; }
	
	int GetFPS()						{ return FPS; }
	void SetFPS(unsigned short fps)	{ FPS = fps; }
	
	void SetClearColor( const CColor& color)  { graphics->SetClearColor( color); }
	
	// ------- The System Clock ------------------

	Uint32 GetSystemTime();	   // returns the number of milliseconds since the application was started
	
	// -----------    Open Window/Full Screen -----------
	bool OpenWindow(int width, int height, string title);
	bool OpenFullScreen(int width, int height);
	bool OpenFullScreen();

	// -----------   Main Game Loop ----------------------
	bool Run(CGame *pGame = NULL);

};
