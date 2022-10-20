#include "j1Module.h"
#include "SDL/include/SDL_rect.h"

//#include "UI_Window.h"

class UI;
class UI_Image;
struct SDL_Texture;
class UI_Text;
class UI_Button;
class UI_Window;



class j1MainMenu : public j1Module
{
public:

	j1MainMenu();

	~j1MainMenu();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:
	
	SDL_Texture* debug_tex;


	UI_Button* play_button;
	UI_Button* continue_button;
	UI_Button* settings_button;
	UI_Button* credits_button;
	UI_Button* exit_button;

	UI_Image*  background;

	p2List<UI*> UI_mainmenulist;
	
	SDL_Rect button_rect = { 0, 0, 244, 56 };

	SDL_Rect default_rect = { 0, 0, 244, 56 };
	SDL_Rect hover_rect = { 0, 60, 244, 56 };
	SDL_Rect press_rect = { 0, 120, 244, 56 };

	UI_Text* Bootleg;
	SDL_Color bootleg_color = { (0),(0),(0),(255) };

	SDL_Texture* basic_button_texture;

	SDL_Texture* close_button_texture;

	//SettingsWindow
	UI_Window* settings_window;
	UI_Slider* fx_slider;
	UI_Slider* music_slider;

	//Credits Window
	UI_Window* credits_window;
	UI_Button* link_button;
	UI_Text* credits_text;

	SDL_Rect border_rect = {0,0,448,384};
	SDL_Texture* border_tex;

	//X button
	UI_Button* x_button;

	SDL_Rect x_rect = { 0,0,62,45 };

	SDL_Rect x_default = { 0,0,62,45 };
	SDL_Rect x_hover = { 0,45,62,45 };
	SDL_Rect x_press = { 0,90,62,45 };

	SDL_Texture* x_button_texture;

	_TTF_Font* font;

	
};
