#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "EnemyAir.h"
#include "EnemyFloor.h"
#include "GuiButton.h"
struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool SaveState(pugi::xml_node& data);

	bool LoadState(pugi::xml_node& data);

	void CreateItem(pugi::xml_node itemNode);

	void CreateBullet(pugi::xml_node itemNode, int x, int y, int direction);

	void InitPlayerSpawn(pugi::xml_node itemNode);

	void InitEnemyAirSpawn(pugi::xml_node itemNode);
	
	void InitEnemyFloorSpawn(pugi::xml_node itemNode);

	void DebugPathfinding();

	void SpawnPlayer();

	bool SceneStart(int level);
public:

	//L02: DONE 3: Declare a Player attribute 
	Player* playerptr;
	Item* itemptr;
	EnemyAir* enemyairptr;
	EnemyFloor* enemyfloorptr;

	pugi::xml_node params;

	const char* fileName;

	bool toggle;

	bool gamePaused;

private:
	SDL_Rect settingsRect;
	SDL_Texture* settingsBox;
	const char* settingsBoxPath;

	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	SDL_Texture* deathTex;
	const char* deathTexPath;
	int deathTexW;
	int deathTexH;

	SDL_Texture* finishTex;
	const char* finishTexPath;
	int finishTexW;
	int finishTexH;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;


	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;

	GuiControlState preState1;
	GuiControlState preState2;
	GuiControlState preState3;
	GuiControlState preState4;

	const int numButtons = 4;
	GuiButton* buttons[4] = { btn1, btn2, btn3, btn4 };
	GuiControlState* preStates[4] = { &preState1, &preState2, &preState3, &preState4 };

	int click1FxId;
	int click2FxId;

	


};

#endif // __SCENE_H__