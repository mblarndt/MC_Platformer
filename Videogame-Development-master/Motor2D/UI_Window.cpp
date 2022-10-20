#include "j1Gui.h"
//#include "UI_Window.h"
//#include "UI_Button.h"

UI_Window::UI_Window(int x, int y, bool enabled, UI_Button* x_button,SDL_Texture* border_texture) : position(x,y) , enabled(enabled),x_button(x_button),border_texture(border_texture)
{
	addUI(x_button, 0,0);
	border = App->gui->CreateImage(0, 100, false, false, border_rect, border_texture);

	border->position = position;
	x_button->position.x = position.x + border->rect.w - x_button->rect.w - 8;
	x_button->position.y = position.y + 6;
}

void UI_Window::Update()
{
	border->position = position;

	x_button->position.x = position.x + border->rect.w - x_button->rect.w - 8;
	x_button->position.y = position.y + 6;

	p2List_item<UI*>* elementsIt = elements.start;

	while (elementsIt != NULL)
	{
		elementsIt->data->Update();

		//elementsIt->data->position = elementsIt->data->startingPosition;
		//elementsIt->data->position += position;

		elementsIt = elementsIt->next;
	}

	if (x_button->buttonPressed())
	{
		disable();
	}
}

void UI_Window::Draw()
{
	border->Draw();

	p2List_item<UI*>* elementsIt = elements.start;

	while (elementsIt != NULL)
	{
		elementsIt->data->Draw();

		elementsIt = elementsIt->next;
	}
}

void UI_Window::addUI(UI* ui,int x, int y)
{
	elements.add(ui);

	ui->position.x = position.x + x;
	ui->position.y = position.y + y;
}

void UI_Window::enable()
{
	//p2List_item<UI*>* elementsIt = elements.start;

	//while (elementsIt != NULL)
	//{
	//	elementsIt->data->Enable();
	//}
	//border->Enable();

	enabled = true;
}

void UI_Window::disable()
{
	/*p2List_item<UI*>* elementsIt = elements.start;

	while (elementsIt != NULL)
	{
		elementsIt->data->Disable();
	}
	border->Disable();*/

	enabled = false;
}