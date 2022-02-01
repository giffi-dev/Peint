#include <string.h>
#include <stdio.h>
#include "undo.h"

void UndoReset(undostack_t* undo) {
    memset((void*)undo, 0, sizeof(undostack_t));
	undo->record = true;
}

drawtool_t UndoPop(undostack_t* undo) {
    drawtool_t out = undo->stack[undo->idx];
    undo->stack[undo->idx--].type = TOOL_NULL; // TODO: redo
    if( undo->idx <= -1) {
        undo->idx = UNDO_STACK_SIZE - 1;
    }

    return out;
}

void UndoPush(undostack_t* undo, const drawtool_t* tool) {
	if (!undo->record)
		return;
	
	if (++undo->idx >= UNDO_STACK_SIZE) {
        undo->idx = 0;
    }
    undo->stack[undo->idx] = *tool;
}
