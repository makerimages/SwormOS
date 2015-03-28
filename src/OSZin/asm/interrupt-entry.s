.type interrupt_handler_prepare, @function
interrupt_handler_prepare:
	xchg %bx, %bx
	pushl %eax
	pushl %ecx
	pushl %edx
	pushl %ebx
	pushl %ebp
	pushl %esi
	pushl %edi

	movl %ds, %ebp
	pushl %ebp
	movl %es, %ebp
	pushl %ebp
	movl %fs, %ebp
	pushl %ebp
	movl %gs, %ebp
	pushl %ebp

	movl $0x10 /* Kernel Data Segment */, %ebp
	movl %ebp, %ds
	movl %ebp, %es
	movl %ebp, %fs
	movl %ebp, %gs

	movl %cr2, %ebp
	pushl %ebp

	movl %esp, %ebx
	subl $4, %esp
	andl $0xFFFFFFF0, %esp /* 16-byte align stack */
	call interrupt_handler
	movl %ebx, %esp

	popl %ebp
	movl %ebp, %cr2

	popl %ebp
	movl %ebp, %gs
	popl %ebp
	movl %ebp, %fs
	popl %ebp
	movl %ebp, %es
	popl %ebp
	movl %ebp, %ds

	popl %edi
	popl %esi
	popl %ebp
	popl %ebx
	popl %edx
	popl %ecx
	popl %eax

	addl $8, %esp
	iret
.size interrupt_handler_prepare, . - interrupt_handler_prepare