#include "MyGame.h"
#include <fstream>

using namespace std;

CMyGame::CMyGame(void) {}

CMyGame::~CMyGame(void) {}

// --------  game initialisation --------
void CMyGame::OnInitialize()
{
	lineofsight = false;
	enemyAttack = false;
	action = false;
	gamewon = false;
	start = false;
	// setup healthbar
	healthBar.SetSize(150, 25);
	healthBar.SetPosition(50, 100);
	healthBar.Rotate(90);

	// setup Alien healthbar
	AlienHB.SetSize(150, 25);
	AlienHB.SetPosition(640, 650);
	// Loading graphics and sound assets
	cout << "Loading assets" << endl;
	
	font.LoadDefault(); 
	
	// enable lighting
	Light.Enable();
	// abarlith model
	player.LoadModel("Abarlith/Abarlith.md2"); 
	player.SetScale( 3.5f);
	// ogro model
	ogro.LoadModel("droid/droid.md2");
	ogro.SetScale( 5.0f);
	// coin model
	coin.LoadModel("coin/coin.obj"); 
	coin.SetScale(10.0f);

	wallSegment.LoadModel("wall/wall2.obj");
	wallSegment.SetScale(4.0f);
	wallSegment.SetToFloor(0);

	// tank model
	tank.LoadModel("tank/triax_tracks.obj");
	tank.SetScale(4.0f);
	tank.SetToFloor(0);

	// pallette model
	pallette.LoadModel("pallette/pallette.obj");
	pallette.SetScale(7.0f);
	pallette.SetToFloor(0);

	// ammoBox model
	ammoBox.LoadModel("Ammo_box/box_ammo.obj");
	ammoBox.SetScale(30.0f);
	ammoBox.SetToFloor(0);

	// medic Kit model
	medicKit.LoadModel("medic_kit/medi.obj");
	medicKit.SetScale(7.0f);
	medicKit.SetToFloor(0);

	// box of rockets model
	rocketBox.LoadModel("box_rockets/rocketBox.obj");
	rocketBox.SetScale(10.0f);
	rocketBox.SetToFloor(0);

	// single rocket model
	rocket.LoadModel("Rocket/rocket.obj");
	rocket.SetScale(35.0f);
	rocket.SetToFloor(0);

	// building model;
	building.LoadModel("building/building.obj");
	building.SetScale(100.0f);
	building.SetToFloor(0);

	// grass model;
	grass.LoadModel("grass/grass.obj");
	grass.SetScale(1.5f);
	grass.SetToFloor(5);

	// hatch model;
	goal.LoadModel("hatch/hatch.obj");
	goal.SetScale(1.5f);
	goal.SetToFloor(0);

	// alien model;
	alien.LoadModel("alien/alien.md2");
	alien.SetScale(3.5f);
	alien.SetToFloor(0);

	// alien2 model;
	alien2.LoadModel("alien2/alien2.md2");
	alien2.SetScale(5.0f);
	alien2.SetToFloor(0);

	// hunter model;
	hunter.LoadModel("hunter/hunter.md2");
	hunter.SetScale(3.5f);
	hunter.SetToFloor(0);

	// locker model;
	locker.LoadModel("lockerkast/lockerkast.md2");
	locker.SetScale(3.5f);
	locker.SetToFloor(0);

	// sink model;
	sink.LoadModel("keuken_rvs/sink.obj");
	sink.SetScale(1.0f);
	sink.SetToFloor(50);

	// bed model;
	bed.LoadModel("bed/bed.obj");
	bed.SetScale(0.5f);
	bed.SetToFloor(0);

	// furniture model;
	furniture.LoadModel("Bkast/kast.obj");
	furniture.SetScale(130.0f);
	furniture.SetToFloor(0);

	// table model;
	table.LoadModel("labtafel/labtafel.obj");
	table.SetScale(50.0f);
	table.SetToFloor(0);

	// bottles model;
	bottles.LoadModel("Mfles/Mfles.obj");
	bottles.SetScale(40.0f);
	bottles.SetToFloor(100);

	// monitor model;
	monitor.LoadModel("Monitoren/monitoren.obj");
	monitor.SetScale(50.0f);
	monitor.SetToFloor(0);

	// panel model;
	panel.LoadModel("techpanel/techpanel.obj");
	panel.SetScale(4.0f);
	panel.SetToFloor(60);

	// microscppe model;
	microscope.LoadModel("Microscope/microsc.obj");
	microscope.SetScale(20.0f);
	microscope.SetToFloor(100);

	
	// floor texture
	floor.LoadTexture("metalfloor.bmp");
	
	
	// floor texture
	floor.LoadTexture("floor.bmp");
	floor.SetTiling(false);
	
	// start screenw
	startScreen.LoadImage("startScreen.bmp");
	startScreen.SetPosition(Width/2.0f,Height/2.0f);

	// paused screen
	pausedScreen.LoadImage("pausedScreen.bmp");
	pausedScreen.SetPosition(Width / 2.0f, Height / 2.0f);

	//UI and other 2D images
	meds.LoadImage("medikit.bmp");
	meds.SetPosition(142, 80);

	gun.LoadImage("gun.bmp", CColor::Red());
	gun.SetPosition(1200, 80);

	avatar.LoadImage("alienAvatar.bmp", CColor::Red());
	avatar.SetPosition(520, 650);

	crosshair.LoadImage("crossHair.bmp", CColor::Green());

	screen1.LoadImage("level1.bmp", CColor::White());
	screen1.SetPosition(225, 325);

	screen2.LoadImage("level2.bmp", CColor::White());
	screen2.SetPosition(610, 325);

	screen3.LoadImage("level3.bmp", CColor::White());
	screen3.SetPosition(1010, 325);


	//Loading sounds
	background_M.LoadAudio("level1.wav");
	background_M2.LoadAudio("level2.wav");
	walkFX.LoadAudio("walk.wav");
	doorAlarm.LoadAudio("alarm.wav");
	silencerShot.LoadAudio("silencerShot.wav");
	emptyGun.LoadAudio("emptyGun.wav");
	strayBullet.LoadAudio("strayBullet.wav");
	enemyAlert.LoadAudio("enemyAlert.wav");
	droidOff.LoadAudio("droidOff.wav");
	lowhealth.LoadAudio("lowHealth.wav");
	healing.LoadAudio("healing.wav");
	error.LoadAudio("error.wav");
	reload.LoadAudio("reload.wav");
	pickUpObj.LoadAudio("pickUpObj.wav");
	mDoor.LoadAudio("metalDoor.wav");
	menu.LoadAudio("menu.wav");
}
	

void CMyGame::SetupLevel1()
{
    level=1;
	background_M.Stop();
	background_M2.Stop();
    // --- clear model lists ----
    walls.clear();
    ogros.clear();
    coins.clear();	
	tanks.clear();
	pallettes.clear();
	ammoBoxes.clear();
	medicKits.clear();
	shots.clear();
	enemyShots.clear();
	rocketBoxes.clear();
	rockets.clear();
	boxes.clear();
	aliens.clear();
	aliens2.clear();
	hunters.clear();
	lockers.clear();
	objects.clear();
	buildings.clear();
	grassList.clear();
	
	
	background_M.Play("level1.wav", -1);
	background_M.SetVolume(15.0);

	// set size of the game world (2000x2000)
	floor.SetSize(6000,6000);
	// move gameworld so that it starts at 0,0 and extends in positive xz-direction
	floor.SetPosition(3000,3000);
	floor.LoadTexture("floor.bmp");
	floor.SetTiling(false);
  
  // set box position, size and motion
	goal.SetPosition((float)5700, (float) 0, (float)2260);
	goal.SetToFloor(0);
    

	CModel* pBox = box.Clone(); // clone wall segment
	pBox->SetSize(1000, 500, 1500);
	pBox->SetPosition((float)1900, (float)100, (float)6750);
	pBox->SetColor(CColor::Black());
	pBox->SetToFloor(0);
	boxes.push_back(pBox);

	CModel* pBox2 = box.Clone(); // clone wall segment
	pBox2->SetSize(1000, 500, 1500);
	pBox2->SetPosition((float)1900, (float)100, (float)-750);
	pBox2->SetColor(CColor::Black());
	pBox2->SetToFloor(0);
	boxes.push_back(pBox2);

	


  
    // player position
    player.SetPosition(310,100,100); player.SetToFloor(0); player.SetDirection(0); player.SetRotation(0); 
  
    // reset score
    score=0; 
	// declaring new file
	fstream myfile;
	// opening file for reading
	myfile.open("level1.txt", ios_base::in);

	int type, x, y, z, rot;
	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		myfile >> type >> x >> y >> z >> rot;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << " " << z << " " << rot << endl;
		if (neof) // if not end of file
		{
			if (type == 1)   // wall segments
			{
				CModel* pWall = wallSegment.Clone(); // clone wall segment
				pWall->SetPosition((float)x, (float)y, (float)z);
				pWall->SetRotation(float(rot));
				pWall->SetToFloor(0);
				walls.push_back(pWall);
			}

			if (type == 2)  // coins
			{
				CModel* pCoin = coin.Clone();
				pCoin->SetPosition((float)x, (float)y, (float)z); pCoin->SetRotation((float)rot);
				coins.push_back(pCoin);
			}

			if (type == 3) // ogros
			{
				CModel* pEnemy = ogro.Clone();
				pEnemy->SetPosition((float)x, (float)y, (float)z); pEnemy->SetToFloor(0);
				pEnemy->SetRotation(float(rot)); pEnemy->SetDirection((float)rot);
				pEnemy->SetSpeed(100);
				ogros.push_back(pEnemy);
			}
			if (type == 4) //tank
			{
				CModel* pTank = tank.Clone();
				pTank->SetPosition((float)x, (float)y, (float)z);
				pTank->SetRotation(float(rot));
				pTank->SetStatus(4); // indicate the type of object
				tanks.push_back(pTank);

			}
			if (type == 5) //pallette
			{
				CModel* pPallette = pallette.Clone();
				pPallette->SetPosition((float)x, (float)y, (float)z);
				pPallette->SetRotation(float(rot));
				pPallette->SetStatus(5); // indicate the type of object
				pallettes.push_back(pPallette);
			}
			if (type == 6) //ammo box
			{
				CModel* pAmmoBox = ammoBox.Clone();
				pAmmoBox->SetPosition((float)x, (float)y, (float)z);
				pAmmoBox->SetRotation(float(rot));
				pAmmoBox->SetStatus(6); // indicate the type of object
				pAmmoBox->SetOmega(50);
				ammoBoxes.push_back(pAmmoBox);
			}
			if (type == 7) //medic Kit
			{
				CModel* pMedicKit = medicKit.Clone();
				pMedicKit->SetPosition((float)x, (float)y, (float)z);
				pMedicKit->SetRotation(float(rot));
				pMedicKit->SetStatus(7); // indicate the type of object
				pMedicKit->SetOmega(50);
				medicKits.push_back(pMedicKit);
			}
			if (type == 8) //rocketBox
			{
				CModel* pRBox = rocketBox.Clone();
				pRBox->SetPosition((float)x, (float)y, (float)z);
				pRBox->SetRotation(float(rot));
				pRBox->SetStatus(8); // indicate the type of object
				rocketBoxes.push_back(pRBox);
			}
			if (type == 9) //single rocket
			{
				CModel* pRocket = rocket.Clone();
				pRocket->SetPosition((float)x, (float)y, (float)z);
				pRocket->SetRotation(float(rot));
				pRocket->SetStatus(9); // indicate the type of object
				rockets.push_back(pRocket);
			}
			if (type == 10) //building
			{
				CModel* pBuilding = building.Clone();
				pBuilding->SetPosition((float)x, (float)y, (float)z);
				pBuilding->SetRotation(float(rot));
				pBuilding->SetStatus(10); // indicate the type of object
				buildings.push_back(pBuilding);
			}
			if (type == 12) //grass
			{
				CModel* pGrass = grass.Clone();
				pGrass->SetPosition((float)x, (float)y, (float)z);
				pGrass->SetRotation(float(rot));
				pGrass->SetStatus(12); // indicate the type of object
				grassList.push_back(pGrass);
			}
			if (type == 13) //alien
			{
				CModel* pAlien = alien.Clone();
				pAlien->SetPosition((float)x, (float)y, (float)z);
				pAlien->SetRotation(float(rot));
				pAlien->SetStatus(13); // indicate the type of object
				aliens.push_back(pAlien);
			}
			if (type == 14) //alien2
			{
				CModel* pAlien2 = alien2.Clone();
				pAlien2->SetPosition((float)x, (float)y, (float)z);
				pAlien2->SetRotation(float(rot));
				pAlien2->SetStatus(14); // indicate the type of object
				aliens2.push_back(pAlien2);
			}
			if (type == 15) //hunter
			{
				CModel* pHunter = hunter.Clone();
				pHunter->SetPosition((float)x, (float)y, (float)z);
				pHunter->SetRotation(float(rot));
				pHunter->SetStatus(15); // indicate the type of object
				hunters.push_back(pHunter);
			}
			if (type == 16) //locker
			{
				CModel* pLocker = locker.Clone();
				pLocker->SetPosition((float)x, (float)y, (float)z);
				pLocker->SetRotation(float(rot));
				pLocker->SetStatus(16); // indicate the type of object
				lockers.push_back(pLocker);
			}
			if (type == 17) //sink
			{
				CModel* pSink = sink.Clone();
				pSink->SetPosition((float)x, (float)y, (float)z);
				pSink->SetRotation(float(rot));
				pSink->SetStatus(17); // indicate the type of object
				objects.push_back(pSink);
			}
			if (type == 18) //bed
			{
				CModel* pBed = bed.Clone();
				pBed->SetPosition((float)x, (float)y, (float)z);
				pBed->SetRotation(float(rot));
				pBed->SetStatus(18); // indicate the type of object
				objects.push_back(pBed);
			}
			if (type == 19) //furniture
			{
				CModel* pF = furniture.Clone();
				pF->SetPosition((float)x, (float)y, (float)z);
				pF->SetRotation(float(rot));
				pF->SetStatus(19); // indicate the type of object
				objects.push_back(pF);
			}
			if (type == 20) //table
			{
				CModel* pTable = table.Clone();
				pTable->SetPosition((float)x, (float)y, (float)z);
				pTable->SetRotation(float(rot));
				pTable->SetStatus(20); // indicate the type of object
				objects.push_back(pTable);
			}
			if (type == 21) //bottles
			{
				CModel* pBot = bottles.Clone();
				pBot->SetPosition((float)x, (float)y, (float)z);
				pBot->SetRotation(float(rot));
				pBot->SetStatus(21); // indicate the type of object
				objects.push_back(pBot);
			}
			if (type == 22) //panel
			{
				CModel* pPanel = panel.Clone();
				pPanel->SetPosition((float)x, (float)y, (float)z);
				pPanel->SetRotation(float(rot));
				pPanel->SetStatus(22); // indicate the type of object
				objects.push_back(pPanel);
			}
			if (type == 23) //Microscope
			{
				CModel* pMS = microscope.Clone();
				pMS->SetPosition((float)x, (float)y, (float)z);
				pMS->SetRotation(float(rot));
				pMS->SetStatus(23); // indicate the type of object
				objects.push_back(pMS);
			}
			if (type == 24) //monitor
			{
				CModel* pMonitor = monitor.Clone();
				pMonitor->SetPosition((float)x, (float)y, (float)z);
				pMonitor->SetRotation(float(rot));
				pMonitor->SetStatus(24); // indicate the type of object
				objects.push_back(pMonitor);
			}
		}
	} while (neof);
	myfile.close();


	
	
    
    // ----  set up door ----
    door1.SetSize(40, 200,800);
    // door location, door is currently closed
    door1.SetPosition(4140,200/2-10,1300);
    // set door open position and closed position
    door1.SetPositions(CVector(4140,200/2-10,1950),CVector(4140, 200 / 2 - 10, 1300));
    door1.SetColor(CColor(0,0,255));
  

    // ---- set up button -----
    button1.SetPosition(1060,200/2,5420);
    button1.SetSize(40,40,40); 
    button1.SetColor(CColor::Red());

	// ----  set up door 2 ----
	door2.SetSize(40, 200, 850);
	// door location, door is currently closed
	door2.SetPosition(4635, 200 / 2 - 10, 475);
	// set door open position and closed position
	door2.SetPositions(CVector(4635, 200 / 2 - 10, -250), CVector(4635, 200 / 2 - 10, 475));
	door2.SetColor(CColor(0, 0, 255));


	// ---- set up button 2 -----
	button2.SetPosition(5847, 200 / 2, 5755);
	button2.SetSize(40, 40, 40);
	button2.SetColor(CColor::Red());
  
	// ----  set up door 3 ----
	door3.SetSize(40, 200, 850);
	// door location, door is currently closed
	door3.SetPosition(5960, 200 / 2 - 10, 4820);
	// set door open position and closed position
	door3.SetPositions(CVector(5960, 200 / 2 - 10, 5520), CVector(5960, 200 / 2 - 10, 4820));
	door3.SetColor(CColor(0, 0, 255));


	// ---- set up button 3 -----
	button3.SetPosition(530, 200 / 2, 130);
	button3.SetSize(40, 40, 40);
	button3.SetColor(CColor::Red());
}



void CMyGame::SetupLevel2()
{
	level =2;
	background_M.Stop();
	background_M2.Stop();
	background_M2.Play("level2.wav", -1);
	background_M2.SetVolume(10.0);
	// --- clear model lists ----
	walls.clear();
	ogros.clear();
	coins.clear();
	tanks.clear();
	pallettes.clear();
	ammoBoxes.clear();
	medicKits.clear();
	shots.clear();
	enemyShots.clear();
	rocketBoxes.clear();
	rockets.clear();
	boxes.clear();
	aliens.clear();
	aliens2.clear();
	hunters.clear();
	lockers.clear();
	objects.clear();
	buildings.clear();
	grassList.clear();

	// set size of the game world (2000x2000)
	floor.SetSize(6000, 6000);
	// move gameworld so that it starts at 0,0 and extends in positive xz-direction
	floor.SetPosition(3000, 3000);
	floor.LoadTexture("metalfloor.bmp");
	floor.SetTiling(true);
	//// set box position, size and motion
	//  box.SetSize(100,100,100);  box.SetPosition( 220,50,220); box.SetToFloor(0);
	//  box.SetColor( CColor::Blue());

	  // player position
	player.SetPosition(310, 100, 5257); player.SetToFloor(0); player.SetDirection(0); player.SetRotation(0);

	// reset score
	score = 0;

	// declaring new file
	fstream myfile;
	// opening file for reading
	myfile.open("level2.txt", ios_base::in);

	int type, x, y, z, rot;
	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		myfile >> type >> x >> y >> z >> rot;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << " " << z << " " << rot << endl;
		if (neof) // if not end of file
		{
			if (type == 1)   // wall segments
			{
				CModel* pWall = wallSegment.Clone(); // clone wall segment
				pWall->SetPosition((float)x, (float)y, (float)z);
				pWall->SetRotation(float(rot));
				pWall->SetToFloor(0);
				walls.push_back(pWall);
			}

			if (type == 2)  // coins
			{
				CModel* pCoin = coin.Clone();
				pCoin->SetPosition((float)x, (float)y, (float)z); pCoin->SetRotation((float)rot);
				coins.push_back(pCoin);
			}

			if (type == 3) // ogros
			{
				CModel* pEnemy = ogro.Clone();
				pEnemy->SetPosition((float)x, (float)y, (float)z); pEnemy->SetToFloor(0);
				pEnemy->SetRotation(float(rot)); pEnemy->SetDirection((float)rot);
				pEnemy->SetSpeed(100);
				ogros.push_back(pEnemy);
			}
			if (type == 4) //tank
			{
				CModel* pTank = tank.Clone();
				pTank->SetPosition((float)x, (float)y, (float)z);
				pTank->SetRotation(float(rot));
				pTank->SetStatus(4); // indicate the type of object
				tanks.push_back(pTank);

			}
			if (type == 5) //pallette
			{
				CModel* pPallette = pallette.Clone();
				pPallette->SetPosition((float)x, (float)y, (float)z);
				pPallette->SetRotation(float(rot));
				pPallette->SetStatus(5); // indicate the type of object
				pallettes.push_back(pPallette);
			}
			if (type == 6) //ammo box
			{
				CModel* pAmmoBox = ammoBox.Clone();
				pAmmoBox->SetPosition((float)x, (float)y, (float)z);
				pAmmoBox->SetRotation(float(rot));
				pAmmoBox->SetStatus(6); // indicate the type of object
				pAmmoBox->SetOmega(50);
				ammoBoxes.push_back(pAmmoBox);
			}
			if (type == 7) //medic Kit
			{
				CModel* pMedicKit = medicKit.Clone();
				pMedicKit->SetPosition((float)x, (float)y, (float)z);
				pMedicKit->SetRotation(float(rot));
				pMedicKit->SetStatus(7); // indicate the type of object
				pMedicKit->SetOmega(50);
				medicKits.push_back(pMedicKit);
			}
			if (type == 8) //rocketBox
			{
				CModel* pRBox = rocketBox.Clone();
				pRBox->SetPosition((float)x, (float)y, (float)z);
				pRBox->SetRotation(float(rot));
				pRBox->SetStatus(8); // indicate the type of object
				rocketBoxes.push_back(pRBox);
			}
			if (type == 9) //single rocket
			{
				CModel* pRocket = rocket.Clone();
				pRocket->SetPosition((float)x, (float)y, (float)z);
				pRocket->SetRotation(float(rot));
				pRocket->SetStatus(9); // indicate the type of object
				rockets.push_back(pRocket);
			}
			if (type == 10) //building
			{
				CModel* pBuilding = building.Clone();
				pBuilding->SetPosition((float)x, (float)y, (float)z);
				pBuilding->SetRotation(float(rot));
				pBuilding->SetStatus(10); // indicate the type of object
				buildings.push_back(pBuilding);
			}
			if (type == 12) //grass
			{
				CModel* pGrass = grass.Clone();
				pGrass->SetPosition((float)x, (float)y, (float)z);
				pGrass->SetRotation(float(rot));
				pGrass->SetStatus(12); // indicate the type of object
				grassList.push_back(pGrass);
			}
			if (type == 13) //alien
			{
				CModel* pAlien = alien.Clone();
				pAlien->SetPosition((float)x, (float)y, (float)z);
				pAlien->SetRotation(float(rot));
				pAlien->SetStatus(13); // indicate the type of object
				aliens.push_back(pAlien);
			}
			if (type == 14) //alien2
			{
				CModel* pAlien2 = alien2.Clone();
				pAlien2->SetPosition((float)x, (float)y, (float)z);
				pAlien2->SetRotation(float(rot));
				pAlien2->SetStatus(14); // indicate the type of object
				aliens2.push_back(pAlien2);
			}
			if (type == 15) //hunter
			{
				CModel* pHunter = hunter.Clone();
				pHunter->SetPosition((float)x, (float)y, (float)z);
				pHunter->SetRotation(float(rot));
				pHunter->SetStatus(15); // indicate the type of object
				hunters.push_back(pHunter);
			}
			if (type == 16) //locker
			{
				CModel* pLocker = locker.Clone();
				pLocker->SetPosition((float)x, (float)y, (float)z);
				pLocker->SetRotation(float(rot));
				pLocker->SetStatus(16); // indicate the type of object
				lockers.push_back(pLocker);
			}
			if (type == 17) //sink
			{
				CModel* pSink = sink.Clone();
				pSink->SetPosition((float)x, (float)y, (float)z);
				pSink->SetRotation(float(rot));
				pSink->SetStatus(17); // indicate the type of object
				objects.push_back(pSink);
			}
			if (type == 18) //bed
			{
				CModel* pBed = bed.Clone();
				pBed->SetPosition((float)x, (float)y, (float)z);
				pBed->SetRotation(float(rot));
				pBed->SetStatus(18); // indicate the type of object
				objects.push_back(pBed);
			}
			if (type == 19) //furniture
			{
				CModel* pF = furniture.Clone();
				pF->SetPosition((float)x, (float)y, (float)z);
				pF->SetRotation(float(rot));
				pF->SetStatus(19); // indicate the type of object
				objects.push_back(pF);
			}
			if (type == 20) //table
			{
				CModel* pTable = table.Clone();
				pTable->SetPosition((float)x, (float)y, (float)z);
				pTable->SetRotation(float(rot));
				pTable->SetStatus(20); // indicate the type of object
				objects.push_back(pTable);
			}
			if (type == 21) //bottles
			{
				CModel* pBot = bottles.Clone();
				pBot->SetPosition((float)x, (float)y, (float)z);
				pBot->SetRotation(float(rot));
				pBot->SetStatus(21); // indicate the type of object
				objects.push_back(pBot);
			}
			if (type == 22) //panel
			{
				CModel* pPanel = panel.Clone();
				pPanel->SetPosition((float)x, (float)y, (float)z);
				pPanel->SetRotation(float(rot));
				pPanel->SetStatus(22); // indicate the type of object
				objects.push_back(pPanel);
			}
			if (type == 23) //Microscope
			{
				CModel* pMS = microscope.Clone();
				pMS->SetPosition((float)x, (float)y, (float)z);
				pMS->SetRotation(float(rot));
				pMS->SetStatus(23); // indicate the type of object
				objects.push_back(pMS);
			}
			if (type == 24) //monitor
			{
				CModel* pMonitor = monitor.Clone();
				pMonitor->SetPosition((float)x, (float)y, (float)z);
				pMonitor->SetRotation(float(rot));
				pMonitor->SetStatus(24); // indicate the type of object
				objects.push_back(pMonitor);
			}
		}
	} while (neof);
	myfile.close();



	// ----  set up door ----
	door1.SetSize(40, 200, 800);
	// door location, door is currently closed
	door1.SetPosition(4140, 200 / 2 - 10, 1300);
	// set door open position and closed position
	door1.SetPositions(CVector(4140, 200 / 2 - 10, 1950), CVector(4140, 200 / 2 - 10, 1300));
	door1.SetColor(CColor(0, 0, 255));


	// ---- set up button -----
	button1.SetPosition(1060, 200 / 2, 5420);
	button1.SetSize(40, 40, 40);
	button1.SetColor(CColor::Red());

	// ----  set up door 2 ----
	door2.SetSize(40, 200, 850);
	// door location, door is currently closed
	door2.SetPosition(4635, 200 / 2 - 10, 475);
	// set door open position and closed position
	door2.SetPositions(CVector(4635, 200 / 2 - 10, -250), CVector(4635, 200 / 2 - 10, 475));
	door2.SetColor(CColor(0, 0, 255));


	// ---- set up button 2 -----
	button2.SetPosition(5847, 200 / 2, 5755);
	button2.SetSize(40, 40, 40);
	button2.SetColor(CColor::Red());

	// ----  set up door 3 ----
	door3.SetSize(40, 200, 850);
	// door location, door is currently closed
	door3.SetPosition(5960, 200 / 2 - 10, 4820);
	// set door open position and closed position
	door3.SetPositions(CVector(5960, 200 / 2 - 10, 5520), CVector(5960, 200 / 2 - 10, 4820));
	door3.SetColor(CColor(0, 0, 255));


	// ---- set up button 3 -----
	button3.SetPosition(530, 200 / 2, 130);
	button3.SetSize(40, 40, 40);
	button3.SetColor(CColor::Red());
	   
}
void CMyGame::SetupLevel3() 
{
	level = 3;
	
	background_M.Stop();
	background_M3.Stop();
	menu.Play("menu.wav", -1);
	background_M3.SetVolume(30.0);
	// --- clear model lists ----
	walls.clear();
	ogros.clear();
	coins.clear();
	tanks.clear();
	pallettes.clear();
	ammoBoxes.clear();
	medicKits.clear();
	shots.clear();
	enemyShots.clear();
	rocketBoxes.clear();
	rockets.clear();
	boxes.clear();
	aliens.clear();
	aliens2.clear();
	hunters.clear();
	lockers.clear();
	objects.clear();
	buildings.clear();
	grassList.clear();

	// set size of the game world (2000x2000)
	floor.SetSize(3550, 6000);
	// move gameworld so that it starts at 0,0 and extends in positive xz-direction
	floor.SetPosition(1775, 3000);
	floor.LoadTexture("metalfloor.bmp");
	floor.SetTiling(true);

	  // player position
	player.SetPosition(310, 100, 5257); player.SetToFloor(0); player.SetDirection(0); player.SetRotation(0);

	// reset score
	score = 0;

	// declaring new file
	fstream myfile;
	// opening file for reading
	myfile.open("level3.txt", ios_base::in);

	int type, x, y, z, rot;
	// reading while the end of file has not been reached
	bool neof; // not end of file
	do
	{
		myfile >> type >> x >> y >> z >> rot;

		neof = myfile.good();
		//cout << type << " " << x << " " << y << " " << z << " " << rot << endl;
		if (neof) // if not end of file
		{
			if (type == 1)   // wall segments
			{
				CModel* pWall = wallSegment.Clone(); // clone wall segment
				pWall->SetPosition((float)x, (float)y, (float)z);
				pWall->SetRotation(float(rot));
				pWall->SetToFloor(0);
				walls.push_back(pWall);
			}

			if (type == 2)  // coins
			{
				CModel* pCoin = coin.Clone();
				pCoin->SetPosition((float)x, (float)y, (float)z); pCoin->SetRotation((float)rot);
				coins.push_back(pCoin);
			}

			if (type == 3) // ogros
			{
				CModel* pEnemy = ogro.Clone();
				pEnemy->SetPosition((float)x, (float)y, (float)z); pEnemy->SetToFloor(0);
				pEnemy->SetRotation(float(rot)); pEnemy->SetDirection((float)rot);
				pEnemy->SetSpeed(100);
				ogros.push_back(pEnemy);
			}
			if (type == 4) //tank
			{
				CModel* pTank = tank.Clone();
				pTank->SetPosition((float)x, (float)y, (float)z);
				pTank->SetRotation(float(rot));
				pTank->SetStatus(4); // indicate the type of object
				tanks.push_back(pTank);

			}
			if (type == 5) //pallette
			{
				CModel* pPallette = pallette.Clone();
				pPallette->SetPosition((float)x, (float)y, (float)z);
				pPallette->SetRotation(float(rot));
				pPallette->SetStatus(5); // indicate the type of object
				pallettes.push_back(pPallette);
			}
			if (type == 6) //ammo box
			{
				CModel* pAmmoBox = ammoBox.Clone();
				pAmmoBox->SetPosition((float)x, (float)y, (float)z);
				pAmmoBox->SetRotation(float(rot));
				pAmmoBox->SetStatus(6); // indicate the type of object
				pAmmoBox->SetOmega(50);
				ammoBoxes.push_back(pAmmoBox);
			}
			if (type == 7) //medic Kit
			{
				CModel* pMedicKit = medicKit.Clone();
				pMedicKit->SetPosition((float)x, (float)y, (float)z);
				pMedicKit->SetRotation(float(rot));
				pMedicKit->SetStatus(7); // indicate the type of object
				pMedicKit->SetOmega(50);
				medicKits.push_back(pMedicKit);
			}
			if (type == 8) //rocketBox
			{
				CModel* pRBox = rocketBox.Clone();
				pRBox->SetPosition((float)x, (float)y, (float)z);
				pRBox->SetRotation(float(rot));
				pRBox->SetStatus(8); // indicate the type of object
				rocketBoxes.push_back(pRBox);
			}
			if (type == 9) //single rocket
			{
				CModel* pRocket = rocket.Clone();
				pRocket->SetPosition((float)x, (float)y, (float)z);
				pRocket->SetRotation(float(rot));
				pRocket->SetStatus(9); // indicate the type of object
				rockets.push_back(pRocket);
			}
			if (type == 10) //building
			{
				CModel* pBuilding = building.Clone();
				pBuilding->SetPosition((float)x, (float)y, (float)z);
				pBuilding->SetRotation(float(rot));
				pBuilding->SetStatus(10); // indicate the type of object
				buildings.push_back(pBuilding);
			}
			if (type == 12) //grass
			{
				CModel* pGrass = grass.Clone();
				pGrass->SetPosition((float)x, (float)y, (float)z);
				pGrass->SetRotation(float(rot));
				pGrass->SetStatus(12); // indicate the type of object
				grassList.push_back(pGrass);
			}
			if (type == 13) //alien
			{
				CModel* pAlien = alien.Clone();
				pAlien->SetPosition((float)x, (float)y, (float)z);
				pAlien->SetRotation(float(rot));
				pAlien->SetStatus(13); // indicate the type of object
				aliens.push_back(pAlien);
			}
			if (type == 14) //alien2
			{
				CModel* pAlien2 = alien2.Clone();
				pAlien2->SetPosition((float)x, (float)y, (float)z);
				pAlien2->SetRotation(float(rot));
				pAlien2->SetStatus(14); // indicate the type of object
				aliens2.push_back(pAlien2);
			}
			if (type == 15) //hunter
			{
				CModel* pHunter = hunter.Clone();
				pHunter->SetPosition((float)x, (float)y, (float)z);
				pHunter->SetRotation(float(rot));
				pHunter->SetStatus(15); // indicate the type of object
				hunters.push_back(pHunter);
			}
			if (type == 16) //locker
			{
				CModel* pLocker = locker.Clone();
				pLocker->SetPosition((float)x, (float)y, (float)z);
				pLocker->SetRotation(float(rot));
				pLocker->SetStatus(16); // indicate the type of object
				lockers.push_back(pLocker);
			}
			if (type == 17) //sink
			{
				CModel* pSink = sink.Clone();
				pSink->SetPosition((float)x, (float)y, (float)z);
				pSink->SetRotation(float(rot));
				pSink->SetStatus(17); // indicate the type of object
				objects.push_back(pSink);
			}
			if (type == 18) //bed
			{
				CModel* pBed = bed.Clone();
				pBed->SetPosition((float)x, (float)y, (float)z);
				pBed->SetRotation(float(rot));
				pBed->SetStatus(18); // indicate the type of object
				objects.push_back(pBed);
			}
			if (type == 19) //furniture
			{
				CModel* pF = furniture.Clone();
				pF->SetPosition((float)x, (float)y, (float)z);
				pF->SetRotation(float(rot));
				pF->SetStatus(19); // indicate the type of object
				objects.push_back(pF);
			}
			if (type == 20) //table
			{
				CModel* pTable = table.Clone();
				pTable->SetPosition((float)x, (float)y, (float)z);
				pTable->SetRotation(float(rot));
				pTable->SetStatus(20); // indicate the type of object
				objects.push_back(pTable);
			}
			if (type == 21) //bottles
			{
				CModel* pBot = bottles.Clone();
				pBot->SetPosition((float)x, (float)y, (float)z);
				pBot->SetRotation(float(rot));
				pBot->SetStatus(21); // indicate the type of object
				objects.push_back(pBot);
			}
			if (type == 22) //panel
			{
				CModel* pPanel = panel.Clone();
				pPanel->SetPosition((float)x, (float)y, (float)z);
				pPanel->SetRotation(float(rot));
				pPanel->SetStatus(22); // indicate the type of object
				objects.push_back(pPanel);
			}
			if (type == 23) //Microscope
			{
				CModel* pMS = microscope.Clone();
				pMS->SetPosition((float)x, (float)y, (float)z);
				pMS->SetRotation(float(rot));
				pMS->SetStatus(23); // indicate the type of object
				objects.push_back(pMS);
			}
			if (type == 24) //monitor
			{
				CModel* pMonitor = monitor.Clone();
				pMonitor->SetPosition((float)x, (float)y, (float)z);
				pMonitor->SetRotation(float(rot));
				pMonitor->SetStatus(24); // indicate the type of object
				objects.push_back(pMonitor);
			}
		}
	} while (neof);
	myfile.close();




	// ----  set up door ----
	door1.SetSize(40, 200, 800);
	// door location, door is currently closed
	door1.SetPosition(36, 200 / 2 - 10, 5280);
	// set door open position and closed position
	door1.SetPositions(CVector(36, 200 / 2 - 10, 5280), CVector(36, 200 / 2 - 10, 5280));
	door1.SetColor(CColor(0, 0, 255));


	// ---- set up button -----
	button1.SetPosition(3465, 200 / 2, 3240);
	button1.SetSize(40, 40, 40);
	button1.SetColor(CColor::Red());

	// ----  set up door 2 ----
	door2.SetSize(40, 200, 500);
	// door location, door is currently closed
	door2.SetPosition(1266, 200 / 2 - 10, 2103);
	// set door open position and closed position
	door2.SetPositions(CVector(1266, 200 / 2 - 10, 2500), CVector(1266, 200 / 2 - 10, 2013));
	door2.SetColor(CColor(0, 0, 255));


	// ----  set up door 3 ----
	door3.SetSize(40, 200, 500);
	// door location, door is currently closed
	door3.SetPosition(1266, 200 / 2 - 10, 1096);
	// set door open position and closed position
	door3.SetPositions(CVector(1266, 200 / 2 - 10, 1500), CVector(1266, 200 / 2 - 10, 1096));
	door3.SetColor(CColor(0, 0, 255));

	// ----  set up door 4 ----
	door4.SetSize(40, 200, 500);
	// door location, door is currently closed
	door4.SetPosition(1266, 200 / 2 - 10, 240);
	// set door open position and closed position
	door4.SetPositions(CVector(1266, 200 / 2 - 10, 600), CVector(1266, 200 / 2 - 10, 240));
	door4.SetColor(CColor(0, 0, 255));

	// ----  set up door 5 ----
	door5.SetSize(40, 200, 500);
	// door location, door is currently closed
	door5.SetPosition(2413, 200 / 2 - 10, 702);
	// set door open position and closed position
	door5.SetPositions(CVector(2413, 200 / 2 - 10, 350), CVector(2413, 200 / 2 - 10, 702));
	door5.SetColor(CColor(0, 0, 255));

	// ----  set up door 6 ----
	door6.SetSize(40, 200, 500);
	// door location, door is currently closed
	door6.SetPosition(2389, 200 / 2 - 10, 1689);
	// set door open position and closed position
	door6.SetPositions(CVector(2389, 200 / 2 - 10, 1189), CVector(2389, 200 / 2 - 10, 1689));
	door6.SetColor(CColor(0, 0, 255));

	// ----  set up door 7 ----
	door7.SetSize(40, 200, 500);
	// door location, door is currently closed
	door7.SetPosition(2411, 200 / 2 - 10, 2708);
	// set door open position and closed position
	door7.SetPositions(CVector(2411, 200 / 2 - 10, 2208), CVector(2411, 200 / 2 - 10, 2708));
	door7.SetColor(CColor(0, 0, 255));

	// ----  set up door 8 ----
	door8.SetSize(40, 200, 1100);
	// door location, door is currently closed
	door8.SetPosition(1784, 200 / 2 - 10, 2910);
	door8.SetRotation(door8.GetRotation() + 90);
	// set door open position and closed position
	door8.SetPositions(CVector(784, 200 / 2 - 10, 2910), CVector(1784, 200 / 2 - 10, 2910)); 
	door8.SetColor(CColor(0, 0, 255));

	// ----  set up door 9 ----
	door9.SetSize(40, 200, 850);
	// door location, door is currently closed
	door9.SetPosition(3550, 200 / 2 - 10, 5121);
	// set door open position and closed position
	door9.SetPositions(CVector(3550, 200 / 2 - 10, 4270), CVector(3550, 200 / 2 - 10, 5121));
	door9.SetColor(CColor(0, 0, 255));

	for (CModel* alien : aliens2) 
	{
		alien->SetHealth(100);
	}

}
   
   
void CMyGame::OnStartLevel(int level)
{
}

// Game Logic in the OnUpdate function called every frame

void CMyGame::OnUpdate() 
{
	if (IsMenuMode() || IsGameOver()) 
	{
	
		return;
	}

	
	long t =  GetTime();
	
	// --- updating models ----

	
	walls.Update( t);
	coins.Update(t);
	shots.Update(t);
	tanks.Update(t);
	pallettes.Update(t);
	enemyShots.Update(t);
	ammoBoxes.Update(t);
	medicKits.Update(t);
	rocketBoxes.Update(t);
	rockets.Update(t);
	door1.Update(t);
	door2.Update(t);
	door3.Update(t);
	door4.Update(t);
	door5.Update(t);
	door6.Update(t);
	door7.Update(t);
	door8.Update(t);
	door9.Update(t);
	meds.Update(t);
	gun.Update(t);
	buildings.Update(t);
	grassList.Update(t);
	boxes.Update(t);
	goal.Update(t);
	aliens.Update(t);
	aliens2.Update(t);
	hunters.Update(t);
	lockers.Update(t);
	objects.Update(t);
	crosshair.Update(t);
	screen1.Update(t);
	screen2.Update(t);
	screen3.Update(t);
	
	OgroControl();
	tankControl();
    ogros.Update( t);
	PlayerControl();
	AlienControl();
	EnemyAlienControl();
	ShotsControl();
    player.Update( t);



	if (healthBar.GetHealth() <= 0 || AlienHB.GetHealth() <= 0 || gamewon ) GameOver();



	//cout << "x: " << player.GetX() << " y: " << player.GetY() << " z: " << player.GetZ() << endl;
}

void CMyGame::PlayerControl()
{
	// player control 
	// Only move the player forward if it does not collide with a wall
	if (IsKeyDown(SDLK_w))
	{ 
		bool move=true; // we assume that we can move
		
		for (CModel* pBox : boxes)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pBox)) move = false;
		}
		// checking whether the player collides with any of the walls 
		for (CModel* pWall : walls)
		{
            // don't allow to move if there is a collision
			if (player.HitTestFront( pWall)) move=false;	
		}

		for (CModel* pTank : tanks)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pTank)) move = false;
		}

		for (CModel* pPallette : pallettes)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pPallette)) move = false;
		}
		for (CModel* pRB : rocketBoxes)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pRB)) move = false;
		}
		for (CModel* pRocket : rockets)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pRocket)) move = false;
		}

		for (CModel* pBuilding : buildings)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pBuilding)) move = false;
		}
		for (CModel* pObj : objects)
		{
			// don't allow to move if there is a collision
			if (player.HitTestFront(pObj)) move = false;
		}
		
		
		// check if player collides with the door
		if (level==2 && player.HitTestFront(&door1) || level == 2 && player.HitTestFront(&door2) || level == 2 && player.HitTestFront(&door3)) move=false;
		if (level == 3 && player.HitTestFront(&door1) || level == 3 && player.HitTestFront(&door2) || level == 3 && player.HitTestFront(&door3)) move = false;
		if (level == 3 && player.HitTestFront(&door4) || level == 3 && player.HitTestFront(&door5) || level == 3 && player.HitTestFront(&door6)) move = false;
		if (level == 3 && player.HitTestFront(&door7) || level == 3 && player.HitTestFront(&door8) || level == 3 && player.HitTestFront(&door9)) move = false;

		// if no collision, move forward
		if (move)
		{
			player.SetSpeed(500);
			player.PlayAnimation(40, 45, 7, true);
		}
		else player.SetSpeed(0); 

	}
	else player.SetSpeed(0);

	static bool kill = false;
	//PLAYER ANIMATIONS
	for (CModel* pOgro : ogros) 
	{
		
		if (/*!player.HitTest(pOgro) && */IsKeyDown(SDLK_w)) 
		{
			player.PlayAnimation(40, 45, 7, true);
			kill = false;
		}
		if (!player.HitTest(pOgro) && !IsKeyDown(SDLK_w)&& kill == false) 
		{ 
			player.PlayAnimation(1, 39, 7, true); 
		}
		if (player.HitTestFront(pOgro) && pOgro->HitTestBack(&player))
		{
			if (IsKeyDown(SDLK_e) && kill == false) 
			{
				player.SetSpeed(0);
				player.PlayAnimation(72, 83, 15, false); 
				droidOff.Play("droidOff.wav");
				pOgro->SetSpeed(-2);
				pOgro->Die(1000);
				kill = true;
			}
		}
	}
	
	
	
	// reload your ammo and delete the ammoBox
	for (CModel* pAmmo : ammoBoxes)
	{
		if(player.HitTest(pAmmo) && ammo < 15)
		{
			ammo = ammo + 3;
			reload.Play("reload.wav");
			pAmmo->Delete();
		}
		else if (player.HitTest(pAmmo) && ammo == 15)
		{
			//donothing
		}
	}
	ammoBoxes.remove_if(deleted);

	// reload your ammo and delete the ammoBox
	for (CModel* pAid : medicKits)
	{
		if (player.HitTest(pAid) && aidKit < 4)
		{
			aidKit++;
			pickUpObj.Play("pickUpObj.wav");
			pAid->Delete();
		}
		else if(player.HitTest(pAid) && aidKit == 4)
		{
			//donothing
		}
	}
	medicKits.remove_if(deleted);

	if (level == 1 && player.HitTest(&goal) && IsKeyDown(SDLK_e)) 
	{
		mDoor.Play("metalDoor.wav");
		SetupLevel2();
	}
	if (level == 2 && player.GetX() > 6000) 
	{
		SetupLevel3();
	}

	
	bool grassHit = false;
	for (CModel* p : grassList)
	{
		if (player.HitTest(p))
		{

			grassHit = true;

		}

	}
	if (grassHit == true) player.SetColor(0, 0, 0);
	else player.SetColor(215, 215, 215);

}
void CMyGame::AlienControl() 
{
	if (level == 3) 
	{
		static bool follow = false;
		for (CModel* alien : aliens) 
		{
			if (!follow) alien->PlayAnimation(112, 122, 10, true);

			if (player.HitTest(alien)) follow = true;
		
			if (follow && player.GetSpeed() > 0) 
			{
				alien->PlayAnimation("run", 10, true);
				alien->SetDirectionAndRotationToPoint(player.GetX(), player.GetZ());
				alien->SetSpeed(player.GetSpeed() - 50);
			}
			if (follow && player.GetSpeed() == 0) 
			{
				alien->PlayAnimation("crstnd", 10, true);
				alien->SetSpeed(player.GetSpeed());
			}
			if (alien->GetX() > 3500) gamewon = true;
		}
		if (IsKeyDown(SDLK_F2)) { follow = false; }
		/*cout << follow << endl;*/
	}
}

void CMyGame::EnemyAlienControl() 
{
	if (level == 3) 
	{

		static bool follow = false;
		for (CModel* pA : aliens) 
		{
			for (CModel* alien : aliens2)
			{
			/*	cout << alien->GetHealth() << endl;*/

				if (!follow) alien->PlayAnimation("crstand", 10, true);

				if (player.HitTest(&button1)) follow = true;

				if (follow)
				{
					alien->SetColor(CColor::Red());
					alien->PlayAnimation("run", 10, true);
					alien->SetDirectionAndRotationToPoint(pA->GetX(), pA->GetZ());
					alien->SetSpeed(280);
				}
				if (follow && alien->HitTest(pA))
				{
					AlienHB.SetHealth(AlienHB.GetHealth() - 100);
				}
				for (CModel* pWall : walls)
				{
					if (alien->HitTestFront(pWall) || alien->HitTestFront(&door2) || alien->HitTestFront(&door3) || alien->HitTestFront(&door5) || alien->HitTestFront(&door6) || alien->HitTestFront(&door7))
					{
						alien->SetSpeed(0);

					}
				}

			}

		}
		if (IsKeyDown(SDLK_F2)) { follow = false; }
		/*cout << follow << endl;*/
		
	}

}

void CMyGame::ShotsControl()
{

	// collision detection between shots and enemies
	for (CModel* pEnemy : ogros)
	{
		for (CModel* pShot : shots)
		{
			if (pEnemy->HitTest(pShot))
			{
				pEnemy->SetSpeed(-1);
				pEnemy->Die(1000);
				pShot->Delete();
				break;
			}
		}
	}
	ogros.delete_if(deleted);
	shots.delete_if(deleted);

	for (CModel* badAlien : aliens2)
	{
		for (CModel* pShot : shots)
		{
			if (badAlien->HitTest(pShot))
			{
				badAlien->SetHealth(badAlien->GetHealth() -25);
				pShot->Delete();
			}
			if (badAlien->GetHealth() <= 0)
			{
				badAlien->Delete();
				
			}
		}

	}
	aliens2.delete_if(deleted);
	shots.delete_if(deleted);

	
	for (CModel* pShot : shots)
	{
		for (CModel* wall : walls) 
		{
			if (pShot->HitTest(wall))
			{
				pShot->Delete(); // remove shots when hitting the wall
				strayBullet.Play("strayBullet.wav");
			}

		}

		for (CModel* pTank : tanks)
		{
			if (pShot->HitTest(pTank))
			{
				pShot->Delete(); // remove shots when hitting the wall
				strayBullet.Play("strayBullet.wav");
			}

		}
	}
	shots.delete_if(deleted);

	//Enemyshots

	for (CModel* pShot : enemyShots)
	{
		if (pShot->HitTest(&player))
		{
			healthBar.SetHealth(healthBar.GetHealth() - 10);
			pShot->Delete();
			if (healthBar.GetHealth() <= 25) { lowhealth.Play("lowHealth.wav", -1); }

		}

		for (CModel* alien : aliens)
		{
			if (pShot->HitTest(alien))
			{
				AlienHB.SetHealth(AlienHB.GetHealth() - 20);

				pShot->Delete();
				//soundeffect
			}
		}

		for (CModel* wall : walls) 
		{
			if (pShot->HitTest(wall))
			{
				pShot->Delete();
				//soundeffect
			}
		}

		for (CModel* pTank : tanks)
		{
			if (pShot->HitTest(pTank))
			{
				pShot->Delete(); // remove shots when hitting the wall
				strayBullet.Play("strayBullet.wav"); //anothersound would be better
			}

		}

		
	}
	enemyShots.delete_if(deleted);
}


void CMyGame::OgroControl()
{
	// ----- A.I. enemies (ogros) control -------
	// ogros turn around once they hit a wall
	if (enemyAttack) 
	{
		for (CModel* pOgro : ogros) 
		{

			pOgro->SetDirectionAndRotationToPoint(player.GetX(), player.GetZ());
			if (pOgro->HitTest(player.GetPositionV(), 5000) && enemyAttack == true)
			{
				// create a new shot as a short line
				CModel* pShot = new CLine(pOgro->GetPositionV(), 50);
				// rotation and direction towards ogro
				pShot->SetDirectionToPoint(player.GetX(), player.GetZ());
				pShot->SetRotationToPoint(player.GetX(), player.GetZ());
				pShot->SetSpeed(1000);
				enemyShots.push_back(pShot);
			}
			
		}
		enemyAttack = false;

	}
	else 
	{
		for (CModel* pOgro : ogros)
		{
			for (CModel* pWall : walls)
			{

				// is ogro hitting a wall or door1?
				if (pOgro->HitTestFront(pWall) || pOgro->HitTestFront(&door1) || pOgro->HitTestFront(&door2) || pOgro->HitTestFront(&door3))
				{
					// change direction and rotation of hitting a wall
					pOgro->SetRotation(pOgro->GetRotation() + 180);
					pOgro->SetDirection(pOgro->GetRotation());
					break;

				}
			}

			for (CModel* pTank : tanks)
			{

				// is ogro hitting a tank
				if (pOgro->HitTestFront(pTank))
				{
					// change direction and rotation of hitting a wall
					pOgro->SetRotation(pOgro->GetRotation() + 180);
					pOgro->SetDirection(pOgro->GetRotation());
					break;

				}
			}

			for (CModel* pPallette : pallettes)
			{

				// is ogro hitting a tank
				if (pOgro->HitTestFront(pPallette))
				{
					// change direction and rotation of hitting a wall
					pOgro->SetRotation(pOgro->GetRotation() + 180);
					pOgro->SetDirection(pOgro->GetRotation());
					break;

				}
			}
			for (CModel* pRB : rocketBoxes)
			{

				// is ogro hitting a tank
				if (pOgro->HitTestFront(pRB))
				{
					// change direction and rotation of hitting a wall
					pOgro->SetRotation(pOgro->GetRotation() + 180);
					pOgro->SetDirection(pOgro->GetRotation());
					break;

				}
			}
			for (CModel* pRocket : rockets)
			{

				// is ogro hitting a tank
				if (pOgro->HitTestFront(pRocket))
				{
					// change direction and rotation of hitting a wall
					pOgro->SetRotation(pOgro->GetRotation() + 180);
					pOgro->SetDirection(pOgro->GetRotation());
					break;

				}
			}
			for (CModel* pBuilding : buildings)
			{

				// is ogro hitting a tank
				if (pOgro->HitTestFront(pBuilding))
				{
					// change direction and rotation of hitting a wall
					pOgro->SetRotation(pOgro->GetRotation() + 180);
					pOgro->SetDirection(pOgro->GetRotation());
					break;

				}
			}

		}

		// play ogro animations according to motion behaviour
		for (CModel* pOgro : ogros)
		{
			// ogro animations
			if (pOgro->GetSpeed() == 100) 
			{ //walk
				pOgro->PlayAnimation(40, 45, 5, true); 
			}
			else if (pOgro->GetSpeed() == 0) 
			{ //stand
				pOgro->PlayAnimation(1, 39, 5, true);
			}
			else 
			{ //die
				pOgro->PlayAnimation(184, 189, 5, false);
			}


		
			if (level == 3)
			{
				
				if (action == false) pOgro->SetSpeed(0);
				if (action == true && door9.GetZ() <= 4280)
				{

					pOgro->SetDirectionAndRotationToPoint(1740.0, 5150.0);
					pOgro->SetSpeed(100);

					if (pOgro->GetX() <= 3080) 
					{

						pOgro->SetDirectionAndRotationToPoint(player.GetX(), player.GetZ());


						for (CModel* pWall : walls)
						{
							if (pOgro->HitTestFront(pWall) || pOgro->HitTestFront(&door1) || pOgro->HitTestFront(&door9) || pOgro->HitTestFront(&door8))
							{
								pOgro->SetSpeed(0);
								
							}

						}
					}
			
				

					
				}
			}

		}

	}
	
}

void CMyGame::tankControl() 
{
	if (level == 1) 
	{
		for (CModel* o : tanks)
		{
			if (o->HitTest(player.GetPositionV()))
			{
				GameOver();
			}

			o->SetZVelocity(200);
			if (o->GetZ() >= 6780) 
			{
				o->SetPosition(1900, 0, -520);
			}

		
		}
		
	}
}



//-----------------  Draw 2D overlay ----------------------
void CMyGame::OnDraw(CGraphics* g)
{
	if (IsMenuMode())
	{
	  startScreen.Draw(g);
	  screen1.Draw(g);
	  screen2.Draw(g);
	  screen3.Draw(g);
	  crosshair.Draw(g);
	  return;
	}
	if (IsPaused())
	{
		pausedScreen.Draw(g);
		crosshair.Draw(g);
		return;
	}
	
	// draw level text
	font.SetColor(CColor::Green()); font.SetSize(14); font.DrawText(600, 10, "press Space to pause");
	font.SetColor( CColor::Green()); font.SetSize( 32); font.DrawText( 10,Height-50, "LEVEL" );
	font.DrawNumber(120,Height-50, level);
	// draw score
	font.SetColor( CColor::Green()); font.DrawNumber(Width-50,Height-50, score);
	////draw ammo
	//font.SetColor(CColor::Green()); font.SetSize(20); font.DrawText(500, Height - 50, "AMMO: ");
	//font.DrawNumber(580, Height - 50, ammo);

	//draw 2D UI
	meds.Draw(g);
	font.SetColor(CColor::Green()); font.SetSize(32); font.DrawText(100, 20, "( ");
	font.DrawNumber(110, 20, aidKit);
	font.SetColor(CColor::Green()); font.SetSize(32); font.DrawText(130, 20, "/4)");

	gun.Draw(g);
	font.SetColor(CColor::Green()); font.SetSize(32); font.DrawText(1160, 20, "( ");
	font.DrawNumber(1170, 20, ammo);
	font.SetColor(CColor::Green()); font.SetSize(32); font.DrawText(1190, 20, "/15)");

	



	// inform player to press the button
	if (level == 2 && player.HitTest(&button1) || player.HitTest(&button2) || player.HitTest(&button3) || (level == 3 && player.HitTest(&button1)) || level == 1 && player.HitTest(&goal))
	{
		font.SetColor( CColor::Green());
		font.DrawText( Width/3,2*Height/3, "PRESS E TO INTERACT" );
	}

	for (CModel* pOgro : ogros) 
	{
		if (player.HitTestFront(pOgro) && pOgro->HitTestBack(&player)) 
		{
			font.SetColor(CColor::Green());
			font.DrawText(Width / 3, 2 * Height / 3, "PRESS E TO KILL");
		}

	}
	
	
	// draw GAME OVER if game over
   	if (IsGameOver() && !gamewon)
   	{
		font.SetSize(48); font.SetColor( CColor::Red()); font.DrawText( 250,300, "MISSION FAILED");	
	}
	else if(gamewon)
	{
		font.SetSize(48); font.SetColor(CColor::Red()); font.DrawText(250, 300, "MISSION COMPLETED");
	}

	healthBar.Draw(g);

	if (level == 3 && action == true) 
	{
		avatar.Draw(g);
		AlienHB.Draw(g);

	}
	
	crosshair.Draw(g);
}

// ----------------   Draw 3D world -------------------------
void CMyGame::OnRender3D(CGraphics* g)
{

	CameraControl(g);


	// ------- Draw your 3D Models here ----------

	floor.Draw(g);

	boxes.Draw(g);
	walls.Draw(g);
	player.Draw(g);
	ogros.Draw(g);
    coins.Draw(g);
	tanks.Draw(g);
	pallettes.Draw(g);
	ammoBoxes.Draw(g);
	medicKits.Draw(g);
	shots.Draw(g);
	enemyShots.Draw(g);
	rocketBoxes.Draw(g);
	rockets.Draw(g);
	buildings.Draw(g);
	grassList.Draw(g);
	aliens.Draw(g);
	aliens2.Draw(g);
	hunters.Draw(g);
	lockers.Draw(g);
	objects.Draw(g);
	if(level == 1) goal.Draw(g);

	if (level == 2)
	{
	  door1.Draw(g);
	  button1.Draw(g);
	  door2.Draw(g);
	  button2.Draw(g);
	  door3.Draw(g);
	  button3.Draw(g);
    }
	if (level == 3)
	{
		button1.Draw(g);
		door1.Draw(g);
		door2.Draw(g);
		door3.Draw(g);
		door4.Draw(g);
		door5.Draw(g);
		door6.Draw(g);
		door7.Draw(g);
		door8.Draw(g);
		door9.Draw(g);
	}

	//Line of sight
	if (!IsGameOver()) 
	{
		static int trigger = 0;
		if (trigger == 0)
		{
			for (CModel* pOgro : ogros)
			{
				if (pOgro->GetSpeed() > 0 && pOgro->HitTest(player.GetPositionV(), 1500))
				{
					ray.SetPositionV(pOgro->GetPositionV(), player.GetPositionV());
					ray.SetColor(CColor::Red());
					CVector p = pOgro->GetPositionV() - player.GetPositionV();
					p.Normalize();
					//cout << p.Dot(pOgro->GetDirectionV()) << endl;
					if (p.Dot(pOgro->GetDirectionV()) <= 0)
					{
						lineofsight = true;
						for (CModel* pWall : walls)
						{
							if (ray.HitTest(pWall)) lineofsight = false;
						}
						for (CModel* pTank : tanks)
						{
							if (ray.HitTest(pTank)) lineofsight = false;
						}
						if (ray.HitTest(&door1) || ray.HitTest(&door2) || ray.HitTest(&door3)) lineofsight = false;
						for (CModel* pGrass : grassList)
						{
							if (ray.HitTest(pGrass)) lineofsight = false;
						}
						if (lineofsight == true)
						{
							ray.Draw(g);
							enemyAlert.Play("enemyAlert.wav");
							enemyAttack = true;
						}

					}

				}


			}
		}
		if (trigger++ == 40) trigger = 0;  // reset trigger1 counter
	}
	
	
	//ShowBoundingBoxes();
	ShowCoordinateSystem();
	
}

void CMyGame::CameraControl(CGraphics* g)
{
	// game world tilt 
    float tilt=70;
	float scale = 1.5f;

	if (IsKeyDown(SDLK_c)) 
	{
		scale = scale - 1.0f;

	}

	// ------ Global Transformation Functions -----

    glScalef( scale,scale,scale); // scaling the game world
	glTranslatef(0,-50,0);        // move game world down 
	
	glRotatef(tilt,1,0,0);	      // tilt game world around x axis
	
	// ---- rotate game world to have the origin in bottom left hand corner -----
    glRotatef(90,0,1,0);  

	// translate game world to player position 
	glTranslatef(-player.GetX(), 0, -player.GetZ());    

	UpdateView();
	Light.Apply();
}


// called at the start of a new game - display menu here
void CMyGame::OnDisplayMenu()
{
	

	background_M.Stop();
	background_M2.Stop();
	background_M3.Stop();
	menu.Stop();
	menu.Play("menu.wav");
	HideMouse();
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{	
	lineofsight = false;
	enemyAttack = false;
	action = false;
	gamewon = false;
	start = false;
	healthBar.SetHealth(100);
	AlienHB.SetHealth(100);
	ammo = 6;
	aidKit = 0;

	if (level == 3) 
	{
		door8.Close();
		door9.Close();
	}
	
	HideMouse();

}


// called when Game is Over
void CMyGame::OnGameOver()
{
	lineofsight = false;
	
}

// one time termination code
void CMyGame::OnTerminate()
{
	
}

// -------    Keyboard Event Handling ------------

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame();

	//selecting level on menu screen
	if (IsMenuMode()) 
	{
		if (sym == SDLK_1) SetupLevel1(); StartGame();
		if (sym == SDLK_2) SetupLevel2(); StartGame();
		if (sym == SDLK_3) SetupLevel3(); StartGame();
	}

	if (sym == SDLK_SPACE)
	{
		if (IsPaused()) ResumeGame();
		else PauseGame();

	}
	if (sym == SDLK_F2) NewGame();
	
	
	// ------- Door Button Interaction -------------------
	// activate door when E is pressed and player colliding with button.
	// Status determines whether the door is opening or closing.
	if (level==2 && sym == SDLK_e && player.HitTest(&button1) && door1.GetStatus()==0 ) 
	{
		door1.Open();
		button1.SetColor( CColor::Green());
		doorAlarm.Play("alarm.wav");

		
	}
	else if (level ==2 && sym == SDLK_e && player.HitTest(&button1) && door1.GetStatus()==1 ) 
	{
		door1.Close();
		button1.SetColor( CColor::Red());
		doorAlarm.Play("alarm.wav");
	}
	if (level == 2 && sym == SDLK_e && player.HitTest(&button2) && door2.GetStatus() == 0)
	{
		door2.Open();
		button2.SetColor(CColor::Green());
		doorAlarm.Play("alarm.wav");

	}
	else if (level == 2 && sym == SDLK_e && player.HitTest(&button2) && door2.GetStatus() == 1)
	{
		door2.Close();
		button2.SetColor(CColor::Red());
		doorAlarm.Play("alarm.wav");
	}
	if (level == 2 && sym == SDLK_e && player.HitTest(&button3) && door3.GetStatus() == 0)
	{
		door3.Open();
		button3.SetColor(CColor::Green());
		doorAlarm.Play("alarm.wav");

	}
	else if (level == 2 && sym == SDLK_e && player.HitTest(&button3) && door3.GetStatus() == 1)
	{
		door3.Close();
		button3.SetColor(CColor::Red());
		doorAlarm.Play("alarm.wav");
	}
	if (level == 3 && sym == SDLK_e && player.HitTest(&button1)/* && door1.GetStatus() == 0*/)
	{
		menu.Stop();
		background_M3.Play("level3.wav", -1);
		door2.Open();
		door3.Open();
		door4.Open();
		door5.Open();
		door6.Open();
		door7.Open();
		door8.Open();
		door9.Open();
		button1.SetColor(CColor::Green());
		doorAlarm.Play("alarm.wav");
		action = true;

	}
  
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	
}

// -----  Mouse Events Handlers -------------

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	if (IsGameMode())
	{
		// project screen to floor coordinates
		CVector pos = ScreenToFloorCoordinate(x, y);
		player.SetDirectionAndRotationToPoint(pos.x, pos.z);
	}

	crosshair.SetPosition(x, y);
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{    
	if (IsMenuMode())
	{
		if (crosshair.HitTest(&screen1)) { SetupLevel1(); StartGame(); }
		if (crosshair.HitTest(&screen2)) { SetupLevel2(); StartGame(); }
		if (crosshair.HitTest(&screen3)) { SetupLevel3(); StartGame(); }
	}
	
	if (!IsMenuMode() && ammo > 0)
	{
		// create a new shot as a short line
		CModel* pShot = new CLine(player.GetPositionV(), 50);
		// rotation and direction same as the player
		pShot->SetColor(CColor::Yellow());
		silencerShot.Play("silencerShot.wav");
		pShot->SetDirection(player.GetRotation());
		pShot->SetRotation(player.GetRotation());
		pShot->SetSpeed(1500);
		pShot->Die(2000);
		shots.push_back(pShot);
		ammo--;

	}
	else emptyGun.Play("emptyGun.wav");

}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
	

	//player's health can be max. 100
	if (aidKit > 0 && healthBar.GetHealth() == 100)
	{
		//nothing to do
		error.Play("error.wav");
	}
	else if (aidKit > 0 && healthBar.GetHealth() < 75) 
	{
		healthBar.SetHealth(healthBar.GetHealth() + 25);
		aidKit--;
		healing.Play("healing.wav");
	}
	else if (aidKit > 0 && healthBar.GetHealth() > 75) 
	{
		healthBar.SetHealth(100);
		aidKit--;
		healing.Play("healing.wav");
	}
	else if (aidKit == 0)
	{
		//no aidkits on the inventory
		error.Play("error.wav");
	}
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
