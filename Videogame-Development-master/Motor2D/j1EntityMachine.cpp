#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "BroFiler/Brofiler.h"
#include "j1Audio.h"

#include "j1EntityMachine.h"

j1EntityMachine::j1EntityMachine() {
	name.create("entities");
};


bool j1EntityMachine::Start()
{
	damage_timer = new j1Timer();

	p2List_item<Entity*>* entityIter = entity_list.start;

	while (entityIter != NULL)
	{
		entityIter->data->Start();

		entityIter = entityIter->next;
	}

	return true;
};

bool j1EntityMachine::Init()
{
	return true;
};

bool  j1EntityMachine::Awake(pugi::xml_node& config)
{
	/*p2List_item<Entity*>* entityIter = entity_list.start;

	while (entityIter != NULL)
	{
		entityIter->data->Awake(config);

		entityIter = entityIter->next;
	}*/
	coin = App->audio->LoadFx("audio/fx/coin.wav");
	hurt = App->audio->LoadFx("audio/fx/hurt.wav");
	return true;
};

bool j1EntityMachine::PreUpdate()
{
	p2List_item<Entity*>* entityIter = entity_list.start;

	while (entityIter != NULL)
	{
		entityIter->data->PreUpdate();

		entityIter = entityIter->next;
	}

	return true;
}

bool j1EntityMachine::Update(float dt)
{
	BROFILER_CATEGORY("j1EntityMachine::Update", Profiler::Color::Red);

	p2List_item<Entity*>* entityIter = entity_list.start;

	while (entityIter != NULL)
	{
		entityIter->data->Update(dt);

		entityIter = entityIter->next;
	}

	return true;
};

bool j1EntityMachine::PostUpdate()
{
	p2List_item<Entity*>* entityIter = entity_list.start;

	while (entityIter != NULL)
	{
		entityIter->data->PostUpdate();

		entityIter = entityIter->next;
	}

	return true;
}

bool j1EntityMachine::CleanUp() {

	p2List_item<Entity*>* entityIter = entity_list.start;

	while (entityIter != NULL)
	{
		
		entityIter->data->CleanUp();
		
		//RELEASE(entityIter->data);
		
		
		entityIter = entityIter->next;
	}
	entity_list.clear();
	return true;
};

// Create an Entity and add to the list ----------------------------------------------------
Entity* j1EntityMachine::CreateEntity(float x, float y, EntityType Type) {

	static_assert(EntityType::UNKNOWN == 4, " Something broke :( ");

	Entity* ret = nullptr;

	switch (Type) {
	case PLAYER:

		player = new j1Player(x, y, Type); //Struct Player es igual

		ret = player;

		if (ret != nullptr)
		{
			entity_list.add(ret);
		}

		player->Awake();
		player->Start();

		break;

	case COIN:

		ret = new Coin(x, y, Type);

		if (ret != nullptr)
		{
			entity_list.add(ret);
		}
		break;

	case FLYING_ENEMY:

		ret = new Flying_Enemy(x, y, Type);

		if (ret != nullptr)
		{
			entity_list.add(ret);
		}

		break;

	case GROUND_ENEMY:

		ret = new Ground_Enemy(x, y, Type);

		if (ret != nullptr)
		{
			entity_list.add(ret);
		}	
		break;
	}
	return ret;
};


// Destroy an Entity and remove it from the list -----------------------------------------------------
void j1EntityMachine::DeleteEntity(Entity* entity) 
{
	if (entity->collider != nullptr) {
		entity->collider->to_delete = true;
	}
	entity_list.del(entity_list.At(entity_list.find(entity)));
	
}

bool j1EntityMachine::Save(pugi::xml_node& node) const 
{

	p2List_item<Entity*>* entityIter = entity_list.start;
	while (entityIter != NULL)
	{
		pugi::xml_node entity_node = node.append_child("entity");
		
		entity_node.append_child("position").append_attribute("x") = entityIter->data->position.x;
		entity_node.child("position").append_attribute("y") = entityIter->data->position.y;

		entity_node.append_child("prevposition").append_attribute("x") = entityIter->data->prevposition.x;
		entity_node.child("prevposition").append_attribute("y") = entityIter->data->prevposition.y;

		entity_node.append_child("speed").append_attribute("x") = entityIter->data->speed.x;
		entity_node.child("speed").append_attribute("y") = entityIter->data->speed.y;

		entityIter->data->Save(entity_node);

		entityIter = entityIter->next;
	}

	return true;
};

bool j1EntityMachine::Load(pugi::xml_node& node)
{	
	
	/*
	pugi::xml_node node_i = node.child("entity");
	Entity* loaded_entity;
	
	for (; node_i; node_i = node_i.next_sibling("entity")) {
		
		iPoint pos;
		pos.x = node_i.child("position").attribute("x").as_float();
		pos.y = node_i.child("position").attribute("y").as_float();
		
		if (strcmp(node_i.attribute("EntityType").as_string(), "FLYING_ENEMY") == 0) {
			loaded_entity = CreateEntity(pos.x, pos.y, FLYING_ENEMY);
		}
		else if (strcmp(node_i.attribute("EntityType").as_string(), "GROUND_ENEMY") == 0) {
			loaded_entity = CreateEntity(pos.x, pos.y, GROUND_ENEMY);
		}
		else if (strcmp(node_i.attribute("EntityType").as_string(), "PLAYER") == 0) {
			loaded_entity = CreateEntity((float)pos.x, (float)pos.y, PLAYER);
			//loaded_entity = nullptr;
			//pla
		}
		else if (strcmp(node_i.attribute("EntityType").as_string(), "COIN") == 0) {
			loaded_entity = CreateEntity((float)pos.x, (float)pos.y, COIN);
		}
		else continue;
		
		if (loaded_entity != nullptr) {
			loaded_entity->prevposition.x = node_i.child("position").attribute("x").as_float();
			loaded_entity->prevposition.x = node_i.child("position").attribute("y").as_float();

			entity_list.add(loaded_entity);
		}

	}
*/
	return true;
};


bool j1EntityMachine::Load_Now()
{

	
	entity_list.clear();

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file("save_game.xml");

	root = data.child("game_state");

	pugi::xml_node node_i = root.child("entities").child("entity");
	Entity* loaded_entity;

	for (; node_i; node_i = node_i.next_sibling("entity")) {

		iPoint pos;
		pos.x = node_i.child("position").attribute("x").as_float();
		pos.y = node_i.child("position").attribute("y").as_float();

		if (strcmp(node_i.attribute("EntityType").as_string(), "FLYING_ENEMY") == 0) {
			loaded_entity = CreateEntity(pos.x, pos.y, FLYING_ENEMY);
		}
		else if (strcmp(node_i.attribute("EntityType").as_string(), "GROUND_ENEMY") == 0) {
			loaded_entity = CreateEntity(pos.x, pos.y, GROUND_ENEMY);
		}
		else if (strcmp(node_i.attribute("EntityType").as_string(), "PLAYER") == 0) {
			loaded_entity = CreateEntity((float)pos.x, (float)pos.y, PLAYER);
		}
		else if (strcmp(node_i.attribute("EntityType").as_string(), "COIN") == 0) {
			loaded_entity = CreateEntity((float)pos.x, (float)pos.y, COIN);
		}
		else continue;
		if (loaded_entity != nullptr) {
			loaded_entity->prevposition.x = node_i.child("position").attribute("x").as_float();
			loaded_entity->prevposition.y = node_i.child("position").attribute("y").as_float();

			//entity_list.add(loaded_entity);
		}
	}

	return true;
};

void j1EntityMachine::OnCollision(Collider* A, Collider* B)
{
	PlayerCollisions(A, B);

	AttackCollisions(A, B);

	GroundenemyCollisions(A, B);
}

void j1EntityMachine::AttackCollisions(Collider* A, Collider* B)
{
	if (A->type == ObjectType::ATTACK && B->type == ObjectType::ENEMY) {

		if (B->entity->type == GROUND_ENEMY) {
			App->scene->score += 150;
		}
		else if (B->entity->type == FLYING_ENEMY) {
			App->scene->score += 100;
		}
		DeleteEntity(B->entity);

	}
}


void j1EntityMachine::GroundenemyCollisions(Collider* A, Collider* B)
{
	if (A->type != ObjectType::ENEMY && B->type != ObjectType::ENEMY)
	{
		return;
	}
	if (A->type == ObjectType::PLAYER && B->type == ObjectType::PLAYER)
	{
		return;
	}
	if (B->type == ObjectType::ENEMY) {
		Collider temp = *A;
		A = B;
		B = &temp;
	}
	if (B->type == ObjectType::ENEMY || B->type == ObjectType::PLAYER) {
		return;
	}

	if (A->type == ObjectType::ENEMY && B->type == ObjectType::SOLID) 
	{
		//from above
		if (A->entity->position.y + A->rect.h > B->rect.y
			&& A->entity->position.y < B->rect.y
			&& A->rect.x < B->rect.x + B->rect.w
			&& A->rect.x + A->rect.w > B->rect.x
			&& A->entity->prevposition.y + A->rect.h <= B->rect.y + 1) //MagicNumber
			
		{
			if (A->entity->speed.y > 0)
			{
				A->entity->speed.y = 0;
			}
			A->entity->position.y = B->rect.y - A->rect.h + 1;
			A->entity->collider->SetPos(A->entity->position.x , A->entity->position.y);
			A->entity->grounded=true;
		}

		//from below
		if (A->entity->prevposition.y > (B->rect.y + B->rect.h - 1))
		{
			A->entity->position.y = B->rect.y + B->rect.h;
			A->entity->collider->SetPos(A->entity->position.x , A->entity->position.y);
		}

		//from a side
		if (A->entity->position.y + (A->rect.h* 0.1f) < B->rect.y + B->rect.h
			&& A->entity->position.y + (A->rect.h* 0.9f) > B->rect.y)
		{
			//player->player.wall = true;
			//LOG("Touching WALL");

			if ((A->rect.x + A->rect.w) < (B->rect.x + B->rect.w / 4))
			{ //Player to the left 
				A->entity->position.x = B->rect.x - A->rect.w + 1;

			}
			else if (A->rect.x > (B->rect.x + B->rect.w * 3 / 4))
			{ //Player to the right
				A->entity->position.x = B->rect.x + B->rect.w - 1;
			}
			A->entity->collider->SetPos(A->entity->position.x , A->entity->position.y);
		}
	}
}



void j1EntityMachine::PlayerCollisions(Collider* A, Collider* B)
{
	if (player->player.godMode) return; //While in God Mode Collisions are disregarded

	if (B->type == ObjectType::PLAYER) {
		Collider temp = *A;
		A = B;
		B = &temp;
	}

	/*if (A->type != ObjectType::PLAYER ) {
		return;
	}*/

	/*if (A->type == ObjectType::ENEMY || B->type == ObjectType::ENEMY) {

		return;
	}*/

	if (A->type == ObjectType::PLAYER && B->type == ObjectType::ENEMY)
	{
		if (damage_timer->Read() >= 1000.f)
		{
			App->scene->hp--;
			damage_timer->Start();
			App->audio->PlayFx(hurt);
		}
		if (App->scene->hp <= 0)
		{
			App->audio->PlayFx(hurt);
			App->fade->FadeToBlack(1);
			App->scene->hp = 3;
		}
		return;
	}

	if (A->type == ObjectType::PLAYER && B->type == ObjectType::COIN)
	{
		Coin* tmp = (Coin*)B->entity;
		if (tmp->picked_up == false) {
			App->scene->coin_score += 1;
			tmp->picked_up = true;
			App->audio->PlayFx(coin);
		}
		App->scene->score += 25;

		DeleteEntity(B->entity);
		return;
	}

	// ------------ Player Colliding against solids ------------------
	if (A->type == ObjectType::PLAYER && B->type == ObjectType::SOLID) {

		//from above
		if (player->position.y + A->rect.h > B->rect.y
			&& player->position.y < B->rect.y
			&& A->rect.x < B->rect.x + B->rect.w
			&& A->rect.x + A->rect.w > B->rect.x
			&& player->player.prevposition.y + player->player.boxH <= B->rect.y + 1 //MagicNumber
			&& player->player.justLoaded == false)
		{
			if (player->player.speed.y > 0)
			{
				player->player.speed.y = 0;
			}
			player->position.y = B->rect.y - player->player.collider->rect.h + 1;
			player->player.collider->SetPos(player->position.x + player->player.boxOffset_x, player->position.y);
			player->player.SetGroundState(true);
		}

		//from below
		if (player->player.prevposition.y > (B->rect.y + B->rect.h - 1))
		{
			player->position.y = B->rect.y + B->rect.h;
			player->player.cealing = true;
			player->player.collider->SetPos(player->position.x + player->player.boxOffset_x, player->position.y);
		}


		//from a side
		if (player->position.y + (A->rect.h* 0.1f) < B->rect.y + B->rect.h
			&& player->position.y + (A->rect.h* 0.9f) > B->rect.y
			)
		{
			player->player.wall = true;
			LOG("Touching WALL");

			//Player to the left 
			if ((A->rect.x + A->rect.w) < (B->rect.x + B->rect.w / 4)
				&& player->prevposition.x + A->rect.w + 1 <= (B->rect.x + B->rect.w) )
			{ 
				player->position.x = B->rect.x - A->rect.w - player->player.boxOffset_x + 1.0f;

			}

			//Player to the right
			else if (A->rect.x > (B->rect.x + B->rect.w * 3.0f / 4.0f))
			{ 
				player->position.x = B->rect.x + B->rect.w - player->player.boxOffset_x - 1.0f;
			}

			player->player.collider->SetPos(player->position.x + player->player.boxOffset_x, player->position.y);
		}
	}
	// ------------ Player Colliding against a platform -----------------
	if (A->type == ObjectType::PLAYER && B->type == ObjectType::PLATFORM) {

		if (player->player.drop_plat == false) {

			if ((player->player.prevposition.y + player->player.collider->rect.h) < B->rect.y + (B->rect.h / 2.0f) && (player->player.prevposition.y + player->player.collider->rect.h) > B->rect.y && player->player.speed.y >= 0) {//this won't ever happen
				player->position.y = B->rect.y - player->player.collider->rect.h + 1.0f;
				player->player.able_to_jump = false;
				player->player.onPlatform = true;
			}
			else if ((player->position.y >= player->player.prevposition.y) && (player->player.prevposition.y + player->player.collider->rect.h) < B->rect.y && player->player.speed.y >= 0) {
				player->position.y = B->rect.y - player->player.collider->rect.h + 1.0f;
				player->player.SetGroundState(true);
				player->player.able_to_jump = false;
				player->player.onPlatform = true;
			}
		}

	}
	// ------------ Player Colliding against a warp zone -----------------
	if (A->type == ObjectType::PLAYER && B->type == ObjectType::WARP) {
		if (strcmp(B->userdata->Get("MapToLoad").v_string, "Dark_Map.tmx") == 0) {
			App->fade->FadeToBlack(1);
		}
		else if (strcmp(B->userdata->Get("MapToLoad").v_string, "Dark_Map2.tmx") == 0) {
			App->fade->FadeToBlack(2);
		}
		else LOG("Issues on warp Loading");
		
	}
}