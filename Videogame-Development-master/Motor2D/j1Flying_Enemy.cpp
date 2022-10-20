#include "j1App.h"

#include "j1Map.h"
#include "j1Pathfinding.h"
#include "SDL/include/SDL_rect.h"

#include "j1EntityMachine.h"

//Flying Enemy
Flying_Enemy::Flying_Enemy(float x, float y, EntityType Type) : Entity(x, y, Type)
{
	colliderRect = { (int)position.x, (int)position.y, 50, 50 }; //Magic
	collider = App->collisions->AddCollider(colliderRect, ObjectType::ENEMY, App->entities, (Entity*)this);

	speed = { 100,100 };
}

bool Flying_Enemy::Save(pugi::xml_node & node) const {
	node.append_attribute("EntityType") = "FLYING_ENEMY";
	

	return true;
};

bool Flying_Enemy::Load(pugi::xml_node & node) {

	return true;
};

bool Flying_Enemy::Awake(pugi::xml_node &)
{

	return true;
}

bool Flying_Enemy::Start()
{
	
	return true;
}

bool Flying_Enemy::Update(float dt)
{
	/*if (App->do_logic)
	{*/
		//Pathfinding -------------------------------------------
		
	//}
	/*
	if(Path->Pop(last_tile)) {
		iPoint closest_center = App->map->MapToWorldCentered(last_tile.x, last_tile.y);


		if (closest_center.DistanceTo(position) < 2) {
			iPoint penultima_tile;
			if (Path->Pop(penultima_tile)) {
				closest_center = App->map->MapToWorldCentered(penultima_tile.x, penultima_tile.y);
			}
		}
		
		position.x += 2 * ((position.x < closest_center.x) ? 1 : -1);
		position.y += 2 * ((position.y < closest_center.y) ? 1 : -1);
		
	}*/
	pathfind();


	if (collider != nullptr)
	{
		collider->SetPos(position.x+7, position.y+7);
	}

	//Draw enemy
	App->map->DrawAnimation("skull_still", "Skull1", position , &Ainfo, flip);

	return true;
}

void Flying_Enemy::pathfind()
{
	goal = App->entities->player->position;

	if (goal.DistanceTo(position) < 1000) { //Detection radius

		//Find the closest tile to current position
		App->pathfinding->CreatePath(App->map->WorldToMap(position.x, position.y), App->map->WorldToMap(goal.x, goal.y));

		const p2DynArray<iPoint>* Path = App->pathfinding->GetLastPath();
		//LOG("PATH COUNT: %d", Path->Count());

		const iPoint* tile;
		if (Path->Count() != 0) {
			if (Path->Count() > 2) {
				tile = Path->At(2);
			}
			else if (Path->Count() > 1) {
				tile = Path->At(1);
			}
			else
			{
				tile = Path->At(0);
			}

			iPoint closest_center = App->map->MapToWorldCentered(tile->x, tile->y);

			if (closest_center.x > position.x) {
				position.x += speed.x * App->dt;
				flip = false;
			}
			else if (closest_center.x < position.x) {
				position.x -= speed.x * App->dt;
				flip = true;
			}
			if (closest_center.y > position.y) {
				position.y += speed.y * App->dt;
			}
			else if (closest_center.y < position.y) {
				position.y -= speed.y * App->dt;
			}
		}
	}
	
}