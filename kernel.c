#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "driver/vga.h"
#include "lib/str.h"
#include "lib/printf.h"
#include "kernel.h"
 
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
	vga_set_cursor_display(false,0,0);
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

extern void system_fullhalt() __attribute__ ((noreturn));
void do_kernel_panic(const char *message, const char* file, const char* function, unsigned const int line) {
	vga_set_cursor_display(false,0,0);
	vga_setcolor(vga_make_color(COLOR_BLACK,COLOR_GREEN));
	vga_clear();
	vga_draw_dialog(0,0,VGA_WIDTH,VGA_HEIGHT,vga_make_color(COLOR_RED,COLOR_GREEN),"Kernel Panic\x13");
	vga_row=2;vga_column=2;vga_setcolor(vga_make_color(COLOR_WHITE,COLOR_GREEN));
	vga_writestring("Something has gone horribly wrong inside LambdaOS.");
	vga_row=6;vga_column=2;
	vga_putchar(4);
	vga_writestring(" Please report this problem, along with steps to reproduce it (if possible),");
	vga_row=7;vga_column=4;
	vga_writestring("to: ");
	vga_setcolor(vga_make_color(COLOR_BLUE,COLOR_GREEN));vga_writestring("http://github.com/wolfgang42/lambdaos/issues/");
	vga_row=9;vga_column=2;vga_setcolor(vga_make_color(COLOR_WHITE,COLOR_GREEN));
	vga_putchar(4);
	vga_writestring(" You must restart your computer manually.");
	vga_row=3;vga_column=5;
	vga_setcolor(vga_make_color(COLOR_LIGHT_GREY,COLOR_GREEN));vga_writestring("Error:    ");
	vga_setcolor(vga_make_color(COLOR_BLACK,COLOR_GREEN));vga_writestring(message);
	vga_row=4;vga_column=5;
	vga_setcolor(vga_make_color(COLOR_LIGHT_GREY,COLOR_GREEN));vga_writestring("Location: ");
	vga_setcolor(vga_make_color(COLOR_BLACK,COLOR_GREEN));
	printf("%s:%u (in function %s)", file, line, function);
	system_fullhalt();
}

void kernel_shutdown() {
	char* message=" It is now safe to switch off your computer. ";
	size_t x=(VGA_WIDTH/2)-(strlen(message)/2);
	size_t y=(VGA_HEIGHT/2)-1;
	vga_set_cursor_display(false,0,0);
	vga_setcolor(vga_make_color(COLOR_WHITE,COLOR_BLACK));
	vga_clear();
	vga_draw_dialog(x,y,strlen(message)+2,4,vga_color,"System halted.");
	vga_row=y+2;vga_column=x+1;
	vga_writestring(message);
	system_fullhalt();
}

/* We will use this later on for reading from the I/O ports to get data
*  from devices such as the keyboard. We are using what is called
*  'inline assembly' in these routines to actually do the work */
unsigned char inportb (unsigned short _port) {
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
	return rv;
}

/* We will use this to write to I/O ports to send bytes to devices. This
*  will be used in the next tutorial for changing the textmode cursor
*  position. Again, we use some inline assembly for the stuff that simply
*  cannot be done in C */
void outportb (unsigned short _port, unsigned char _data) {
	__asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
extern void gdt_install();
extern void idt_install();
extern void isrs_install();
void kernel_main() {
	vga_reset();
	vga_bootsplash();
	gdt_install();
	idt_install();
	isrs_install();
	// TODO do stuff
	// TODO vga_reset(); // (after boot)
	kernel_shutdown();
}
