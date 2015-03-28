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
	mov  $0, %ebp

	push %eax
	push %ebx
		# Load the Global Descriptor Table pointer register.

	subl $6, %esp
	movw gdt_size_minus_one, %cx
	movw %cx, 0(%esp)
	movl $gdt, %ecx
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

	
	call kernelMain
	lock incl (0xB8000)
	jmp .Lhang
.Lhang:

	jmp .Lhang


.size _start, . - _start
