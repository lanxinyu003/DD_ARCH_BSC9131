/*
 *  @ fsl_bsc913x_ipc_kern_mod.c
 *
 * Copyright (c) 2011-2013, Freescale Semiconductor, Inc.
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
 */
#include "fsl_types.h"
#include "fsl_het_mgr.h"
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <asm/fsl_msg.h>
#include "bsc913x_heterogeneous.h"
#include "bsc913x_heterogeneous_ipc.h"
#include "fsl_ipc_kmod.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/errno.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/fcntl.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/io.h>

#define MAX_MSG_IRQ	4

/*
Data Structure to store the callback handlers for channels
*/
typedef struct {
	int 		sig;
	spinlock_t	lock;
	struct	pid	*pid;
	struct	siginfo siginfo;
} user_irq_t;

/* IRQ - MSG related */
int msg_irq_cnt=0;
user_irq_t	uirq[MAX_MSG_IRQ];
user_irq_t user_irq_arr[MAX_SC_PA_CHANNELS];
struct fsl_msg_unit *ipc_msg_intr[MAX_MSG_IRQ];

int fslipc_devno;
dev_t fslipc_dev;
struct cdev fslipc_cdev;
uint32_t het_ipc_major;
uint32_t het_ipc_minor;

/*IPC */
os_het_ipc_channel_t *ch;
os_het_ipc_t *ipc;

/* From bootargs */
int num_channels, channel_depth;
static char *max_num_ipc_channels;
static char *max_channel_depth;

#ifdef CONFIG_MULTI_RAT
int get_hetmgr_ipc_addr(range_t *, int inst_id);
#else
int get_hetmgr_ipc_addr(range_t *);
#endif
int get_hetmgr_mem(range_t *);

void open_channel_zero(os_het_ipc_channel_t *);
extern struct fsl_msg_unit *fsl_get_msg_unit(void);
extern uint32_t get_hetmgr_rat_instances(void);


static long get_max_num_ipc_channels(void)
{
	unsigned long val;
	if (!max_num_ipc_channels || (strict_strtoul(max_num_ipc_channels,
				0, &val) < 0))
		return -EINVAL;

	return val;
}

static long get_max_channel_depth(void)
{
	unsigned long val;

	if (!max_channel_depth || (strict_strtoul(max_channel_depth,
				0, &val) < 0))
		return -EINVAL;

	return val;
}

static int __init get_max_num_ipc_channels_from_cmdline(char *str)
{
	if (!str)
		return 0;
	pr_err("%s\n", str);
	max_num_ipc_channels = str;

	return 1;
}

static int __init get_max_channel_depth_from_cmdline(char *str)
{
	if (!str)
		return 0;

	pr_err("%s\n", str);
	max_channel_depth = str;

	return 1;
}

__setup("max_num_ipc_channels=", get_max_num_ipc_channels_from_cmdline);
__setup("max_channel_depth=", get_max_channel_depth_from_cmdline);

/*******************************
 * Interrupt Handlers
 ******************************/
static irqreturn_t msg_intr(int irq, void *dev_id)
{
	int i;
	struct task_struct *dst_tsk;
	pr_info("Recvd %x intr\n", irq);
	for (i = 0; i < MAX_MSG_IRQ; i++) {
		if (ipc_msg_intr[i]->irq == irq) {
			fsl_clear_msg(ipc_msg_intr[i]);
			/* push the signal */
			dst_tsk = pid_task(uirq[i].pid, PIDTYPE_PID);
			if (dst_tsk) {
				send_sig_info(uirq[i].siginfo.si_signo,
						&uirq[i].siginfo, dst_tsk);
			}
		}
	}

	return IRQ_HANDLED;
}

int register_msg_intr(void)
{
	int err, i;
	struct fsl_msg_unit *msg;

	for (i = 0; i < MAX_MSG_IRQ; i++) {
		msg = fsl_get_msg_unit();
		if (IS_ERR(msg)) {
			printk(KERN_ERR"Error in fsl_get_msg_uint\n");
			return (int)msg;
		}
		pr_info("msg irq %x msg num %x\n", msg->irq,
			msg->msg_num);
		fsl_enable_msg(msg);
		ipc_msg_intr[i] = msg;
		err = request_irq(msg->irq, msg_intr, IRQF_NODELAY,
			"IPC_MSG", NULL);
		pr_info("request_irq err = %x\n", err);
	}
	return 0;
}

/*
 * @fsl_913xipc_init
 *
 *This method is called by het_ipc to initialize ipc ptr channels
 *It is assumed that the caller (het_ipc) has mapped ipc structre
 *already at this address
*/
#ifndef CONFIG_MULTI_RAT
int fsl_913xipc_init(void)
{
	int ret = 0;
	int i = 0;
	range_t r, r1;
	phys_addr_t phys_addr, phys_addr1;

	ret = get_hetmgr_ipc_addr(&r);
	if (ret)
		goto end;
	ipc = r.vaddr;
	pr_err("os_het_ipc_t phys=%x vaddr=%x \n", (uint32_t)r.phys_addr,
			(uint32_t)r.vaddr);
	num_channels = get_max_num_ipc_channels();
	if (num_channels < 0 || num_channels > 64) {
		num_channels = MAX_IPC_CHANNELS;
		printk(KERN_ERR"warning: max_num_ipc_channels not set properly,\
		setting default value = %d\n", num_channels);
	}

	channel_depth = get_max_channel_depth();
	if (channel_depth < 0) {
		channel_depth = DEFAULT_CHANNEL_DEPTH;
		printk(KERN_ERR"warning: max_channel_depth not set\
		setting default value = %d\n", channel_depth);
	}

	r1.size = sizeof(os_het_ipc_channel_t)*num_channels +
		/* array to hold channel pointers */
		sizeof(os_het_ipc_bd_t)*num_channels*channel_depth;
		/* ptr channel ring buffer */

	ret = get_hetmgr_mem(&r1);
	if (ret)
		goto end;

	memcpy(&ipc->ipc_channels, &r1.phys_addr, sizeof(phys_addr_t));
	ipc->num_ipc_channels = num_channels;
	ipc->ipc_max_bd_size = channel_depth;
	phys_addr = r1.phys_addr;
	ch = (os_het_ipc_channel_t *)(r1.vaddr);
	phys_addr1 = phys_addr + sizeof(os_het_ipc_channel_t)*num_channels;

	/*
		In a loop of num_channels, set the ptr of channel structures
		in ipc->channels
	*/

	for (i = 0; i < num_channels; i++) {
		ch[i].ipc_ind = OS_HET_NO_INT;
		ch[i].id = i;
		memcpy(&ch[i].bd_base, &phys_addr1, sizeof(phys_addr_t));
		phys_addr1 += sizeof(os_het_ipc_bd_t)*channel_depth;
	}
	ret = register_msg_intr();
	if (ret)
		goto end;
	open_channel_zero(ch);
end:
	return ret;
}
#else
int fsl_913xipc_init(void)
{
	int ret = 0;
	int i = 0;
	int j = 0;
	uint32_t rat_inst = 0;
	range_t r, r1;
	phys_addr_t phys_addr, phys_addr1;

	num_channels = get_max_num_ipc_channels();
	if (num_channels < 0 || num_channels > 64) {
		num_channels = MAX_IPC_CHANNELS;
		printk(KERN_ERR"warning: max_num_ipc_channels not set properly,\
		setting default value = %d\n", num_channels);
	}

	channel_depth = get_max_channel_depth();
	if (channel_depth < 0) {
		channel_depth = DEFAULT_CHANNEL_DEPTH;
		printk(KERN_ERR"warning: max_channel_depth not set\
		setting default value = %d\n", channel_depth);
	}

	rat_inst = get_hetmgr_rat_instances();
	if (rat_inst <= 1) {
		rat_inst = 2;
		printk(KERN_ERR"warning: max_rat_inst not set\
		setting default value = %d\n", rat_inst);
	}



	for (j = 0; j < rat_inst; j++) {

		ret = get_hetmgr_ipc_addr(&r, j);
		if (ret)
			goto end;

		ipc = r.vaddr;

		r1.size = sizeof(os_het_ipc_channel_t)*num_channels +
				/* array to hold channel pointers */
				sizeof(os_het_ipc_bd_t)*
				num_channels*
				channel_depth;

		/* ptr channel ring buffer */
		ret = get_hetmgr_mem(&r1);
		if (ret)
			goto end;
		memcpy(&ipc->ipc_channels, &r1.phys_addr,
				sizeof(phys_addr_t));
		ipc->num_ipc_channels = num_channels;
		ipc->ipc_max_bd_size = channel_depth;

		phys_addr = r1.phys_addr;
		ch = (os_het_ipc_channel_t *)(r1.vaddr);
		phys_addr1 = phys_addr +
				sizeof(os_het_ipc_channel_t)*num_channels;

		/*
			In a loop of num_channels, set the ptr of channel
			structures
			in ipc->channels
		*/

		for (i = 0; i < num_channels; i++) {
			ch[i].ipc_ind = OS_HET_NO_INT;
			ch[i].id = i;
			memcpy(&ch[i].bd_base, &phys_addr1,
				sizeof(phys_addr_t));
			phys_addr1 += sizeof(os_het_ipc_bd_t)*channel_depth;
		}

		open_channel_zero(ch);
	}
end:
	return ret;
}
#endif

void open_channel_zero(os_het_ipc_channel_t *ch)
{
	ch[0].consumer_initialized = OS_HET_INITIALIZED;
	ch[0].id = 0;
	ch[0].bd_ring_size = channel_depth;
	ch[0].ch_type = OS_HET_IPC_POINTER_CH;
}


int het_ipc_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg)
{
	unsigned long flags;
	int ret = 0;
	ipc_rc_t r;
	switch (cmd) {

	case IOCTL_IPC_REGISTER_SIGNAL:
		copy_from_user(&r, (void *)arg, sizeof(ipc_rc_t));
		if (msg_irq_cnt < MAX_MSG_IRQ) {
			spin_lock_irqsave(&uirq[msg_irq_cnt].lock, flags);

			uirq[msg_irq_cnt].pid = task_pid(current);
			uirq[msg_irq_cnt].siginfo.si_signo = r.signal;
			uirq[msg_irq_cnt].siginfo.si_errno = 0;
			uirq[msg_irq_cnt].siginfo.si_code = SI_QUEUE;

			spin_unlock_irqrestore(&uirq[msg_irq_cnt].lock, flags);

			/* update channel structure with msg */
			ch[r.channel_id].ipc_ind = OS_HET_PA_MPIC;
			ch[r.channel_id].ind_offset = msg_irq_cnt;
			ch[r.channel_id].ind_value = 0x12345678;

			msg_irq_cnt++;
		}
		break;

		default:  /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}

	return ret;
}

static int het_ipc_open(struct inode *inode, struct file *filep)
{
	return 0;
}

static int het_ipc_release(struct inode *inode, struct file *filep)
{
	return 0;
}

/*
file operations data structure
*/
static const struct file_operations het_ipc_fops = {
	.owner	= THIS_MODULE,
	.open = 	het_ipc_open,
	.release =	het_ipc_release,
	.ioctl = 	het_ipc_ioctl,
};


int ipc_driver_init(void)
{
	int ret;
	het_ipc_major = 0;
	het_ipc_minor = 0;
	fslipc_dev = 0;
	/*register /dev/het_ipc character driver */
	if (het_ipc_major) {
		fslipc_dev = MKDEV(het_ipc_major, het_ipc_minor);
		ret = register_chrdev_region(fslipc_dev, 1, "/dev/het_ipc");

	} else {
		ret = alloc_chrdev_region(&fslipc_dev, het_ipc_minor, 1,
				"/dev/het_ipc");
		het_ipc_major = MAJOR(fslipc_dev);
	}

	if (ret < 0) {
		pr_err("het_ipc_dev: can't get major %d\n", het_ipc_major);
		return ret;
	}

	fslipc_devno = MKDEV(het_ipc_major, het_ipc_minor);
	pr_info("Het Ipc %d %d\n", het_ipc_major, het_ipc_minor);
	cdev_init(&fslipc_cdev, &het_ipc_fops);
	fslipc_cdev.owner = THIS_MODULE;
	fslipc_cdev.ops = &het_ipc_fops;
	ret = cdev_add(&fslipc_cdev, fslipc_devno, 1);

	/* Fail gracefully if need be */
	if (ret)
		pr_err("Error %d adding Heterogeneous System Manager", ret);

	fsl_913x_ipc_init();

	return 0;
}

void ipc_driver_exit(void)
{
	cdev_del(&fslipc_cdev);
	unregister_chrdev_region(fslipc_dev, 1);
}

MODULE_AUTHOR("manish.jaggi@freescale.com");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("IPC kernel mode helper driver");
module_init(ipc_driver_init);
module_exit(ipc_driver_exit);
