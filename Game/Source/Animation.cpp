#include "App.h"
#include "Animation.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"



Animation::Animation() : Module()
{
	name.Create("animation");
}

// Destructor
Animation::~Animation()
{}


void Animation::Update()
{
	currentFrame += speed;
	if (currentFrame >= totalFrames)
	{
		currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
		++loopCount;

		if (pingpong)
			pingpongDirection = -pingpongDirection;
	}
}

const SDL_Rect& Animation::GetCurrentFrame() const
{
	int actualFrame = currentFrame;
	if (pingpongDirection == -1)
		actualFrame = totalFrames - currentFrame;

	return frames[actualFrame];
}

Animation Animation::CreateAnimation(Animation animation, bool loop, float animSpeed)
{
	for (int i = animation.startCol; i <= animation.endCol; i++) {
		animation.PushBack({ i * animation.width,  animation.row * animation.height, animation.width, animation.height });
	}
	animation.loop = loop;
	animation.speed = animSpeed;

	return animation;
}

void Animation::PushBack(const SDL_Rect& rect)
{
	frames[totalFrames++] = rect;
}

void Animation::Reset()
{
	currentFrame = 0;
}

bool Animation::HasFinished()
{
	return !loop && !pingpong && loopCount > 0;
}