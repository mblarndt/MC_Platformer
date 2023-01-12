#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Module.h"
#include "GuiSlider.h"

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

	bool MainMenuButtons();

	bool SettingsButtons();

	bool StartButtons();

	bool NoButtons();

	bool OnGuiMouseClickEvent(GuiControl* control);

	void AutomaticTransition();

public:
	GuiSlider* slider1;

private:
	SDL_Rect logoRect;
	SDL_Texture* logo;
	const char* texturePath;

	SDL_Rect settingsRect;
	SDL_Texture* settingsBox;
	const char* settingsBoxPath;

	bool settings;
	bool exit;

	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;
	GuiButton* btn5;
	GuiButton* btn6;

	GuiControlState preState1;
	GuiControlState preState2;
	GuiControlState preState3;
	GuiControlState preState4;
	GuiControlState preState5;
	GuiControlState preState6;

	const int numButtons = 6;
	GuiButton* buttons[6] = { btn1, btn2, btn3, btn4, btn5, btn6 };
	GuiControlState* preStates[6] = { &preState1, &preState2, &preState3, &preState4, &preState5, &preState6 };

	



	float duration;

	int click1FxId;
	int click2FxId;

	

};

#endif // __TITLE_SCENE_H__
