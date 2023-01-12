#include "App.h"
#include "Gui.h"
#include "Window.h"
#include "Scene.h"
#include "FadeToBlack.h"

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
	buttons[0] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "StartButton", { 100, (int)w / 10,     190, 66 }, this);
	buttons[1] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "ExitButton", { 100, (int)w / 10 * 3, 190, 66 }, this);
	buttons[2] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "OptionsButton", { 100, (int)w / 10 * 2, 190, 66 }, this);
	buttons[3] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Level 1", { 100 + 200, (int)w / 10,     190, 66 }, this);
	buttons[4] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Level 2", { 100 + 200, (int)w / 10 * 2, 190, 66 }, this);
	buttons[5] = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Back", { 100 + 200, (int)w / 10 * 3, 190, 66 }, this);

	SDL_Rect sliderRect = { 300, 250, 300,38 };
	slider1 = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 13, "Slider 1", sliderRect, this);

	SDL_Rect sliderRect2 = { 300, 200, 300,38 };
	slider2 = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 14, "Slider 1", sliderRect2, this);

	NoButtons();

	return true;
}

bool Gui::Update(float dt)
{
	if (mainMenu) {
		MainMenuButtons();
		mainMenu = false;
	}

	if (settings) {
		SettingsButtons();
		settings = false;
	}

	if (nothing) {
		NoButtons();
		nothing = false;
	}


	//app->guiManager->Draw();

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
		LOG("Button 1 click");
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
		MainMenuButtons();
	}
	
	return true;
}

bool Gui::MainMenuButtons() {
	buttons[3]->state = GuiControlState::DISABLED;
	buttons[4]->state = GuiControlState::DISABLED;
	buttons[5]->state = GuiControlState::DISABLED;
	buttons[0]->state = GuiControlState::NORMAL;
	buttons[1]->state = GuiControlState::NORMAL;
	buttons[2]->state = GuiControlState::NORMAL;
	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;
	return true;
}

bool Gui::SettingsButtons()
{
	buttons[0]->state = GuiControlState::DISABLED;
	buttons[1]->state = GuiControlState::DISABLED;
	buttons[2]->state = GuiControlState::DISABLED;
	buttons[3]->state = GuiControlState::DISABLED;
	buttons[4]->state = GuiControlState::DISABLED;
	buttons[5]->state = GuiControlState::NORMAL;
	slider1->state = GuiControlState::NORMAL;
	slider2->state = GuiControlState::NORMAL;

	return true;
}

bool Gui::StartButtons()
{
	buttons[0]->state = GuiControlState::DISABLED;
	buttons[1]->state = GuiControlState::DISABLED;
	buttons[2]->state = GuiControlState::DISABLED;
	buttons[3]->state = GuiControlState::NORMAL;
	buttons[4]->state = GuiControlState::NORMAL;
	buttons[5]->state = GuiControlState::NORMAL;
	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;

	return true;
}

bool Gui::NoButtons() {
	buttons[0]->state = GuiControlState::DISABLED;
	buttons[1]->state = GuiControlState::DISABLED;
	buttons[2]->state = GuiControlState::DISABLED;
	buttons[3]->state = GuiControlState::DISABLED;
	buttons[4]->state = GuiControlState::DISABLED;
	buttons[5]->state = GuiControlState::DISABLED;

	slider1->state = GuiControlState::DISABLED;
	slider2->state = GuiControlState::DISABLED;

	return true;
}
