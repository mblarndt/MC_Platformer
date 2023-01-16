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
	uint w, h;
	app->win->GetWindowSize(w, h);

	CreateButton(1, "START", 100, (int)w / 13, startBtn);
	CreateButton(3, "SETTINGS", 100, (int)w / 13 * 2, settingsBtn);
	CreateButton(7, "CREDITS", 100, (int)w / 13 * 3, creditsBtn);
	CreateButton(2, "EXIT", 100, (int)w / 13 * 4, exitBtn);
	CreateButton(8, "CONTINUE", 100, (int)w / 13, continueBtn);
	CreateButton(4, "LEVEL 1", 100, (int)w / 13 * 2, lvl1Btn);
	CreateButton(5, "LEVEL 2", 100, (int)w / 13 * 3, lvl2Btn);
	CreateButton(6, "BACK", 100, (int)w / 13 * 4, backBtn);
	CreateButton(9, "RESUME", 100 + 200, (int)w / 13 * 2, resumeBtn);
	CreateButton(10, "SAVE", 100 + 200, (int)w / 13 * 3, saveBtn);
	CreateButton(11, "LOAD", 100 + 200, (int)w / 13 * 4, loadBtn);
	CreateButton(12, "TITLE", 510, (int)w / 13 * 3, titleBtn);
	CreateButton(13, "AUDIO", 229, 30, audioBtn);
	CreateButton(14, "VIDEO", 229 + 188, 30, videoBtn);
	CreateButton(15, "GAME", 229 + 374, 30, gameBtn);

	CreateToggle(19, "VSync", 420, 140, vsyncToggle);
	vsyncToggle->toggle = app->render->VSYNC;

	CreateToggle(20, "Fullscreen", 420, 195, fullscreenToggle);

	CreateToggle(21, "Debug", 420, 140, debugToggle);
	debugToggle->toggle = app->physics->debug;
	CreateToggle(22, "Godmode", 420, 195, godmodeToggle);
	
	CreateToggle(24, "Music", 350, 140, audioToggle);
	audioToggle->toggle = app->audio->isAudioEnabled;


	SDL_Rect sliderRect2 = { 350, 200, 300,38 };
	musicSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 17, "Music Volume", sliderRect2, this);

	SDL_Rect sliderRect = { 350, 250, 300,38 };
	fxSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 18, "FX Volume", sliderRect, this);

	

	NoButtons();

	audioBtn->selected = true;
	toggle = false;

	settingsBox = app->tex->Load("Assets/Textures/SettingsBox.png");

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
		settingsState = SettingsState::AUDIO;
		SettingsButtons();
		break;

	case 14:
		settingsState = SettingsState::VIDEO;
		SettingsButtons();
		break;

	case 15:
		settingsState = SettingsState::GAME;
		SettingsButtons();
		break;

	case 19:
		if (app->render->VSYNC == true)
			app->render->VSYNC = false;
		else
			app->render->VSYNC = true;
		break;

	case 20:

		break;

	case 21:
		if (app->physics->debug == false)
			app->physics->debug = true;
		else
			app->physics->debug = false;
		break;
	case 24:
		app->audio->ToggleMusic();
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

	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	musicSlider->state = GuiControlState::DISABLED;
	fxSlider->state = GuiControlState::DISABLED;

	audioToggle->state = GuiControlState::DISABLED;
	vsyncToggle->state = GuiControlState::DISABLED;
	fullscreenToggle->state = GuiControlState::DISABLED;
	debugToggle->state = GuiControlState::DISABLED;
	godmodeToggle->state = GuiControlState::DISABLED;

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
	backBtn->boundx = 250;
	backBtn->bounds.y = 370;
	backBtn->state = GuiControlState::NORMAL;

	saveBtn->state = GuiControlState::DISABLED;
	loadBtn->state = GuiControlState::DISABLED;
	resumeBtn->state = GuiControlState::DISABLED;
	titleBtn->state = GuiControlState::DISABLED;

	audioBtn->state = GuiControlState::NORMAL;
	videoBtn->state = GuiControlState::NORMAL;
	gameBtn->state = GuiControlState::NORMAL;

	switch (settingsState) {
	case SettingsState::AUDIO:
		audioBtn->selected = true;
		videoBtn->selected = false;
		gameBtn->selected = false;

		audioToggle->state = GuiControlState::NORMAL;
		musicSlider->state = GuiControlState::NORMAL;
		fxSlider->state = GuiControlState::NORMAL;

		vsyncToggle->state = GuiControlState::DISABLED;
		fullscreenToggle->state = GuiControlState::DISABLED;

		debugToggle->state = GuiControlState::DISABLED;
		godmodeToggle->state = GuiControlState::DISABLED;
		break;
	case SettingsState::VIDEO:
		audioBtn->selected = false;
		videoBtn->selected = true;
		gameBtn->selected = false;

		audioToggle->state = GuiControlState::DISABLED;
		musicSlider->state = GuiControlState::DISABLED;
		fxSlider->state = GuiControlState::DISABLED;

		vsyncToggle->state = GuiControlState::NORMAL;
		fullscreenToggle->state = GuiControlState::NORMAL;

		debugToggle->state = GuiControlState::DISABLED;
		godmodeToggle->state = GuiControlState::DISABLED;
		break;
	case SettingsState::GAME:
		audioBtn->selected = false;
		videoBtn->selected = false;
		gameBtn->selected = true;

		audioToggle->state = GuiControlState::DISABLED;
		musicSlider->state = GuiControlState::DISABLED;
		fxSlider->state = GuiControlState::DISABLED;

		vsyncToggle->state = GuiControlState::DISABLED;
		fullscreenToggle->state = GuiControlState::DISABLED;

		debugToggle->state = GuiControlState::NORMAL;
		godmodeToggle->state = GuiControlState::NORMAL;
		break;
	}
	
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
	backBtn->bounds.y = 1024 / 13 * 4;
	backBtn->state = GuiControlState::NORMAL;

	saveBtn->state = GuiControlState::DISABLED;
	loadBtn->state = GuiControlState::DISABLED;
	resumeBtn->state = GuiControlState::DISABLED;
	titleBtn->state = GuiControlState::DISABLED;
	vsyncToggle->state = GuiControlState::DISABLED;

	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	musicSlider->state = GuiControlState::DISABLED;
	fxSlider->state = GuiControlState::DISABLED;

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
	audioToggle->state = GuiControlState::DISABLED;
	fullscreenToggle->state = GuiControlState::DISABLED;
	debugToggle->state = GuiControlState::DISABLED;
	godmodeToggle->state = GuiControlState::DISABLED;

	musicSlider->state = GuiControlState::DISABLED;
	fxSlider->state = GuiControlState::DISABLED;

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
	audioToggle->state = GuiControlState::DISABLED;
	fullscreenToggle->state = GuiControlState::DISABLED;
	debugToggle->state = GuiControlState::DISABLED;
	godmodeToggle->state = GuiControlState::DISABLED;

	audioBtn->state = GuiControlState::DISABLED;
	videoBtn->state = GuiControlState::DISABLED;
	gameBtn->state = GuiControlState::DISABLED;

	musicSlider->state = GuiControlState::DISABLED;
	fxSlider->state = GuiControlState::DISABLED;
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

bool Gui::CreateButton(int id, const char* label, int x, int y, GuiButton*& button)
{
	int w = 190;
	int h = 66;
	button = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, id, label, { x, y, w, h }, this);
	return button != nullptr;
}

bool Gui::CreateToggle(int id, const char* label, int x, int y, GuiToggle*& toggle)
{
	int w = 78;
	int h = 48;
	toggle = (GuiToggle*)app->guiManager->CreateGuiControl(GuiControlType::TOGGLE, id, label, { x, y, w, h }, this);
	return toggle != nullptr;
}

bool Gui::SettingsWindow() {
	if (app->scene->toggle)
		app->render->DrawTexture(settingsBox, 229 - app->render->camera.x, 30);

	if (settings == true) {
		app->render->DrawTexture(settingsBox, 229 - app->render->camera.x, 30);
		if (audioBtn->selected) {
			app->render->DrawText("Sound", 250, 145, 0, 0, "white", false);
			app->render->DrawText("Music", 250, 200, 0, 0, "white", false);
			app->render->DrawText("FX", 250, 250, 0, 0, "white", false);
		}

		else if (videoBtn->selected == true) {
			app->render->DrawText("VSync", 250, 145, 0, 0, "white", false);
			app->render->DrawText("Fullscreen", 250, 200, 0, 0, "white", false);
		}

		else if (gameBtn->selected == true) {
			app->render->DrawText("Debug", 250, 145, 0, 0, "white", false);
			app->render->DrawText("GodMode", 250, 200, 0, 0, "white", false);
		}
	}

	

	return true;
}