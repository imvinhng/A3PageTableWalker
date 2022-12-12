#include<linux/module.h>
#include<linux/sched/signal.h>
#include<linux/pid_namespace.h>
#include<asm/io.h>

long virt2phys(struct mm_struct *mm, long vpage) {
  long physical_page_addr = 0;
  pgd_t *pgd;
  p4d_t *p4d;
  pud_t *pud;
  pmd_t *pmd;
  pte_t *pte;
  struct page *page;
  pgd = pgd_offset(mm, vpage);
  if (pgd_none(*pgd) || pgd_bad(*pgd))
    return 0;
  p4d = p4d_offset(pgd, vpage);
  if (p4d_none(*p4d) || p4d_bad(*p4d))
    return 0;
  pud = pud_offset(p4d, vpage);
  if (pud_none(*pud) || pud_bad(*pud))
    return 0;
  pmd = pmd_offset(pud, vpage);
  if (pmd_none(*pmd) || pmd_bad(*pmd))
    return 0;
  if (!(pte = pte_offset_map(pmd, vpage)))
    return 0;
  if (!(page = pte_page(*pte)))
    return 0;
  physical_page_addr = page_to_phys(page);
  pte_unmap(pte);
  if (physical_page_addr == 70368744173568)
    return 0;
  return physical_page_addr;
}

// void cont_pages(struct task_struct *task) {
//   long current_page
// }


int proc_count(void){
  int i=0;
  struct task_struct *thechild;
  for_each_process(thechild)
    i++;
  return i;
}

int total_pages_calc(struct task_struct *task) {
  // DEFINE total_pages 0
  // DEFINE start as vm_start
  // DEFINE end as vm_end
  // for i from start to end
  //  if (virt2phys(i) != 0)
  //   total_pages++;
  // return total_pages
  int totalPageCount = 0;
  struct vm_area_struct *vma = 0;
  unsigned long vpage;
  if (task->mm && task->mm->mmap)
    for (vma = task->mm->mmap; vma; vma = vma->vm_next)
      for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE)
        if (virt2phys(task->mm, vpage) != 0)
          totalPageCount++;
  return totalPageCount;
}

void proc_print(void) {
  // code
  struct task_struct *thechild;
  printk(KERN_INFO "PROCESS REPORT:");
  printk(KERN_INFO "proc_id, proc_name, contig_pages, noncontig_pages, total_pages");
  for_each_process(thechild){
    if (thechild->pid > 650) {
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
