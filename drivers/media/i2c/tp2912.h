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

#ifndef _TP219_H_
#define _TP219_H_

enum {
	TP2801A = 0,
	TP2801B,
	TP2803,
	TP2910,
	TP2912,
	TP2912B,
	TP2915
};

enum {
	AHD = 0,
	TVI,
	VIDEO_MODE_NUM
};

#define TABLE_HEADER_SIZE 2
enum {
	WIDTH_INDEX = 0,
	HEIGHT_INDEX,
	FPS_INDEX,
	CHIPID_BITMASK_INDEX,
	MODE_INDEX,
	TABLE_INDEX,
	COL_INDEX,
};

#define V4L2_CID_TP2912_DIFF_MODE		(V4L2_CID_DV_CLASS_BASE + 0x1000)
#define V4L2_CID_TP2912_VIDEO_MODE		(V4L2_CID_DV_CLASS_BASE + 0x1001)

#define TP2912_MAX_WIDTH 1920
#define TP2912_MAX_HEIGHT 1200
#define TP2912_MIN_PIXELCLOCK 20000000
#define TP2912_MAX_PIXELCLOCK 225000000

#define LINE_NUM_INTR 640

/* Clock Control Register */
#define REG_CLK 0x00

/* Mode Control Register */
#define REG_MODE 0x02

/* Encoding mode control register */
#define REG_ENC_MODE 0x03

/* Frequency Register */
#define REG_FREQ_HIGH 0x01
#define REG_FREQ_LOW 0x04

/* Burst Length Control Register */
#define REG_BSTLN 0x05

/* Input/Output Control Register */
#define REG_INOUT 0x06

/* Misc Control Register */
#define REG_MISC1 0x07

/* Video To H0 Timing Control Register */
#define REG_VDTH0_HIGH 0x0B
#define REG_VDTH0_LOW 0x08

/* Negative Sync Amplitude Register */
#define REG_SYLO 0x09

/* Positive Sync Amplitude Register */
#define REG_SYHI 0x0A

/* H0 To VS Timing Control Register */
#define REG_H0TVS_HIGH 0x0B
#define REG_H0TVS_LOW 0x0C

/* H0 To VD Timing Control Register */
#define REG_H0TVD_HIGH 0x0B
#define REG_H0TVD_LOW 0x0D

/* H0 To Burst Control Register */
#define REG_H0TBST_HIGH 0x0E

/* Horizontal Length Control Register */
#define REG_HLEN_3 0x29
#define REG_HLEN_2 0x10
#define REG_HLEN_1 0x0F

/* Output H-delay Control Register */
#define REG_HINDLY_HIGH 0x10
#define REG_HINDLY_LOW 0x11

/* Output V-delay Control Register */
#define REG_VINDLY 0x12

/* Blank Level Control Register */
#define REG_BKLVL 0x13

/* Positive Sync Amplitude Control Register */
#define REG_SYPHT 0x14

/* Negative Sync Amplitude Control Register */
#define REG_SYNHT 0x15

/* Y High Limit Register */
#define REG_YLMHI 0x16

/* Y Low Limit Register */
#define REG_YLMLO 0x17

/* Y Offset Control Register */
#define REG_YOFFSET 0x18

/* C High Limit Register */
#define REG_CLMHI 0x19

/* C Low Limit Register */
#define REG_CLMLO 0x1A

/* Y Channel Gain Control Register */
#define REG_YGAIN 0x1B

/* Cb Channel Gain Control Register */
#define REG_UGAIN 0x1C

/* Cr Channel Gain Control Register */
#define REG_VGAIN 0x1D

/* Color Burst Gain Control Register */
#define REG_BGAIN 0x1E

/* Test Control Register */
#define REG_TEST 0x1F

/* Frequency Control Register */
#define REG_FS_4 0x20
#define REG_FS_3 0x21
#define REG_FS_2 0x22
#define REG_FS_1 0x23

/* Vertical Frame Size Control Register */
#define REG_VLEN_HIGH 0x24
#define REG_VLEN_LOW 0x25

/* Output H Active Control Register */
#define REG_HACT_HIGH 0x24
#define REG_HACT_LOW 0x26

/* Vertical Active Control Register */
#define REG_VACT_HIGH 0x27
#define REG_VACT_LOW 0x28

/* Miscellaneous Control Register */
#define REG_MISC2 0x29

/* Miscellaneous Control Register */
#define REG_MISC3 0x2A

/* Format Control Extension Register */
#define REG_FMT_EXT 0x2D

/* External Sync Input Control Register */
#define REG_EX_SYNCIN 0x2E

/* LVLDI Delay Control Register */
#define REG_HSDLY_HIGH 0x35
#define REG_HSDLY_LOW 0x2F

/* FVLDI Delay Control Register */
#define REG_VSDLY 0x30

/* EVDelay Register */
#define REG_EVDELAY 0x35

/* EHDelay Register */
#define REG_EHDELAY 0x36

/* PTZ Control 1 Register */
#define REG_PTZ_1 0x3A

/* PTZ Control 2 Register */
#define REG_PTZ_2 0x3B

/* TX Driver Control 1 Register */
#define REG_TXDRIVER_1 0x3C

/* TX Driver Control 2 Register */
#define REG_TXDRIVER_2 0x3D

/* TX Driver Control 3 Register */
#define REG_TXDRIVER_3 0x3E

/* RCOSC Counter Register */
#define REG_RCOSC_CNT 0x3F

/* Video DAC Control Register */
#define REG_DAC 0x45

/* TX Data HLEVEL Control Register */
#define REG_TXHLEVEL 0x46

/* TX Data Bit Cell Width Register */
#define REG_TXBITCKNUM 0x47

/* TX Data H Position Control Register */
#define REG_TXHST_HIGH 0x9B
#define REG_TXHST_LOW 0x48

/* TX Data LINE1 Location Register */
#define REG_TXLINE1_HIGH 0x49
#define REG_TXLINE1_LOW 0x4A

/* TX Data LINE2 Location Register */
#define REG_TXLINE2_HIGH 0x49
#define REG_TXLINE2_LOW 0x4B

/* TX DATA2 Value Register */
#define REG_TXDATA2_6 0x9D
#define REG_TXDATA2_5 0x4C
#define REG_TXDATA2_4 0x4D
#define REG_TXDATA2_3 0x4E
#define REG_TXDATA2_2 0x4F
#define REG_TXDATA2_1 0x50

/* TX DATA1 Value Register */
#define REG_TXDATA1_6 0x9C
#define REG_TXDATA1_5 0x51
#define REG_TXDATA1_4 0x52
#define REG_TXDATA1_3 0x53
#define REG_TXDATA1_2 0x54
#define REG_TXDATA1_1 0x55

/* TX Enable and BITNUM Register */
#define REG_TXENBITNUM 0x56

/* RX Bit Cell Width Register */
#define REG_RXFREQ_3 0x57
#define REG_RXFREQ_2 0x58
#define REG_RXFREQ_1 0x59

/* RX High Detection Control Register */
#define REG_RRXDETMD_RXHINUM 0x5A

/* RX Data LINE1 Location Register */
#define REG_RXLINE1_HIGH 0x5B
#define REG_RXLINE1_LOW 0x5C

/* RX Data LINE2 Location Register */
#define REG_RXLINE2_HIGH 0x5B
#define REG_RXLINE2_LOW 0x5D

/* RX Data Enable and BITNUM Register */
#define REG_RXENBITNUM 0x5E

/* RX DATA1 Register */
#define REG_RXDATA1_6 0x8A
#define REG_RXDATA1_5 0x5F
#define REG_RXDATA1_4 0x60
#define REG_RXDATA1_3 0x61
#define REG_RXDATA1_2 0x62
#define REG_RXDATA1_1 0x63

/* RX DATA2 Register */
#define REG_RXDATA2_6 0x8B
#define REG_RXDATA2_5 0x64
#define REG_RXDATA2_4 0x65
#define REG_RXDATA2_3 0x66
#define REG_RXDATA2_2 0x67
#define REG_RXDATA2_1 0x68

/* INT Status Register */
#define REG_INT_STATUS 0x69

/* INT Length Register */
#define REG_INTLEN 0x6A

/* INT Control Register */
#define REG_INTCTRL 0x6B

/* Line Count INT Register */
#define REG_INTLINE_3 0x6D
#define REG_INTLINE_2 0x6B
#define REG_INTLINE_1 0x6C

/* INT Mode Register */
#define REG_INTMODE 0x6D

/* TX Data LINE3 Location Register */
#define REG_TXLINE3_HIGH 0x6E
#define REG_TXLINE3_LOW 0x6F

/* TX Data LINE4 Location Register */
#define REG_TXLINE4_HIGH 0x6E
#define REG_TXLINE4_LOW 0x70

/* TX DATA4 Value Register */
#define REG_TXDATA4_6 0x9F
#define REG_TXDATA4_5 0x71
#define REG_TXDATA4_4 0x72
#define REG_TXDATA4_3 0x73
#define REG_TXDATA4_2 0x74
#define REG_TXDATA4_1 0x75

/* TX DATA3 Value Register */
#define REG_TXDATA3_6 0x9E
#define REG_TXDATA3_5 0x76
#define REG_TXDATA3_4 0x77
#define REG_TXDATA3_3 0x78
#define REG_TXDATA3_2 0x79
#define REG_TXDATA3_1 0x7A

/* RX Horizontal Control Register */
#define REG_RXHST_HIGH 0x8E
#define REG_RXHST_LOW 0x7B

/* RX Data LINE3 Location Register */
#define REG_RXLINE3_HIGH 0x7C
#define REG_RXLINE3_LOW 0x7D

/* RX Data LINE4 Location Register */
#define REG_RXLINE4_HIGH 0x7C
#define REG_RXLINE4_LOW 0x7E

/* RXDATA3 Register */
#define REG_RXDATA3_6 0x8C
#define REG_RXDATA3_5 0x7F
#define REG_RXDATA3_4 0x80
#define REG_RXDATA3_3 0x81
#define REG_RXDATA3_2 0x82
#define REG_RXDATA3_1 0x83

/* RXDATA4 Register */
#define REG_RXDATA4_6 0x8D
#define REG_RXDATA4_5 0x84
#define REG_RXDATA4_4 0x85
#define REG_RXDATA4_3 0x86
#define REG_RXDATA4_2 0x87
#define REG_RXDATA4_1 0x88

/* RX Line Status Register */
#define REG_RXLINE_STATUS 0x89

/* RX Data FIFO Status Register */
#define REG_RXFIFO_STATUS 0x90

/* RX Data FIFO Register */
#define REG_RXFIFO_DATA 0x91

/* RX Line1-4 CRC Status Register */
#define REG_RXLINE14_CRC_STATUS 0x92

/* RX Line5-6 CRC Status Register */
#define REG_RXLINE56_CRC_STATUS 0x93

/* RX Control Register */
#define REG_RXCTRL 0x94

/* RX Multi Frame Control Register */
#define REG_RXMULFRAME_CTRL 0x95

/* Transmit Data FIFO Status Register */
#define REG_TXFIFO_STATUS 0x97

/* TX Data FIFO Register */
#define REG_TXFIFO_DATA 0x98

/* TX Control Register */
#define REG_TXCTRL 0x99

/* TX Multi Frame Control Register */
#define REG_TXMULFRAME_CTRL 0x9A

/* Data Upgrade RX Bit Frequency Register */
#define REG_UG_FREQ_3 0xA0
#define REG_UG_FREQ_2 0xA1
#define REG_UG_FREQ_1 0xA2

/* Data Upgrade RX High Control */
#define REG_UG_HINUM 0xA3

/* Data Upgrade RX Bit Number Control */
#define REG_UG_BITNUM 0xA4

/* Data Upgrade RX Line Number Control */
#define REG_UG_LNUM 0xA5

/* Data Upgrade RX H Start Control */
#define REG_UG_HSART_HIGH 0xA6
#define REG_UG_HSART_LOW 0xA7

/* Data Upgrade RX V Start Control */
#define REG_UG_VSTART_HIGH 0xA6
#define REG_UG_VSTART_LOW 0xA8

/* Data Upgrade RX Format Control */
#define REG_UG_FMT 0xA9

/* Data Upgrade RX Received Data */
#define REG_UG_RXDATA 0xAA

/* Data Upgrade RX FIFO Status */
#define REG_UG_RXFIFO_STATUS 0xAB

/* PLL Analog Control I Register */
#define REG_PLLANCTRL_I 0xF1

/* PLL Analog Control I Register */
#define REG_PLLANCTRL_II 0xF2

/* PLL Digital Control I Register */
#define REG_PLLDICTRL_I 0xF3

/* PLL Digital Control II Register */
#define REG_PLLDICTRL_II 0xF4

/* PLL Digital Control III Register */
#define REG_PLLDICTRL_III 0xF5

/* PLL Lock Indicator I Register */
#define REG_PLLLOCK_I 0xF6

/* PLL Lock Indicator II Register */
#define REG_PLLLOCK_II 0xFA

/* Digital Input Voltage Control Register */
#define REG_DIINVOL 0xFB

/* Clock Control Register */
#define REG_CLKCTRL 0xFC

/* Revision and MISC Control Register */
#define REG_REV 0xFD

/* DEVICE_ID Register */
#define REG_DEVID_HIGH 0xFE
#define REG_DEVID_LOW 0xFF

/* TP2912, TP2912B and TP2915 use the same data set for AHD1080P */
uint8_t  TP2912_AHD1080P_DataSet[] = {
	/* Table header */
	3, /* Column count */
	25, /* Row count */

	/* Table data */
	/* Registers     1080P30       1080P25   */
	REG_MODE,         0x83,         0x83,
	REG_BSTLN,        0x4a,         0x4a,
	REG_VDTH0_LOW,    0x90,         0x90,
	REG_SYLO,         0x94,         0x94,
	REG_SYHI,         0x94,         0x94,
	REG_VDTH0_HIGH,   0x00,         0x00,
	REG_H0TVS_LOW,    0x04,         0x04,
	REG_H0TVD_LOW,    0xf0,         0xf0,
	REG_H0TBST_HIGH,  0x98,         0x98,
	REG_HLEN_1,       0x98,         0x50,
	REG_HINDLY_HIGH,  0x08,         0x1a,
	REG_HINDLY_LOW,   0x2a,         0xe2,
	REG_VINDLY,       0x00,         0x00,
	REG_YLMHI,        0xeb,         0xeb,
	REG_CLMHI,        0xf0,         0xf0,
	REG_CLMLO,        0x10,         0x10,
	REG_UGAIN,        0xa6,         0xa6,
	REG_VGAIN,        0xe6,         0xe6,
	REG_BGAIN,        0xf8,         0xf8,
	REG_FS_4,         0x29,         0x29,
	REG_FS_3,         0x65,         0x61,
	REG_FS_2,         0x78,         0xbe,
	REG_FS_1,         0x16,         0xd6,
	REG_MISC2,        0x37,         0x37,
	REG_TXDRIVER_1,   0x50,         0x50
};

/* TP2912B and TP2915 use the same data set for AHD720P */
uint8_t  TP2912_AHD720P_DataSet[] = {
	/* Table header */
	9, /* Column count */
	26, /* Row count */

	/* Table data */
	/*               |-----------------------------------------------------|-----------------------------------------------------|
	 *               |   TP2912                                            |            TP2912B                                  |
	 *|--------------|----------|------------|--------------|--------------|---------- |------------|---------------|------------|
	 *| Registers    | 720P30   |    720P25  |   720P30_36M |  720P25_36M  | 720P30    |    720P25  |    720P30_36M |   720P25   |
     *|--------------|----------|------------|--------------|--------------|---------- |------------|---------------|------------| */
	REG_MODE,         0x8b,         0x8b,        0x8b,         0x8b,         0x8b,         0x8b,        0x8b,         0x8b,
	REG_ENC_MODE,     0x42,         0x42,        0x42,         0x42,         0x42,         0x42,        0x42,         0x42,
	REG_BSTLN,        0x70,         0x70,        0x6c,         0x6c,         0x70,         0x70,        0x6c,         0x6c,
	REG_VDTH0_LOW,    0x7a,         0x7a,        0x76,         0x76,         0x7a,         0x7a,        0x76,         0x76,
	REG_SYLO,         0x7a,         0x7a,        0x76,         0x76,         0x7a,         0x7a,        0x76,         0x76,
	REG_SYHI,         0x7a,         0x7a,        0x76,         0x76,         0x7a,         0x7a,        0x76,         0x76,
	REG_VDTH0_HIGH,   0x00,         0x00,        0x00,         0x00,         0x00,         0x00,        0x00,         0x00,
	REG_H0TVS_LOW,    0x04,         0x04,        0x04,         0x04,         0x04,         0x04,        0x04,         0x04,
	REG_H0TVD_LOW,    0xf0,         0xf0,        0xf0,         0xf0,         0xf0,         0xf0,        0xf0,         0xf0,
	REG_H0TBST_HIGH,  0x86,         0x86,        0x82,         0x82,         0x86,         0x86,        0x82,         0x82,
	REG_HLEN_1,       0x72,         0xbc,        0x40,         0x80,         0x72,         0xbc,        0x40,         0x80,
	REG_HINDLY_HIGH,  0x06,         0x17,        0x06,         0x17,         0x06,         0x17,        0x06,         0x17,
	REG_HINDLY_LOW,   0x68,         0xb0,        0x3e,         0x80,         0x68,         0xb0,        0x3e,         0x80,
	REG_VINDLY,       0x00,         0x00,        0x00,         0x00,         0x00,         0x00,        0x00,         0x00,
	REG_YLMHI,        0xeb,         0xeb,        0xeb,         0xeb,         0xeb,         0xeb,        0xeb,         0xeb,
	REG_CLMHI,        0xf0,         0xf0,        0xf0,         0xf0,         0xf0,         0xf0,        0xf0,         0xf0,
	REG_CLMLO,        0x10,         0x10,        0x10,         0x10,         0x10,         0x10,        0x10,         0x10,
	REG_UGAIN,        0xa6,         0xa6,        0xa6,         0xa6,         0xa6,         0xa6,        0xa6,         0xa6,
	REG_VGAIN,        0xe6,         0xe6,        0xe6,         0xe6,         0xe6,         0xe6,        0xe6,         0xe6,
	REG_BGAIN,        0xf8,         0xf8,        0xf8,         0xf8,         0xf8,         0xf8,        0xf8,         0xf8,
	REG_FS_4,         0x27,         0x27,        0x28,         0x28,         0x27,         0x27,        0x28,         0x28,
	REG_FS_3,         0x72,         0x88,        0xae,         0xc4,         0x72,         0x88,        0xae,         0xc4,
	REG_FS_2,         0x80,         0x04,        0x14,         0x44,         0x80,         0x04,        0x14,         0x44,
	REG_FS_1,         0x77,         0x23,        0x7a,         0x44,         0x77,         0x23,        0x7a,         0x44,
	REG_MISC2,        0x37,         0x37,        0x37,         0x37,         0x37,         0x37,        0x37,         0x37,
	REG_TXDRIVER_1,   0x90,         0x90,        0x90,         0x90,         0x10,         0x10,        0x10,         0x10
};

uint8_t  TP2912_AHD4M_DataSet[] = {
	/* Table header */
	5, /* Column count */
	32, /* Row count */

	/* Table data */
	/*             |------------------------|--------------------------|
	 *             |         TP2912         |           TP2912B        |
	 *|------------|-----------|------------|--------------|-----------|
	 *| Registers  |  4M30     |    4M25    |    4M30      |   4M25    |
	 *|------------|-----------|------------|--------------|-----------|*/
	REG_MODE,         0x8b,         0x8b,        0x8b,         0x8b,
	REG_ENC_MODE,     0x52,         0x52,        0x52,         0x52,
	REG_BSTLN,        0xd0,         0xd0,        0xd0,         0xd0,
	REG_MISC1,        0x45,         0x45,        0x46,         0x46,
	REG_VDTH0_LOW,    0xd0,         0xd0,        0xd0,         0xd0,
	REG_SYLO,         0x98,         0x98,        0x98,         0x98,
	REG_SYHI,         0x98,         0x98,        0x98,         0x98,
	REG_VDTH0_HIGH,   0x12,         0x12,        0x12,         0x12,
	REG_H0TVS_LOW,    0x04,         0x04,        0x04,         0x04,
	REG_H0TVD_LOW,    0x04,         0x04,        0x04,         0x04,
	REG_H0TBST_HIGH,  0xb0,         0xb0,        0xb0,         0xb0,
	REG_HLEN_1,       0xe4,         0x78,        0xe4,         0x78,
	REG_HINDLY_HIGH,  0x1c,         0x3f,        0x1c,         0x3f,
	REG_HINDLY_LOW,   0x12,         0xa6,        0x12,         0xa6,
	REG_VINDLY,       0x0a,         0x0a,        0x0a,         0x0a,
	REG_CLMHI,        0xf0,         0xf0,        0xf0,         0xf0,
	REG_CLMLO,        0x10,         0x10,        0x10,         0x10,
	REG_UGAIN,        0x72,         0x72,        0x72,         0x72,
	REG_VGAIN,        0x9d,         0x9d,        0x9d,         0x9d,
	REG_BGAIN,        0x80,         0x80,        0x80,         0x80,
	REG_FS_4,         0x36,         0x36,        0x36,         0x36,
	REG_FS_3,         0x50,         0x53,        0x50,         0x53,
	REG_FS_2,         0x0e,         0xc8,        0x0e,         0xc8,
	REG_FS_1,         0xe5,         0x25,        0xe5,         0x25,
	REG_VLEN_HIGH,    0xa5,         0xa5,        0xa5,         0xa5,
	REG_VLEN_LOW,     0xdc,         0xdc,        0xdc,         0xdc,
	REG_HACT_LOW,     0x00,         0x00,        0x00,         0x00,
	REG_VACT_HIGH,    0x55,         0x55,        0x55,         0x55,
	REG_VACT_LOW,     0xa0,         0xa0,        0xa0,         0xa0,
	REG_MISC2,        0x36,         0x36,        0x36,         0x36,
	REG_MISC3,        0xd7,         0xd7,        0xd7,         0xd7,
	REG_TXDRIVER_1,   0x10,         0x10,        0x90,         0x90
};

uint8_t  TP2910_AHD1080P_DataSet[] = {
	/* Table header */
	3, /* Column count */
	18, /* Row count */

	/* Table data */
	/* Registers     1080P30       1080P25   */
	REG_BSTLN,        0x4a,         0x4a,
	REG_VDTH0_LOW,    0xc0,         0xc0,
	REG_SYLO,         0x94,         0x94,
	REG_SYHI,         0x94,         0x94,
	REG_VDTH0_HIGH,   0x10,         0x10,
	REG_H0TVS_LOW,    0x02,         0x02,
	REG_H0TVD_LOW,    0x64,         0x64,
	REG_H0TBST_HIGH,  0x94,         0x94,
	REG_HLEN_1,       0x98,         0x50,
	REG_HINDLY_HIGH,  0x08,         0x1a,
	REG_HINDLY_LOW,   0x28,         0xe0,
	REG_VINDLY,       0x00,         0x00,
	REG_FS_4,         0x29,         0x29,
	REG_FS_3,         0x65,         0x61,
	REG_FS_2,         0x78,         0x78,
	REG_FS_1,         0x16,         0x16,
	REG_MISC2,        0x35,         0x35,
	REG_TXDRIVER_1,   0x00,         0x00
};

uint8_t  TP2910_AHD720P_DataSet[] = {
	/* Table header */
	3, /* Column count */
	13, /* Row count */

	/* Table data */
	/* Registers     720P30       720P25   */
	REG_MODE,         0x8f,         0x9f,
	REG_BSTLN,        0x60,         0xb8,
	REG_SYLO,         0x80,         0x8c,
	REG_VDTH0_HIGH,   0x05,         0x15,
	REG_H0TBST_HIGH,  0x80,         0xbc,
	REG_HLEN_1,       0x72,         0x17,
	REG_HINDLY_HIGH,  0x06,         0xcd,
	REG_FS_4,         0x27,         0x27,
	REG_FS_3,         0x72,         0x88,
	REG_FS_2,         0x80,         0x04,
	REG_FS_1,         0x77,         0x23,
	REG_MISC2,        0x35,         0x35,
	REG_TXDRIVER_1,   0x10,         0x10
};

uint8_t  TP2912_TVI1080P_DataSet[] = {
	/* Table header */
	3, /* Column count */
	21, /* Row count */

	/* Table data */
	/* Registers     1080P30       1080P25   */
	REG_MODE,         0x83,         0x93,
	REG_ENC_MODE,     0x42,         0x42,
	REG_BSTLN,        0x40,         0x40,
	REG_VDTH0_LOW,    0x58,         0x10,
	REG_SYLO,         0x2c,         0x2c,
	REG_SYHI,         0x2c,         0x2c,
	REG_VDTH0_HIGH,   0x00,         0x20,
	REG_H0TVS_LOW,    0xc0,         0xc0,
	REG_H0TVD_LOW,    0xc0,         0xc0,
	REG_HLEN_1,       0x98,         0x50,
	REG_HINDLY_HIGH,  0x08,         0x2a,
	REG_HINDLY_LOW,   0x6c,         0x24,
	REG_VINDLY,       0x04,         0x04,
	REG_UGAIN,        0xa6,         0xa6,
	REG_VGAIN,        0xe6,         0xe6,
	REG_BGAIN,        0xf8,         0xf8,
	REG_FS_4,         0x48,         0x48,
	REG_FS_3,         0xbb,         0xbb,
	REG_FS_2,         0x2e,         0x2e,
	REG_FS_1,         0x8b,         0x8b,
	REG_TXDRIVER_1,   0x50,         0x50
};

uint8_t  TP2912_TVI720P_DataSet[] = {
	/* Table header */
	5, /* Column count */
	23, /* Row count */

	/* Table data */
	/* Registers     720P60       720P50         720P30V2       720P25V2 */
	REG_MODE,         0x8b,         0x9b,         0x8b,          0x9b,
	REG_ENC_MODE,     0x42,         0x42,         0x4a,          0x4a,
	REG_BSTLN,        0x46,         0x46,         0x46,          0x46,
	REG_MISC1,        0x41,         0x41,         0x41,          0x41,
	REG_VDTH0_LOW,    0x6e,         0xb8,         0x6e,          0xb8,
	REG_SYLO,         0x28,         0x28,         0x28,          0x28,
	REG_SYHI,         0x28,         0x28,         0x28,          0x28,
	REG_VDTH0_HIGH,   0x05,         0x15,         0x05,          0x05,
	REG_H0TVS_LOW,    0x04,         0x04,         0x04,          0x04,
	REG_H0TVD_LOW,    0x04,         0x04,         0x04,          0x04,
	REG_H0TBST_HIGH,  0x28,         0x28,         0x28,          0x28,
	REG_HLEN_1,       0x72,         0xbc,         0x72,          0xbc,
	REG_HINDLY_HIGH,  0x06,         0x17,         0x06,          0x17,
	REG_HINDLY_LOW,   0x83,         0xcd,         0x83,          0xcd,
	REG_VINDLY,       0x05,         0x05,         0x05,          0x05,
	REG_UGAIN,        0xa6,         0xa6,         0xa6,          0xa6,
	REG_VGAIN,        0xe6,         0xe6,         0xe6,          0xe6,
	REG_BGAIN,        0xf8,         0xf8,         0xf8,          0xf8,
	REG_FS_4,         0x48,         0x48,         0x24,          0x24,
	REG_FS_3,         0xbb,         0xbb,         0x5d,          0x5d,
	REG_FS_2,         0x2e,         0x2e,         0x17,          0x17,
	REG_FS_1,         0x8b,         0x8b,         0x45,          0x45,
	REG_TXDRIVER_1,   0x50,         0x50,         0x90,          0x90
};

uint8_t TP2912_TVINTSC_DataSet[] = {
	/* Table header */
	2, /* Column count */
	26, /* Row count */

	/* Table data */
	REG_MODE,         0x09,
	REG_BSTLN,        0x24,
	REG_MISC1,        0x81,
	REG_VDTH0_LOW,    0x20,
	REG_SYLO,         0x42,
	REG_SYHI,         0x28,
	REG_VDTH0_HIGH,   0x05,
	REG_H0TVS_LOW,    0x04,
	REG_H0TVD_LOW,    0x04,
	REG_H0TBST_HIGH,  0x3c,
	REG_HLEN_1,       0x5a,
	REG_HINDLY_HIGH,  0x03,
	REG_HINDLY_LOW,   0x16,
	REG_VINDLY,       0x04,
	REG_SYNHT,        0x2a,
	REG_YGAIN,        0x77,
	REG_UGAIN,        0xaa,
	REG_VGAIN,        0xf0,
	REG_BGAIN,        0xa9,
	REG_FS_4,         0x48,
	REG_FS_3,         0xbb,
	REG_FS_2,         0x2e,
	REG_FS_1,         0x8b,
	REG_TXDRIVER_1,   0x90,
	0x41,             0xc1,
	REG_DAC,          0x8c
};

uint8_t TP2912_TVIPAL_DataSet[] = {
	/* Table header */
	2, /* Column count */
	28, /* Row count */

	/* Table data */
	REG_MODE,         0x1a,
	REG_ENC_MODE,     0x43,
	REG_BSTLN,        0x20,
	REG_MISC1,        0x81,
	REG_VDTH0_LOW,    0x20,
	REG_SYLO,         0x42,
	REG_SYHI,         0x28,
	REG_VDTH0_HIGH,   0x05,
	REG_H0TVS_LOW,    0x04,
	REG_H0TVD_LOW,    0x04,
	REG_H0TBST_HIGH,  0x3c,
	REG_HLEN_1,       0x60,
	REG_HINDLY_HIGH,  0x03,
	REG_HINDLY_LOW,   0x20,
	REG_VINDLY,       0x04,
	REG_BKLVL,        0x3f,
	REG_SYNHT,        0x2c,
	REG_YGAIN,        0x77,
	REG_UGAIN,        0xaa,
	REG_VGAIN,        0xf0,
	REG_BGAIN,        0x76,
	REG_FS_4,         0x48,
	REG_FS_3,         0xbb,
	REG_FS_2,         0x2e,
	REG_FS_1,         0x8b,
	REG_TXDRIVER_1,   0x90,
	0x41,             0xc1,
	REG_DAC,          0x8c
};

uint8_t TP2912_TVI8M15_DataSet[] = {
	/* Table header */
	2, /* Column count */
	29, /* Row count */

	/* Table data */
	REG_MODE,         0x8b,
	REG_ENC_MODE,     0x52,
	REG_BSTLN,        0x78,
	REG_MISC1,        0x45,
	REG_VDTH0_LOW,    0x80,
	REG_SYLO,         0x50,
	REG_SYHI,         0x50,
	REG_VDTH0_HIGH,   0x15,
	REG_H0TVS_LOW,    0x00,
	REG_H0TVD_LOW,    0x80,
	REG_H0TBST_HIGH,  0x50,
	REG_HLEN_1,       0x30,
	REG_HINDLY_HIGH,  0x01,
	REG_HINDLY_LOW,   0xaa,
	REG_VINDLY,       0x08,
	REG_UGAIN,        0x55,
	REG_VGAIN,        0x76,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x57,
	REG_FS_3,         0x43,
	REG_FS_2,         0x4e,
	REG_FS_1,         0x02,
	REG_VLEN_HIGH,    0xf8,
	REG_VLEN_LOW,     0xca,
	REG_HACT_LOW,     0x00,
	REG_VACT_HIGH,    0x58,
	REG_VACT_LOW,     0x70,
	REG_MISC2,        0xb4,
	REG_TXDRIVER_1,   0x10
};

uint8_t TP2912_TVI8M125_DataSet[] = {
	/* Table header */
	2, /* Column count */
	29, /* Row count */

	/* Table data */
	REG_MODE,         0x8b,
	REG_ENC_MODE,     0x52,
	REG_BSTLN,        0x78,
	REG_MISC1,        0x45,
	REG_VDTH0_LOW,    0xe0,
	REG_SYLO,         0x50,
	REG_SYHI,         0x50,
	REG_VDTH0_HIGH,   0x35,
	REG_H0TVS_LOW,    0x00,
	REG_H0TVD_LOW,    0x80,
	REG_H0TBST_HIGH,  0x50,
	REG_HLEN_1,       0xa0,
	REG_HINDLY_HIGH,  0x44,
	REG_HINDLY_LOW,   0x18,
	REG_VINDLY,       0x08,
	REG_UGAIN,        0x55,
	REG_VGAIN,        0x76,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x57,
	REG_FS_3,         0x43,
	REG_FS_2,         0x4e,
	REG_FS_1,         0x02,
	REG_VLEN_HIGH,    0xf8,
	REG_VLEN_LOW,     0xca,
	REG_HACT_LOW,     0x00,
	REG_VACT_HIGH,    0x58,
	REG_VACT_LOW,     0x70,
	REG_MISC2,        0xb4,
	REG_TXDRIVER_1,   0x10
};

uint8_t TP2912_TVI5M20_DataSet[] = {
	/* Table header */
	2, /* Column count */
	29, /* Row count */

	/* Table data */
	REG_MODE,         0x8b,
	REG_ENC_MODE,     0x52,
	REG_BSTLN,        0x80,
	REG_MISC1,        0x45,
	REG_VDTH0_LOW,    0x80,
	REG_SYLO,         0x50,
	REG_SYHI,         0x50,
	REG_VDTH0_HIGH,   0x21,
	REG_H0TVS_LOW,    0xe0,
	REG_H0TVD_LOW,    0x80,
	REG_H0TBST_HIGH,  0x50,
	REG_HLEN_1,       0xa6,
	REG_HINDLY_HIGH,  0x3e,
	REG_HINDLY_LOW,   0x00,
	REG_VINDLY,       0x04,
	REG_UGAIN,        0x55,
	REG_VGAIN,        0x76,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x57,
	REG_FS_3,         0x7d,
	REG_FS_2,         0x52,
	REG_FS_1,         0x3b,
	REG_VLEN_HIGH,    0xa7,
	REG_VLEN_LOW,     0xbc,
	REG_HACT_LOW,     0x20,
	REG_VACT_HIGH,    0x57,
	REG_VACT_LOW,     0x98,
	REG_MISC2,        0x34,
	REG_TXDRIVER_1,   0x10
};

uint8_t TP2912_TVI4M_DataSet[] = {
	/* Table header */
	3, /* Column count */
	29, /* Row count */

	/* Table data */
	/*             |------------------------|
	 *             |         TP2912         |
	 *|------------|-----------|------------|
	 *| Registers  |  4M30     |    4M25    |
	 *|------------|-----------|------------|*/
	REG_MODE,         0x8b,        0x8b,
	REG_ENC_MODE,     0x52,        0x52,
	REG_BSTLN,        0x80,        0x80,
	REG_MISC1,        0x45,        0x45,
	REG_VDTH0_LOW,    0xe0,        0xe0,
	REG_SYLO,         0x50,        0x50,
	REG_SYHI,         0x50,        0x50,
	REG_VDTH0_HIGH,   0x0a,        0x3a,
	REG_H0TVS_LOW,    0x08,        0x08,
	REG_H0TVD_LOW,    0x08,        0x08,
	REG_H0TBST_HIGH,  0x50,        0x50,
	REG_HLEN_1,       0xe4,        0x78,
	REG_HINDLY_HIGH,  0x1c,        0x3f,
	REG_HINDLY_LOW,   0x00,        0x90,
	REG_VINDLY,       0x05,        0x05,
	REG_UGAIN,        0x55,        0x55,
	REG_VGAIN,        0x76,        0x76,
	REG_BGAIN,        0x80,        0x80,
	REG_FS_4,         0x57,        0x57,
	REG_FS_3,         0x42,        0x42,
	REG_FS_2,         0x77,        0x77,
	REG_FS_1,         0x87,        0x87,
	REG_VLEN_HIGH,    0xa5,        0xa5,
	REG_VLEN_LOW,     0xdc,        0xdc,
	REG_HACT_LOW,     0x00,        0x00,
	REG_VACT_HIGH,    0x55,        0x55,
	REG_VACT_LOW,     0xa0,        0xa0,
	REG_MISC2,        0x34,        0x34,
	REG_TXDRIVER_1,   0x10,        0x10
};

uint8_t TP2912B_TVI1080P60_DataSet[] = {
	/* Table header */
	2, /* Column count */
	35, /* Row count */

	/* Table data */
	/* Registers     1080P60 */
	REG_MODE,         0x8b,
	REG_ENC_MODE,     0x92,
	REG_BSTLN,        0x64,
	REG_MISC1,        0x46,
	REG_VDTH0_LOW,    0x80,
	REG_SYLO,         0x40,
	REG_SYHI,         0x40,
	REG_VDTH0_HIGH,   0x00,
	REG_H0TVS_LOW,    0xc0,
	REG_H0TVD_LOW,    0xc0,
	REG_H0TBST_HIGH,  0x38,
	REG_HLEN_1,       0x98,
	REG_HINDLY_HIGH,  0x08,
	REG_HINDLY_LOW,   0x70,
	REG_VINDLY,       0x04,
	REG_BKLVL,        0x3c,
	REG_SYNHT,        0x38,
	REG_UGAIN,        0x55,
	REG_VGAIN,        0x76,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x57,
	REG_FS_3,         0x74,
	REG_FS_2,         0xbc,
	REG_FS_1,         0x6a,
	REG_HACT_HIGH,    0x74,
	REG_VLEN_LOW,     0x65,
	REG_HACT_LOW,     0x80,
	REG_VACT_HIGH,    0x54,
	REG_VACT_LOW,     0x38,
	REG_MISC2,        0x35,
	REG_MISC3,        0x39,
	REG_PTZ_2,        0x90,
	REG_TXDRIVER_1,   0x90,
	REG_DAC,          0x41,
	REG_PLLDICTRL_II, 0x21
};

uint8_t TP2912B_TVI1080P3025_DataSet[] = {
	/* Table header */
	3, /* Column count */
	27, /* Row count */

	/* Table data */
	/* Registers     1080P30       1080P25   */
	REG_MODE,         0x83,         0x93,
	REG_ENC_MODE,     0x42,         0x42,
	REG_BSTLN,        0x40,         0x40,
	REG_MISC1,        0x42,         0x42,
	REG_VDTH0_LOW,    0x58,         0x10,
	REG_SYLO,         0x2c,         0x2c,
	REG_SYHI,         0x2c,         0x2c,
	REG_VDTH0_HIGH,   0x00,         0x20,
	REG_H0TVS_LOW,    0xc0,         0xc0,
	REG_H0TVD_LOW,    0xc0,         0xc0,
	REG_H0TBST_HIGH,  0x28,         0x28,
	REG_HLEN_1,       0x98,         0x50,
	REG_HINDLY_HIGH,  0x08,         0x2a,
	REG_HINDLY_LOW,   0x6c,         0x24,
	REG_VINDLY,       0x04,         0x04,
	REG_BKLVL,        0x3c,         0x3c,
	REG_SYNHT,        0x38,         0x38,
	REG_FS_4,         0x48,         0x48,
	REG_FS_3,         0xbb,         0xbb,
	REG_FS_2,         0x2e,         0x2e,
	REG_FS_1,         0x8b,         0x8b,
	REG_MISC2,        0x35,         0x35,
	REG_MISC3,        0x19,         0x19,
	REG_PTZ_2,        0xd0,         0xd0,
	REG_TXDRIVER_1,   0x50,         0x50,
	REG_DAC,          0x40,         0x40,
	REG_PLLDICTRL_II, 0x21,         0x21
};

uint8_t TP2912B_TVI720P_DataSet[] = {
	/* Table header */
	5, /* Column count */
	27, /* Row count */

	/* Table data */
	/* Registers     720P60      720P50        720P30V2     720P25V2 */
	REG_MODE,         0x8b,        0x9b,        0x8b,        0x9b,
	REG_ENC_MODE,     0x42,        0x42,        0x4a,        0x4a,
	REG_BSTLN,        0x46,        0x46,        0x46,        0x46,
	REG_MISC1,        0x42,        0x42,        0x42,        0x42,
	REG_VDTH0_LOW,    0x6e,        0xb8,        0x6e,        0xb8,
	REG_SYLO,         0x28,        0x28,        0x28,        0x28,
	REG_SYHI,         0x28,        0x28,        0x28,        0x28,
	REG_VDTH0_HIGH,   0x05,        0x15,        0x05,        0x05,
	REG_H0TVS_LOW,    0x04,        0x04,        0x04,        0x04,
	REG_H0TVD_LOW,    0x04,        0x04,        0x04,        0x04,
	REG_H0TBST_HIGH,  0x28,        0x28,        0x28,        0x28,
	REG_HLEN_1,       0x72,        0xbc,        0x72,        0xbc,
	REG_HINDLY_HIGH,  0x06,        0x17,        0x06,        0x17,
	REG_HINDLY_LOW,   0x83,        0xcd,        0x83,        0xcd,
	REG_VINDLY,       0x05,        0x05,        0x05,        0x05,
	REG_BKLVL,        0x3c,        0x3c,        0x3c,        0x3c,
	REG_SYNHT,        0x38,        0x38,        0x38,        0x38,
	REG_FS_4,         0x48,        0x48,        0x24,        0x24,
	REG_FS_3,         0xbb,        0xbb,        0x5d,        0x5d,
	REG_FS_2,         0x2e,        0x2e,        0x17,        0x17,
	REG_FS_1,         0x8b,        0x8b,        0x45,        0x45,
	REG_MISC2,        0x35,        0x35,        0x35,        0x35,
	REG_MISC3,        0x19,        0x19,        0x19,        0x19,
	REG_PTZ_2,        0xd0,        0xd0,        0xd0,        0xd0,
	REG_TXDRIVER_1,   0x50,        0x50,        0x10,        0x10,
	REG_DAC,          0x40,        0x40,        0x40,        0x40,
	REG_PLLDICTRL_II, 0x21,        0x21,        0x21,        0x21
};

uint8_t TP2912B_TVINTSC_DataSet[] = {
	/* Table header */
	2, /* Column count */
	31, /* Row count */

	/* Table data */
	/* Registers     NTSC */
	REG_MODE,         0x09,
	REG_ENC_MODE,     0x62,
	REG_BSTLN,        0x24,
	REG_MISC1,        0x82,
	REG_VDTH0_LOW,    0x20,
	REG_SYLO,         0x42,
	REG_SYHI,         0x28,
	REG_VDTH0_HIGH,   0x05,
	REG_H0TVS_LOW,    0x04,
	REG_H0TVD_LOW,    0x04,
	REG_H0TBST_HIGH,  0x3c,
	REG_HLEN_1,       0x5a,
	REG_HINDLY_HIGH,  0x03,
	REG_HINDLY_LOW,   0x16,
	REG_VINDLY,       0x04,
	REG_BKLVL,        0x3c,
	REG_SYNHT,        0x38,
	REG_YGAIN,        0x97,
	REG_UGAIN,        0x81,
	REG_VGAIN,        0xb6,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x48,
	REG_FS_3,         0xbb,
	REG_FS_2,         0x2e,
	REG_FS_1,         0x8b,
	REG_MISC2,        0x34,
	REG_MISC3,        0x19,
	REG_PTZ_2,        0xd0,
	REG_TXDRIVER_1,   0x10,
	REG_DAC,          0x40,
	REG_PLLDICTRL_II, 0xa1
};

uint8_t TP2912B_TVIPAL_DataSet[] = {
	/* Table header */
	2, /* Column count */
	31, /* Row count */

	/* Table data */
	/* Registers     PAL */
	REG_MODE,         0x1a,
	REG_ENC_MODE,     0x63,
	REG_BSTLN,        0x20,
	REG_MISC1,        0x82,
	REG_VDTH0_LOW,    0x20,
	REG_SYLO,         0x42,
	REG_SYHI,         0x28,
	REG_VDTH0_HIGH,   0x05,
	REG_H0TVS_LOW,    0x04,
	REG_H0TVD_LOW,    0x04,
	REG_H0TBST_HIGH,  0x3c,
	REG_HLEN_1,       0x60,
	REG_HINDLY_HIGH,  0x03,
	REG_HINDLY_LOW,   0x20,
	REG_VINDLY,       0x04,
	REG_BKLVL,        0x3f,
	REG_SYNHT,        0x3b,
	REG_YGAIN,        0xa0,
	REG_UGAIN,        0x88,
	REG_VGAIN,        0xc1,
	REG_BGAIN,        0x5f,
	REG_FS_4,         0x48,
	REG_FS_3,         0xbb,
	REG_FS_2,         0x2e,
	REG_FS_1,         0x8b,
	REG_MISC2,        0x34,
	REG_MISC3,        0x19,
	REG_PTZ_2,        0xd0,
	REG_TXDRIVER_1,   0x10,
	REG_DAC,          0x40,
	REG_PLLDICTRL_II, 0xa1
};

uint8_t TP2912B_TVI8M125_DataSet[] = {
	/* Table header */
	2, /* Column count */
	35, /* Row count */

	/* Table data */
	/* Registers     8M125 */
	REG_MODE,         0x8b,
	REG_ENC_MODE,     0x52,
	REG_BSTLN,        0x78,
	REG_MISC1,        0x46,
	REG_VDTH0_LOW,    0xe0,
	REG_SYLO,         0x50,
	REG_SYHI,         0x50,
	REG_VDTH0_HIGH,   0x35,
	REG_H0TVS_LOW,    0x00,
	REG_H0TVD_LOW,    0x80,
	REG_H0TBST_HIGH,  0x50,
	REG_HLEN_1,       0xa0,
	REG_HINDLY_HIGH,  0x44,
	REG_HINDLY_LOW,   0x18,
	REG_VINDLY,       0x08,
	REG_BKLVL,        0x3c,
	REG_SYNHT,        0x38,
	REG_UGAIN,        0x55,
	REG_VGAIN,        0x76,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x57,
	REG_FS_3,         0x43,
	REG_FS_2,         0x4e,
	REG_FS_1,         0x02,
	REG_HACT_HIGH,    0xf8,
	REG_VLEN_LOW,     0xca,
	REG_HACT_LOW,     0x00,
	REG_VACT_HIGH,    0x58,
	REG_VACT_LOW,     0x70,
	REG_MISC2,        0xb5,
	REG_MISC3,        0x39,
	REG_PTZ_2,        0x90,
	REG_TXDRIVER_1,   0x90,
	REG_DAC,          0x41,
	REG_PLLDICTRL_II, 0x21
};

uint8_t TP2912B_TVI5M20_DataSet[] = {
	/* Table header */
	2, /* Column count */
	35, /* Row count */

	/* Table data */
	/* Registers     5M20 */
	REG_MODE,         0x8b,
	REG_ENC_MODE,     0x52,
	REG_BSTLN,        0x80,
	REG_MISC1,        0x46,
	REG_VDTH0_LOW,    0x80,
	REG_SYLO,         0x50,
	REG_SYHI,         0x50,
	REG_VDTH0_HIGH,   0x21,
	REG_H0TVS_LOW,    0xe0,
	REG_H0TVD_LOW,    0x80,
	REG_H0TBST_HIGH,  0x50,
	REG_HLEN_1,       0xa6,
	REG_HINDLY_HIGH,  0x3e,
	REG_HINDLY_LOW,   0x00,
	REG_VINDLY,       0x04,
	REG_BKLVL,        0x3c,
	REG_SYNHT,        0x38,
	REG_UGAIN,        0x55,
	REG_VGAIN,        0x76,
	REG_BGAIN,        0x80,
	REG_FS_4,         0x57,
	REG_FS_3,         0x7d,
	REG_FS_2,         0x52,
	REG_FS_1,         0x3b,
	REG_HACT_HIGH,    0xa7,
	REG_VLEN_LOW,     0xbc,
	REG_HACT_LOW,     0x20,
	REG_VACT_HIGH,    0x57,
	REG_VACT_LOW,     0x98,
	REG_MISC2,        0x35,
	REG_MISC3,        0x39,
	REG_PTZ_2,        0x90,
	REG_TXDRIVER_1,   0x90,
	REG_DAC,          0x41,
	REG_PLLDICTRL_II, 0x21
};

uint8_t TP2912B_TVI4M_DataSet[] = {
	/* Table header */
	3, /* Column count */
	35, /* Row count */

	/* Table data */
	/* Registers     4M30          4M25 */
	REG_MODE,         0x8b,        0x8b,
	REG_ENC_MODE,     0x52,        0x52,
	REG_BSTLN,        0x80,        0x80,
	REG_MISC1,        0x46,        0x46,
	REG_VDTH0_LOW,    0xe0,        0xe0,
	REG_SYLO,         0x50,        0x50,
	REG_SYHI,         0x50,        0x50,
	REG_VDTH0_HIGH,   0x0a,        0x3a,
	REG_H0TVS_LOW,    0x08,        0x08,
	REG_H0TVD_LOW,    0x08,        0x08,
	REG_H0TBST_HIGH,  0x50,        0x50,
	REG_HLEN_1,       0xe4,        0x78,
	REG_HINDLY_HIGH,  0x1c,        0x3f,
	REG_HINDLY_LOW,   0x00,        0x90,
	REG_VINDLY,       0x05,        0x05,
	REG_BKLVL,        0x3c,        0x3c,
	REG_SYNHT,        0x38,        0x38,
	REG_UGAIN,        0x55,        0x55,
	REG_VGAIN,        0x76,        0x76,
	REG_BGAIN,        0x80,        0x80,
	REG_FS_4,         0x57,        0x57,
	REG_FS_3,         0x42,        0x42,
	REG_FS_2,         0x77,        0x77,
	REG_FS_1,         0x87,        0x87,
	REG_HACT_HIGH,    0xa5,        0xa5,
	REG_VLEN_LOW,     0xdc,        0xdc,
	REG_HACT_LOW,     0x00,        0x00,
	REG_VACT_HIGH,    0x55,        0x55,
	REG_VACT_LOW,     0xa0,        0xa0,
	REG_MISC2,        0x35,        0x35,
	REG_MISC3,        0x39,        0x39,
	REG_PTZ_2,        0x90,        0x90,
	REG_TXDRIVER_1,   0x90,        0x90,
	REG_DAC,          0x41,        0x41,
	REG_PLLDICTRL_II, 0x21,        0x21
};

uint8_t TP2910_TVI1080P_DataSet[] = {
	/* Table header */
	3, /* Column count */
	12, /* Row count */

	/* Table data */
	/* Registers     1080P30      1080P25 */
	REG_MODE,         0x87,        0x97,
	REG_BSTLN,        0x40,        0x40,
	REG_VDTH0_LOW,    0x58,        0x10,
	REG_SYLO,         0x2c,        0x2c,
	REG_SYHI,         0x2c,        0x2c,
	REG_VDTH0_HIGH,   0x00,        0x20,
	REG_H0TVS_LOW,    0xc0,        0xc0,
	REG_H0TVD_LOW,    0xc0,        0xc0,
	REG_HLEN_1,       0x98,        0x50,
	REG_HINDLY_HIGH,  0x08,        0x2a,
	REG_HINDLY_LOW,   0x6c,        0x24,
	REG_TXDRIVER_1,   0x00,        0x00
};

uint8_t TP2910_TVI720P_DataSet[] = {
	/* Table header */
	5, /* Column count */
	12, /* Row count */

	/* Table data */
	/* Registers     720P60      720P50       720P30       720P25 */
	REG_MODE,         0x8b,        0x9b,         0x8f,        0x9f,
	REG_VDTH0_LOW,    0x6e,        0xb8,         0xe0,        0x74,
	REG_SYLO,         0x28,        0x28,         0x28,        0x28,
	REG_SYHI,         0x28,        0x28,         0x28,        0x28,
	REG_VDTH0_HIGH,   0x05,        0x15,         0x45,        0x65,
	REG_H0TVS_LOW,    0x04,        0x04,         0x04,        0x04,
	REG_H0TVD_LOW,    0x04,        0x04,         0x04,        0x04,
	REG_HLEN_1,       0x72,        0xbc,         0xe4,        0x78,
	REG_HINDLY_HIGH,  0x06,        0x17,         0x6c,        0x9f,
	REG_HINDLY_LOW,   0x83,        0xcd,         0xf5,        0x89,
	REG_VINDLY,       0x05,        0x05,         0x05,        0x05,
	REG_TXDRIVER_1,   0x00,        0x00,         0x10,        0x10
};

uint8_t TP2910_TVINTSC_DataSet[] = {
	/* Table header */
	2, /* Column count */
	18, /* Row count */

	/* Table data */
	/* Registers     NTSC */
	REG_MODE,         0x09,
	REG_BSTLN,        0x24,
	REG_MISC1,        0x81,
	REG_VDTH0_LOW,    0x20,
	REG_SYLO,         0x42,
	REG_SYHI,         0x28,
	REG_VDTH0_HIGH,   0x05,
	REG_H0TVS_LOW,    0x04,
	REG_H0TVD_LOW,    0x04,
	REG_H0TBST_HIGH,  0x3a,
	REG_HLEN_1,       0x5a,
	REG_HINDLY_HIGH,  0x03,
	REG_HINDLY_LOW,   0x16,
	REG_VINDLY,       0x04,
	REG_YGAIN,        0x97,
	REG_UGAIN,        0x81,
	REG_VGAIN,        0xb6,
	REG_TXDRIVER_1,   0x10
};

uint8_t TP2910_TVIPAL_DataSet[] = {
	/* Table header */
	2, /* Column count */
	18, /* Row count */

	/* Table data */
	/* Registers     PAL */
	REG_MODE,         0x09,
	REG_BSTLN,        0x24,
	REG_MISC1,        0x81,
	REG_VDTH0_LOW,    0x20,
	REG_SYLO,         0x42,
	REG_SYHI,         0x28,
	REG_VDTH0_HIGH,   0x05,
	REG_H0TVS_LOW,    0x04,
	REG_H0TVD_LOW,    0x04,
	REG_H0TBST_HIGH,  0x3a,
	REG_HLEN_1,       0x5a,
	REG_HINDLY_HIGH,  0x03,
	REG_HINDLY_LOW,   0x16,
	REG_VINDLY,       0x04,
	REG_YGAIN,        0x97,
	REG_UGAIN,        0x81,
	REG_VGAIN,        0xb6,
	REG_TXDRIVER_1,   0x10
};

uint64_t tp2912_parent_table[] = {
/* Table header */
	7, /* Column count */
	43, /* Row count */

/* Table data */
/* width         height         fps                chipid  bitmask                         TVI/AHD                   table                        column */
	1920,       1080,           30,        BIT(TP2912) | BIT(TP2912B) | BIT(TP2915),       AHD,       (uint64_t)TP2912_AHD1080P_DataSet      ,      1,
	1920,       1080,           25,        BIT(TP2912) | BIT(TP2912B) | BIT(TP2915),       AHD,       (uint64_t)TP2912_AHD1080P_DataSet      ,      2,
	1920,       1080,           30,        TP2910                                  ,       AHD,       (uint64_t)TP2910_AHD1080P_DataSet      ,      1,
	1920,       1080,           25,        TP2910                                  ,       AHD,       (uint64_t)TP2910_AHD1080P_DataSet      ,      2,
	1920,       1080,           30,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI1080P_DataSet      ,      1,
	1920,       1080,           25,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI1080P_DataSet      ,      2,
	1920,       1080,           30,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI1080P3025_DataSet ,      1,
	1920,       1080,           25,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI1080P3025_DataSet ,      2,
	1920,       1080,           30,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVI1080P_DataSet      ,      1,
	1920,       1080,           25,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVI1080P_DataSet      ,      2,
	1920,       1080,           60,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI1080P60_DataSet   ,      1,
	1280,        720,           30,        BIT(TP2912)                             ,       AHD,       (uint64_t)TP2912_AHD720P_DataSet       ,      1,
	1280,        720,           25,        BIT(TP2912)                             ,       AHD,       (uint64_t)TP2912_AHD720P_DataSet       ,      2,
	1280,        720,           30,        BIT(TP2912B)                            ,       AHD,       (uint64_t)TP2912_AHD720P_DataSet       ,      3,
	1280,        720,           25,        BIT(TP2912B)                            ,       AHD,       (uint64_t)TP2912_AHD720P_DataSet       ,      4,
	1280,        720,           30,        BIT(TP2910)                             ,       AHD,       (uint64_t)TP2910_AHD720P_DataSet       ,      1,
	1280,        720,           25,        BIT(TP2910)                             ,       AHD,       (uint64_t)TP2910_AHD720P_DataSet       ,      2,
	1280,        720,           60,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI720P_DataSet       ,      1,
	1280,        720,           50,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI720P_DataSet       ,      2,
	1280,        720,           30,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI720P_DataSet       ,      3,
	1280,        720,           25,        BIT(TP2912B)                            ,       TVI,       (uint64_t)TP2912_TVI720P_DataSet       ,      4,
	1280,        720,           60,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI720P_DataSet      ,      1,
	1280,        720,           50,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI720P_DataSet      ,      2,
	1280,        720,           30,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI720P_DataSet      ,      3,
	1280,        720,           25,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI720P_DataSet      ,      4,
	1280,        720,           60,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVI720P_DataSet       ,      1,
	1280,        720,           50,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVI720P_DataSet       ,      2,
	1280,        720,           30,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVI720P_DataSet       ,      3,
	1280,        720,           25,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVI720P_DataSet       ,      4,
	/* NTSC */
	720,         480,           60,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVINTSC_DataSet       ,      1,
	720,         480,           60,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVINTSC_DataSet      ,      1,
	720,         480,           60,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVINTSC_DataSet       ,      1,
	/* PAL */
	720,         576,           50,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVIPAL_DataSet        ,      1,
	720,         576,           50,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVIPAL_DataSet       ,      1,
	720,         576,           50,        BIT(TP2910)                             ,       TVI,       (uint64_t)TP2910_TVIPAL_DataSet        ,      1,
	/* 4M */
	2560,        1440,          30,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI4M_DataSet         ,      1,
	2560,        1440,          25,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI4M_DataSet         ,      2,
	2560,        1440,          30,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI4M_DataSet        ,      1,
	2560,        1440,          25,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI4M_DataSet        ,      2,
	/* 5M */
	2592,        1944,          20,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI5M20_DataSet       ,      1,
	2592,        1944,          20,        BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI5M20_DataSet      ,      1,
	/* 8M */
	3840,        2160,          15,        BIT(TP2912)                             ,       TVI,       (uint64_t)TP2912_TVI8M15_DataSet       ,      1,
	3840,        2160,          125,       BIT(TP2912B) | BIT(TP2915)              ,       TVI,       (uint64_t)TP2912B_TVI8M125_DataSet     ,      1,
};

#endif