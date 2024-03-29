
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"
#include "Item.h"
#include "Optick/include/optick.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<MapLayer*>* item;
    item = mapData.maplayers.start;

    for (item = mapData.maplayers.start; item != NULL; item = item->next)
    {
        MapLayer* layer = item->data;

        if (layer->properties.GetProperty("Navigation") != NULL && !layer->properties.GetProperty("Navigation")->value)
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);
                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    //According to the mapType use the ID of the tile to set the walkability value
                    if (mapData.type == MapTypes::MAPTYPE_ISOMETRIC && tileId == 25) map[i] = 1;
                    else if (mapData.type == MapTypes::MAPTYPE_ORTHOGONAL && tileId == 122) map[i] = 1;
                    else map[i] = 0;
                }
                else {
                    LOG("CreateWalkabilityMap: Invalid tileset found");
                    map[i] = 0;
                }
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}

void Map::Draw()
{
    OPTICK_EVENT();

    if(mapLoaded == false)
        return;



    // L05: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        //L06: DONE 7: use GetProperty method to ask each layer if your “Draw” property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // L05: DONE 9: Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }
}

// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    // L08: DONE 1: Add isometric map to world coordinates
    if (mapData.type == MAPTYPE_ORTHOGONAL)
    {
        ret.x = x * mapData.tileWidth;
        ret.y = y * mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPE_ISOMETRIC)
    {
        ret.x = (x - y) * (mapData.tileWidth / 2);
        ret.y = (x + y) * (mapData.tileHeight / 2);
    }

    return ret;
}

// L08: TODO 3: Add method WorldToMap to obtain  map coordinates from screen coordinates
iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    if (mapData.type == MAPTYPE_ORTHOGONAL)
    {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPE_ISOMETRIC)
    {
        float halfWidth = mapData.tileWidth * 0.5f;
        float halfHeight = mapData.tileHeight * 0.5f;
        ret.x = int((x / halfWidth + y / halfHeight) / 2);
        ret.y = int((y / halfHeight - x / halfWidth) / 2);
    }
    else
    {
        LOG("Unknown map type");
        ret.x = x; ret.y = y;
    }

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // L05: DONE 7: Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

// L06: DONE 2: Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");


    // Remove all tilesets
    ListItem<TileSet*>* item;
    item = mapData.tilesets.start;

    while (item != NULL)
    {
        SDL_DestroyTexture(item->data->texture);

        RELEASE(item->data);
        item = item->next;
    }
    mapData.tilesets.Clear();

    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }
    mapData.maplayers.Clear();

    //Object cleanup
    ListItem<ObjectGroups*>* groupItem;
    groupItem = mapData.objectgroups.start;
    
    while (groupItem != NULL)
    {
        RELEASE(groupItem->data);
        groupItem = groupItem->next;
    }
    mapData.objectgroups.Clear();

    // Clean up the pugui tree
    mapFileXML.reset();

    return true;
}

// Load new map
bool Map::Load(const char* fileCName)
{
    bool ret = true;

    pugi::xml_parse_result result = mapFileXML.load_file(fileCName);
    //pugi::xml_parse_result nameResult = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapNamePath, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // L05: DONE 4: Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
        LOG("AllLayers loaded");
    }


    if(ret == true)
    {
        // L04: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
       
        LOG("Successfully parsed map XML file :%s", SString(fileCName).GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // L05: DONE 4: LOG the info for each loaded layer
        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        LOG("MapLayer----");

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    LoadObjectGroups(mapFileXML.child("map"));

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// L04: DONE 3: Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.orientation = map.attribute("orientation").as_string();


        // L08: TODO 2: Read the orientation of the map
        if (mapData.orientation == "orthogonal") {
            mapData.type = MAPTYPE_ORTHOGONAL;
        }
        else if (mapData.orientation == "isometric") {
            mapData.type = MAPTYPE_ISOMETRIC;
        }
        else {
            mapData.type = MAPTYPE_UNKNOWN;
        }
    }

    return ret;
}

// L04: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // L04: DONE 4: Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

// L05: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    //L06: DONE 6 Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// L05: DONE 4: Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadObjectGroups(pugi::xml_node mapNode)
{
    bool ret = true;

    for (pugi::xml_node objectNode = mapNode.child("objectgroup"); objectNode && ret; objectNode = objectNode.next_sibling("objectgroup"))
    {
        ObjectGroups* objectgroup = new ObjectGroups();
        ret = LoadObjects(objectNode, objectgroup);

        mapData.objectgroups.Add(objectgroup);
    }
    return ret;
}

bool Map::LoadObjects(pugi::xml_node& node, ObjectGroups* group)
{
    bool ret = true;

    //Load the attributes
    group->id = node.attribute("id").as_int();
    group->name = node.attribute("name").as_string();



    //Iterate over all the obects and assign the values
    int cnr = 1;
    SString cstr;

    pugi::xml_node object;
    for (object = node.child("object"); object && ret; object = object.next_sibling("object"))
    {
        Object* newObject = new Object();
        
        newObject->id = object.attribute("id").as_int();
        
        newObject->stringType = object.attribute("class").as_string();
        if (newObject->stringType == "solid") {
            newObject->type = ObjectTypes::OBJECTTYPE_SOLID;
        }
        else if (newObject->stringType == "warp") {
            newObject->type = ObjectTypes::OBJECTTYPE_WARP;
        }
        else if (newObject->stringType == "death") {
            newObject->type = ObjectTypes::OBJECTTYPE_DEATH;
        }
        else if (newObject->stringType == "finish") {
            newObject->type = ObjectTypes::OBJECTTYPE_FINISH;
        }
        else if (newObject->stringType == "item") {
            newObject->type = ObjectTypes::OBJECTTYPE_ITEM;
        }
        else if (newObject->stringType == "item_diamond") {
            newObject->type = ObjectTypes::OBJECTTYPE_ITEM_DIAMOND;
        }
        else if (newObject->stringType == "playerSpawn") {
            newObject->type = ObjectTypes::OBJECTTYPE_PLAYERSPAWN;
        }
        else if (newObject->stringType == "enemyair") {
            newObject->type = ObjectTypes::OBJECTTYPE_ENEMYAIR;
        }
        else if (newObject->stringType == "enemyfloor") {
            newObject->type = ObjectTypes::OBJECTTYPE_ENEMYFLOOR;
        }
        else if (newObject->stringType == "checkpoint") {
            newObject->type = ObjectTypes::OBJECTTYPE_CHECKPOINT;
        }
        else if (newObject->stringType == "teleport") {
            newObject->type = ObjectTypes::OBJECTTYPE_TELEPORT;
        }
        else
            newObject->type = ObjectTypes::OBJECTTYPE_ENTITY;

        newObject->height = object.attribute("height").as_int();
        newObject->width = object.attribute("width").as_int();   
        newObject->x = object.attribute("x").as_int();
        newObject->y = object.attribute("y").as_int();     
        newObject->name = object.attribute("name").as_string();

        cstr = "c" + cnr;

        if (newObject->type == ObjectTypes::OBJECTTYPE_SOLID) {
            PhysBody* floor = app->physics->CreateRectangle(newObject->x + (newObject->width) / 2, newObject->y + (newObject->height) / 2, newObject->width, newObject->height, STATIC);
            floor->ctype = ColliderType::FLOOR;
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_DEATH) {
            PhysBody* cstr = app->physics->CreateRectangle(newObject->x + (newObject->width) / 2, newObject->y + (newObject->height) / 2, newObject->width, newObject->height, STATIC);
            cstr->ctype = ColliderType::DEATH;
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_FINISH) {
            PhysBody* cstr = app->physics->CreateRectangle(newObject->x + (newObject->width) / 2, newObject->y + (newObject->height) / 2, newObject->width, newObject->height, STATIC);
            cstr->ctype = ColliderType::FINISH;
            LOG("Finish loaded");
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_CHECKPOINT) {
            PhysBody* cstr = app->physics->CreateRectangleSensor(newObject->x + (newObject->width) / 2, newObject->y + (newObject->height) / 2, newObject->width, newObject->height, STATIC);
            cstr->ctype = ColliderType::CHECKPOINT;
            checkpointPos.x = newObject->x;
            checkpointPos.y = newObject->y;
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_TELEPORT) {
            PhysBody* cstr = app->physics->CreateRectangle(newObject->x + (newObject->width) / 2, newObject->y + (newObject->height) / 2, newObject->width, newObject->height, STATIC);
            cstr->ctype = ColliderType::TELEPORT;
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_PLAYERSPAWN) {
            playerSpawn.x = object.attribute("x").as_int();
            playerSpawn.y = object.attribute("y").as_int();
            //app->scene->SpawnPlayer();
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_ITEM) {
            app->scene->CreateItem(object, Item::ItemType::BULLET);
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_ITEM_DIAMOND) {
            app->scene->CreateItem(object, Item::ItemType::DIAMOND);
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_ENEMYAIR) {
            app->scene->InitEnemyAirSpawn(object);
        }
        else if (newObject->type == ObjectTypes::OBJECTTYPE_ENEMYFLOOR) {
            app->scene->InitEnemyFloorSpawn(object);
        }

        switch (newObject->type) {
        case ObjectTypes::OBJECTTYPE_SOLID:
            PhysBody* floor = app->physics->CreateRectangle(newObject->x + (newObject->width) / 2, newObject->y + (newObject->height) / 2, newObject->width, newObject->height, STATIC);
            floor->ctype = ColliderType::FLOOR;
            bodys.Add(floor);
        }
       


        group->object.Add(newObject);
        

        cnr++;
    }
    
    return ret;
    LOG("DEBUG");
    LOG("Finished loading Objects");
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}


// L06: DONE 7: Ask for the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}
