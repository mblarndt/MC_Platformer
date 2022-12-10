#ifndef __BULLET_H__
#define __BULLET_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Bullet : public Entity
{
public:

	Bullet(pugi::xml_node paras);
	virtual ~Bullet();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact);

	void BulletInitialisation(pugi::xml_node itemNode, int x, int y, int direction);

public:

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

#endif // __BULLET_H__