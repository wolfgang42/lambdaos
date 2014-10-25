#include <stddef.h> // For size_t
#include "queue.h"
#include "malloc.h"
#include "../kernel.h"

void fifo_queue_init(fifo_queue* q) {
	q->first_item = NULL;
	q->last_item = NULL;
}

void fifo_queue_push(fifo_queue* q, void* d) {
	queue_entry* e = malloc(sizeof(queue_entry));
	e->data = d;
	e->next_item = NULL; // Currently the last item in the list
	if (q->last_item == NULL) {
		// No entry in queue, make this the only event
		assert(q->first_item == NULL);
		q->last_item = q->first_item = e;
	} else {
		// Add this entry to the end of the queue
		q->last_item->next_item = e;
		q->last_item = e;
	}
}

void* fifo_queue_pop(fifo_queue* q) {
	if (q->first_item == NULL) {
		// No events in queue
		assert(q->last_item == NULL);
		return NULL;
	} else {
		queue_entry* e = q->first_item;
		if (e->next_item == NULL) {
			// This was the only event in the queue
			assert(q->first_item == q->last_item);
			q->first_item = q->last_item = NULL;
		} else {
			q->first_item = q->first_item->next_item;
		}
		void* data = e->data;
		free(e);
		return data;
	}
}

bool fifo_queue_empty(fifo_queue* q) {
	return q->first_item == NULL;
}

// TODO lifo_queue_push/lifo_queue_pop