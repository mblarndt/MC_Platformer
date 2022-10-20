#define _CRT_SECURE_NO_WARNINGS
#include "j1Gui.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1EntityMachine.h"
#include "j1FadeToBlack.h"
UI_Console::UI_Console(int x, int y, bool enabled, bool draggable, SDL_Color* color, _TTF_Font* font, SDL_Rect rect) : UI(x,y,enabled,draggable), base_rect(rect),color(color),font(font)
{
	texture = App->font->Print(text.GetString(), *color, font);
	base_rect.x = position.x;
	base_rect.y = position.y;
	bruh = App->audio->LoadFx("audio/fx/bruh.wav");
	input_rect = {0,275,700,25};
	input_rect.x = position.x;
	
}

void UI_Console::Update()
{
	base_rect.x = position.x;
	base_rect.y = position.y;

	int x, y;

	App->input->GetMousePosition(x, y);

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		Execute((char*)text.GetString());
		AddText((char*)text.GetString());
		text.Clear();
	}

	p2List_item<UI_Text*>* textIt = logs.start;

	while (textIt != nullptr)
	{
		textIt->data->Update();

		textIt = textIt->next;
	}


}

void UI_Console::Draw()
{
	App->render->DrawQuad(base_rect, 75, 75, 75, 255,true,false);
	App->render->DrawQuad(input_rect, 0, 0, 0, 255,true,false);

	p2List_item<UI_Text*>* textIt = logs.start;

	while (textIt != nullptr)
	{
		if (textIt->data->position.x >= base_rect.x
			&& textIt->data->position.x < base_rect.x + base_rect.w
			&& textIt->data->position.y > base_rect.y
			&& textIt->data->position.y < base_rect.y + base_rect.h)
		{
			textIt->data->Draw();
		}
		textIt = textIt->next;
	}

	if (text != "")
	{
		App->render->Blit(texture, position.x , position.y + base_rect.h - 25, NULL, false, 0.f);
	}
}

void UI_Console::UpdateText()
{
	//SDL_DestroyTexture(texture);

	texture = App->font->Print(text.GetString(), *color, font);
}

const char* UI_Console::GetText()const
{
	return text.GetString();
}

void UI_Console::ConcatNewText(char* newText)
{
	//oldText = text;
	strcat((char*)text.GetString(), newText);
	UpdateText();
}

void UI_Console::Execute(char* call)
{
	if (strcmp("quit", call) == 0)
	{
		App->input->CloseGame();
	}

	if (strcmp("god_mode", call) == 0)
	{
		if (App->entities->player != nullptr)
		{
			App->entities->player->GodMode();
		}
	}

	if (strcmp("bruh", call) == 0)
	{
		App->audio->PlayFx(bruh);
	}

	if (strcmp("FPS <30>", call) == 0)
	{
		App->frame_rate = 30;
	}

	if (strcmp("FPS <45>", call) == 0)
	{
		App->frame_rate = 45;
	}
	if (strcmp("FPS <60>", call) == 0)
	{
		App->frame_rate = 60;
	}
	if (strcmp("FPS <90>", call) == 0)
	{
		App->frame_rate = 90;
	}
	if (strcmp("FPS <120>", call) == 0) 
	{
		App->frame_rate = 120;
	}
	if (strcmp("FPS <144>", call) == 0) //144 bcz u plev, all hail pc master reis up to 144hz
	{
		App->frame_rate = 144;
	}

	if (strcmp("map <1>", call) == 0)
	{
		App->fade->FadeToBlack(1);
	}
	if (strcmp("map <2>", call) == 0)
	{
		App->fade->FadeToBlack(2);
	}

	if (strcmp("list", call) == 0)
	{
		AddText("list of commands:");
		AddText("list - Shows the list of possible commands");
		AddText("quit - Quits the game");
		AddText("god_mode - Toggles godmode on player");
		AddText("FPS <number> - Sets frame rate to specified number");
		AddText("map <number> - Loads specified map (1 or 2)");
	}

}

void UI_Console::AddText(char* string)
{

	p2List_item<UI_Text*>* textIt = logs.start;

	while (textIt != nullptr)
	{
		textIt->data->position.y -= 25;

		textIt = textIt->next;
	}

	logs.add(new UI_Text(position.x + 5, 250, true, false, string, color, font));
}