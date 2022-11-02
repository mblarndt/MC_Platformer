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

	//Get and initialize Player parameters from XML

	//Menu Position
	menu.x = parameters.child("menu").attribute("x").as_int();
	menu.y = parameters.child("menu").attribute("y").as_int();

	//Initial Startposition
	position.x = parameters.child("startpos").attribute("x").as_int();
	position.y = parameters.child("startpos").attribute("y").as_int();

	//Respawn Position
	spawn.x = parameters.child("startpos").attribute("x").as_int();
	spawn.y = parameters.child("startpos").attribute("y").as_int();

	//Camera Offset from Player

	//Texture Variables
	texturePath = parameters.child("texture").attribute("path").as_string();
	width = parameters.child("texture").attribute("width").as_int();
	height = parameters.child("texture").attribute("height").as_int();

	//Player Variables
	speed = parameters.child("movement").attribute("speed").as_int();
	jumpforce = parameters.child("movement").attribute("jumpforce").as_float();
	jumpsteps = parameters.child("movement").attribute("jumpsteps").as_int();


	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	// Sprite rectangle inside the keys of the function
	// Input the animation steps in order
	movement.PushBack({ 1, 90, 44, 30 });
	movement.PushBack({ 60, 90, 44, 30 });
	movement.loop = true;

	idle.PushBack({15, 8, 30, 32});
	idle.PushBack({75, 8, 30, 32});
	idle.loop = true;

	jumpStart.PushBack({135, 48, 30, 32});
	jumpStart.PushBack({191, 48, 34, 32});
	jumpStart.PushBack({});
	jumpStart.PushBack({});
	jumpStart.PushBack({});
	jumpStart.PushBack({});
	jumpStart.PushBack({});
	jumpStart.loop = false;
	
	jumpEnd.PushBack({});
	jumpEnd.PushBack({});
	jumpEnd.PushBack({});
	jumpEnd.PushBack({});
	jumpEnd.PushBack({});
	jumpEnd.PushBack({});
	jumpEnd.PushBack({});
	jumpEnd.loop = false;

	jumpUp.PushBack({13, 48, 31, 32});
	jumpUp.loop = true;

	jumpDown.PushBack({75, 48, 31, 32});
	jumpDown.loop = true;

	
	
	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, bodyType::DYNAMIC);
  
	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);
	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	hurtFxId = app->audio->LoadFx("Assets/Audio/Fx/hurt.ogg");

	app->render->camera.x = menu.x;


	return true;
}


b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);



bool Player::Update()
{
	// L07 TODO 5: Add physics to the player - updated player position using physics
	
	
	b2Vec2 v = pbody->body->GetLinearVelocity();
	b2Vec2 vel = pbody->body->GetLinearVelocity();


	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		startGame = true;
	}

	if (startGame == true) {

		if (camMoved == false) {
			currentAnimation = &idle;
			if (remainingPixels < (spawn.x - (app->render->playerOffset))) {
				remainingPixels += 10;
				app->render->camera.x = -(remainingPixels);
			}
			else {
					camMoved = true;
			}
		}
		else {


			app->render->camera.x = -(position.x) + app->render->playerOffset;
			//L02: DONE 4: modify the position of the player using arrow keys and render the texture
			if (v.y == 0) {
				if (v.x < 0)	currentAnimation = &movement;

				if (v.x > 0)	currentAnimation = &movement;

				if (v.x == 0)	currentAnimation = &idle;

				jumpcount = 2;
			}
			if (v.y > 0) currentAnimation = &jumpDown;

			if (v.y < 0) currentAnimation = &jumpUp;

			/*----------------------------Player Movement Variation 2--------------------------*
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
					pbody->body->SetLinearVelocity(b2Vec2(-speed, v.y));
				}

				else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
					pbody->body->SetLinearVelocity(b2Vec2(speed, v.y));
				}

				// if not pressing anything
				else
				{
					b2Vec2 v = pbody->body->GetLinearVelocity();
					v.x = speed;
					pbody->body->SetLinearVelocity(v);
				}

				//jump
				if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
					if (jumpcount > 0) {
						pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpforce), pbody->body->GetPosition(), true);
						jumpcount--;
					}
				}


			/*----------------------------Player Movement Variation 2--------------------------*/
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
				if (jumpcount <= 3)
					remainingJumpSteps = jumpsteps;
				jumpcount++;
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

			/*----------------------------End of Variation 2---------------------------------*/

			position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
			position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2);



			if (spawnStart == true) {
				position.x = PIXEL_TO_METERS(spawn.x);
				position.y = PIXEL_TO_METERS(spawn.y);
				pbody->body->SetTransform(b2Vec2(position.x, position.y), 0);
				app->render->camera.x = position.x;
				app->render->camera.y = position.y;
				velocitx.x = 0;
				spawnStart = false;
			}

			
		}
		currentAnimation->Update();

		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
	}
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
	case ColliderType::DEATH:
		LOG("Collision FLOOR");
		app->audio->PlayFx(hurtFxId);
		spawnStart = true;
		

		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}
