#include <asm.h>
#include <segment.h>

ENTRY(answerToEverything)
    pushl %ebp
    movl %esp, %ebp
    movl $42, %eax
    movl %ebp, %esp
    popl %ebp
    ret