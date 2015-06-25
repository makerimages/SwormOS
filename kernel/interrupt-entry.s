.macro isr id
    .global isr\id
    .type isr\id, @function
    isr\id:
        pushl $0                        /* fake error code */
        pushl $\id                      /* isr number */
        jmp interrupt_handler_prepare
    .size isr\id, . - isr\id
.endm

.macro isr_ec id
    .global isr\id
    .type isr\id, @function
    isr\id:
        pushl $\id                      /* isr number */
        jmp interrupt_handler_prepare
    .size isr\id, . - isr\id
.endm

.macro irq id
    .global irq\id
    .type irq\id, @function
    irq\id:
        pushl $0                        /* fake error code */
        pushl $(\id + 32)               /* isr number */
        jmp interrupt_handler_prepare
    .size irq\id, . - irq\id
.endm

isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_ec 8
isr 9
isr_ec 10
isr_ec 11
isr_ec 12
isr_ec 13
isr_ec 14
isr 15
isr 16
isr_ec 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr_ec 30
isr 31
irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15

.type interrupt_handler_prepare, @function
interrupt_handler_prepare:
    pusha

	pushl %ds
	pushl %es
	pushl %fs
	pushl %gs

	movl $0x10, %eax
	movl %eax, %ds
	movl %eax, %es
	movl %eax, %fs
	movl %eax, %gs

#	movl %esp, %ebx
#   subl $4, %esp
#   andl $0xFFFFFFF0, %esp /* 16-byte align stack */
    pushl %esp
    movl $interrupt_handler, %eax
    call *%eax
#   call interrupt_handler
    add $4, %esp
#	movl %ebx, %esp

	popl %gs
	popl %fs
	popl %es
	popl %ds

    popa

	addl $8, %esp
	iret
.size interrupt_handler_prepare, . - interrupt_handler_prepare
