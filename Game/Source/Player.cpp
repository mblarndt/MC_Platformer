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
	texturePath = parameters.child("textures").child("player").attribute("path").as_string();
	width = parameters.child("textures").child("player").attribute("width").as_int();
	height = parameters.child("textures").child("player").attribute("height").as_int();

	deathPath = parameters.child("textures").child("death").attribute("path").as_string();
	deathWidth = parameters.child("textures").child("death").attribute("width").as_int();
	deathHeight = parameters.child("textures").child("death").attribute("height").as_int();

	finishPath = parameters.child("textures").child("finish").attribute("path").as_string();
	finishWidth = parameters.child("textures").child("finish").attribute("width").as_int();
	finishHeight = parameters.child("textures").child("finish").attribute("height").as_int();

	//Audio Variables
	hitFxPath = parameters.child("audio").child("hit").attribute("path").as_string();
	pickCoinFxPath = parameters.child("audio").child("pickcoin").attribute("path").as_string();
	backmusicPath = parameters.child("audio").child("music").attribute("path").as_string();

	//Player Variables
	speed = parameters.child("movement").attribute("speed").as_int();
	jumpforce = parameters.child("movement").attribute("jumpforce").as_float();
	jumpsteps = parameters.child("movement").attribute("jumpsteps").as_int();
	

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texDeath = app->tex->Load(deathPath);
	texFinish = app->tex->Load(finishPath);
	
	//Initialize Audio Fx
	hitFxId = app->audio->LoadFx(hitFxPath);

	//Initialize States and Values 
	startGame = false;
	camMoved = false;
	remainingPixels = 0;
	jumpsteps = 3;
	remainingJumpSteps = jumpsteps;
    jumpStart_counter = 4;
	
    movementRight.PushBack({ 0, 80, 60, 40 });
	movementRight.PushBack({ 60, 80, 60, 40 });
	movementRight.loop = true;
	movementRight.speed = 0.1f;
	
	movementLeft.PushBack({ 120, 80, 60, 40 });
	movementLeft.PushBack({ 180, 80, 60, 40 });
	movementLeft.loop = true;
	movementLeft.speed = 0.1f;


	idle.PushBack({ 0, 0, 60, 40 });
	idle.PushBack({ 60, 40, 60, 40 });
	idle.loop = true;
	idle.speed = 0.1f;


	jumpStart.PushBack({298, 48, 60, 40});
	jumpStart.PushBack({358, 48, 60, 40});
	jumpStart.PushBack({418, 48, 60, 40});
	jumpStart.PushBack({478, 48, 60, 40});
	jumpStart.loop = false;
	jumpStart.speed = 0.2f;

	jumpEnd.PushBack({118, 48, 60, 40});
	jumpEnd.PushBack({178, 48, 60, 40});
	jumpEnd.PushBack({238, 48, 60, 40});
	jumpEnd.PushBack({298, 48, 60, 40});
	jumpEnd.loop = false;
	jumpStart.speed = 0.2f;

	jumpUp.PushBack({0, 40, 60, 40});
	jumpUp.loop = true;

	jumpDown.PushBack({60, 40, 60, 40});
	jumpDown.loop = true;
	
	
	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width/2), position.y + (height/2), width, height, bodyType::DYNAMIC);
  
	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);


	app->render->camera.x = menu.x;

	currentAnimation = &idle;

	return true;
}

bool Player::Update()
{
	//Check Velocity of Physical Body
	b2Vec2 v = pbody->body->GetLinearVelocity();
	b2Vec2 vel = pbody->body->GetLinearVelocity();

	//Activate Game
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		app->audio->PlayMusic(backmusicPath);
		startGame = true;
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

		//Main Loop After Transition
		else {

			if (playerDeath == false) {
				//Camera follow
				if (position.x > (camOffset) && position.x < (4222 - (1024 - camOffset))) {
					app->render->camera.x = -(position.x) + camOffset;
					app->render->camera.y = menu.y;
				}

				//L02: DONE 4: modify the position of the player using arrow keys and render the texture
				if (v.y == 0) {
					if (v.x < 0)	currentAnimation = &movementLeft;

					if (v.x > 0)	currentAnimation = &movementRight;

					if (v.x == 0)	currentAnimation = &idle;

					jumpcount = 2;
				}
				if (v.y > 0) {
					//if (jumpStart.HasFinished()) {
					currentAnimation = &jumpDown;
					jumpStart.Reset();
					//}
				}

				if (v.y < 0) {
					//if (jumpStart.HasFinished()) {
					currentAnimation = &jumpUp;
					jumpStart.Reset();
					//}
				}
				//if (v.y == 0 && v.x != 0) currentAnimation = &idle;

				/*----------------------------Player Movement Variation 2--------------------------*/
					if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
						velocitx.x = -speed;
					}

					else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
						velocitx.x = speed;
					}

					//jump
					else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
						if (jumpcount <= 3) {
							pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpforce), pbody->body->GetPosition(), true);
							jumpcount++;
						}
					}
					else {
						velocitx.y = pbody->body->GetLinearVelocity().y;
						pbody->body->SetLinearVelocity(velocitx);
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

				currentAnimation->Update();
				SDL_Rect rect = currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect);
			}

			//When Player collides with Lava he spawns at start again
			if (playerDeath == true) {
				currentAnimation = &jumpStart;
				if (frameCounter < 30) {
					SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
					app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
					frameCounter++;
				}
				else {
					SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
					app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
					SDL_Rect rect = { 0, 0, 1024, 480 };
					if (position.x < 2944)	app->render->DrawTexture(texDeath, position.x - camOffset, 0, &rect);
					else app->render->DrawTexture(texDeath, 3196, 0, &rect);
					//frameCounter++;
					if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
						position.x = spawn.x;
						position.y = spawn.y;
						pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
						velocitx.x = 0;
						playerDeath = false;
					}
				}
			}

			//If Player finished Level
			if (levelFinish) {
				SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
				SDL_Rect rect = { 0, 0, 1024, 480 };
				app->render->DrawTexture(texFinish, position.x - 965, 0, &rect);

				if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
					position.x = spawn.x;
					position.y = spawn.y;
					pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
					velocitx.x = 0;
					levelFinish = false;
				}
			}
			
			Debug();
		}
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
		LOG("Collision DEATH");
		frameCounter = 0;
		app->audio->PlayFx(hitFxId);
		playerDeath = true;
		break;
	case ColliderType::FINISH:
		LOG("Collision FINISH");
		levelFinish = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

void Player::Debug() {
	if (app->input->GetKey(SDL_SCANCODE_F1) || app->input->GetKey(SDL_SCANCODE_F3)) {
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		position.x = spawn.x;
		position.y = spawn.y;
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
		velocitx.x = 0;
	}
}
