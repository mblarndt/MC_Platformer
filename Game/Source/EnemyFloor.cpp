#include "EnemyFloor.h"
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

EnemyFloor::EnemyFloor(pugi::xml_node paras) : Entity(EntityType::ENEMYFLOOR)
{
	name.Create("EnemyFloor");
	parameters = paras;
}

EnemyFloor::~EnemyFloor() {

}

bool EnemyFloor::Awake() {

	InitSpawn(parameters);
	return true;
}

bool EnemyFloor::Start() {

	health = maxHealth = 2;

	return true;
}

bool EnemyFloor::Update()
{
	if (app->scene->gamePaused == false) {
		if (health <= 0) {
			pugi::xml_node item;
			app->scene->CreateItem(item, Item::ItemType::HEALTH, position.x, position.y);

			pbody->body->DestroyFixture(pbody->body->GetFixtureList());
			app->entityManager->DestroyEntity(this);
		}
		FindPath();
		Move();
		RenderEntity();
		UpdateAnim();
	}
	return true;
}

// IMPORTANT FOR MEMORY MANAGEMENT
bool EnemyFloor::CleanUp()
{

	return true;
}

bool EnemyFloor::LoadState(pugi::xml_node& data)
{


	return true;
}

bool EnemyFloor::SaveState(pugi::xml_node& data)
{

	return true;
}

void EnemyFloor::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact)
{
	LOG("EnemyFloor Collision");
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		break;
	case ColliderType::BULLET:
		LOG("Item Collision DEATH");
		pbody->body->ApplyLinearImpulse(b2Vec2(direction * 3, 0), pbody->body->GetPosition(), true);
		health--;
		break;
	case ColliderType::DEATH:
		LOG("Item Collision DEATH");
		health = 0;
		break;
	case ColliderType::UNKNOWN:
		//LOG("Item Collision UNKNOWN");
		break;
	}
}

void EnemyFloor::Debug() {

}

void EnemyFloor::InitSpawn(pugi::xml_node itemNode)
{
	position.x = itemNode.attribute("x").as_int();
	position.y = itemNode.attribute("y").as_int();
	texturePath = "Assets/Textures/ghosties.png";

	spawn = iPoint(position.x, position.y);

	width = 48;
	height = 48;
	idle.width = leftanim.width = rightanim.width = width;
	idle.height = leftanim.height = rightanim.height = height;

	// Initilize textures
	texture = app->tex->Load(texturePath);

	// Animations
	idle.startCol = 0;
	idle.endCol = 3;
	idle.row = 4;
	idle = app->animation->CreateAnimation(idle, true, 0.05f);

	leftanim.startCol = 0;
	leftanim.endCol = 3;
	leftanim.row = 5;
	leftanim = app->animation->CreateAnimation(leftanim, true, 0.12f);

	rightanim.startCol = 0;
	rightanim.endCol = 3;
	rightanim.row = 6;
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

void EnemyFloor::Move()
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
	pbody->body->ApplyLinearImpulse(b2Vec2(impulse.x, 0), pbody->body->GetWorldCenter(), true);

	if (app->scene->playerptr->deadTextureOn)
	{
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(spawn.x), PIXEL_TO_METERS(spawn.y)), 0);
	}
}


void EnemyFloor::FindPath()
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

void EnemyFloor::RenderEntity()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);

	if (!app->scene->playerptr->deadTextureOn)
	{
		HealthBar();
		currentAnimation->Update();
		SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect1);
	}
}

void EnemyFloor::UpdateAnim()
{
	b2Vec2 currSpeed = pbody->body->GetLinearVelocity();

	if (currSpeed.x > 1)
	{
		direction = 1;
		currentAnimation = &rightanim;
	}
	else if (currSpeed.x < -1)
	{
		direction = -1;
		currentAnimation = &leftanim;
	}
	else
	{
		currentAnimation = &idle;
	}
}

void EnemyFloor::HealthBar() {
	healthPerc = ((100 / maxHealth) * health);
	SDL_Rect healthbar = { (position.x - 5), position.y - 10, healthPerc * 0.5, 5 };
	app->render->DrawRectangle(healthbar, 255, 0, 0, 255, true, true);
}