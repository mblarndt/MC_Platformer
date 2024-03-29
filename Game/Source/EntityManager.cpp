#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "Bullet.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "EnemyAir.h"
#include "EnemyFloor.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		if(item->data->active) ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, pugi::xml_node paras)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player(paras);
		break;
	case EntityType::ITEM:
		entity = new Item(paras);
		break;
	case EntityType::BULLET:
		entity = new Bullet(paras);
		break;
	case EntityType::ENEMYAIR:
		entity = new EnemyAir(paras);
		break;
	case EntityType::ENEMYFLOOR:
		entity = new EnemyFloor(paras);
		break;

	default: break;
	}

	// Created entities are added to the list
	entity->Awake();
	entity->Start();
	AddEntity(entity);
	
	

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	entity->toDelete = true;
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update();

		if (pEntity->toDelete == true) {
			entities.Del(item);
			item->data->CleanUp();
		}
	}

	return ret;
}