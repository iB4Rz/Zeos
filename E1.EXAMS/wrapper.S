#include <asm.h>
#include <segment.h>

sysenter_call:
    pushl %ecx
    pushl %edx
    pushl $sysenter_ret
    pushl %ebp
    movl %esp, %ebp
    sysenter
sysenter_ret:
    popl %ebp
    addl $4,%esp
    popl %edx
    popl %ecx
    ret

ENTRY (write)
    pushl %ebp
    movl %esp, %ebp
    pushl %ebx
    call prepare_params
    movl $0x04, %eax
    call sysenter_call
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
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY (getpid)
    pushl %ebp
    movl %esp, %ebp
    movl $0x14, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY (fork)
    pushl %ebp
    movl %esp, %ebp
    movl $0x02, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY (exit)
    pushl %ebp
    movl %esp, %ebp
    movl $0x01, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    movl %ebp, %esp
    popl %ebp
    ret

ENTRY(get_stats)
    push %ebp
    mov %esp, %ebp
    pushl %ebx
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl $0x23, %eax
    call sysenter_call
    popl %ebx
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
