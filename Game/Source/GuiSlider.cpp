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
    value = 50;
    knobTex = app->tex->Load("Assets/Textures/Slime.png");
    barTex = app->tex->Load("Assets/Textures/SliderBar.png");
    
    
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
                value = minValue + (maxValue - minValue) * (knobX / (float)bounds.w);
                NotifyObserver();
                LOG("Slider Value: %i", value);
            }
            else {
                state = GuiControlState::NORMAL;
            }
        }

    }

    return false;
}


bool GuiSlider::Draw(Render* render)
{
    render->DrawTexture(barTex, bounds.x, bounds.y);
    render->DrawTexture(knobTex, knobX + 104, bounds.y + 4);

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
