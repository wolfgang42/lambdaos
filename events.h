typedef struct event event;
struct event {
	unsigned int code;
	void* data;
	event* nextEvent;
};

typedef struct event_handler event_handler;
struct event_handler {
	unsigned int code;
	bool (*fn)(event*);
	event_handler* next_handler;
};

extern void event_enqueue(event* ev);
extern event* event_dequeue();
extern void event_attach(unsigned int code, bool (*fn)(event*));
extern bool event_loop();