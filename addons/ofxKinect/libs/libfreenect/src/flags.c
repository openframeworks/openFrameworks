/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010-2011 individual OpenKinect contributors. See the CONTRIB
 * file for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

#include <string.h> // for memcpy
#include "freenect_internal.h"
#include "flags.h"


// freenect_set_flag is the only function exposed in libfreenect.h
// The rest are available internally via #include flags.h

FN_INTERNAL int register_for_flag(int flag)
{
    switch(flag)
    {
		case FREENECT_MIRROR_DEPTH:
			return 0x17;
		case FREENECT_MIRROR_VIDEO:
			return 0x47;
		default:
			return -1;
    }
}

int freenect_set_flag(freenect_device *dev, freenect_flag flag, freenect_flag_value value)
{
    if (flag >= (1 << 16))
    {
        int reg = register_for_flag(flag);
        if (reg < 0)
            return reg;
        return write_register(dev, reg, value);
    }

	uint16_t cmos_value = read_cmos_register(dev, 0x0106);
	if (cmos_value == UINT16_MAX)
	{
		return -1;
	}
	if (value == FREENECT_ON)
		cmos_value |= flag;
	else
		cmos_value &= ~flag;
	return write_cmos_register(dev, 0x0106, cmos_value);
}

typedef struct {
	uint8_t magic[2];
	uint16_t len;
	uint16_t cmd;
	uint16_t tag;
} cam_hdr;

FN_INTERNAL int send_cmd(freenect_device *dev, uint16_t cmd, void *cmdbuf, unsigned int cmd_len, void *replybuf, int reply_len)
{
	freenect_context *ctx = dev->parent;
	int res, actual_len;
	uint8_t obuf[0x400];
	uint8_t ibuf[0x200];
	cam_hdr *chdr = (cam_hdr*)obuf;
	cam_hdr *rhdr = (cam_hdr*)ibuf;

	if (cmd_len & 1 || cmd_len > (0x400 - sizeof(*chdr))) {
		FN_ERROR("send_cmd: Invalid command length (0x%x)\n", cmd_len);
		return -1;
	}

	chdr->magic[0] = 0x47;
	chdr->magic[1] = 0x4d;
	chdr->cmd = fn_le16(cmd);
	chdr->tag = fn_le16(dev->cam_tag);
	chdr->len = fn_le16(cmd_len / 2);

	memcpy(obuf+sizeof(*chdr), cmdbuf, cmd_len);

	res = fnusb_control(&dev->usb_cam, 0x40, 0, 0, 0, obuf, cmd_len + sizeof(*chdr));
	FN_SPEW("send_cmd: cmd=%04x tag=%04x len=%04x: %d\n", cmd, dev->cam_tag, cmd_len, res);
	if (res < 0) {
		FN_ERROR("send_cmd: Output control transfer failed (%d)\n", res);
		return res;
	}

	do {
		actual_len = fnusb_control(&dev->usb_cam, 0xc0, 0, 0, 0, ibuf, 0x200);
		FN_FLOOD("send_cmd: actual length = %d\n", actual_len);
	} while ((actual_len == 0) || (actual_len == 0x200));
	FN_SPEW("Control reply: %d\n", res);
	if (actual_len < (int)sizeof(*rhdr)) {
		FN_ERROR("send_cmd: Input control transfer failed (%d)\n", res);
		return res;
	}
	actual_len -= sizeof(*rhdr);

	if (rhdr->magic[0] != 0x52 || rhdr->magic[1] != 0x42) {
		FN_ERROR("send_cmd: Bad magic %02x %02x\n", rhdr->magic[0], rhdr->magic[1]);
		return -1;
	}
	if (rhdr->cmd != chdr->cmd) {
		FN_ERROR("send_cmd: Bad cmd %02x != %02x\n", rhdr->cmd, chdr->cmd);
		return -1;
	}
	if (rhdr->tag != chdr->tag) {
		FN_ERROR("send_cmd: Bad tag %04x != %04x\n", rhdr->tag, chdr->tag);
		return -1;
	}
	if (fn_le16(rhdr->len) != (actual_len/2)) {
		FN_ERROR("send_cmd: Bad len %04x != %04x\n", fn_le16(rhdr->len), (int)(actual_len/2));
		return -1;
	}

	if (actual_len > reply_len) {
		FN_WARNING("send_cmd: Data buffer is %d bytes long, but got %d bytes\n", reply_len, actual_len);
		memcpy(replybuf, ibuf+sizeof(*rhdr), reply_len);
	} else {
		memcpy(replybuf, ibuf+sizeof(*rhdr), actual_len);
	}

	dev->cam_tag++;

	return actual_len;
}

// returns UINT16_MAX on error
FN_INTERNAL uint16_t read_register(freenect_device *dev, uint16_t reg)
{
	freenect_context *ctx = dev->parent;

	uint16_t reply[2];
	uint16_t cmd = fn_le16(reg);

	int res = send_cmd(dev, 0x02, &cmd, 2, reply, 4);
	if (res < 0)
	{
		FN_ERROR("read_register: send_cmd() failed: %d\n", res);
		return UINT16_MAX;
	}
	if (res != 4)
		FN_WARNING("read_register: send_cmd() returned %d [%04x %04x], 0000 expected\n", res, reply[0], reply[1]);

	FN_DEBUG("read_register: 0x%04x => 0x%04x\n", reg, reply[1]);
	return reply[1];
}

FN_INTERNAL int write_register(freenect_device *dev, uint16_t reg, uint16_t data)
{
	freenect_context *ctx = dev->parent;
	uint16_t reply[2];
	uint16_t cmd[2];

	cmd[0] = fn_le16(reg);
	cmd[1] = fn_le16(data);

	FN_DEBUG("write_register: 0x%04x <= 0x%02x\n", reg, data);
	int res = send_cmd(dev, 0x03, cmd, 4, reply, 4);
	if (res < 0)
	{
		FN_ERROR("write_register: send_cmd() returned %d\n", res);
		return res;
	}
	if (res != 2)
		FN_WARNING("write_register: send_cmd() returned %d [%04x %04x], 0000 expected\n", res, reply[0], reply[1]);

	return 0;
}

// returns UINT16_MAX on error
FN_INTERNAL uint16_t read_cmos_register(freenect_device *dev, uint16_t reg)
{
	freenect_context *ctx = dev->parent;
	uint16_t replybuf[0x200];
	uint16_t cmdbuf[3];

	cmdbuf[0] = 1;
	cmdbuf[1] = reg & 0x7fff;
	cmdbuf[2] = 0;

	int res = send_cmd(dev, 0x95, cmdbuf, 6, replybuf, 6);
	if (res < 0)
	{
		FN_ERROR("read_cmos_register: send_cmd() returned %d\n", res);
		return UINT16_MAX;
	}

	FN_DEBUG("read_cmos_register: 0x%04x => 0x%04x\n", reg, replybuf[2]);
	return replybuf[2];
}

FN_INTERNAL int write_cmos_register(freenect_device *dev, uint16_t reg, uint16_t value)
{
	freenect_context *ctx = dev->parent;
	uint16_t replybuf[0x200];
	uint16_t cmdbuf[3];

	cmdbuf[0] = 1;
	cmdbuf[1] = reg | 0x8000;
	cmdbuf[2] = value;

	FN_DEBUG("write_cmos_register: 0x%04x <= 0x%04x\n", reg, value);
	int res = send_cmd(dev, 0x95, cmdbuf, 6, replybuf, 6);
	if (res < 0)
		FN_ERROR("write_cmos_register: send_cmd() returned %d\n", res);
	return res;
}
