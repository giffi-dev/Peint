#include <string.h>
#include <tinyfiledialogs.h>

#include "canvas.h"
#include "editor.h"
#include "utils.h"

/*
    SAVING AND LOADING
*/

bool PopupCanvasSaveAs(canvas_t* canvas) {
    static const char* endings[] = {"*.png"};
    const char* path = tinyfd_saveFileDialog("Save As..", "./", 1, endings, (void*)0);
    if (path == (void*)0)
        return false;
	
    memcpy((void*)&canvas->filepath, path, sizeof(canvas->filepath));
	canvas->image_saved = ExportImage(canvas->image, path);
    return canvas->image_saved;
}

bool PopupCanvasSave(canvas_t* canvas) {
	if (canvas->image_saved)
		return false;
	
    // if filepath not known, then can't save without asking a path.
    if (!strlen((void*)&canvas->filepath))
        return PopupCanvasSaveAs(canvas); 
	canvas->image_saved = ExportImage(canvas->image, (void*)&canvas->filepath);
    return canvas->image_saved;
}

bool PopupCanvasLoad(canvas_t* out) {
    static const char* endings[] = { "*.png" };
    const char* path = tinyfd_openFileDialog("Open an Image", "./", 1, (void*)&endings, "desc", false);
    if (path == (void*)0)
        return false;

    return CanvasLoadFile(out, path);
}

bool CanvasLoadFile(canvas_t* out, const char* path) {
    memset(out, 0, sizeof(canvas_t));
    memcpy(&out->filepath, path, sizeof(out->filepath));
	out->undo.record = true;
    out->image = LoadImage(path);
    out->update_texture = true;
	out->image_saved = true;
    return out->image.data != (void*)0;
}

canvas_t CanvasNew(int w, int h, Color bg) {
    canvas_t out;
    memset(&out, 0, sizeof(canvas_t));
    out.undo.record = true;
	out.image = GenImageColor(w, h, bg);
    out.update_texture = true;
	out.image_saved = true;
    return out;
}

void CanvasUndo(canvas_t *canvas) {
    drawtool_t undo = UndoPop(&canvas->undo);
	canvas->undo.record = false;
    ToolUse(canvas, &undo);
	canvas->undo.record = true;
}

void UnloadCanvas(canvas_t* canvas) {
    UnloadTexture(canvas->texture);
    UnloadImage(canvas->image);
}

/*
    Getters
*/
Rectangle CanvasGetRect(const canvas_t* canvas) {
    return (Rectangle){0,0,canvas->image.width, canvas->image.height};
}

/*
    Draw/Update
*/

void CanvasUpdate(canvas_t* canvas) {
	// window title
	if (canvas->image_saved)
		SetWindowTitle(TextFormat("%s %s", PEINT_NAME, (char*)canvas->filepath));
	else
		SetWindowTitle(TextFormat("%s %s (*)", PEINT_NAME, (char*)canvas->filepath));
	
	// keybinds
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z)) {
		CanvasUndo(canvas);
	}
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			PopupCanvasSaveAs(canvas);
		}
		else {
			PopupCanvasSave(canvas);
		}
	}
	
	
	// update
	if (canvas->update_texture) {
        canvas->update_texture = false;
        UnloadTexture(canvas->texture);
        canvas->texture = LoadTextureFromImage(canvas->image);
    }
}

void CanvasDraw(const canvas_t* canvas) {
    // texture
    DrawTexture(canvas->texture, 0, 0, WHITE);
    // texture borders
    const int BR_THIC = 1;
    Rectangle rect = {-BR_THIC, -BR_THIC, canvas->texture.width + (BR_THIC * 2), canvas->texture.height + (BR_THIC * 2)};
    DrawRectangleLinesEx(rect, BR_THIC, BLACK);

    // Pixel Grid
    {
        if (g_EditorData.camera.zoom < 18.0f)
            return;
        
        const Color c = {255,255,255,196};
        const float LN_THIC = 0.08f;

        // Horizontal Lines
        for (int y = 1; y < canvas->image.height; y++) {
            DrawLineEx((Vector2){0, y}, (Vector2){canvas->image.width, y}, LN_THIC, c);
        }
        // Vertical Lines
        for (int x = 1; x < canvas->image.width; x++) {
            DrawLineEx((Vector2){x, 0}, (Vector2){x, canvas->image.height}, LN_THIC, c);
        } 
    }
}
