
//#include "j1EntityMachine.h"


//Flying_Enemy Entity
class Flying_Enemy : public Entity {
	//variables
public:
	fPoint goal;
	

	//Methods
public:
	Flying_Enemy(float x, float y, EntityType Type);

	bool Awake(pugi::xml_node &);

	bool Start();

	bool Update(float dt);

	void pathfind();

	bool Save(pugi::xml_node & node) const;
	bool Load(pugi::xml_node & node);
};