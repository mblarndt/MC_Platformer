#include "j1Gui.h"


UI_Text::UI_Text(int x, int y, bool active, bool draggable , p2SString text ,SDL_Color* color, _TTF_Font* font) : UI(x, y,active,draggable), text(text),color(color),font(font)
{
	//font = App->font->Load(path, size);

	texture = App->font->Print(text.GetString(),*color,font);
}

void UI_Text::Update()
{

}

void UI_Text::Draw()
{
	App->render->Blit(texture, position.x, position.y,NULL,false,0.f);
}

void UI_Text::UpdateText(p2SString text) 
{
	SDL_DestroyTexture(texture);
	
	texture = App->font->Print(text.GetString(), *color, font);
}