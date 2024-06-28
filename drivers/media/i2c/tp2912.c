/*
 * Copyright (C) 2024  Ai-blox Ltd.
 * Author: Nari (nari@ai-blox.com)
 *
 * This program is free software; you may redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/videodev2.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>

static int debug;
module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "debug level (0-2)");
MODULE_DESCRIPTION("TP2912 - Untra High Definition HD-TVI Video Encoder driver");
MODULE_AUTHOR("Nari");
MODULE_LICENSE("GPL v2");

struct tp2912_priv {
	struct v4l2_subdev sd;
	struct v4l2_ctrl_handler hdl;
};

static int tp2912_s_std_output(struct v4l2_subdev *sd, v4l2_std_id std)
{
	return 0;
}

static const struct v4l2_subdev_video_ops tp2912_video_ops = {
	.s_std_output = tp2912_s_std_output,
};

static const struct v4l2_subdev_core_ops tp2912_core_ops = {
};
static const struct v4l2_subdev_ops tp2912_ops = {
	.core	= &tp2912_core_ops,
	.video	= &tp2912_video_ops,
};
static int tp2912_probe(struct i2c_client *client, 
						const struct i2c_device_id *id)
{
	int err = 0;
	struct tp2912_priv *priv;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	v4l_info(client, "chip found @ 0x%x (%s)\n",
			client->addr << 1, client->adapter->name);

	priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	v4l2_i2c_subdev_init(&priv->sd, client, &tp2912_ops);

	return err; 
}

static int tp2912_remove(struct i2c_client *client)
{
	return 0;
}
static const struct i2c_device_id tp2912_id[] = {
	{"tp2912", 0},
	{},
};
MODULE_DEVICE_TABLE(i2c, tp2912_id);

static struct i2c_driver tp2912_driver = {
	.driver = {
		.owner	= THIS_MODULE,
		.name	= "tp2912",
	},
	.probe		= tp2912_probe,
	.remove		= tp2912_remove,
	.id_table	= tp2912_id,
};
module_i2c_driver(tp2912_driver);