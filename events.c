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
 * The caller is responsible for malloc()ing the event, and its data structures.
 */
void event_enqueue(event* ev) {
	fifo_queue_push(event_queue, ev);
}

/* Get the next event in the queue.
 * The caller is responsible for free()ing the event, and its data structures.
 */
event* event_dequeue() {
	return fifo_queue_pop(event_queue);
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
	if ((active_event = event_dequeue()) != NULL) {
		event_handler* handler = event_handlers[active_event->code];
		bool run = true;
		while (handler != NULL && run) {
			run = handler->fn(active_event);
			handler = handler->next_handler;
		}
		free(active_event);
	}
	return !fifo_queue_empty(event_queue);
}