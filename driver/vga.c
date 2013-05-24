#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "vga.h"
#include "../lib/str.h"

size_t vga_row;
size_t vga_column;
uint8_t vga_color;
uint16_t* vga_buffer;

uint8_t vga_make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

uint16_t vga_make_entry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void vga_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index] = vga_make_entry(c, color);
}

void vga_clear() {
	vga_row = 0;
	vga_column = 0;
	for ( size_t y = 0; y <= VGA_HEIGHT; y++ ) // Clear screen
		for ( size_t x = 0; x <= VGA_WIDTH; x++ )
			vga_putentryat(' ',vga_color,x,y);
}

void vga_reset() {
	vga_buffer = (uint16_t*) 0xB8000;
	vga_color = vga_make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	vga_clear();
}
 
void vga_setcolor(uint8_t color) {
	vga_color = color;
}

 
void vga_putchar(char c) {
	if (c == '\n') {
		if ( ++vga_row >= VGA_HEIGHT ) {
			vga_row = 0;
		}
		vga_column=0;
	} else if (c == '\r') {
		vga_column=0;
	} else {
		vga_putentryat(c, vga_color, vga_column, vga_row);
		if ( ++vga_column >= VGA_WIDTH ) {
			vga_column = 0;
			if ( ++vga_row >= VGA_HEIGHT ) {
				vga_row = 0;
			}
		}
	}
}
 
void vga_writestring(const char* data) {
	// XXX BUG: Passing '\r' to vga_writestring() causes gibberish to be written
	// This only occurs if '\r' is the only character in the string.
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ ) 
		vga_putchar(data[i]);
}
