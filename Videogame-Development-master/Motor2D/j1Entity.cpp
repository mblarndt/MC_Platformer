#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL.h"

#include "j1EntityMachine.h"
//#include "j1Entity.h"
//#include "j1Flying_Enemy.h"


//Entity Methods

Entity::Entity(float x, float y, EntityType Type) : position(x, y), type(Type)
{
	
}

bool Entity::Awake()
{
	return true;
}
bool Entity::Start()
{
	return true;
}

bool Entity::Update(float dt)
{
	return true;
}

bool Entity::PostUpdate()
{
	return true;
}

bool Entity::PreUpdate()
{
	return true;
}

bool Entity::CleanUp()
{
	return true;
}

bool Entity::Load(pugi::xml_node &)
{
	return true;
}

bool Entity::Save(pugi::xml_node &) const
{
	return true;
}
