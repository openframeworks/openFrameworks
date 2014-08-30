//
//  keep_alive.c
//  Created by Theodore Watson on 1/23/14.

//This file is part of the OpenKinect Project. http://www.openkinect.org
// 
// Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file 
// for details.
// 
// This code is licensed to you under the terms of the Apache License, version 
// 2.0, or, at your option, the terms of the GNU General Public License, 
// version 2.0. See the APACHE20 and GPL20 files for the text of the licenses, 
// or the following URLs:
// http://www.apache.org/licenses/LICENSE-2.0
// http://www.gnu.org/licenses/gpl-2.0.txt
// 
// If you redistribute this file in source form, modified or unmodified, 
// you may:
// 1) Leave this header intact and distribute it under the same terms, 
// accompanying it with the APACHE20 and GPL20 files, or
// 2) Delete the Apache 2.0 clause and accompany it with the GPL20 file, or
// 3) Delete the GPL v2.0 clause and accompany it with the APACHE20 file
// In all cases you must keep the copyright notice intact and include a copy 
// of the CONTRIB file.
// Binary distributions must follow the binary distribution requirements of 
// either License.


//Based on code provided by Drew Fisher

/*
 * Copyright 2011 Drew Fisher <drew.m.fisher@gmail.com>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS  ''AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL DREW FISHER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Drew Fisher.
 */
#include "keep_alive.h"
#include "freenect_internal.h"

#include <libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h> // For usleep()

#define le32(X) (X)
#define LOG(...) fprintf(stderr, __VA_ARGS__)

static uint32_t tag_seq = 1;
static uint32_t tag_next_ack = 1;

typedef struct {
	uint32_t magic;
	uint32_t tag;
	uint32_t status;
} motor_reply;

typedef struct {
	uint32_t magic;
	uint32_t tag;
	uint32_t arg1;
	uint32_t cmd;
	uint32_t arg2;
} motor_command;

static int get_reply(libusb_device_handle* dev){
	unsigned char buffer[512];
	memset(buffer, 0, 512);
	int transferred = 0;
	int res = 0;
	res = libusb_bulk_transfer(dev, 0x81, buffer, 512, &transferred, 0);
	if (res != 0) {
		LOG("freenect_extra_keep_alive get_reply(): libusb_bulk_transfer failed: %d (transferred = %d)\n", res, transferred);
	} else if (transferred != 12) {
		LOG("freenect_extra_keep_alive get_reply(): weird - got %d bytes (expected 12)\n", transferred);
	} else {
		motor_reply reply;
		memcpy(&reply, buffer, sizeof(reply));
		if (reply.magic != 0x0a6fe000) {
			LOG("freenect_extra_keep_alive Bad magic: %08X (expected 0A6FE000\n", reply.magic);
			res = -1;
		}
		if (reply.tag != tag_next_ack) {
			LOG("freenect_extra_keep_alive Reply tag out of order: expected %d, got %d\n", tag_next_ack, reply.tag);
			res = -1;
		}
		if (reply.status != 0) {
			LOG("freenect_extra_keep_alive reply status != 0: failure?\n");
			res = -1;
		}
		tag_next_ack++;
//		LOG("freenect_extra_keep_alive get_reply(): got %d bytes:", transferred);
//		int i;
//		for (i = 0; i < transferred; i++) {
//			LOG(" %02X", buffer[i]);
//		}
//		LOG("\n");
	}
	return res;
}

static int set_led(libusb_device_handle* dev, int state) {
	int transferred = 0;
	int res = 0;
	motor_command cmd;
	cmd.magic = le32(0x06022009);
	cmd.tag = le32(tag_seq++);
	cmd.arg1 = le32(0);
	cmd.cmd = le32(0x10);
	cmd.arg2 = (uint32_t)(le32((int32_t)state));
	unsigned char buffer[20];
	memcpy(buffer, &cmd, 20);
	// Send command to set LED to solid green
//	LOG("About to send bulk transfer:");
//	int i;
//	for(i = 0; i < 20 ; i++) {
//		LOG(" %02X", buffer[i]);
//	}
//	LOG("\n");
	res = libusb_bulk_transfer(dev, 0x01, buffer, 20, &transferred, 0);
	if (res != 0) {
		LOG("freenect_extra_keep_alive set_led(): libusb_bulk_transfer failed: %d (transferred = %d)\n", res, transferred);
		return res;
	}
	return get_reply(dev);
}

//this is for K4W or 1473 devices - pass in the PID of the audio device that needs the LED set.
void freenect_extra_keep_alive(int pid){

	int res;
	int state_to_set = 3;

	libusb_context* ctx = NULL;
	libusb_init(&ctx);

	libusb_device_handle* dev = NULL;

    //check the default audio device
    dev = libusb_open_device_with_vid_pid(ctx, 0x045e, pid);
    
    //K4W only:
    //if the firmware is uploaded the device could have a two different PIDs based on which firmware was uploaded.
    //so we have to check for both
    //note: it might be better if we pass in the PID of the camera and then find the audio device that is in the same usb tree/hub - might be more reliable when multiple devices are plugged in
    if( dev == NULL && pid == PID_K4W_AUDIO ){
        pid = PID_K4W_AUDIO_ALT_1;
        dev = libusb_open_device_with_vid_pid(ctx, 0x045e, pid);
    }
    if( dev == NULL && pid == PID_K4W_AUDIO_ALT_1 ){
        pid = PID_K4W_AUDIO_ALT_2;
	dev = libusb_open_device_with_vid_pid(ctx, 0x045e, pid);
    }
        
    if(dev == NULL) {
		LOG("freenect extra keepAlive: Failed to open audio device\n");
		libusb_exit(ctx);
        return;
	}

	res = libusb_claim_interface(dev, 0);
	if (res != 0) {
		LOG("freenect extra keepAlive: Failed to claim interface 1: %d\n", res);
        libusb_close(dev);
        libusb_exit(ctx);
        return;
	}

	res = set_led(dev, state_to_set);
	if (res != 0) {
		LOG("freenect extra keepAlive: set_led failed\n");
        libusb_close(dev);
        libusb_exit(ctx);
        return;
	}

    libusb_close(dev);
    libusb_exit(ctx);
}
