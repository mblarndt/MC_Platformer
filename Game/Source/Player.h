#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"

#include "Animation.h"

#include "SDL/include/SDL.h"

#define MAX_JUMP_SPEED 7.0f
#define MAX_VEL 4.0f
#define DAMP 0.9f

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void Debug();
	
	Animation idle;
	Animation movement;
	Animation jumpStart;
	Animation jumpEnd;
	Animation jumpUp;
	Animation jumpDown;

	int jumpStart_counter = 4;
	int frameCounter;

	// Current animation check
	Animation* currentAnimation = nullptr;

public:

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void OnCollision(PhysBody* physA, PhysBody* physB);

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;

	//SoundIDs
	int pickCoinFxId;
	int hurtFxId;
	
	bool spawnStart;
	iPoint spawn;
	iPoint menu;

	bool startGame = false;
	bool camMoved = false;
	int camOffset;

	int remainingPixels = 0;

	bool isjumping;
	int jumpcount = 2;

	int width;
	int height;
	float speed;
	float jumpforce;
	int jumpsteps = 3;
	int remainingJumpSteps = jumpsteps;

};

#endif // __PLAYER_H__