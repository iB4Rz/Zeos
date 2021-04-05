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

void printf(char *msg) {
  write(1,msg,strlen(msg));
}

void perror()
{
  switch (errno) {
    case ESRCH:
      printf("<<ERROR>> No such process\n");
    break;
    case EBADF:
      printf("<<ERROR>> Bad file number\n");
    break;
      case ENOMEM:
      printf("<<ERROR>> Not enough core\n");
    break;
    case EACCES:
      printf("<<ERROR>> Permission denied\n");
    break;
    case EFAULT:
      printf("<<ERROR>> Bad adress\n");
    break;
    case EINVAL:
      printf("<<ERROR>> Invalid argument\n");
    break;
    case ENOSYS:
      printf("<<ERROR>> Function not implemented\n");
    break;
    default:
      printf("<<ERROR>> Undefined error\n");
    break;
  }
}
