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

EnemyAir::EnemyAir() : Entity(EntityType::ENEMYAIR)
{
	name.Create("EnemyAir");
}

EnemyAir::~EnemyAir() {

}

bool EnemyAir::Awake() {

	//Get and initialize Enemy parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	width = 40;
	height = 46;

	return true;
}

bool EnemyAir::Start() {

	// Initilize textures
	texture = app->tex->Load(texturePath);


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

	return true;
}

bool EnemyAir::Update()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);
	pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y), pbody->body->GetPosition(), true);
	
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

	// L07 DONE 7: Detect the type of collision

}

void EnemyAir::Debug() {

}
