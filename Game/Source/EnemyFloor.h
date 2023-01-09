#ifndef __ENEMYFLOOR_H__
#define __ENEMYFLOOR_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"

#include "Animation.h"

#include "SDL/include/SDL.h"
#include "DynArray.h"

#define MAX_JUMP_SPEED 7.0f
#define MAX_VEL 4.0f
#define DAMP 0.9f

struct SDL_Texture;

class EnemyFloor : public Entity
{
public:

	EnemyFloor(pugi::xml_node paras);

	pugi::xml_node paramaters;

	virtual ~EnemyFloor();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	Animation idle;
	Animation leftanim;
	Animation rightanim;

	// Current animation check
	Animation* currentAnimation = nullptr;

	void InitSpawn(pugi::xml_node itemNode);

	enum BehaviourState {
		IDLE,
		CHASE,
		MOVE_RIGHT,
		MOVE_LEFT,
		JUMP,
		JUMP_LEFT,
		JUMP_RIGHT,
		FALL,
		FALL_LEFT,
		FALL_RIGHT,
		GROUNDED,
		DEAD
	};


public:

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact);

	void Debug();

	void Move();

	void FindPath();

	void RenderEntity();

	void UpdateAnim();

	void HealthBar();

	//Player Physics Body
	PhysBody* pbody;

	b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);

	int health;
	int maxHealth;
	float healthPerc;

private:

	//FX-Sound Variables
	int hitFxId;
	const char* hitFxPath;

	//Player, Camera and Game States
	bool enemyDeath;

	//Position Variables
	iPoint spawn;
	iPoint menu;

	//Texture Variables
	SDL_Texture* texture;
	const char* texturePath;
	int width;
	int height;
	SDL_Texture* mouseTileTex = nullptr;


	int speed = 3;
	bool right = true;
	BehaviourState behaviourState = IDLE;
	SString moveClass;
	bool isJumping = false;
	const DynArray<iPoint>* path;

	int direction;




};

#endif // __ENEMYFLOOR_H__
