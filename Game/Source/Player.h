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

	Player(pugi::xml_node paras);
	
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

	void OnCollision(PhysBody* physA, PhysBody* physB, b2Contact* contact);

	void Debug();

	void GetState();

	void StateMachine();

	void Shoot();

	void HandleMovement();

	void HandleDeath(bool dead);

	void HandleGameOver(bool over);

	void HandleFinish(bool finish);

	void RenderEntity();

	void PlayerCamera();

	void InitSpawn(pugi::xml_node itemNode);

	bool CamTransition(int start, int stop);

	void Bump();

	void Reset();

	void InitPlayer();

	void HealthBar();

	void PlayerGUI(bool show);
	
	//Player Physics Body
	PhysBody* pbody;
	PhysBody* projectileBody;

	b2Vec2 velocitx = b2Vec2(0, -GRAVITY_Y);


	enum PlayerState {
		MOVE,
		IDLE,
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


	PlayerState state;
	PlayerState preState;
	PlayerState mainState;

	bool grounded;
	//playerState state;
	int bullets;
	int health;
	int maxHealth;
	float healthPerc;
	int lives;

	int lastcamPos;

	bool deadTextureOn;

	iPoint spawn;

	bool startGame;

	bool camTransition;

	iPoint camPos;

	//Player, Camera and Game States
	bool playerDeath;
	bool levelFinish;

	bool showGUI;

private:

	//Texture Variables
	SDL_Texture* texture;
	const char* texturePath;
	int width;
	int height;

	SDL_Texture* heartTex;

	

	//FX-Sound Variables
	int hitFxId;
	int pickCoinFxId;
	const char* pickCoinFxPath;
	const char* hitFxPath;
	const char* backmusicPath;
	
	
	
	bool gameOver;

	//Position Variables
	
	iPoint menu;
	int camOffset;
	int remainingPixels;
	int frameCounter;

	//Player Movement Variables
	float speed;
	float jumpforce;
	int jumpcount;

	//States
	int shootDir;

	//Player 
	

	//Sensor Fixtures
	b2PolygonShape shapeR;
	b2PolygonShape shapeL;
	b2PolygonShape shapeT;
	b2PolygonShape shapeB;

	b2FixtureDef fixtureDefR;
	b2FixtureDef fixtureDefL;
	b2FixtureDef fixtureDefT;
	b2FixtureDef fixtureDefB;

	//Animations
	int spriteWidth;
	int spriteHeight;
	int row;
	int column;
};

#endif // __PLAYER_H__