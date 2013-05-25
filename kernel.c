#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "driver/vga.h"
#include "lib/str.h"
 
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

void vga_bootsplash_statusmsg(char* message, int8_t percent) {
	size_t index=0;
	size_t length=strlen(message);
	uint8_t color = vga_make_color(COLOR_BLUE,COLOR_LIGHT_GREEN);
	size_t max=VGA_WIDTH*percent/100;
	for (size_t c = 0; c <= 1; c++) { // Do this next bit twice (once for each half of progress bar)
		for (index=index; index <= max; index++)
			if (index < length)
				vga_putentryat(message[index],color,index,VGA_HEIGHT);
			else
				vga_putentryat(' ',           color,index,VGA_HEIGHT);
		color = vga_make_color(COLOR_BLUE,COLOR_GREEN);
		max=VGA_WIDTH;
	}
}

void vga_bootsplash() {
	// TODO be more mathematically precise?
	vga_reset();
	vga_setcolor(vga_make_color(COLOR_BLACK,COLOR_LIGHT_BLUE));
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
	vga_setcolor(vga_make_color(COLOR_WHITE,COLOR_LIGHT_BLUE));
	vga_row=12;
	for (size_t i=0;i <= LOGOTXT_V;i++) { // Draw the 'OS' text
		vga_column=LAMBDA_CENTERING_CONSTANT-8;
		vga_writestring(logotxt[i]);
		vga_writestring("\n");
	}
	vga_bootsplash_statusmsg("Welcome to LambdaOS.",0);
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
extern void gdt_install();
extern void idt_install();
extern void isrs_install();
void kernel_main() {
	vga_bootsplash();
	gdt_install();
	idt_install();
	isrs_install();
	// TODO do stuff
	// TODO vga_reset(); // (after boot)
	vga_writestring("\nWill now halt.");
}
