#ifndef CANVAS_H
#define CANVAS_H
#include <raylib.h>
#include "undo.h"

typedef struct canvas_t {
    Image image;
    Texture texture;
    bool update_texture;
	bool image_saved; // has the image been saved with the current modifications
    char filepath[0xFF];
	
    undostack_t undo;
} canvas_t;

// popups
bool PopupCanvasSaveAs(canvas_t* canvas);
bool PopupCanvasSave(canvas_t* canvas);
bool PopupCanvasLoad(canvas_t* out);

bool CanvasLoadFile(canvas_t* out, const char* path);
canvas_t CanvasNew(int w, int h, Color bg);
void CanvasUndo(canvas_t *canvas);
void UnloadCanvas(canvas_t* canvas);

// getters
Rectangle CanvasGetRect(const canvas_t* canvas);

// update
void CanvasUpdate(canvas_t* canvas);
void CanvasDraw(const canvas_t* canvas);

#endif
