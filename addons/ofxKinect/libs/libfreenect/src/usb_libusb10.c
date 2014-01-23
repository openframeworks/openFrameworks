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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libusb.h>
#include "freenect_internal.h"
#include "loader.h"

FN_INTERNAL int fnusb_num_devices(fnusb_ctx *ctx)
{
	libusb_device **devs; 
	//pointer to pointer of device, used to retrieve a list of devices	
	ssize_t cnt = libusb_get_device_list (ctx->ctx, &devs); 
	//get the list of devices	
	if (cnt < 0)
		return (-1);
	int nr = 0, i = 0;
	struct libusb_device_descriptor desc;
	for (i = 0; i < cnt; ++i)
	{
		int r = libusb_get_device_descriptor (devs[i], &desc);
		if (r < 0)
			continue;
		if (desc.idVendor == VID_MICROSOFT && (desc.idProduct == PID_NUI_CAMERA || desc.idProduct == PID_K4W_CAMERA))
			nr++;
	}
	libusb_free_device_list (devs, 1);
	// free the list, unref the devices in it
	return nr;
}

FN_INTERNAL int fnusb_list_device_attributes(fnusb_ctx *ctx, struct freenect_device_attributes** attribute_list)
{
	*attribute_list = NULL; // initialize some return value in case the user is careless.
	libusb_device **devs;
	//pointer to pointer of device, used to retrieve a list of devices
	ssize_t count = libusb_get_device_list (ctx->ctx, &devs);
	if (count < 0)
		return -1;

	struct freenect_device_attributes** camera_prev_next = attribute_list;

	// Pass over the list.  For each camera seen, if we already have a camera
	// for the newest_camera device, allocate a new one and append it to the list,
	// incrementing num_devs.  Likewise for each audio device.
	struct libusb_device_descriptor desc;
	int num_cams = 0;
	int i;
	for (i = 0; i < count; i++) {
		int r = libusb_get_device_descriptor (devs[i], &desc);
		if (r < 0)
			continue;
		if (desc.idVendor == VID_MICROSOFT && (desc.idProduct == PID_NUI_CAMERA || desc.idProduct == PID_K4W_CAMERA)) {
			// Verify that a serial number exists to query.  If not, don't touch the device.
			if (desc.iSerialNumber == 0) {
				continue;
			}

			// Open device.
			int res;
			libusb_device_handle *this_device;
			res = libusb_open(devs[i], &this_device);
			unsigned char string_desc[256]; // String descriptors are at most 256 bytes.
			if (res != 0) {
				continue;
			}

			// Read string descriptor referring to serial number.
			res = libusb_get_string_descriptor_ascii(this_device, desc.iSerialNumber, string_desc, 256);
			libusb_close(this_device);
			if (res < 0) {
				continue;
			}

			// Add item to linked list.
			struct freenect_device_attributes* new_dev_attrs = (struct freenect_device_attributes*)malloc(sizeof(struct freenect_device_attributes));
			memset(new_dev_attrs, 0, sizeof(*new_dev_attrs));

			*camera_prev_next = new_dev_attrs;
			// Copy string with serial number
			new_dev_attrs->camera_serial = strdup((char*)string_desc);
			camera_prev_next = &(new_dev_attrs->next);
			// Increment number of cameras found
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

FN_INTERNAL int fnusb_open_subdevices(freenect_device *dev, int index)
{
	freenect_context *ctx = dev->parent;

	dev->usb_cam.parent = dev;
	dev->usb_cam.dev = NULL;
	dev->usb_motor.parent = dev;
	dev->usb_motor.dev = NULL;
#ifdef BUILD_AUDIO
	dev->usb_audio.parent = dev;
	dev->usb_audio.dev = NULL;
#endif

	libusb_device **devs; //pointer to pointer of device, used to retrieve a list of devices
	ssize_t cnt = libusb_get_device_list (dev->parent->usb.ctx, &devs); //get the list of devices
	if (cnt < 0)
		return -1;

	int i = 0, nr_cam = 0, nr_mot = 0;
#ifdef BUILD_AUDIO
	int nr_audio = 0;
#endif
	int res;
	struct libusb_device_descriptor desc;

	for (i = 0; i < cnt; i++) {
		int r = libusb_get_device_descriptor (devs[i], &desc);
		if (r < 0)
			continue;

		if (desc.idVendor != VID_MICROSOFT)
			continue;
		res = 0;
		// Search for the camera
		if ((ctx->enabled_subdevices & FREENECT_DEVICE_CAMERA) && !dev->usb_cam.dev && (desc.idProduct == PID_NUI_CAMERA || desc.idProduct == PID_K4W_CAMERA)) {
			// If the index given by the user matches our camera index
			if (nr_cam == index) {
				res = libusb_open (devs[i], &dev->usb_cam.dev);
				if (res < 0 || !dev->usb_cam.dev) {
					FN_ERROR("Could not open camera: %d\n", res);
					dev->usb_cam.dev = NULL;
					break;
				}
				if(desc.idProduct == PID_K4W_CAMERA || desc.bcdDevice != fn_le32(267)){
					/* Not the old kinect so we only set up the camera*/ 
					ctx->enabled_subdevices = FREENECT_DEVICE_CAMERA;
					ctx->zero_plane_res = 334;
				}else{
					/* The good old kinect that tilts and tweets */
					ctx->zero_plane_res = 322;
				}
				
#ifndef _WIN32
				// Detach an existing kernel driver for the device
				res = libusb_kernel_driver_active(dev->usb_cam.dev, 0);
				if (res == 1) {
					res = libusb_detach_kernel_driver(dev->usb_cam.dev, 0);
					if (res < 0) {
						FN_ERROR("Could not detach kernel driver for camera: %d\n", res);
						libusb_close(dev->usb_cam.dev);
						dev->usb_cam.dev = NULL;
						break;
					}
				}
#endif
				res = libusb_claim_interface (dev->usb_cam.dev, 0);
				if (res < 0) {
					FN_ERROR("Could not claim interface on camera: %d\n", res);
					libusb_close(dev->usb_cam.dev);
					dev->usb_cam.dev = NULL;
					break;
				}
				if(desc.idProduct == PID_K4W_CAMERA){
					res = libusb_set_interface_alt_setting(dev->usb_cam.dev, 0, 1);
         				if (res != 0) {
           					FN_ERROR("Failed to set alternate interface #1 for K4W: %d\n", res);
           					libusb_close(dev->usb_cam.dev);
          					dev->usb_cam.dev = NULL;
           					break;
          				}
					
				}
			} else {
				nr_cam++;
			}
		}
	}
	
	if(ctx->enabled_subdevices == FREENECT_DEVICE_CAMERA || res < 0) cnt = 0;
	
		// Search for the motor
	
	for (i = 0; i < cnt; i++) {
		int r = libusb_get_device_descriptor (devs[i], &desc);
		if (r < 0)
			continue;

		if (desc.idVendor != VID_MICROSOFT)
			continue;
		if ((ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR) && !dev->usb_motor.dev && desc.idProduct == PID_NUI_MOTOR) {
			// If the index given by the user matches our camera index
			if (nr_mot == index) {
				res = libusb_open (devs[i], &dev->usb_motor.dev);
				if (res < 0 || !dev->usb_motor.dev) {
					FN_ERROR("Could not open motor: %d\n", res);
					dev->usb_motor.dev = NULL;
					break;
				}
				res = libusb_claim_interface (dev->usb_motor.dev, 0);
				if (res < 0) {
					FN_ERROR("Could not claim interface on motor: %d\n", res);
					libusb_close(dev->usb_motor.dev);
					dev->usb_motor.dev = NULL;
					break;
				}
			} else {
				nr_mot++;
			}
		}

#ifdef BUILD_AUDIO
		// TODO: check that the firmware has already been loaded; if not, upload firmware.
		// Search for the audio
		if ((ctx->enabled_subdevices & FREENECT_DEVICE_AUDIO) && !dev->usb_audio.dev && (desc.idProduct == PID_NUI_AUDIO || desc.idProduct == PID_K4W_AUDIO)) {
			// If the index given by the user matches our audio index
			if (nr_audio == index) {
				res = libusb_open (devs[i], &dev->usb_audio.dev);
				if (res < 0 || !dev->usb_audio.dev) {
					FN_ERROR("Could not open audio: %d\n", res);
					dev->usb_audio.dev = NULL;
					break;
				}
				res = libusb_claim_interface (dev->usb_audio.dev, 0);
				if (res < 0) {
					FN_ERROR("Could not claim interface on audio: %d\n", res);
					libusb_close(dev->usb_audio.dev);
					dev->usb_audio.dev = NULL;
					break;
				}
				// Using the device handle that we've claimed, see if this
				// device has already uploaded firmware (has 2 interfaces).  If
				// not, save the serial number (by reading the appropriate
				// descriptor), upload the firmware, and then enter a loop
				// waiting for a device with the same serial number to
				// reappear.
				int num_interfaces = fnusb_num_interfaces(&dev->usb_audio);
				if (num_interfaces == 1) {
					// Read the serial number from the string descriptor and save it.
					unsigned char string_desc[256]; // String descriptors are at most 256 bytes
					res = libusb_get_string_descriptor_ascii(dev->usb_audio.dev, desc.iSerialNumber, string_desc, 256);
					if (res < 0) {
						FN_ERROR("Failed to retrieve serial number for audio device in bootloader state\n");
						break;
					}
					char* audio_serial = strdup((char*)string_desc);

					FN_SPEW("Uploading firmware to audio device in bootloader state.\n");
					res = upload_firmware(&dev->usb_audio);
					if (res < 0) {
						FN_ERROR("upload_firmware failed: %d\n", res);
						break;
					}
					libusb_close(dev->usb_audio.dev);
					dev->usb_audio.dev = NULL;
					// Wait for the device to reappear.
					int loops = 0;
					for (loops = 0; loops < 10; loops++) { // Loop for at most 10 tries.
						FN_SPEW("Try %d: Looking for new audio device matching serial %s\n", loops, audio_serial);
						// Scan devices.
						libusb_device **new_dev_list;
						int dev_index;
						ssize_t num_new_devs = libusb_get_device_list(ctx->usb.ctx, &new_dev_list);
						for (dev_index = 0; dev_index < num_new_devs; ++dev_index) {
							struct libusb_device_descriptor new_dev_desc;
							int r;
							r = libusb_get_device_descriptor (new_dev_list[dev_index], &new_dev_desc);
							if (r < 0)
								continue;
							// If this dev is a Kinect audio device, open device, read serial, and compare.
							if (new_dev_desc.idVendor == VID_MICROSOFT && new_dev_desc.idProduct == PID_NUI_AUDIO) {
								FN_SPEW("Matched VID/PID!\n");
								libusb_device_handle* new_dev_handle;
								// Open device
								r = libusb_open(new_dev_list[dev_index], &new_dev_handle);
								if (r < 0)
									continue;
								// Read serial
								r = libusb_get_string_descriptor_ascii(new_dev_handle, new_dev_desc.iSerialNumber, string_desc, 256);
								if (r < 0) {
									FN_SPEW("Lost new audio device while fetching serial number.\n");
									libusb_close(new_dev_handle);
									continue;
								}
								// Compare to expected serial
								if (r == strlen(audio_serial) && strcmp((char*)string_desc, audio_serial) == 0) {
									// We found it!
									r = libusb_claim_interface(new_dev_handle, 0);
									if (r != 0) {
										// Ouch, found the device but couldn't claim the interface.
										FN_SPEW("Device with serial %s reappeared but couldn't claim interface 0\n", audio_serial);
										libusb_close(new_dev_handle);
										continue;
									}
									// Save the device handle.
									dev->usb_audio.dev = new_dev_handle;
									// Verify that we've actually found a device running the right firmware.
									if (fnusb_num_interfaces(&dev->usb_audio) != 2) {
										FN_SPEW("Opened audio with matching serial but too few interfaces.\n");
										dev->usb_audio.dev = NULL;
										libusb_close(new_dev_handle);
										continue;
									}
									break;
								} else {
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
			} else {
				nr_audio++;
			}
		}
#endif

	}

	libusb_free_device_list (devs, 1);  // free the list, unref the devices in it

	// Check that each subdevice is either opened or not enabled.
	if ( (dev->usb_cam.dev || !(ctx->enabled_subdevices & FREENECT_DEVICE_CAMERA))
		&& (dev->usb_motor.dev || !(ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR))
#ifdef BUILD_AUDIO
		&& (dev->usb_audio.dev || !(ctx->enabled_subdevices & FREENECT_DEVICE_AUDIO))
#endif
		) {
		return 0;
	} else {
		if (dev->usb_cam.dev) {
			libusb_release_interface(dev->usb_cam.dev, 0);
			libusb_close(dev->usb_cam.dev);
		}
		if (dev->usb_motor.dev) {
			libusb_release_interface(dev->usb_motor.dev, 0);
			libusb_close(dev->usb_motor.dev);
		}
#ifdef BUILD_AUDIO
		if (dev->usb_audio.dev) {
			libusb_release_interface(dev->usb_audio.dev, 0);
			libusb_close(dev->usb_audio.dev);
		}
#endif
		return -1;
	}
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
#ifdef BUILD_AUDIO
	if (dev->usb_audio.dev) {
		libusb_release_interface(dev->usb_audio.dev, 0);
		libusb_close(dev->usb_audio.dev);
		dev->usb_audio.dev = NULL;
	}
#endif
	return 0;
}

static void iso_callback(struct libusb_transfer *xfer)
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
				FN_ERROR("iso_callback(): failed to resubmit transfer after successful completion: %d\n", res);
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
				FN_ERROR("Isochronous transfer resubmission failed after unknown error: %d\n", res);
				strm->dead_xfers++;
				if (res == LIBUSB_ERROR_NO_DEVICE) {
					strm->parent->device_dead = 1;
				}
			}
			break;
		}
	}
}

FN_INTERNAL int fnusb_start_iso(fnusb_dev *dev, fnusb_isoc_stream *strm, fnusb_iso_cb cb, int ep, int xfers, int pkts, int len)
{
	freenect_context *ctx = dev->parent->parent;
	int ret, i;

	strm->parent = dev;
	strm->cb = cb;
	strm->num_xfers = xfers;
	strm->pkts = pkts;
	strm->len = len;
	strm->buffer = (uint8_t*)malloc(xfers * pkts * len);
	strm->xfers = (struct libusb_transfer**)malloc(sizeof(struct libusb_transfer*) * xfers);
	strm->dead = 0;
	strm->dead_xfers = 0;

	uint8_t *bufp = strm->buffer;

	for (i=0; i<xfers; i++) {
		FN_SPEW("Creating EP %02x transfer #%d\n", ep, i);
		strm->xfers[i] = libusb_alloc_transfer(pkts);

		libusb_fill_iso_transfer(strm->xfers[i], dev->dev, ep, bufp, pkts * len, pkts, iso_callback, strm, 0);

		libusb_set_iso_packet_lengths(strm->xfers[i], len);

		ret = libusb_submit_transfer(strm->xfers[i]);
		if (ret < 0) {
			FN_WARNING("Failed to submit isochronous transfer %d: %d\n", i, ret);
			strm->dead_xfers++;
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
	memset(strm, 0, sizeof(*strm));
	FN_FLOOD("fnusb_stop_iso() done\n");
	return 0;
}

FN_INTERNAL int fnusb_control(fnusb_dev *dev, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, uint8_t *data, uint16_t wLength)
{
	return libusb_control_transfer(dev->dev, bmRequestType, bRequest, wValue, wIndex, data, wLength, 0);
}

#ifdef BUILD_AUDIO
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
#endif
