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
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	width = 32;
	height = 32;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, bodyType::DYNAMIC);

	return true;
}

bool Player::Update()
{


	// L07 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		remainingJumpSteps = 6;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//position.y += 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		//position.x -= 1;
		velocity = b2Vec2(-10, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		//position.x -= 1;
		velocity = b2Vec2(10, -GRAVITY_Y);
	}

	if (remainingJumpSteps > 0) {
		pbody->body->ApplyForce(b2Vec2(0, -400), pbody->body->GetPosition(), true);
		remainingJumpSteps--;
	}


	if (remainingJumpSteps == 0) {
		pbody->body->SetLinearVelocity(velocity);
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