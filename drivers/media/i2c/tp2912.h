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
	TP2801A=0,
	TP2801B,
	TP2803,
	TP2910,
	TP2912,
	TP2912B,
	TP2915
};

#define V4L2_CID_TP2912_DIFF_MODE		(V4L2_CID_DV_CLASS_BASE + 0x1000)

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
#define REG_INTLINE_4 0x6B
#define REG_INTLINE_3 0x6C
#define REG_INTLINE_2 0x6D

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

#endif