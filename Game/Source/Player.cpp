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
	
	// Sprite rectangle inside the keys of the function
	// Input the animation steps in order
	movement.PushBack({  1, 120, 44, 30 });
	movement.PushBack({ 60, 120, 44, 30 });
	movement.loop = true;

	idle.PushBack({15, 8, 30, 32});
	idle.PushBack({75, 8, 30, 32});
	movement.loop = true;

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

	//pbody->body->CreateFixture(&plR);
	//pbody->body->CreateFixture(&plL);

	return true;
}


b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

bool Player::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	pbody->body->SetFixedRotation(true);

	//L02: DONE 4: modify the position of the player using arrow keys and render the texture
	
	currentAnimation = &idle;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		pbody->body->SetLinearVelocity(b2Vec2(-10, 0));
		
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			pbody->body->ApplyLinearImpulse(b2Vec2(-10, -6), pbody->body->GetPosition(), true);
		}
	}

	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		pbody->body->SetLinearVelocity(b2Vec2(10, 0));
		
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			pbody->body->ApplyLinearImpulse(b2Vec2(0, -6), pbody->body->GetPosition(), true);
		}
	}
	else if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -6), pbody->body->GetPosition(), true);
	}
	else
	{
		b2Vec2 v = pbody->body->GetLinearVelocity();
		v.x = 10;
		pbody->body->SetLinearVelocity(v);
	}
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);

	if (velocity == b2Vec2(0, -GRAVITY_Y))	currentAnimation = &idle;

	currentAnimation->Update();
	
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	app->render->DrawTexture(texture, position.x, position.y, &rect);
	return true;
}

bool Player::CleanUp()
{

	return true;
}