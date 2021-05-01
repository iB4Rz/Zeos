#include <libc.h>

char buff[24];

int pid;

int fib(int n) {
  if (n <= 1) return 1;
  else return fib(n-1) + fib(n-2);
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  int ret;
  char buff[256];

  write(1, " ", 1);

  //for (int number = 10; number < 100; number++) {
  //  ret = fib(number);
  //  itoa(ret, buff);
  //  write(1, buff, strlen(buff));
  //  write(1, " ", 1);
  //}
  while(1) { }
}
