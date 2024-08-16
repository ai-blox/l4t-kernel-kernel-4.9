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
#include <linux/v4l2-dv-timings.h>
#include <linux/gpio/consumer.h>

#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-dv-timings.h>

#include "tp2912.h"

static int debug = 2;
static int video_mode;
static bool diff_mode = false;
static bool current_mode = false;
static bool test_pattern;
module_param(debug, int, 0644);
MODULE_PARM_DESC(debug, "debug level (0-2)");
MODULE_DESCRIPTION("TP2912 - Untra High Definition HD-TVI Video Encoder driver");
MODULE_AUTHOR("Nari");
MODULE_LICENSE("GPL v2");

struct tp2912_priv {
	uint8_t chipid;
	struct gpio_desc *gpiod_fhd;
	struct v4l2_device v4l2_dev;
	struct media_device mdev;
	struct v4l2_subdev sd;
	struct media_pad pad;
	struct v4l2_ctrl_handler hdl;
	struct v4l2_dv_timings dv_timings;
	bool power_on;
} * priv;

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
		v4l_err(client, "%s (line %d): failed to read register 0x%02x. Error = %d\n", __func__, __LINE__, reg, ret);
		return ret;
	}

	val = (uint8_t)ret;
	if(reg == REG_MODE) {
		v4l_info(client, "ret = 0x%02x val = 0x%02x", ret, val);
	}
	val &= ~clear;
	if(reg == REG_MODE) {
		v4l_info(client, "ret = 0x%02x val = 0x%02x", ret, val);
	}
	val |= set;
	if(reg == REG_MODE) {
		v4l_info(client, "ret = 0x%02x val = 0x%02x", ret, val);
	}

	ret = tp2912_write(priv, reg, val);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register 0x%02x. Error = %d\n", __func__, __LINE__, reg, ret);
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

	for (i = 0; i < sizeof(data) / sizeof(data[0]); i += 2) {
		ret = i2c_smbus_write_byte_data(client, data[i], data[i + 1]);
		if (ret < 0) {
			dev_err(&client->dev, "%s: failed to write to 0x%02x. Error = %d\n", __func__, data[i], ret);
			return ret;
		}
	}

	return 0;
}

static int tp2912_write_table (struct tp2912_priv *priv, uint8_t *table, uint8_t col)
{
	int ret;
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	uint8_t col_count, row_count;
	uint8_t *data;
	int row;

	col_count = table[0];
	row_count =  table[1];
	data = &table[TABLE_HEADER_SIZE];

	v4l2_dbg(2, debug, client, "%s (line %d): col_count = %d, row_count = %d sizeof(table) = %ld\n", __func__, __LINE__,
			 col_count, row_count, sizeof(table));

	for (row = 0; row < row_count; row++) {
		v4l2_dbg(2, debug, client, "%s (line %d): write value 0x%02x to register 0x%02x\n", __func__, __LINE__,
				data[row * col_count + col], data[row * col_count]);

		ret = i2c_smbus_write_byte_data(client, data[row * col_count], data[row * col_count + col]);
		if (ret < 0) {
			dev_err(&client->dev, "%s: failed to write to 0x%02x. Error = %d\n", __func__, data[row * col_count], ret);
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

		/* Current mode or voltage mode */
		ret = tp2912_modify(priv, REG_TXDRIVER_3, 
									BIT(3), 
									current_mode ? 0 : BIT(3)
							);
		if(ret < 0) {
			v4l_err(client, "%s (line %d): failed to write register REG_TXDRIVER_3. Error = %d\n", __func__, __LINE__, ret);
			return ret;
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
	uint32_t line_num = LINE_NUM_INTR;

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

	/* Interrupt configuration */
	ret = tp2912_modify(priv, REG_INTMODE, 
								0,
								BIT(2) /* All bits in REG_INT_STATUS is clear after REG_INT_STATUS is read */
					);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register REG_INT_STATUS. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	if(line_num > TP2912_MAX_WIDTH) {
		v4l_warn(client, "%s (line %d): line_num (%d) exceeds max TP2912_MAX_WIDTH (%d).\n   \
		Set line_num to %d\n", __func__, __LINE__, line_num, TP2912_MAX_WIDTH, TP2912_MAX_WIDTH);
		line_num = TP2912_MAX_WIDTH;
	}

	/* Write line count */
	ret = tp2912_modify(priv, REG_INTLINE_3, BIT(3), ((line_num >> 11) & 0x01) << 3);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write to register REG_INTLINE_3. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	ret = tp2912_modify(priv, REG_INTLINE_2, 0x07, (line_num >> 8) & 0x07);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write to register REG_INTLINE_2. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	ret = tp2912_write(priv, REG_INTLINE_1, line_num & 0xFF);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write to register REG_INTLINE_1. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	priv->power_on = true;
	return ret;
}

static const struct v4l2_dv_timings_cap tp2912_timings_cap = {
	.type = V4L2_DV_BT_656_1120,
	/* keep this initialization for compatibility with GCC < 4.4.6 */
	.reserved = { 0 },
	V4L2_INIT_BT_TIMINGS(640, TP2912_MAX_WIDTH, 
		350, TP2912_MAX_HEIGHT,
		TP2912_MIN_PIXELCLOCK, TP2912_MAX_PIXELCLOCK,
		V4L2_DV_BT_STD_CEA861 | V4L2_DV_BT_STD_DMT | V4L2_DV_BT_STD_GTF | V4L2_DV_BT_STD_CVT,
		V4L2_DV_BT_CAP_PROGRESSIVE | V4L2_DV_BT_CAP_INTERLACED | V4L2_DV_BT_CAP_CUSTOM)
};

static uint8_t *tp2912_find_table(struct tp2912_priv *priv, 
							   int width, int height, int fps, 
							   uint8_t chipid,
							   uint8_t mode, uint8_t *out_col, uint8_t *out_fhd_en) {
	struct v4l2_subdev *sd = &priv->sd;
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	uint64_t col_count, row_count, data_size;
	int i, row, col;
	uint64_t input[] = {width, height, fps, BIT(chipid), mode};
	uint64_t *data;

	data_size = sizeof(tp2912_parent_table) / sizeof(tp2912_parent_table[0]) - TABLE_HEADER_SIZE;
	data = &tp2912_parent_table[TABLE_HEADER_SIZE];
	col_count = tp2912_parent_table[0];
	row_count = data_size / col_count;

	v4l2_dbg(2, debug, client, "%s (line %d): col_count = %lld, row_count = %lld sizeof(tp2912_parent_table) = %ld\n", __func__, __LINE__,
			 col_count, row_count, sizeof(tp2912_parent_table));

	if(row_count != tp2912_parent_table[1]) {
		v4l_warn(client, "%s (line %d): tp2912_parent_table has %lld row(s), but header info gives %lld row(s)\n",
				 __func__, __LINE__, row_count, tp2912_parent_table[1]);
	}

	for(row = 0; row < row_count; row++) {
		for(col = 0; col < TABLE_INDEX; col++) {
			i = col + row * col_count;

			if((col == CHIPID_BITMASK_INDEX)) {
				if(!(data[i] & input[col])) {
					v4l2_dbg(2, debug, client, "%s (line %d): row = %d, col = %d, chipid = %d \n", __func__, __LINE__,
							row, col, chipid);
					break;
				} else {
					v4l2_dbg(2, debug, client, "%s (line %d): row = %d, col = %d, chipid = %d \n", __func__, __LINE__,
							row, col, chipid);
					continue;
				}
			}

			if(data[i] != input[col]) {
				v4l2_dbg(2, debug, client, "%s (line %d): row = %d, col = %d, chipid = %d \n", __func__, __LINE__,
						 row, col, chipid);
				break;
			}
		}

		if(col == TABLE_INDEX) {
			*out_col = data[COL_INDEX + row * col_count];
			*out_fhd_en = data[FHD_EN_INDEX + row * col_count];
			v4l2_dbg(2, debug, client, "%s (line %d): GOT row = %d, out_col = %d, out_fhd_en = %d, chipid = %d, table address = 0x%llx\n", __func__, __LINE__,
					 row, *out_col, *out_fhd_en, chipid, data[TABLE_INDEX + row * col_count]);
			return (uint8_t *)data[TABLE_INDEX + row * col_count];
		}
	}

	return NULL;
}

static bool tp2912_check_dv_timings(const struct v4l2_dv_timings *timing, void *hdl)
{
	struct i2c_client *client = v4l2_get_subdevdata(&priv->sd);
	const struct v4l2_bt_timings *bt = &timing->bt;
	uint32_t fps;
	uint8_t *table;
	uint8_t col, fhd_en;

	fps = DIV_ROUND_CLOSEST_ULL(bt->pixelclock, 
								(uint32_t)(V4L2_DV_BT_FRAME_WIDTH(bt) * V4L2_DV_BT_FRAME_HEIGHT(bt)));
	table = tp2912_find_table(priv, bt->width, bt->height, fps, priv->chipid, video_mode, &col, &fhd_en);
	if(!table) {
		v4l_err(client, "%s (line %d): failed to lookup table for %s video %dx%d@%dHz\n", __func__, __LINE__, 
				video_mode == TVI ? "TVI" : "AHD", bt->width, bt->height, fps);
		return false;
	}

	return true;
}

static int tp2912_s_dv_timings(struct v4l2_subdev *sd,
			       struct v4l2_dv_timings *timings)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);
	struct v4l2_bt_timings *bt = &timings->bt;
	uint32_t fps;
	uint8_t *table;
	uint8_t col, fhd_en;
	int ret;

	v4l2_dbg(1, debug, sd, "%s:\n", __func__);

	if (debug > 1) {
		v4l2_print_dv_timings(sd->name, "tp2912_s_dv_timings: ",
						  timings, true);
	}

	if (!v4l2_valid_dv_timings(timings, &tp2912_timings_cap, tp2912_check_dv_timings, NULL)) {
		v4l_err(client, "%s (line %d): timing is not valid\n", __func__, __LINE__);
		return -EINVAL;
	}

	/* Fill the optional fields .standards and .flags in struct v4l2_dv_timings
	   if the format is one of the CEA or DMT timings. */
	v4l2_find_dv_timings_cap(timings, &tp2912_timings_cap, 0, NULL, NULL);

	priv->dv_timings = *timings;

	/* set hsync, vsync polarity */
	ret = tp2912_modify(priv, REG_EX_SYNCIN,
		((bt->polarities & V4L2_DV_HSYNC_POS_POL) ? BIT(6) : 0) | ((bt->polarities & V4L2_DV_VSYNC_POS_POL) ? BIT(7) : 0),
		((bt->polarities & V4L2_DV_HSYNC_POS_POL) ? 0 : BIT(6)) | ((bt->polarities & V4L2_DV_VSYNC_POS_POL) ? 0 : BIT(7))) ;
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register REG_EX_SYNCIN. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	fps = DIV_ROUND_CLOSEST_ULL(bt->pixelclock, 
								(uint32_t)(V4L2_DV_BT_FRAME_WIDTH(bt) * V4L2_DV_BT_FRAME_HEIGHT(bt)));
	table = tp2912_find_table(priv, bt->width, bt->height, fps, priv->chipid, video_mode, &col, &fhd_en);
	if(!table) {
		v4l_err(client, "%s (line %d): failed to lookup table for %s video %dx%d@%dHz\n", __func__, __LINE__, 
				video_mode == TVI ? "TVI" : "AHD", bt->width, bt->height, fps);
		return -ENODEV;
	}

	/* Set GPIO FHD/CVBS enable */
	gpiod_set_value_cansleep(priv->gpiod_fhd, fhd_en);

	ret = tp2912_write_table(priv, table, col);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write table. Error = %d\n", __func__, __LINE__, ret);
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

	/* AHD or TVI */
	ret = tp2912_modify(priv, REG_ENC_MODE, 
								BIT(5),
								video_mode == AHD ? BIT(5) : 0
						);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to write register REG_ENC_MODE. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	return 0;
}

static int tp2912_g_dv_timings(struct v4l2_subdev *sd,
				struct v4l2_dv_timings *timings)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);

	v4l2_dbg(1, debug, client, "%s:\n", __func__);

	if (!timings) {
		v4l_err(client, "%s (line %d): timing == NULL! \nn", __func__, __LINE__);
		return -EINVAL;
	}

	*timings = priv->dv_timings;

	return 0;
}

static const struct v4l2_subdev_video_ops tp2912_video_ops = {
	.s_dv_timings = tp2912_s_dv_timings,
	.g_dv_timings = tp2912_g_dv_timings,
};

static int tp2912_log_status(struct v4l2_subdev *sd)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);
	int ret;
	uint8_t intr_status;
	uint8_t rx_line;

	/* Read interrupt status register */
	ret = tp2912_read(priv, REG_INT_STATUS);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to read register REG_INT_STATUS. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}
	intr_status = (uint8_t)ret;

	ret = tp2912_read(priv, REG_RXLINE_STATUS);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to read register REG_RXLINE_STATUS. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}
	rx_line = (uint8_t)ret;

	v4l_info(client, "=== Configured video info ===\n \
			Video signal: %s\n \
			FHD enable: %s\n \
			Current mode enable: %s\n \
			Output mode: %s \n\n",
			video_mode == TVI ? "TVI" : "AHD",
			gpiod_get_value_cansleep(priv->gpiod_fhd) ? "yes" : "no",
			current_mode ? "yes" : "no",
			diff_mode == true ? "Differential" : "Single-ended");

	v4l_info(client, "=== Interrupt status ===\n \
			LINEINT (general-purpose line interrupt): %s \n\
			TXINT (TX data transmit): %s \n\
			RXINT (RX data transmit): %s\n\n",
			intr_status & BIT(2) ? "yes" : "no",
			intr_status & BIT(1) ? "yes" : "no",
			intr_status & BIT(0) ? "yes" : "no");

	v4l_info(client, "=== RX line receive status ===\n \
			Line 8: %s \n\
			Line 7: %s \n\
			Line 6: %s \n\
			Line 5: %s \n\
			Line 4: %s \n\
			Line 3: %s \n\
			Line 2: %s \n\
			Line 1: %s \n\n",
			rx_line & BIT(7) ? "yes" : "no",
			rx_line & BIT(6) ? "yes" : "no",
			rx_line & BIT(5) ? "yes" : "no",
			rx_line & BIT(4) ? "yes" : "no",
			rx_line & BIT(3) ? "yes" : "no",
			rx_line & BIT(2) ? "yes" : "no",
			rx_line & BIT(1) ? "yes" : "no",
			rx_line & BIT(0) ? "yes" : "no");

	
	return 0;
}

#ifdef CONFIG_VIDEO_ADV_DEBUG
static int tp2912_g_register(struct v4l2_subdev *sd, struct v4l2_dbg_register *reg)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	struct tp2912_priv *priv = sd_to_priv(sd);
	int ret;


	ret = tp2912_read(priv, reg->reg);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to read register 0x%02x. Error = %d\n", __func__, __LINE__, (uint8_t)reg->reg, ret);
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
		v4l_err(client, "%s (line %d): failed to write register at 0x%02x. Error = %d\n", __func__, __LINE__, (uint8_t)reg->reg, ret);
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

static int tp2912_dv_timings_cap(struct v4l2_subdev *sd,
			struct v4l2_dv_timings_cap *cap)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);

	if (cap->pad != 0) {
		v4l_err(client, "%s (line %d): checked if cap->pad == 0, and found cap->pad == %d\n", __func__, __LINE__, cap->pad);
		return -EINVAL;
	}

	*cap = tp2912_timings_cap;

	return 0;
}

static int tp2912_enum_dv_timings(struct v4l2_subdev *sd,
			struct v4l2_enum_dv_timings *timings)
{
	struct i2c_client *client = v4l2_get_subdevdata(sd);
	int ret = 0;

	if (timings->pad != 0) {
		v4l_err(client, "%s (line %d): checked if timings->pad == 0, and found timings->pad == %d\n", __func__, __LINE__, timings->pad);
		return -EINVAL;
	}

	ret= v4l2_enum_dv_timings_cap(timings,
								  &tp2912_timings_cap,
								  tp2912_check_dv_timings, NULL);
	if (ret < 0) {
		v4l_err(client, "%s (line %d): v4l2_enum_dv_timings_cap() failed. Error = %d\n", __func__, __LINE__, ret);
		return ret;
	}

	return ret;
}

static const struct v4l2_subdev_pad_ops tp2912_pad_ops = {
	.dv_timings_cap = tp2912_dv_timings_cap,
	.enum_dv_timings = tp2912_enum_dv_timings,
};

static const struct v4l2_subdev_ops tp2912_ops = {
	.core	= &tp2912_core_ops,
	.video	= &tp2912_video_ops,
	.pad	= &tp2912_pad_ops,
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
	struct v4l2_dv_timings timings;
	int ret = 0;
	bool backup_bool;
	int backup_int;

	switch (ctrl->id) {
		case V4L2_CID_GAIN:
			break;
		case V4L2_CID_TEST_PATTERN:
			backup_bool = test_pattern;
			test_pattern = ctrl->val ? true : false;
			/* Output test pattern if enabled */
			ret = tp2912_modify(priv, REG_MODE, 
										test_pattern ? 0 : BIT(6),
										test_pattern ? BIT(6) : 0 
							);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to write register REG_MODE. Error = %d\n", __func__, __LINE__, ret);
				test_pattern = backup_bool;
				return ret;
			}
			break;
		case V4L2_CID_TP2912_DIFF_MODE:
			backup_bool = diff_mode;
			diff_mode = ctrl->val ? true : false;
			ret = tp2912_set_output_mode(priv);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to set output mode. Error = %d\n", __func__, __LINE__, ret);
				diff_mode = backup_bool;
				return ret;
			}
		break;
		case V4L2_CID_TP2912_CURRENT_MODE:
			backup_bool = current_mode;
			current_mode = ctrl->val ? true : false;
			ret = tp2912_set_output_mode(priv);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to set output mode. Error = %d\n", __func__, __LINE__, ret);
				current_mode = backup_bool;
				return ret;
			}
		break;
		case V4L2_CID_TP2912_FORCE_FHD_EN:
			gpiod_set_value_cansleep(priv->gpiod_fhd, ctrl->val ? 1 : 0);
		break;
		case V4L2_CID_TP2912_VIDEO_MODE:
			ret = sd->ops->video->g_dv_timings(sd, &timings);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to get timing. Error = %d\n", __func__, __LINE__, ret);
				return ret;
			}

			/* Update video mode */
			backup_int = video_mode;
			video_mode = ctrl->val;

			/* Update timing */
			ret = sd->ops->video->s_dv_timings(sd, &timings);
			if(ret < 0) {
				v4l_err(client, "%s (line %d): failed to set timing. Error = %d\n", __func__, __LINE__, ret);
				video_mode = backup_int;
				return ret;
			}
		break;
		default:
			v4l_err(client, "%s: Unknown control id\n", __func__);
			return -EINVAL;
	}

	return ret;
}

static const struct v4l2_ctrl_ops tp2912_ctrl_ops = {
	.s_ctrl = tp2912_s_ctrl,
};

static const struct v4l2_ctrl_config tp2912_ctrl_diff_mode = {
	.ops = &tp2912_ctrl_ops,
	.id = V4L2_CID_TP2912_DIFF_MODE,
	.name = "Differential mode output",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = false,
	.max = true,
	.step = 1,
	.def = false,
};

static const struct v4l2_ctrl_config tp2912_ctrl_current_mode = {
	.ops = &tp2912_ctrl_ops,
	.id = V4L2_CID_TP2912_CURRENT_MODE,
	.name = "Current mode output",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = false,
	.max = true,
	.step = 1,
	.def = false,
};

static const struct v4l2_ctrl_config tp2912_ctrl_fhd_en = {
	.ops = &tp2912_ctrl_ops,
	.id = V4L2_CID_TP2912_FORCE_FHD_EN,
	.name = "Enable FHD",
	.type = V4L2_CTRL_TYPE_BOOLEAN,
	.min = false,
	.max = true,
	.step = 1,
	.def = false,
};

static const struct v4l2_ctrl_config tp2912_ctrl_video_mode = {
	.ops = &tp2912_ctrl_ops,
	.id = V4L2_CID_TP2912_VIDEO_MODE,
	.name = "TVI/AHD mode",
	.type = V4L2_CTRL_TYPE_INTEGER,
	.min = 0,
	.max = VIDEO_MODE_NUM - 1,
	.step = 1,
	.def = TVI,
};

static int tp2912_probe(struct i2c_client *client, 
						const struct i2c_device_id *id)
{
	int ret = 0;
	struct v4l2_ctrl_handler *hdl;
	struct v4l2_subdev *sd;
	static const struct v4l2_dv_timings default_timing =
						 V4L2_DV_BT_CEA_1280X720P60;

	/* Check if the adapter supports the needed features */
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA))
		return -ENODEV;

	v4l_info(client, "chip found @ 0x%x (%s)\n",
			client->addr << 1, client->adapter->name);

	priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	priv->gpiod_fhd = devm_gpiod_get(&client->dev, "fhd", GPIOD_OUT_HIGH);
	if (IS_ERR(priv->gpiod_fhd)) {
		v4l_err(client, "%s (line %d): failed to get FHD gpio\n", __func__, __LINE__);
		return PTR_ERR(priv->gpiod_fhd);
	}

	priv->dv_timings = default_timing;
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
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE | V4L2_SUBDEV_FL_HAS_EVENTS;

	ret = v4l2_ctrl_handler_init(hdl, 6);
	if (ret) {
		v4l_err(client, "%s (line %d): failed to v4l2_ctrl_handler_init(). Error = %d\n", __func__, __LINE__, ret);
		goto error_1;
	}

	v4l2_ctrl_new_std_menu_items(hdl, &tp2912_ctrl_ops,
								 V4L2_CID_TEST_PATTERN,
								 ARRAY_SIZE(tp2912_test_pattern_menu) - 1, 0,
								 1 /* def */, tp2912_test_pattern_menu);
	v4l2_ctrl_new_std(hdl, &tp2912_ctrl_ops,
					  V4L2_CID_GAIN, -128, 127, 1, 0);
	v4l2_ctrl_new_custom(hdl, &tp2912_ctrl_diff_mode, NULL);
	v4l2_ctrl_new_custom(hdl, &tp2912_ctrl_current_mode, NULL);
	v4l2_ctrl_new_custom(hdl, &tp2912_ctrl_fhd_en, NULL);
	v4l2_ctrl_new_custom(hdl, &tp2912_ctrl_video_mode, NULL);

	ret = tp2912_init(priv);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed to init TP2912. Error = %d\n", __func__, __LINE__, ret);
		goto error_2;
	}

	ret = v4l2_ctrl_handler_setup(hdl);
	if(ret < 0) {
		v4l_err(client, "%s (line %d): failed v4l2_ctrl_handler_setup(). Error = %d\n", __func__, __LINE__, ret);
		goto error_1;
	}

	/* Register the v4l2_device structure */
	ret = v4l2_device_register(&client->dev, &priv->v4l2_dev);
	if (ret) {
		v4l_err(client, "%s (line %d): failed register v4l2-device. Error = %d\n", __func__, __LINE__, ret);
		goto error_2;
	}

	priv->v4l2_dev.ctrl_handler = hdl;
	priv->mdev.dev = &client->dev;
	priv->mdev.hw_revision = 10;
	strlcpy(priv->mdev.model, "TP2912", sizeof(priv->mdev.model));
	snprintf(priv->mdev.bus_info, sizeof(priv->mdev.bus_info), "platform:%s",
		 dev_name(priv->mdev.dev));

	media_device_init(&priv->mdev);

	ret = v4l2_device_register_subdev(&priv->v4l2_dev, sd);
	if (ret < 0) {
		v4l_err(client, "%s (line %d): failed to register subdev. Error = %d\n", __func__, __LINE__, ret);
		goto error_3;
	}

	ret = v4l2_device_register_subdev_nodes(&priv->v4l2_dev);
	if (ret < 0) {
		v4l_err(client, "%s (line %d): failed to register subdev nodes. Error = %d\n", __func__, __LINE__, ret);
		goto error_4;
	}

	ret = media_device_register(&priv->mdev);
	if(ret) {
		v4l_err(client, "%s (line %d): failed register media device. Error = %d\n", __func__, __LINE__, ret);
		goto error_4;
	}

	return ret; 

error_4:
	v4l2_device_unregister_subdev(sd);
error_3:
	v4l2_device_unregister(&priv->v4l2_dev);
error_2:
	v4l2_ctrl_handler_free(hdl);
error_1:
	media_entity_cleanup(&sd->entity);
	return ret;
}

static int tp2912_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct tp2912_priv *priv = sd_to_priv(sd);
	struct v4l2_ctrl_handler *hdl = &priv->hdl;

	v4l2_device_unregister(&priv->v4l2_dev);
	v4l2_ctrl_handler_free(hdl);
	media_entity_cleanup(&sd->entity);
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