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
	push %esp

	push %eax
	push %ebx

	call kernelMain

	jmp .Lhang
.Lhang:
	jmp .Lhang


.size _start, . - _start
