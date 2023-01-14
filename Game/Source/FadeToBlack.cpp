#include <math.h>

#include "App.h"
#include "Module.h"
#include "FadeToBlack.h"
#include "Window.h"
#include "Log.h"
#include "Render.h"
#include "EntityManager.h"
#include "Map.h"
#include "Scene.h"
#include "LogoScene.h"
#include "TitleScene.h"
#include "External/SDL/include/SDL_render.h"
#include "External/SDL/include/SDL_timer.h"
#include "Physics.h"
#include "Gui.h"

FadeToBlack::FadeToBlack()
{
	name.Create("fade");

}

FadeToBlack::~FadeToBlack()
{}

bool FadeToBlack::Awake(pugi::xml_node& conf)
{
	uint width, height;
	app->win->GetWindowSize(width, height);
	screen.w = width;
	screen.h = height;
	screen.x = screen.y = 0;

	return true;
}

// Load assets
bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	fadingPlayer = false;
	sceneSwitch = false;
	wantToSwitchScene = "LogoScene";

	return true;
}

// Update: draw background
bool FadeToBlack::Update(float dt)
{

	if (currentStep == FadeStep::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - startTime;
	float normalized = MIN(1.0f, (float)now / (float)totalTime);

	switch (currentStep)
	{
	case FadeStep::FADE_TO:
	{
		if (now >= totalTime)
		{
			if (!sceneSwitch)
			{
			}
			else
			{
				SwitchScenes(wantToSwitchScene);
			}
			totalTime += totalTime;
			startTime = SDL_GetTicks();
			currentStep = FadeStep::FADE_FROM;
		}
	} break;

	case FadeStep::FADE_FROM:
	{
		normalized = 1.0f - normalized;

		if (now >= totalTime)
			currentStep = FadeStep::NONE;

		fadingPlayer = false;
		sceneSwitch = false;
	} break;
	}

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool FadeToBlack::DoFadeToBlack(int level, float time)
{
	bool ret = false;

	nextLevel = level;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;

	}

	return ret;
}





bool FadeToBlack::FadeToBlackScene(char* scene, float time)
{
	bool ret = false;

	sceneSwitch = true;
	wantToSwitchScene = scene;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	if (scene == "TitleScene") {
		//CleanUp
		app->map->CleanUp();
		app->scene->CleanUp();
		app->entityManager->CleanUp();
	}

	app->gui->toggle = true;

	return ret;
}

bool FadeToBlack::SwitchMap(int level)
{

	bool ret = true;
	LOG("Switching Maps...");
	
	app->map->CleanUp();
	app->scene->CleanUp();
	app->entityManager->CleanUp();
	
	b2Body* body = app->physics->world->GetBodyList();
	while (body != NULL)
	{
		app->physics->world->DestroyBody(app->physics->world->GetBodyList());
		body = body->GetNext();
	}

	app->scene->SceneStart(level);		//Load specified map

	return ret;
}

bool FadeToBlack::SwitchScenes(char* scene)
{
	if (activeScene != scene)
	{

		if (scene == "LogoScene")
		{
			app->scene->active = false;
			app->map->active = false;
			app->physics->active = false;
			app->logoScene->active = true;
			app->titleScene->active = false;
		}
		if (scene == "TitleScene")
		{
			app->scene->active = false;
			app->logoScene->active = false;
			app->titleScene->active = true;
			app->map->active = false;
			app->physics->active = false;
			app->input->active = true;
			//app->scene->CleanUp();
		}
		if (scene == "Scene")
		{
			app->scene->active = true;
			app->logoScene->active = false;
			app->map->active = true;
			app->titleScene->active = false;
			app->physics->active = true;
			app->input->active = true;
		}
		activeScene = scene;
	}

	return true;
}

