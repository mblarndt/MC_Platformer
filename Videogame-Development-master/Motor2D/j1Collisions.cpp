#include "j1App.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Input.h"

#include "BroFiler/Brofiler.h"

j1Collisions::j1Collisions() : j1Module(), debug_colliders(false)
{
	name.create("collisions");

}

bool j1Collisions::Init() {

	return true;
};

bool j1Collisions::Awake(pugi::xml_node& config) {


	return true;
};

bool j1Collisions::Start() {

	LoadFromMap();

	return true;
};

bool j1Collisions::PreUpdate() {

	p2List_item<Collider*>* Coll_iterator = colliders.start;
	while (Coll_iterator != nullptr) {

		if (Coll_iterator->data->to_delete == true)
		{
			colliders.del(Coll_iterator);
		}

		Coll_iterator = Coll_iterator->next;
	}

	// Calculate collisions
	Collider* c1;
	Collider* c2;

	Coll_iterator = colliders.start;
	while (Coll_iterator != nullptr) {

		c1 = Coll_iterator->data;
		
		p2List_item<Collider*>* Coll_iterator2 = colliders.start;

		while (Coll_iterator2 != nullptr) {

			c2 = Coll_iterator2->data;

			if (c1 != c2) {
				if (c1->CheckCollision(c2->rect) == true )
				{
					if (c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			}
			Coll_iterator2 = Coll_iterator2->next;
		}
		Coll_iterator = Coll_iterator->next;
	}
	return true;
};

bool j1Collisions::Update(float dt) {

	DebugDraw();
	return true;
};

void j1Collisions::DebugDraw() {
	
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		if (debug_colliders)
		{
			debug_colliders = false;
		}
		else 
		{
			debug_colliders = true;
		}
	}

	if (debug_colliders == false)
		return;

	Uint8 alpha = 80; 	//Alpha value for all debug colliders
	p2List_item<Collider*>* Coll_iterator = colliders.start;
	while (Coll_iterator != nullptr) {

		switch (Coll_iterator->data->type)
		{
		case ObjectType::PLATFORM: // white
			App->render->DrawQuad(Coll_iterator->data->rect, 255, 255, 255, alpha);
			break;
		case ObjectType::SOLID: // red
			App->render->DrawQuad(Coll_iterator->data->rect, 255, 0, 0, alpha);
			break;
		case ObjectType::DAMAGE: // blue
			App->render->DrawQuad(Coll_iterator->data->rect, 0, 0, 255, alpha);
			break;
		case ObjectType::PLAYER: // green
			App->render->DrawQuad(Coll_iterator->data->rect, 0, 255, 0, alpha);
			break;
		case ObjectType::WARP: // pink
			App->render->DrawQuad(Coll_iterator->data->rect, 255, 20, 147, alpha);
			break;
		case ObjectType::ATTACK:
			App->render->DrawQuad(Coll_iterator->data->rect, 255, 0, 0, alpha);

		case ObjectType::ENEMY:
			App->render->DrawQuad(Coll_iterator->data->rect, 255, 75, 0, alpha);
			break;
		case ObjectType::COIN:
			App->render->DrawQuad(Coll_iterator->data->rect, 128, 0, 128, alpha);
			break;
		}
		Coll_iterator = Coll_iterator->next;
	}
	
}

bool j1Collisions::PostUpdate() {

	return true;
};

bool j1Collisions::CleanUp() {




	p2List_item<Collider *>* item;
	item = colliders.start;

	while (item != NULL)
	{

		RELEASE(item->data);

		item = item->next;
	}

	colliders.clear();
	return true;
};


Collider* j1Collisions::AddCollider(SDL_Rect rect, ObjectType type, j1Module* callback, Entity* entity, Properties* userdata)
{
	Collider* ret = new Collider;

	ret->callback = callback;
	ret->rect = rect;
	ret->type = type;
	ret->userdata = userdata;
	ret->entity = entity;

	colliders.add(ret);
	
	return ret;
}

//Add Colliders to the list from the map
void j1Collisions::LoadFromMap(bool LoadEntities) {
	
	p2List_item<MapObjectgroup*>* list_i = App->map->data.objectgroups.start;
	while (list_i != nullptr) {
		for (int i = 0; i < list_i->data->objects_size; i++) {
			
			if (list_i->data->objects[i].type == ObjectType::ENEMY ) {
				if (LoadEntities) {
				
					if (strcmp(list_i->data->objects[i].properties.list.start->data->data.v_string, "FLYING_ENEMY") == 0)
					{
						App->entities->CreateEntity(list_i->data->objects[i].box->x, list_i->data->objects[i].box->y, FLYING_ENEMY);
					}
					else if (strcmp(list_i->data->objects[i].properties.list.start->data->data.v_string, "GROUND_ENEMY") == 0)
					{
						App->entities->CreateEntity(list_i->data->objects[i].box->x, list_i->data->objects[i].box->y, GROUND_ENEMY);
					}
					else if (strcmp(list_i->data->objects[i].properties.list.start->data->data.v_string, "PLAYER") == 0)
					{
						App->entities->CreateEntity(list_i->data->objects[i].box->x, list_i->data->objects[i].box->y, PLAYER);
					}
				}
			}
			else if (list_i->data->objects[i].type == ObjectType::COIN) {
				if (LoadEntities) App->entities->CreateEntity(list_i->data->objects[i].box->x, list_i->data->objects[i].box->y, COIN);
			}
			else {
				AddCollider(*list_i->data->objects[i].box, list_i->data->objects[i].type, nullptr, nullptr, &list_i->data->objects[i].properties);
			}
		}
		list_i = list_i->next;
	}
	LOG("Load Collisions From Map complete");
}

//  Struct Collider Methods --------------------------------------
bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return ((r.x + r.w > rect.x ) && (r.x < rect.x + rect.w) &&
		(r.y + r.h > rect.y) && (r.y < rect.y + rect.h));
}

Collider::Collider(Object object) {
	rect = *object.box;
	type = object.type;
}