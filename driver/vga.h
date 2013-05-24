#include <stddef.h>
#include <stdint.h>
/* Hardware text mode color constants. */
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 24;

extern size_t vga_row;
extern size_t vga_column;
extern uint8_t vga_color;
extern uint16_t* vga_buffer;

extern uint8_t vga_make_color(enum vga_color fg, enum vga_color bg);
extern uint16_t vga_make_entry(char c, uint8_t color);
extern void vga_putentryat(char c, uint8_t color, size_t x, size_t y);
extern void vga_clear();
extern void vga_reset();
extern void vga_setcolor(uint8_t color);
extern void vga_putchar(char c);
extern void vga_writestring(const char* data);
