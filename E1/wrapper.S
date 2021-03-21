#include <asm.h>
#include <segment.h>

ENTRY (write)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx
    movl $0x04, %eax
    pushl %ecx
    pushl %edx
    pushl $retusm_write
    pushl %ebp
    movl %esp, %ebp
    sysenter
retusm_write:
    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    popl %ebx
    cmp  $0,%eax
    jge enderror_write
    neg %eax
    mov %eax, errno
    mov $-1, %eax
enderror_write:
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY (gettime)
    pushl %ebp
    movl %esp, %ebp
    movl $0x0A, %eax
    pushl %ecx
    pushl %edx
    pushl $retusm_gettime
    pushl %ebp
    movl %esp, %ebp
    sysenter
retusm_gettime:
    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    cmp  $0,%eax
    jge enderror_gettime
    neg %eax
    mov %eax, errno
    mov $-1, %eax
enderror_gettime:
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY (getpid)
    pushl %ebp
    movl %esp, %ebp
    movl $20, %eax
    pushl %ecx
    pushl %edx
    pushl $retusm_getpid
    pushl %ebp
    movl %esp, %ebp
    sysenter
retusm_getpid:
    popl %ebp
    addl $4, %esp
    popl %edx
    popl %ecx
    cmp  $0,%eax
    jge enderror_getpid
    neg %eax
    mov %eax, errno
    mov $-1, %eax
enderror_getpid:
    movl %ebp, %esp
    popl %ebp
    ret
