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
    this->minValue = minValue;
    this->maxValue = maxValue;
    knobTex = app->tex->Load("Assets/Textures/Slime.png");
    barTex = app->tex->Load("Assets/Textures/SliderBar.png");

    SetValue(maxValue / 2);
    
    boundx = bounds.x;
    boundw = bounds.w - 38;
}

GuiSlider::~GuiSlider()
{
    delete knobTex;
}

bool GuiSlider::Update(float dt)
{
    int camX = app->render->camera.x;
    bounds.x = boundx - camX;

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
                knobX = mouseX - bounds.x - camX;
                if (knobX < 0) knobX = 0;
                if (knobX > bounds.w) knobX = bounds.w;
                NotifyObserver();
            }
        }
        else {
            if (state == GuiControlState::PRESSED && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KeyState::KEY_UP) {
                knobX = mouseX - bounds.x - camX;
                
                if (knobX < 0) knobX = 0;
                if (knobX > bounds.w) knobX = bounds.w;
                
                
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
        //knobX = bounds.w * (value - minValue) / (float)(maxValue - minValue);

        SDL_Rect BarBlack = bounds;
        render->DrawRectangle(BarBlack, 0, 0, 0);

        SDL_Rect BarGrey = {bounds.x + 4, bounds.y + 4, bounds.w - 8, bounds.h - 8};
        render->DrawRectangle(BarGrey, 255, 255, 255, 150);
        //render->DrawTexture(barTex, bounds.x, bounds.y);
        render->DrawTexture(knobTex, bounds.x + ((boundw) * (value - minValue) / (float)(maxValue - minValue)) + 4, bounds.y + 4);

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
