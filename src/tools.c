#include <raylib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tools.h"
#include "editor.h"
#include "utils.h"

/* draws the tools preview under the cursor, aka what would be drawn */
void ToolDraw(const struct canvas_t* canvas, const drawtool_t* tool) {
    if (tool->type == TOOL_COLOR_PICKER) 
        return;
    
    Vector2 floored = {(int)tool->position.x, (int)tool->position.y};
    const Rectangle image_col = {0,0, g_EditorData.canvas.image.width, g_EditorData.canvas.image.height};
    if (CheckCollisionPointRec(floored, image_col))
        DrawRectangleV(floored, (Vector2){1, 1}, g_EditorData.draw_tool.color);  
}

/* Using a tools */
static void UsePencil(canvas_t* canvas, const drawtool_t* tool, Color col) {
    // Don't edit if it literally does nothing
    Vector2 floored = {(int)tool->position.x, (int)tool->position.y};
    Color under_cursor = GetImageColor(canvas->image, floored.x, floored.y);
    if (IsColorEqual(&under_cursor, &col))
        return;

    // Draw
    ImageDrawPixelV(&canvas->image, floored, col);
    g_EditorData.canvas.update_texture = true;
	canvas->image_saved = false;
    
    // Create Undo
    drawtool_t undo = *tool;
    undo.color = under_cursor;
    UndoPush(&canvas->undo, &undo);
}

void ToolUse(canvas_t* canvas, drawtool_t* tool) {
    if (!CheckCollisionPointRec(tool->position, CanvasGetRect(&g_EditorData.canvas)))
        return;

    switch (tool->type) {
        case TOOL_NULL:
            break;
        case TOOL_PENCIL:
            UsePencil(canvas, tool, tool->color);
            break;
        case TOOL_ERASER:
            UsePencil(canvas, tool, (Color){0,0,0,0});
            break;
        case TOOL_COLOR_PICKER:
            tool->color = GetImageColor(canvas->image, (int)tool->position.x, (int)tool->position.y);
            break;
        default:
            assert(false); // unkown drawtool
            break;
    }

}
