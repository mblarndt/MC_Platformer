#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1FadeToBlack.h"
#include "j1EntityMachine.h"
#include "j1Pathfinding.h"
#include "j1Gui.h"
#include "j1MainMenu.h"
//#include "j1MainMenu.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;



	channel = App->audio->LoadFx("audio/fx/Ambience.wav");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	if (App->map->Load(1) == true) {

	}

	InGameTime = new j1Timer;
	
	debug_tex = App->tex->Load("maps/path.png");


	//Ambience sound

	App->audio->PlayFx(channel, -1);

	//Music
	App->audio->PlayMusic("audio/music/roundabout_msc.ogg", 0.0f);

	//HUD
	font = App->font->Load("fonts/MajorMonoDisplay.ttf", 25);
	border_tex = App->tex->Load("gui/UI_Border.png");

	//Starting Values
	time = 180;
	coin_score = 0;
	score = 0;
	hp = 3;

	UI_HUD_Left_Tex = App->tex->Load("gui/UI_HUD_Left.png");
	UI_HUD_Right_Tex = App->tex->Load("gui/UI_HUD_Right.png");

	UI_HUD_Left = App->gui->CreateImage(10, 10, true, false, {0,0,132,88}, UI_HUD_Left_Tex);
	UI_HUD_Right = App->gui->CreateImage(856, 10, true, false, {0,0,158,104}, UI_HUD_Right_Tex);

	UI_Life_Segments[0] = App->gui->CreateImage(28, 10, true, false, SDL_Rect{ 132,0,30,34 }, UI_HUD_Left_Tex);
	UI_Life_Segments[1] = App->gui->CreateImage(60, 10, true, false, SDL_Rect{ 132,0,30,34 }, UI_HUD_Left_Tex);
	UI_Life_Segments[2] = App->gui->CreateImage(92, 10, true, false, SDL_Rect{ 132,0,30,34 }, UI_HUD_Left_Tex);
	
	//Text
	SDL_Color black = { 0,0,0 };

	time_text = App->gui->CreateText(72, 59, true, false, "0", &bootleg_color, font);
	score_text = App->gui->CreateText(900, 76, true, false, "0", &bootleg_color, font);
	coin_score_text = App->gui->CreateText(943, 19, true, false, "0", &black, font);

	//Settings
	SDL_Texture* x_button_texture = App->tex->Load("gui/UI_Button_close.png");
	x_button = App->gui->CreateButton(0, 0, false, false, "", &bootleg_color, font, &x_default, &x_hover, &x_press, x_button_texture, x_rect, 0, 0);

	settings_window = App->gui->CreateUiWindow(275, 200, false, x_button,border_tex);

	SDL_Texture* icons = App->tex->Load("gui/UI_Slider.png");

	int currentVol = ((float)App->audio->SetFxVol(-1.0f)/128.0f)*152.0f;

	fx_slider = App->gui->CreateSlider(0, 0, false, false, currentVol);
	settings_window->addUI(fx_slider, 90, 125);
	settings_window->addUI(App->gui->CreateText(0, 0, false, false, "fx", &bootleg_color, font), 90, 90);
	settings_window->addUI(App->gui->CreateImage(0, 0, false, false, SDL_Rect{ 76, 48, 19, 22 }, icons), 104, 138);

	currentVol = ((float)App->audio->SetMusicVol(-1.0f) / 128.0f)*152.0f;
	
	music_slider = App->gui->CreateSlider(0, 0, false, false, currentVol);
	settings_window->addUI(music_slider, 90, 275);
	settings_window->addUI(App->gui->CreateText(0, 0, false, false, "music", &bootleg_color, font), 90, 240);
	settings_window->addUI(App->gui->CreateImage(0, 0, false, false, SDL_Rect{ 95, 48, 19, 22 }, icons), 104, 288);

	settings_window->addUI(App->gui->CreateText(0, 0, false, false, "settings", &bootleg_color, font), 150, 20);

	InGameTime->Start();
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}


void j1Scene::HUD() {
//	LOG("HUD");
	
	char str[6]; 

	time = (180 - (int)InGameTime->ReadSec());
	if (time < 0) {
		time = 0;

		App->mainmenu->activate();
		App->scene->disactivate();
		App->entities->disactivate();
		App->collisions->disactivate();
	}
	
	sprintf_s(str, "%d", time);
	time_text->UpdateText(str);

	sprintf_s(str, "%d", coin_score);
	coin_score_text->UpdateText(str);

	sprintf_s(str, "%d", score);
	score_text->UpdateText(str);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		settings_window->enable();
	}

	switch (hp)
	{
		case 0:
			UI_Life_Segments[0]->Disable();
			UI_Life_Segments[1]->Disable();
			UI_Life_Segments[2]->Disable();
			break;
		case 1:
			UI_Life_Segments[0]->Enable();
			UI_Life_Segments[1]->Disable();
			UI_Life_Segments[2]->Disable();
			break;
		case 2:
			UI_Life_Segments[0]->Enable();
			UI_Life_Segments[1]->Enable();
			UI_Life_Segments[2]->Disable();
			break;
		case 3:
			UI_Life_Segments[0]->Enable();
			UI_Life_Segments[1]->Enable();
			UI_Life_Segments[2]->Enable();
			break;
	}


//	LOG("SetVOl");
	App->audio->SetFxVol(fx_slider->SliderValue());
	App->audio->SetMusicVol(music_slider->SliderValue());

//	LOG("Finished HUD");
}


// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		App->fade->FadeToBlack(1);


	if(App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		App->fade->FadeToBlack(2);

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) 
	{
		if(strcmp(App->map->data.name, "Dark_Map2.tmx") == 0) App->fade->FadeToBlack(1);
		else if (strcmp(App->map->data.name, "Dark_Map.tmx") == 0) App->fade->FadeToBlack(2);
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_UP) App->map->debug = !App->map->debug;
	
	//LOG("%d", App->scene->button->buttonPressed());

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
	



	App->map->Draw();

	// Draw path of the closest entity
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	if (App->map->debug) {
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}

	HUD();

	return true;
}


// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;




	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
