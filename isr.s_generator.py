from isr_exceptions import exceptions

print """# This file is autogenerated by isr.s_generator.py
# DO NOT EDIT IT MANUALLY!\n\n"""

for i in range(0,32):
	print "# "+str(i)+": "+exceptions[i][0]
	print ".global isr"+str(i)
	print "isr"+str(i)+":"
	print "\tcli"
	if not exceptions[i][1]: print "\tpushw $0 # Dummy error code to maintain consistent stack frame"
	print "\tpushw $"+str(i)
	print "\tjmp isr_common_stub"
	print

print """.extern fault_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	movw $0x10,%ax # Load the Kernel Data Segment descriptor!
	movw %ax,%ds
	movw %ax,%es
	movw %ax,%fs
	movw %ax,%gs
	movl %esp,%eax # Push us the stack
	pushl %eax
	movl $fault_handler, %eax
	call *%eax     # A special call, preserves the 'eip' register
	popl %eax
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	addl $8,%esp   # Cleans up the pushed error code and pushed ISR number
	iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!"""

