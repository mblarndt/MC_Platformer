#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.child("startpos").attribute("x").as_int();
	position.y = parameters.child("startpos").attribute("y").as_int();
	texturePath = parameters.child("texture").attribute("path").as_string();
	width = parameters.child("texture").attribute("width").as_int();
	height = parameters.child("texture").attribute("height").as_int();
	speed = parameters.child("movement").attribute("speed").as_int();
	jumpforce = parameters.child("movement").attribute("jumpforce").as_float();
	jumpsteps = parameters.child("movement").attribute("jumpsteps").as_int();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}


b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);


bool Player::Update()
{


	// L07 TODO 5: Add physics to the player - updated player position using physics
	pbody->body->SetFixedRotation(true);
	b2Vec2 vel = pbody->body->GetLinearVelocity();


	//L02: DONE 4: modify the position of the player using arrow keys and render the texture

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		jumpcount++;
		if (jumpcount == 1) {
			remainingJumpSteps = jumpsteps;
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		velocitx = b2Vec2(speed * (-1), -GRAVITY_Y);
	}

	else if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		velocitx = b2Vec2(-speed, -GRAVITY_Y);
	}

	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		//position.x -= 1;
		velocitx = b2Vec2(speed, -GRAVITY_Y);
	}



	if (remainingJumpSteps > 0) {
		vel.y = -jumpforce;//upwards - don't change x velocity
		pbody->body->SetLinearVelocity(b2Vec2(velocitx.x, vel.y));
		remainingJumpSteps--;
	}
	else {
		pbody->body->SetLinearVelocity(b2Vec2(velocitx.x, velocitx.y));
	}



	



	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Player::CleanUp()
{

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	position.x = data.child("camera").attribute("x").as_int();
	position.y = data.child("camera").attribute("y").as_int();

	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Player::SaveState(pugi::xml_node& data)
{
	pugi::xml_node player = data.append_child("position");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;


	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::FLOOR:
		LOG("Collision FLOOR");
		jumpcount = 0;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}
