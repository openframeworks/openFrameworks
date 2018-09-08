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
#include <unistd.h>
#include <libusb.h>
#include "freenect_internal.h"
#include "loader.h"

#ifdef _MSC_VER
	# define sleep(x) Sleep((x)*1000) 
#endif 


FN_INTERNAL int fnusb_num_devices(freenect_context *ctx)
{
	libusb_device **devs; // pointer to pointer of device, used to retrieve a list of devices
	ssize_t count = libusb_get_device_list (ctx->usb.ctx, &devs);
	if (count < 0)
		return (count >= INT_MIN) ? (int)count : -1;

	int number_found = 0, i = 0;
	struct libusb_device_descriptor desc;
	for (i = 0; i < count; ++i)
	{
		int r = libusb_get_device_descriptor (devs[i], &desc);
		if (r < 0)
		{
			FN_WARNING("Failed to query USB device descriptor.\n");
			continue;
		}

		if (desc.idVendor == VID_MICROSOFT)
		{
			if (desc.idProduct == PID_NUI_CAMERA || desc.idProduct == PID_K4W_CAMERA)
			{
				number_found++;
			}
			else if (desc.idProduct == PID_KV2_CAMERA)
			{
				FN_NOTICE("Skipping Kinect v2 device (needs https://github.com/libfreenect2).\n");
			}
		}
	}

	libusb_free_device_list (devs, 1);
	return number_found;
}

FN_INTERNAL short fnusb_is_camera(struct libusb_device_descriptor desc)
{
	return desc.idVendor == VID_MICROSOFT
	    && (desc.idProduct == PID_NUI_CAMERA || desc.idProduct == PID_K4W_CAMERA);
}

FN_INTERNAL short fnusb_is_motor(struct libusb_device_descriptor desc)
{
	return desc.idVendor == VID_MICROSOFT
	    && desc.idProduct == PID_NUI_MOTOR;
}

FN_INTERNAL short fnusb_is_audio(struct libusb_device_descriptor desc)
{
	return desc.idVendor == VID_MICROSOFT
	    && (desc.idProduct == PID_NUI_AUDIO
	     || desc.idProduct == PID_K4W_AUDIO ||  desc.idProduct == PID_K4W_AUDIO_ALT_1 || desc.idProduct == PID_K4W_AUDIO_ALT_2);
}

FN_INTERNAL libusb_device * fnusb_find_sibling_device(freenect_context* ctx, libusb_device* camera,
                                                      libusb_device** deviceList, int count,
                                                      short (*predicate)(struct libusb_device_descriptor))
{
	if (count <= 0) return NULL;

	const int cameraBusNo = libusb_get_bus_number(camera);
	if (cameraBusNo < 0) return NULL;
	const libusb_device * cameraParent = libusb_get_parent(camera);

	int siblingCount_Total = 0;
	int siblingCount_SameBus = 0;
	libusb_device* siblingMatch_Single = NULL;
	libusb_device* siblingMatch_SameBus = NULL;

	int i = 0;
	for (i = 0; i < count; i++)
	{
		libusb_device* currentDevice = deviceList[i];

		struct libusb_device_descriptor desc;
		int res = libusb_get_device_descriptor(currentDevice, &desc);
		if (res < 0) {
			continue;
		}

		if (!(*predicate)(desc)) {
			continue;
		}

		siblingCount_Total++;
		siblingMatch_Single = (siblingCount_Total == 1) ? currentDevice : NULL;

		if (cameraBusNo != libusb_get_bus_number(currentDevice)){
			continue;
		}

		siblingCount_SameBus++;
		siblingMatch_SameBus = (siblingCount_SameBus == 1) ? currentDevice : NULL;

		if (cameraParent == NULL || cameraParent != libusb_get_parent(currentDevice)) {
			continue;
		}

		FN_DEBUG("Found sibling device [same parent]\n");
		return currentDevice;
	}

	if (siblingMatch_SameBus != NULL) {
		FN_DEBUG("Found sibling device [single on same bus]\n");
		return siblingMatch_SameBus;
	}
	if (siblingMatch_Single != NULL) {
		FN_DEBUG("Found sibling device [single on system]\n");
		return siblingMatch_Single;
	}

	return NULL;
}

FN_INTERNAL int fnusb_list_device_attributes(freenect_context *ctx, struct freenect_device_attributes** attribute_list)
{
	*attribute_list = NULL; // initialize some return value in case the user is careless.
	libusb_device **devs;   // pointer to pointer of device, used to retrieve a list of devices
	ssize_t count = libusb_get_device_list (ctx->usb.ctx, &devs);
	if (count < 0)
	{
		return (count >= INT_MIN) ? (int)count : -1;
	}

	struct freenect_device_attributes** next_attr = attribute_list;

	// Pass over the list.  For each camera seen, if we already have a camera
	// for the newest_camera device, allocate a new one and append it to the list,
	// incrementing num_cams.
	int num_cams = 0;
	int i;
	for (i = 0; i < count; i++)
	{
		libusb_device* camera_device = devs[i];

		struct libusb_device_descriptor desc;
		int res = libusb_get_device_descriptor (camera_device, &desc);
		if (res < 0)
		{
			continue;
		}

		if (desc.idVendor == VID_MICROSOFT && (desc.idProduct == PID_NUI_CAMERA || desc.idProduct == PID_K4W_CAMERA))
		{
			// Verify that a serial number exists to query.  If not, don't touch the device.
			if (desc.iSerialNumber == 0)
			{
				continue;
			}

			libusb_device_handle *camera_handle;
			res = libusb_open(camera_device, &camera_handle);
			if (res != 0)
			{
				continue;
			}

			// Read string descriptor referring to serial number.
			unsigned char serial[256]; // String descriptors are at most 256 bytes.
			res = libusb_get_string_descriptor_ascii(camera_handle, desc.iSerialNumber, serial, 256);
			libusb_close(camera_handle);
			if (res < 0)
			{
				continue;
			}

			// K4W and 1473 don't provide a camera serial; use audio serial instead.
			const char* const K4W_1473_SERIAL = "0000000000000000";
			if (strncmp((const char*)serial, K4W_1473_SERIAL, 16) == 0)
			{
				libusb_device* audio_device = fnusb_find_sibling_device(ctx, camera_device, devs, count, &fnusb_is_audio);
				if (audio_device != NULL)
				{
					struct libusb_device_descriptor audio_desc;
					res = libusb_get_device_descriptor(audio_device, &audio_desc);
					if (res != 0)
					{
						FN_WARNING("Failed to get audio serial descriptors of K4W or 1473 device: %s\n", libusb_error_name(res));
					}
					else
					{
						libusb_device_handle * audio_handle = NULL;
						res = libusb_open(audio_device, &audio_handle);
						if (res != 0)
						{
							FN_WARNING("Failed to open audio device for serial of K4W or 1473 device: %s\n", libusb_error_name(res));
						}
						else
						{
							res = libusb_get_string_descriptor_ascii(audio_handle, audio_desc.iSerialNumber, serial, 256);
							libusb_close(audio_handle);
							if (res != 0)
							{
								FN_WARNING("Failed to get audio serial of K4W or 1473 device: %s\n", libusb_error_name(res));
							}
						}
					}
				}
			}

			// Add item to linked list.
			struct freenect_device_attributes* current_attr = (struct freenect_device_attributes*)malloc(sizeof(struct freenect_device_attributes));
			memset(current_attr, 0, sizeof(*current_attr));

			current_attr->camera_serial = strdup((char*)serial);
			*next_attr = current_attr;
			next_attr = &(current_attr->next);
			num_cams++;
		}
	}

	libusb_free_device_list(devs, 1);
	return num_cams;
}

FN_INTERNAL int fnusb_init(fnusb_ctx *ctx, freenect_usb_context *usb_ctx)
{
	int res;
	if (!usb_ctx) {
		res = libusb_init(&ctx->ctx);
		if (res >= 0) {
			ctx->should_free_ctx = 1;
			return 0;
		} else {
			ctx->should_free_ctx = 0;
			ctx->ctx = NULL;
			return res;
		}
	} else {
    // explicit cast required: in WIN32, freenect_usb_context* maps to void*
    ctx->ctx = (libusb_context*)usb_ctx;
		ctx->should_free_ctx = 0;
		return 0;
	}
}

FN_INTERNAL int fnusb_shutdown(fnusb_ctx *ctx)
{
	//int res;
	if (ctx->should_free_ctx) {
		libusb_exit(ctx->ctx);
		ctx->ctx = NULL;
	}
	return 0;
}

FN_INTERNAL int fnusb_process_events(fnusb_ctx *ctx)
{
	return libusb_handle_events(ctx->ctx);
}

FN_INTERNAL int fnusb_process_events_timeout(fnusb_ctx *ctx, struct timeval* timeout)
{
	return libusb_handle_events_timeout(ctx->ctx, timeout);
}

FN_INTERNAL int fnusb_claim_camera(freenect_device* dev)
{
	freenect_context *ctx = dev->parent;

	int ret = 0;

#ifndef _WIN32 // todo: necessary?
	// Detach an existing kernel driver for the device
	ret = libusb_kernel_driver_active(dev->usb_cam.dev, 0);
	if (ret == 1)
	{
		ret = libusb_detach_kernel_driver(dev->usb_cam.dev, 0);
		if (ret < 0)
		{
			FN_ERROR("Failed to detach camera kernel driver: %s\n", libusb_error_name(ret));
			libusb_close(dev->usb_cam.dev);
			dev->usb_cam.dev = NULL;
			return ret;
		}
	}
#endif

	ret = libusb_claim_interface(dev->usb_cam.dev, 0);
	if (ret < 0)
	{
		FN_ERROR("Failed to claim camera interface: %s\n", libusb_error_name(ret));
		libusb_close(dev->usb_cam.dev);
		dev->usb_cam.dev = NULL;
		return ret;
	}

	if (dev->usb_cam.PID == PID_K4W_CAMERA)
	{
		ret = libusb_set_interface_alt_setting(dev->usb_cam.dev, 0, 1);
		if (ret != 0)
		{
			FN_ERROR("Failed to set alternate interface #1 for K4W: %s\n", libusb_error_name(ret));
			libusb_close(dev->usb_cam.dev);
			dev->usb_cam.dev = NULL;
			return ret;
		}
	}

	return ret;
}

FN_INTERNAL int fnusb_keep_alive_led(freenect_context* ctx, libusb_device* audio)
{
	if (audio == NULL) return -1;

	int res = 0;
	libusb_device_handle * audioHandle = NULL;

	res = libusb_open(audio, &audioHandle);
	if (res < 0) {
		FN_ERROR("Failed to set the LED of K4W or 1473 device: %s\n", libusb_error_name(res));
		return res;
	}

	// we need to do this as it is possible that the device was not closed properly in a previous session
	// if we don't do this and the device wasn't closed properly - it can cause infinite hangs on LED and TILT functions
	libusb_reset_device(audioHandle);
	libusb_close(audioHandle);

	res = libusb_open(audio, &audioHandle);
	if (res < 0) {
		FN_ERROR("Failed to set the LED of K4W or 1473 device: %s\n", libusb_error_name(res));
		return res;
	}

	res = libusb_claim_interface(audioHandle, 0);
	if (res < 0)
	{
		FN_ERROR("Unable to claim interface %s\n", libusb_error_name(res));
	}
	else
	{
		fnusb_set_led_alt(audioHandle, ctx, LED_GREEN);
		libusb_release_interface(audioHandle, 0);
	}

	libusb_close(audioHandle);
	return res;
}

FN_INTERNAL int fnusb_open_subdevices(freenect_device *dev, int index)
{
	freenect_context *ctx = dev->parent;

	dev->device_does_motor_control_with_audio = 0;
	dev->motor_control_with_audio_enabled = 0;
    
	dev->usb_cam.parent = dev;
	dev->usb_cam.dev = NULL;
	dev->usb_motor.parent = dev;
	dev->usb_motor.dev = NULL;
	dev->usb_audio.parent = dev;
	dev->usb_audio.dev = NULL;

	libusb_device **devs; // pointer to pointer of device, used to retrieve a list of devices
	ssize_t count = libusb_get_device_list (dev->parent->usb.ctx, &devs); //get the list of devices
	if (count < 0)
		return -1;

	libusb_device* camera = NULL;

	int res = 0;
	int i = 0, nr_cam = 0;
	for (i = 0; i < count; i++)
	{
		struct libusb_device_descriptor desc;
		res = libusb_get_device_descriptor (devs[i], &desc);
		if (res < 0)
			continue;

		if (!fnusb_is_camera(desc)) {
			continue;
		}

		if (nr_cam != index) {
			nr_cam++;
			continue;
		}

		camera = devs[i]; // found the requested camera
		if (ctx->enabled_subdevices & FREENECT_DEVICE_CAMERA)
		{
			res = libusb_open(camera, &dev->usb_cam.dev);
			if (res < 0 || !dev->usb_cam.dev)
			{
				FN_ERROR("Could not open camera: %s\n", libusb_error_name(res));
				dev->usb_cam.dev = NULL;
				goto failure;
			}

			if (desc.idProduct == PID_K4W_CAMERA || desc.bcdDevice != fn_le32(267))
			{
				freenect_device_flags requested_devices = ctx->enabled_subdevices;

				// Not the 1414 kinect so remove the motor flag, this should preserve the audio flag if set
				ctx->enabled_subdevices = (freenect_device_flags)(ctx->enabled_subdevices & ~FREENECT_DEVICE_MOTOR);

				ctx->zero_plane_res = 334;
				dev->device_does_motor_control_with_audio = 1;

				// set the LED for non 1414 devices to keep the camera alive for some systems which get freezes
				libusb_device * audio = fnusb_find_sibling_device(ctx, camera, devs, count, &fnusb_is_audio);
				fnusb_keep_alive_led(ctx, audio);

				// for newer devices we need to enable the audio device for motor control
				// we only do this though if motor has been requested.
				if ((requested_devices & FREENECT_DEVICE_MOTOR) && (requested_devices & FREENECT_DEVICE_AUDIO) == 0)
				{
					ctx->enabled_subdevices = (freenect_device_flags)(ctx->enabled_subdevices | FREENECT_DEVICE_AUDIO);
				}
			}
			else
			{
				// The good old kinect that tilts and tweets
				ctx->zero_plane_res = 322;
			}

			res = fnusb_claim_camera(dev);
			if (res < 0) {
				goto failure;
			}
		}

		break;
	}
	
	if (res < 0 || camera == NULL) {
		res = -1;
		goto failure;
	}

	// FIND MOTOR
	if (ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR)
	{
		libusb_device* motor = fnusb_find_sibling_device(ctx, camera, devs, count, &fnusb_is_motor);
		if (motor == NULL) {
			FN_ERROR("Could not find device sibling\n");
			res = -1;
			goto failure;
		}

		struct libusb_device_descriptor desc;
		res = libusb_get_device_descriptor(motor, &desc);
		if (res < 0) {
			FN_ERROR("Could not query device: %s\n", libusb_error_name(res));
			goto failure;
		}

		res = libusb_open(motor, &dev->usb_motor.dev);
		if (res < 0 || !dev->usb_motor.dev)
		{
			FN_ERROR("Could not open device: %s\n", libusb_error_name(res));
			dev->usb_motor.dev = NULL;
			goto failure;
		}

		res = libusb_claim_interface(dev->usb_motor.dev, 0);
		if (res < 0) {
			FN_ERROR("Could not claim interface: %s\n", libusb_error_name(res));
			libusb_close(dev->usb_motor.dev);
			dev->usb_motor.dev = NULL;
			goto failure;
		}

		dev->usb_motor.VID = desc.idVendor;
		dev->usb_motor.PID = desc.idProduct;
	}
    
	// FIND AUDIO
	if (ctx->enabled_subdevices & FREENECT_DEVICE_AUDIO)
	{
		libusb_device* audio = fnusb_find_sibling_device(ctx, camera, devs, count, &fnusb_is_audio);
		if (audio == NULL) {
			FN_ERROR("Could not find device sibling\n");
			res = -1;
			goto failure;
		}

		struct libusb_device_descriptor desc;
		res = libusb_get_device_descriptor(audio, &desc);
		if (res < 0) {
			FN_ERROR("Could not query device: %s\n", libusb_error_name(res));
			goto failure;
		}

		res = libusb_open(audio, &dev->usb_audio.dev);
		if (res < 0 || !dev->usb_audio.dev)
		{
			FN_ERROR("Could not open device: %s\n", libusb_error_name(res));
			dev->usb_audio.dev = NULL;
			goto failure;
		}

		res = libusb_claim_interface(dev->usb_audio.dev, 0);
		if (res < 0) {
			FN_ERROR("Could not claim interface: %s\n", libusb_error_name(res));
			libusb_close(dev->usb_audio.dev);
			dev->usb_audio.dev = NULL;
			goto failure;
		}

		dev->usb_audio.VID = desc.idVendor;
		dev->usb_audio.PID = desc.idProduct;

		// Using the device handle that we've claimed, see if this
		// device has already uploaded firmware (has 2 interfaces).
		// If not, save the serial number (by reading the appropriate
		// descriptor), upload the firmware, and then enter a loop
		// waiting for a device with the same serial number to
		// reappear.
		int num_interfaces = fnusb_num_interfaces(&dev->usb_audio);
		if (num_interfaces >= 2)
		{
			if (dev->device_does_motor_control_with_audio)
			{
				dev->motor_control_with_audio_enabled = 1;
			}
		}
		else
		{
			// Read the serial number from the string descriptor and save it.
			unsigned char string_desc[256]; // String descriptors are at most 256 bytes
			res = libusb_get_string_descriptor_ascii(dev->usb_audio.dev, desc.iSerialNumber, string_desc, 256);
			if (res < 0) {
				FN_ERROR("Failed to retrieve serial number for audio device in bootloader state\n");
				goto failure;
			}

			char* audio_serial = strdup((char*)string_desc);

			FN_SPEW("Uploading firmware to audio device in bootloader state.\n");

			if (desc.idProduct == PID_NUI_AUDIO && ctx->fn_fw_nui_ptr && ctx->fn_fw_nui_size > 0)
			{
				FN_SPEW("loading firmware from memory\n");
				res = upload_firmware_from_memory(&dev->usb_audio, ctx->fn_fw_nui_ptr, ctx->fn_fw_nui_size);
			}
			else if (desc.idProduct == PID_K4W_AUDIO && ctx->fn_fw_k4w_ptr && ctx->fn_fw_k4w_size > 0)
			{
				FN_SPEW("loading firmware from memory\n");
				res = upload_firmware_from_memory(&dev->usb_audio, ctx->fn_fw_k4w_ptr, ctx->fn_fw_k4w_size);
			}
			else
			{
				res = upload_firmware(&dev->usb_audio, "audios.bin");
			}

			if (res < 0) {
				FN_ERROR("upload_firmware failed: %d\n", res);
				free(audio_serial);
				goto failure;
			}

			libusb_close(dev->usb_audio.dev);
			dev->usb_audio.dev = NULL;

			// Wait for the device to reappear.
			int loops = 0;
			for (loops = 0; loops < 10; loops++)
			{
				FN_SPEW("Try %d: Looking for new audio device matching serial %s\n", loops, audio_serial);
				libusb_device **new_dev_list;
				int dev_index;
				ssize_t num_new_devs = libusb_get_device_list(ctx->usb.ctx, &new_dev_list);

				for (dev_index = 0; dev_index < num_new_devs; ++dev_index)
				{
					struct libusb_device_descriptor new_dev_desc;
					int r;
					r = libusb_get_device_descriptor (new_dev_list[dev_index], &new_dev_desc);
					if (r < 0)
						continue;
					// If this dev is a Kinect audio device, open device, read serial, and compare.
					if (fnusb_is_audio(new_dev_desc))
					{
						FN_SPEW("Matched VID/PID!\n");
						libusb_device_handle* new_dev_handle;
						// Open device
						r = libusb_open(new_dev_list[dev_index], &new_dev_handle);
						if (r < 0)
							continue;
						// Read serial
						r = libusb_get_string_descriptor_ascii(new_dev_handle, new_dev_desc.iSerialNumber, string_desc, 256);
						if (r < 0)
						{
							FN_SPEW("Lost new audio device while fetching serial number.\n");
							libusb_close(new_dev_handle);
							continue;
						}
						// Compare to expected serial
						if (r == strlen(audio_serial) && strcmp((char*)string_desc, audio_serial) == 0)
						{
							// We found it!
							r = libusb_claim_interface(new_dev_handle, 0);
							if (r != 0)
							{
								// Ouch, found the device but couldn't claim the interface.
								FN_SPEW("Device with serial %s reappeared but couldn't claim interface 0\n", audio_serial);
								libusb_close(new_dev_handle);
								continue;
							}
							// Save the device handle.
							dev->usb_audio.dev = new_dev_handle;

							// Verify that we've actually found a device running the right firmware.
							num_interfaces = fnusb_num_interfaces(&dev->usb_audio);

							if (num_interfaces >= 2)
							{
								if (dev->device_does_motor_control_with_audio)
								{
									dev->motor_control_with_audio_enabled = 1;
								}
							}
							else
							{
								FN_SPEW("Opened audio with matching serial but too few interfaces.\n");
								dev->usb_audio.dev = NULL;
								libusb_close(new_dev_handle);
								continue;
							}

							break;
						}
						else
						{
							FN_SPEW("Got serial %s, expected serial %s\n", (char*)string_desc, audio_serial);
						}
					}
				}

				libusb_free_device_list(new_dev_list, 1);
				// If we found the right device, break out of this loop.
				if (dev->usb_audio.dev)
					break;
				// Sleep for a second to give the device more time to reenumerate.
				sleep(1);
			}

			free(audio_serial);
		}
	}

	if ((dev->usb_cam.dev || !(ctx->enabled_subdevices & FREENECT_DEVICE_CAMERA))
   && (dev->usb_motor.dev || !(ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR))
   && (dev->usb_audio.dev || !(ctx->enabled_subdevices & FREENECT_DEVICE_AUDIO)))
	{
		// Each requested subdevice is open.
		goto finally;
	}

failure:
	fnusb_close_subdevices(dev);

finally:
	libusb_free_device_list(devs, 1); // free the list, unref the devices in it
	return res;
}

FN_INTERNAL int fnusb_close_subdevices(freenect_device *dev)
{
	if (dev->usb_cam.dev) {
		libusb_release_interface(dev->usb_cam.dev, 0);
#ifndef _WIN32
		libusb_attach_kernel_driver(dev->usb_cam.dev, 0);
#endif
		libusb_close(dev->usb_cam.dev);
		dev->usb_cam.dev = NULL;
	}
	if (dev->usb_motor.dev) {
		libusb_release_interface(dev->usb_motor.dev, 0);
		libusb_close(dev->usb_motor.dev);
		dev->usb_motor.dev = NULL;
	}
	if (dev->usb_audio.dev) {
		libusb_release_interface(dev->usb_audio.dev, 0);
		libusb_close(dev->usb_audio.dev);
		dev->usb_audio.dev = NULL;
	}
	return 0;
}

static void LIBUSB_CALL iso_callback(struct libusb_transfer *xfer)
{
	int i;
	fnusb_isoc_stream *strm = (fnusb_isoc_stream*)xfer->user_data;
	freenect_context *ctx = strm->parent->parent->parent;

	if (strm->dead) {
		strm->dead_xfers++;
		FN_SPEW("EP %02x transfer complete, %d left\n", xfer->endpoint, strm->num_xfers - strm->dead_xfers);
		return;
	}

	switch(xfer->status) {
		case LIBUSB_TRANSFER_COMPLETED: // Normal operation.
		{
			uint8_t *buf = (uint8_t*)xfer->buffer;
			for (i=0; i<strm->pkts; i++) {
				strm->cb(strm->parent->parent, buf, xfer->iso_packet_desc[i].actual_length);
				buf += strm->len;
			}
			int res;
			res = libusb_submit_transfer(xfer);
			if (res != 0) {
				FN_ERROR("iso_callback(): failed to resubmit transfer after successful completion: %s\n", libusb_error_name(res));
				strm->dead_xfers++;
				if (res == LIBUSB_ERROR_NO_DEVICE) {
					strm->parent->device_dead = 1;
				}
			}
			break;
		}
		case LIBUSB_TRANSFER_NO_DEVICE:
		{
			// We lost the device we were talking to.  This is a large problem,
			// and one that we should eventually come up with a way to
			// properly propagate up to the caller.
			if(!strm->parent->device_dead) {
				FN_ERROR("USB device disappeared, cancelling stream %02x :(\n", xfer->endpoint);
			}
			strm->dead_xfers++;
			strm->parent->device_dead = 1;
			break;
		}
		case LIBUSB_TRANSFER_CANCELLED:
		{
			if(strm->dead) {
				FN_SPEW("EP %02x transfer cancelled\n", xfer->endpoint);
			} else {
				// This seems to be a libusb bug on OSX - instead of completing
				// the transfer with LIBUSB_TRANSFER_NO_DEVICE, the transfers
				// simply come back cancelled by the OS.  We can detect this,
				// though - the stream should be marked dead if we're
				// intentionally cancelling transfers.
				if(!strm->parent->device_dead) {
					FN_ERROR("Got cancelled transfer, but we didn't request it - device disconnected?\n");
				}
				strm->parent->device_dead = 1;
			}
			strm->dead_xfers++;
			break;
		}
		default:
		{
			// On other errors, resubmit the transfer - in particular, libusb
			// on OSX tends to hit random errors a lot.  If we don't resubmit
			// the transfers, eventually all of them die and then we don't get
			// any more data from the Kinect.
			FN_WARNING("Isochronous transfer error: %d\n", xfer->status);
			int res;
			res = libusb_submit_transfer(xfer);
			if (res != 0) {
				FN_ERROR("Isochronous transfer resubmission failed after unknown error: %s\n", libusb_error_name(res));
				strm->dead_xfers++;
				if (res == LIBUSB_ERROR_NO_DEVICE) {
					strm->parent->device_dead = 1;
				}
			}
			break;
		}
	}
}

FN_INTERNAL int fnusb_get_max_iso_packet_size(fnusb_dev *dev, unsigned char endpoint, int default_size)
{
	freenect_context *ctx = dev->parent->parent;

	int size = libusb_get_max_iso_packet_size(libusb_get_device(dev->dev), endpoint);
	if (size <= 0)
	{
		FN_WARNING("libusb_get_max_iso_packet_size() returned %d; using default %d\n", size, default_size);
		size = default_size;
	}
	return size;
}

FN_INTERNAL int fnusb_start_iso(fnusb_dev *dev, fnusb_isoc_stream *strm, fnusb_iso_cb cb, unsigned char endpoint, int xfers, int pkts, int len)
{
	freenect_context *ctx = dev->parent->parent;

	strm->parent = dev;
	strm->cb = cb;
	strm->num_xfers = xfers;
	strm->pkts = pkts;
	strm->len = len;
	strm->buffer = (uint8_t*)malloc(xfers * pkts * len);
	strm->xfers = (struct libusb_transfer**)malloc(sizeof(struct libusb_transfer*) * xfers);
	strm->dead = 0;
	strm->dead_xfers = 0;

	int i;
	uint8_t *bufp = strm->buffer;

	for (i = 0; i < xfers; i++)
	{
		FN_SPEW("Creating endpoint %02x transfer #%d\n", endpoint, i);

		strm->xfers[i] = libusb_alloc_transfer(pkts);
		if (strm->xfers[i] == NULL)
		{
			FN_WARNING("Failed to allocate transfer\n");
			strm->dead_xfers++;
		}
		else
		{
			libusb_fill_iso_transfer(strm->xfers[i], dev->dev, endpoint, bufp, pkts * len, pkts, iso_callback, strm, 0);
			libusb_set_iso_packet_lengths(strm->xfers[i], len);

			int ret = libusb_submit_transfer(strm->xfers[i]);
			if (ret < 0)
			{
				FN_WARNING("Failed to submit isochronous transfer %d: %s\n", i, libusb_error_name(ret));
				strm->dead_xfers++;
			}
		}

		bufp += pkts*len;
	}

	return 0;
}

FN_INTERNAL int fnusb_stop_iso(fnusb_dev *dev, fnusb_isoc_stream *strm)
{
	freenect_context *ctx = dev->parent->parent;
	int i;

	FN_FLOOD("fnusb_stop_iso() called\n");

	strm->dead = 1;

	for (i=0; i<strm->num_xfers; i++)
		libusb_cancel_transfer(strm->xfers[i]);
	FN_FLOOD("fnusb_stop_iso() cancelled all transfers\n");

	while (strm->dead_xfers < strm->num_xfers) {
		FN_FLOOD("fnusb_stop_iso() dead = %d\tnum = %d\n", strm->dead_xfers, strm->num_xfers);
		libusb_handle_events(ctx->usb.ctx);
	}

	for (i=0; i<strm->num_xfers; i++)
		libusb_free_transfer(strm->xfers[i]);
	FN_FLOOD("fnusb_stop_iso() freed all transfers\n");

	free(strm->buffer);
	free(strm->xfers);

	FN_FLOOD("fnusb_stop_iso() freed buffers and stream\n");
	FN_FLOOD("fnusb_stop_iso() done\n");
	return 0;
}

FN_INTERNAL int fnusb_control(fnusb_dev *dev, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint8_t *data, uint16_t wLength)
{
	return libusb_control_transfer(dev->dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, 0);
}

FN_INTERNAL int fnusb_bulk(fnusb_dev *dev, uint8_t endpoint, uint8_t *data, int len, int *transferred) {
	*transferred = 0;
	return libusb_bulk_transfer(dev->dev, endpoint, data, len, transferred, 0);
}

FN_INTERNAL int fnusb_num_interfaces(fnusb_dev *dev) {
	int retval = 0;
	int res;
	libusb_device* d = libusb_get_device(dev->dev);
	struct libusb_config_descriptor* config;
	res = libusb_get_active_config_descriptor(d, &config);
	if (res < 0) // Something went wrong
		return res;
	retval = config->bNumInterfaces;
	libusb_free_config_descriptor(config);
	return retval;
}
