#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool SaveState(pugi::xml_node& data);

	bool LoadState(pugi::xml_node& data);

	void CreateItem(pugi::xml_node itemNode);

	void CreateBullet(pugi::xml_node itemNode, int x, int y, int direction);

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* playerptr;
	Item* itemptr;

private:
	SDL_Texture* img;

};

#endif // __SCENE_H__