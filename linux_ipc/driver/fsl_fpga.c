/*
 * drivers/misc/fsl_fpga.c
 * Freescale FPGA driver
 *
 * Existing FPGA driver is providing minimal functionality for RF BAND
 * configuration
 * Author: bhaskar <bhaskar.upadhaya@freescale.com>
 *
 * Copyright 2012 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/fsl_fpga.h>

struct fpga_priv {
	struct i2c_client *client;
	struct device *dev;
};

struct fpga_priv *fpga_priv;

static const struct i2c_device_id fpga_id[] = {
	{ "fpga", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, fpga_id);

/*
** Function: reset_phy_via_fpga()
** Used to reset the AD9361 by toggling the RF connector specific
** bits of RST_FORCE1 via FPGA
*/
void reset_phy_via_fpga(int rf_num)
{
	u8 val;

	val = (u8) i2c_smbus_read_byte_data(fpga_priv->client, RST_FORCE1);

	switch (rf_num) {
	case RF1:
		val = val & ~RF1_RST_MASK;
		val |= RF1_RST_ASSERT;
		i2c_smbus_write_byte_data(fpga_priv->client, RST_FORCE1, val);
		val = val & ~RF1_RST_MASK;
		i2c_smbus_write_byte_data(fpga_priv->client, RST_FORCE1, val);
		break;
	case RF2:
		val = val & ~RF2_RST_MASK;
		val |= RF2_RST_ASSERT;
		i2c_smbus_write_byte_data(fpga_priv->client, RST_FORCE1, val);
		val = val & ~RF2_RST_MASK;
		i2c_smbus_write_byte_data(fpga_priv->client, RST_FORCE1, val);
		break;
	case RF3:
		val = val & ~RF3_RST_MASK;
		val |= RF3_RST_ASSERT;
		i2c_smbus_write_byte_data(fpga_priv->client, RST_FORCE1, val);
		val = val & ~RF3_RST_MASK;
		i2c_smbus_write_byte_data(fpga_priv->client, RST_FORCE1, val);
		break;
	default:
		dev_err(&fpga_priv->client->dev, "Invalid RF connector "
					"selected for reset\n");
	}
}
EXPORT_SYMBOL(reset_phy_via_fpga);

/*
** Function: config_rf_regulator()
** Used to configure different bands ie BAND1, BAND7, BAND13 on 9132QDS
** via FPGA
** The FPGA register used to configure the various bands is BRDCFG9
*/
int config_rf_regulator(bool rf_pa, bool rf_lna, int rf_num)
{
	int rc = 0;
	struct device *dev = &fpga_priv->client->dev;
	u8 val;

	if (!fpga_priv) {
		dev_err(dev, "fpga_priv un-initialized \r\n");
		rc = -EINVAL;
		goto out;
	}

	val = (u8) i2c_smbus_read_byte_data(fpga_priv->client, BRDCFG9);

	switch (rf_num) {
	case RF1:
		val = val & RF1_MASK;
		val |= (rf_pa ? 0 : 1) << RF1_PA_SHIFT;
		val |= (rf_lna ? 0 : 1) << RF1_LNA_SHIFT;
		i2c_smbus_write_byte_data(fpga_priv->client, BRDCFG9, val);
		break;
	case RF2:
		val = val & RF2_MASK;
		val |= (rf_pa ? 0 : 1) << RF2_PA_SHIFT;
		val |= (rf_lna ? 0 : 1) << RF2_LNA_SHIFT;
		i2c_smbus_write_byte_data(fpga_priv->client, BRDCFG9, val);
		break;
	case RF3:
		val = val & RF3_MASK;
		val |= (rf_pa ? 0 : 1) << RF3_PA_SHIFT;
		val |= (rf_lna ? 0 : 1) << RF3_LNA_SHIFT;
		i2c_smbus_write_byte_data(fpga_priv->client, BRDCFG9, val);
		break;
	default:
		dev_err(dev, "Invalid RF Connector selected for"
				" RF Configuration\r\n");
		rc = -EINVAL;
	}

out:
	return rc;
}
EXPORT_SYMBOL(config_rf_regulator);

/*
** Function: select_ref_clock_source()
** Selects reference clock source for AIC block
** Possible value are
** - CON1_XCVR_REF (clock from RF1 connector)
** - CON2_XCVR_REF (clock from RF2 connector)
*/
void select_ref_clock_source(void)
{
	u8 val;

	val = (u8) i2c_smbus_read_byte_data(fpga_priv->client, BRDCFG6);
	val |= CON2_XCVR_REF;
	i2c_smbus_write_byte_data(fpga_priv->client, BRDCFG6, val);
}
EXPORT_SYMBOL(select_ref_clock_source);

static int fpga_probe(struct i2c_client *client,
		const struct i2c_device_id *id)
{
	fpga_priv = kzalloc(sizeof(struct fpga_priv), GFP_KERNEL);
	if (!fpga_priv) {
		dev_err(&client->dev, "Failed to allocate fpga_priv\n");
		return -ENOMEM;
	}

	fpga_priv->client = client;
	fpga_priv->dev = &client->dev;

	return 0;
}

static int __devexit fpga_remove(struct i2c_client *client)
{
	kfree(fpga_priv);
	return 0;
}

static struct i2c_driver fpga_driver = {
	.driver = {
		.name	= "fsl-fpga",
		.owner	= THIS_MODULE,
	},
	.probe		= fpga_probe,
	.remove		= __devexit_p(fpga_remove),
	.id_table	= fpga_id,
};

static int __init fpga_mod_init(void)
{
	return i2c_add_driver(&fpga_driver);
}

static void __exit fpga_exit(void)
{
	i2c_del_driver(&fpga_driver);
}

module_init(fpga_mod_init);
module_exit(fpga_exit);
