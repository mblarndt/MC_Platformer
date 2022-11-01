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

enum class PlayerState {
	IDLE,
	JUMPING,
	FALLING,
	LANDING,
	MOVE_RIGHT,
	MOVE_LEFT
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();
	
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

	void Jump(int jumpSteps);

	bool isjumping;

	



private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	PlayerState playerState;

	int pickCoinFxId;
	int jumpcount = 0;

	int width;
	int height;
	float speed;
	float jumpforce;
	int jumpsteps;
	int remainingJumpSteps;

	
	


};

#endif // __PLAYER_H__