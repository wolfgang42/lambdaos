typedef struct event event;
struct event {
	unsigned int code;
	void* data;
	void (*freefn)(void*);
};

typedef struct event_handler event_handler;
struct event_handler {
	bool (*fn)(event*);
	event_handler* next_handler;
};

extern void event_enqueue(unsigned int code, void* data, void (*fn)(void*));
extern event* event_dequeue();
extern void event_attach(unsigned int code, bool (*fn)(event*));
extern void events_install();
extern bool event_loop();

#define EVENT_ALL 0
#define EVENT_TIMER_TICK 1
#define EVENT_KBD_SCANCODE 2
#define EVENT_KBD_ACTION 3
#define EVENT_KBD_KEY 4

#define EVENT_MAX EVENT_KBD_KEY
