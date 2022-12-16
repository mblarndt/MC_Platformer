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
	pbody->body->SetGravityScale(0.0f);
	return true;
}

bool EnemyAir::Update()
{
	if (health <= 0) {
		pbody->body->DestroyFixture(pbody->body->GetFixtureList());
		app->entityManager->DestroyEntity(this);
	}

	FindPath();
	Move();
	RenderEntity();
	UpdateAnim();
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

void EnemyAir::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact)
{
	LOG("EnemyAir Collision");
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::BULLET:
		LOG("Item Collision DEATH");
		pbody->body->ApplyLinearImpulse(b2Vec2(direction * 3, 0), pbody->body->GetPosition(), true);
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
	texturePath = "Assets/Textures/ghosties.png";

	width = 48;
	height = 48;
	idle.width = leftanim.width = rightanim.width = width;
	idle.height = leftanim.height = rightanim.height = height;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// Animations
	idle.startCol = 0;
	idle.endCol = 2;
	idle.row = 0;
	idle = app->animation->CreateAnimation(idle, true, 0.12f);

	leftanim.startCol = 2;
	leftanim.endCol = 5;
	leftanim.row = 1;
	leftanim = app->animation->CreateAnimation(leftanim, true, 0.12f);

	rightanim.startCol = 2;
	rightanim.endCol = 5;
	rightanim.row = 2;
	rightanim = app->animation->CreateAnimation(rightanim, true, 0.12f);

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
	if (distanceToPlayer > 8)
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

void EnemyAir::UpdateAnim()
{
	b2Vec2 currSpeed = pbody->body->GetLinearVelocity();

	if (currSpeed.x > 1)
	{
		currentAnimation = &rightanim;
	}
	else if (currSpeed.x < -1)
	{
		currentAnimation = &leftanim;
	}
	else
	{
		currentAnimation = &idle;
	}
}