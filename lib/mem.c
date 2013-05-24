#include <stddef.h> /* for size_t */

/* Copy 'count' bytes of data from 'src' to 'dest', finally return 'dest' */
void *memcpy(void *dest, const void *src, size_t n) {
	// Implementation from http://clc-wiki.net/wiki/C_standard_library:string.h:memcpy
	// Public domain.
	char *dp = dest;
	const char *sp = src;
	while (n--)
		*dp++ = *sp++;
	return dest;
}

/* Set 'count' bytes in 'dest' to 'val'. Again, return 'dest' */
#include <stddef.h> /* size_t */
void *memset(void *s, int c, size_t n) {
	// Implementation from http://clc-wiki.net/wiki/C_standard_library:string.h:memset
	// Public domain.
	unsigned char* p=s;
	while(n--)
		*p++ = (unsigned char)c;
	return s;
}

/* Same as above, but this time, we're working with a 16-bit
 *  'val' and dest pointer. Your code can be an exact copy of
 *  the above, provided that your local variables if any, are
 *  unsigned short * /
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) {
	for (int i=0; i < count; i++)
		dest[i]=val;
	return dest;
}*/

