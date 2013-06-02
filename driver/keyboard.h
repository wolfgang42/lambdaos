#include <stdbool.h>
#define KEYBOARD_FLAG_ASCII    0x0001  // Is this an ASCII character? (if false, event.ascii is null)
#define KEYBOARD_FLAG_PRINTS   0x0002  // Is it a *printable* ASCII char? (\b, \n, etc. are false)
#define KEYBOARD_FLAG_CTRL     0x0004  // Was CTRL  down?
#define KEYBOARD_FLAG_LCTRL    0x0008
#define KEYBOARD_FLAG_RCTRL    0x0010
#define KEYBOARD_FLAG_ALT      0x0020  // Was ALT   down?
#define KEYBOARD_FLAG_LALT     0x0040
#define KEYBOARD_FLAG_RALT     0x0080
#define KEYBOARD_FLAG_SHIFT    0x0100  // Was SHIFT down?
#define KEYBOARD_FLAG_LSHIFT   0x0200  // Was left  SHIFT down?
#define KEYBOARD_FLAG_RSHIFT   0x0400  // Was right SHIFT down?
#define KEYBOARD_FLAG_CAPSLOCK 0x0800  // Was CAPSLOCK down?
#define KEYBOARD_FLAG_CAPS     0x1000  // Was this a capital letter? (takes both SHIFT and CAPSLOCK into account)
#define KEYBOARD_FLAG_SCRLOCK  0x2000  // Was Scroll Lock enabled?
#define KEYBOARD_FLAG_INSERT   0x4000  // Is insert mode enabled?
#define KEYBOARD_FLAG_BADSCAN  0x8000  // Is this a valid scancode? (Keyboard maps only!)
// Remainder of flags reserved.

struct keyboard_event {
	bool keydown;
	char ascii;
	unsigned char scancode;
	uint16_t flags; // Note: flags are for *after* scancode is processed
}

struct keyboard_map {
	// Could theoretically be anything, this is just what I picked
	char* name;
	uint16_t flags[127];
	char lowercase[127];
	char uppercase[127];
}

extern void keyboard_keycode_handler(char keycode);
extern void keyboard_set_map(keyboard_map map);
