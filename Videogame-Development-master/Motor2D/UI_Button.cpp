#include "j1App.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "p2Point.h"
#include "p2Log.h"


UI_Button::UI_Button(int x, int y, bool enabled, bool draggable, p2SString text, SDL_Color* color, _TTF_Font* font, SDL_Rect* default_rect, SDL_Rect* hover_rect, SDL_Rect* press_rect, SDL_Texture* image,SDL_Rect rect, int x_offset, int y_offset)
: UI(x,y, enabled ,draggable) ,font(font), default_rect(default_rect) , hover_rect(hover_rect), press_rect(press_rect), image_texture(image),text(text),color(color),rect(rect),textPos(x_offset,y_offset)
{
	//font = App->font->Load(path, size);
	
	text_texture = App->font->Print(text.GetString(),*color,font);

	X_offset = x_offset;
	Y_offset = y_offset;
	rect.x = position.x;
	rect.y = position.y;
}

void UI_Button::Update()
{
	rect.x = position.x;
	rect.y = position.y;

	textPos.x = position.x + X_offset;
	textPos.y = position.y + Y_offset;

	pressed = false;

	state = Button_State::DEFAULT;

	int x, y;

	App->input->GetMousePosition(x, y);

	if (x > rect.x && x < rect.x + rect.w
		&& y > rect.y && y < rect.y + rect.h)
	{
		state = Button_State::HOVER;
		
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT || App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			state = Button_State::PRESSED;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !pressed)
		{
			LOG("Button do thing");
			App->audio->PlayFx(App->gui->click_audio);
			pressed = true;
		}
	}
}

void UI_Button::Draw()
{
	switch (state)
	{
	case Button_State::DEFAULT:

		App->render->Blit(image_texture, position.x, position.y, default_rect, false, 0.f);
		break;

	case Button_State::HOVER:

		App->render->Blit(image_texture, position.x, position.y, hover_rect, false, 0.f);
		break;

	case Button_State::PRESSED:

		App->render->Blit(image_texture, position.x, position.y, press_rect, false, 0.f);
		break;
	}
	
	if (debug)
	{
		App->render->DrawQuad(rect,0,255,255,50);
	}
	if (text != "")
	{
		App->render->Blit(text_texture, textPos.x, textPos.y, NULL, false, 0.f);
	}
}

bool UI_Button::buttonPressed()
{
	return pressed;
}