

struct SDL_Texture;
struct SDL_Color;
struct _TTF_Font;

class UI_Text : public UI
{
//Vars
	p2SString text;
	SDL_Texture * texture;
	SDL_Color* color;
	_TTF_Font* font;
//Methods
public:

	UI_Text(int x, int y,bool active, bool draggable,  p2SString text,SDL_Color* color, _TTF_Font* font);

	void Update();

	void Draw();

	void UpdateText(p2SString text);
};