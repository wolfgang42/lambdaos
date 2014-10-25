#include <stddef.h>
#include "kernel.h"
#include "lib/malloc.h"
#include "events.h"

event* firstEvent = NULL;
event* lastEvent = NULL;

/* Add an event to the event queue
 * The caller is responsible for malloc()ing the event, and its data structures.
 */
void event_enqueue(event* ev) {
	if (lastEvent == NULL) {
		// No events in queue, make this the only event
		assert(firstEvent == NULL);
		lastEvent = firstEvent = ev;
	} else {
		lastEvent->nextEvent = ev;
		lastEvent = ev;
	}
	ev->nextEvent = NULL; // This is the last event in the list
}

/* Get the next event in the queue.
 * The caller is responsible for free()ing the event, and its data structures.
 */
event* event_dequeue() {
	if (firstEvent == NULL) {
		// No events in queue
		assert(lastEvent == NULL);
		return NULL;
	} else {
		event* ev = firstEvent;
		if (firstEvent->nextEvent == NULL) {
			// This is the only event in the queue
			assert(firstEvent == lastEvent);
			firstEvent = lastEvent = NULL;
		} else {
			firstEvent = firstEvent->nextEvent;
		}
		return ev;
	}
}

event_handler* first_event_handler = NULL;

void event_attach(unsigned int code, bool (*fn)(event*)) {
	event_handler* handler = malloc(sizeof(event_handler));
	handler->code = code;
	handler->fn = fn;
	if (first_event_handler == NULL) {
		handler->next_handler = NULL;
	} else {
		handler->next_handler = first_event_handler;
	}
	first_event_handler = handler;
}

bool event_loop() {
	event* active_event;
	if (first_event_handler == NULL) return false; // No event handlers!
	if ((active_event = event_dequeue()) != NULL) {
		event_handler* handler = first_event_handler;
		bool run = true;
		do {
			run = handler->fn(active_event);
			handler = handler->next_handler;
		} while (handler != NULL && run);
		free(active_event);
	}
	return firstEvent != NULL;
}