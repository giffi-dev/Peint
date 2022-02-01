#include <tinyfiledialogs.h>
#include <raygui.h>
#include <raygui_ext.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "style/cyper.h"
#include "editor.h"
#include "canvas.h"
#include "utils.h"

editor_data_t g_EditorData = {0};

static void EditorCamera(void) {
    assert(g_EditorData.canvas.image.width != 0);
    assert(g_EditorData.canvas.image.height != 0);
	
	g_EditorData.camera.offset.x = (GetScreenWidth() / 2.0f) - ((g_EditorData.canvas.image.width / 2.0f) * g_EditorData.camera.zoom) + g_EditorData.drag_offset.x;
	g_EditorData.camera.offset.y = (GetScreenHeight() / 2.0f) - ((g_EditorData.canvas.image.height / 2.0f) * g_EditorData.camera.zoom) + g_EditorData.drag_offset.y;
}

static void CenterCamera(void) {
    g_EditorData.drag_offset.x = .0f;
    g_EditorData.drag_offset.y = .0f;
    EditorCamera();
}

static void HandleWindowEvents(void) {
    GuiMouseClear();
    
    // Check windows changes
    if (IsWindowResized()) {
        CenterCamera();
    }
    
    // File droppin'
    if (IsFileDropped()) {
        int count = 0;
        char** files = GetDroppedFiles(&count);
        if (count > 0) {
            if (!strcmp(GetFileExtension(files[0]), ".png")) {
                CanvasLoadFile(&g_EditorData.canvas, files[0]);
            }    
        }
        
        ClearDroppedFiles();
    }

    // Zoomin'
    if (GetMouseWheelMove() != 0.0f) {
        const float zmult = 0.12f * g_EditorData.camera.zoom;
        g_EditorData.camera.zoom += GetMouseWheelMove() * zmult;
        if (g_EditorData.camera.zoom < 0.1f)
            g_EditorData.camera.zoom = 0.1f;
        if (g_EditorData.camera.zoom > 256.0f)
            g_EditorData.camera.zoom = 256.0f;
        EditorCamera();
    }
    
    // Draggin'
    if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
            g_EditorData.mouse_startpos = GetMousePosition();
            g_EditorData.mouse_startpos.x -= g_EditorData.drag_offset.x;
            g_EditorData.mouse_startpos.y -= g_EditorData.drag_offset.y;
        }
        EditorCamera();
        g_EditorData.drag_offset.x = -(g_EditorData.mouse_startpos.x - GetMouseX());
        g_EditorData.drag_offset.y = -(g_EditorData.mouse_startpos.y - GetMouseY());
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {
        g_EditorData.mouse_startpos = (Vector2){.0f, .0f};
    }
}

static void DrawBg() {
    ClearBackground(GRAY);
    
    // Grid background
    const Color c = DARKGRAY;
    const int passes = GetScreenWidth() / 32 + 1;
    const Vector2 square_size = {32.f, 32.f};
    
    bool flip = false;
    for (int x = -passes; x < passes; x++) {
        for (int y = -passes; y < passes+1; y++) {
            flip = !flip;
            if (flip)
                DrawRectangleV((Vector2){x*square_size.x, y*square_size.y}, square_size, c);
        }
    }
}

static void DrawGui(void) {
    // Topbar
    GuiBeginTopBar(24);
    if (GuiTopBarButton("File")) {
        GuiBeginMenu((Rectangle){ 0, 24, 64, 24 });
        
        if (GuiMenuButton("New")) {
            UnloadCanvas(&g_EditorData.canvas);
            g_EditorData.canvas = CanvasNew(32, 32, (Color){0,0,0,0});
        }

        if (GuiMenuButton("Open..")) {
            PopupCanvasLoad(&g_EditorData.canvas);
        }
        
        if (GuiMenuButton("Save")) {
            PopupCanvasSave(&g_EditorData.canvas);
        }
        
        if (GuiMenuButton("Save As..")) {
            PopupCanvasSaveAs(&g_EditorData.canvas);
        }
        
        if (GuiMenuButton("Exit")) {
            exit(0);
        }
        GuiEndMenu();
    }
    if (GuiTopBarButton("Edit")) {
        GuiBeginMenu((Rectangle){ 64, 24, 64, 24 });
        if (GuiMenuButton("Undo")) {
            CanvasUndo(&g_EditorData.canvas);
        }
        GuiEndMenu();
    }
    if (GuiTopBarButton("View")) {
        GuiBeginMenu((Rectangle){ 128, 24, 64, 24 });
        GuiMenuButton("Nothing");
        GuiEndMenu();
    }
    if (GuiTopBarButton("About")) {
		// FIXME: all these position values are hardcoded (and eye balled), should use maths instead..
		
		// bg
		Rectangle bounds = {0, 0, 512, 216};
		bounds.x = (GetScreenWidth() - bounds.width) / 2;
		bounds.y = (GetScreenHeight() - bounds.height) / 2;
		GuiPanel(bounds);
		if (CheckCollisionPointRec(GetMousePosition(), bounds))
			SetMouseOnGui(true);
		
		const Color font_color = WHITE;
		const int   font_size  = GetFontDefault().baseSize * 2;
		const int px = bounds.x + 12;
		
		// title
		DrawText(TextFormat("Peint (%s)", PEINT_VERSION), px, bounds.y+8, font_size, font_color);
		DrawText("About:", bounds.x + 223, bounds.y + 8, font_size, font_color);
		
		// desc
		int dy = bounds.y + 40;
		DrawText("Peint is a very simplistic image editor written", px, dy, font_size, font_color); dy += font_size;
		DrawText("in c99 and built with portablility in mind.", px, dy, font_size, font_color); dy += font_size;
		
		// libs
		const int py = bounds.y + (bounds.height / 2);
		DrawText(           "<---libs--->",      bounds.x + 200, py,               font_size, font_color);
		DrawText(TextFormat("opengl v: %s", OPENGL_VERSION),  px, py + font_size,   font_size, font_color);
		DrawText(TextFormat("raygui v: %s", RAYLIB_VERSION),  px, py + font_size*2, font_size, font_color);
		DrawText(TextFormat("raylib v: %s", RAYLIB_VERSION),  px, py + font_size*3, font_size, font_color);
		DrawText(TextFormat("tinyfiledialogs v: %s", &tinyfd_version),  px, py + font_size*4, font_size, font_color);
		
    }
    GuiEndTopBar();
    
    
    // Side Tools
    static Rectangle side_tools = { 0.0f, 0.0f, 32.0f, 32.0f};
    side_tools.y = (GetScreenHeight() / 2.0f) - (1.5*32); // 6btns / 2 = 2
    GuiBeginMenu(side_tools);
        
        // FIXME: Somewhat of a dirty way to get these icons highlighted.
        if (g_EditorData.draw_tool.type == TOOL_PENCIL) {
            GuiSetState(GUI_STATE_PRESSED);
        }
        if (GuiMenuButton("#23#")) {
            g_EditorData.draw_tool.type = TOOL_PENCIL;
        }
        GuiSetState(GUI_STATE_NORMAL);
        
        if (g_EditorData.draw_tool.type == TOOL_ERASER) {
            GuiSetState(GUI_STATE_PRESSED);
        }
        if (GuiMenuButton("#28#")) {
            g_EditorData.draw_tool.type = TOOL_ERASER;
        }
        GuiSetState(GUI_STATE_NORMAL);

        if (g_EditorData.draw_tool.type == TOOL_COLOR_PICKER) {
            GuiSetState(GUI_STATE_PRESSED);
        }
        if (GuiMenuButton("#27#")) {
            g_EditorData.draw_tool.type = TOOL_COLOR_PICKER;
        }
        GuiSetState(GUI_STATE_NORMAL);
    GuiEndMenu();
    
    // Color picker
    Rectangle color_panel;
    color_panel.width = 192;
    color_panel.height = 256;
    color_panel.x = (GetScreenWidth() - color_panel.width) - 4;
    color_panel.y = (GetScreenHeight() - color_panel.height) / 2;
    GuiComplexColorPicker(color_panel, &g_EditorData.draw_tool.color);
    
    // Mouse Dropdown
    static Vector2 mouse_location = {-1.f, -1.f};
    if (mouse_location.x > 0.0f) {
        GuiBeginMenu((Rectangle){ mouse_location.x, mouse_location.y, 124, 24 });
        if (GuiMenuButton("center camera")) {
            CenterCamera();
        }
        if (GuiMenuButton("reset zoom")) {
            g_EditorData.camera.zoom = 1.0f;
        }
        GuiEndMenu();
    }
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && mouse_location.x == -1.f) {
        mouse_location = GetMousePosition();
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        mouse_location = (Vector2){-1.f, -1.f};
    }
}

void InitEditor(void) {
    // Init Raylib
    const int screenWidth = 1280;
    const int screenHeight = 720;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Peint");
    SetTargetFPS(60);
    GuiLoadStyleCyber();
    
    // Init Camera
    g_EditorData.camera.zoom = 1.0f;
    g_EditorData.camera.offset.x = (GetScreenWidth() / 2.0f);
    g_EditorData.camera.offset.y = (GetScreenHeight() / 2.0f);
	g_EditorData.drag_offset = (Vector2){.0f, .0f};

    // Canvas
    g_EditorData.canvas = CanvasNew(32, 32, (Color){.0f,.0f,.0f,.0f});
	
	// Mouse
	g_EditorData.mouse_startpos = (Vector2){.0f, .0f};
    
    // Color
    g_EditorData.draw_tool.type = TOOL_PENCIL;
    g_EditorData.draw_tool.color = BLACK;
    g_EditorData.draw_tool.position = (Vector2){.0f, .0f};
}

void MainLoopEditor(void) {
    while (!WindowShouldClose()) {
        g_EditorData.draw_tool.position = GetScreenToWorld2D(GetMousePosition(), g_EditorData.camera);

        HandleWindowEvents();
        CanvasUpdate(&g_EditorData.canvas);

        BeginDrawing();
            DrawBg();
            
            BeginMode2D(g_EditorData.camera);
                CanvasDraw(&g_EditorData.canvas);
                ToolDraw(&g_EditorData.canvas, &g_EditorData.draw_tool);  
            EndMode2D();
            DrawGui();

            // Editing image
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !MouseOnGui())
                ToolUse(&g_EditorData.canvas, &g_EditorData.draw_tool);
            
        EndDrawing();
    }
}

void CleanupEditor(void) {
    CloseWindow();
}
