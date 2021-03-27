/*
 * libc.c 
 */

#include <libc.h>
#include <errno.h>

#include <types.h>

int errno;

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

void perror()
{
  switch (errno) {
    case EBADF:
      write(1, "<<ERROR>> Bad file number\n", 26);
    break;
    case EACCES:
      write(1, "<<ERROR>> Permission denied\n", 28);
    break;
    case EFAULT:
      write(1, "<<ERROR>> Bad adress\n", 21);
    break;
    case EINVAL:
      write(1, "<<ERROR>> Invalid argument\n", 27);
    break;
    default:
      write(1, "<<ERROR>> Unknown error\n", 24);
    break;
  }

  // falta ENOMEM

}
