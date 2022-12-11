#include "Enemy.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//Get and initialize Enemy parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();


	return true;
}

bool Enemy::Start() {

	// Initilize textures


	// Initialize Audio Fx


	// Initialize States and Values 


	// Animations


	// Add physics to the enemy - initialize physics body


	// Assign collider type


	// Activate Collision Detection


	return true;
}

bool Enemy::Update()
{
	
	return true;
}

// IMPORTANT FOR MEMORY MANAGEMENT
bool Enemy::CleanUp()
{

	return true;
}

bool Enemy::LoadState(pugi::xml_node& data)
{


	return true;
}

bool Enemy::SaveState(pugi::xml_node& data)
{

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

}

void Enemy::Debug() {

}
