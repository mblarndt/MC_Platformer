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

public:

	GuiSlider* slider1;
	GuiSlider* slider2;

	// L15: TODO 2: Declare a GUI Button and create it using the GuiManager
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;
	GuiButton* btn5;
	GuiButton* btn6;


	const int numButtons = 6;
	GuiButton* buttons[6] = { btn1, btn2, btn3, btn4, btn5, btn6 };

	bool mainMenu;
	bool start;
	bool settings;
	bool exit;
	bool nothing;

private:
	

};

#endif // __GUI_H__