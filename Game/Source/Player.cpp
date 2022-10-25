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
	width = 30;
	height = 30;

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, bodyType::DYNAMIC);

	b2PolygonShape polyShapeR;
	polyShapeR.SetAsBox(0.1,0.5, b2Vec2(0.5, 0),0);

	b2PolygonShape polyShapeL;
	polyShapeL.SetAsBox(0.1, 0.5, b2Vec2(-0.5, 0), 0);

	b2FixtureDef plR;
	plR.shape = &polyShapeR;

	b2FixtureDef plL;
	plL.shape = &polyShapeL;

	pbody->body->CreateFixture(&plR);
	pbody->body->CreateFixture(&plL);

	return true;
}


b2Vec2 velocity = b2Vec2(10, -GRAVITY_Y);

bool Player::Update()
{


	// L07 TODO 5: Add physics to the player - updated player position using physics
	pbody->body->SetFixedRotation(true);


	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		remainingJumpSteps = 6;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		//position.y += 1;
		int val = velocity.x* (-1);
		velocity = b2Vec2(val, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		//position.x -= 1;
		velocity = b2Vec2(-10, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		//position.x -= 1;
		velocity = b2Vec2(10, -GRAVITY_Y);
	}

	if (remainingJumpSteps > 0)
	{
			pbody->body->ApplyForce(b2Vec2(0, -400), pbody->body->GetPosition(), true);
			remainingJumpSteps--;
	}

	else 
	{
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