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
#include "GuiToggle.h"

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

	bool Credits();

	bool ButtonInit();

	bool CreateButton(int id, const char* label, int x, int y, GuiButton*& button);

	bool CreateToggle(int id, const char* label, int x, int y, GuiToggle*& toggle);

	bool SettingsWindow();

	bool SetSceneState(const char* scene);

	bool ToggleGodmode();


public:
	int numButtons;

	GuiSlider* musicSlider;
	GuiSlider* fxSlider;
	GuiSlider* fpsSlider;

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
	GuiButton* audioBtn;
	GuiButton* videoBtn;
	GuiButton* gameBtn;
	GuiButton* fifteenBtn;
	GuiButton* thirtyBtn;
	GuiButton* sixtyBtn;

	GuiToggle* vsyncToggle;
	GuiToggle* fullscreenToggle;
	GuiToggle* debugToggle;
	GuiToggle* godmodeToggle;
	GuiToggle* audioToggle;

	
	bool mainMenu = true;
	bool start = false;
	bool settings = false;
	bool exit = false;
	bool nothing;

	SDL_Texture* settingsBox;

	bool toggle;

	bool credits = false;

	enum class SettingsState {
		AUDIO,
		VIDEO,
		GAME
	};

	enum class MainState {
		TITLE,
		IN_GAME
	};

	MainState mainState;
	SettingsState settingsState;

	int musicVolume;
	int fxVolume;
	int preMusicVolume;
	int preFxVolume;

	const char* currentFps;

	bool godmode;

private:
	

};

#endif // __GUI_H__