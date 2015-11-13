/*
 *  @ fsl_bsc913x_ipc_kern_mod.c
 *
 * Copyright (c) 2011-2013, Freescale Semiconductor Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Freescale Semiconductor Inc nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      Author: Manish Jaggi <manish.jaggi@freescale.com>
 *      Author: Ashish Kumar <ashish.kumar@freescale.com>
 *
 */
#include "fsl_types.h"
#include "fsl_het_mgr.h"
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <asm/uaccess.h>
#include "bsc913x_heterogeneous.h"

#define	PA_CCSRBAR	0xff700000
#define PA_CCSR_SZ	0x00100000
#define DSP_CCSRBAR	0xff600000
#define DSP_CCSR_SZ	0x00100000
#define DSP_CORE0_M2	0xb0000000
#define DSP_CORE1_M2	0xb1000000
#define DSP_M3		0xc0000000
#define DSP_CORE0_M2_SZ	(512*1024)
#define DSP_CORE1_M2_SZ	(512*1024)
#define DSP_M3_SZ	(32*1024)
#define HW_SEM_OFFSET	0x17100

#define	DSP_SHARED_SZ			0x1000000
#define	SHARED_CTRL_AREA_START_ADDR	0x37000000
#define	SHARED_CTRL_AREA_SZ		0x1000000
#define	DSP_PVT_START_ADDR		0x38000000
#define	DSP_PVT_SZ			0x8000000

#define  SETUP_BOOTARG(A) 	\
		static char *A; \
		static long get_##A(void)\
		{		\
			unsigned long val;\
			if (!A || \
				(strict_strtoul(A, 0, &val) < 0))\
				return -EINVAL;\
			return val;\
			\
		}	\
		\
		static int __init get_from_cmdline_##A(char *str) \
		{\
			if (!str) \
				return 0; \
			A = str; \
			return 1;  \
		} \
		\
		__setup(#A"=", get_from_cmdline_##A);

#define GETBA(A, B) do {	\
			int rval = B;	\
			if (rval == -EINVAL)	\
				A = 0xffffffff;	\
			else	\
				A = rval;	\
			} while (0);


int 			fslhet_devno;
dev_t 			fslhet_dev;
struct cdev 		fslhet_cdev;
uint32_t 		het_mgr_major;
uint32_t 		het_mgr_minor;
sys_map_t 		sys_map;

int 			shmid;
volatile void		*sem;
shared_area_t 		shared_area;
phys_addr_t 		sh_ctrl_area_mark;
os_het_control_t	*ctrl;

SETUP_BOOTARG(dsp_shared_size);
SETUP_BOOTARG(dsp_private_addr);
SETUP_BOOTARG(dsp_private_size);
SETUP_BOOTARG(shared_ctrl_addr);
SETUP_BOOTARG(shared_ctrl_size);
#ifdef CONFIG_MULTI_RAT
#define MAX_NUM_IPC_REGIONS 2
#endif
/*
SETUP_BOOTARG(pa_shared_size);
*/
static int het_mgr_open(struct inode *inode, struct file *filep);
static int het_mgr_release(struct inode *inode, struct file *filep);
static int het_mgr_ioctl(struct inode *inode, struct file *filp,
unsigned int cmd, unsigned long arg);

#ifdef CONFIG_MULTI_RAT
uint32_t get_hetmgr_rat_instances(void)
{
	return ctrl->num_ipc_regions;
}
EXPORT_SYMBOL(get_hetmgr_rat_instances);
#endif


int init_sh_ctrl_area(void)
{
	int ctr = 0;
	uint32_t tmp;
	os_het_debug_print_t      *het_debug_print_v;
	os_het_debug_print_sc_t   *het_debug_print_sc_array_v;

	pr_info("Initializing Shared control area\n");
	pr_info("Shared Control area start address = %x\n",
			sys_map.sh_ctrl_area.phys_addr);

	ctrl = (os_het_control_t *)ioremap(
		sys_map.sh_ctrl_area.phys_addr, sys_map.sh_ctrl_area.size);
	if (!ctrl) {
		printk(KERN_ERR"ioremap failed with addr=%x val=%x\n",
		 sys_map.sh_ctrl_area.phys_addr, sys_map.sh_ctrl_area.size);
		 return -1;
	}
	memset((void *)ctrl, 0, sys_map.sh_ctrl_area.size);

	/* zeroize the structure */
	ctr += sizeof(os_het_control_t);

	ctrl->shared_ctrl_size = 0x4000; /* 16k */
	/* set the physical adress for ipc */
	/* casting is done just to avoid compilation errors */
	ctrl->ipc =  (void *)(sys_map.sh_ctrl_area.phys_addr + ctr);
	pr_info("IPC structure start address = %x\n",
			sys_map.sh_ctrl_area.phys_addr + ctr);
#ifndef CONFIG_MULTI_RAT
	ctr += sizeof(os_het_ipc_t);
#else
	ctrl->num_ipc_regions =  MAX_NUM_IPC_REGIONS;
	ctr += ctrl->num_ipc_regions * sizeof(os_het_ipc_t);
#endif
	tmp = sys_map.sh_ctrl_area.phys_addr + ctr;

	memcpy((void *)&ctrl->smartdsp_debug, &tmp, sizeof(uint32_t));
	pr_info("Smart DSP Debug start address = %x\n", tmp);

	ctr += sizeof(os_het_smartdsp_log_t) * 2;

	/*debug_print initilization start*/

	tmp = sys_map.sh_ctrl_area.phys_addr + ctr;
	ctrl->het_debug_print = (os_het_debug_print_t *)tmp;

	het_debug_print_v =  (os_het_debug_print_t *) ((uint32_t) ctrl + ctr);

	ctr += sizeof(os_het_debug_print_t);

	het_debug_print_v->sc_array_size = NUM_OF_DBGP_SC_CORES*
						NUM_OF_DBGP_TABLES_PER_SC;

	tmp = sys_map.sh_ctrl_area.phys_addr + ctr;
	het_debug_print_v->sc_debug_print = (void *)tmp;

	het_debug_print_sc_array_v =
	(os_het_debug_print_sc_t *) ((uint32_t) ctrl + ctr);

	memset((void *)het_debug_print_sc_array_v,
			0,
			het_debug_print_v->sc_array_size*
			sizeof(os_het_debug_print_sc_t));

	ctr += (het_debug_print_v->sc_array_size*
	sizeof(os_het_debug_print_sc_t));

	/*debug_print initilization end*/

	sh_ctrl_area_mark = sys_map.sh_ctrl_area.phys_addr + ctr;
	pr_info("Free Area starts from %x\n", sh_ctrl_area_mark);

	return 0;
}

int init_hw_sem(void)
{
	sem = ioremap(sys_map.dsp_ccsrbar.phys_addr + HW_SEM_OFFSET, 0x100);
	if (sem == (void *)0xffffffff) {
		pr_err("Error in ioremap %x\n", (uint32_t)sem);
		return -1;
	}

	pr_info("virt addr = %x\n", (uint32_t)sem);

	return 0;
}

int hugev2p(range_t *r)
{
	int ret = 0;
	struct task_struct *tsk;
	struct mm_struct *mm;
	unsigned long start;
	int len;
	int write;
	int force;
	struct page *pages = NULL;
	struct page *pg;
	struct vm_area_struct *vmas = NULL;
	int npages;

	tsk = current;
	mm = current->mm;
	start = (unsigned long)r->vaddr;
	len = 1;
	write = 0;
	force = 0;

	down_read(&current->mm->mmap_sem);
	ret = get_user_pages(tsk, mm, start, len, write, force, &pages, &vmas);
	up_read(&current->mm->mmap_sem);
	if (ret > 0) {
		npages = ret;
		pg = &pages[0];
		pr_info("%x Addr\n", page_to_phys(pg));
		r->phys_addr = page_to_phys(pg);
		put_page(pg);
	}

	return 0;
}

int get_het_sys_map(sys_map_t *sysmap)
{
	memset(sysmap, 0, sizeof(sys_map_t));
	/* GETBA(sysmap->pa_shared_size,get_pa_shared_size()); */
	GETBA(sysmap->dsp_shared_size, get_dsp_shared_size());
	if (sysmap->dsp_shared_size == 0xffffffff)
		sysmap->dsp_shared_size = DSP_SHARED_SZ;

	pr_info("IPC: dsp_shared_size - %#x\n", sysmap->dsp_shared_size);

	sysmap->pa_ccsrbar.phys_addr = PA_CCSRBAR;
	sysmap->pa_ccsrbar.size = PA_CCSR_SZ;

	sysmap->dsp_ccsrbar.phys_addr = DSP_CCSRBAR;
	sysmap->dsp_ccsrbar.size = DSP_CCSR_SZ;

	GETBA(sysmap->smart_dsp_os_priv_area.phys_addr,
					 get_dsp_private_addr());
	if (sysmap->smart_dsp_os_priv_area.phys_addr == 0xffffffff)
		sysmap->smart_dsp_os_priv_area.phys_addr = DSP_PVT_START_ADDR;

	pr_info("IPC: dsp_private_addr - %#x\n",
		sysmap->smart_dsp_os_priv_area.phys_addr);

	GETBA(sysmap->smart_dsp_os_priv_area.size, get_dsp_private_size());
	if (sysmap->smart_dsp_os_priv_area.size == 0xffffffff)
		sysmap->smart_dsp_os_priv_area.size = DSP_PVT_SZ;

	pr_info("IPC: dsp_private_size - %#x\n",
		sysmap->smart_dsp_os_priv_area.size);

	GETBA(sysmap->sh_ctrl_area.phys_addr, get_shared_ctrl_addr());
	if (sysmap->sh_ctrl_area.phys_addr == 0xffffffff)
		sysmap->sh_ctrl_area.phys_addr = SHARED_CTRL_AREA_START_ADDR;

	pr_info("IPC: shared_ctrl_addr - %#x\n",
		sysmap->sh_ctrl_area.phys_addr);

	GETBA(sysmap->sh_ctrl_area.size, get_shared_ctrl_size());
	if (sysmap->sh_ctrl_area.size == 0xffffffff)
		sysmap->sh_ctrl_area.size = SHARED_CTRL_AREA_SZ;

	pr_info("IPC: shared_ctrl_size - %#x\n", sysmap->sh_ctrl_area.size);

	sysmap->dsp_core0_m2.phys_addr = DSP_CORE0_M2;
	sysmap->dsp_core0_m2.size = DSP_CORE0_M2_SZ;

	if (of_find_compatible_node(NULL, NULL, "fsl,bsc9132-immr")) {
		sysmap->dsp_core1_m2.phys_addr = DSP_CORE1_M2;
		sysmap->dsp_core1_m2.size = DSP_CORE1_M2_SZ;
		sysmap->dsp_m3.phys_addr = DSP_M3;
		sysmap->dsp_m3.size = DSP_M3_SZ;
	}

	return 0;
}

#ifndef CONFIG_MULTI_RAT
int get_hetmgr_ipc_addr(range_t *r)
#else
int get_hetmgr_ipc_addr(range_t *r, int inst_id)
#endif
{
	r->phys_addr = (phys_addr_t)ctrl->ipc;
#ifdef CONFIG_MULTI_RAT
	r->phys_addr += inst_id*sizeof(os_het_ipc_t);
#endif
	r->vaddr = (void *)ctrl + ((uint32_t)ctrl->ipc -
#ifndef CONFIG_MULTI_RAT
	sys_map.sh_ctrl_area.phys_addr);
#else
	sys_map.sh_ctrl_area.phys_addr) +
	sizeof(os_het_ipc_t)*inst_id;
#endif
	r->size = sizeof(os_het_ipc_t);

	return 0;
}
EXPORT_SYMBOL(get_hetmgr_ipc_addr);

int get_hetmgr_mem(range_t *r)
{
	r->phys_addr = sh_ctrl_area_mark;
	r->vaddr = (void *)ctrl + (sh_ctrl_area_mark -
			sys_map.sh_ctrl_area.phys_addr);
	sh_ctrl_area_mark += r->size;
	if (sh_ctrl_area_mark > ctrl->shared_ctrl_size)
		ctrl->shared_ctrl_size *= 4;
	return 0;
}
EXPORT_SYMBOL(get_hetmgr_mem);

static int het_mgr_open(struct inode *inode, struct file *filep)
{
	return 0;
}

static int het_mgr_release(struct inode *inode, struct file *filep)
{
	return 0;
}

static int het_mgr_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	sys_map_t *tmp_sys_map;
	range_t	*tmp;
	range_t r;
	hw_sem_info_t	hinfo;
	hw_sem_t	hwsem;

	switch (cmd) {

	case IOCTL_HET_MGR_GET_SYS_MAP:
		tmp_sys_map = (sys_map_t *)arg;
		copy_to_user(tmp_sys_map, &sys_map, sizeof(sys_map_t));
		break;

	case IOCTL_HET_MGR_V2P:
		tmp = (range_t *)arg;
		copy_from_user(&r, tmp, sizeof(range_t));
		pr_info("Virt Address=%x Size=%x\n", (uint32_t)r.vaddr, r.size);
		ret = hugev2p(&r);
		copy_to_user(tmp, &r, sizeof(range_t));
		break;

	case IOCTL_HET_MGR_SET_SHMID:
		copy_from_user(&shmid, (uint32_t *)arg, sizeof(uint32_t));
		break;

	case IOCTL_HET_MGR_GET_SHMID:
		copy_to_user((uint32_t *)arg, &shmid, sizeof(uint32_t));
		break;

	case IOCTL_HET_MGR_SET_SHARED_AREA:
		copy_from_user(&shared_area, (shared_area_t *)arg,
				sizeof(shared_area_t));
		ctrl->pa_shared_mem.start_addr =
			shared_area.pa_ipc_shared.phys_addr;
		ctrl->pa_shared_mem.size = shared_area.pa_ipc_shared.size;
		ctrl->sc_shared_mem.start_addr =
			shared_area.dsp_ipc_shared.phys_addr;
		ctrl->sc_shared_mem.size = shared_area.dsp_ipc_shared.size;
		break;

	case IOCTL_HET_MGR_SET_INITIALIZED:
		#ifdef CONFIG_MULTI_RAT
		ctrl->initialized.pa_initialized = OS_HET_INITIALIZED_MULTIMODE;
		#else
		ctrl->initialized.pa_initialized = OS_HET_INITIALIZED;
		#endif
		ctrl->initialized.sc_initialized = OS_HET_UNINITIALIZED;
		break;

	case IOCTL_HW_SEM_GET_UVALUE:
		hinfo.pa_uniq_val = OS_HET_PA_SEMAPHORE_VAL;
		hinfo.dsp_uniq_val = OS_HET_SC_SEMAPHORE_VAL;
		copy_to_user((void *)arg, &hinfo, sizeof(hw_sem_info_t));
		break;

	case IOCTL_HW_SEM_SET_VALUE:
		copy_from_user(&hwsem, (void *)arg, sizeof(hw_sem_t));
		*(uint32_t *)(sem + hwsem.sem_no*0x8) = hwsem.value ;
		break;

	case IOCTL_HW_SEM_GET_VALUE:
		copy_from_user(&hwsem, (void *)arg, sizeof(hw_sem_t));
		hwsem.value = *(uint32_t *)(sem + hwsem.sem_no*0x8) ;
		copy_to_user((void *)arg, &hwsem, sizeof(hw_sem_t));
		break;

	default:
		return -ENOTTY;
	}

	return ret;
}
/*
file operations data structure
*/
static const struct file_operations het_mgr_fops = {
	.owner	= THIS_MODULE,
	.open = 	het_mgr_open,
	.release =	het_mgr_release,
	.ioctl =  	het_mgr_ioctl,
};

int het_mgr_init(void)
{
	int ret;
	het_mgr_major = 0;
	het_mgr_minor = 0;
	fslhet_dev = 0;
	/*register /dev/het_mgr character driver */
	if (het_mgr_major) {
		fslhet_dev = MKDEV(het_mgr_major, het_mgr_minor);
		ret = register_chrdev_region(fslhet_dev, 1, "/dev/het_mgr");

	} else {
		ret = alloc_chrdev_region(&fslhet_dev, het_mgr_minor,
				1, "/dev/het_mgr");
		het_mgr_major = MAJOR(fslhet_dev);
	}

	if (ret < 0) {
		pr_err("het_mgr_dev: can't get major %d\n", het_mgr_major);
		return ret;
	}

	fslhet_devno = MKDEV(het_mgr_major, het_mgr_minor);
	pr_info("Het Mgr %d %d\n", het_mgr_major, het_mgr_minor);
	cdev_init(&fslhet_cdev, &het_mgr_fops);
	fslhet_cdev.owner = THIS_MODULE;
	fslhet_cdev.ops = &het_mgr_fops;
	ret = cdev_add(&fslhet_cdev, fslhet_devno, 1);

	/* Fail gracefully if need be */
	if (ret) {
		pr_err("Error %d adding Heterogeneous System Manager", ret);
		return ret;
	}

	ret = get_het_sys_map(&sys_map);
	if (ret)
		goto end;

	ret = init_sh_ctrl_area();
	if (ret)
		goto end;

	ret = init_hw_sem();
	if (ret)
		goto end;

end:
	return ret;
}

void het_mgr_exit(void)
{
	cdev_del(&fslhet_cdev);
	unregister_chrdev_region(fslhet_dev, 1);
}

MODULE_AUTHOR("manish.jaggi@freescale.com");
MODULE_DESCRIPTION("Heterogeneous System Manager driver");
MODULE_LICENSE("GPL");

module_init(het_mgr_init);
module_exit(het_mgr_exit);
