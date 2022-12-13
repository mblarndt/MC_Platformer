#include "EnemyAir.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "EntityManager.h"

#include "Pathfinding.h"
#include "EntityManager.h"

EnemyAir::EnemyAir(pugi::xml_node paras) : Entity(EntityType::ENEMYAIR)
{
	name.Create("EnemyAir");
}

EnemyAir::~EnemyAir() {

}

bool EnemyAir::Awake() {

	return true;
}

bool EnemyAir::Start() {


	return true;
}

bool EnemyAir::Update()
{

	if (health == 0)
		app->entityManager->DestroyEntity(this);

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);
	

	app->pathfinding->CreatePath(position, app->scene->playerptr->position);
/*
	// Get path to make the pathfinding
	if (app->pathfinding->GetLastPath() != nullptr) {
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();

		iPoint pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);

		b2Vec2 movVec = b2Vec2(pos.x - position.x, pos.y - position.y);

		pbody->body->ApplyLinearImpulse(b2Vec2(movVec.x, movVec.y + GRAVITY_Y), pbody->body->GetPosition(), true);
	}
	else 
*/
	//pbody->body->ApplyLinearImpulse(b2Vec2(0, -GRAVITY_Y), pbody->body->GetPosition(), true);
	currentAnimation->Update();
	SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect1);

	return true;
}

// IMPORTANT FOR MEMORY MANAGEMENT
bool EnemyAir::CleanUp()
{

	return true;
}

bool EnemyAir::LoadState(pugi::xml_node& data)
{


	return true;
}

bool EnemyAir::SaveState(pugi::xml_node& data)
{

	return true;
}

void EnemyAir::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::BULLET:
		//LOG("Item Collision DEATH");
		health = health - 1;
		break;
	case ColliderType::UNKNOWN:
		//LOG("Item Collision UNKNOWN");
		break;
	}

}

void EnemyAir::Debug() {

}


void EnemyAir::InitSpawn(pugi::xml_node itemNode)
{
	position.x = itemNode.attribute("x").as_int();
	position.y = itemNode.attribute("y").as_int();
	texturePath = "Assets/Textures/ghosties.png";//parameters.attribute("texturepath").as_string();

	width = 40;
	height = 46;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	health = 1;
	// Initialize Audio Fx


	// Initialize States and Values 


	// Animations
	idle.PushBack({ 148, 2, width, height });
	idle.PushBack({ 196, 2, width, height });
	idle.PushBack({ 243, 2, width, height });
	idle.loop = true;
	//idle.pingpong = true;
	idle.speed = 0.1f;

	// Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, DYNAMIC);

	// Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	// Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);

	currentAnimation = &idle;
}