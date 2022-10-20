#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collisions.h"

#include "p2Log.h"
#include "j1EntityMachine.h"

// ----------------------------------------------------
union value {
	const char* v_string;
	int			v_int;
	float		v_float;
};

struct Properties //Properties
{
	struct Property
	{
		p2SString name;
		value data;
		//Entity* entity;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	value Get(const char* name, value* default_value = nullptr) const;

	p2List<Property*>	list;
};

// Object for Colliders
enum class ObjectType {
	UNKNOWN = 0,
	PLATFORM,
	SOLID,
	DAMAGE,
	PLAYER,
	ENEMY,
	COIN,
	WARP,
	ATTACK,
	MAX_OBJECTS = 7
};

struct Animations
{
	p2SString name;
	uint id; //Tile which is animated
	uint nFrames; //Number of frames of the animation
	uint* frames;
	//p2SString prev_Anim_Name = "idle";
	float speed;
	//float frameCount = 0.0f;
};

struct Object 
{
	uint				id;
	p2SString			name;
	ObjectType			type;
	SDL_Rect*			box;
	SDL_Texture*		texture;
	Properties			properties;
};

struct MapObjectgroup 
{
	uint				id;
	p2SString			name;
	Object*				objects;
	uint				objects_size;
};

// ----------------------------------------------------
struct MapLayer 
{
	p2SString			name;
	uint				width;
	uint				height;
	uint*				data; //Tile Ids
	float				speed;
	bool				navigation;
	inline uint Get(int x, int y) const
	{
		return x + y * width;
	}
	inline uint GetPath(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	
	SDL_Rect Tilerect ;

	SDL_Rect TileRect(uint tile_id) 
	{

		SDL_Rect ret = Tilerect; 

		int x = ((tile_id - firstgid) % num_tiles_width);
		int y = ((tile_id - firstgid) / num_tiles_width);

		ret.x = x*tile_width  + margin + spacing*x;
		ret.y = y*tile_height + margin + spacing*y;
		ret.w = tile_width;
		ret.h = tile_height;

		return ret;
	}
	
	SDL_Rect PlayerTilerect ;

	SDL_Rect PlayerTileRect(uint tile_id) 
	{

		SDL_Rect ret = PlayerTilerect;

		int num_t_width = tex_width / tile_width;
		int num_t_height = tex_height / tile_height;

		int x = tile_id % num_t_width;
		int y = tile_id / num_t_width;

		ret.x = x * tile_width;
		ret.y = y * tile_height;
		ret.w = tile_width;
		ret.h = tile_height;

		return ret;
	}

	inline p2Point<uint> GetPos(uint x, uint y) {
		p2Point<uint> ret;
		ret.x = x * tile_width;
		ret.y = y * tile_height;

		return ret;
	}

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	p2List<Animations*> animations;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	
	const char*				name;
	fPoint					start_position;
	
	SDL_Color				background_color;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	p2List<MapLayer*>		layers;
	p2List<MapObjectgroup*> objectgroups;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	virtual ~j1Map();

	bool Awake(pugi::xml_node& conf);

	void Draw();

	void DrawAnimation(p2SString name,char* tileset,fPoint& position, AnimationInfo* ainfo, bool flip = false);

	bool CleanUp();

	bool Load(int level);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	iPoint MapToWorldCentered(int x, int y) const;
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;
	TileSet* GetTilesetFromTileId(int id) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetAnimation(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectgroup(pugi::xml_node& node, MapObjectgroup* objectgroup);

public:

	MapData data;

	bool debug;

private:

	int cmp;


	int i;
	int j;
	int camera_x_tile;
	int camera_y_tile;
	int camera_w_tile;
	int camera_h_tile;

	int ms_to_frame;

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__