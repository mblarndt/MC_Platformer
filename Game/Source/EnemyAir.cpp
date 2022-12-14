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
	pbody->body->SetGravityScale(0.0f);

	return true;
}

bool EnemyAir::Update()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);
	/*pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y), pbody->body->GetPosition(), true);*/

	app->pathfinding->ClearLastPath();
	
	if (app->scene->playerptr->playerMoving) {
		
		startTile = app->map->WorldToMap((position.x - 32) - app->render->camera.x, position.y - app->render->camera.y);
		endTile = app->map->WorldToMap(app->scene->playerptr->position.x - app->render->camera.x, app->scene->playerptr->position.y - app->render->camera.y);

		app->pathfinding->CreatePath(startTile, endTile);

		// Get path to make the pathfinding
		if (app->pathfinding->GetLastPath() != nullptr) {
			const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
			if (path->Count() > 0) {
				iPoint pos = app->map->MapToWorld(path->At(0)->x, path->At(0)->y);

				b2Vec2 movVec = b2Vec2((position.x - pos.x + app->render->camera.x), (pos.y - position.y + app->render->camera.y));
				LOG("movVec x speed: %d  movVec y speed: %d", movVec.x, movVec.y);
				pbody->body->SetLinearVelocity(movVec);
			}
		}
	}
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


void EnemyAir::InitSpawn(pugi::xml_node itemNode)
{
	position.x = itemNode.attribute("x").as_int();
	position.y = itemNode.attribute("y").as_int();
	texturePath = "Assets/Textures/ghosties.png";//parameters.attribute("texturepath").as_string();

	width = 40;
	height = 46;

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
}