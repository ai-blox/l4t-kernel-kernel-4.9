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

#include "tp2912.h"

static int debug;
static bool diff_mode = false;
static bool test_pattern = false;
module_param(debug, int, 0644);
module_param(diff_mode, bool, 0644);
module_param(test_pattern, bool, 0644);
MODULE_PARM_DESC(debug, "debug level (0-2)");
MODULE_DESCRIPTION("TP2912 - Untra High Definition HD-TVI Video Encoder driver");
MODULE_AUTHOR("Nari");
MODULE_LICENSE("GPL v2");

struct tp2912_priv {
	uint8_t chipid;
	struct v4l2_subdev sd;
	struct media_pad pad;
	struct v4l2_ctrl_handler hdl;
	bool power_on;
};

static inline struct tp2912_priv *sd_to_priv(struct v4l2_subdev *sd)
{
	return container_of(sd, struct tp2912_priv, sd);
}

static int __attribute__((unused)) tp2912_read (struct tp2912_priv *priv, uint8_t reg)
{
	int ret;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);


	ret = i2c_smbus_read_byte_data(client, reg);
	if (ret < 0) {
		dev_err(&client->dev, "%s: failed to read at 0x%02x. Error = %d\n", __func__, reg, ret);
		return ret;
	}

	return ret;
}

static int __attribute__((unused)) tp2912_write (struct tp2912_priv *priv, uint8_t reg, uint8_t val)
{
	int ret;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	ret = i2c_smbus_write_byte_data(client, reg, val);
	if (ret < 0) {
		dev_err(&client->dev, "%s: failed to write to 0x%02x. Error = %d\n", __func__, reg, ret);
		return ret;
	}

	return 0;
}

static int tp2912_modify(struct tp2912_priv *priv, uint8_t reg, 
							uint8_t clear, uint8_t set)
{
	int ret;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	uint8_t val;

	ret = tp2912_read(priv, reg);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to read register 0x%08x. Error = %d\n", __func__, __LINE__, reg, ret);
		return ret;
	}

	val = (uint8_t)ret;
	val &= ~clear;
	val |= set;

	ret = tp2912_write(priv, reg, val);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x%08x. Error = %d\n", __func__, __LINE__, reg, ret);
		return ret;
	}

	return ret;
}

static int tp2912_write_block (struct tp2912_priv *priv, uint8_t *data)
{
	int ret;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int i;

	for (i = 0; i < sizeof(data); i += 2) {
		ret = i2c_smbus_write_byte_data(client, data[i], data[i + 1]);
		if (ret < 0) {
			dev_err(&client->dev, "%s: failed to write to 0x%02x. Error = %d\n", __func__, data[i], ret);
			return ret;
		}
	}

	return 0;
}

static int tp2912_chipid(struct tp2912_priv *priv) {
	int ret;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	int byte_high, byte_low, id_low, revision;
	int chipid;

	id_low = tp2912_read(priv, REG_DEVID_LOW);
	if(id_low < 0) {
		ret = id_low;
		v4l_err(client, "%s: failed to read REG_DEVID_LOW. Error = %d\n", __func__, ret);
		return ret;
	}

	switch (id_low) {
		case 0x01:
			/* Soft reset*/
			tp2912_write(priv, REG_INOUT, 0x80);

			/* Frequency Register: Read only register, for internal use only 
			* No speicific information in datasheet
			*/
			byte_high = tp2912_read(priv, REG_FREQ_HIGH);
			if(byte_high < 0) {
				ret = byte_high;
				v4l_err(client, "%s: failed to read REG_FREQ_HIGH. Error = %d\n", __func__, ret);
				return ret;
			}

			byte_low = tp2912_read(priv,REG_FREQ_LOW);
			if(byte_low < 0) {
				ret = byte_low;
				v4l_err(client, "%s: failed to read REG_FREQ_LOW. Error = %d\n", __func__, ret);
				return ret;
			}

			if((byte_high == 0x00) && (byte_low ==0x00)){
				chipid = TP2801B;
				v4l_info(client, "Chip id TP2801B detected\n");
			} else{
				chipid = TP2801A;
				v4l_info(client, "Chip id TP2801A detected\n");
			}
			break;
		case 0x10:
			chipid = TP2910;
			v4l_info(client, "Chip id TP2910 detected\n");
			break;
		case 0x03:
			chipid = TP2803;
			v4l_info(client, "Chip id TP2803 detected\n");
			break;
		case 0x12:
			revision = tp2912_read(priv, REG_REV);
			if(revision < 0) {
				ret = revision;
				v4l_err(client, "%s: failed to read REG_REV. Error = %d\n", __func__, ret);
				return ret;
			}

			if(revision == 0x01){
				chipid = TP2912B;
				v4l_info(client, "Chip id TP2912B detected\n");
			} else {
				chipid = TP2912;
				v4l_info(client, "Chip id TP2912 detected\n");
			}
			break;
		case 0x15:
			chipid = TP2915;
			v4l_info(client, "Chip id TP2915 detected\n");
			break;
		default:
			v4l_err(client, "%s: Unknown chip id\n", __func__);
			return -ENODEV;
	}

	return chipid;
}

uint8_t TP2910_init_data[] = {
	REG_YLMHI, 0xeb,
	REG_CLMHI, 0xf0,
	REG_CLMLO, 0x10,
	REG_UGAIN, 0x55,
	REG_VGAIN, 0x76,
	REG_FS_4, 0x48,
	REG_FS_3, 0xbb,
	REG_FS_2, 0x2e,
	REG_FS_1, 0x8b,
	0x41, 0x00, /* Hidden register */
	0x42, 0x12, /* Hidden register */
	0x43, 0x07, /* Hidden register */
	0x44, 0x49, /* Hidden register */
	REG_DAC, 0xcb,
	REG_DEVID_LOW, 0xff
};

static int tp2912_init_pll(struct tp2912_priv *priv) {
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret = 0;

	ret = tp2912_write(priv,0x42, 0x52); /* Hidden register */
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x42. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	ret = tp2912_write(priv,0x43, 0x07); /* Hidden register */
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x43. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}


	ret = tp2912_write(priv,0x44, 0x49); /* Hidden register */
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x44. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	return ret;
}

static int tp2912_startup_pll(struct tp2912_priv *priv){
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret = 0;
	uint8_t pll_con;

	ret = tp2912_read(priv, 0x43);
	if(ret < 0) {
		v4l_err(client, "%s: failed to read register 0x43. Error = %d\n", __func__, ret);
		return ret;
	}

	pll_con = (uint8_t)ret;

	ret = tp2912_write(priv, 0x43, pll_con & 0xbf);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x43. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	mdelay(100);
	ret = tp2912_write(priv, 0x43, pll_con);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x43. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	return ret;
}

static int tp2912_set_output_mode(struct tp2912_priv *priv) {
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret = 0;

	if((priv->chipid == TP2912) || (priv->chipid == TP2912B) || (priv->chipid == TP2915)){
		ret = tp2912_modify(priv, REG_TXDRIVER_3, 
						0, 
						BIT(3) /* voltage mode */
						);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to write register REG_TXDRIVER_3. Error = %d\n", __func__, __LINE__, ret);
			return ret;
		}

		if(diff_mode == true){
			ret = tp2912_modify(priv, REG_TXDRIVER_3, 
										0, 
										BIT(7) /* differential output mode */
								);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write register REG_TXDRIVER_3. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}

			if(priv->chipid != TP2912) {
				ret = tp2912_write(priv, REG_PTZ_2, 0x90);
				if(ret < 0) {
					v4l_err(client, "%s (line %d): failed to write register REG_PTZ_2. Error = %d\n", __func__, __LINE__, ret);
					return ret;
				}

				/* Hidden register */
				ret = tp2912_write(priv, 0x45, 0x40);
				if(ret < 0) {
					v4l_err(client, "%s (line %d): failed to write register 0x45. Error = %d\n", __func__, __LINE__, ret);
					return ret;
				}
			}
		} else {
			ret = tp2912_modify(priv, REG_TXDRIVER_3, 
										BIT(7), /* Single-ended output mode */
										0
							);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write register REG_TXDRIVER_3. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}
		}
	} else if(priv->chipid == TP2910){
		ret = tp2912_write(priv, REG_TXDRIVER_3, 0x08);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to write register REG_TXDRIVER_3. Error = %d\n", __func__, __LINE__, ret);
			return ret;
		}

		ret = tp2912_write(priv, REG_TXDRIVER_2, 0x05);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to write register REG_TXDRIVER_2. Error = %d\n", __func__, __LINE__, ret);
			return ret;
		}
	} else {
		v4l_err(client, "%s (line %d): Unkown chip id\n", __func__, __LINE__);
		return -ENODEV;
	}

	return ret;
}

static int tp2912_init(struct tp2912_priv *priv) {
	int ret = 0;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	ret = tp2912_chipid(priv);
	if(ret < 0) {
		v4l_err(client, "%s: failed to read chip id. Error = %d\n", __func__, ret);
		return ret;
	}

	priv->chipid = (uint8_t)ret;

	if((priv->chipid == TP2915) || (priv->chipid == TP2912B)) {
		ret = tp2912_write(priv, REG_PTZ_1, 0x00);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to write REG_PTZ_1. Error = %d\n", __func__, __LINE__, ret);
			return ret;
		}
	} else {
		if(priv->chipid == TP2912) {
			/* Set REG_DAC = 0x8b */
			ret = tp2912_write(priv, REG_DAC, 
						BIT(7) |        /* VCMO: 0 (1.6V), 1 (1.475V) recommended for VDD3 = 3V */
						(0b0 << 4) |    /* DAC gain control: -16%  */
						(0b1 << 3) |    /* AC compensation mode control: enable  */
						(0b101 << 0)    /* DAC full scale current control: 24mA  */
						);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write REG_DAC. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}
		} else {
			ret = tp2912_write_block(priv, TP2910_init_data);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write TP2910_init_data. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}
		}

		ret = tp2912_init_pll(priv);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to init PLL. Error = %d\n", __func__, __LINE__, ret);
			return ret;
		}

		ret = tp2912_startup_pll(priv);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to startup PLL. Error = %d\n", __func__, __LINE__, ret);
			return ret;
		}

		mdelay(20);

		/* Hidden register */
		if((priv->chipid == TP2801B) || (priv->chipid == TP2803)) {
			ret = tp2912_write(priv, 0x43, 0x47);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write register 0x43. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}
		}
	}

	ret = tp2912_set_output_mode(priv);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to set output mode. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	/* Output test pattern if enabled */
	ret = tp2912_modify(priv, REG_MODE, 
								test_pattern ? 0 : BIT(6),
								test_pattern ? BIT(6) : 0 
					);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register REG_MODE. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	priv->power_on = true;
	return ret;
}

static int tp2912_s_std_output(struct v4l2_subdev *sd, v4l2_std_id std)
{
	return 0;
}

static const struct v4l2_subdev_video_ops tp2912_video_ops = {
	.s_std_output = tp2912_s_std_output,
};

static int tp2912_log_status(struct v4l2_subdev *sd)
{
	struct i2c_client __attribute__((unused)) *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv __attribute__((unused)) *priv = sd_to_priv(sd);

	return 0;
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int tp2912_g_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);
	int ret;


	ret = tp2912_read(priv, reg);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to read register 0x%08x. Error = %d\n", __func__, __LINE__, reg->reg, ret);
		return ret;
	}


	reg->size = 1;
	reg->val = ret;

	return 0;
}

static int tp2912_s_register(struct v4l2_subdev *sd, const struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);
	int ret;

	ret = tp2912_write(priv, reg->reg, reg->val);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register at 0x%08x. Error = %d\n", __func__, __LINE__, reg->reg, ret);
		return ret;
	}

	return 0;
}
#endif

static int tp2912_s_power(struct v4l2_subdev *sd, int on)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);
	int ret = 0;

	v4l2_dbg(1, debug, client, "%s: power %s\n", __func__, on ? "on" : "off");

	if (priv->power_on == !!on)
		return 0;

	if (on) {
		priv->power_on = true;
	} else {
		priv->power_on = false;
	}

	return ret;
}

static const struct v4l2_subdev_core_ops tp2912_core_ops = {
	.log_status = tp2912_log_status,
#ifdef CONFIG_VIDEO_ADV_DEBUG
	.g_register = tp2912_g_register,
	.s_register = tp2912_s_register,
#endif
	.s_power = tp2912_s_power,
	.interrupt_service_routine = NULL,
};

static const struct v4l2_subdev_ops tp2912_ops = {
	.core	= &tp2912_core_ops,
	.video	= &tp2912_video_ops,
};

static const struct v4l2_ctrl_config tp2912_ctrl_diff_mode = {
	.ops = &tp2912_ops,
	.id = V4L2_CID_TP2912_DIFF_MODE,
	.name = "Differential mode output",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = false,
	.max = true,
	.step = 1,
	.def = false,
};

static const char * const tp2912_test_pattern_menu[] = {
	"Disabled",
	"Enabled",
};

static int tp2912_s_ctrl(struct v4l2_ctrl *ctrl)
{
	struct v4l2_subdev *sd =
		&container_of(ctrl->handler, struct tp2912_priv, hdl)->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);

	switch (ctrl->id) {
		case V4L2_CID_GAIN:
			break;
		case V4L2_CID_TEST_PATTERN:
			test_pattern = ctrl->val ? true : false;
			/* Output test pattern if enabled */
			ret = tp2912_modify(priv, REG_MODE, 
										test_pattern ? 0 : BIT(6),
										test_pattern ? BIT(6) : 0 
							);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write register REG_MODE. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}
			break;
		case V4L2_CID_TP2912_DIFF_MODE:
		break;
		default:
			v4l_err(client, "%s: Unknown control id\n", __func__);
			return -EINVAL;
	}

	return -EINVAL;
}

static const struct v4l2_ctrl_ops tp2912_ctrl_ops = {
	.s_ctrl = tp2912_s_ctrl,
};

static int tp2912_probe(struct i2c_client *client, 
						const struct i2c_device_id *id)
{
	int ret = 0;
	struct tp2912_priv *priv;
	struct v4l2_ctrl_handler *hdl;
	struct v4l2_subdev *sd;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	v4l_info(client, "chip found @ 0x%x (%s)\n",
			client->addr << 1, client->adapter->name);

	priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	sd = &priv->sd;
	hdl = &priv->hdl;
	v4l2_i2c_subdev_init(sd, client, &tp2912_ops);

	priv->pad.flags = MEDIA_PAD_FL_SINK;
	ret = media_entity_pads_init(&sd->entity, 1, &priv->pad);
	if (ret) {
		v4l_err(client, "%s (line %d): failed to media_entity_pads_init(). Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	sd->ctrl_handler = hdl;

	ret = v4l2_ctrl_handler_init(hdl, 2);
	if (ret) {
		v4l_err(client, "%s (line %d): failed to v4l2_ctrl_handler_init(). Error = %d\n", __func__, __LINE__, ret);
		goto error_1;
	}

	v4l2_ctrl_new_std_menu_items(hdl, &tp2912_ctrl_ops,
								 V4L2_CID_TEST_PATTERN,
								 ARRAY_SIZE(tp2912_test_pattern_menu) - 1, 0,
								 0, tp2912_test_pattern_menu);
	v4l2_ctrl_new_std(hdl, &tp2912_ctrl_ops,
					  V4L2_CID_GAIN, -128, 127, 1, 0);
	v4l2_ctrl_new_custom(hdl, &tp2912_ctrl_diff_mode, NULL);

	ret = tp2912_init(priv);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to init TP2912. Error = %d\n", __func__, __LINE__, ret);
		goto error_2;
	}

	return ret; 

error_2:
	v4l2_ctrl_handler_free(hdl);
error_1:
	media_entity_cleanup(&sd->entity);
	return ret;
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