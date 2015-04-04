/* Ports input/output. */

.section .text

/* uint8_t inb (uint16_t port) */
.global inb
.type inb, @function
inb:
    pushl %ebp                          /* save %ebp */
    movl %esp, %ebp                     /* use %ebp to access the stack */
    movl 8(%ebp), %edx                  /* arg. 1: port number */

    xorl %eax, %eax
    in %dx, %al

    popl %ebp                           /* restore %ebp */
    ret

/* void outb (uint16_t port, uint8_t value) */
.global outb
.type outb, @function
outb:
    pushl %ebp                          /* save %ebp */
    movl %esp, %ebp                     /* use %ebp to access the stack */
    movl 8(%ebp), %edx                  /* arg. 1: port number */
    movl 12(%ebp), %eax                 /* arg. 2: value to write */

    out %al, %dx

    popl %ebp                           /* restore %ebp */
    ret

/* void outw (uint16_t port, uint16_t value) */
.global outw
.type outw, @function
outw:
    pushl %ebp                          /* save %ebp */
    movl %esp, %ebp                     /* use %ebp to access the stack */
    movl 8(%ebp), %edx                  /* arg. 1: port number */
    movl 12(%ebp), %eax                 /* arg. 2: value to write */

    out %ax, %dx

    popl %ebp                           /* restore %ebp */
    ret
