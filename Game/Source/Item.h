#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item(pugi::xml_node paras);
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact);

	void ItemInitialisation(pugi::xml_node itemNode, int x, int y);

public:

	enum class ItemType {
		BULLET,
		DIAMOND,
		HEALTH,
		OTHER
	};

	ItemType type;

	bool isPicked = false;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//TODO 4: Add a physics to an item
	

	int width;
	int height;
	int radius;
};

#endif // __ITEM_H__