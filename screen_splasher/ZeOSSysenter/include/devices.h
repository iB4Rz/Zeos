#ifndef DEVICES_H__
#define  DEVICES_H__

int sys_write_console(int fd, char *buffer,int size);
int trat_cursor(char *buffer, int i, int* x, int* y, int size);
void scroll();
#endif /* DEVICES_H__*/
