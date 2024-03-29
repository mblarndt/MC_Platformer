#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"


#include <chrono>
using namespace std;
using namespace chrono;
typedef high_resolution_clock Clock;


#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"


// Modules
class Window;
class Input;
class Render;
class FadeToBlack;
class Textures;
class Audio;
class Scene;
class LogoScene;
class TitleScene;
class EntityManager;
class Map;
class PathFinding;
//L07 DONE 2: Add Physics module
class Physics;
class Animation;
class GuiManager;
class Gui;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L03: DONE 1: Create methods to control that the real Load and Save happens at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest();
	bool CheckSaveExist();
	bool LoadFromFile();
	bool SaveToFile();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	LogoScene* logoScene;
	TitleScene* titleScene;
	EntityManager* entityManager;
	Map* map;
	//L07 DONE 2: Add Physics module
	Physics* physics;
	FadeToBlack* fadeToBlack;
	PathFinding* pathfinding;
	Animation* animation;
	GuiManager* guiManager;
	Gui* gui;

	bool saveExist;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

	// L03: DONE 1: Create control variables to control that the real Load and Save happens at the end of the frame
	bool saveGameRequested;
	bool loadGameRequested;

	steady_clock::time_point start_time;
	steady_clock::time_point end_time;
	long long frame_time;
	
	double fps;
	double total_fps;
	double avg_fps;
	double fps_count;
	
	std::string str;
	const char* aux;

	void WindowTitleFPS();

};

extern App* app;

#endif	// __APP_H__