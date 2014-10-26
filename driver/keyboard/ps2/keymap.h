#include <stdbool.h>

typedef struct keyboard_action keyboard_action;
struct keyboard_action {
	unsigned int key;
	bool pressed;
};

extern void keyboard_kmap(unsigned char byte);
