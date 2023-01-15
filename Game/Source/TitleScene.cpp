#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "GuiSlider.h"
#include "Gui.h"


#include "Defs.h"
#include "Log.h"
#include "TitleScene.h"

TitleScene::TitleScene() : Module()
{
	name.Create("titleScene");
}

// Destructor
TitleScene::~TitleScene()
{}

// Called before render is available
bool TitleScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	
	logoRect.x = 0;
	logoRect.y = 0;
	logoRect.h = 480;
	logoRect.w = 1024;

	texturePath = "Assets/Textures/background1.png";
	settingsBoxPath = "Assets/Textures/SettingsBox.png";



	return ret;
}

// Called before the first frame
bool TitleScene::Start()
{
	app->fadeToBlack->activeScene = "TitleScene";
	app->scene->active = false;

	logo = app->tex->Load(texturePath);
	settingsBox = app->tex->Load(settingsBoxPath);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	toggle = true;

	return true;
}

// Called each loop iteration
bool TitleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool TitleScene::Update(float dt)
{
	if (toggle == true) {
		app->gui->MainMenuButtons();
		toggle = false;
	}
		
	app->render->camera.x = 0;
	app->render->camera.y = 0;
		
	//SDL_SetTextureAlphaMod(logo, accumulatedTime * 10.0f);
	app->render->DrawTexture(logo, 0, 0);

	if (app->gui->settings) {
		//app->render->DrawTexture(settingsBox, 100 - app->render->camera.x, 0);
		app->render->DrawTexture(settingsBox, 229 + app->render->camera.x, 30);
		app->render->DrawText("Sound", 250 - app->render->camera.x, 145, 0, 0, "white", false);
		app->render->DrawText("Music", 250 - app->render->camera.x, 200, 0, 0, "white", false);
		app->render->DrawText("FX", 250 + app->render->camera.x, 250, 0, 0, "white", false);
	}

	//L15: Draw GUI
	app->guiManager->Draw();



	return true;
}

// Called each loop iteration
bool TitleScene::PostUpdate()
{
	bool ret = true;

	//Exit Button
	if (exit)
		ret = false;

	return ret;
}

// Called before quitting
bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScene");
	app->tex->UnLoad(logo);

	return true;
}
