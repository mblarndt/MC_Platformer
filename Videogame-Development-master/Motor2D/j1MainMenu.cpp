#include "j1App.h"
#include "j1Gui.h"
#include "j1FadeToBlack.h"
#include "j1Scene.h"
#include "j1Collisions.h"
#include "j1MainMenu.h"
#include "j1Audio.h"

j1MainMenu::j1MainMenu() : j1Module()
{
	name.create("mainmenu");
}

j1MainMenu::~j1MainMenu()
{

}

bool j1MainMenu::Awake()
{
	return true;
}

bool j1MainMenu::Start()
{
	//Music
	App->audio->PlayMusic("audio/music/Interstellar_msc.ogg", 0.0f);

	//Textures
	basic_button_texture = App->tex->Load("gui/UI_Button.png");

	x_button_texture = App->tex->Load("gui/UI_Button_close.png");

	border_tex = App->tex->Load("gui/UI_Border.png");

	SDL_Texture* icons = App->tex->Load("gui/UI_Slider.png");

	//Text load
	font = App->font->Load("fonts/MajorMonoDisplay.ttf", 25); //Magic

	
	//Buttons
	background =  App->gui->CreateImage(0, 0, true, false, { 0,0,1024,768 }, App->tex->Load("gui/Titlescreen.png"));
	UI_mainmenulist.add(background);

	play_button = App->gui->CreateButton(700, 250, true, false, "plAy", &bootleg_color,font, &default_rect, &hover_rect, &press_rect, basic_button_texture, button_rect, 80, 13);
	UI_mainmenulist.add(play_button);
	
	continue_button = App->gui->CreateButton(700, 350, true, false, "continue", &bootleg_color, font, &default_rect, &hover_rect, &press_rect, basic_button_texture, button_rect, 50, 13);
	UI_mainmenulist.add(continue_button);
	
	credits_button = App->gui->CreateButton(700, 450, true, false, "credits", &bootleg_color, font, &default_rect, &hover_rect, &press_rect, basic_button_texture, button_rect, 60, 13);
	UI_mainmenulist.add(credits_button);

	settings_button = App->gui->CreateButton(700, 550, true, false, "settings", &bootleg_color, font, &default_rect, &hover_rect, &press_rect, basic_button_texture, button_rect, 50, 13);
	UI_mainmenulist.add(settings_button);

	exit_button = App->gui->CreateButton(700, 650, true, false, "exit", &bootleg_color, font, &default_rect, &hover_rect, &press_rect, basic_button_texture, button_rect, 80, 13);
	UI_mainmenulist.add(exit_button);

	//Settings
	x_button = App->gui->CreateButton(0, 0, false, false, "", &bootleg_color, font, &x_default, &x_hover, &x_press, x_button_texture, x_rect, 0, 0);
	settings_window = App->gui->CreateUiWindow(175, 300,false,x_button,border_tex);
	//Credits
	credits_window = App->gui->CreateUiWindow(175, 300, false, x_button, border_tex);

	link_button = App->gui->CreateButton(700, 650, false, false, "website", &bootleg_color, font, &default_rect, &hover_rect, &press_rect, basic_button_texture, button_rect, 60, 13);
	credits_window->addUI(link_button, 100, 300);

	credits_window->addUI(App->gui->CreateText(0, 0, false, false, "credits", &bootleg_color, font), 150, 20);

	fx_slider = App->gui->CreateSlider(0, 0, false, false);
	settings_window->addUI(fx_slider, 90, 125);
	settings_window->addUI(App->gui->CreateText(0, 0, false, false, "fx", &bootleg_color, font), 90, 90);
	settings_window->addUI(App->gui->CreateImage(0, 0, false, false, SDL_Rect{76, 48, 19, 22}, icons), 104, 138);

	music_slider = App->gui->CreateSlider(0, 0, false, false);
	settings_window->addUI(music_slider, 90, 275);
	settings_window->addUI(App->gui->CreateText(0, 0, false, false, "music", &bootleg_color, font), 90, 240);
	settings_window->addUI(App->gui->CreateImage(0, 0, false, false, SDL_Rect{ 95, 48, 19, 22 }, icons), 104, 288);

	settings_window->addUI(App->gui->CreateText(0,0,false,false,"settings", &bootleg_color,font), 150, 20);

	return true;
}

bool j1MainMenu::PreUpdate()
{
	return true;
}

bool j1MainMenu::Update(float dt)
{
	//BUTTONS
	if (play_button->buttonPressed())
	{
		App->mainmenu->disactivate();
		App->scene->activate();
		App->entities->activate();
		App->collisions->activate();

	}

	if (continue_button->buttonPressed())
	{
		App->mainmenu->disactivate();
		App->scene->activate();
		App->entities->activate();
		App->collisions->activate();

		App->LoadGame();

	}
	
	if (credits_button->buttonPressed())
	{
		credits_window->enable();
	}
	
	if (settings_button->buttonPressed())
	{
		settings_window->enable();
	}

	if (exit_button->buttonPressed())
	{
		App->input->CloseGame();
	}

	if (link_button->buttonPressed())
	{
		//ShellExecuteA(NULL, "open", "https://paupedra.github.io/Videogame-Development/", NULL , NULL , SW_SHOWNORMAL);
	}

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->frame_rate == 0)
		{
			App->frame_rate = 30;
		}
		else
		{
			App->frame_rate = 0;
		}
	}

	

	
	
	App->audio->SetFxVol(fx_slider->SliderValue());
	App->audio->SetMusicVol(music_slider->SliderValue());



	return true;
}

bool j1MainMenu::PostUpdate()
{
	return true;
}

bool j1MainMenu::CleanUp()
{
	p2List_item<UI*>* it = UI_mainmenulist.start;

	while (it != nullptr)
	{
		it->data->Disable();
		it = it->next;
	}

	return true;
}