/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 1
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;
struct list_head freequeue;
struct list_head readyqueue;
struct task_struct *idle_task;
void writeMSR();
void task_switch(union task_union*t);
void get_ebp(int* new);

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{
  struct list_head *first = list_first(&freequeue);
  list_del(first);
  struct task_struct *idle = list_head_to_task_struct(first);
  idle -> PID = 0;
  allocate_DIR(idle);
  union task_union *uidle = (union task_union*) idle;
  uidle -> stack[KERNEL_STACK_SIZE-1] = (unsigned long)cpu_idle;
  uidle -> stack[KERNEL_STACK_SIZE-2] = 0;
  idle -> dir = (int *)&uidle -> stack[KERNEL_STACK_SIZE-2];

  idle_task = idle;
}

void init_task1(void)
{
  struct list_head *first = list_first(&freequeue);
  list_del(first);
  struct task_struct *init = list_head_to_task_struct(first);
  init-> PID = 1;
  allocate_DIR(init);
  set_user_pages(init);
  union task_union *uinit = (union task_union*) uinit;
  tss.esp0 = (DWord)&uinit -> stack[KERNEL_STACK_SIZE];
  writeMSR(tss.esp0, 0x175);
  set_cr3(get_DIR(init));

}


void init_sched()
{
  INIT_LIST_HEAD(&freequeue);
  for (int i = 0; i < NR_TASKS; ++i) {
    list_add_tail(&(task[i].task.list), &freequeue);
  }
  INIT_LIST_HEAD(&readyqueue);

}

void inner_task_switch(union task_union*t) {
  tss.esp0 = (DWord)&t -> stack[KERNEL_STACK_SIZE];
  writeMSR(tss.esp0, 0x175);
  set_cr3(get_DIR(t->task));
  current()-> kernel_esp = (int) get_ebp();
  t
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

