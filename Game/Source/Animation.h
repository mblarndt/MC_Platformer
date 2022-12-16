#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "Module.h"

#define MAX_FRAMES 90

class Animation : public Module
{
public:
	Animation();

	virtual ~Animation();

	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;
	int startCol;
	int endCol;
	int row;
	int width;
	int height;

private:
	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:
	void PushBack(const SDL_Rect& rect);

	void Reset();

	bool HasFinished();

	void Update();

	const SDL_Rect& GetCurrentFrame() const;

	Animation CreateAnimation(Animation animation, bool loop, float animSpeed);

};

#endif