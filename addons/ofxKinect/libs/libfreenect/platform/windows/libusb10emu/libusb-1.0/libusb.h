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

#pragma once

// This interface emulator requires the libusb-win32 v1.2.2.1 (snapshot)
// or later. Prior win32 versions of the library were not conformal to
// the official libusb-0.1 API naming convention
// (e.g.: usb_device_t instead of usb_device)
// One can either workaround prior releases to match the official naming
// or upgrade to the libusb-win32 1.2.2.1 snapshot.

#include <unistd.h>

#if defined(_MSC_VER)
  typedef unsigned __int8   uint8_t;
  typedef unsigned __int16  uint16_t;
  // We need struct timeval.
  #include <winsock.h>
#else
  #include <stdint.h>
  #include <sys/time.h>
#endif

#define LIBUSBEMU 1

// guard to enable mix of compiler semantics (C/C++ calling C++);
// this should allow builds under VC++ and gcc/g++ profiles
#ifdef __cplusplus
  extern "C" {
#endif

// gotta use "_t" suffix here...
struct libusb_context_t;
// ...otherwise C++ complains with such typedef declaration
typedef struct libusb_context_t libusb_context;
int libusb_init(libusb_context** context);
void libusb_exit(libusb_context* ctx);
void libusb_set_debug(libusb_context *ctx, int level);

struct libusb_device_t;
typedef struct libusb_device_t libusb_device;
ssize_t libusb_get_device_list(libusb_context* ctx, libusb_device*** list);
void libusb_free_device_list(libusb_device** list, int unref_devices);

int libusb_get_device_descriptor(libusb_device* dev, struct libusb_device_descriptor*	desc);

struct libusb_device_handle_t;
typedef struct libusb_device_handle_t libusb_device_handle;
int libusb_open(libusb_device* dev, libusb_device_handle** handle);
void libusb_close(libusb_device_handle*	dev_handle);

int libusb_get_string_descriptor(libusb_device_handle *dev_handle, uint8_t desc_index, uint16_t langid, unsigned char *data, int length);
int libusb_get_string_descriptor_ascii(libusb_device_handle *dev_handle, uint8_t desc_index, unsigned char *data, int length);

int libusb_set_configuration(libusb_device_handle *dev, int configuration);
int libusb_set_interface_alt_setting(libusb_device_handle *dev,int interface_number,int alternate_setting);	
int libusb_claim_interface(libusb_device_handle* dev, int interface_number);
int libusb_release_interface(libusb_device_handle* dev, int interface_number);

libusb_device_handle *libusb_open_device_with_vid_pid(libusb_context *ctx,
	uint16_t vendor_id, uint16_t product_id);

int libusb_control_transfer(libusb_device_handle* dev_handle, uint8_t bmRequestType, uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char* data, uint16_t wLength, unsigned int timeout);
int libusb_bulk_transfer(libusb_device_handle *dev_handle, unsigned char endpoint, unsigned char *data, int length, int *actual_length, unsigned int timeout);


struct libusb_transfer* libusb_alloc_transfer(int iso_packets);
void libusb_free_transfer(struct libusb_transfer* transfer);

typedef void(*libusb_transfer_cb_fn)(struct libusb_transfer *transfer);
void libusb_fill_iso_transfer(struct libusb_transfer* transfer, libusb_device_handle* dev_handle, unsigned char endpoint, unsigned char* buffer, int length, int num_iso_packets, libusb_transfer_cb_fn callback, void* user_data, unsigned int timeout);
void libusb_set_iso_packet_lengths(struct libusb_transfer* transfer, unsigned int length);

int libusb_submit_transfer(struct libusb_transfer* transfer);
int libusb_cancel_transfer(struct libusb_transfer* transfer);

int libusb_handle_events(libusb_context* ctx);  // WORK IN PROGRESS...
int libusb_handle_events_timeout(libusb_context* ctx, struct timeval* timeout);

// the signature of libusb_device_descriptor is identical to usb_device_descriptor
// which means that the below struct could be replaced by a typedef; however, that
// would require #including <usb.h> in this header, polluting the scope and possibly
// creating conflicts with existing <usb.h> declarations...
struct libusb_device_descriptor
{
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
};

enum libusb_endpoint_direction {
	LIBUSB_ENDPOINT_IN = 0x80,
	LIBUSB_ENDPOINT_OUT = 0x00
};

enum libusb_transfer_status
{
	LIBUSB_TRANSFER_COMPLETED,
	LIBUSB_TRANSFER_ERROR,
	LIBUSB_TRANSFER_TIMED_OUT,
	LIBUSB_TRANSFER_CANCELLED,
	LIBUSB_TRANSFER_STALL,
	LIBUSB_TRANSFER_NO_DEVICE,
	LIBUSB_TRANSFER_OVERFLOW
};

enum libusb_transfer_type
{
	LIBUSB_TRANSFER_TYPE_CONTROL = 0,
	LIBUSB_TRANSFER_TYPE_ISOCHRONOUS = 1,
	LIBUSB_TRANSFER_TYPE_BULK = 2,
	LIBUSB_TRANSFER_TYPE_INTERRUPT = 3,
};

enum libusb_transfer_flags {
	LIBUSB_TRANSFER_SHORT_NOT_OK = 1<<0,
	LIBUSB_TRANSFER_FREE_BUFFER = 1<<1,
	LIBUSB_TRANSFER_FREE_TRANSFER = 1<<2,
};

struct libusb_transfer
{
	libusb_device_handle *dev_handle;
	uint8_t flags;
	unsigned char endpoint;
	unsigned char type;
	unsigned int timeout;
	enum libusb_transfer_status status;
	int length;
	int actual_length;
	libusb_transfer_cb_fn callback;
	void *user_data;
	unsigned char *buffer;
	int num_iso_packets;
	struct libusb_iso_packet_descriptor* iso_packet_desc;
};

struct libusb_iso_packet_descriptor
{
	unsigned int length;
	unsigned int actual_length;
	enum libusb_transfer_status status;
};

enum libusb_error
{
	LIBUSB_SUCCESS = 0,
	LIBUSB_ERROR_IO = -1,
	LIBUSB_ERROR_INVALID_PARAM = -2,
	LIBUSB_ERROR_ACCESS = -3,
	LIBUSB_ERROR_NO_DEVICE = -4,
	LIBUSB_ERROR_NOT_FOUND = -5,
	LIBUSB_ERROR_BUSY = -6,
	LIBUSB_ERROR_TIMEOUT = -7,
	LIBUSB_ERROR_OVERFLOW = -8,
	LIBUSB_ERROR_PIPE = -9,
	LIBUSB_ERROR_INTERRUPTED = -10,
	LIBUSB_ERROR_NO_MEM = -11,
	LIBUSB_ERROR_NOT_SUPPORTED = -12,
	LIBUSB_ERROR_OTHER = -99,
};

#ifdef __cplusplus
  }
#endif
