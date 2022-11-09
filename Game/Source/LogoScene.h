#ifndef __LOGOSCENE_H__
#define __LOGOSCENE_H__

#include "Module.h"

class LogoScene : public Module
{
public:
	LogoScene(bool isEnabled);
	
	virtual ~LogoScene();
	
	bool Awake();

	bool Start();

	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Rect rect;
	SDL_Texture* textureLogo;
	
	int framectr = 120;

	float accumulatedTime = 0.0f;
};


#endif // __LOGOSCENE_H__

