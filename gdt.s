# G L O B A L   D E S C R I P T O R   T A B L E   R O U T I N E S
# This will set up our new segment registers. We need to do
# something special in order to set CS. We do what is called a
# far jump. A jump that includes a segment as well as an offset.
# This is declared in C as 'extern void gdt_flush();'
.global gdt_flush     # Allows the C code to link to this
.extern gp            # Says that '_gp' is in another file
gdt_flush:
	lgdt gp           # Load the GDT with our '_gp' which is a special pointer

	movw $0x10,%ax    # 0x10 is the offset in the GDT to our data segment
	movw %ax,%ds
	movw %ax,%es
	movw %ax,%fs
	movw %ax,%gs
	movw %ax,%ss
	jmp 0x08 # TODO in nasm this is jmp 0x08:flush2, translate properly
	jmp flush2   # 0x08 is the offset to our code segment: Far jump!
flush2:
	ret               # Returns back to the C code!

