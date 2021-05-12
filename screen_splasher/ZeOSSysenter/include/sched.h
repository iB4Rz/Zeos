/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#include <list.h>
#include <types.h>
#include <mm_address.h>
#include <stats.h>


#define NR_TASKS        10
#define KERNEL_STACK_SIZE	1024
#define NR_PROC_SCREENS 10
#define NR_SCREENS      30


enum state_t { ST_RUN, ST_READY, ST_BLOCKED };

struct task_struct {
  int PID;			/* Process ID. This MUST be the first field of the struct. */
  page_table_entry * dir_pages_baseAddr;
  struct list_head list;	/* Task struct enqueuing */
  int register_esp;		/* position in the stack */
  enum state_t state;		/* State of the process */
  int total_quantum;		/* Total quantum of the process */
  struct stats p_stats;		/* Process stats */
  int screens[NR_PROC_SCREENS]; /* Screen id's vector */
};

union task_union {
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

struct Screen {
  Word display[25][80]; /* Matriu pantalla */
  int num_refs; /* Contador de procesos amb aquesta pantalla */
  int x,y;  /* Posició cursor */
  Byte current_format; /* Format actual */
  struct list_head list;  /* Screen enqueuing */
};

extern union task_union protected_tasks[NR_TASKS+2];
extern union task_union *task; /* Vector de tasques */
extern struct task_struct *idle_task;

extern struct Screen *vscreens[30]; /* Vector de pantalles */
extern struct list_head screen_queue;
extern struct Screen *focus;
void changeFocus();
void screenToolbar();


#define KERNEL_ESP(t)       	(DWord) &(t)->stack[KERNEL_STACK_SIZE]

#define INITIAL_ESP       	KERNEL_ESP(&task[1])

extern struct list_head freequeue;
extern struct list_head readyqueue;

/* Inicialitza les dades del proces inicial */
void init_task1(void);

void init_idle(void);

void init_sched(void);

void schedule(void);

struct task_struct * current();

void task_switch(union task_union*t);
void switch_stack(int * save_sp, int new_sp);

void sched_next_rr(void);

void force_task_switch(void);

struct task_struct *list_head_to_task_struct(struct list_head *l);
struct Screen* list_head_to_screen(struct list_head *l);

int allocate_DIR(struct task_struct *t);

page_table_entry * get_PT (struct task_struct *t) ;

page_table_entry * get_DIR (struct task_struct *t) ;

/* Headers for the scheduling policy */
void sched_next_rr();
void update_process_state_rr(struct task_struct *t, struct list_head *dest);
int needs_sched_rr();
void update_sched_data_rr();

void init_stats(struct stats *s);

#endif  /* __SCHED_H__ */
