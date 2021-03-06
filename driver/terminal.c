#include "vga.h"
#include "timer.h"
#include "keyboard/ps2/keymap.h"
#include "keyboard/ps2/key_constants.h"
#include "../events.h"
#include "keyboard/ascii_map.h"
#include "../lib/str.h"
#include "../lib/mem.h"
#include "../lib/malloc.h"

char term_input[81] = "";
unsigned short term_input_pos=0;
bool term_bell=false;

void terminal_draw_input() {
	if (term_bell) {
		vga_setcolor(vga_make_color(COLOR_BLACK, COLOR_LIGHT_BLUE));
	} else {
		vga_setcolor(vga_make_color(COLOR_LIGHT_BLUE, COLOR_BLACK));
	}
	vga_set_cursor_pos(term_input_pos, VGA_HEIGHT);
	vga_row=VGA_HEIGHT; vga_column=0;
	vga_writestring(term_input);
	while (vga_column != VGA_WIDTH-1) vga_putchar(' ');
	vga_putchar(' ');
}

void terminal_bell_off() {
	//tone(0);
	term_bell = false;
	terminal_draw_input();
}

void terminal_bell() {
	//tone(800);
	term_bell = true;
	terminal_draw_input();
	timer_schedule(terminal_bell_off, 30);
}

bool terminal_keyboard_handler(event* ev) {
	keyboard_action action = *(keyboard_action*)ev->data;
	if (action.key == KBD_KEY_LEFT_SHIFT || action.key == KBD_KEY_RIGHT_SHIFT) {
		kbd_shift = action.pressed;
	} else if (action.pressed) {
		if (action.key == KBD_KEY_BACKSPACE) {
			if (term_input_pos == 0) {
				terminal_bell();
			} else if (term_input_pos == strlen(term_input)) {
				term_input[--term_input_pos] = '\0';
			} else {
				unsigned short i = --term_input_pos;
				while (term_input[i] != '\0') {
					term_input[i] = term_input[i+1];
					i++;
				}
			}
		} else if (action.key == KBD_KEY_CURSOR_LEFT) {
			if (term_input_pos == 0) {
				terminal_bell();
			} else {
				term_input_pos--;
			}
		} else if (action.key == KBD_KEY_CURSOR_RIGHT) {
			if (term_input_pos == strlen(term_input)) {
				terminal_bell();
			} else {
				term_input_pos++;
			}
		} else {
			char c = ascii_map(action.key);
			if (c == '\0') {
				terminal_bell();
			} else {
				if (term_input_pos == VGA_WIDTH) {
					terminal_bell();
				} else if (term_input_pos == strlen(term_input)) {
					term_input[term_input_pos++] = c;
					term_input[term_input_pos+1] = '\0';
				} else {
					unsigned short i = strlen(term_input);
					while (i > term_input_pos) {
						term_input[i] = term_input[i-1];
						i--;
					}
					term_input[term_input_pos] = c;
					term_input_pos++;
				}
			}
		}
		terminal_draw_input();
	}
	return true;
}

#define TERM_BUFMAX 99
char* term_buffer[TERM_BUFMAX+1];
unsigned int term_buf_writepos  = TERM_BUFMAX;
unsigned int term_buf_scrollpos = 0;

void terminal_refresh() {
	vga_setcolor(vga_make_color(COLOR_LIGHT_GREY, COLOR_BLACK));
	vga_clear();
	vga_row = 0;
	vga_column = 0;
	for (unsigned int i = term_buf_scrollpos; i < term_buf_scrollpos+23; i++) {
		vga_writestring(term_buffer[i]);
		vga_putchar('\n');
	}
}

void terminal_writeline(char* line) {
	// First handle oversize lines.
	if (strlen(line) > 79) {
		char* firsthalf = "";
		memcpy(firsthalf, line, 79);
		firsthalf[80] = '\0';
		terminal_writeline(firsthalf);
		terminal_writeline(line+80); // Second half
		return;
	}
	
	term_buf_writepos++;
	if (term_buf_writepos > TERM_BUFMAX) term_buf_writepos = 0;
	
	free(term_buffer[term_buf_writepos]);
	term_buffer[term_buf_writepos] = malloc(strlen(line)+1);
	memcpy(term_buffer[term_buf_writepos], line, strlen(line)+1);
	terminal_refresh();
}

void terminal_init() {
	for (unsigned int i = 0; i <= TERM_BUFMAX; i++) {
		term_buffer[i] = malloc(1);
		*term_buffer[i] = '\0';
	}
	
	vga_set_cursor_display(true, 12, 15);
	event_attach(EVENT_KBD_ACTION, terminal_keyboard_handler);
	
	terminal_draw_input();
	terminal_refresh();
}