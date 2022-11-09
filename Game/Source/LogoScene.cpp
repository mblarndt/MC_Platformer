#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "FadeToBlack.h"

#include "LogoScene.h"

LogoScene::LogoScene(bool isEnabled) : Module(isEnabled)
{
	textureLogo = NULL;
	name.Create("LogoScene");
}

LogoScene::~LogoScene()
{}

bool LogoScene::Awake()
{
	return true;
}

bool LogoScene::Start()
{
	textureLogo = app->tex->Load("Assets/Textures/logo.png");
	return true;
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.w = 1024;
	rect.h = 480;

	app->render->DrawTexture(textureLogo, 1024/2 - rect.w/2, 480/2 - rect.h/2, &rect);
	
	if (framectr == 0) {
		app->fadeBlack->Fade(this, (Module*)app->scene, 60);
	}

	framectr--;
	return true;
}

bool LogoScene::PostUpdate()
{
	return true;
}

bool LogoScene::CleanUp() {
	textureLogo = nullptr;
	return true;
}