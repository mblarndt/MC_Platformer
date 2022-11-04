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
	camOffset = parameters.child("cam").attribute("offset").as_int();

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

	movement.PushBack({ 15, 90, 30, 30 });
	movement.PushBack({ 74, 90, 30, 30 });

	movement.loop = true;
	movement.speed = 0.1f;

	idle.PushBack({15, 8, 44, 32});
	idle.PushBack({75, 8, 44, 32});
	idle.loop = true;
	idle.speed = 0.1f;

	jumpStart.PushBack({298, 48, 64, 32});
	jumpStart.PushBack({358, 48, 64, 32});
	jumpStart.PushBack({418, 48, 64, 32});
	jumpStart.PushBack({478, 48, 64, 32});
	jumpStart.loop = false;
	jumpStart.speed = 0.2f;

	jumpEnd.PushBack({118, 48, 64, 32});
	jumpEnd.PushBack({178, 48, 64, 32});
	jumpEnd.PushBack({238, 48, 64, 32});
	jumpEnd.PushBack({298, 48, 64, 32});
	jumpEnd.loop = false;
	jumpStart.speed = 0.2f;

	jumpUp.PushBack({14, 48, 30, 32});
	jumpUp.loop = true;

	jumpDown.PushBack({76, 48, 30, 32});
	jumpDown.loop = true;
	
	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, bodyType::DYNAMIC);

	//Add Fixtures to detect collision direction
	//b2FixtureDef myFixR;
	//b2FixtureDef myFixL;
	//b2PolygonShape polyL;
	//b2PolygonShape polyR;
	//myFixR.density = 0;
	//myFixL.density = 0;
	//polyL.SetAsBox(0.01, 0.01, b2Vec2(0.3,0),0);
	//polyR.SetAsBox(0.01, 0.01, b2Vec2(-0.3, 0), 0);
	//myFixR.shape = &polyL;
	//myFixL.shape = &polyR;
	//pbody->body->CreateFixture(&myFixR);
	//pbody->body->CreateFixture(&myFixL);
  
	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);
	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	hurtFxId = app->audio->LoadFx("Assets/Audio/Fx/hurt.ogg");

	app->render->camera.x = menu.x;

	currentAnimation = &idle;

	return true;
}

b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);

bool Player::Update()
{
	//Check Velocity of Physical Body
	b2Vec2 v = pbody->body->GetLinearVelocity();
	b2Vec2 vel = pbody->body->GetLinearVelocity();

	//Activate Game
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		startGame = true;
		app->audio->PlayMusic("Assets/Audio/Music/Sweden - C418.ogg");
	}

	if (startGame == true) {
		
		//Camera Transition from StartScreen to Player
		if (camMoved == false) {
			currentAnimation = &idle;
			if (remainingPixels < (spawn.x - (camOffset))) {
				remainingPixels += 10;
				app->render->camera.x = -(remainingPixels);
			}
			else {camMoved = true;}
		}

		//When Camera Transition has played it will start in normal Loop
		else {

			//Camera follow
			if (position.x > (camOffset) && position.x < (4160 - (1024-camOffset)) || position.x >(4160) && position.x < (5440 - (1024 - camOffset))) {
				app->render->camera.x = -(position.x) + camOffset;
				app->render->camera.y = menu.y;
			}

			//L02: DONE 4: modify the position of the player using arrow keys and render the texture
			if (v.y == 0) {
				if (v.x < 0)	currentAnimation = &movement;

				if (v.x > 0)	currentAnimation = &movement;

				if (v.x == 0)	currentAnimation = &idle;

				jumpcount = 2;
			}
			if (v.y > 0) {
				if (jumpStart.HasFinished()) {
					currentAnimation = &jumpDown;
					jumpStart.Reset();
				}
			}

			if (v.y < 0) {
				if (jumpStart.HasFinished()) {
					currentAnimation = &jumpUp;
					jumpStart.Reset();
				}
			}

			/*----------------------------Player Movement Variation 2--------------------------*/
				if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
					pbody->body->SetLinearVelocity(b2Vec2(-speed, v.y));
				}

				else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
					pbody->body->SetLinearVelocity(b2Vec2(speed, v.y));
				}

				// if not pressing anything
				// else
				// {
				// 	b2Vec2 v = pbody->body->GetLinearVelocity();
				// 	pbody->body->SetLinearVelocity(v);
				// }

				//jump
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
					if (jumpcount > 0) {
						if (currentAnimation != &jumpStart)
							jumpStart.Reset();
						
						currentAnimation = &jumpStart;

						pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpforce), pbody->body->GetPosition(), true);
						jumpcount--;
					}
				}


			/*----------------------------Player Movement Variation 2--------------------------*
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
				velocitx = b2Vec2(speed, -GRAVITY_Y);
			}

			//Jump Action
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


			//When Player collides with Lava he spawns at start again
			if (spawnStart == true) {
				position.x = spawn.x;
				position.y = spawn.y;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);

				velocitx.x = 0;
				spawnStart = false;
			}
		}

		Debug();

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

void Player::Debug() {
	if (app->input->GetKey(SDL_SCANCODE_F1) || app->input->GetKey(SDL_SCANCODE_F3)) {
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		spawnStart = true;

	}
}
