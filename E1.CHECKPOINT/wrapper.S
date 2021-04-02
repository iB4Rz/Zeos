#include <asm.h>
#include <segment.h>


ENTRY (syscall_sysenter)
    pushl %ecx
    pushl %edx
    pushl $sysenter_ret
    pushl %ebp
    movl %esp, %ebp
    sysenter
ENTRY(sysenter_ret)
    popl %ebp
    popl %edx
    popl %edx
    popl %ecx
    ret

ENTRY (write)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx
    movl $0x04, %eax
    call syscall_sysenter
    popl %ebx
    cmp  $0,%eax
    jl sysenter_error
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY (gettime)
    pushl %ebp
    movl %esp, %ebp
    movl $0x0A, %eax
    call syscall_sysenter
    cmp  $0,%eax
    jl sysenter_error
    movl %ebp, %esp
    popl %ebp
    ret

sysenter_error:
    neg %eax
    mov %eax, errno
    mov $-1, %eax
    movl %ebp, %esp
    popl %ebp
    ret