#include <stdbool.h>
#include <stdint.h>
/* Handles the keyboard interrupt */
void kbd_ps2_handler(struct regs *r __attribute__((__unused__))) {
	unsigned char scancode;
	static uint8_t extended_counter=0;

	/* Read from the keyboard's data buffer */
	scancode = inportb(0x60);
	printf("0x%02X ",scancode);
	
	if (superextended_extended) {
		
	} if (extended_counter==2 && scancode == 0xE0) {
		extended_counter=3;
	else if (extended_counter==1 && scancode == 0xAA) {
		extended_counter=2;
	} else if (extended_counter==1) {
	
	if (scancode == 0xe0) { // 101-key extended
		extended_counter=1;
	/* If the top bit of the byte we read from the keyboard is
	*  set, that means that a key has just been released */
	} else if (scancode & 0x80) {
		/* You can use this one to see if the user released the
		*  shift, alt, or control keys... */
	} else {
		/* Here, a key was just pressed. Please note that if you
		*  hold a key down, you will get repeated key press
		*  interrupts. */
		keyboard_keycode_handler(kbdus[scancode]);
	}
}

void kbd_ps2_install() {
	irq_install_handler(1, kbd_ps2_handler);
}
