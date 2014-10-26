#include "../irq.h"
#include "../kernel.h"
#include "../events.h"
#include "../lib/malloc.h"
/* This will keep track of how many ticks that the system
*  has been running for */
volatile unsigned long long timer_ticks = 0;
const int timer_phase = 100;

typedef struct {
	void (*handler)();
	unsigned long long time;
} timer_schedule_item;
#define TIMER_SCHEDULE_MAXITEMS 10
volatile timer_schedule_item timer_schedule_list[TIMER_SCHEDULE_MAXITEMS];

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
	event* e = malloc(sizeof(event));
	e->code = EVENT_TIMER_TICK;
	unsigned long long* t = malloc(sizeof(unsigned long long));
	*t = timer_ticks;
	e->data=t;
	event_enqueue(e);
}

bool timer_event(event* e) {
	unsigned long long tick = *(unsigned long long*)(e->data);
	for (int i=0; i < TIMER_SCHEDULE_MAXITEMS; i++) {
		if (timer_schedule_list[i].handler) {
			if (timer_schedule_list[i].time == tick) {
				timer_schedule_list[i].handler();
				timer_schedule_list[i].handler=0;
			}
		}
	}
	return true; // Allow other things that hook the timer to run
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install() {
	for (int i=0; i < TIMER_SCHEDULE_MAXITEMS; i++) // Initialize scheduler
		timer_schedule_list[i].handler = 0;
	timer_set_phase(timer_phase); // 100 Hz
	/* Installs 'timer_handler' to IRQ0 */
	irq_install_handler(0, timer_handler);
	event_attach(EVENT_TIMER_TICK, timer_event);
}

unsigned long long timer_schedule(void(*handler)(), unsigned long long ticks) {
	unsigned long long time=timer_ticks+ticks;
	if (time == 0) time = 1;
	for (int i=0; i < TIMER_SCHEDULE_MAXITEMS; i++) {
		if (!timer_schedule_list[i].handler) { // This slot is free
			timer_schedule_list[i].handler=handler;
			timer_schedule_list[i].time=time;
			return time;
		}
	}
	kernel_panic("Timer out of scheduler slots.");
}
