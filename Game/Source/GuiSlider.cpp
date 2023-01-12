#include "App.h"
#include "GuiControl.h"
#include "Input.h"
#include "GuiSlider.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"


GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->bounds.h = 38;
    this->bounds.w = 300 - 38;
    this->minValue = 0;
    this->maxValue = 100;
    knobTex = app->tex->Load("Assets/Textures/Slime.png");
    barTex = app->tex->Load("Assets/Textures/SliderBar.png");

    SetValue(50);
    
    boundx = bounds.x;
}

GuiSlider::~GuiSlider()
{
    delete knobTex;
}

bool GuiSlider::Update(float dt)
{
    int camX = app->render->camera.x;
    bounds.x = app->render->camera.x * (-1) + boundx;

    if (state != GuiControlState::DISABLED)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        if (((mouseX - camX) > bounds.x) && ((mouseX - camX) < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                knobX = mouseX - bounds.x;
            }
        }
        else {
            if (state == GuiControlState::PRESSED && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KeyState::KEY_UP) {
                knobX = mouseX - bounds.x;
                if (knobX < 0) knobX = 0;
                if (knobX > bounds.w) knobX = bounds.w;
                
                NotifyObserver();
            }
            else {
                state = GuiControlState::NORMAL;
            }
        }
        value = minValue + (maxValue - minValue) * (knobX / (float)bounds.w);

    }

    return false;
}


bool GuiSlider::Draw(Render* render)
{
    // Draw the right button depending on state
    switch (state)
    {

    case GuiControlState::DISABLED:
    {
        render->DrawRectangle(bounds, 0, 0, 0, 0);
    } break;

    default:
    {
        render->DrawTexture(barTex, bounds.x, bounds.y);
        render->DrawTexture(knobTex, bounds.x + knobX + 4 , bounds.y + 4);
    } break;
    

    }
    return true;
}

void GuiSlider::SetValue(int value)
{
    this->value = value;
    knobX = bounds.w * (value - minValue) / (float)(maxValue - minValue);
    
}

int GuiSlider::GetValue()
{
    return value;
}
