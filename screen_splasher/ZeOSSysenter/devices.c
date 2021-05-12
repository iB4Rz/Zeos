#include <io.h>
#include <utils.h>
#include <list.h>
#include <sched.h>

// Queue for blocked processes in I/O 
struct list_head blocked;

void scroll(struct Screen *ns) {
  for (int i = 2; i < 25; ++i) {
    for (int j = 0; j < 80; ++j) {
      ns->display[i-1][j] = ns->display[i][j];
    }
  }
  for (int j = 0; j < 80; ++j) {
    ns->display[24][j] = ns->display[0][79]; 
  }
}

int sys_write_console(int fd, char *buffer, int size)
{
  struct Screen *ns = (struct Screen*) vscreens[current()->screens[fd]];
  Word w = (Word) ns->current_format << 8;

  int x = ns->x;
  int y = ns->y;

  for (int i = 0; i < size; i++) {
    Word nw = (Word) (buffer[i] & 0x00FF) | w;
    ns->display[x][y] = nw;
    if (++y > 79) { 
      ++x; y = 0; 
      if (x > 24) {
        scroll(ns);
        x = 24;
      }
    }
  }
  ns->x = x;
  ns->y = y;

  return size;
}

