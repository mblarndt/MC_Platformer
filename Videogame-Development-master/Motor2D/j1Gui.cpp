#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

//#include "UI_Window.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	bool ret = true;
	click_audio = App->audio->LoadFx("audio/fx/button_click.wav");
	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	click_audio = App->audio->LoadFx("audio/fx/button_click.wav");

	consoleFont = App->font->Load("fonts/Minecraftia-Regular.ttf", 20);
	console = App->gui->CreateConsole(150, 0, false, false, &white, consoleFont, console_rect);


	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	p2List_item<UI*>* it = UI_list.start;

	while (it != nullptr)
	{
		if(it->data->IsEnabled() == true && it->data->update_it) it->data->Update();

		it = it->next;
	}

	p2List_item<UI_Window*>* winit = Windows_list.end;

	while (winit != nullptr)
	{
		if (winit->data->enabled )
		{ 
			winit->data->Update(); 

			disableUI();

			return true;
		}

		winit = winit->prev;
	}

	enableUI();

	return true;
}

// Update all guis
bool j1Gui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		p2List_item<UI*>* it = UI_list.start;

		while (it != nullptr)
		{
			it->data->debug = !it->data->debug;
			it = it->next;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN && !console->IsEnabled())
	{
		console->Enable();
		console->text.Clear();
		SDL_StartTextInput();
	}
	else if (App->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN && console->IsEnabled())
	{
		console->Disable();
		SDL_StopTextInput();
	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<UI*>* it = UI_list.start;

	while (it != nullptr)
	{

		if (it->data->IsEnabled() == true )  it->data->Draw();

		it = it->next;
	}

	p2List_item<UI_Window*>* winit = Windows_list.start;

	while (winit != nullptr)
	{

		if (winit->data->enabled)
		{
			winit->data->Draw();
		}
		winit = winit->next;
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	return true;
}

UI_Text* j1Gui::CreateText(int x, int y, bool enabled, bool draggable, p2SString text, SDL_Color* color, _TTF_Font* font)
{
	UI_Text* ret = new UI_Text(x,y, enabled, draggable,text,color,font);

	UI_list.add(ret);

	return ret;
}

UI_Image* j1Gui::CreateImage(int x, int y, bool enabled ,bool draggable, SDL_Rect rect,SDL_Texture* image)
{
	UI_Image* ret = new UI_Image(x,y, enabled, draggable, rect, image);

	UI_list.add(ret);

	return ret;
}

UI_Button* j1Gui::CreateButton(int x, int y, bool active, bool draggable, p2SString text, SDL_Color* color, _TTF_Font* font, SDL_Rect* default_rect, SDL_Rect* hover_rect, SDL_Rect* press_rect, SDL_Texture* image,SDL_Rect rect,int x_offset, int y_offset)
{
	UI_Button* ret = new UI_Button(x,y,active,draggable,text,color,font,default_rect,hover_rect,press_rect,image,rect,x_offset,y_offset);

	UI_list.add(ret);

	return ret;
}

UI_Window* j1Gui::CreateUiWindow(int x, int y, int enabled,UI_Button* x_button,SDL_Texture* board_texture)
{
	UI_Window* ret = new UI_Window(x, y, enabled,x_button, board_texture);

	Windows_list.add(ret);

	return ret;
}

UI_Slider* j1Gui::CreateSlider(int x, int y, bool active, bool draggable, int sliderposition)
{
	UI_Slider* ret = new UI_Slider(x, y, active, draggable, sliderposition);

	UI_list.add(ret);

	return ret;
}

UI_Console* j1Gui::CreateConsole(int x, int y, bool enabled, bool draggable, SDL_Color* color, _TTF_Font* font, SDL_Rect rect)
{
	UI_Console* ret = new UI_Console(x, y, enabled,draggable,color,font,rect);

	UI_list.add(ret);

	return ret;
}

void j1Gui::disableUI()
{
	p2List_item<UI*>* it = UI_list.start;

	while (it != nullptr)
	{

		if (it->data->IsEnabled() == true)  it->data->update_it = false;

		it = it->next;
	}
}

void j1Gui::enableUI()
{
	p2List_item<UI*>* it = UI_list.start;

	while (it != nullptr)
	{

		if (it->data->IsEnabled() == true)  it->data->update_it = true;

		it = it->next;
	}
}