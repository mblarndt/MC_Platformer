#ifndef __GUI_H__
#define __GUI_H__

#include "Module.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "GuiSlider.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Gui : public Module
{
public:

	Gui();

	// Destructor
	virtual ~Gui();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool MainMenuButtons();

	bool SettingsButtons();

	bool StartButtons();

	bool NoButtons();

	bool InGameMenu();

	bool ButtonInit();

public:

	GuiSlider* slider1;
	GuiSlider* slider2;

	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager

	GuiButton* startBtn;
	GuiButton* settingsBtn;
	GuiButton* creditsBtn;
	GuiButton* exitBtn;
	GuiButton* continueBtn;
	GuiButton* lvl1Btn;
	GuiButton* lvl2Btn;
	GuiButton* backBtn;
	GuiButton* saveBtn;
	GuiButton* loadBtn;
	GuiButton* resumeBtn;
	GuiButton* titleBtn;

	bool mainMenu;
	bool start;
	bool settings;
	bool exit;
	bool nothing;

	SDL_Texture* settingsBox;

	bool toggle;

private:
	

};

#endif // __GUI_H__