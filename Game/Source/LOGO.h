#ifndef __LOGO_H__
#define __LOGO_H__

#include "Module.h"

class Logo : public Module
{
public:
	Logo(bool isEnabled);
	
	virtual ~Logo();
	
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
	SDL_Texture* textureLogo;
	
	int framectr = 120;
};


#endif // __LOGO_H__

