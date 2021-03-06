/*
 * Copyright (C) 2009 Francisco Jerez.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <linux/module.h>
#include <linux/i2c.h>

#include <drm/drm_encoder_slave.h>

struct regval_list {
	u8 reg_num;
	u8 value;
};

static const struct regval_list pwr_vals[] = {
	/* Page 0 (0x03 = 0x00) */
	{ 0x03, 0x00 },
	{ 0x00, 0x56 },
	{ 0x01, 0xF1 },
	{ 0x02, 0xF3 },
	{ 0x03, 0x00 },
	{ 0x04, 0x36 },
	{ 0x05, 0x58 },
	{ 0x06, 0xAC },
	{ 0x07, 0xD0 },
	{ 0x08, 0x0F },
	{ 0x09, 0x02 },
	{ 0x0A, 0x04 },
	{ 0x0B, 0x2C },
	{ 0x0C, 0x00 },
	{ 0x0D, 0x40 },
	{ 0x0E, 0x00 },
	{ 0x0F, 0x18 },
	{ 0x10, 0x88 },
	{ 0x11, 0x1B },
	{ 0x12, 0x00 },
	{ 0x13, 0x26 },
	{ 0x14, 0x00 },
	{ 0x15, 0x03 },
	{ 0x16, 0x06 },
	{ 0x17, 0x00 },
	{ 0x18, 0x00 },
	{ 0x19, 0x88 },
	{ 0x1A, 0x00 },
	{ 0x1B, 0x00 },
	{ 0x1C, 0x69 },
	{ 0x1D, 0x78 },
	{ 0x1E, 0x00 },
	{ 0x1F, 0x47 },
	{ 0x20, 0x80 },
	{ 0x21, 0x98 },
	{ 0x22, 0x00 },
	{ 0x23, 0x10 },
	{ 0x24, 0x60 },
	{ 0x25, 0x24 },
	{ 0x26, 0x38 },
	{ 0x27, 0x65 },
	{ 0x28, 0x00 },
	{ 0x29, 0x0A },
	{ 0x2A, 0x02 },
	{ 0x2B, 0x08 },
	{ 0x2C, 0x00 },
	{ 0x2D, 0x00 },
	{ 0x2E, 0x3F },
	{ 0x2F, 0x00 },
	{ 0x30, 0x01 },
	{ 0x31, 0x01 },
	{ 0x32, 0xC0 },
	{ 0x33, 0x01 },
	{ 0x34, 0x01 },
	{ 0x35, 0x80 },
	{ 0x36, 0x40 },
	{ 0x37, 0x40 },
	{ 0x38, 0x47 },
	{ 0x39, 0x88 },
	{ 0x3A, 0x00 },
	{ 0x3B, 0x00 },
	{ 0x3C, 0x00 },
	{ 0x3D, 0x8A },
	{ 0x3E, 0x00 },
	{ 0x3F, 0x11 },
	{ 0x40, 0x10 },
	{ 0x41, 0x00 },
	{ 0x42, 0x00 },
	{ 0x43, 0x00 },
	{ 0x44, 0x00 },
	{ 0x45, 0x00 },
	{ 0x46, 0x00 },
	{ 0x47, 0x00 },
	{ 0x48, 0x00 },
	{ 0x49, 0x00 },
	{ 0x4A, 0x00 },
	{ 0x4B, 0x40 },
	{ 0x4C, 0x40 },
	{ 0x4D, 0x80 },
	{ 0x4E, 0x00 },
	{ 0x4F, 0x00 },
	{ 0x50, 0x00 },
	{ 0x51, 0x1F },
	{ 0x52, 0xFF },
	{ 0x53, 0x00 },
	{ 0x54, 0x80 },
	{ 0x55, 0x58 },
	{ 0x56, 0x2C },
	{ 0x57, 0x00 },
	{ 0x58, 0x04 },
	{ 0x59, 0x05 },
	{ 0x5A, 0x08 },
	{ 0x5B, 0x00 },
	{ 0x5C, 0x00 },
	{ 0x5D, 0x00 },
	{ 0x5E, 0x54 },
	{ 0x5F, 0x00 },
	{ 0x60, 0x00 },
	{ 0x61, 0x00 },
	{ 0x62, 0x00 },
	{ 0x63, 0x01 },
	{ 0x64, 0x20 },
	{ 0x65, 0x90 },
	{ 0x66, 0x20 },
	{ 0x67, 0x22 },
	{ 0x68, 0x46 },
	{ 0x69, 0x24 },
	{ 0x6A, 0x40 },
	{ 0x6B, 0x60 },
	{ 0x6C, 0x10 },
	{ 0x6D, 0x00 },
	{ 0x6E, 0xA8 },
	{ 0x6F, 0x4B },
	{ 0x70, 0x98 },
	{ 0x71, 0x01 },
	{ 0x72, 0x00 },
	{ 0x73, 0x00 },
	{ 0x74, 0x20 },
	{ 0x75, 0x80 },
	{ 0x76, 0x18 },
	{ 0x77, 0x00 },
	{ 0x78, 0x00 },
	{ 0x79, 0x01 },
	{ 0x7A, 0x00 },
	{ 0x7B, 0x00 },
	{ 0x7C, 0x00 },
	{ 0x7D, 0xFF },
	{ 0x7E, 0x0F },
	{ 0x7F, 0x00 },

	/* Page 1 (0x03 = 0x01): */
	{ 0x03, 0x01 },
	{ 0x00, 0x56 },
	{ 0x01, 0xF1 },
	{ 0x02, 0xF3 },
	{ 0x03, 0x01 },
	{ 0x04, 0x36 },
	{ 0x05, 0x58 },
	{ 0x06, 0xAC },
	{ 0x07, 0x26 },
	{ 0x08, 0x00 },
	{ 0x09, 0x4B },
	{ 0x0A, 0x00 },
	{ 0x0B, 0x6D },
	{ 0x0C, 0x74 },
	{ 0x0D, 0x51 },
	{ 0x0E, 0x13 },
	{ 0x0F, 0x1C },
	{ 0x10, 0x00 },
	{ 0x11, 0x08 },
	{ 0x12, 0xD6 },
	{ 0x13, 0x28 },
	{ 0x14, 0x91 },
	{ 0x15, 0x68 },
	{ 0x16, 0x20 },
	{ 0x17, 0x0E },
	{ 0x18, 0xC8 },
	{ 0x19, 0x42 },
	{ 0x1A, 0x7C },
	{ 0x1B, 0x00 },
	{ 0x1C, 0x00 },
	{ 0x1D, 0x00 },
	{ 0x1E, 0x00 },
	{ 0x1F, 0x00 },
	{ 0x20, 0x00 },
	{ 0x21, 0x10 },
	{ 0x22, 0x07 },
	{ 0x23, 0x24 },
	{ 0x24, 0xB6 },
	{ 0x25, 0x10 },
	{ 0x26, 0x00 },
	{ 0x27, 0x00 },
	{ 0x28, 0x54 },
	{ 0x29, 0x60 },
	{ 0x2A, 0x00 },
	{ 0x2B, 0x00 },
	{ 0x2C, 0x00 },
	{ 0x2D, 0x00 },
	{ 0x2E, 0x00 },
	{ 0x2F, 0x00 },
	{ 0x30, 0x00 },
	{ 0x31, 0x00 },
	{ 0x32, 0x00 },
	{ 0x33, 0x00 },
	{ 0x34, 0x00 },
	{ 0x35, 0x00 },
	{ 0x36, 0x00 },
	{ 0x37, 0x00 },
	{ 0x38, 0x00 },
	{ 0x39, 0x00 },
	{ 0x3A, 0x00 },
	{ 0x3B, 0x0B },
	{ 0x3C, 0x00 },
	{ 0x3D, 0x00 },
	{ 0x3E, 0x00 },
	{ 0x3F, 0x00 },
	{ 0x40, 0x08 },
	{ 0x41, 0x00 },
	{ 0x42, 0x14 },
	{ 0x43, 0x20 },
	{ 0x44, 0x00 },
	{ 0x45, 0x00 },
	{ 0x46, 0x20 },
	{ 0x47, 0x00 },
	{ 0x48, 0x49 },
	{ 0x49, 0x10 },
	{ 0x4A, 0xFF },
	{ 0x4B, 0xFF },
	{ 0x4C, 0xFF },
	{ 0x4D, 0x00 },
	{ 0x4E, 0x08 },
	{ 0x4F, 0x00 },
	{ 0x50, 0x00 },
	{ 0x51, 0x00 },
	{ 0x52, 0x00 },
	{ 0x53, 0x00 },
	{ 0x54, 0x00 },
	{ 0x55, 0xA0 },
	{ 0x56, 0x00 },
	{ 0x57, 0x00 },
	{ 0x58, 0x00 },
	{ 0x59, 0x00 },
	{ 0x5A, 0x00 },
	{ 0x5B, 0x7A },
	{ 0x5C, 0x5E },
	{ 0x5D, 0x6E },
	{ 0x5E, 0x1F },
	{ 0x5F, 0x1F },
	{ 0x60, 0x00 },
	{ 0x61, 0x00 },
	{ 0x62, 0x00 },
	{ 0x63, 0x20 },
	{ 0x64, 0x40 },
	{ 0x65, 0x40 },
	{ 0x66, 0x40 },
	{ 0x67, 0x00 },
	{ 0x68, 0x00 },
	{ 0x69, 0x00 },
	{ 0x6A, 0x00 },
	{ 0x6B, 0xFF },
	{ 0x6C, 0xFF },
	{ 0x6D, 0xFF },
	{ 0x6E, 0xFF },
	{ 0x6F, 0x50 },
	{ 0x70, 0x00 },
	{ 0x71, 0x00 },
	{ 0x72, 0x09 },
	{ 0x73, 0x00 },
	{ 0x74, 0x00 },
	{ 0x75, 0x70 },
	{ 0x76, 0x00 },
	{ 0x77, 0x50 },
	{ 0x78, 0x00 },
	{ 0x79, 0x98 },
	{ 0x7A, 0x00 },
	{ 0x7B, 0x98 },
	{ 0x7C, 0xFF },
	{ 0x7D, 0x00 },
	{ 0x7E, 0x00 },
	{ 0x7F, 0x80 },

	/*Page 2 (0x03 = 0x02): */
	{ 0x03, 0x02 },
	{ 0x00, 0x56 },
	{ 0x01, 0xF1 },
	{ 0x02, 0xF3 },
	{ 0x03, 0xFF },
	{ 0x04, 0x36 },
	{ 0x05, 0x58 },
	{ 0x06, 0xAC },
	{ 0x07, 0xFF },
	{ 0x08, 0xFF },
	{ 0x09, 0xFF },
	{ 0x0A, 0xFF },
	{ 0x0B, 0xFF },
	{ 0x0C, 0xFF },
	{ 0x0D, 0xFF },
	{ 0x0E, 0xFF },
	{ 0x0F, 0xFF },
	{ 0x10, 0xFF },
	{ 0x11, 0xFF },
	{ 0x12, 0xFF },
	{ 0x13, 0xFF },
	{ 0x14, 0xFF },
	{ 0x15, 0xFF },
	{ 0x16, 0xFF },
	{ 0x17, 0xFF },
	{ 0x18, 0xFF },
	{ 0x19, 0xFF },
	{ 0x1A, 0xFF },
	{ 0x1B, 0xFF },
	{ 0x1C, 0xFF },
	{ 0x1D, 0xFF },
	{ 0x1E, 0xFF },
	{ 0x1F, 0xFF },
	{ 0x20, 0xFF },
	{ 0x21, 0xFF },
	{ 0x22, 0xFF },
	{ 0x23, 0xFF },
	{ 0x24, 0xFF },
	{ 0x25, 0xFF },
	{ 0x26, 0xFF },
	{ 0x27, 0xFF },
	{ 0x28, 0xFF },
	{ 0x29, 0xFF },
	{ 0x2A, 0xFF },
	{ 0x2B, 0xFF },
	{ 0x2C, 0xFF },
	{ 0x2D, 0xFF },
	{ 0x2E, 0xFF },
	{ 0x2F, 0xFF },
	{ 0x30, 0xFF },
	{ 0x31, 0xFF },
	{ 0x32, 0xFF },
	{ 0x33, 0xFF },
	{ 0x34, 0xFF },
	{ 0x35, 0xFF },
	{ 0x36, 0xFF },
	{ 0x37, 0xFF },
	{ 0x38, 0xFF },
	{ 0x39, 0xFF },
	{ 0x3A, 0xFF },
	{ 0x3B, 0xFF },
	{ 0x3C, 0xFF },
	{ 0x3D, 0xFF },
	{ 0x3E, 0xFF },
	{ 0x3F, 0xFF },
	{ 0x40, 0xFF },
	{ 0x41, 0xFF },
	{ 0x42, 0xFF },
	{ 0x43, 0xFF },
	{ 0x44, 0xFF },
	{ 0x45, 0xFF },
	{ 0x46, 0xFF },
	{ 0x47, 0xFF },
	{ 0x48, 0xFF },
	{ 0x49, 0xFF },
	{ 0x4A, 0xFF },
	{ 0x4B, 0xFF },
	{ 0x4C, 0xFF },
	{ 0x4D, 0xFF },
	{ 0x4E, 0xFF },
	{ 0x4F, 0xFF },
	{ 0x50, 0xFF },
	{ 0x51, 0xFF },
	{ 0x52, 0xFF },
	{ 0x53, 0xFF },
	{ 0x54, 0xFF },
	{ 0x55, 0xFF },
	{ 0x56, 0xFF },
	{ 0x57, 0xFF },
	{ 0x58, 0xFF },
	{ 0x59, 0xFF },
	{ 0x5A, 0xFF },
	{ 0x5B, 0xFF },
	{ 0x5C, 0xFF },
	{ 0x5D, 0xFF },
	{ 0x5E, 0xFF },
	{ 0x5F, 0xFF },
	{ 0x60, 0xFF },
	{ 0x61, 0xFF },
	{ 0x62, 0xFF },
	{ 0x63, 0xFF },
	{ 0x64, 0xFF },
	{ 0x65, 0xFF },
	{ 0x66, 0xFF },
	{ 0x67, 0xFF },
	{ 0x68, 0xFF },
	{ 0x69, 0xFF },
	{ 0x6A, 0xFF },
	{ 0x6B, 0xFF },
	{ 0x6C, 0xFF },
	{ 0x6D, 0xFF },
	{ 0x6E, 0xFF },
	{ 0x6F, 0xFF },
	{ 0x70, 0xFF },
	{ 0x71, 0xFF },
	{ 0x72, 0xFF },
	{ 0x73, 0xFF },
	{ 0x74, 0xFF },
	{ 0x75, 0xFF },
	{ 0x76, 0xFF },
	{ 0x77, 0xFF },
	{ 0x78, 0xFF },
	{ 0x79, 0xFF },
	{ 0x7A, 0xFF },
	{ 0x7B, 0xFF },
	{ 0x7C, 0xFF },
	{ 0x7D, 0xFF },
	{ 0x7E, 0xFF },
	{ 0x7F, 0xFF },

	/* Page 3 {0x03 , 0x03}, */
	{ 0x03, 0x03 },
	{ 0x00, 0x56 },
	{ 0x01, 0xF1 },
	{ 0x02, 0xF3 },
	{ 0x03, 0x03 },
	{ 0x04, 0x36 },
	{ 0x05, 0x58 },
	{ 0x06, 0xAC },
	{ 0x07, 0x00 },
	{ 0x08, 0x00 },
	{ 0x09, 0x00 },
	{ 0x0A, 0x00 },
	{ 0x0B, 0x00 },
	{ 0x0C, 0xFF },
	{ 0x0D, 0x00 },
	{ 0x0E, 0x03 },
	{ 0x0F, 0x19 },
	{ 0x10, 0x40 },
	{ 0x11, 0x01 },
	{ 0x12, 0x00 },
	{ 0x13, 0x00 },
	{ 0x14, 0x00 },
	{ 0x15, 0x00 },
	{ 0x16, 0x00 },
	{ 0x17, 0x00 },
	{ 0x18, 0x00 },
	{ 0x19, 0x00 },
	{ 0x1A, 0x00 },
	{ 0x1B, 0x00 },
	{ 0x1C, 0x00 },
	{ 0x1D, 0x00 },
	{ 0x1E, 0x00 },
	{ 0x1F, 0x00 },
	{ 0x20, 0x00 },
	{ 0x21, 0x00 },
	{ 0x22, 0x42 },
	{ 0x23, 0x1D },
	{ 0x24, 0x8A },
	{ 0x25, 0x11 },
	{ 0x26, 0xFF },
	{ 0x27, 0xFF },
	{ 0x28, 0xFF },
	{ 0x29, 0xFF },
	{ 0x2A, 0xFF },
	{ 0x2B, 0xFF },
	{ 0x2C, 0xFF },
	{ 0x2D, 0xFF },
	{ 0x2E, 0xFF },
	{ 0x2F, 0xFF },
	{ 0x30, 0xFF },
	{ 0x31, 0xFF },
	{ 0x32, 0xFF },
	{ 0x33, 0xFF },
	{ 0x34, 0xFF },
	{ 0x35, 0xFF },
	{ 0x36, 0xFF },
	{ 0x37, 0xFF },
	{ 0x38, 0xFF },
	{ 0x39, 0xFF },
	{ 0x3A, 0xFF },
	{ 0x3B, 0xFF },
	{ 0x3C, 0xFF },
	{ 0x3D, 0xFF },
	{ 0x3E, 0xFF },
	{ 0x3F, 0xFF },
	{ 0x40, 0xFF },
	{ 0x41, 0x09 },
	{ 0x42, 0x1D },
	{ 0x43, 0x0F },
	{ 0x44, 0x00 },
	{ 0x45, 0xFF },
	{ 0x46, 0xFF },
	{ 0x47, 0xFF },
	{ 0x48, 0xFF },
	{ 0x49, 0xFF },
	{ 0x4A, 0xFF },
	{ 0x4B, 0xFF },
	{ 0x4C, 0xFF },
	{ 0x4D, 0xFF },
	{ 0x4E, 0xFF },
	{ 0x4F, 0xFF },
	{ 0x50, 0xFF },
	{ 0x51, 0xFF },
	{ 0x52, 0xFF },
	{ 0x53, 0xFF },
	{ 0x54, 0xFF },
	{ 0x55, 0xFF },
	{ 0x56, 0xFF },
	{ 0x57, 0xFF },
	{ 0x58, 0xFF },
	{ 0x59, 0xFF },
	{ 0x5A, 0xFF },
	{ 0x5B, 0xFF },
	{ 0x5C, 0xFF },
	{ 0x5D, 0xFF },
	{ 0x5E, 0xFF },
	{ 0x5F, 0xFF },
	{ 0x60, 0xFF },
	{ 0x61, 0xFF },
	{ 0x62, 0xFF },
	{ 0x63, 0xFF },
	{ 0x64, 0xFF },
	{ 0x65, 0xFF },
	{ 0x66, 0xFF },
	{ 0x67, 0xFF },
	{ 0x68, 0xFF },
	{ 0x69, 0xFF },
	{ 0x6A, 0xFF },
	{ 0x6B, 0xFF },
	{ 0x6C, 0xFF },
	{ 0x6D, 0xFF },
	{ 0x6E, 0x00 },
	{ 0x6F, 0x00 },
	{ 0x70, 0xFF },
	{ 0x71, 0xF8 },
	{ 0x72, 0xFF },
	{ 0x73, 0x00 },
	{ 0x74, 0x02 },
	{ 0x75, 0xFF },
	{ 0x76, 0x00 },
	{ 0x77, 0x00 },
	{ 0x78, 0x00 },
	{ 0x79, 0xFF },
	{ 0x7A, 0xFF },
	{ 0x7B, 0xFF },
	{ 0x7C, 0xFF },
	{ 0x7D, 0xFF },
	{ 0x7E, 0xFF },
	{ 0x7F, 0xFF },

	/*Page 4 (0x03 = 0x04): */
	{ 0x03, 0x04 },
	{ 0x00, 0x56 },
	{ 0x01, 0xF1 },
	{ 0x02, 0xF3 },
	{ 0x03, 0x04 },
	{ 0x04, 0x36 },
	{ 0x05, 0x58 },
	{ 0x06, 0xAC },
	{ 0x07, 0xFF },
	{ 0x08, 0x00 },
	{ 0x09, 0x00 },
	{ 0x0A, 0x00 },
	{ 0x0B, 0x00 },
	{ 0x0C, 0x00 },
	{ 0x0D, 0x00 },
	{ 0x0E, 0x00 },
	{ 0x0F, 0x28 },
	{ 0x10, 0x44 },
	{ 0x11, 0x14 },
	{ 0x12, 0x88 },
	{ 0x13, 0x70 },
	{ 0x14, 0xFD },
	{ 0x15, 0xE8 },
	{ 0x16, 0x00 },
	{ 0x17, 0x00 },
	{ 0x18, 0xFF },
	{ 0x19, 0xFF },
	{ 0x1A, 0xFF },
	{ 0x1B, 0xFF },
	{ 0x1C, 0xFF },
	{ 0x1D, 0xFF },
	{ 0x1E, 0xFF },
	{ 0x1F, 0xFF },
	{ 0x20, 0xFF },
	{ 0x21, 0xFF },
	{ 0x22, 0xFF },
	{ 0x23, 0xFF },
	{ 0x24, 0xFF },
	{ 0x25, 0xFF },
	{ 0x26, 0xFF },
	{ 0x27, 0xFF },
	{ 0x28, 0x00 },
	{ 0x29, 0x00 },
	{ 0x2A, 0x7F },
	{ 0x2B, 0x07 },
	{ 0x2C, 0xEF },
	{ 0x2D, 0x07 },
	{ 0x2E, 0x5F },
	{ 0x2F, 0x07 },
	{ 0x30, 0x86 },
	{ 0x31, 0x07 },
	{ 0x32, 0x80 },
	{ 0x33, 0x00 },
	{ 0x34, 0x80 },
	{ 0x35, 0x00 },
	{ 0x36, 0x08 },
	{ 0x37, 0x87 },
	{ 0x38, 0x89 },
	{ 0x39, 0x0B },
	{ 0x3A, 0x5F },
	{ 0x3B, 0x9D },
	{ 0x3C, 0x10 },
	{ 0x3D, 0x00 },
	{ 0x3E, 0x00 },
	{ 0x3F, 0x00 },
	{ 0x40, 0x00 },
	{ 0x41, 0xFF },
	{ 0x42, 0xFF },
	{ 0x43, 0xFF },
	{ 0x44, 0xFF },
	{ 0x45, 0x00 },
	{ 0x46, 0x03 },
	{ 0x47, 0x00 },
	{ 0x48, 0x00 },
	{ 0x49, 0x00 },
	{ 0x4A, 0x24 },
	{ 0x4B, 0x00 },
	{ 0x4C, 0x48 },
	{ 0x4D, 0xFF },
	{ 0x4E, 0xFF },
	{ 0x4F, 0x7F },
	{ 0x50, 0x56 },
	{ 0x51, 0x81 },
	{ 0x52, 0x2F },
	{ 0x53, 0x00 },
	{ 0x54, 0xC4 },
	{ 0x55, 0x5A },
	{ 0x56, 0x6F },
	{ 0x57, 0x02 },
	{ 0x58, 0x20 },
	{ 0x59, 0x80 },
	{ 0x5A, 0x04 },
	{ 0x5B, 0x00 },
	{ 0x5C, 0x80 },
	{ 0x5D, 0x00 },
	{ 0x5E, 0x05 },
	{ 0x5F, 0x38 },
	{ 0x60, 0x01 },
	{ 0x61, 0x83 },
	{ 0x62, 0x11 },
	{ 0x63, 0x01 },
	{ 0x64, 0x80 },
	{ 0x65, 0x90 },
	{ 0x66, 0x06 },
	{ 0x67, 0xC0 },
	{ 0x68, 0x4A },
	{ 0x69, 0xBC },
	{ 0x6A, 0x00 },
	{ 0x6B, 0x00 },
	{ 0x6C, 0x00 },
	{ 0x6D, 0x00 },
	{ 0x6E, 0x89 },
	{ 0x6F, 0x10 },
	{ 0x70, 0xAC },
	{ 0x71, 0x00 },
	{ 0x72, 0xE0 },
	{ 0x73, 0x80 },
	{ 0x74, 0x80 },
	{ 0x75, 0x19 },
	{ 0x76, 0x7D },
	{ 0x77, 0x00 },
	{ 0x78, 0xFF },
	{ 0x79, 0xFF },
	{ 0x7A, 0xFF },
	{ 0x7B, 0xFF },
	{ 0x7C, 0xFF },
	{ 0x7D, 0xFF },
	{ 0x7E, 0x80 },
	{ 0x7F, 0x4C },
	{ 0xff, 0xff },
};

struct ch7036 {
	struct i2c_client *client;
	struct drm_encoder *encoder;
};

/* Some helper macros */
#define ch7036_info(client, format, ...) \
	dev_info(&client->dev, format, __VA_ARGS__)
#define ch7036_err(client, format, ...) \
	dev_err(&client->dev, format, __VA_ARGS__)

/* HW access functions */
void ch7036_write(struct i2c_client *client, uint8_t addr, uint8_t val)
{
	uint8_t buf[] = {addr, val};
	int ret;

	ret = i2c_master_send(client, buf, ARRAY_SIZE(buf));
	if (ret < 0)
		ch7036_err(client, "Error %d writing to subaddress 0x%x\n",
				ret, addr);
}

uint8_t ch7036_read(struct i2c_client *client, uint8_t addr)
{
	uint8_t val;
	int ret;

	ret = i2c_master_send(client, &addr, sizeof(addr));
	if (ret < 0)
		goto fail;

	ret = i2c_master_recv(client, &val, sizeof(val));
	if (ret < 0)
		goto fail;

	return val;

fail:
	ch7036_err(client, "Error %d reading from subaddress 0x%x\n",
			ret, addr);
	return 0;
}

/* I2C driver functions */

static int ch7036_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	const struct regval_list *reg_def = pwr_vals;
	int dev_id;

	ch7036_write(client, 0x3, 0x4);
	dev_id = ch7036_read(client, 0x50);
	ch7036_info(client, "ch7036 is probed. The device ID is 0x%x\n",
				dev_id);

	if (dev_id != 0x56)
		return -ENODEV;

	for (; reg_def->reg_num != 0xff; ) {
		ch7036_write(client, reg_def->reg_num, reg_def->value);
		reg_def++;
	}

	return 0;
}

static int ch7036_remove(struct i2c_client *client)
{
	return 0;
}

static int ch7036_resume(struct device *dev)
{
	return 0;
}

static int ch7036_encoder_init(struct i2c_client *i2c,
	struct drm_device *dev, struct drm_encoder_slave *encoder)
{
	struct ch7036 *ch7036 = i2c_get_clientdata(i2c);

	encoder->slave_priv = ch7036;
	encoder->slave_funcs = NULL;

	ch7036->encoder = &encoder->base;

	return 0;
}

static struct i2c_device_id ch7036_ids[] = {
	{ "ch7036", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ch7036_ids);

static const struct dev_pm_ops ch7036_pm_ops = {
	.resume = ch7036_resume,
};

static const struct of_device_id ch7036_of_ids[] = {
	{ .compatible = "cil,ch7036", },
	{ }
};
MODULE_DEVICE_TABLE(of, ch7036_of_ids);

static struct drm_i2c_encoder_driver ch7036_driver = {
	.i2c_driver = {
		.probe = ch7036_probe,
		.remove = ch7036_remove,
		.driver = {
			.name = "ch7036",
			.of_match_table = ch7036_of_ids,
		},
		.id_table = ch7036_ids,
	},

	.encoder_init = ch7036_encoder_init,
};

static int __init ch7036_init(void)
{
	return drm_i2c_encoder_register(THIS_MODULE, &ch7036_driver);
}

static void __exit ch7036_exit(void)
{
	drm_i2c_encoder_unregister(&ch7036_driver);
}

MODULE_AUTHOR("Francisco Jerez <currojerez@riseup.net>");
MODULE_DESCRIPTION("Chrontel ch7036 HDMI driver");
MODULE_LICENSE("GPL and additional rights");

module_init(ch7036_init);
module_exit(ch7036_exit);
