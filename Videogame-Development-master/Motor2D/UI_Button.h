
//#include "j1Gui.h"


class UI_Text;
class UI_Image;

enum class Button_State
{
	DEFAULT,
	HOVER,
	PRESSED
};

class UI_Button : public UI
{
	//Vars
public:
	SDL_Rect rect;

	Button_State state;

	bool pressed;

	int X_offset;
	int Y_offset;

	//Text
	p2SString text;
	SDL_Texture * text_texture;
	SDL_Color* color;
	_TTF_Font* font;

	//Image
	SDL_Texture* image_texture;
	SDL_Rect* default_rect;
	SDL_Rect* hover_rect;
	SDL_Rect* press_rect;
	iPoint textPos;

	//Methods
public:

	UI_Button(int x, int y, bool enable, bool draggable, p2SString text, SDL_Color* color, _TTF_Font* font, SDL_Rect* default_rect, SDL_Rect* hover_rect, SDL_Rect* press_rect, SDL_Texture* image ,SDL_Rect rect,int x_offset,int y_offset);

	void Update();

	void Draw();

	bool buttonPressed();


};
