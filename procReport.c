#include<linux/module.h>
#include<linux/sched/signal.h>
#include<linux/pid_namespace.h>
#include<asm/io.h>

int proc_count(void){
  int i=0;
  struct task_struct *thechild;
  for_each_process(thechild)
    i++;
  return i;
}

int total_pages_calc(struct task_struct *thechild) {
  // DEFINE total_pages 0
  // DEFINE start as vm_start
  // DEFINE end as vm_end
  // for i from start to end
  //  if (virt2phys(i) != 0)
  //   total_pages++;
  // return total_pages

  return 0;
}

void proc_print(void) {
  // code
  struct task_struct *thechild;
  printk(KERN_INFO "PROCESS REPORT:");
  printk(KERN_INFO "proc_id, proc_name, contig_pages, noncontig_pages, total_pages");
  for_each_process(thechild){
    if (thechild->pid > 650) {
      // printk("%d, %s", thechild->pid, thechild->comm);
      printk("%d, %s, %d", thechild->pid, thechild->comm,
        total_pages_calc(thechild));
    }
  }
  printk(KERN_INFO "TOTAL,,%d,%d,%d", 0, 0, 0);

}

int proc_init (void) {
  printk(KERN_INFO "helloModule: kernel module initialized\n");
  printk(KERN_INFO "There are %d running processes.\n", proc_count());
  proc_print();
  return 0;
}

void proc_cleanup(void) {
  printk(KERN_INFO "helloModule: performing cleanup of module\n");
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);
