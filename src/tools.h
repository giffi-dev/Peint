#ifndef TOOLS_H
#define TOOLS_H

#include <raylib.h>

typedef enum drawtool_type {
    TOOL_NULL,
	TOOL_PENCIL,
	TOOL_ERASER,
    TOOL_COLOR_PICKER,
} drawtool_type;

typedef struct drawtool_t {
    drawtool_type type;
    Color color;
    int brush_size;
    Vector2 position;
} drawtool_t;

typedef struct canvas_t canvas_t;

void ToolDraw(const canvas_t* canvas, const drawtool_t* tool);
void ToolUse(canvas_t* canvas, drawtool_t* tool);

#endif
