/*
    "raygui_ext.h/.c" is github.com/giffi-dev's additions to the raygui framework.
*/
#include <assert.h>
#include "raygui_ext.h"

/*
    Mouse
*/
static bool mouse_on_gui = false;
void GuiMouseClear() {
    mouse_on_gui = false;
}

bool MouseOnGui() {
    return mouse_on_gui;
}

void SetMouseOnGui(bool state) {
	mouse_on_gui = state;
}

/*
    Menu
*/
#define MENU_STACK 8
static Rectangle menu_stack[MENU_STACK] = {0};
static int menu_ptr = -1;

void GuiBeginMenu(Rectangle menu) {
    assert(menu_ptr < MENU_STACK); // if asserted here: "remembered to add 'GuiEndMenu'?" or exceeded stack size of 8?
    menu_stack[++menu_ptr] = menu;
}

void GuiEndMenu() {
    assert(menu_ptr-- > -1); // if asserted here: "remembered to add 'GuiBeginMenu'?"
}

bool GuiMenuButton(const char* text) {
    const int CAP_SIZE = 1; // the cap between buttons
	Rectangle rect = menu_stack[menu_ptr];
	menu_stack[menu_ptr].y += menu_stack[menu_ptr].height + CAP_SIZE;
    
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
        mouse_on_gui = true;
    }
    
    return GuiButton(rect, text);
}

/*
    Topbar
*/
#define MAX_TOPBAR_BTN_COUNT 16
static float topbar_height = -1.0f;
static float topbar_next_x = .0f;

static bool topbar_btn_actives[MAX_TOPBAR_BTN_COUNT] = {0};
static int topbar_ptr = -1;

void GuiBeginTopBar(int height) {
    topbar_height = height;
    topbar_next_x = .0f;
    topbar_ptr = -1;

    const Rectangle topbar_bg = {0, 0, GetScreenWidth(), topbar_height};
    GuiPanel(topbar_bg);
    
    if (CheckCollisionPointRec(GetMousePosition(), topbar_bg)) {
        mouse_on_gui = true;
    } 
}

void GuiEndTopBar() {
    const Rectangle topbar_bg = {0, 0, GetScreenWidth(), topbar_height};
    if (!CheckCollisionPointRec(GetMousePosition(), topbar_bg) && IsMouseButtonReleased(0)) {
        for (int i = 0; i < MAX_TOPBAR_BTN_COUNT; i++)
            topbar_btn_actives[i] = false;
    }

    topbar_height = -1.0f;
    topbar_next_x = .0f;
    topbar_ptr = -1;
}

bool GuiTopBarButton(const char* text) {
    assert(++topbar_ptr < MAX_TOPBAR_BTN_COUNT); // if asserted here: got past max_count?
    
    const int BTN_WIDTH = 64; 
    Rectangle bounds = {topbar_next_x, 0, BTN_WIDTH, topbar_height};
    topbar_next_x += bounds.width;
    topbar_btn_actives[topbar_ptr] = GuiToggle(bounds, text, topbar_btn_actives[topbar_ptr]);
    
    // Keep only one of the toggle buttons activated at one time. 
    if (topbar_btn_actives[topbar_ptr]) {
        for (int i = 0; i < MAX_TOPBAR_BTN_COUNT; i++) {
            if (i == topbar_ptr)
                continue;
            topbar_btn_actives[i] = false;
        }
    }

    return topbar_btn_actives[topbar_ptr];
}

/*
    Other
*/
void GuiComplexColorPicker(Rectangle bounds, Color* color) {
    if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
        mouse_on_gui = true;
    } 
    
    int font_size = GetFontDefault().baseSize;
    assert(font_size > 0);
    
    // Picker
    const int slider_count = 4;
    const int slider_height = 24;
    const Rectangle picker_bounds = {bounds.x, bounds.y, bounds.width - 32, bounds.height - (slider_height * slider_count)};
    *color = GuiColorPicker(picker_bounds, *color);
    
    // RGBA Sliders
    const Rectangle r_bar = {bounds.x, bounds.y + picker_bounds.height, bounds.width, slider_height};
    int r_val = (int)GuiSliderBar(r_bar, "", "", (float)color->r, 0.0f, 255.9f);
    color->r = (unsigned char)r_val;
    DrawText(TextFormat("%u", color->r), bounds.x + 8, r_bar.y + 7, font_size, BLACK);
    
    const Rectangle g_bar = {bounds.x, r_bar.y + slider_height, bounds.width, slider_height};
    int g_val = (int)GuiSliderBar(g_bar, "", "", (float)color->g, 0.0f, 255.9f);
    color->g = (unsigned char)g_val;
    DrawText(TextFormat("%u", color->g), bounds.x + 8, g_bar.y + 7, font_size, BLACK);
    
    const Rectangle b_bar = {bounds.x, g_bar.y + slider_height, bounds.width, slider_height};
    int b_val = (int)GuiSliderBar(b_bar, "", "", (float)color->b, 0.0f, 255.9f);
    color->b = (unsigned char)b_val;
    DrawText(TextFormat("%u", color->b), bounds.x + 8, b_bar.y + 7, font_size, BLACK);
    
    const Rectangle a_bar = {bounds.x, b_bar.y + slider_height, bounds.width, slider_height};
    int a_val = (int)GuiSliderBar(a_bar, "", "", (float)color->a, 0.0f, 255.9f);
    color->a = (unsigned char)a_val;
    DrawText(TextFormat("%u", color->a), bounds.x + 8, a_bar.y + 7, font_size, BLACK);
}

