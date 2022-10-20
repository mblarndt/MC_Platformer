#include "j1App.h"
#include "j1Gui.h"

UI_Image::UI_Image(int x, int y, bool active, bool draggable, SDL_Rect rect,SDL_Texture* image) : UI(x,y,active,draggable) , rect(rect), image(image)
{

}

void UI_Image::Update()
{

}

void UI_Image::Draw()
{
	App->render->Blit(image, position.x, position.y, &rect,false, 0.f);
}

void UI_Image::UpdatePosition(int x, int y) {
	if (!draggable) return;
	position.x = x;
	position.y = y;
};