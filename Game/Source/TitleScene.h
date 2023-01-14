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


	void AutomaticTransition();

public:
	GuiSlider* slider1;
	GuiSlider* slider2;

	GuiButton* button1;

	bool toggle;


private:
	SDL_Rect logoRect;
	SDL_Texture* logo;
	const char* texturePath;

	SDL_Rect settingsRect;
	SDL_Texture* settingsBox;
	const char* settingsBoxPath;

	bool settings;
	bool exit;

	
	float duration;

};

#endif // __TITLE_SCENE_H__
