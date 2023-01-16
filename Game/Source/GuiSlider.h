#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

class GuiSlider : public GuiControl
{
public:
    GuiSlider(uint32 id, SDL_Rect bounds, int minValue, int maxValue);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw(Render* render);

    void SetValue(int value);
    int GetValue();

    int value;
    int knobX;

private:
    int minValue;
    int maxValue;
    
    SDL_Texture* knobTex;
    SDL_Texture* barTex;

    int mouseX, mouseY;
    unsigned int click;

    int boundx;
    int boundw;
};

#endif // __GUISLIDER_H__
