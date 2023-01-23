#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L04: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	// L05: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

enum ObjectTypes
{
	OBJECTTYPE_SOLID,
	OBJECTTYPE_WARP,
	OBJECTTYPE_DEATH,
	OBJECTTYPE_FINISH,
	OBJECTTYPE_ITEM,
	OBJECTTYPE_PLAYERSPAWN,
	OBJECTTYPE_CHECKPOINT,
	OBJECTTYPE_TELEPORT,
	OBJECTTYPE_ENEMYAIR,
	OBJECTTYPE_ENEMYFLOOR,
	OBJECTTYPE_ENTITY
};

// L06: DONE 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	// L06: DONE 7: Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

	List<Property*> list;
};

// L05: DONE 1: Create a struct for the map layer
struct MapLayer
{
	SString	name;
	int id; 
	int width;
	int height;
	uint* data;

	// L06: DONE: Store custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L05: DONE 6: Short function to get the gid value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct Object
{
	int id;
	SString stringType;
	ObjectTypes type;
	SString name;
	int x;
	int y;
	int width;
	int height;

};

struct ObjectGroups 
{
	List<Object*> object;
	int id;
	SString name;
	Properties properties;
};

// L04: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SString orientation;
	List<TileSet*> tilesets;
	MapTypes type;
	List<MapLayer*> maplayers;
	List<ObjectGroups*> objectgroups;
};



class Map : public Module
{
public:

	Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();


    // Load new map
    bool Load(const char* fileCName);

	// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y);

	bool LoadObjectGroups(pugi::xml_node mapNode);

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;


	SString mapFileName;
	const char* mapNamePath;
	pugi::xml_parse_result nameResult;
	pugi::xml_document mapFileXML;

	List<PhysBody*> bodys;

	bool LoadMap(pugi::xml_node mapFile);

private:

	

	// L04: DONE 4: Create and call a private function to load a tileset
	bool LoadTileSet(pugi::xml_node mapFile);

	// L05
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	// L06: DONE 2
	TileSet* GetTilesetFromTileId(int gid) const;

	// L06: DONE 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	

	bool LoadObjects(pugi::xml_node& node, ObjectGroups* group);

	

public: 

	// L04: DONE 1: Declare a variable data of the struct MapData
	MapData mapData;
	iPoint playerSpawn;
	iPoint checkpointPos;

private:

    //SString mapFileName;
	SString mapFolder;
    bool mapLoaded;
};

#endif // __MAP_H__