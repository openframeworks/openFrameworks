/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2011 individual OpenKinect contributors. See the CONTRIB file
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
#include "libfreenect.h"
#include "libfreenect-audio.h"
#include "freenect_internal.h"

#include <string.h>
#include <stdlib.h>

static void prepare_iso_out_data(freenect_device* dev, uint8_t* buffer) {
	audio_stream* stream = &dev->audio;
	if (dev->audio_out_cb) {
		// Then pull data from the ring buffer, calling the callback as needed to refill the buffer
	} else {
		memset(buffer, 0, 76);
	}
	((uint16_t*)buffer)[0] = stream->out_window;
	buffer[2] = stream->out_seq;
	if (stream->out_window_parity == 0) {
		if (stream->out_counter_within_window < 4) {
			// madness type 1 - high nibble of buffer[3] should be the seq_in_window-th nibble of timestamp
			buffer[3] = (((stream->out_weird_timestamp >> (stream->out_counter_within_window*4)) & 0x000f) << 4) | 0x05; // I have no idea why we do this.
		} else if (stream->out_counter_within_window < 8) {
			// madness type 2 - same thing mod 4, but we add 23 to weird_timestamp for no clear reason.
			buffer[3] = ((((stream->out_weird_timestamp+23) >> ((stream->out_counter_within_window-4)*4)) & 0x000f) << 4) | 0x05; // I have even less idea why we do this.
		} else {
			buffer[3] = 0x01; // Oh, and then this.
		}
	} else {
		if (stream->out_counter_within_window < 4) {
			// madness type 1
			buffer[3] = (((stream->out_weird_timestamp >> (stream->out_counter_within_window*4)) & 0x000f) << 4) | 0x05; // I have no idea why we do this.
		} else {
			buffer[3] = 0x01;
		}
	}
	// Now, update the values for their next usage:
	stream->out_seq++;
	stream->out_counter_within_window++;
	stream->out_weird_timestamp += (stream->out_window_parity == 1) ? 6 : 5;
	switch(stream->out_seq) {
		case 0x80:
			stream->out_seq = 0;
			// TODO: if we're at the beginning of a new outgoing window, save a timestamp
		case 0x2b:
		case 0x56:
			stream->out_counter_within_window = 0;
			stream->out_window++;
			stream->out_window_parity++;
		default:
			break;
	}
	if (stream->out_window_parity == 3) stream->out_window_parity = 0;
	return;
}

static void iso_out_callback(freenect_device *dev, uint8_t *pkt, int len) {
	prepare_iso_out_data(dev, pkt);
}

static void iso_in_callback(freenect_device *dev, uint8_t *pkt, int len) {
	freenect_context *ctx = dev->parent;
	if (len == 524) { // Cool, this is audio data
		audio_in_block* block = (audio_in_block*)pkt;
		if (block->magic != 0x80000080) {
			FN_ERROR("audio: invalid magic in iso IN packet: %08X\n", block->magic);
			return;
		}
		if (block->window != dev->audio.in_window) {
			FN_SPEW("audio: IN window changed: was %04X now %04X\n", dev->audio.in_window, block->window);
			if (dev->audio_in_cb) {
				dev->audio_in_cb(dev, 256, dev->audio.mic_buffer[0], dev->audio.mic_buffer[1],
				                           dev->audio.mic_buffer[2], dev->audio.mic_buffer[3],
				                           dev->audio.cancelled_buffer, dev->audio.in_unknown);
			}
			int t;
			for(t = 0; t < 10; t++) {
				if (dev->audio.last_seen_window[t] != dev->audio.in_window) {
					FN_SPEW("audio: did not receive data for channel 0x%02x\n", t+1);
				}
			}
			if (block->window - dev->audio.in_window > 3) {
				FN_SPEW("audio: packet loss, dropped %d windows\n", (block->window - dev->audio.in_window - 3) / 3);
			}
			dev->audio.in_window = block->window;
		}
		switch(block->channel) {
			case 1:
				memcpy(dev->audio.cancelled_buffer, &block->samples, 512);
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				if (block->channel & 1) {
					memcpy(&(dev->audio.mic_buffer[(block->channel-2) / 2][128]), &block->samples, 512);
				} else {
					memcpy(dev->audio.mic_buffer[(block->channel-2) / 2], &block->samples, 512);
				}
				break;
			default:
				FN_ERROR("audio: invalid channel in iso IN packet: %d\n", block->channel);
				break;
		}
		dev->audio.last_seen_window[block->channel-1] = block->window;

	} else if (len == 60) { // Then this is the uninterpreted signalling information

	} else if (len != 0) {
		FN_ERROR("audio: received an iso IN packet of strange length: %d\n", len);
	}
}

void freenect_set_audio_in_callback(freenect_device *dev, freenect_audio_in_cb callback) {
	dev->audio_in_cb = callback;
}
void freenect_set_audio_out_callback(freenect_device *dev, freenect_audio_out_cb callback) {
	dev->audio_out_cb = callback;
}

int freenect_start_audio(freenect_device* dev) {
	freenect_context *ctx = dev->parent;
	int res;

	if (dev->audio.running)
		return -1;

	// Allocate buffers
	dev->audio.audio_out_ring = (freenect_sample_51*)malloc(256 * sizeof(freenect_sample_51));
	memset(dev->audio.audio_out_ring, 0, 256 * sizeof(freenect_sample_51));
	dev->audio.cancelled_buffer = (int16_t*)malloc(256*sizeof(int16_t));
	memset(dev->audio.cancelled_buffer, 0, 256*sizeof(int16_t));
	int i;
	for(i = 0; i < 4; i++) {
		dev->audio.mic_buffer[i] = (int32_t*)malloc(256*sizeof(int32_t));
		memset(dev->audio.mic_buffer[i], 0, 256*sizeof(int32_t));
	}
	dev->audio.in_unknown = malloc(48); // amount of data in small transfer

	// Set initial parameter values
	dev->audio.ring_reader_idx = 0;
	dev->audio.ring_writer_idx = 0;
	dev->audio.out_window = 0;
	dev->audio.out_seq = 0;
	dev->audio.out_counter_within_window = 0;
	dev->audio.out_weird_timestamp = 0;
	dev->audio.out_window_parity = 0;
	dev->audio.in_window = 0;
	dev->audio.in_counter = 0;
	for(i = 0; i < 10; i++) {
		dev->audio.last_seen_window[i] = 0;
	}

	// Start isochronous streams
	res = fnusb_start_iso(&dev->usb_audio, &dev->audio_in_isoc, iso_in_callback, 0x82, NUM_XFERS, PKTS_PER_XFER, 524);
	if (res < 0) {
		FN_ERROR("audio: failed to start isochronous IN stream: %d\n", res);
		return res;
	}
	res = fnusb_start_iso(&dev->usb_audio, &dev->audio_out_isoc, iso_out_callback, 0x02, NUM_XFERS, PKTS_PER_XFER, 76);
	if (res < 0) {
		FN_ERROR("audio: failed to start isochronous OUT stream: %d\n", res);
		return res;
	}

	dev->audio.running = 1;
	return 0;
}

int freenect_stop_audio(freenect_device* dev) {
	freenect_context *ctx = dev->parent;
	int res;
	int ret = 0;

	if (!dev->audio.running)
		return -1;

	// Stop isochronous transfers
	// Note: I'm not sure how to make this work if there's partial failure.
	dev->audio.running = 0;
	res = fnusb_stop_iso(&dev->usb_audio, &dev->audio_in_isoc);
	if (res < 0) {
		FN_ERROR("audio: failed to stop isochronous IN stream: %d\n", res);
		return res;
	}
	res = fnusb_stop_iso(&dev->usb_audio, &dev->audio_out_isoc);
	if (res < 0) {
		FN_ERROR("audio: failed to stop isochronous OUT stream: %d\n", res);
		return res;
	}

	// Free buffers
	if (dev->audio.audio_out_ring)
		free(dev->audio.audio_out_ring);
	if (dev->audio.cancelled_buffer)
		free(dev->audio.cancelled_buffer);
	if (dev->audio.in_unknown)
		free(dev->audio.in_unknown);
	
	int i;
	for (i = 0; i < 4; i++) {
		if (dev->audio.mic_buffer[i])
			free(dev->audio.mic_buffer[i]);
		dev->audio.mic_buffer[i] = NULL;
	}

	dev->audio.audio_out_ring = NULL;
	dev->audio.cancelled_buffer = NULL;
	dev->audio.in_unknown = NULL;

	return ret;
}

