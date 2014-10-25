#include <stdbool.h>
extern void do_kernel_panic(const char *message, bool assertion, const char* file,
		const char* function, unsigned const int line) __attribute__((noreturn));
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
#define kernel_panic(M) do_kernel_panic(M,false,__FILE__,__FUNCTION__,__LINE__)
#define assert(EX) (void)((EX) || (do_kernel_panic (#EX,true,__FILE__, __FUNCTION__, __LINE__),0))