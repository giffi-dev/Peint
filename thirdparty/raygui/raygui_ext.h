/*
    "raygui_ext.h/.c" is github.com/giffi-dev's additions to the raygui framework.
*/

#ifndef RAYGUI_EXT_H
#define RAYGUI_EXT_H
#include "raygui.h"

void GuiMouseClear(void);
// Only works if on top "raygui_ext" GUI functions, can be set manually
bool MouseOnGui(void); 
void SetMouseOnGui(bool state);

/*
    ImGui style menus
*/
void GuiBeginMenu(Rectangle menu);
void GuiEndMenu(void);
bool GuiMenuButton(const char* text);

void GuiBeginTopBar(int height);
void GuiEndTopBar(void);
bool GuiTopBarButton(const char* text);

void GuiComplexColorPicker(Rectangle bounds, Color* out);

#endif
