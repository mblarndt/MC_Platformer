#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "Player.h"
#include "Item.h"
#include "Bullet.h"
#include "Pathfinding.h"
#include "EnemyAir.h"
#include "EnemyFloor.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	
	playerparams = config;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
	LOG("Scene PreUpdate Finished");
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	//MainButtons();
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		app->fadeToBlack->SwitchMap(1);

	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
		app->fadeToBlack->SwitchMap(2);


	//GUI();


	//app->guiManager->Draw();
	// Draw map
	app->map->Draw();

	DebugPathfinding();

	return true;
	LOG("Scene Update Finished");
}

bool Scene::PostUpdate()
{
	bool ret = true;



	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::SaveState(pugi::xml_node &data)
{
	pugi::xml_node player = data.append_child("player");

	player.append_attribute("x") = playerptr->position.x;
	player.append_attribute("y") = playerptr->position.y;
	player.append_attribute("health") = playerptr->health;
	player.append_attribute("bullets") = playerptr->bullets;

	pugi::xml_node enemyair = data.append_child("enemyair");

	enemyair.append_attribute("x") = enemyairptr->position.x;
	enemyair.append_attribute("y") = enemyairptr->position.y;
	enemyair.append_attribute("health") = enemyairptr->health;

	pugi::xml_node enemyfloor = data.append_child("enemyfloor");

	enemyfloor.append_attribute("x") = enemyfloorptr->position.x;
	enemyfloor.append_attribute("y") = enemyfloorptr->position.y;
	enemyfloor.append_attribute("health") = enemyfloorptr->health;

	return true;
}

bool Scene::LoadState(pugi::xml_node& data)
{
	playerptr->position.x = data.child("player").attribute("x").as_int();
	playerptr->position.y = data.child("player").attribute("y").as_int();
	playerptr->health = data.child("player").attribute("health").as_int();
	playerptr->bullets = data.child("player").attribute("bullets").as_int();

	playerptr->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(playerptr->position.x),
												PIXEL_TO_METERS(playerptr->position.y)),0);
	playerptr->velocitx.x = 0;


	enemyairptr->position.x = data.child("enemyair").attribute("x").as_int();
	enemyairptr->position.y = data.child("enemyair").attribute("y").as_int();
	enemyairptr->health = data.child("enemyair").attribute("health").as_int();

	enemyairptr->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemyairptr->position.x),
												  PIXEL_TO_METERS(enemyairptr->position.y)), 0);


	enemyfloorptr->position.x = data.child("enemyfloor").attribute("x").as_int();
	enemyfloorptr->position.y = data.child("enemyfloor").attribute("y").as_int();
	enemyfloorptr->health = data.child("enemyfloor").attribute("health").as_int();

	enemyfloorptr->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(enemyfloorptr->position.x),
												    PIXEL_TO_METERS(enemyfloorptr->position.y)), 0);


	app->entityManager->LoadState(data);

	return true;
}

void Scene::CreateItem(pugi::xml_node itemNode)
{
	Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, itemNode);
	item->ItemInitialisation(itemNode);
}

void Scene::CreateBullet(pugi::xml_node itemNode, int x, int y, int direction)
{
	Bullet* bullet = (Bullet*)app->entityManager->CreateEntity(EntityType::BULLET, itemNode);
	bullet->BulletInitialisation(itemNode, x, y, direction);
}

void Scene::InitEnemyAirSpawn(pugi::xml_node itemNode)
{
	enemyairptr = (EnemyAir*)app->entityManager->CreateEntity(EntityType::ENEMYAIR, itemNode);
}

void Scene::InitEnemyFloorSpawn(pugi::xml_node itemNode)
{
	enemyfloorptr = (EnemyFloor*)app->entityManager->CreateEntity(EntityType::ENEMYFLOOR, itemNode);
}

void Scene::DebugPathfinding() {

	if (app->physics->debug) {

		// L12: Get the latest calculated path and draw
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}

		// L12: Debug pathfinding
		iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
		app->render->DrawTexture(originTex, originScreen.x, originScreen.y);
	}
}

bool Scene::SceneStart(int level)
{
	switch (level)
	{
	case 1:
		fileName = "Assets/Maps/Level1.tmx";
		break;
	case 2:
		fileName = "Assets/Maps/Level2.tmx";
	default:
		LOG("No Map found");
		break;
	}
		
	bool retLoad = app->map->Load(fileName);

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Maps/x_square.png");

	LOG("Loaded Tile Textures");

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	SpawnPlayer();

	return retLoad;
}

void Scene::SpawnPlayer() {
	pugi::xml_node playerNode;
	playerptr = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, playerNode);
	playerptr->camTransition = true;
}

//void Scene::GUI() {
//	//Check for Button Click
//	//Load Button
//	if (buttons[0]->state == GuiControlState::PRESSED)
//		app->SaveGameRequest();
//
//	//Save Button
//	if (buttons[1]->state == GuiControlState::PRESSED) {
//		app->LoadGameRequest();
//	}
//
//	//Settings Button
//	if (buttons[2]->state == GuiControlState::PRESSED) {
//	}
//
//	//Menu Button
//	if (buttons[3]->state == GuiControlState::PRESSED) {
//		NoButtons();
//		app->fadeToBlack->FadeToBlackScene("TitleScene", 0.5);
//	}
//
//
//	
//}
//
//
//bool Scene::MainButtons() {
//	buttons[0]->state = GuiControlState::NORMAL;
//	buttons[1]->state = GuiControlState::NORMAL;
//	buttons[2]->state = GuiControlState::NORMAL;
//	buttons[3]->state = GuiControlState::NORMAL;
//
//	return true;
//}
//
//bool Scene::SettingsButtons()
//{
//	return false;
//}
//
//
//bool Scene::NoButtons() {
//	for (int i = 0; i < numButtons; i++) {
//		buttons[i]->state = GuiControlState::DISABLED;
//	}
//
//	return true;
//}
//
//bool Scene::ButtonSetup() {
//	//Button Setup
//	uint w, h;
//	app->win->GetWindowSize(w, h);
//	buttons[0] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "SaveButton", { 100, (int)w / 10,     190, 66 }, this);
//	buttons[1] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "LoadButton", { 100, (int)w / 10 * 3, 190, 66 }, this);
//	buttons[2] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SettingsButton", { 100, (int)w / 10 * 2, 190, 66 }, this);
//	buttons[3] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "MenuButton", { 100 + 200, (int)w / 10,     190, 66 }, this);
//
//	//Load Button Click Sounds
//	click1FxId = app->audio->LoadFx("Assets/Audio/Fx/click1.ogg");
//	click2FxId = app->audio->LoadFx("Assets/Audio/Fx/click2.ogg");
//
//	//MainButtons();
//	//NoButtons();
//
//	return true;
//}



