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
#include "FadeToBlack.h"
#include "TitleScene.h"

Player::Player(pugi::xml_node paras) : Entity(EntityType::PLAYER)
{	
	//parameters = paras;
	name.Create("Player");
	
}

Player::~Player() {

}

bool Player::Awake() {

	
	return true;
}

bool Player::Start() {

	InitPlayer();
	camTransition = false;
	//position = spawn = app->map->playerSpawn;

	return true;
}

bool Player::Update()
{
	level_now = Clock::now();

	level_time = duration_cast<seconds>(level_now - level_start).count();

	if (level_time >= maxtime)
	{
		playerDeath = true;
	}

	if (health == 0)
		playerDeath = true;
	if (lives == 0)
		gameOver = true;

	if (startGame == true) {
		if (app->scene->gamePaused == false) {
			if (levelFinish == false) {

				if (playerDeath == false && gameOver == false) {

					showGUI = true;
					/*----------------------------Follow Camera--------------------------*/
					PlayerCamera();
					/*----------------------------Get State of Player--------------------------*/
					StateMachine();
					/*----------------------------Player Movement--------------------------*/
					HandleMovement();

					/*----------------------------Rendering Player--------------------------*/
					RenderEntity();

					PlayerGUI(showGUI);

				}
			}

			//When Player collides with Lava he spawns at start again	
			HandleDeath(playerDeath);

			if (teleport == true && isTeleported == false) {
				position.x = 4*32;
				position.y = 0;
				//app->render->camera.x = 0;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
				velocitx.x = 1;
				
				isTeleported = true;
				teleport = false;
			}
			else if (teleport == true && isTeleported == true) {
				position.x = 72*32;
				position.y = 14*32;
				//app->render->camera.x = 0;
				pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
				velocitx.x = 0;
				pbody->body->ApplyLinearImpulse(b2Vec2(0, -5), pbody->body->GetPosition(), true);
				isTeleported = false;
				teleport = false;
			}

			Debug();

		}
	}
	return true;
}

bool Player::CleanUp()
{
	toDelete = true;
	//app->physics->world->DestroyBody(pbody->body);
	return true;
}

// L07 DONE 6: Define OnCollision function for the player. Check the virtual function on Entity class
void Player::OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact) {


	switch (physB->ctype)
	{
	case ColliderType::ITEM_BULLET:
		app->audio->PlayFx(pickCoinFxId);
		bullets = bullets+1;
		break;
	case ColliderType::ITEM_DIAMOND:
		app->audio->PlayFx(pickCoinFxId);
		diamondCollected = true;
		break;
	case ColliderType::ITEM_HEALTH:
		if (health != maxHealth)
			health = health + 1;
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
		app->audio->PlayFx(hitFxId);
		playerDeath = true;
		break;
	case ColliderType::FINISH:
		//LOG("Collision FINISH");
		levelFinish = true;
		break;
	case ColliderType::CHECKPOINT:
		LOG("Collision Checkpoint");
		//app->SaveGameRequest();
		app->scene->checkpointReached = true;
		break;
	case ColliderType::ENEMY:
		app->audio->PlayFx(hitFxId);
		health = health - 1;
		pbody->body->ApplyLinearImpulse(b2Vec2(2, 0), pbody->body->GetPosition(), true);

		if (mainState = FALL) {
			pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpforce), pbody->body->GetPosition(), true);
		}

		break;
	case ColliderType::TELEPORT:
		LOG("Collision Telleport");
		teleport = true;

		
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
		
		lives = lives - 1;
				
		health = 5;

		if (app->scene->checkpointReached) {
			position = app->map->checkpointPos;
		}
		else {
			position.x = spawn.x;
			position.y = spawn.y;
		}
		
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
		velocitx.x = 0;
		playerDeath = false;
		deadTextureOn = false;
	}

	else if (app->scene->checkpointReached) {

	}

}

void Player::HandleGameOver(bool over)
{
	if (over) {

		currentAnimation = &jumpStart;
		if (frameCounter < 30) {
			SDL_Rect rect1 = currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(texture, position.x - 15, position.y - 10, &rect1);
			frameCounter++;
		}
		else {
			deadTextureOn = true;
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
	if (position.x > 1024 + camOffset && position.x < 5120 - (1024-camOffset)) {
		app->render->camera.x = -(position.x - camOffset);
		app->render->camera.y = menu.y;
	}
	if(position.x < 1024+camOffset) {
		app->render->camera.x = -1024;
		app->render->camera.y = menu.y;
	}
	if (position.x > 5120 - (1024 - camOffset)) {
		app->render->camera.x = -(5120 - 1024);
	}

	if (position.x > 0 && position.x < 1024) {
		app->render->camera.x = 0;
	}
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

void Player::Reset() {
	position.x = 200;
	position.y = 300;
}

void Player::InitPlayer() {

	pugi::xml_document config;
	pugi::xml_parse_result parseResult = config.load_file("config.xml");
	if (parseResult) {
		parameters = config.child("config").child("scene").child("player");
	}

	//Get and initialize Player parameters from XML
	lives = parameters.child("stats").attribute("lives").as_int();
	health = maxHealth = parameters.child("stats").attribute("health").as_int();
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



	//initilize textures
	texture = app->tex->Load(texturePath);
	heartTex = app->tex->Load("Assets/Textures/heart.png");
	slimeball = app->tex->Load("Assets/Textures/slimeball.png");
	bulletsBG = app->tex->Load("Assets/Textures/bullets.png");

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

	spawn = position = app->map->playerSpawn;
	LOG("Player Spawn Position X: %i", spawn.x);
	LOG("Player Spawn Position Y: %i", spawn.y);

	// L07 TODO 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x + (width / 2), position.y + (height / 2), width, height, bodyType::DYNAMIC);

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	//Activate Collision Detection
	pbody->listener = this;

	pbody->body->SetFixedRotation(true);


	

	currentAnimation = &idle;

	CamTransition(0, spawn.x);
	app->audio->PlayMusic(backmusicPath);
	startGame = true;

	toDelete = false;

	teleport = false;

	diamondCollected = false;
}

void Player::PlayerGUI(bool show) {
	if (show) {
		HealthBar();

		switch (lives) {
		case 3:
			app->render->DrawTexture(heartTex, camPos.x * (-1) + 20, 20);
			app->render->DrawTexture(heartTex, camPos.x * (-1) + 55, 20);
			app->render->DrawTexture(heartTex, camPos.x * (-1) + 90, 20);
			break;
		case 2:
			app->render->DrawTexture(heartTex, camPos.x * (-1) + 20, 20);
			app->render->DrawTexture(heartTex, camPos.x * (-1) + 55, 20);
			break;
		case 1:
			app->render->DrawTexture(heartTex, camPos.x * (-1) + 20, 20);
			break;
		}

		app->render->DrawTexture(bulletsBG, 20 - camPos.x, 60);
		app->render->DrawTexture(slimeball, 88 - camPos.x, 72);
		
		std::string str = std::to_string(bullets) + " x";
		const char* c_str = str.c_str();
		app->render->DrawText(c_str, 35, 58, 0, 0, "white", false);

		app->render->DrawTexture(bulletsBG, 900 - camPos.x, 30);
		str = std::to_string(60 - level_time);
		c_str = str.c_str();
		app->render->DrawText(c_str, 950, 45, 0, 0, "white", true);
	}
}

void Player::HealthBar() {
	healthPerc = ((100 / maxHealth) * health);
	camPos = iPoint(app->render->camera.x, app->render->camera.y);
	SDL_Rect healthbar = { (camPos.x * (-1) + 412), 25, healthPerc * 2, 20 };
	int thickness = 5;
	SDL_Rect stroke = { healthbar.x - thickness, healthbar.y - thickness, 200 + thickness * 2, healthbar.h + 2 * thickness };
	app->render->DrawRectangle(stroke, 0, 0, 0, 255, true, true);
	app->render->DrawRectangle(healthbar, 255, 0, 0, 255, true, true);
}