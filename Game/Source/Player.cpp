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
	lives = parameters.child("stats").attribute("lives").as_int();
	health = parameters.child("stats").attribute("health").as_int();
	bullets = parameters.child("stats").attribute("bullets").as_int();

	speed = parameters.child("movement").attribute("speed").as_int();
	jumpforce = parameters.child("movement").attribute("jumpforce").as_float();

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
	

	//Idle
	idle.row = parameters.child("animations").child("idle").attribute("row").as_int();
	idle.startCol = parameters.child("animations").child("idle").attribute("startcol").as_int();
	idle.endCol = parameters.child("animations").child("idle").attribute("endcol").as_int();

	//MoveRight
	movementRight.row = parameters.child("animations").child("moveright").attribute("row").as_int();
	movementRight.startCol = parameters.child("animations").child("moveright").attribute("startcol").as_int();
	movementRight.endCol = parameters.child("animations").child("moveright").attribute("endcol").as_int();

	//MoveLeft
	movementLeft.row = parameters.child("animations").child("moveleft").attribute("row").as_int();
	movementLeft.startCol = parameters.child("animations").child("moveleft").attribute("startcol").as_int();
	movementLeft.endCol = parameters.child("animations").child("moveleft").attribute("endcol").as_int();

	//Jump Up
	jumpUp.row = parameters.child("animations").child("jumpup").attribute("row").as_int();
	jumpUp.startCol = parameters.child("animations").child("jumpup").attribute("startcol").as_int();
	jumpUp.endCol = parameters.child("animations").child("jumpup").attribute("endcol").as_int();

	//Jump Down
	jumpDown.row = parameters.child("animations").child("jumpdown").attribute("row").as_int();
	jumpDown.startCol = parameters.child("animations").child("jumpdown").attribute("startcol").as_int();
	jumpDown.endCol = parameters.child("animations").child("jumpdown").attribute("endcol").as_int();

	//Jump Down
	jumpStart.row = parameters.child("animations").child("jumpstart").attribute("row").as_int();
	jumpStart.startCol = parameters.child("animations").child("jumpstart").attribute("startcol").as_int();
	jumpStart.endCol = parameters.child("animations").child("jumpstart").attribute("endcol").as_int();

	//Jump Down
	jumpEnd.row = parameters.child("animations").child("jumpend").attribute("row").as_int();
	jumpEnd.startCol = parameters.child("animations").child("jumpend").attribute("startcol").as_int();
	jumpEnd.endCol = parameters.child("animations").child("jumpend").attribute("endcol").as_int();

	spriteHeight = row = parameters.child("animations").attribute("height").as_int();
	spriteWidth = column = parameters.child("animations").attribute("width").as_int();

	idle.width = movementLeft.width = movementRight.width = jumpUp.width = jumpDown.width = jumpStart.width = jumpEnd.width = spriteWidth;
	idle.height = movementLeft.height = movementRight.height = jumpUp.height = jumpDown.height = jumpStart.height = jumpEnd.height = spriteHeight;

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

	//Animations
	idle = app->animation->CreateAnimation(idle, true, 0.1f);
    movementRight = app->animation->CreateAnimation(movementRight, true, 0.1f);
	movementLeft = app->animation->CreateAnimation(movementLeft, true, 0.1f);
	jumpUp = app->animation->CreateAnimation(jumpUp, true, 0.1f);
	jumpDown = app->animation->CreateAnimation(jumpDown, true, 0.1f);
	jumpEnd = app->animation->CreateAnimation(jumpEnd, false, 0.2f);
	jumpStart = app->animation->CreateAnimation(jumpStart, false, 0.2f);	
	
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
	//pbody->body->CreateFixture(&fixtureDefR);
	//pbody->body->CreateFixture(&fixtureDefL);
	//pbody->body->CreateFixture(&fixtureDefT);
	//pbody->body->CreateFixture(&fixtureDefB);

  
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
	if (lives == 0)
		gameOver = true;

	//Activate Game
	if (startGame == false) {
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			app->audio->PlayMusic(backmusicPath);
			startGame = true;
		}
	}

	if (startGame == true) {

		//Main Loop starts when CamTransition finished
		if (CamTransition(0, spawn.x) && levelFinish == false) {


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
		//velocitx.x = -speed;
		//health = health - 1;
		//LOG("Health: %s", health);
	}
	if (fix2->GetDensity() == 0.1f) {
		//velocitx.x = speed;
		//health = health - 1;
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
		if ((physA->body->GetPosition().y > physB->body->GetPosition().y)) {

			if (physA->body->GetPosition().x < physB->body->GetPosition().x) {
				velocitx.x = -speed;
			}
			if (physA->body->GetPosition().x > physB->body->GetPosition().x) {
				velocitx.x = speed;
			}
		}
		
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
	case ColliderType::CHECKPOINT:
		LOG("Collision Checkpoint");
		app->SaveGameRequest();
		break;
	case ColliderType::ENEMY:
		app->audio->PlayFx(hitFxId);
		health = health - 1;
		pbody->body->ApplyLinearImpulse(b2Vec2(2, 0), pbody->body->GetPosition(), true);

		if (mainState = FALL) {
			pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpforce), pbody->body->GetPosition(), true);
		}

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
		mainState = MOVE;
		grounded = true;
		if (vel.x < 0)
			state = MOVE_LEFT;

		if (vel.x > 0)
			state = MOVE_RIGHT;

		if (vel.x == 0)
			state = IDLE;
	}
	if (vel.y > 0) {
		mainState = FALL;
		grounded = false;
		if (vel.x > 0)
			state = FALL_LEFT;

		if (vel.x < 0)
			state = FALL_RIGHT;
	}

	if (vel.y < 0) {
		mainState = JUMP;
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
		currentAnimation = &jumpStart;
		if (frameCounter < 30) {
			SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
			deadTextureOn = true;
			frameCounter++;
		}
		else {

			SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
			SDL_Rect rect = { 0, 0, 1024, 480 };
			app->render->DrawTexture(texDeath, app->render->camera.x * -1, 0, &rect);
			deadTextureOn = true;
			//frameCounter++;
			if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				health = 5;
				position.x = spawn.x;
				position.y = spawn.y;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
				velocitx.x = 0;
				playerDeath = false;
				deadTextureOn = false;
			}
		}
	}

}

void Player::HandleFinish(bool finish)
{
	if (finish) {
		SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
		SDL_Rect rect = { 0, 0, app->win->width, 480 };
		app->render->DrawTexture(texFinish, app->render->camera.x * (-1), 0, &rect);

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
	else {
		lastcamPos = app->render->camera.x;
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

void Player::Bump() {
	
}