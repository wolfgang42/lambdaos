#include "../../../irq.h"
#include "../../../kernel.h"
#include "keymap.h"

/* Handles the keyboard interrupt */
void keyboard_handler(struct regs *r __attribute__((__unused__))) {
	unsigned char scancode;

	/* Read from the keyboard's data buffer */
	scancode = inportb(0x60);

	// TODO handle keyboard status codes
	// See http://stanislavs.org/helppc/keyboard_commands.html
	keyboard_kmap(scancode);
}

void keyboard_install() {
	irq_install_handler(1, keyboard_handler);
}
