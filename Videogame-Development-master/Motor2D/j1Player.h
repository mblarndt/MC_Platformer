#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"

#include "j1PerfTimer.h"

#include "j1EntityMachine.h"


struct Collider;

enum PlayerState {
	idle,
	running,
	jumping,
	falling,
	crouch,
	dashLeft,
	dashRight,
	attack
};

struct Player {
	
	p2Point<float> acceleration;
	p2Point<float> speed;
	p2Point<float> maxSpeed;
	fPoint prevposition;
	p2Point<float> lastGroundedPos;

	p2SString animation;

	float gravity; 

	bool able_to_jump; //Only lets the player jump if it's true
	bool able_to_dash;
	bool dashing;
	bool jumping; //True when the player is jumping
	bool drop_plat; //Checks if the player is dropping from a platform.
	bool playerGrounded;
	bool flip;
	bool godMode;
	bool cealing;
	bool wall;
	bool onPlatform;
	bool movingRight;
	bool movingLeft;
	bool justLoaded;
	bool attacking;

	bool freeze = false;

	int boxW;
	int boxH;
	int boxOffset_x;

	SDL_Rect playerBox;
	PlayerState playerState;
	Collider*	collider;
	Collider*	attackCollider;
	SDL_Rect attackBox;

	bool attackCollider_active = false;

	inline void SetGroundState(bool state) 
	{
		playerGrounded = state;
	};
};

class j1Player : public Entity{
public:// methods

	j1Player(float x, float y, EntityType Type);

	// Destructor
	virtual ~j1Player();

	bool Init();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	//Actions and Checks functions
	void check_x_move(float dt);
	void MoveRight(float dt);
	void MoveLeft(float dt);
	void MoveUp(float dt);
	void MoveDown(float dt);
	void DashInput();
	void RunCheck();
	void JumpInput();
	void Jump();
	void Fall();
	void Attack();

	void GodMode();
	bool StartPlayer();
	bool Dash();
	void GroundedLogic();

	// Collision handling -----
	void j1Player::OnCollision(Collider* A, Collider* B);

	virtual bool Save(pugi::xml_node & node) const;
	virtual bool Load(pugi::xml_node & node);

private:

public://variables

	Player player;
	
private:

	bool jump_key_down = false;
	float gravity_tmp = 0;
	
	int dashTime  = 0;
	int jumpSound = 1;

	int jump1;
	int jump2;
	int jump3;
	int swordslash;

	j1PerfTimer* dashtimercheck;
	j1PerfTimer* jump_key_down_timer;

};


#endif
