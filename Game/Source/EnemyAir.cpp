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

	health = 2;
	return true;
}

bool EnemyAir::Update()
{
	if (health == 0) {
		app->entityManager->DestroyEntity(this);
	}
	FindPath();
	Move();
	RenderEntity();

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

void EnemyAir::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact) {
	LOG("EnemyAir Collision");
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::BULLET:
		LOG("Item Collision DEATH");
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

void EnemyAir::Move()
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	b2Vec2 desiredVel = { 0,0 };
	b2Vec2 impulse;

	if (behaviourState == CHASE) {
		if (path->Count() > 1) {
			const iPoint* tile = path->At(0);
			const iPoint* nextTile = path->At(1);

			b2Vec2 dif = { (float)nextTile->x - tile->x , (float)nextTile->y - tile->y };
			dif.Normalize();
			desiredVel.x = dif.x * speed;

			desiredVel.y = dif.y * speed;

		}
	}
	else
	{
		desiredVel = { 0,0 };
	}
	if (desiredVel.x > 0)
	{
		right = true;
	}
	else if (desiredVel.x < 0)
	{
		right = false;
	}


	impulse = desiredVel - vel;
	float mass = pbody->body->GetMass();
	pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
}


void EnemyAir::FindPath()
{
	iPoint playerTile = app->map->WorldToMap(app->scene->playerptr->position.x, app->scene->playerptr->position.y);
	iPoint enemyTile = app->map->WorldToMap(position.x, position.y);
	int distanceToPlayer = playerTile.DistanceTo(enemyTile);
	if (distanceToPlayer > 10)
	{
		behaviourState = IDLE;
	}
	else {

		if (app->pathfinding->CreatePath(enemyTile, playerTile) != -1) {
			behaviourState = CHASE;
			path = app->pathfinding->GetLastPath();
		}
		else {
			behaviourState = IDLE;
		}
	}
}

void EnemyAir::RenderEntity()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);

	currentAnimation->Update();
	SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, &rect1);
}
