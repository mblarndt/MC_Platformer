
#include "j1Gui.h"


UI::UI(int x, float y, bool enabled, bool draggable) : position(x,y), enabled(enabled), draggable(draggable)
{
	debug = false;

	startingPosition = position;
}

void UI::Update()
{

}

void UI::Draw()
{

}

bool UI::IsEnabled()
{
	return enabled;
}

void UI::Enable()
{
	enabled = true;
}

void UI::Disable()
{
	enabled = false;
}