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
#include "Timer.h"

Bullet::Bullet(pugi::xml_node paras) : Entity(EntityType::BULLET)
{
	name.Create("bullet");
	parameters = paras;

}

Bullet::~Bullet() {}

bool Bullet::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	radius = 7.5;
	texturePath = "Assets/Textures/slimeball.png";


	return true;
}

bool Bullet::Start() {



	return true;
}

bool Bullet::Update()
{
	
	RenderEntity();

	if (t.ReadSec() > 1) {
		app->entityManager->DestroyEntity(this);
	}
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
		LOG("Bullet Collision PLAYER");
		//app->entityManager->DestroyEntity(this);
		break;
	case ColliderType::DEATH:
		LOG("Bullet Collision DEATH");
		app->entityManager->DestroyEntity(this);
		break;
	case ColliderType::FLOOR:
		LOG("Bullet Collision FLOOR");
		break;
	case ColliderType::ENEMY:
		LOG("Bullet Collision ENEMY");
		break;
	case ColliderType::UNKNOWN:
		LOG("Bullet Collision UNKNOWN");
		break;
	}

}

void Bullet::BulletInitialisation(pugi::xml_node itemNode, int x, int y, int direction)
{
	t.Start();
	
	position.x = x + (direction * 35);
	position.y = y;
	radius = 7.5;
	texturePath = "Assets/Textures/slimeball.png";

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 4: Add a physics to an bullet - initialize the physics body
	pbody = app->physics->CreateCircle(position.x + (2*radius), position.y + (2 * radius), radius, DYNAMIC);

	LOG("Create Bullet Body");

	pbody->ctype = ColliderType::BULLET;

	pbody->listener = this;

	pbody->body->ApplyLinearImpulse(b2Vec2(direction * 2.3, 0), pbody->body->GetPosition(), true);
}

void Bullet::RenderEntity()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - radius;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - radius;
	app->render->DrawTexture(texture, position.x, position.y);
}
