#include <stdbool.h>

typedef struct queue_entry queue_entry;
struct queue_entry {
	queue_entry* next_item;
	void* data;
};

typedef struct fifo_queue fifo_queue;
struct fifo_queue {
	queue_entry* first_item;
	queue_entry* last_item;
};

/*typedef struct lifo_queue lifo_queue;
struct lifo_queue {
	queue_entry* first_item;
};*/

extern void fifo_queue_init(fifo_queue*);
extern void fifo_queue_push(fifo_queue*, void*);
//extern void lifo_queue_push(lifo_queue*, void*);
extern void* fifo_queue_pop(fifo_queue*);
//extern void* lifo_queue_pop(lifo_queue*);
extern bool fifo_queue_empty(fifo_queue*);
// extern bool lifo_queue_empty(lifo_queue*);