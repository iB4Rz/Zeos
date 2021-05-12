/*
 * io.c - 
 */

#include <io.h>

#include <types.h>

/**************/
/** Screen  ***/
/**************/

#define NUM_COLUMNS 80
#define NUM_ROWS    25

Byte x, y = 0;

/* Read a byte from 'port' */
Byte inb (unsigned short port)
{
  Byte v;

  __asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (port));
  return v;
}

void printc(Word w)
{
  char c = (char) (w & 0xFF);
  w = (Word) w & 0xFF00;
  __asm__ __volatile__ ( "movb %0, %%al; outb $0xe9" ::"a"(c));
  if (c=='\n')
  {
    x = 0;
    y=(y+1)%NUM_ROWS;
  }
  else
  {
    Word ch = (Word) (c & 0x00FF) | w;
    DWord screen = 0xb8000 + (y * NUM_COLUMNS + x) * 2;
    if (++x >= NUM_COLUMNS)
    {
      x = 0;
      y=(y+1)%NUM_ROWS;
    }
    asm("movw %0, (%1)" : : "g"(ch), "g"(screen));
  }
}

// mx, my por referencia
void printc_xy(Byte mx, Byte my, Word c)
{
  Byte cx, cy;
  cx=x;
  cy=y;
  x=my;
  y=mx;
  printc(c);
  x=cx;
  y=cy;
}

void printk(char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    printc((Word)(string[i]|0x0200));
}
