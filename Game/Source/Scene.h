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

	void CreateItem(pugi::xml_node itemNode, Item::ItemType type, int x = 0, int y = 0);

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

	bool checkpointReached;

	bool timerStarted;

	bool timer2Started;

	bool clickedSave;

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

	Timer t;
	Timer t2;


};

#endif // __SCENE_H__