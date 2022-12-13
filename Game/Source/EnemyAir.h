#ifndef __ENEMYAIR_H__
#define __ENEMYAIR_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"

#include "Animation.h"

#include "SDL/include/SDL.h"

#define MAX_JUMP_SPEED 7.0f
#define MAX_VEL 4.0f
#define DAMP 0.9f

struct SDL_Texture;


class Enemy : public Entity
{
public:

	EnemyAir();

	virtual ~EnemyAir();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	Animation idle;

	// Current animation check
	Animation* currentAnimation = nullptr;

public:

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Debug();

	//Player Physics Body
	PhysBody* pbody;

	b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);
private:

	//Texture Variables
	SDL_Texture* texture;
	const char* texturePath;
	int width;
	int height;

	SDL_Texture* texFinish;
	const char* finishPath;
	int finishWidth;
	int finishHeight;

	//FX-Sound Variables
	int hitFxId;
	const char* hitFxPath;

	//Player, Camera and Game States
	bool enemyDeath;
	bool levelFinish;
	bool startGame;
	bool camMoved;

	//Position Variables
	iPoint spawn;
	iPoint menu;

	// Enemy Movement Variables
	float speed;
};

#endif // __ENEMYAIR_H__

