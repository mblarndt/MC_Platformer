#ifndef __j1COLLISIONS_H__
#define __j1COLLISIONS_H__

//#include "j1Module.h"

#include "j1EntityMachine.h"



enum class ObjectType;
struct Object;
struct Properties;


struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	ObjectType type;
	j1Module* callback = nullptr;
	Properties* userdata;

	Entity* entity;

	Collider(SDL_Rect rectangle, ObjectType type, j1Module* callback = nullptr) :
		
		rect(rectangle),
		type(type),
		callback(callback)

	{}

	Collider(Object object);

	Collider() {};

	inline void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	inline bool CheckCollision(const SDL_Rect& r) const;
};



class j1Collisions : public j1Module {
	//Methods
public:
	j1Collisions();

	bool Init();
	// Destructor
	virtual ~j1Collisions() {};

	// Called before render is available
	bool Awake(pugi::xml_node& node);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, ObjectType type, j1Module* callback, Entity* entity = nullptr, Properties* userdata = nullptr);
	void LoadFromMap(bool LoadEntities = true);

private:
	
	void DebugDraw();

	//Variables
public:

	p2List<Collider*> colliders;
	bool debug_colliders;

private:

};


#endif
