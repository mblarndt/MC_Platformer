#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"

#include "Animation.h"

#include "SDL/include/SDL.h"

#define MAX_JUMP_SPEED 7.0f
#define MAX_VEL 4.0f
#define DAMP 0.9f

struct SDL_Texture;

class Player : public Entity
{
public:

	Player(pugi::xml_node parameters);
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();
	
	Animation idle;
	Animation movementRight;
	Animation movementLeft;
	Animation jumpStart;
	Animation jumpEnd;
	Animation jumpUp;
	Animation jumpDown;

	// Current animation check
	Animation* currentAnimation = nullptr;

public:

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Debug();

	void GetState();

	void StateMachine();
	
	//Player Physics Body
	PhysBody* pbody;

	b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);

	struct playerState {
		bool idle;
		bool moveRight;
		bool moveLeft;
		bool isJumping;
		bool dead;
		bool isFalling;
		bool isLanding;
	};

	playerState state;

private:

	//Texture Variables
	SDL_Texture* texture;
	const char* texturePath;
	int width;
	int height;

	SDL_Texture* texDeath;
	const char* deathPath;
	int deathWidth;
	int deathHeight;
	
	SDL_Texture* texFinish;
	const char* finishPath;
	int finishWidth;
	int finishHeight;

	//FX-Sound Variables
	int hitFxId;
	int pickCoinFxId;
	const char* pickCoinFxPath;
	const char* hitFxPath;
	const char* backmusicPath;
	
	//Player, Camera and Game States
	bool playerDeath;
	bool levelFinish;
	bool startGame;
	bool camMoved;

	//Position Variables
	iPoint spawn;
	iPoint menu;
	int camOffset;
	int remainingPixels;
	int frameCounter;

	//Player Movement Variables
	float speed;
	float jumpforce;
	int jumpsteps;
	int jumpcount;
	int remainingJumpSteps;
	int jumpStart_counter;

	//States

	
	
};

#endif // __PLAYER_H__