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
    this->bounds.w = bounds.w - 38;
    this->minValue = minValue;
    this->maxValue = maxValue;
    knobTex = app->tex->Load("Assets/Textures/Slime.png");
    barTex = app->tex->Load("Assets/Textures/SliderBar.png");

    SetValue(50);
    
    
}

GuiSlider::~GuiSlider()
{
    delete knobTex;
}

bool GuiSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
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
            if (state == GuiControlState::PRESSED) {
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
        //LOG("Slider Value: %i", value);

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
        render->DrawTexture(knobTex, knobX + 104, bounds.y + 4);
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
