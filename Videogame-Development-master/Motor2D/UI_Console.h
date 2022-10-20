class UI_Console : public UI
{
public:
	SDL_Rect base_rect;
	SDL_Rect input_rect;

	p2List<UI_Text*> logs;

	//Input Text
	p2SString text = "";
	SDL_Texture * texture;
	SDL_Color* color;
	_TTF_Font* font;

	int bruh;

public:

	UI_Console(int x, int y, bool enabled, bool draggable, SDL_Color* color, _TTF_Font* font, SDL_Rect rect);

	void Update();

	void Draw();

	void UpdateText();

	const char* GetText() const;

	void ConcatNewText(char* newText);

	void Execute(char* call);

	void AddText(char* string);
};