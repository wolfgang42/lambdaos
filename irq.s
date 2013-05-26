# This file is autogenerated by irq.s_generator.py
# DO NOT EDIT IT MANUALLY!


.global irq0
irq0:
	cli
	push $0
	push $32
	jmp irq_common_stub

.global irq1
irq1:
	cli
	push $0
	push $33
	jmp irq_common_stub

.global irq2
irq2:
	cli
	push $0
	push $34
	jmp irq_common_stub

.global irq3
irq3:
	cli
	push $0
	push $35
	jmp irq_common_stub

.global irq4
irq4:
	cli
	push $0
	push $36
	jmp irq_common_stub

.global irq5
irq5:
	cli
	push $0
	push $37
	jmp irq_common_stub

.global irq6
irq6:
	cli
	push $0
	push $38
	jmp irq_common_stub

.global irq7
irq7:
	cli
	push $0
	push $39
	jmp irq_common_stub

.global irq8
irq8:
	cli
	push $0
	push $40
	jmp irq_common_stub

.global irq9
irq9:
	cli
	push $0
	push $41
	jmp irq_common_stub

.global irq10
irq10:
	cli
	push $0
	push $42
	jmp irq_common_stub

.global irq11
irq11:
	cli
	push $0
	push $43
	jmp irq_common_stub

.global irq12
irq12:
	cli
	push $0
	push $44
	jmp irq_common_stub

.global irq13
irq13:
	cli
	push $0
	push $45
	jmp irq_common_stub

.global irq14
irq14:
	cli
	push $0
	push $46
	jmp irq_common_stub

.global irq15
irq15:
	cli
	push $0
	push $47
	jmp irq_common_stub

.extern irq_handler
irq_common_stub: 
	pusha
	push %ds
	push %es
	push %fs
	push %gs
	movw $0x10,%ax
	movw %ax,%ds
	movw %ax,%es
	movw %ax,%fs
	movw %ax,%gs
	movl %esp,%eax
	pushl %eax
	movl $irq_handler, %eax
	call *%eax
	popl %eax
	popl %gs
	popl %fs
	popl %es
	popl %ds
	popa
	addl $8,%esp
	iret