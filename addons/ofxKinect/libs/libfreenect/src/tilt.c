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
#include <math.h>

#include "freenect_internal.h"

// The kinect can tilt from +31 to -31 degrees in what looks like 1 degree increments
// The control input looks like 2*desired_degrees
#define MAX_TILT_ANGLE 31
#define MIN_TILT_ANGLE (-31)

#define GRAVITY 9.80665

// Structs for 1473 and K4W communication
typedef struct {
	uint32_t magic;
	uint32_t tag;
	uint32_t arg1;
	uint32_t cmd;
	uint32_t arg2;
}fn_alt_motor_command;

typedef struct {
	uint32_t magic;
	uint32_t tag;
	uint32_t status;
}fn_alt_motor_reply;

static int tag_seq = 0;
static int tag_next_ack = 0;

int get_reply(libusb_device_handle* dev, freenect_context *ctx){
	unsigned char buffer[512];
	memset(buffer, 0, 512);
	int transferred = 0;
	int res = 0;
	res = libusb_bulk_transfer(dev, 0x81, buffer, 512, &transferred, 100);
	if (res != 0) {
		FN_ERROR("get_reply(): libusb_bulk_transfer failed: %d (transferred = %d)\n", res, transferred);
	} else if (transferred != 12) {
		FN_ERROR("get_reply(): weird - got %d bytes (expected 12)\n", transferred);
	} else {
		fn_alt_motor_reply reply;
		memcpy(&reply, buffer, sizeof(reply));
		if (reply.magic != 0x0a6fe000) {
			FN_ERROR("Bad magic: %08X (expected 0A6FE000\n", reply.magic);
			res = -1;
		}

        //can't really do this as tag_seq is static. with two cameras this is not going to match. could put in as part of libusb_device but I don't think it really matters.
//		if (reply.tag != tag_next_ack) {
//			FN_ERROR("Reply tag out of order: expected %d, got %d\n", tag_next_ack, reply.tag);
//			res = -1;
//		}

		if (reply.status != 0) {
			FN_ERROR("reply status != 0: failure?\n");
			res = -1;
		}
		
        tag_next_ack++;
	}
	return res;
}


freenect_raw_tilt_state* freenect_get_tilt_state(freenect_device *dev)
{
	return &dev->raw_state;
}

#ifdef BUILD_AUDIO
int update_tilt_state_alt(freenect_device *dev){
	freenect_context *ctx = dev->parent;

	int transferred = 0;
	int res = 0;
	fn_alt_motor_command cmd;
	cmd.magic = fn_le32(0x06022009);
	cmd.tag = fn_le32(tag_seq++);
	cmd.arg1 = fn_le32(0x68); // 104.  Incidentally, the number of bytes that we expect in the reply.
	cmd.cmd = fn_le32(0x8032);
    
	unsigned char buffer[256];
	memcpy(buffer, &cmd, 16);
    
	res = libusb_bulk_transfer(dev->usb_audio.dev, 0x01, buffer, 16, &transferred, 250);
	if (res != 0) {
		return res;
	}
    
	res = libusb_bulk_transfer(dev->usb_audio.dev, 0x81, buffer, 256, &transferred, 250); // 104 bytes
	if (res != 0) {
		return res;
	} else {
//		int i;
//		for(i = 0 ; i < transferred ; i += 4) {
//			int32_t j;
//			memcpy(&j, buffer + i, 4);
//			printf("\t%d\n", j);
//		}
//		printf("\n");
		struct {
			int32_t x;
			int32_t y;
			int32_t z;
            int32_t tilt;
		} accel_and_tilt;
        
		memcpy(&accel_and_tilt, buffer + 16, sizeof(accel_and_tilt));
		//printf("\tX: %d  Y: %d  Z:%d - tilt is %d\n", accel_and_tilt.x, accel_and_tilt.y, accel_and_tilt.z, accel_and_tilt.tilt);
        
    	dev->raw_state.accelerometer_x  = (int16_t)accel_and_tilt.x;
        dev->raw_state.accelerometer_y  = (int16_t)accel_and_tilt.y;
        dev->raw_state.accelerometer_z  = (int16_t)accel_and_tilt.z;
        
        //this is multiplied by 2 as the older 1414 device reports angles doubled and freenect takes this into account
        dev->raw_state.tilt_angle       = (int8_t)accel_and_tilt.tilt * 2;

	}
	// Reply: skip four uint32_t, then you have three int32_t that give you acceleration in that direction, it seems.
	// Units still to be worked out.
	return get_reply(dev->usb_audio.dev, ctx);
}
#endif 

int freenect_update_tilt_state(freenect_device *dev)
{
	freenect_context *ctx = dev->parent;
	
    #ifdef BUILD_AUDIO
    //if we have motor control via audio and fw is uploaded - call the alt function
    if( dev->motor_control_with_audio_enabled ){
        return update_tilt_state_alt(dev);
    }
    #endif
    
    if(!(ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR))
		return 0;
    
    
	uint8_t buf[10];
	uint16_t ux, uy, uz;
	int ret = fnusb_control(&dev->usb_motor, 0xC0, 0x32, 0x0, 0x0, buf, 10);
	if (ret != 10) {
		FN_ERROR("Error in accelerometer reading, libusb_control_transfer returned %d\n", ret);
		return ret < 0 ? ret : -1;
	}

	ux = ((uint16_t)buf[2] << 8) | buf[3];
	uy = ((uint16_t)buf[4] << 8) | buf[5];
	uz = ((uint16_t)buf[6] << 8) | buf[7];

	dev->raw_state.accelerometer_x = (int16_t)ux;
	dev->raw_state.accelerometer_y = (int16_t)uy;
	dev->raw_state.accelerometer_z = (int16_t)uz;
	dev->raw_state.tilt_angle = (int8_t)buf[8];
	dev->raw_state.tilt_status = (freenect_tilt_status_code)buf[9];

	return ret;
}

#ifdef BUILD_AUDIO
int freenect_set_tilt_degs_alt(freenect_device *dev, int tilt_degrees)
{
	freenect_context *ctx = dev->parent;

	if (tilt_degrees > 31 || tilt_degrees < -31) {
		FN_WARNING("set_tilt(): degrees %d out of safe range [-31, 31]\n", tilt_degrees);
		return -1;
	}
    
	fn_alt_motor_command cmd;
	cmd.magic = fn_le32(0x06022009);
	cmd.tag = fn_le32(tag_seq++);
	cmd.arg1 = fn_le32(0);
	cmd.cmd = fn_le32(0x803b);
	cmd.arg2 = (uint32_t)(fn_le32((int32_t)tilt_degrees));
	int transferred = 0;
	int res = 0;
	unsigned char buffer[20];
	memcpy(buffer, &cmd, 20);

	res = libusb_bulk_transfer(dev->usb_audio.dev, 0x01, buffer, 20, &transferred, 250);
	if (res != 0) {
		FN_ERROR("freenect_set_tilt_alt(): libusb_bulk_transfer failed: %d (transferred = %d)\n", res, transferred);
		return res;
	}
    
	return get_reply(dev->usb_audio.dev, ctx);
}
#endif

int freenect_set_tilt_degs(freenect_device *dev, double angle)
{
	freenect_context *ctx = dev->parent;
    
    #ifdef BUILD_AUDIO
    //if we have motor control via audio and fw is uploaded - call the alt function
    if( dev->motor_control_with_audio_enabled ){
        return freenect_set_tilt_degs_alt(dev, angle);
    }
    #endif
    
	if(!(ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR))
		return 0;
    
	int ret;
	uint8_t empty[0x1];

	angle = (angle<MIN_TILT_ANGLE) ? MIN_TILT_ANGLE : ((angle>MAX_TILT_ANGLE) ? MAX_TILT_ANGLE : angle);
	angle = angle * 2;

	ret = fnusb_control(&dev->usb_motor, 0x40, 0x31, (uint16_t)angle, 0x0, empty, 0x0);
	return ret;
}


FN_INTERNAL int fnusb_set_led_alt(libusb_device_handle * dev, freenect_context * ctx, freenect_led_options state)
{
    typedef enum {
        LED_ALT_OFF = 1,
        LED_ALT_BLINK_GREEN = 2,
        LED_ALT_SOLID_GREEN = 3,
        LED_ALT_SOLID_RED = 4,
    }led_alt_state;

	int transferred = 0;
	int res = 0;
    
    //The LED states are different between K4W/1473 and older 1414 
    if( state == LED_GREEN ){
        state = (freenect_led_options)LED_ALT_SOLID_GREEN;
    }else if( state == LED_RED ){
        state = (freenect_led_options)LED_ALT_SOLID_RED;
    }else if( state == LED_YELLOW ){
        state = (freenect_led_options)LED_ALT_SOLID_GREEN;
    }else if( state == LED_OFF ){
        state = (freenect_led_options)LED_ALT_OFF;
    }else if( state == LED_BLINK_GREEN ){
        state = (freenect_led_options)LED_ALT_BLINK_GREEN;
    }else{
        state = LED_GREEN;
    }
    
	fn_alt_motor_command cmd;
	cmd.magic = fn_le32(0x06022009);
	cmd.tag = fn_le32(tag_seq++);
	cmd.arg1 = fn_le32(0);
	cmd.cmd = fn_le32(0x10);
	cmd.arg2 = (uint32_t)(fn_le32((int32_t)state));
    
	unsigned char buffer[20];
	memcpy(buffer, &cmd, 20);
    
	res = libusb_bulk_transfer(dev, 0x01, buffer, 20, &transferred, 100);
	if (res != 0) {
		FN_WARNING("fnusb_set_led_alt(): libusb_bulk_transfer failed: %d (transferred = %d)\n", res, transferred);
		return res;
	}
	return get_reply(dev, ctx);
}

#ifdef BUILD_AUDIO
int freenect_set_led_alt(freenect_device *dev, freenect_led_options state)
{
	freenect_context *ctx = dev->parent;
    return fnusb_set_led_alt(dev->usb_audio.dev, ctx, state);
}
#endif

int freenect_set_led(freenect_device *dev, freenect_led_options option)
{
	freenect_context *ctx = dev->parent;

    //if we have motor control via audio and fw is uploaded - call the alt function
    #ifdef BUILD_AUDIO
    if( dev->motor_control_with_audio_enabled ){
        return freenect_set_led_alt(dev, option);
    }
    #endif
    
	if(!(ctx->enabled_subdevices & FREENECT_DEVICE_MOTOR))
		return 0;
    
	int ret;
	uint8_t empty[0x1];
	ret = fnusb_control(&dev->usb_motor, 0x40, 0x06, (uint16_t)option, 0x0, empty, 0x0);
	return ret;
}

double freenect_get_tilt_degs(freenect_raw_tilt_state *state)
{
	return ((double)state->tilt_angle) / 2.;
}

freenect_tilt_status_code freenect_get_tilt_status(freenect_raw_tilt_state *state)
{
	return state->tilt_status;
}

void freenect_get_mks_accel(freenect_raw_tilt_state *state, double* x, double* y, double* z)
{
	//the documentation for the accelerometer (http://www.kionix.com/Product%20Sheets/KXSD9%20Product%20Brief.pdf)
	//states there are 819 counts/g
	*x = (double)state->accelerometer_x/FREENECT_COUNTS_PER_G*GRAVITY;
	*y = (double)state->accelerometer_y/FREENECT_COUNTS_PER_G*GRAVITY;
	*z = (double)state->accelerometer_z/FREENECT_COUNTS_PER_G*GRAVITY;
}
