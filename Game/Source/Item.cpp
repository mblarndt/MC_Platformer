#include "Item.h"
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

Item::Item(pugi::xml_node paras) : Entity(EntityType::ITEM)
{
	name.Create("item");
	
}

Item::~Item() {}

bool Item::Awake() {


	return true;
}

bool Item::Start() {



	return true;
}

bool Item::Update()
{
	if (app->scene->gamePaused == false) {
		// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - radius;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - radius;

		if (!app->scene->playerptr->deadTextureOn)
		{
			app->render->DrawTexture(texture, position.x, position.y);
		}
	}
	return true;
}

bool Item::CleanUp()
{
	app->physics->world->DestroyBody(pbody->body);
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact) {
	LOG("Item Collision");
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		//LOG("Item Collision ITEM");
		app->entityManager->DestroyEntity(this);
		break;
	case ColliderType::DEATH:
		//LOG("Item Collision DEATH");
		app->entityManager->DestroyEntity(this);
		break;
	case ColliderType::UNKNOWN:
		//LOG("Item Collision UNKNOWN");
		break;
	}

}

void Item::ItemInitialisation(pugi::xml_node itemNode, int x, int y)
{
	position.x = itemNode.attribute("x").as_int();
	position.y = itemNode.attribute("y").as_int();

	switch (type) {
	case ItemType::BULLET:
		radius = 7.5;
		texturePath = "Assets/Textures/slimeball.png";
		pbody = app->physics->CreateCircleSensor(position.x + (15), position.y + (15), radius, STATIC);
		pbody->ctype = ColliderType::ITEM_BULLET;
		break;

	case ItemType::DIAMOND:
		radius = 16;
		texturePath = "Assets/Textures/diamond.png";
		pbody = app->physics->CreateCircleSensor(position.x + (15), position.y + (15), radius, STATIC);
		pbody->ctype = ColliderType::ITEM_DIAMOND;
		break;

	case ItemType::HEALTH:
		position.x = x;
		position.y = y;
		radius = 16;
		texturePath = "Assets/Textures/heart.png";
		pbody = app->physics->CreateCircle(position.x + (15), position.y + (15), radius, DYNAMIC);
		pbody->ctype = ColliderType::ITEM_HEALTH;
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -3), pbody->body->GetPosition(), true);
		break;
	}

	//initilize textures
	texture = app->tex->Load(texturePath);

	

	pbody->listener = this;
}

