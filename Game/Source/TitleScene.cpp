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

	return ret;
}

// Called before the first frame
bool TitleScene::Start()
{
	app->fadeToBlack->activeScene = "TitleScene";

	logo = app->tex->Load(texturePath);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	uint w, h;
	app->win->GetWindowSize(w, h);
	buttons[0] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "StartButton", { 100, (int)w / 10,     190, 66 }, this);
	buttons[1] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "ExitButton", { 100, (int)w / 10 * 3, 190, 66 }, this);
	buttons[2] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "OptionsButton", { 100, (int)w / 10 * 2, 190, 66 }, this);

	click1FxId = app->audio->LoadFx("Assets/Audio/Fx/click1.ogg");
	click2FxId = app->audio->LoadFx("Assets/Audio/Fx/click2.ogg");
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
	app->render->camera.x = 0;
	app->render->camera.y = 0;
		
	//SDL_SetTextureAlphaMod(logo, accumulatedTime * 10.0f);
	app->render->DrawTexture(logo, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlackScene("Scene", 0.5);
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->DoFadeToBlack(2);
	}

	if (buttons[0]->state == GuiControlState::PRESSED) {
		app->fadeToBlack->FadeToBlackScene("Scene", 0.5);
	}


	for (int i = 0; i < numButtons; i++) {
		GuiControl* button = buttons[i];
		GuiControlState* preState = preStates[i];

		if (*preState != button->state) {
			if (button->state == GuiControlState::FOCUSED) {
				app->audio->PlayFx(click1FxId);
			}
			if (buttons[0]->state == GuiControlState::PRESSED) {
				app->audio->PlayFx(click2FxId);
			}
			*preState = button->state;
		}
	}

	

	//L15: Draw GUI
	app->guiManager->Draw();

	return true;
}

// Called each loop iteration
bool TitleScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if (buttons[2]->state == GuiControlState::PRESSED)
		ret = false;


	return ret;
}

// Called before quitting
bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScene");

	return true;
}


