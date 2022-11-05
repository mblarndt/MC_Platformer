#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "LOGO.h"

Logo::Logo(bool isEnabled) : Module(isEnabled)
{
	textureLogo = NULL;
	name.Create("Logo screen");
}

Logo::~Logo()
{}

bool Logo::Awake()
{
	return true;
}

bool Logo::Start()
{
	textureLogo = app->tex->Load("Assets/Textures/test.png");
	return true;
}

bool Logo::PreUpdate()
{
	return true;
}

bool Logo::Update(float dt)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 538;
	rect.h = 381;
	app->render->DrawTexture(textureLogo, 0, 0, &rect);
	
	if (framectr == 0) {
		app->fadeBlack->Fade(this, (Module*)app->scene, 120);
	}

	framectr--;
	return true;
}

bool Logo::PostUpdate()
{
	return true;
}

bool Logo::CleanUp() {
	textureLogo = nullptr;
	return true;
}