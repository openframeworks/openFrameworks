#pragma once

extern "C"{
	typedef void (*html5audio_stream_callback)(int bufferSize, int inputChannels, int outputChannels, void * userData);

	extern int html5audio_context_create();
	extern int html5audio_context_start();
	extern int html5audio_context_stop();
	extern int html5audio_context_samplerate();
	extern void html5audio_context_spectrum(int bands, float * spectrum);

	extern int html5audio_sound_load(const char* url);
	extern void html5audio_sound_play(int sound, double offset);
	extern void html5audio_sound_stop(int sound);
	extern void html5audio_sound_pause(int sound);
	extern double html5audio_sound_rate(int sound);
	extern void html5audio_sound_set_rate(int sound, double rate);
	extern int html5audio_sound_done(int sound);
	extern double html5audio_sound_duration(int sound);
	extern double html5audio_sound_position(int sound);
	extern void html5audio_sound_set_loop(int sound, int loop);
	extern double html5audio_sound_gain(int sound);
	extern void html5audio_sound_set_gain(int sound, double volume);
	extern void html5audio_sound_free(int sound);

	extern int html5audio_stream_create(int bufferSize, int inputChannels, int outputChannels, float * inbuffer, float * outbuffer, html5audio_stream_callback callback, void * userData);
	extern int html5audio_stream_free();
	extern bool html5audio_sound_is_loaded(int sound);
}
