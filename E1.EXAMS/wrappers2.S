#include <asm.h>
#include <segment.h>

ENTRY(prepare_params)
    movl 8(%ebp), %ebx
    movl 12(%ebp), %ecx
    movl 16(%ebp), %edx
    movl 20(%ebp), %esi
    movl 24(%ebp), %edi
    ret
