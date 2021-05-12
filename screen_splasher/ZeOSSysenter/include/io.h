/*
 * io.h - Definició de l'entrada/sortida per pantalla en mode sistema
 */

#ifndef __IO_H__
#define __IO_H__

#include <types.h>

/** Screen functions **/
/**********************/

Byte inb (unsigned short port);
void printc(Word w);
void printc_xy(Byte x, Byte y, Word c);
void printk(char *string);

#endif  /* __IO_H__ */
