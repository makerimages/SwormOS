# MB HEADER.
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:

	movl $stack_top, %esp
    xorl %ebp, %ebp

	pushl %eax
	pushl %ebx

	subl $6, %esp

	movw %cx, 0(%esp)
	movl %ecx, 2(%esp)
	lgdt 0(%esp)
	addl $6, %esp

	push $0x08
	push $1f
	retf
1:
	movw $0x10, %cx
	movw %cx, %ds
	movw %cx, %es
	movw %cx, %fs
	movw %cx, %gs
	movw %cx, %ss

    .extern kernel_main
	call kernel_main
	hlt
.Lhang:
	jmp .Lhang

.size _start, . - _start
