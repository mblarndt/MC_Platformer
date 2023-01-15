#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Timer.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* buttonTex;

	GuiControlState preState;

	uint click1FxId;
	uint click2FxId;

	int boundx;

	GuiControlState previousState;

	bool isPressed;
	bool selected;

};

#endif // __GUIBUTTON_H__