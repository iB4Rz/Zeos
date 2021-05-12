#include <libc.h>

// ----------- UTILITIES -----------
char buff[24];

#define delay fib(31);

int fib(int n) {
  if (n <= 1) return 1;
  else return fib(n-1) + fib(n-2);
}

void printf(int fd,char *msg) {
  write(fd,msg,strlen(msg));
}
// ---------------------------------

// ------------- TESTS -------------
/* create screen test */
void jp_1 () {
  int fd = createScreen();
  printf(fd,"Hola Mundo");
  setFocus(fd);
  delay;
  // Undo
  close(fd);
}

/* screen sharing test */
void jp_2 () {
  int fd1 = createScreen();
  int fd2 = createScreen();
  switch (fork())
  {
  case 0:
    printf(fd1,"Estoy aprendiendo a escribir");
    printf(fd2,"Ya he aprendido a escribir  ");
    int fd3 = createScreen();
    printf(fd3,"Querido diario...");
    delay;
    delay;
    exit();
    break;
  
  default:
    delay;
    printf(fd2,"Yo soy tu padre");
    break;
  }
  delay;
  // Undo
  close(fd1);
  close(fd2);
}

/* close test */
void jp_3() {
  int fd = createScreen();
  setFocus(fd);
  printf(fd,"Pantalla creada para jp_3: ");
  delay;
  switch (fork())
  {
  case 0:
    close(fd);
    delay;
    delay;
    exit();
    break;
  
  default:
    delay;
    printf(fd,"Mi hijo ya ha hecho close, ahora es mi turno");
    delay;
    close(fd);
    break;
  }
}

/* exit test */
void jp_4() {
  delay;
  int fd = 0;
  switch (fork())
  {
  case 0:
    createScreen();
    if (fork() == 0) {
      createScreen();
      delay;
      delay;
      exit();
    }
    delay;
    exit();
  default:
    fd = createScreen();
    break;
  }
  delay;
  delay;
  close(fd);
}
// ---------------------------------

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  printf(0, "Ejecutando test... 1");
  delay;
  jp_1();
  printf(0, ", 2");
  jp_2();
  printf(0, ", 3");
  jp_3();
  printf(0, ", 4");
  jp_4();
  printf(0, ", fin.");
  while(1) { 
  }
}
