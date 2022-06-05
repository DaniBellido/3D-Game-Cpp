#pragma once

#include "Game.h"
#include "Door.h"

class CMyGame : public CGame
{
public:
	CMyGame();
	~CMyGame();

    // ----  Declare your game variables and objects here -------------

	// Variables
	int score;
	int level;
	int ammo;
	int aidKit;
	bool lineofsight;
	bool enemyAttack;
	bool action;
	bool gamewon;
	bool start;

    // Models and Model Lists
    CModelMd2 player;   // animated md2 model
    CModel box;			// a single box
	CModel goal;
    CModelMd2 ogro;		// animated ogro md2 model 
	CModel coin;
	CModel ammoBox;
	CModel medicKit;
	CModel tank;
	CModel pallette;
	CModel rocketBox;
	CModel rocket;
	CModel wallSegment;
	CModel building;
	CModel grass;
	CModelMd2 alien;
	CModelMd2 alien2;
	CModelMd2 hunter;
	CModelMd2 locker;
	CModel sink;
	CModel bed;
	CModel furniture;
	CModel table;
	CModel bottles;
	CModel monitor;
	CModel panel;
	CModel microscope;

	CLine ray;
	CSprite meds;
	CSprite gun;
	CSprite crosshair;
	
	CModelList walls;   // list containing models
	CModelList ogros;   // list of ogro enemies
	CModelList coins;	// list of coins to collect
	CModelList tanks;
	CModelList pallettes;
	CModelList ammoBoxes;
	CModelList medicKits;
	CModelList shots;  // list of shots
	CModelList enemyShots;
	CModelList rocketBoxes;
	CModelList rockets;
	CModelList boxes;
	CModelList aliens;
	CModelList aliens2;
	CModelList hunters;
	CModelList lockers;
	CModelList objects;
	CModelList buildings;
	CModelList grassList;
	
	CDoor door1; // door (our new door model class)
	CDoor door2;
	CDoor door3;
	CDoor door4; // door (our new door model class)
	CDoor door5;
	CDoor door6;
	CDoor door7; // door (our new door model class)
	CDoor door8;
	CDoor door9;
	CModel button1;    // button (simple cuboid model)
	CModel button2;
	CModel button3;
	
	CSprite startScreen;
	CSprite pausedScreen;
	CSprite avatar;
	CSprite screen1;
	CSprite screen2;
	CSprite screen3;

	//sounds
	CSoundPlayer background_M;
	CSoundPlayer background_M2;
	CSoundPlayer background_M3;
	CSoundPlayer walkFX;
	CSoundPlayer doorAlarm;
	CSoundPlayer silencerShot;
	CSoundPlayer emptyGun;
	CSoundPlayer strayBullet;
	CSoundPlayer enemyAlert;
	CSoundPlayer droidOff;
	CSoundPlayer lowhealth;
	CSoundPlayer healing;
	CSoundPlayer error;
	CSoundPlayer reload;
	CSoundPlayer pickUpObj;
	CSoundPlayer mDoor;
	CSoundPlayer menu;

	
	// game world floor
	CFloor floor;
	
	// Font
	CFont font;
	CFont text;

	// health indicator
	CHealthBar healthBar;
	CHealthBar AlienHB;


   // -----   Add you member functions here ------
   
   void PlayerControl();
   void AlienControl();
   void EnemyAlienControl();
   void ShotsControl();
   void OgroControl();
   void tankControl();
   
   void CameraControl(CGraphics* g);
   
   void SetupLevel1();
   void SetupLevel2();
   void SetupLevel3();
	
	
   // ---------------------Event Handling --------------------------

	// Game Loop Funtions
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	virtual void OnRender3D(CGraphics* g);

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartLevel(int level);
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
