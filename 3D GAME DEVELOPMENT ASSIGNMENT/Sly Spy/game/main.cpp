#include "Sprite.h"

#include "GameApp.h"
#include "MyGame.h"

// export game to be accessed from other classes
extern CMyGame game;  

// make these global
CMyGame game;
CGameApp app;
  
int main(int argc, char* argv[])
{
	app.OpenWindow(1280, 720, "Sly Spy");
    //app.OpenFullScreen();

	app.Run(&game);
	return(0);
}
