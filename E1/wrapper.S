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
    call sysenter_call
    popl %ebx
    cmp  $0,%eax
    jl sysenter_error
    jmp end

ENTRY (gettime)
    pushl %ebp
    movl %esp, %ebp
    movl $0x0A, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    jmp end

ENTRY (getpid)
    pushl %ebp
    movl %esp, %ebp
    movl $0x14, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    jmp end

ENTRY (fork)
    pushl %ebp
    movl %esp, %ebp
    movl $0x02, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    jmp end

ENTRY (exit)
    pushl %ebp
    movl %esp, %ebp
    movl $0x01, %eax
    call sysenter_call
    cmp  $0,%eax
    jl sysenter_error
    jmp end

ENTRY(get_stats)
    push %ebp
    movl %esp, %ebp
    pushl %ebx
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl $0x23, %eax
    call sysenter_call
    popl %ebx
    cmp  $0,%eax
    jl sysenter_error
    jmp end

// Factorizations

ENTRY(sysenter_call)
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

sysenter_error:
    neg %eax
    movl %eax, errno
    movl $-1, %eax
    jmp end

end:
    movl %ebp, %esp
    popl %ebp
    ret
