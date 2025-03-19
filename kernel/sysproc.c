#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "date.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;


  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}


#ifdef LAB_PGTBL
int
sys_pgaccess(void)
{
  uint64 start_vm;
  int page_nums;
  uint64 user_addr;
  unsigned int bitmask = 0;
  struct proc* p = myproc();
  if(argaddr(0,&start_vm) < 0){
    return -1;
  }
  if(argint(1,&page_nums) < 0){
    return -1;
  }
  // bitmask = 1L << (page_nums-1);
  if(argaddr(2,&user_addr) < 0){
    return -1;
  }
  // printf("%d\n",page_nums);
  // printf("%p\n",start_vm);
  for(int i = 0;i<page_nums;i++){
    pte_t* pte = walk(p->pagetable,start_vm,0);
    if((*pte & PTE_A)){
      bitmask = bitmask | (1L << i);
      // printf("%p\n",PTE_A);
      *pte = (*pte) & (~PTE_A);
      // printf("%p\n",*pte);
    }
    start_vm += PGSIZE;

  }
  copyout(p->pagetable,user_addr,(char*)&bitmask,sizeof(int));
  return 0;

}
#endif

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
