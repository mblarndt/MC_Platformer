#include <math.h>

#include "j1App.h"
#include "j1Module.h"
#include "j1FadeToBlack.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1Map.h"

#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FadeToBlack::j1FadeToBlack()
{
	name.create("fade");

}

j1FadeToBlack::~j1FadeToBlack()
{}

bool j1FadeToBlack::Awake(pugi::xml_node& conf) {

	
	uint width, height;
	App->win->GetWindowSize(width, height);
	screen.w = width;
	screen.h = height;
	screen.x = screen.y = 0;

	return true;
}

// Load assets
bool j1FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
bool j1FadeToBlack::Update(float dt)
{
	
	if (current_step == fade_step::none) {
		//Freeze = false
		return true;
	}
	

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch(current_step)
	{
		case fade_step::fade_to_black:
		{
			if(now >= total_time)
			{
				//freeze = true
				SwitchMap(level);

				total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::fade_from_black;
			}
		} break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;

			if(now >= total_time)
				current_step = fade_step::none;
				
		} break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool j1FadeToBlack::FadeToBlack(int lvl, bool isSaveLoad, float time)
{
	bool ret = false;
	


	if(current_step == fade_step::none)
	{
		isLoad = isSaveLoad;
		level = lvl;

		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}


bool j1FadeToBlack::SwitchMap(int level) {
	bool ret = true;
	LOG("Switching Maps...");

	/*p2List_item<Entity*>* entityIter = App->entities->entity_list.start;

	while (entityIter != NULL)
	{
		App->entities->entity_list.del(entityIter);

		entityIter = entityIter->next;
	}
	
	if (App->entities->active) App->entities->entity_list.clear();
	if (App->collisions->active) App->collisions->colliders.clear(); //Clear colliders
	
	//Empty all map data.
	App->map->CleanUp();

	ret = App->map->Load(level);		//Load specified map
	App->collisions->LoadFromMap(false);		//Load Collisions
	
												//ret = App->player->StartPlayer();	//Reset Player
	LOG("Starting Load_Now");
	App->entities->Load_Now();
	LOG("Finished Load_Now");*/

	App->map->CleanUp();
	App->entities->CleanUp();
	App->collisions->CleanUp();

	App->map->Load(level);
	if (!isLoad) {
		App->collisions->LoadFromMap();
	}
	else if (isLoad) {
		App->collisions->LoadFromMap(false);

		//Delete all entities.
		p2List_item<Collider*>* Coll_iterator = App->collisions->colliders.start;
		while (Coll_iterator != nullptr) {

			if (Coll_iterator->data->type == ObjectType::ENEMY || Coll_iterator->data->type == ObjectType::PLAYER || Coll_iterator->data->type == ObjectType::COIN)
			{
				int tmp = App->entities->entity_list.find(Coll_iterator->data->entity);
				if (tmp != -1) {
					
					Entity* to_delete = App->entities->entity_list.At(tmp)->data;
					App->entities->DeleteEntity(to_delete);
				}	
			
			}
			Coll_iterator = Coll_iterator->next;
		}
	


		App->entities->Load_Now();
	}


	return ret;
}