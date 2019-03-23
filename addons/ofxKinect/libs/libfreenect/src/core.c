/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
 * for details.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <unistd.h>

#include "freenect_internal.h"
#include "registration.h"
#include "cameras.h"
#include "loader.h"


FREENECTAPI int freenect_init(freenect_context **ctx, freenect_usb_context *usb_ctx)
{
	int res;

	*ctx = (freenect_context*)malloc(sizeof(freenect_context));
	if (*ctx == NULL)
		return -1;

	memset(*ctx, 0, sizeof(freenect_context));

	(*ctx)->log_level = LL_NOTICE;
	(*ctx)->enabled_subdevices = (freenect_device_flags)(FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA);
	res = fnusb_init(&(*ctx)->usb, usb_ctx);
	if (res < 0) {
		free(*ctx);
		*ctx = NULL;
	}
	return res;
}

FREENECTAPI int freenect_shutdown(freenect_context *ctx)
{
	while (ctx->first) {
		FN_NOTICE("Device %p open during shutdown, closing...\n", ctx->first);
		freenect_close_device(ctx->first);
	}

	fnusb_shutdown(&ctx->usb);
	free(ctx);
	return 0;
}

FREENECTAPI int freenect_process_events(freenect_context *ctx)
{
	struct timeval timeout;
	timeout.tv_sec = 60;
	timeout.tv_usec = 0;
	return freenect_process_events_timeout(ctx, &timeout);
}

FREENECTAPI int freenect_process_events_timeout(freenect_context *ctx, struct timeval *timeout)
{
	int res = fnusb_process_events_timeout(&ctx->usb, timeout);
	// Iterate over the devices in ctx.  If any of them are flagged as
	freenect_device* dev = ctx->first;
	while(dev) {
		if (dev->usb_cam.device_dead) {
			FN_ERROR("USB camera marked dead, stopping streams\n");
			res = -1;
			freenect_stop_video(dev);
			freenect_stop_depth(dev);
		}
		if (dev->usb_audio.device_dead) {
			FN_ERROR("USB audio marked dead, stopping streams\n");
			res = -1; // Or something else to tell the user that the device just vanished.
			freenect_stop_audio(dev);
		}
		dev = dev->next;
	}
	return res;
}

FREENECTAPI int freenect_num_devices(freenect_context *ctx)
{
	return fnusb_num_devices(ctx);
}

FREENECTAPI int freenect_list_device_attributes(freenect_context *ctx, struct freenect_device_attributes **attribute_list)
{
	return fnusb_list_device_attributes(ctx, attribute_list);
}

FREENECTAPI void freenect_free_device_attributes(struct freenect_device_attributes *attribute_list)
{
	// Iterate over list, freeing contents of each item as we go.
	struct freenect_device_attributes* to_free;
	while(attribute_list != NULL) {
		to_free = attribute_list;
		if (attribute_list->camera_serial != NULL) {
			free((char*)attribute_list->camera_serial);
			attribute_list->camera_serial = NULL;
		}
		attribute_list = attribute_list->next;
		free(to_free);
	}
	return;
}

FREENECTAPI int freenect_supported_subdevices(void)
{
	return FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA | FREENECT_DEVICE_AUDIO;
}

FREENECTAPI void freenect_select_subdevices(freenect_context *ctx, freenect_device_flags subdevs)
{
	ctx->enabled_subdevices = (freenect_device_flags)(subdevs & (FREENECT_DEVICE_MOTOR | FREENECT_DEVICE_CAMERA | FREENECT_DEVICE_AUDIO));
}

FREENECTAPI freenect_device_flags freenect_enabled_subdevices(freenect_context *ctx)
{
	return ctx->enabled_subdevices;
}

FREENECTAPI int freenect_open_device(freenect_context *ctx, freenect_device **dev, int index)
{
	int res;
	freenect_device *pdev = (freenect_device*)malloc(sizeof(freenect_device));
	if (!pdev)
		return -1;

	memset(pdev, 0, sizeof(*pdev));

	pdev->parent = ctx;

	res = fnusb_open_subdevices(pdev, index);
	if (res < 0) {
		free(pdev);
		return res;
	}

	if (!ctx->first) {
		ctx->first = pdev;
	} else {
		freenect_device *prev = ctx->first;
		while (prev->next)
			prev = prev->next;
		prev->next = pdev;
	}

	*dev = pdev;

	// Do device-specific initialization
	if (pdev->usb_cam.dev) {
		if (freenect_camera_init(pdev) < 0) {
			return -1;
		}
	}

	return 0;
}

FREENECTAPI int freenect_open_device_by_camera_serial(freenect_context *ctx, freenect_device **dev, const char* camera_serial)
{
	// This is implemented by listing the devices and seeing which index (if
	// any) has a camera with a matching serial number, and then punting to
	// freenect_open_device with that index.
	struct freenect_device_attributes* attrlist;
	struct freenect_device_attributes* item;
	int count = fnusb_list_device_attributes(ctx, &attrlist);
	if (count < 0) {
		FN_ERROR("freenect_open_device_by_camera_serial: Couldn't enumerate serial numbers\n");
		return count;
	}
	int index = 0;
	for(item = attrlist ; item != NULL; item = item->next , index++) {
		if (strlen(item->camera_serial) == strlen(camera_serial) && strcmp(item->camera_serial, camera_serial) == 0) {
			freenect_free_device_attributes(attrlist);
			return freenect_open_device(ctx, dev, index);
		}
	}
	freenect_free_device_attributes(attrlist);
	FN_ERROR("freenect_open_device_by_camera_serial: Couldn't find a device with serial %s\n", camera_serial);
	return -1;
}

FREENECTAPI int freenect_close_device(freenect_device *dev)
{
	freenect_context *ctx = dev->parent;
	int res;

	if (dev->usb_cam.dev) {
		freenect_camera_teardown(dev);
	}

	res = fnusb_close_subdevices(dev);
	if (res < 0) {
		FN_ERROR("fnusb_close_subdevices failed: %d\n", res);
		return res;
	}

	freenect_device *last = NULL;
	freenect_device *cur = ctx->first;

	while (cur && cur != dev) {
		last = cur;
		cur = cur->next;
	}

	if (!cur) {
		FN_ERROR("device %p not found in linked list for this context!\n", dev);
		return -1;
	}

	if (last)
		last->next = cur->next;
	else
		ctx->first = cur->next;

	free(dev);
	return 0;
}

FREENECTAPI void freenect_set_user(freenect_device *dev, void *user)
{
	dev->user_data = user;
}

FREENECTAPI void *freenect_get_user(freenect_device *dev)
{
	return dev->user_data;
}

FREENECTAPI void freenect_set_log_level(freenect_context *ctx, freenect_loglevel level)
{
	ctx->log_level = level;
}

FREENECTAPI void freenect_set_log_callback(freenect_context *ctx, freenect_log_cb cb)
{
	ctx->log_cb = cb;
}

FN_INTERNAL void fn_log(freenect_context *ctx, freenect_loglevel level, const char *fmt, ...)
{
	va_list ap;

	if (level > ctx->log_level)
		return;

	if (ctx->log_cb) {
		char msgbuf[1024];

		va_start(ap, fmt);
		vsnprintf(msgbuf, 1024, fmt, ap);
		msgbuf[1023] = 0;
		va_end(ap);

		ctx->log_cb(ctx, level, msgbuf);
	} else {
		va_start(ap, fmt);
		vfprintf(stderr, fmt, ap);
		va_end(ap);
	}
}


FREENECTAPI void freenect_set_fw_address_nui(freenect_context * ctx, unsigned char * fw_ptr, unsigned int num_bytes)
{
    ctx->fn_fw_nui_ptr = fw_ptr;
    ctx->fn_fw_nui_size = num_bytes;
}

FREENECTAPI void freenect_set_fw_address_k4w(freenect_context * ctx, unsigned char * fw_ptr, unsigned int num_bytes)
{
    ctx->fn_fw_k4w_ptr = fw_ptr;
    ctx->fn_fw_k4w_size = num_bytes;
}

