#include "App.h"
#include "Gui.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "TitleScene.h"

#include "Defs.h"
#include "Log.h"


Gui::Gui() : Module()
{
	name.Create("gui");
}

// Destructor
Gui::~Gui()
{}

// Called before render is available
bool Gui::Awake(pugi::xml_node& config)
{
	
	return true;
}

bool Gui::Start()
{
	//Button Setup
	uint w, h;
	app->win->GetWindowSize(w, h);
	startBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START",      { 100, (int)w / 13,     190, 66 }, this);
	settingsBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS",{ 100, (int)w / 13 * 2, 190, 66 }, this);
	creditsBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "CREDITS",  { 100, (int)w / 13 * 3, 190, 66 }, this);
	exitBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "EXIT",        { 100, (int)w / 13 * 4, 190, 66 }, this);

	continueBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "CONTINUE",{ 100, (int)w / 13,      190, 66 }, this);
	lvl1Btn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "LEVEL 1", { 100, (int)w / 13 * 2,  190, 66 }, this);
	lvl2Btn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "LEVEL 2", { 100, (int)w / 13 * 3,  190, 66 }, this);
	backBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "BACK",    { 100, (int)w / 13 * 4,  190, 66 }, this);

	resumeBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "RESUME", { 100 + 200, (int)w / 13 * 2,      190, 66 }, this);
	saveBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "SAVE",    { 100 + 200, (int)w / 13 * 3,      190, 66 }, this);
	loadBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "LOAD",    { 100 + 200, (int)w / 13 * 4,  190, 66 }, this);
	titleBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "TITLE",  { 510, (int)w / 13 * 3,  190, 66 }, this);

	vsyncToggle = (GuiToggle*)app->guiManager->CreateGuiControl(GuiControlType::TOGGLE, 13, "VSYNC", { 350, 140,  78, 48 }, this);
	vsyncToggle->toggle = app->render->VSYNC;

	SDL_Rect sliderRect = { 350, 250, 300,38 };
	slider1 = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 14, "Slider 1", sliderRect, this);

	SDL_Rect sliderRect2 = { 350, 200, 300,38 };
	slider2 = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 15, "Slider 2", sliderRect2, this);

	audioBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "AUDIO", { 229, 30, 190, 66 }, this);
	videoBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "VIDEO", { 229 + 188, 30,  190, 66 }, this);
	gameBtn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "GAME", { 229+376, 30,  190, 66 }, this);

	settingsBox = app->tex->Load("Assets/Textures/settingsBox.png");

	NoButtons();

	audioBtn->selected = true;
	toggle = false;

	

	return true;
}

bool Gui::Update(float dt)
{


	ButtonInit();


	return true;
}

bool Gui::PostUpdate()
{
	bool ret = true;

	//Exit Button
	if (exit)
		ret = false;

	return ret;
}

// Called before quitting
bool Gui::CleanUp()
{
	
	

	return true;
}



bool Gui::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		StartButtons();
		break;
	case 2:
		LOG("Button 2 click");
		exit = true;
		break;
	case 3:
		LOG("Button 3 click");
		settings = true;
		SettingsButtons();
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
		settings = false;
		if(app->fadeToBlack->activeScene == "TitleScene")
			MainMenuButtons();
		if (app->fadeToBlack->activeScene == "Scene")
			InGameMenu();
		break;

	case 8:
		LOG("Button 7: Continue from saved state");
		NoButtons();
		app->scene->SceneStart(1);
		app->fadeToBlack->FadeToBlackScene("Scene", 0.5);
		app->LoadGameRequest();
		break;

	case 9:
		LOG("Button 9: Continue");
		NoButtons();
		app->scene->toggle = false;
		break;

	case 10:
		LOG("Button 10: Save Game");
		app->SaveGameRequest();
		break;

	case 11:
		LOG("Button 11: Load Game");
		app->LoadGameRequest();
		app->scene->playerptr->startGame = true;
		app->scene->toggle = false;
		NoButtons();
		break;

	case 12:
		LOG("Button 12: Back to title");
		NoButtons();
		app->fadeToBlack->FadeToBlackScene("TitleScene", 0.5);
		app->titleScene->toggle = true;
		app->scene->toggle = false;
		break;
	case 13:
		if (app->render->VSYNC == true)
			app->render->VSYNC = false;
		else
			app->render->VSYNC = true;
		break;
	case 16:
		audioBtn->selected = true;
		videoBtn->selected = false;
		gameBtn->selected = false;
		break;
	case 17:
		audioBtn->selected = false;
		videoBtn->selected = true;
		gameBtn->selected = false;
		break;
	case 18:
		audioBtn->selected = false;
		videoBtn->selected = false;
		gameBtn->selected = true;
		break;
}
	
	return true;
}

bool Gui::MainMenuButtons() {

	startBtn->state = GuiControlState::NORMAL;
	settingsBtn->state = GuiControlState::NORMAL;
	creditsBtn->state = GuiControlState::NORMAL;
	exitBtn->state = GuiControlState::NORMAL;

	continueBtn->state = GuiControlState::DISABLED;
	lvl1Btn->state = GuiControlState::DISABLED;
	lvl2Btn->state = GuiControlState::DISABLED;
	backBtn->boundx = 100;
	backBtn->state = GuiControlState::DISABLED;

	saveBtn->state = GuiControlState::DISABLED;
	loadBtn->state = GuiControlState::DISABLED;
	resumeBtn->state = GuiControlState::DISABLED;
	titleBtn->state = GuiControlState::DISABLED;
	vsyncToggle->state = GuiControlState::DISABLED;

	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;

	return true;
}

bool Gui::SettingsButtons()
{
	startBtn->state = GuiControlState::DISABLED;
	settingsBtn->state = GuiControlState::DISABLED;
	creditsBtn->state = GuiControlState::DISABLED;
	exitBtn->state = GuiControlState::DISABLED;

	continueBtn->state = GuiControlState::DISABLED;
	lvl1Btn->state = GuiControlState::DISABLED;
	lvl2Btn->state = GuiControlState::DISABLED;
	backBtn->boundx = 350;
	backBtn->state = GuiControlState::NORMAL;

	saveBtn->state = GuiControlState::DISABLED;
	loadBtn->state = GuiControlState::DISABLED;
	resumeBtn->state = GuiControlState::DISABLED;
	titleBtn->state = GuiControlState::DISABLED;

	vsyncToggle->state = GuiControlState::NORMAL;
	audioBtn->state = GuiControlState::NORMAL;
	videoBtn->state = GuiControlState::NORMAL;
	gameBtn->state = GuiControlState::NORMAL;

	slider1->state = GuiControlState::NORMAL;
	slider2->state = GuiControlState::NORMAL;

	return true;
}

bool Gui::StartButtons()
{
	startBtn->state = GuiControlState::DISABLED;
	settingsBtn->state = GuiControlState::DISABLED;
	creditsBtn->state = GuiControlState::DISABLED;
	exitBtn->state = GuiControlState::DISABLED;

	continueBtn->state = GuiControlState::NORMAL;
	lvl1Btn->state = GuiControlState::NORMAL;
	lvl2Btn->state = GuiControlState::NORMAL;
	backBtn->state = GuiControlState::NORMAL;

	saveBtn->state = GuiControlState::DISABLED;
	loadBtn->state = GuiControlState::DISABLED;
	resumeBtn->state = GuiControlState::DISABLED;
	titleBtn->state = GuiControlState::DISABLED;
	vsyncToggle->state = GuiControlState::DISABLED;

	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;

	return true;
}

bool Gui::NoButtons() {
	startBtn->state = GuiControlState::DISABLED;
	settingsBtn->state = GuiControlState::DISABLED;
	creditsBtn->state = GuiControlState::DISABLED;
	exitBtn->state = GuiControlState::DISABLED;

	continueBtn->state = GuiControlState::DISABLED;
	lvl1Btn->state = GuiControlState::DISABLED;
	lvl2Btn->state = GuiControlState::DISABLED;
	backBtn->state = GuiControlState::DISABLED;

	saveBtn->state = GuiControlState::DISABLED;
	loadBtn->state = GuiControlState::DISABLED;
	resumeBtn->state = GuiControlState::DISABLED;
	titleBtn->state = GuiControlState::DISABLED;
	vsyncToggle->state = GuiControlState::DISABLED;
	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;

	return true;
}

bool Gui::InGameMenu() {
	startBtn->state = GuiControlState::DISABLED;
	settingsBtn->state = GuiControlState::NORMAL;
	creditsBtn->state = GuiControlState::DISABLED;
	exitBtn->state = GuiControlState::NORMAL;

	continueBtn->state = GuiControlState::DISABLED;
	lvl1Btn->state = GuiControlState::DISABLED;
	lvl2Btn->state = GuiControlState::DISABLED;
	backBtn->state = GuiControlState::DISABLED;

	saveBtn->state = GuiControlState::NORMAL;
	loadBtn->state = GuiControlState::NORMAL;
	resumeBtn->state = GuiControlState::NORMAL;
	titleBtn->state = GuiControlState::NORMAL;
	vsyncToggle->state = GuiControlState::DISABLED;

	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;
	return true;
}

bool Gui::ButtonInit() {
	if (app->fadeToBlack->activeScene == "TitleScene") {
		uint w, h;
		app->win->GetWindowSize(w, h);

		settingsBtn->boundx = 100;
		settingsBtn->bounds.y = (int)w / 13 * 2;

		exitBtn->boundx = 100;
		exitBtn->bounds.y = (int)w / 13 * 4;
	}

	if (app->fadeToBlack->activeScene == "Scene") {
		//Button Setup
		uint w, h;
		app->win->GetWindowSize(w, h);

		settingsBtn->boundx = 510;
		settingsBtn->bounds.y = (int)w / 13 * 2;
		titleBtn->boundx = 510;
		titleBtn->bounds.y = (int)w / 13 * 3;
		exitBtn->boundx = 510;
		exitBtn->bounds.y = (int)w / 13 * 4;
	}

	return true;
}
