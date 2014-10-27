#include "malloc.h"
#include "../kernel.h" // For kernel_panic()
#define MALLOC_FAILURE_ACTION kernel_panic("malloc() failed");
#define ABORT kernel_panic("malloc() aborted");

#define LACKS_STRING_H // Use our own, below
#include "mem.h" // for memcpy(), memset()

// These are returned in places
#define EINVAL 1
#define ENOMEM 2

#define LACKS_SYS_TYPES_H
#define LACKS_ERRNO_H
#define LACKS_TIME_H
#define LACKS_STDLIB_H
#define LACKS_SYS_MMAN_H
#define LACKS_FCNTL_H
#define LACKS_UNISTD_H
#define LACKS_SYS_PARAM_H

#define NO_MALLOC_STATS 1

#define HAVE_MMAP 0

// These constants from http://wiki.osdev.org/Memory_Map_%28x86%29
// This area guaranteed free for use
#define FREEMEM_START 0x00007E00
#define FREEMEM_END   0x0007FFFF
size_t sbrk_watermark = FREEMEM_START;
void* sbrk(ptrdiff_t d) {
	if (d < 0) kernel_panic("sbrk() called with d < 0");
	size_t old_watermark = sbrk_watermark;
	sbrk_watermark += d;
	if (sbrk_watermark >= FREEMEM_END)
		kernel_panic("sbrk() failed - kernel out of memory");
	return (void*)old_watermark;
}

size_t unusedMemory() {
	return FREEMEM_END - sbrk_watermark;
}

#define MORECORE sbrk
#define MORECORE_CONTIGUOUS 1
#define MORECORE_CANNOT_TRIM

#include "malloc.c.impl" // Now that we've set everything up, actually call malloc
