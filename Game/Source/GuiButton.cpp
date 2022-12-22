#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	if(id == 1)
		buttonTex = app->tex->Load("Assets/Textures/button_start.png");
	if (id == 2)
		buttonTex = app->tex->Load("Assets/Textures/button_exit.png");
	if (id == 3)
		buttonTex = app->tex->Load("Assets/Textures/button_options.png");
	if (id == 4)
		buttonTex = app->tex->Load("Assets/Textures/button_level1.png");
	if (id == 5)
		buttonTex = app->tex->Load("Assets/Textures/button_level2.png");
	if (id == 6)
		buttonTex = app->tex->Load("Assets/Textures/button_back.png");
	if (id == 7)
		buttonTex = app->tex->Load("Assets/Textures/button_save.png");
	if (id == 8)
		buttonTex = app->tex->Load("Assets/Textures/button_load.png");
	if (id == 9)
		buttonTex = app->tex->Load("Assets/Textures/button_menu.png");

}

GuiButton::~GuiButton()
{
	delete buttonTex;
}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				NotifyObserver();
			}

		}
		else {
			state = GuiControlState::NORMAL;
		}

	}

	return false;
}

bool GuiButton::Draw(Render* render)
{

	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED: 
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		//render->DrawRectangle(bounds, 255, 0, 0, 255);
		SDL_Rect rect = {0,70,190,66};
		render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

	} break;

	//L15: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		//render->DrawRectangle(bounds, 255, 255, 255, 160);
		SDL_Rect rect = { 0,0,190,66 };
		render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);

	} break;
	case GuiControlState::PRESSED:
	{
		SDL_Rect rect = { 0,141,190,66 };
		render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		

	} break;

	case GuiControlState::SELECTED: render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;

	default:
		break;
	}

	return false;
}