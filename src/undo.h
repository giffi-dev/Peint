#ifndef UNDO_H
#define UNDO_H
#include <raylib.h>
#include "tools.h"

#define UNDO_STACK_SIZE 64
typedef struct undostack_t {
    drawtool_t stack[UNDO_STACK_SIZE];
    int idx;
	bool record;
} undostack_t;

void UndoReset(undostack_t* undo);
drawtool_t UndoPop(undostack_t* undo);
void UndoPush(undostack_t* undo, const drawtool_t* tool);

#endif
