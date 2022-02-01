#ifndef EDITOR_H
#define EDITOR_H
#include <raylib.h>
#include "canvas.h"
#include "tools.h"

typedef struct editor_data_t {
	/* Image */
   	canvas_t canvas;

	/* Mouse */
	Vector2 mouse_startpos;
	
	/* Camera */
	Camera2D camera;
	Vector2 drag_offset;

	/* Tool */
	drawtool_t draw_tool;
} editor_data_t;

extern editor_data_t g_EditorData;

void InitEditor(void);
void MainLoopEditor(void);
void CleanupEditor(void);

#endif
