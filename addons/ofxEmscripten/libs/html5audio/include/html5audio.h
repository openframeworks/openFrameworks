#pragma once

extern "C"{
	typedef void (*html5audio_stream_callback)(int bufferSize, int inputChannels, int outputChannels, void * userData);

	extern void html5audio_list_devices();
	extern int html5audio_context_create();
	extern int html5audio_context_start();
	extern int html5audio_context_stop();
	extern int html5audio_context_samplerate();
	extern void html5audio_context_spectrum(int bands, float * spectrum);

	extern int html5audio_player_create();
	extern void html5audio_sound_load(int sound_id, const char* url);
	extern void html5audio_sound_play(int sound, bool multiplay, double volume, double speed, double pan, double offset);
	extern void html5audio_sound_stop(int sound);
	extern void html5audio_sound_pause(int sound);
	extern double html5audio_sound_rate(int sound);
	extern void html5audio_sound_set_rate(int sound, double rate);
	extern int html5audio_sound_done(int sound);
	extern double html5audio_sound_duration(int sound);
	extern void html5audio_sound_set_position(int sound, float position);
	extern double html5audio_sound_position(int sound);
	extern void html5audio_sound_set_loop(int sound, int loop);
	extern double html5audio_sound_volume(int sound);
	extern void html5audio_sound_set_volume(int sound, double volume);
	extern double html5audio_sound_pan(int sound);
	extern void html5audio_sound_set_pan(int sound, double pan);
	extern void html5audio_sound_free(int sound);

	extern void html5audio_stream_create(int bufferSize, int inputChannels, int outputChannels, float * inbuffer, float * outbuffer, html5audio_stream_callback callback, void * userData);
	extern void html5audio_stream_free();
	extern bool html5audio_sound_is_loaded(int sound);
}
