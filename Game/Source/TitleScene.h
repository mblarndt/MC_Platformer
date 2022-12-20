#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Module.h"

struct SDL_Texture;

class TitleScene : public Module
{
public:

	TitleScene();

	// Destructor
	virtual ~TitleScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

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

	void AutomaticTransition();

public:

private:
	SDL_Rect logoRect;
	SDL_Texture* logo;
	const char* texturePath;

	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;

	GuiControlState preState1;
	GuiControlState preState2;
	GuiControlState preState3;

	const int numButtons = 3;
	GuiButton* buttons[3] = { btn1, btn2, btn3 };
	GuiControlState* preStates[3] = { &preState1, &preState2, &preState3 };



	float duration;

	int click1FxId;
	int click2FxId;

	

};

#endif // __TITLE_SCENE_H__
