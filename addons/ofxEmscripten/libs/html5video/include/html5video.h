#pragma once

extern "C"{
    extern int html5video_player_create();
    extern void html5video_player_delete(int id);
    extern void html5video_player_load(int id,const char* src);
    extern const char* html5video_player_pixel_format(int it);
    extern void html5video_player_set_pixel_format(int it, const char* format);
    extern int html5video_player_update(int id, int update_pixels, unsigned char* pixels);
    extern void html5video_player_play(int id);
    extern void html5video_player_pause(int id);
    extern void html5video_player_stop(int id);
    extern int html5video_player_texture_id(int id);
    extern int html5video_player_width(int id);
    extern int html5video_player_height(int id);
    extern int html5video_player_is_paused(int id);
    extern int html5video_player_ready_state(int id);
    extern double html5video_player_duration(int id);
    extern double html5video_player_current_time(int id);
    extern void html5video_player_set_current_time(int id, double time);
    extern int html5video_player_ended(int id);
    extern void html5video_player_set_playback_rate(int id, double rate);
    extern double html5video_player_playback_rate(int id);
    extern double html5video_player_volume(int id);
    extern void html5video_player_set_volume(int id, double volume);
    extern double html5video_player_volume(int id);
    extern void html5video_player_set_loop(int id, int loop);
    extern int html5video_player_loop(int id);


    extern int html5video_grabber_create();
    extern void html5video_grabber_init(int id, int w, int h, int framerate=-1);
    extern const char* html5video_grabber_pixel_format(int it);
    extern void html5video_grabber_set_pixel_format(int it, const char* format);
    extern int html5video_grabber_update(int id, int update_pixels, unsigned char* pixels);
    extern int html5video_grabber_texture_id(int id);
    extern int html5video_grabber_width(int id);
    extern int html5video_grabber_height(int id);
    extern int html5video_grabber_ready_state(int id);
}
