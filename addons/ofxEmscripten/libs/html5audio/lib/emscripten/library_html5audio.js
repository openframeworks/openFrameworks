var LibraryHTML5Audio = {
    $AUDIO: {
        player: [],
        lastPlayerID: 0,
    },

    html5audio_list_devices: function () {
        if (!navigator.mediaDevices.enumerateDevices) {
            console.log("enumerateDevices() not supported.");
        } else {
            // List cameras and microphones.
            navigator.mediaDevices.enumerateDevices()
            .then((devices) => {
                devices.forEach((device) => {
                    if(device.kind == "audioinput"){
                        console.log(`${device.kind}: ${device.label} id = ${device.deviceId}`);
                    }
                });
                return devices;
            })
            .catch((err) => {
                console.error(`${err.name}: ${err.message}`);
            });
        }
    },

    html5audio_context_create: function () {
       try {
            // Fix up for prefixing
            window.AudioContext = window.AudioContext || window.webkitAudioContext;
            var context = new AudioContext({});

            // Fix issue with chrome autoplay policy
            document.addEventListener('mousedown', function cb(event) {
		context.resume();
		event.currentTarget.removeEventListener(event.type, cb);
            });

            AUDIO.context = context;
            
            var fft = context.createAnalyser();
            fft.smoothingTimeConstant = 0;
            fft.connect(AUDIO.context.destination);
            fft.maxDecibels = 0;
            fft.minDecibels = -100;
            AUDIO.fft = fft;
            return 0;
        } catch (e) {
            console.log('Web Audio API is not supported in this browser', e);
            return -1;
        }
    },

    html5audio_context_start: function () {
        AUDIO.context.resume();
    },

    html5audio_context_stop: function () {
        AUDIO.context.suspend();
    },

    html5audio_context_spectrum: function (bands, spectrum) {
        AUDIO.fft.fftSize = bands * 2;
        var spectrumArray = Module.HEAPF32.subarray(spectrum >> 2, (spectrum >> 2) + bands);
        AUDIO.fft.getFloatFrequencyData(spectrumArray);
    },

    html5audio_context_samplerate: function () {
        return AUDIO.context.sampleRate.value;
    },

   html5audio_player_create: function () {
	var audio = document.createElement('audio');
	var player_id = AUDIO.lastPlayerID++;
	AUDIO.player[player_id] = audio;
	var source = AUDIO.context.createMediaElementSource(AUDIO.player[player_id]); 
	AUDIO.player[player_id].soundPan = AUDIO.context.createStereoPanner();
	source.connect(AUDIO.player[player_id].soundPan).connect(AUDIO.fft);
	return player_id;
    },
    
    html5audio_sound_load: function (player_id, url) {
        AUDIO.player[player_id].src = UTF8ToString(url);
    },

    html5audio_sound_play: function (player_id, multiplay, volume, speed, pan, offset) {
        if (AUDIO.player[player_id].src != "") {
        	if (multiplay) {
        		const clone = AUDIO.player[player_id].cloneNode();
			clone.soundPan = AUDIO.context.createStereoPanner();
			clone.volume = volume;
        		clone.playbackRate = speed;
        		clone.soundPan.pan.value = pan;
        		AUDIO.player[player_id] = clone;
        	}
            	AUDIO.player[player_id].play(offset);
	}
    },

    html5audio_sound_stop: function (player_id) {
        AUDIO.player[player_id].currentTime = 0;
        AUDIO.player[player_id].pause();
    },

    html5audio_sound_pause: function (player_id) {
        AUDIO.player[player_id].pause();
    },

    html5audio_sound_rate: function (player_id) {
	return AUDIO.player[player_id].playbackRate;
    },

    html5audio_sound_set_rate: function (player_id, rate) {
	AUDIO.player[player_id].playbackRate = rate;
    },

    html5audio_sound_done: function (player_id) {
	return AUDIO.player[player_id].done;
    },

    html5audio_sound_duration: function (player_id) {
        if (AUDIO.player[player_id].src != "") {
	    return AUDIO.player[player_id].duration;
	} else {
	    return 0;
	}
    },

    html5audio_sound_position: function (player_id) {
        if (AUDIO.player[player_id].src != "") {
            return AUDIO.player[player_id].currentTime;
	} else {
	    return 0;
	}
    },
    
    html5audio_sound_set_position: function (player_id, position) {
        if (AUDIO.player[player_id].src != "") {
            AUDIO.player[player_id].currentTime = position * AUDIO.player[player_id].duration;
        }
    },

    html5audio_sound_set_loop: function (player_id, loop) {
        AUDIO.player[player_id].loop = true;
    },

    html5audio_sound_set_volume: function (player_id, volume) {
        AUDIO.player[player_id].volume = volume;
    },

    html5audio_sound_volume: function (player_id) {
        return AUDIO.player[player_id].volume;
    },

    html5audio_sound_set_pan: function (player_id, pan) {
    	AUDIO.player[player_id].soundPan.pan.value = pan;
    },

    html5audio_sound_pan: function (player_id) {
        return AUDIO.player[player_id].soundPan.pan.value;
    },
    
    html5audio_sound_free: function (player_id) {
	if (AUDIO.player[player_id].src != "") {
            AUDIO.player[player_id].pause();
            URL.revokeObjectURL(AUDIO.player[player_id].src);
	}
    },

    html5audio_stream_create: function(bufferSize, inputChannels, outputChannels, inbuffer, outbuffer, callback, userData) {
        var stream = AUDIO.context.createScriptProcessor(bufferSize, inputChannels, outputChannels);
        var inbufferArray = Module.HEAPF32.subarray(inbuffer >> 2,(inbuffer>>2) + bufferSize * inputChannels);
        var outbufferArray = Module.HEAPF32.subarray(outbuffer >> 2, (outbuffer>>2) + bufferSize * outputChannels);

        stream.onaudioprocess = function(event) {
            var i, j, c;
            if (inputChannels > 0) {
                for (c = 0; c < inputChannels; ++c) {
                    var inChannel = event.inputBuffer.getChannelData(c);
                    for (i = 0, j = c; i < bufferSize; ++i, j += inputChannels) {
                        inbufferArray[j] = inChannel[i];
                    }
                }
            }

            {{{ makeDynCall('viiii', 'callback') }}}(bufferSize, inputChannels, outputChannels, userData);

            if (outputChannels > 0) {
                for (c = 0; c < outputChannels; ++c) {
                    var outChannel = event.outputBuffer.getChannelData(c);
                    for (i = 0, j = c; i<bufferSize; ++i, j += outputChannels) {
                        outChannel[i] = outbufferArray[j];
                    }
                }
            }
        };

        if (inputChannels > 0) {
            navigator.getUserMedia = navigator.getUserMedia ||
            navigator.webkitGetUserMedia ||
            navigator.mozGetUserMedia ||
            navigator.msGetUserMedia;
            
	    if (navigator.getUserMedia) {
                navigator.getUserMedia(
                {audio: true},
                function (audioIn){
                    var mediaElement = AUDIO.context.createMediaStreamSource(audioIn);
                    mediaElement.connect(stream);
                    AUDIO.mediaElement = mediaElement;
                },
                function (error){
                    console.log("error creating audio in",error);
                });
	    }
        }

        stream.connect(AUDIO.fft);
    },

    html5audio_stream_free: function () {

    },

    html5audio_sound_is_loaded: function (player_id) {
        if (AUDIO.player[player_id].readyState > 0) {
            return true;
        }
        return false;
    }
}

autoAddDeps(LibraryHTML5Audio, '$AUDIO');
mergeInto(LibraryManager.library, LibraryHTML5Audio);
