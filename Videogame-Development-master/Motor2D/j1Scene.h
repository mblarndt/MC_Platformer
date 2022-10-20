#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

class UI;
class UI_Image;
struct SDL_Texture;
class UI_Text;
class UI_Button;
class UI_Window;
class UI_Slider;

class j1Timer;
struct _TTF_Font;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	void HUD();

public:
	//HUD data
	int coin_score;
	int score;
	int time;
	int hp;

private:

	int channel;

	j1Timer*	InGameTime;

	UI_Text* coin_score_text;
	UI_Text* score_text;
	UI_Text* time_text;
	UI_Text* lives_text;

	SDL_Texture* debug_tex;

	//Default buttons
	UI_Button* button;

	SDL_Rect button_rect = { 0, 0, 244, 56 };

	SDL_Rect default_rect = { 0, 0, 244, 56 };
	SDL_Rect hover_rect = { 0, 60, 244, 56 };
	SDL_Rect press_rect = { 0, 120, 244, 56 };

	UI_Text* Bootleg;
	SDL_Color bootleg_color = {(220),(220),(255),(255)};

	//X button
	UI_Button* x_button;

	SDL_Rect x_rect = { 0,0,62,45 };

	SDL_Rect x_default = { 0,0,62,45 };
	SDL_Rect x_hover = { 0,45,62,45 };
	SDL_Rect x_press = { 0,90,62,45 };

	//Settings
	SDL_Rect border_rect = { 0,0,448,384 };
	SDL_Texture* border_tex;

	UI_Window* settings_window;
	UI_Slider* fx_slider;
	UI_Slider* music_slider;

	//HUD
	SDL_Texture* UI_HUD_Left_Tex;
	SDL_Texture* UI_HUD_Right_Tex;

	UI_Image* UI_HUD_Left;
	UI_Image* UI_HUD_Right;

	UI_Image* UI_Life_Segments[3];

	p2List<UI*>* HUD_elements;

	_TTF_Font* font;
};

#endif //__j1SCENE_H__