#include "Bullet.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"

Bullet::Bullet(pugi::xml_node paras) : Entity(EntityType::BULLET)
{
	name.Create("bullet");
	parameters = paras;
	
}

Bullet::~Bullet() {}

bool Bullet::Awake() {

	
	//radius = parameters.attribute("radius").as_float();
	//texturePath = parameters.attribute("texturepath").as_string();
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	radius = 7.5;
	texturePath = "Assets/Textures/slimeball.png";
	

	return true;
}

bool Bullet::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 TODO 4: Add a physics to an bullet - initialize the physics body
	pbody = app->physics->CreateCircle(position.x+(15), position.y+(15), radius, DYNAMIC);

	pbody->ctype = ColliderType::BULLET;

	pbody->listener = this;

	return true;
}

bool Bullet::Update()
{
	// L07 TODO 4: Add a physics to an bullet - update the position of the object from the physics.
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x)-radius;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y)-radius;
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Bullet::CleanUp()
{
	app->physics->world->DestroyBody(pbody->body);
	return true;
}

void Bullet::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact) {
	LOG("Bullet Collision");
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Bullet Collision BULLET");
		break;
	case ColliderType::DEATH:
		LOG("Bullet Collision DEATH");
		app->entityManager->DestroyEntity(this);
		
		break;
	case ColliderType::UNKNOWN:
		LOG("Bullet Collision UNKNOWN");
		break;
	}

}

void Bullet::BulletInitialisation(pugi::xml_node paras)
{
	name.Create("bullet");

	position.x = 1300;
	position.y = 360;
	radius = 7.5;
	texturePath = "Assets/Textures/slimeball.png";

	texture = app->tex->Load(texturePath);

	// L07 TODO 4: Add a physics to an bullet - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + (15), position.y + (15), radius, DYNAMIC);

	pbody->ctype = ColliderType::BULLET;

	pbody->listener = this;
}

