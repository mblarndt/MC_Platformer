#ifndef __LOGO_H__
#define LOGO_H

class Logo : public Module {
	Logo();
	
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
};


#endif // !__LOGO_H__

