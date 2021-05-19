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
    ns->display[24][j] = 0x0220; 
  }
}

int trat_cursor(char *buffer, int i, int* x, int* y, int size) {
  int first,second;
  if (buffer[i+4] == '>') first = 3;
  else if (size > i+5 && buffer[i+5] == '>') first = 4;
  else return 0;
  if (size > i+first+2 && buffer[i+2+first] == ';') {
    if (size > i+first+5 && buffer[i+5+first] == '>') second = 3;
    else if (size > i+first+6 && buffer[i+6+first] == '>') second = 4;
    else return 0;
    if (size <=i+first+second+3 || (buffer[i+first+second+3]!='H' && buffer[i+first+second+3]!='f')) return 0;
    if (first == 3 && (buffer[i+3] < '0' || buffer[i+3] > '9')) return 0;
    if (first == 4 && ((buffer[i+3] < '0' || buffer[i+3] > '2' ||
             buffer[i+4] < '0' || buffer[i+4] > '9') || (buffer[i+3] == '2' && buffer[i+4] > '4'))) return 0;
    if (second == 3 && (buffer[i+first+4] < '0' || buffer[i+first+4] > '9')) return 0;
    if (second == 4 && (buffer[i+4+first] < '0' || buffer[i+4+first] > '7' ||
             buffer[i+5+first] < '0' || buffer[i+5+first] > '9')) return 0;
    if (first == 3) *x = buffer[i+3] - '0';
    else *x = (buffer[i+3]-'0')*10 + buffer[i+4]-'0';  
    if (second == 3) *y = buffer[i+first+4] - '0';
    else *y = (buffer[i+4+first]-'0')*10 + buffer[i+5+first]-'0'; 
    return first+second+3;
  }
  return 0;
}

int sys_write_console(int fd, char *buffer, int size)
{
  struct Screen *ns = (struct Screen*) vscreens[current()->screens[fd]];

  int x = ns->x;
  int y = ns->y;

  for (int i = 0; i < size; i++) {
    char c = buffer[i];
    // Newline
    if (c == '\n') {
      x++; y = 0;
      if (x > 24) {
        scroll(ns);
        x = 24;
      }
    }
    // Delete character
    else if (c == '\b') {
      if (--y < 0) {
        --x; y = 79;
        if (x == 0) {
          x = 1; y = 0;
        }
      }
      ns->display[x][y] =  (ns->display[x][y] & 0x00) | ' ';
    }
    // Foreground & background
    else if (c == '\e') {
      if (i + 4 < size && buffer[i+1] == '[') {
        // Cursor
        if (buffer[i+2] == '<') i += trat_cursor(buffer,i,&x,&y,size);
        else if (buffer[i+2] >= '3' && buffer[i+2] < '5') {
          if (buffer[i+4] == 'm') {
            if (buffer[i+3] >= '0' && buffer[i+3] <= '9') {
              char nc = buffer[i+3];
              nc -= '0';
              if (buffer[i+2] == '3') { ns->current_format = (ns->current_format & 0xF0) | nc; i+= 4; }
              else if (buffer[i+3]<'8') {ns->current_format = (ns->current_format & 0x0F) | (nc << 4); i+= 4;}
            }
          }
          else if (i+5 < size && buffer[i+5] == 'm' && buffer[i+2] == '3') {
            if (buffer[i+3] == '1' && buffer[i+4] >= '0' && buffer[i+4] < '6') {
              char nc = buffer[i+4];
              nc -= '0';
              nc += 10;
              ns->current_format = (ns->current_format & 0xF0) | nc;
              i += 5;
            }
          }
        }
      }

    }
    else {
      Word nw = (Word) ns->current_format << 8;
      nw = (Word) (c & 0x00FF) | nw;
      ns->display[x][y] = nw;
      if (++y > 79) { 
        ++x; y = 0; 
        if (x > 24) {
          scroll(ns);
          x = 24;
        }
      }
    }
  }
  ns->x = x;
  ns->y = y;

  return size;
}