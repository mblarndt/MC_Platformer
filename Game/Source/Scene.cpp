#include "App.h"
#include "Input.h"
#include "Textures.h"
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
	

	//L02: DONE 3: Instantiate the player using the entity manager
	playerptr = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, config.child("player"));

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	bool retLoad = app->map->Load();

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}


	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		uint width, height;
		app->win->GetWindowSize(width, height);
		app->render->camera.x = width / 2;

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x.png");
	}

	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {

		// Texture to highligh mouse position 
		mouseTileTex = app->tex->Load("Assets/Maps/path_square.png");

		// Texture to show path origin 
		originTex = app->tex->Load("Assets/Maps/x_square.png");
	}

	
	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	//Camera Movement
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 3;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 3;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 3;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 3;

	// Draw map
	app->map->Draw();

	// L08: DONE 3: Test World to map method
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	if (app->map->mapData.type == MapTypes::MAPTYPE_ISOMETRIC) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x - app->map->mapData.tileWidth / 2,
			mouseY - app->render->camera.y - app->map->mapData.tileHeight / 2);
	}
	if (app->map->mapData.type == MapTypes::MAPTYPE_ORTHOGONAL) {
		mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,
			mouseY - app->render->camera.y);
	}

	//Convert again the tile coordinates to world coordinates to render the texture of the tile
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

	//Test compute path function
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (originSelected == true)
		{
			app->pathfinding->CreatePath(origin, mouseTile);
			originSelected = false;
			app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);
		}
		else
		{
			origin = mouseTile;
			originSelected = true;
			app->pathfinding->ClearLastPath();
		}
	}

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

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

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

void Scene::InitPlayerSpawn(pugi::xml_node itemNode)
{
	playerptr->InitSpawn(itemNode);
}

void Scene::InitEnemySpawn(pugi::xml_node itemNode)
{
	EnemyAir* enemyair = (EnemyAir*)app->entityManager->CreateEntity(EntityType::ENEMYAIR, itemNode);
	enemyair->InitSpawn(itemNode);
}




