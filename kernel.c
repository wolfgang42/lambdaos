#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "driver/vga.h"
 
/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#define LOGOTXT_V 5
static const char* logotxt[LOGOTXT_V]={
	"  _|_|       _|_|_|",
	"_|    _|   _|      ",
	"_|    _|     _|_|  ",
	"_|    _|         _|",
	"  _|_|     _|_|_|  "};

void vga_bootsplash() {
	// TODO be more mathematically precise?
	vga_reset();
	vga_setcolor(make_color(COLOR_BLACK,COLOR_LIGHT_BLUE));
	vga_clear();
	#define LAMBDA_CENTERING_CONSTANT 36
	for (size_t i=2;i<21;i++) { // Draw an uppercase lambda
		vga_putentryat('\\',vga_color,LAMBDA_CENTERING_CONSTANT+0+i,i);
		vga_putentryat('\\',vga_color,LAMBDA_CENTERING_CONSTANT+1+i,i);
		vga_putentryat('/', vga_color,LAMBDA_CENTERING_CONSTANT+3-i,i);
	}
	// Serifs
	vga_putentryat('_',vga_color,LAMBDA_CENTERING_CONSTANT-18,20);
	vga_putentryat('_',vga_color,LAMBDA_CENTERING_CONSTANT-16,20);
	vga_putentryat('_',vga_color,LAMBDA_CENTERING_CONSTANT+19,20);
	vga_putentryat('_',vga_color,LAMBDA_CENTERING_CONSTANT+22,20);
	vga_setcolor(make_color(COLOR_WHITE,COLOR_LIGHT_BLUE));
	vga_row=12;
	for (size_t i=0;i <= LOGOTXT_V;i++) { // Draw the 'OS' text
		vga_column=LAMBDA_CENTERING_CONSTANT-8;
		vga_writestring(logotxt[i]);
		vga_writestring("\n");
	}
	vga_setcolor(make_color(COLOR_BLUE,COLOR_LIGHT_GREEN));
	vga_row=VGA_HEIGHT;
	for (size_t i=0; i <= VGA_WIDTH; i++) // Clear bottom bar
		vga_putentryat(' ',vga_color,i,VGA_HEIGHT);
	vga_writestring("Welcome to LambdaOS.\r");
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main()
{
	vga_bootsplash();
	// TODO do stuff
	// TODO reset vga after boot
	//vga_writestring("\nWill now halt.");
}
