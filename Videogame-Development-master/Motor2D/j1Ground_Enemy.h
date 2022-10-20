
//#include "j1EntityMachine.h"

#include "j1Map.h"

//Flying_Enemy Entity
class Ground_Enemy : public Entity {
	//variables
public:
	fPoint goal;

	iPoint gravity;



	//bool grounded = false;
	//Methods
public:
	Ground_Enemy(float x, float y, EntityType Type);

	bool Awake(pugi::xml_node &);

	bool Start();

	bool Update(float dt);

	void pathfind();

	bool Save(pugi::xml_node & node) const;
	bool Load(pugi::xml_node & node);

	bool CleanUp();

};