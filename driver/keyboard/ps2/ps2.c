#include "../../../irq.h"
#include "../../../kernel.h"
#include "../../../events.h"
#include "../../../lib/malloc.h"
#include "keymap.h"
#include "key_constants.h"

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r __attribute__((__unused__))) {
	unsigned char* scancode = malloc(sizeof(unsigned char));

	/* Read from the keyboard's data buffer */
	*scancode = inportb(0x60);
	
	event_enqueue(EVENT_KBD_SCANCODE, scancode, free);
}

bool keyboard_scancode_event(event* ev) {
	keyboard_kmap(*(unsigned char*)ev->data);
	return true;
}

bool keyboard_action_event(event* ev) {
	keyboard_action* action = (keyboard_action*)ev->data;
	vga_writestring("Key ");
	if (action->pressed) {
		vga_writestring("pressed");
	} else {
		vga_writestring("released");
	}
	vga_writestring(": ");
	vga_writestring(action->key);
	vga_writestring("\n");
}

void keyboard_install() {
	irq_install_handler(1, keyboard_handler);
	event_attach(EVENT_KBD_SCANCODE, keyboard_scancode_event);
	//event_attach(EVENT_KBD_ACTION,   keyboard_action_event);
}
