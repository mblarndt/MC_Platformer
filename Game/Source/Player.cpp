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
#include "Item.h"
#include "Map.h"
#include "Bullet.h"
#include "EntityManager.h"
#include "Window.h"

Player::Player(pugi::xml_node paras) : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	parameters = paras;
}

Player::~Player() {

}

bool Player::Awake() {

	//Get and initialize Player parameters from XML

	//Menu Position
	menu.x = parameters.child("menu").attribute("x").as_int();
	menu.y = parameters.child("menu").attribute("y").as_int();

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
	health = parameters.child("stats").attribute("health").as_int();
	bullets = parameters.child("stats").attribute("bullets").as_int();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	texDeath = app->tex->Load(deathPath);
	texFinish = app->tex->Load(finishPath);
	
	//Initialize Audio Fx
	hitFxId = app->audio->LoadFx(hitFxPath);
	pickCoinFxId = app->audio->LoadFx(pickCoinFxPath);

	//Initialize States and Values 
	startGame = false;
	remainingPixels = 0;

	spriteWidth = column =60;
	spriteHeight = row = 40;

	//Animations
	idle.PushBack({ 0 * column,  0 * row, spriteWidth, spriteHeight });
	idle.PushBack({ 1 * column, 0 * row, spriteWidth, spriteHeight });
	idle.loop = true;
	idle.speed = 0.1f;

    movementRight.PushBack({ 0 * column, 2 * row, spriteWidth, spriteHeight });
	movementRight.PushBack({ 1 * column, 2 * row, spriteWidth, spriteHeight });
	movementRight.loop = true;
	movementRight.speed = 0.1f;
	
	movementLeft.PushBack({ 2 * column, 2 * row, spriteWidth, spriteHeight });
	movementLeft.PushBack({ 3 * column, 2 * row, spriteWidth, spriteHeight });
	movementLeft.loop = true;
	movementLeft.speed = 0.1f;


	jumpUp.PushBack({ 0 * spriteWidth, 1 * spriteHeight, spriteWidth, spriteHeight });
	jumpUp.loop = true;

	jumpDown.PushBack({ 1 * spriteWidth, 1 * spriteHeight, spriteWidth, spriteHeight });
	jumpDown.loop = true;

	jumpEnd.PushBack({ 2 * column, 1 * row, spriteWidth, spriteHeight });
	jumpEnd.PushBack({ 3 * column, 1 * row, spriteWidth, spriteHeight });
	jumpEnd.PushBack({ 4 * column, 1 * row, spriteWidth, spriteHeight });
	jumpEnd.PushBack({ 5 * column, 1 * row, spriteWidth, spriteHeight });
	jumpEnd.loop = false;
	jumpStart.speed = 0.2f;

	jumpStart.PushBack({ 6 * column, 1 * row, spriteWidth, spriteHeight });
	jumpStart.PushBack({ 7 * column, 1 * row, spriteWidth, spriteHeight });
	jumpStart.PushBack({ 8 * column, 1 * row, spriteWidth, spriteHeight });
	jumpStart.PushBack({ 9 * column, 1 * row, spriteWidth, spriteHeight });
	jumpStart.loop = false;
	jumpStart.speed = 0.2f;

	
	
	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width/2), position.y + (height/2), width, height, bodyType::DYNAMIC);


	shapeR.SetAsBox(0.01, 0.01, b2Vec2(0.3, 0), 0);
	shapeL.SetAsBox(0.01, 0.01, b2Vec2(-0.3, 0), 0);
	shapeT.SetAsBox(0.01, 0.01, b2Vec2( 0, -0.3), 0);
	shapeB.SetAsBox(0.01, 0.01, b2Vec2(0, 0.29), 0);
	fixtureDefR.shape = &shapeR;
	fixtureDefL.shape = &shapeL;
	fixtureDefT.shape = &shapeT;
	fixtureDefB.shape = &shapeB;
	fixtureDefR.density = 0;
	fixtureDefL.density = 0.1f;
	fixtureDefT.density = 0.2f;
	fixtureDefB.density = 0.3f;
	pbody->body->CreateFixture(&fixtureDefR);
	pbody->body->CreateFixture(&fixtureDefL);
	pbody->body->CreateFixture(&fixtureDefT);
	pbody->body->CreateFixture(&fixtureDefB);

  
	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);


	app->render->camera.x = menu.x;

	currentAnimation = &idle;

	toDelete = false;

	return true;
}

bool Player::Update()
{

	if (health == 0)
		playerDeath = true;

	//Activate Game
	if (startGame == false) {
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			app->audio->PlayMusic(backmusicPath);
			startGame = true;
		}
	}

	if (startGame == true) {

		//Main Loop starts when CamTransition finished
		if (CamTransition(0, spawn.x)) {
			if (playerDeath == false) {

				/*----------------------------Follow Camera--------------------------*/
				PlayerCamera();
				/*----------------------------Get State of Player--------------------------*/
				StateMachine();
				/*----------------------------Player Movement--------------------------*/
				HandleMovement();
				/*----------------------------Rendering Player--------------------------*/
				RenderEntity();
			}
		}


		//When Player collides with Lava he spawns at start again	
		HandleDeath(playerDeath);

		//If Player finished Level
		HandleFinish(levelFinish);
			
		Debug();
		
	}

	return true;
}

bool Player::CleanUp()
{
	app->physics->world->DestroyBody(pbody->body);
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact) {

	b2Fixture* fix1 = contact->GetFixtureA();
	b2Fixture* fix2 = contact->GetFixtureB();

	if (fix2->GetDensity() == 0) {
		velocitx.x = -speed;
		health = health - 1;
		//LOG("Health: %s", health);
	}
	if (fix2->GetDensity() == 0.1f) {
		velocitx.x = speed;
		health = health - 1;
	}
	if (fix2->GetDensity() == 0.2f) {
	}
	if (fix2->GetDensity() == 0.3f) {
	}

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		//LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		bullets = bullets+1;
		break;
	case ColliderType::PLATFORM:
		//LOG("Collision PLATFORM");
		break;
	case ColliderType::FLOOR:
		//LOG("Collision FLOOR");
		jumpcount = 0;
		break;
	case ColliderType::DEATH:
		//LOG("Collision DEATH");
		frameCounter = 0;
		app->audio->PlayFx(hitFxId);
		playerDeath = true;
		break;
	case ColliderType::FINISH:
		//LOG("Collision FINISH");
		levelFinish = true;
		break;
	case ColliderType::UNKNOWN:
		//LOG("Collision UNKNOWN");
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

void Player::GetState()
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	if (vel.y == 0) {
		grounded = true;
		if (vel.x < 0)
			state = MOVE_LEFT;

		if (vel.x > 0)
			state = MOVE_RIGHT;

		if (vel.x == 0)
			state = IDLE;
	}
	if (vel.y > 0) {
		
		grounded = false;
		if (vel.x > 0)
			state = FALL_LEFT;

		if (vel.x < 0)
			state = FALL_RIGHT;
	}

	if (vel.y < 0) {

		grounded = false;

		if (vel.x > 0)
			state = JUMP_LEFT;

		if (vel.x < 0)
			state = JUMP_RIGHT;
	}
}

void Player::StateMachine()
{
	GetState();

	switch (state) {
		case IDLE: 
			currentAnimation = &idle;
			break;
		case MOVE_RIGHT:
			currentAnimation = &movementRight;
			shootDir = 1;
			break;
		case MOVE_LEFT:
			currentAnimation = &movementLeft;
			shootDir = -1;
			break;
		case JUMP_RIGHT:
			currentAnimation = &jumpUp;
			break;
		case JUMP_LEFT:
			currentAnimation = &jumpUp;
			break;
		case FALL_RIGHT:
			currentAnimation = &jumpDown;
			break;
		case FALL_LEFT:
			currentAnimation = &jumpDown;
			break;
	}

	if (preState == FALL) {
		//currentAnimation = &jumpEnd;
	}
		

	if (grounded)
		jumpcount = 2;

	preState = state;

}

void Player::Shoot()
{
	if (bullets > 0) {
		bullets = bullets - 1;
		pugi::xml_node object;
		object.attribute("x") = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
		object.attribute("y") = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);
		object.attribute("direction") = shootDir;
		app->scene->CreateBullet(object, position.x, position.y, shootDir);
	}
	
}

void Player::HandleMovement()
{
	//Move Left
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		velocitx.x = -speed;
	}

	//Move Right
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		velocitx.x = speed;
	}
	//Jump
	else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		if (jumpcount <= 3) {
			pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpforce), pbody->body->GetPosition(), true);
			jumpcount++;
		}
	}

	//Shoot
	else if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		Shoot();
	}


	else {
		velocitx.y = pbody->body->GetLinearVelocity().y;
		pbody->body->SetLinearVelocity(velocitx);
	}
}

void Player::HandleDeath(bool dead)
{
	if (dead) {
		health = 7;
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

}

void Player::HandleFinish(bool finish)
{
	if (finish) {
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
}

void Player::RenderEntity()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - (width / 2);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - (height / 2)+1;

	currentAnimation->Update();
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect);
}

void Player::PlayerCamera()
{
	if (position.x > (camOffset) && position.x < ((app->map->mapData.width * app->map->mapData.tileWidth) - (app->win->width - camOffset))) {
		app->render->camera.x = -(position.x) + camOffset;
		app->render->camera.y = menu.y;
	}
}

void Player::InitSpawn(pugi::xml_node itemNode)
{
	position.x = spawn.x = itemNode.attribute("x").as_int();
	position.y = spawn.y = itemNode.attribute("y").as_int();
}

bool Player::CamTransition(int start, int stop)
{
	bool ret = false;

	if (remainingPixels < (stop - (camOffset))) {
		remainingPixels += 10;
		app->render->camera.x = -(remainingPixels);
	}
	else 
		ret = true;

	return ret;
}
