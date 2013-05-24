# I N T E R R U P T   D E S C R I P T O R   T A B L E   R O U T I N E
# Loads the IDT defined in '_idtp' into the processor.
# This is declared in C as 'extern void idt_load();'
.global idt_load
.extern idtp
idt_load:
    lidt idtp
    ret
