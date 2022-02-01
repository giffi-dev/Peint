#define RAYGUI_IMPLEMENTATION

#include <stdlib.h>
#include <raylib.h>
#include <raygui.h>
#include "editor.h"

int main() {
    InitEditor();
    atexit(CleanupEditor);
    MainLoopEditor();
    return 0;
}
