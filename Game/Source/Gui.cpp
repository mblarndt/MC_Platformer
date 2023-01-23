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

	CreateButton(1, "START", 100, 1024 / 13, startBtn);
	CreateButton(3, "SETTINGS", 100, 1024 / 13 * 2, settingsBtn);
	CreateButton(7, "CREDITS", 100, 1024 / 13 * 3, creditsBtn);
	CreateButton(2, "EXIT", 100, 1024 / 13 * 4, exitBtn);
	CreateButton(8, "CONTINUE", 100, 1024 / 13, continueBtn);

	CreateButton(4, "LEVEL 1", 100, 1024 / 13 * 2, lvl1Btn);
	CreateButton(5, "LEVEL 2", 100, 1024 / 13 * 3, lvl2Btn);
	CreateButton(6, "BACK", 100, 1024 / 13 * 4, backBtn);
	CreateButton(9, "RESUME", 100 + 200 + 105, 1024 / 13 * 1, resumeBtn);
	CreateButton(10, "SAVE", 100 + 200, 1024 / 13 * 2, saveBtn);
	CreateButton(11, "LOAD", 100 + 200, 1024 / 13 * 3, loadBtn);
	CreateButton(12, "TITLE", 510, 1024 / 13 * 3, titleBtn);
	CreateButton(13, "AUDIO", 229, 62, audioBtn);
	CreateButton(14, "VIDEO", 229 + 188, 62, videoBtn);
	CreateButton(15, "GAME", 229 + 374, 62, gameBtn);

	CreateToggle(19, "VSync", 420, 140 + 20, vsyncToggle);
	vsyncToggle->toggle = app->render->VSYNC;

	CreateToggle(20, "Fullscreen", 420, 195 + 20, fullscreenToggle);
	fullscreenToggle->toggle = app->win->fullscreen;

	CreateToggle(21, "Debug", 420, 140 + 20, debugToggle);
	debugToggle->toggle = app->physics->debug;
	CreateToggle(22, "Godmode", 420, 195 + 20, godmodeToggle);
	
	CreateToggle(24, "Music", 350, 140 + 20, audioToggle);
	audioToggle->toggle = app->audio->isAudioEnabled;


	SDL_Rect sliderRect2 = { 350, 200 + 20, 300-38,38 };
	musicSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 17, "Music Volume", sliderRect2, this, { 0,128,0,0 });
	musicVolume = preMusicVolume = app->audio->musicVolume;
	musicSlider->SetValue(musicVolume);

	SDL_Rect sliderRect = { 350, 250 + 20, 300-38,38 };
	fxSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 18, "FX Volume", sliderRect, this, { 0,128,0,0 });
	fxVolume = preFxVolume = app->audio->fxVolume;
	fxSlider->SetValue(fxVolume);

	SDL_Rect sliderRect3 = { 420, 250 + 20, 100, 38 };
	fpsSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 25, "FPS", sliderRect3, this, {1,3,0,0});
	
	TargetFps fps = app->physics->fpsTarget;
	if (fps == TargetFps::FIFTEEN)
		fpsSlider->SetValue(1);
	if (fps == TargetFps::THIRTY)
		fpsSlider->SetValue(2);
	if (fps == TargetFps::SIXTY)
		fpsSlider->SetValue(3);
	

	NoButtons();

	audioBtn->selected = true;
	toggle = false;

	settingsBox = app->tex->Load("Assets/Textures/SettingsBox.png");

	return true;
}

bool Gui::Update(float dt)
{

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
		credits = false;
		settings = false;
		if(app->fadeToBlack->activeScene == "TitleScene")
			MainMenuButtons();
		if (app->fadeToBlack->activeScene == "Scene")
			InGameMenu();
		break;

	case 7:
		LOG("Button 7: Show Credits");
		Credits();
		if (!credits)
			credits = true;
		else
			credits = false;
		
		break;

	case 8:
		LOG("Button 8: Continue from saved state");
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
		app->saveExist = true;
		app->scene->clickedSave = true;
		break;

	case 11:
		LOG("Button 11: Load Game");
		app->LoadGameRequest();
		app->scene->playerptr->startGame = true;
		app->scene->toggle = false;
		app->scene->gamePaused = false;
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

	case 17:
		app->audio->SetMusicVolume(musicSlider->value);
		break;

	case 18:
		app->audio->SetFxVolume(fxSlider->value);
		break;

	case 19:
		if (app->render->VSYNC == true)
			app->render->VSYNC = false;
		else
			app->render->VSYNC = true;
		break;

	case 20:
		app->win->ToggleFullscreen();
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
	case 25:
		LOG("FPS Slider Value: %i", fpsSlider->value);
		if (fpsSlider->value == 1)
			app->physics->SetFPS(TargetFps::FIFTEEN);
		if (fpsSlider->value == 2)
			app->physics->SetFPS(TargetFps::THIRTY);
		if (fpsSlider->value == 3)
			app->physics->SetFPS(TargetFps::SIXTY);
		break;
}

	
	return true;
}

bool Gui::MainMenuButtons() {

	settingsBtn->boundx = 100;
	settingsBtn->bounds.y = 1024 / 13 * 2;

	exitBtn->boundx = 100;
	exitBtn->bounds.y = 1024 / 13 * 4;

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
	fpsSlider->state = GuiControlState::DISABLED;

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
	backBtn->boundx = 229 + 188;
	backBtn->bounds.y = 352;
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
		fpsSlider->state = GuiControlState::DISABLED;

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
		fpsSlider->state = GuiControlState::NORMAL;

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
		fpsSlider->state = GuiControlState::DISABLED;

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

	if(app->saveExist == false)
		continueBtn->isActive = false;
	else
		continueBtn->isActive = true;

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
	fpsSlider->state = GuiControlState::DISABLED;

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
	fpsSlider->state = GuiControlState::DISABLED;

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
	fpsSlider->state = GuiControlState::DISABLED;
	return true;
}

bool Gui::Credits() {
	startBtn->state = GuiControlState::DISABLED;
	settingsBtn->state = GuiControlState::DISABLED;
	creditsBtn->state = GuiControlState::DISABLED;
	exitBtn->state = GuiControlState::DISABLED;

	continueBtn->state = GuiControlState::DISABLED;
	lvl1Btn->state = GuiControlState::DISABLED;
	lvl2Btn->state = GuiControlState::DISABLED;
	backBtn->boundx = 229 + 188;
	backBtn->bounds.y = 352;

	backBtn->state = GuiControlState::NORMAL;

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
	fpsSlider->state = GuiControlState::DISABLED;
	return true;
}

bool Gui::ButtonInit() {
	if (app->fadeToBlack->activeScene == "TitleScene") {
		settingsBtn->boundx = 100;
		settingsBtn->bounds.y = 1024 / 13 * 2;

		exitBtn->boundx = 100;
		exitBtn->bounds.y = 1024 / 13 * 4;
	}

	if (app->fadeToBlack->activeScene == "Scene") {
		//Button Setup
		settingsBtn->boundx = 510;
		settingsBtn->bounds.y = 1024 / 13 * 2;
		titleBtn->boundx = 510;
		titleBtn->bounds.y = 1024 / 13 * 3;
		exitBtn->boundx = 229 + 188;
		exitBtn->bounds.y = 352;
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
	if (credits) {
		int offset = 15;
		int spacing = 15;
		app->render->DrawTexture(settingsBox, 200 - app->render->camera.x, 30);
		app->render->DrawText("Credits", 424, 70, 0, 0, "white", false,"big");
		app->render->DrawText("Programming", 420, 150 - offset, 0, 0, "black", false);
		app->render->DrawText("Art & Animations:", 390, 180 - offset, 0, 0, "black", false);
		app->render->DrawText("Ben Arndt", 438, 220 - offset, 0, 0, "white", false);

		app->render->DrawText("Programming:", 415, 300 - 30, 0, 0, "black", false);
		app->render->DrawText("Pau Fusco", 438, 340 - 30, 0, 0, "white", false);
	}

	if (app->scene->toggle)
		app->render->DrawTexture(settingsBox, 200 - app->render->camera.x, 30);

	if (settings == true) {
		app->render->DrawTexture(settingsBox, 200 - app->render->camera.x, 30);



		switch (settingsState) {

		case SettingsState::AUDIO:
			app->render->DrawText("Sound", 250, 145 + 20, 0, 0, "white", false);
			app->render->DrawText("Music", 250, 200 + 20, 0, 0, "white", false);
			app->render->DrawText("FX", 250, 250 + 20, 0, 0, "white", false);
			break;
		case SettingsState::VIDEO:
			app->render->DrawText("VSync", 250, 145 + 20, 0, 0, "white", false);
			app->render->DrawText("Fullscreen", 250, 200 + 20, 0, 0, "white", false);
			app->render->DrawText("FPS:", 250, 250 + 20, 0, 0, "white", false);

			if (fpsSlider->value == 1)
				currentFps = "15 fps";
			else if (fpsSlider->value == 2)
				currentFps = "30 fps";
			else if (fpsSlider->value == 3)
				currentFps = "60 fps";

			app->render->DrawText(currentFps, 250 + 65, 250 + 20, 0, 0, "white", false);
			break;

		case SettingsState::GAME:
			app->render->DrawText("Debug", 250, 145 + 20, 0, 0, "white", false);
			app->render->DrawText("GodMode", 250, 200 + 20, 0, 0, "white", false);
			break;
		}
	}

	return true;
}

bool Gui::SetSceneState(const char* scene)
{
	if (scene == "title")
		mainState = MainState::TITLE;
	if (scene == "ingame")
		mainState = MainState::IN_GAME;
	return true;
}

