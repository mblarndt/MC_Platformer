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

Item::Item(pugi::xml_node paras) : Entity(EntityType::ITEM)
{
	name.Create("item");
	parameters = paras;
	
}

Item::~Item() {}

bool Item::Awake() {

	
	//radius = parameters.attribute("radius").as_float();
	//texturePath = parameters.attribute("texturepath").as_string();
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	radius = 7.5;
	texturePath = "Assets/Textures/slimeball.png";

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x+(15), position.y+(15), radius, DYNAMIC);

	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Item::Update()
{
	// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x)-radius;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y)-radius;
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Item::CleanUp()
{
	return true;
}