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

EnemyFloor::EnemyFloor() : Entity(EntityType::ENEMYFLOOR)
{
	name.Create("EnemyFloor");
}

EnemyFloor::~EnemyFloor() {

}

bool EnemyFloor::Awake() {

	//Get and initialize Enemy parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	width = 40;
	height = 48;

	return true;
}

bool EnemyFloor::Start() {

	// Initilize textures
	texture = app->tex->Load(texturePath);


	// Initialize Audio Fx


	// Initialize States and Values 
	

	// Animations


	// Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, DYNAMIC);

	// Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	// Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);

	return true;
}

bool EnemyFloor::Update()
{
	
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

void EnemyFloor::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

}

void EnemyFloor::Debug() {

}
