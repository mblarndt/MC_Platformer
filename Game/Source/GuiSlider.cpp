#include "App.h"
#include "GuiControl.h"
#include "Input.h"
#include "GuiSlider.h"
#include "Textures.h"
#include "Scene.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, bool draggable, int sliderposition) : GuiControl(GuiControlType::SLIDER, id)
{
	clickable_rect = bounds;

	thumb_offset = 46;

	sliderpos = sliderposition;

	texture = app->tex->Load("Assets/Textures/UI_Slider.png");

	bar = { 0, 0, 280, 48 };
	thumb = { 0, 48, 36, 28 };
	thumb_hovered = { 38, 48, 36, 28 };

	clickable_rect = { app->render->camera.x + 46, app->render->camera.y + 4, 188, 40 };
}

GuiSlider::~GuiSlider()
{
}


bool GuiSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		clickable_rect = { app->render->camera.x + 46, app->render->camera.y + 4, 188, 40 };

		hovered = false;
		int x, y;//Mouse coords

		app->input->GetMousePosition(x, y);


		if (x > clickable_rect.x && x < clickable_rect.x + clickable_rect.w
			&& y > clickable_rect.y && y < clickable_rect.y + clickable_rect.h)
		{
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				state = GuiControlState::PRESSED;

				hovered = true;
				sliderpos = x - app->render->camera.x - thumb_offset;
				if (sliderpos < 0)	sliderpos = 0;
				else if (sliderpos > 152) sliderpos = 152;
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

bool GuiSlider::Draw(Render* render) {
	
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
		app->render->DrawTexture(texture, app->render->camera.x, app->render->camera.y, &bar, false, .0f);

	} break;

	//L15: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		app->render->DrawTexture(texture, app->render->camera.x + thumb_offset + sliderpos, app->render->camera.y + 10, &thumb_hovered, false, .0f);
		break;
	} 
	case GuiControlState::PRESSED:
	{
		app->render->DrawTexture(texture, app->render->camera.x + thumb_offset + sliderpos, app->render->camera.y + 10, &thumb, false, .0f);
		break;
	} 

	case GuiControlState::SELECTED:
		break;
		

	default:
		break;
	}

	return false;
}

float GuiSlider::SliderValue() {

	return ((float)sliderpos / 152.0f);
}


