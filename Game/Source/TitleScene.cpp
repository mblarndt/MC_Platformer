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
	app->scene->active = false;

	logo = app->tex->Load(texturePath);

	app->render->camera.x = 0;
	app->render->camera.y = 0;


	//Button Setup
	uint w, h;
	app->win->GetWindowSize(w, h);
	buttons[0] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "StartButton", { 100, (int)w / 10,     190, 66 }, this);
	buttons[1] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "ExitButton", { 100, (int)w / 10 * 3, 190, 66 }, this);
	buttons[2] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "OptionsButton", { 100, (int)w / 10 * 2, 190, 66 }, this);
	buttons[3] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Level 1", { 100+200, (int)w / 10,     190, 66 }, this);
	buttons[4] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Level 2", { 100+200, (int)w / 10 * 2, 190, 66 }, this);
	buttons[5] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Back", { 100+200, (int)w / 10 * 3, 190, 66 }, this);

	//Load Button Click Sounds
	click1FxId = app->audio->LoadFx("Assets/Audio/Fx/click1.ogg");
	click2FxId = app->audio->LoadFx("Assets/Audio/Fx/click2.ogg");

	MainMenuButtons();
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

	//Button Click Sound
	for (int i = 0; i < numButtons; i++) {
		GuiControl* button = buttons[i];
		GuiControlState* preState = preStates[i];

		if (*preState != button->state) {
			if (button->state == GuiControlState::FOCUSED) {
				app->audio->PlayFx(click1FxId);
			}
			if (button->state == GuiControlState::PRESSED) {
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

	//Exit Button
	if (buttons[1]->state == GuiControlState::PRESSED)
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

bool TitleScene::MainMenuButtons() {
		buttons[3]->state = GuiControlState::DISABLED;
		buttons[4]->state = GuiControlState::DISABLED;
		buttons[5]->state = GuiControlState::DISABLED;
		buttons[0]->state = GuiControlState::NORMAL;
		buttons[1]->state = GuiControlState::NORMAL;
		buttons[2]->state = GuiControlState::NORMAL;
	return true;
}

bool TitleScene::SettingsButtons()
{
	return false;
}

bool TitleScene::StartButtons()
{
		buttons[0]->state = GuiControlState::DISABLED;
		buttons[1]->state = GuiControlState::DISABLED;
		buttons[2]->state = GuiControlState::DISABLED;
		buttons[3]->state = GuiControlState::NORMAL;
		buttons[4]->state = GuiControlState::NORMAL;
		buttons[5]->state = GuiControlState::NORMAL;
	
	return true;
}

bool TitleScene::NoButtons() {
	//for (int i = 0; i < numButtons; i++) {
	//	buttons[i]->state = GuiControlState::DISABLED;
	//}

	buttons[0]->state = GuiControlState::DISABLED;
	buttons[1]->state = GuiControlState::DISABLED;
	buttons[2]->state = GuiControlState::DISABLED;
	buttons[3]->state = GuiControlState::DISABLED;
	buttons[4]->state = GuiControlState::DISABLED;
	buttons[5]->state = GuiControlState::DISABLED;

	return true;
}

bool TitleScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		StartButtons();
		break;

	case 4:
		LOG("Button 4: Start Level 1");
		NoButtons();
		app->scene->SceneStart(1);
		app->fadeToBlack->FadeToBlackScene("Scene", 0.5);
		app->scene->playerptr->camTransition = true;
		break;
	
	case 5:
		LOG("Button 5: Start Level 2");
		NoButtons();
		app->scene->SceneStart(2);
		app->fadeToBlack->FadeToBlackScene("Scene", 0.5);
		app->scene->playerptr->camTransition = true;
		break;

	case 6:
		LOG("Button 6: Back to Main Menu");
		MainMenuButtons();
	}
	return true;
}
