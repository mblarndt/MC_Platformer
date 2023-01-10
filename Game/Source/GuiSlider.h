#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"
#include "Point.h"
#include "SString.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, bool draggable, int sliderposition);
	virtual ~GuiSlider();

	//Methods
	bool Update(float dt);

	bool Draw(Render* render);

	float SliderValue();


	//Vars
	SDL_Rect clickable_rect;
	
	SDL_Rect bar;
	SDL_Rect thumb;
	SDL_Rect thumb_hovered;

	SDL_Texture* texture;

	bool hovered;
	
	int thumb_offset;
	
	int sliderpos;

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUISLIDER_H__