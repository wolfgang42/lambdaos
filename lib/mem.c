/* Copy 'count' bytes of data from 'src' to 'dest', finally return 'dest' */
unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) {
	for (int i=0; i < count; i++)
		dest[i]=src[i];
	return dest;
}

/* Set 'count' bytes in 'dest' to 'val'. Again, return 'dest' */
unsigned char *memset(unsigned char *dest, unsigned char val, int count) {
	for (int i=0; i < count; i++)
		dest[i]=val;
	return dest;
}

/* Same as above, but this time, we're working with a 16-bit
 *  'val' and dest pointer. Your code can be an exact copy of
 *  the above, provided that your local variables if any, are
 *  unsigned short */
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) {
	for (int i=0; i < count; i++)
		dest[i]=val;
	return dest;
}

