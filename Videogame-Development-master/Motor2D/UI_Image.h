class UI_Image : public UI
{
public:
	//Vars
	SDL_Rect rect;

	SDL_Texture* image;

	//Methods
public:

	UI_Image(int x, int y, bool active, bool draggable, SDL_Rect rect,SDL_Texture* image = nullptr);

	void Update();

	void Draw();

	void UpdatePosition(int x, int y);
};