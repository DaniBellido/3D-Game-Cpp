#include "GameApp.h"


// ----- Constructor & Destructor --------

CGameApp::CGameApp()
{
	pGame = NULL;
	pSurface = NULL;
	Height=0; Width=0;
	FPS=30; GameTime=0;
	graphics = new CGraphics();
	
	
	/* new seed for random number generator */
	srand((unsigned)time(NULL));
}

CGameApp::~CGameApp() { delete graphics;}


// ------ Game Time -----------

Uint32 CGameApp::GetSystemTime()
{
	// returns the number of milliseconds since the application was started
    return SDL_GetTicks();

}

// ----  Application Management ----------

bool CGameApp::OpenWindow(int width, int height, string title)
{
	/* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	Width=width; Height=height;

	/* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */	
 

    /* Create a SDL surface */
    pSurface = SDL_SetVideoMode( width, height, 16, videoFlags );

    /* Verify if there is a surface */
    if ( !pSurface ) { cout << " Error: Video not initialized" << endl; return false; }
		
	// call SQL quit when closing the game	
	atexit(SDL_Quit);
	
	SDL_WM_SetCaption( title.c_str(), title.c_str());

    // Set up OpenGL graphics
	graphics->SetViewPort( Width,Height);
	graphics->InitializeGL(); 
	graphics->SetProjection2D();
	
	Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048);

	return true;
}

bool CGameApp::OpenFullScreen(int width, int height)
{
	/* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	Width=width; Height=height;

	/* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_FULLSCREEN;      /* Enable full screen */
 
    /* Set screen size */
       
    /* Create a SDL surface */
    pSurface = SDL_SetVideoMode( width, height, 16, videoFlags );

    /* Verify if there is a surface */
    if ( !pSurface ) { cout << " Error: Video not initialized" << endl; return false; }
		
	// call SQL quit when closing the game	
	atexit(SDL_Quit);

	// Set up OpenGL graphics
	graphics->SetViewPort( Width,Height);
	graphics->InitializeGL(); 
	graphics->SetProjection2D();
	
	Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048);

	
	return true;
}

bool CGameApp::OpenFullScreen()
{
	/* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	/* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_FULLSCREEN;      /* Enable full screen */
 
    /* Set screen size */
    
    
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    Width = info->current_w;
    Height = info->current_h;
    
    cout << "Screen: " << Width << "x" << Height << endl;
	
	/* Create a SDL surface */
    pSurface = SDL_SetVideoMode( Width, Height, 16, videoFlags );

    /* Verify if there is a surface */
    if ( !pSurface ) { cout << " Error: Video not initialized" << endl; return false; }
		
	// call SQL quit when closing the game	
	atexit(SDL_Quit);

	// Set up OpenGL graphics
	graphics->SetViewPort( Width,Height);
	graphics->InitializeGL(); 
	graphics->SetProjection2D();
	
	Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048);
	
	return true;
	
}


bool CGameApp::Run(CGame *pgame)
{
	cout << "GTEC engine v1.9.2" << endl;
	cout << "(c) 2012-2020 Andreas Hoppe" << endl;
	
	// set graphics context
	pgame->SetGraphics (graphics);
	
	 // Handling of loading screen
	CSprite LoadingScreen;
	 LoadingScreen.LoadImage("LoadingScreen.bmp");
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	 LoadingScreen.SetPosition(float(Width / 2), float(Height / 2));
	 LoadingScreen.Draw(graphics);
	SDL_GL_SwapBuffers();
	
	SDL_Event anEvent;				// declare an Event
	

	// Controlling the Time
	double period = 1000.0 / FPS;

	if (!pgame) return false;        // return false if game does not exist
	else pGame = pgame;
	
	// provide game with the size of the screen
	pGame->SetSize(Width, Height);
	
	// update current modelview matrix
	graphics->UpdateViewMatrix();

   
	// --- Call In-Game Initialisation ---
	pGame->OnInitialize();
	

	GameTime = 0; // resetting game time at the start of the game
	
	Uint32 startTime = GetSystemTime(); // current system time in milliseconds
	
	Uint32 framecycles = 0;
	Uint32 gamecycles = 0;
    
	bool loading = true;           // loading game assets?
	bool runGame = true;		   // indicates whether to run or quit the game

	// -------------   Game Loop -------------------------------
	do
	{

		// ----- Game Over Mode -----
		if (pGame->IsGameOver())
		{
		   GameTime=0; gamecycles=0;
           pGame->SetGameTime( GameTime);
		}
		
		if (pGame->IsExit()) runGame=false;
       
       // clear screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		
		
		if (!pGame->IsMenuMode())
		{
		
		 // set 3D projection
		 graphics->SetProjection3D();
		 
		 // -------  game world transformations -------
		 // transformations applied to affect the whole game world
				
		 glScalef(0.005f,0.005f,0.005f); // scale game world
       
       
         // update viewmatrix
		 graphics->UpdateViewMatrix();
		 
		 // ----- render the 3D game world ------------
	     pGame->OnRender3D( graphics);  // render 3D scene and objects
		
		
		 // update viewmatrix
		 graphics->UpdateViewMatrix();
	    }
		
		// --- Event Handling ------- 
		while ( SDL_PollEvent(&anEvent))
		{
			// --- Processing of exit events ---
            pGame->DispatchEvents(&anEvent);
			
		    if (anEvent.type == SDL_QUIT) { runGame = false;}  
		}
		
		if (loading) 
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
			SDL_GL_SwapBuffers();
			loading=false;
			pGame->OnDisplayMenu();
			
		}
		
		// ---- Game Running: Updating the game time ------
		if (pGame->IsRunning() || pGame->IsMenuMode())
		{
		   gamecycles++;
           GameTime = Uint32(gamecycles*period);
		   pGame->SetGameTime( GameTime);
		   pGame->OnUpdate();
		}
		
		// set 2D projection
		graphics->SetProjection2D();
		
		bool lighting=false;
		
		if (glIsEnabled(GL_LIGHTING))
		{	
		 lighting=true;
		 glDisable(GL_LIGHTING);
	    }			
		 
		pGame->OnDraw(  graphics); // draw 2D screen and sprites
		 
		if (lighting) glEnable(GL_LIGHTING);
		
		Uint32 timeStamp2 = Uint32(framecycles*period + startTime);

	    if (timeStamp2 < GetSystemTime()) framecycles=Uint32((GetSystemTime()-startTime)/period);

		while (timeStamp2 > GetSystemTime())
		{
			SDL_Delay(1);
		}

		// flip buffers
		SDL_GL_SwapBuffers();
		
		framecycles++;
	} while (runGame);

	pGame->OnTerminate();
	SDL_Quit();
	return true;
}

