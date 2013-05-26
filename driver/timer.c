#include "../irq.h"
#include "../kernel.h"
/* This will keep track of how many ticks that the system
*  has been running for */
volatile unsigned long long timer_ticks = 0;
const int timer_phase = 100;

void timer_set_phase(int hz) {
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outportb(0x43, 0x36);             /* Set our command byte 0x36 */
	outportb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outportb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz? Some engineer at IBM must've
*  been smoking something funky */
void timer_handler(struct regs *r __attribute__((__unused__))) {
	/* Increment our 'tick count' */
	timer_ticks++;
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install() {
	timer_set_phase(timer_phase); // 100 Hz
	/* Installs 'timer_handler' to IRQ0 */
	irq_install_handler(0, timer_handler);
}

