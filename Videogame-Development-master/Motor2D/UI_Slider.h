class UI_Slider : public UI
{
public:
	//Vars
	SDL_Rect clickable_rect;
	
	SDL_Rect bar;
	SDL_Rect thumb;
	SDL_Rect thumb_hovered;

	SDL_Texture* texture;

	//UI_Image* bar;
	//UI_Image* thumb;
	//UI_Image* thumb_hovered;

	bool hovered;
	
	int thumb_offset;
	
	int sliderpos;

	//Methods
public:

	UI_Slider(int x, int y, bool active, bool draggable, int sliderposition = 76);

	void Update();
	
	void Draw();

	float SliderValue();
};