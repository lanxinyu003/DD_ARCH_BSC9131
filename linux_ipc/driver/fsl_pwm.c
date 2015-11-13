/*
 * Copyright 2011 Freescale Semiconductor, Inc.
 *
 * PWM (Pulse Width Modulator) controller driver
 *
 * Author: Chunhe Lan <Chunhe.Lan@freescale.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/pwm.h>
#include <linux/of_platform.h>
#include <sysdev/fsl_soc.h>
#include <asm/fsl_pwm.h>
#include <asm/clock.h>
#include <asm/prom.h>

static DEFINE_MUTEX(pwm_lock);
static LIST_HEAD(pwm_list);

static ssize_t show_duty_ns(struct device *dev,
			    struct device_attribute *attr,
			    char *buf)
{
	struct pwm_device *pwm = dev_get_drvdata(dev);

	return sprintf(buf, "%u\n", pwm->duty);
}

static ssize_t show_period_ns(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct pwm_device *pwm = dev_get_drvdata(dev);

	return sprintf(buf, "%u\n", pwm->period);
}

static ssize_t store_duty_ns(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t count)
{
	struct pwm_device *pwm = dev_get_drvdata(dev);
	unsigned long val;

	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;

	mutex_lock(&pwm_lock);

	pwm->duty = (int)val;
	if ((pwm->duty < pwm->period) || (pwm->duty == pwm->period)) {
		pwm_disable(pwm);
		pwm_config(pwm, pwm->duty, pwm->period);
		pwm_enable(pwm);
	}

	mutex_unlock(&pwm_lock);

	return count;
}

static ssize_t store_period_ns(struct device *dev,
			       struct device_attribute *attr,
			       const char *buf, size_t count)
{
	struct pwm_device *pwm = dev_get_drvdata(dev);
	unsigned long val;

	if (strict_strtoul(buf, 10, &val))
		return -EINVAL;

	mutex_lock(&pwm_lock);

	pwm->period = (int)val;
	if ((pwm->duty < pwm->period) || (pwm->duty == pwm->period)) {
		pwm_disable(pwm);
		pwm_config(pwm, pwm->duty, pwm->period);
		pwm_enable(pwm);
	}

	mutex_unlock(&pwm_lock);

	return count;
}

static DEVICE_ATTR(duty_ns, S_IRUGO | S_IWUSR,
		   show_duty_ns, store_duty_ns);
static DEVICE_ATTR(period_ns, S_IRUGO | S_IWUSR,
		   show_period_ns, store_period_ns);

static struct attribute *pwm_attrs[] = {
	&dev_attr_duty_ns.attr,
	&dev_attr_period_ns.attr,
	NULL
};

static const struct attribute_group pwm_group = {
	.attrs = pwm_attrs,
};

int pwm_config(struct pwm_device *pwm, int duty_ns, int period_ns)
{
	unsigned long long c;
	unsigned long period_cycles, duty_cycles, prescale;
	u32 cr;

	if (pwm == NULL || period_ns == 0 || duty_ns > period_ns)
		return -EINVAL;

	if (pwm->pwmo_invert)
		duty_ns = period_ns - duty_ns;

	c = clk_get_rate(pwm->clk);
	c = c * period_ns;
	do_div(c, 1000000000);
	period_cycles = c;

	prescale = period_cycles / 0x10000 + 1;
	if (prescale > FSL_MAX_PRESCALER)
		return -EINVAL;

	period_cycles /= prescale;
	c = (unsigned long long)period_cycles * duty_ns;
	do_div(c, period_ns);
	duty_cycles = c;

	out_be32(&pwm->regs->pwmsar, duty_cycles);
	out_be32(&pwm->regs->pwmpr, period_cycles);

	cr = FSL_PWMCR_POUTC_HIGHT | FSL_PWMCR_CLKSRC | FSL_PWMCR_DOZEEN |
		 FSL_PWMCR_WAITEN | FSL_PWMCR_DEBUGEN | FSL_PWMCR_STOPEN;
	cr |= FSL_PWMCR_PRESCALER(prescale);
	out_be32(&pwm->regs->pwmcr, cr);

	return 0;
}
EXPORT_SYMBOL(pwm_config);

int pwm_enable(struct pwm_device *pwm)
{
	unsigned int reg;
	int rc = 0;

	if (!pwm->clk_enabled) {
		rc = clk_enable(pwm->clk);
		if (!rc)
			pwm->clk_enabled = 1;
	}

	reg = in_be32(&pwm->regs->pwmcr);
	reg |= FSL_PWMCR_EN;
	out_be32(&pwm->regs->pwmcr, reg);

	if (pwm->enable_pwm_pad)
		pwm->enable_pwm_pad();

	return rc;
}
EXPORT_SYMBOL(pwm_enable);

void pwm_disable(struct pwm_device *pwm)
{
	unsigned int reg;

	if (pwm->disable_pwm_pad)
		pwm->disable_pwm_pad();

	reg = in_be32(&pwm->regs->pwmcr);
	reg &= ~FSL_PWMCR_EN;
	out_be32(&pwm->regs->pwmcr, reg);

	if (pwm->clk_enabled) {
		clk_disable(pwm->clk);
		pwm->clk_enabled = 0;
	}
}
EXPORT_SYMBOL(pwm_disable);

struct pwm_device *pwm_request(int pwm_id, const char *label)
{
	struct pwm_device *pwm;
	int found = 0;

	mutex_lock(&pwm_lock);

	list_for_each_entry(pwm, &pwm_list, node) {
		if (pwm->pwm_id == pwm_id) {
			found = 1;
			break;
		}
	}

	if (found) {
		if (pwm->use_count == 0) {
			pwm->use_count++;
			pwm->label = label;
		} else
			pwm = ERR_PTR(-EBUSY);
	} else
		pwm = ERR_PTR(-ENOENT);

	mutex_unlock(&pwm_lock);
	return pwm;
}
EXPORT_SYMBOL(pwm_request);

void pwm_free(struct pwm_device *pwm)
{
	mutex_lock(&pwm_lock);

	if (pwm->use_count) {
		pwm->use_count--;
		pwm->label = NULL;
	} else
		pr_warning("PWM device already freed\n");

	mutex_unlock(&pwm_lock);
}
EXPORT_SYMBOL(pwm_free);

static irqreturn_t fsl_pwm_irq(int irq, void *context_data)
{
	struct pwm_device *fsl_pwm = context_data;
	u32 status;

	/* Get interrupt events */
	status = in_be32(&fsl_pwm->regs->pwmsr);

	if (status) {
		if (status & FSL_PWMSR_FWE)
			dev_err(&fsl_pwm->dev, "FIFO write error occurred: "
				"PWMSR 0x%08X\n", status);
		if (status & FSL_PWMSR_CMP)
			dev_err(&fsl_pwm->dev, "Compare event occurred: "
				"PWMSR 0x%08X\n", status);
		if (status & FSL_PWMSR_ROV)
			dev_err(&fsl_pwm->dev, "Roll-over event occurred: "
				"PWMSR 0x%08X\n", status);

		if (status & ~FSL_PWMSR_ALL_MASK)
			dev_err(&fsl_pwm->dev, "Unknown error: "
				"PWMSR 0x%08X\n", status);

		/* Clear the events */
		out_be32(&fsl_pwm->regs->pwmsr, status &
				 FSL_PWMSR_FWE_CMP_ROV_MASK);
		return IRQ_HANDLED;
	}

	return IRQ_NONE;
}

static int __devinit fsl_pwm_probe(struct of_device *ofdev,
				   const struct of_device_id *match)
{
	struct device_node *np;
	struct gubr __iomem *gubr;
	struct pwm_device *fsl_pwm;
	struct resource res_mem;
	struct resource res_irq;
	struct resource *res = &res_mem;
	struct resource *irq = &res_irq;
	const u32 *id;
	int ret = 0;
	unsigned long rate;

	fsl_pwm = kzalloc(sizeof(struct pwm_device), GFP_KERNEL);
	if (fsl_pwm == NULL) {
		dev_err(&ofdev->dev, "failed to allocate memory\n");
		return -ENOMEM;
	}

	ret = of_address_to_resource(ofdev->node, 0, res);
	if (ret) {
		dev_err(&ofdev->dev, "invalid address\n");
		goto err_free;
	}

	if (!request_mem_region(res->start,
				res->end - res->start + 1, "fsl-pwm")) {
		dev_err(&ofdev->dev, "memory request failure\n");
		ret = -ENXIO;
		goto err_free;
	}

	/* IOMAP the entire PWM region */
	fsl_pwm->regs = ioremap(res->start, res->end - res->start + 1);
	if (fsl_pwm->regs == NULL) {
		dev_err(&ofdev->dev, "failed to ioremap memory region\n");
		ret = -ENOMEM;
		goto err_release;
	}

	fsl_pwm->clk = clk_get(&ofdev->dev, "pwm");
	if (IS_ERR(fsl_pwm->clk)) {
		dev_err(&ofdev->dev, "failed to get clock\n");
		ret = PTR_ERR(fsl_pwm->clk);
		goto err_unmap;
	}

	rate = fsl_get_sys_freq();
	if (rate)
		fsl_pwm->clk->rate_hz = rate;
	else
		fsl_pwm->clk->rate_hz = FSL_DEFAULT_IPG_CLK;

	/* Find the id of the pwm */
	id = of_get_property(ofdev->node, "cell-index", NULL);
	if (!id) {
		dev_err(&ofdev->dev, "failed to get cell-index\n");
		goto err_unmap;
	}

	fsl_pwm->pwm_id = *id;
	fsl_pwm->clk_enabled = 0;
	fsl_pwm->use_count = 0;
	fsl_pwm->duty = 0;
	fsl_pwm->period = 0;
	fsl_pwm->dev = ofdev->dev;

	np = of_find_node_by_name(NULL, "global-utilities");
	if (np) {
		gubr = of_iomap(np, 0);

		if (fsl_pwm->pwm_id) {
			clrsetbits_be32(&gubr->pmuxcr2,
					FSL_PMUXCR2_UART_PWM_DSP_GPIO,
					FSL_PMUXCR2_PWM_GPIO);
			clrsetbits_be32(&gubr->devdisr2, FSL_DEVDISR2_PWM2,
					FSL_DEVDISR2_PWM2_EN);
		} else {
			clrsetbits_be32(&gubr->pmuxcr1,
					FSL_PMUXCR1_SPI1_ANT_TCXO_PWM_GPIO,
					FSL_PMUXCR1_ANT_TCXO_PWM_GPIO);
			clrsetbits_be32(&gubr->devdisr2, FSL_DEVDISR2_PWM1,
					FSL_DEVDISR2_PWM1_EN);
		}

		of_node_put(np);
	} else {
		printk(KERN_EMERG "Error: Global Utilities Block Register "
				  "node is not found!\n");
		goto err_unmap;
	}

	ret = of_irq_to_resource(ofdev->node, 0, irq);
	if (ret == NO_IRQ) {
		dev_warn(&ofdev->dev, "no IRQ found\n");
		goto err_unmap;
	}

	fsl_pwm->irq = irq->start;
	if (fsl_pwm->irq < 0) {
		ret = -ENXIO;
		goto err_unmap;
	}

	/* Register for PWM Interrupt */
	ret = request_irq(fsl_pwm->irq, fsl_pwm_irq, 0, "fsl-pwm", fsl_pwm);
	if (ret != 0)
		goto err_unmap;
	else
		dev_info(&ofdev->dev,
			 "Freescale PWM Controller driver at 0x%p (irq = %d)\n",
			 fsl_pwm->regs, fsl_pwm->irq);

	/* Register sysfs hooks */
	ret = sysfs_create_group(&ofdev->dev.kobj, &pwm_group);
	if (ret != 0)
		goto err_unmap;

	dev_set_drvdata(&ofdev->dev, fsl_pwm);

	mutex_lock(&pwm_lock);
	list_add_tail(&fsl_pwm->node, &pwm_list);
	mutex_unlock(&pwm_lock);

	return 0;

err_unmap:
	iounmap(fsl_pwm->regs);
err_release:
	release_mem_region(res->start, res->end - res->start + 1);
err_free:
	kfree(fsl_pwm);
	return ret;
}

static int __devexit fsl_pwm_remove(struct of_device *ofdev)
{
	struct pwm_device *fsl_pwm;

	fsl_pwm = dev_get_drvdata(&ofdev->dev);
	if (fsl_pwm == NULL)
		return -ENODEV;

	mutex_lock(&pwm_lock);
	list_del(&fsl_pwm->node);
	mutex_unlock(&pwm_lock);

	if (fsl_pwm->regs)
		iounmap(fsl_pwm->regs);

	dev_set_drvdata(&ofdev->dev, NULL);
	clk_put(fsl_pwm->clk);
	sysfs_remove_group(&ofdev->dev.kobj, &pwm_group);
	kfree(fsl_pwm);

	return 0;
}

static const struct of_device_id fsl_pwm_match[] = {
	{
		.compatible = "fsl,pwm",
	},
	{},
};

static struct of_platform_driver fsl_pwm_driver = {
	.driver		= {
		.name	= "fsl-pwm",
	},
	.match_table    = fsl_pwm_match,
	.probe		= fsl_pwm_probe,
	.remove		= fsl_pwm_remove,
};

static int __init fsl_pwm_init(void)
{
	int ret;

	ret = of_register_platform_driver(&fsl_pwm_driver);
	if (ret)
		printk(KERN_ERR "fsl-pwm: Failed to register platform "
				"driver\n");

	return ret;
}

static void __exit fsl_pwm_exit(void)
{
	of_unregister_platform_driver(&fsl_pwm_driver);
}

module_init(fsl_pwm_init);
module_exit(fsl_pwm_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Chunhe Lan <Chunhe.Lan@freescale.com>");
MODULE_DESCRIPTION("Freescale PWM Controller driver");
