#include <libc.h>
#include <stats.h>

char buff[24];

int pid;

void printi(int i) {
  char c;
  itoa(i, &c);
  write(1, &c, strlen(&c));
}

void fork_test() {
  printf("\nFork test: ");
  int pid = fork();

  if (pid == 0) {
    printf("\nChild with PID: ");
    printi(getpid());
    exit();
  }
  else if (pid != -1) {
    printf("\nParent with PID: ");
    printi(getpid());
  }
  else {
    printf("\nError while calling the fork function\n");
  }
}

void print_stats(int pid) {
  struct stats st;
  if (get_stats(pid, &st) == -1) { perror(); return; }

  printf("\n** Stats for PID "); printi(pid); printf(" **");
  printf("\nUser ticks: "); printi(st.user_ticks);
  printf("\nSystem ticks: "); printi(st.system_ticks);
  printf("\nBlocked ticks: "); printi(st.blocked_ticks);
  printf("\nReady ticks: " ); printi(st.ready_ticks);
  printf("\nElapsed total ticks: "); printi(st.elapsed_total_ticks);
  printf("\nTotal trans: "); printi(st.total_trans);
  printf("\nRemaining ticks: "); printi(st.remaining_ticks);
  printf("\n*************************\n");
}

int fibonacci(int n) {
  if (n <= 1) return 1;
  else return fibonacci(n-1) + fibonacci(n-2);
}

void work() {
  int pid = fork();

  if (pid == 0) {
    fibonacci(25);
    print_stats(getpid());
    exit();
  }
  pid = fork();
  if (pid == 0) {
    fibonacci(32);
    print_stats(getpid());
    exit();
  }

  return;
}


int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
  printf("\n----------------------");
  printf("\n--Bienvenidos a SOA!--");
  printf("\n----------------------\n");
  printf("\nEjecutando tests: ");
  fork_test();
  work();
  while(1);
}
