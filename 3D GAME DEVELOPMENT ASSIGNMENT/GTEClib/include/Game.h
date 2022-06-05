/**************************************************************************
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
**************************************************************************/
#pragma once

#include "GTEC.h"

#include "Texture.h"
#include "Font.h"
#include "Graphics.h"
#include "Sprite.h"
#include "SpriteRectangle.h"
#include "SpritePoint.h"
#include "SoundPlayer.h"
#include "Floor.h"
#include "Model.h"
#include "ModelPoint.h"
#include "Light.h"
#include "Line.h"
#include "MD2model.h"
#include "MD3model.h"
#include "HealthBar.h"


using namespace std;


class CGame
{
public:
	CGame();
	virtual ~CGame();

	// ------------  Game Modes -------------
	enum GAMEMODE { MODE_MENU, MODE_RUNNING, MODE_PAUSED, MODE_GAMEOVER, MODE_EXIT };
	
protected:
	
	int Height, Width;   				// width and height of the game
	
	CGraphics* graphics;

	// -------------- Game State Flags --------
	GAMEMODE GameMode;				// current game mode - either MODE_MENU, MODE_RUNNING, MODE_PAUSED or MODE_GAMEOVER

	
	// Game Timing
	Uint32 GameTime;		// the time the game has been running for in ms
	Uint8* kbarray;  		// pointer to keyboard states array
	
	// modelview matrix
	GLfloat* view;
	
	// light
	CLight Light;
	
	// indicator if deleted
	bool deleted;


public:
	// ---------  Accessor Functions ---------------
	
	int GetWidth()									{ return Width; }
	int GetHeight()									{ return Height; }
	void SetSize(int width, int height)			    { Width = width; Height = height; }
	
	
	// ----   Time Functions ------------------------
	
	void SetGameTime(Uint32 time)		{ GameTime = time; }
	Uint32 GetTime()			{ return GameTime; }
    void ResetGameTime()			{ SetGameTime(0); }
	
	

	// -----------  Game States ---------------------
	
    bool IsRunning() 			{ return GameMode == MODE_RUNNING; }
	bool IsGameMode()			{ return GameMode == MODE_RUNNING; }
	bool IsPaused()				{ return GameMode == MODE_PAUSED; }
	bool IsMenuMode()			{ return GameMode == MODE_MENU; }
	bool IsGameOver()			{ return GameMode == MODE_GAMEOVER; }
	bool IsGameOverMode()		{ return GameMode == MODE_GAMEOVER; }
	bool IsExit()				{ return GameMode == MODE_EXIT; }
	
	GAMEMODE GetGameMode()			{ return GameMode; }
	void SetGameMode(GAMEMODE mode)	{ GameMode = mode; }	
	

	void StartGame() 			{ SetGameMode(MODE_RUNNING); OnStartGame(); }
	void GameOver() 			{ SetGameMode(MODE_GAMEOVER); OnGameOver(); ShowMouse(); }
	void NewGame()				{ SetGameMode(MODE_MENU); ResetGameTime(); OnDisplayMenu(); }
	void PauseGame()			{ SetGameMode(MODE_PAUSED); }
	void ResumeGame()			{ SetGameMode(MODE_RUNNING); }
	void ExitGame() 			{ SetGameMode(MODE_EXIT); }
	void StopGame()				{ SetGameMode(MODE_EXIT); }


    // --------------- Mouse Functions ----------

    void HideMouse()			{ SDL_ShowCursor( false); }
    void ShowMouse()			{ SDL_ShowCursor( true); }
    
    void GetMouseCoords(int &x, int &y) { SDL_GetMouseState( &x, &y); }
    CVector GetMouseCoords() 
    { 
		CVector v;
		int x,y;
		SDL_GetMouseState( &x, &y); 
		v.x=(float)x; v.y=(float)y; v.z=0.0f;
		return v;
	}
	
	bool IsLButtonDown() { return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK) != 0; }
	bool IsRButtonDown() { return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK) != 0; }
	bool IsMButtonDown() { return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MMASK) != 0; }
    

 	// ---------------- Game Cycle Functions ------

	virtual void OnInitialize()		{ }
	virtual void OnDisplayMenu()	{ }
	virtual void OnStartGame()		{ }
	virtual void OnGameOver()		{ }
	virtual void OnTerminate()		{ }
	virtual void OnStartLevel(int level) 	{ }

	
	virtual void OnUpdate()			{ }
	virtual void OnDraw(CGraphics* g)			{ }
    virtual void OnRender3D(CGraphics* g)		{ }
    
    
	// ------- Event Dispatcher  ---------
	void DispatchEvents(SDL_Event* pEvent);

	
	// ---------------  Event Handling ------------

	// Check if a particular key is pressed
    bool IsKeyDown(SDLKey sym) { if (kbarray&&kbarray[sym]==1) return true; return false;}
    bool IsKeyUp(SDLKey sym) { if (kbarray&&kbarray[sym]==0) return true; return false;}	

	// Keyboard events
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse events
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
	virtual void OnWheelUp(Uint16 x,Uint16 y);
	virtual void OnWheelDown(Uint16 x,Uint16 y);

	// Window Events
	virtual void OnMinimize();
	virtual void OnRestore();
	virtual void OnResize(int w,int h);
	virtual void OnExpose();


    // used by game app to provide an up-to-date modelview matrix
    void SetGraphics(CGraphics* g) { graphics=g;}
    CGraphics* GetGraphics() { return graphics;}
    void UpdateView() { graphics->UpdateViewMatrix(); }
    
    void ShowCoordinateSystem(bool b=true);
    void ShowBoundingBoxes(bool b=true) { graphics->SetDebugGraphics(b);}
	
	

protected:

   // where is a 3D point on the screen
   CVector WorldToScreenCoordinate(CVector pos);
   // where is a 2D point on the floor
   CVector ScreenToFloorCoordinate( int x, int y);
	
};
