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

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	width = parameters.attribute("width").as_int();
	height = parameters.attribute("height").as_int();

	texturePath = "Assets/Textures/slimeball.png";
	radius = 10;

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// L07 TODO 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateCircle(position.x+(width/2), position.y+(height/2), radius, DYNAMIC);

	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Item::Update()
{
	// L07 TODO 4: Add a physics to an item - update the position of the object from the physics.
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x)- (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y)- (height / 2);
	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Item::CleanUp()
{
	return true;
}