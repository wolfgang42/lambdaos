#include <stddef.h>
#include "kernel.h"
#include "lib/malloc.h"
#include "lib/queue.h"
#include "events.h"

fifo_queue* event_queue;

void events_install() {
	fifo_queue_init(event_queue);
}

/* Add an event to the event queue
 * The caller is responsible for malloc()ing the data parameter.
 * Once the event is no longer used, freefn will be called
 * to free the data structure (if desired) before destroying the event.
 * The easiest way to do this (if you don't have a special data structure)
 * is to simply pass in malloc's free().
 */
void event_enqueue(unsigned int code, void* data, void (*freefn)(void*)) {
	event* ev = malloc(sizeof(event));
	ev->code = code;
	ev->data = data;
	ev->freefn = freefn;
	fifo_queue_push(event_queue, ev);
}

event_handler* event_handlers[EVENT_MAX+1] = { [0 ... EVENT_MAX] = NULL};

void event_attach(unsigned int code, bool (*fn)(event*)) {
	event_handler* handler = malloc(sizeof(event_handler));
	handler->fn = fn;
	if (event_handlers[code] == NULL) {
		handler->next_handler = NULL;
	} else {
		handler->next_handler = event_handlers[code];
	}
	event_handlers[code] = handler;
}

bool event_loop() {
	event* active_event;
	if ((active_event = fifo_queue_pop(event_queue)) != NULL) {
		event_handler* handler = event_handlers[active_event->code];
		bool run = true;
		while (handler != NULL && run) {
			run = handler->fn(active_event);
			handler = handler->next_handler;
		}
		active_event->freefn(active_event->data);
		free(active_event); // TODO free data parameter!
	}
	return !fifo_queue_empty(event_queue);
}