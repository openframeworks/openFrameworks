var LibraryHTML5Audio = {
    $AUDIO: {
        players: [],

        soundSources: [],
        soundPans: [],
        lastSoundID: 0,
    },

    html5audio_list_devices: function(){
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

    html5audio_sound_load: function (url) {
	var audio = document.createElement('audio');
	var id = AUDIO.lastSoundID++;
	AUDIO.players[id] = audio;
	AUDIO.players[id].src = UTF8ToString(url);
	AUDIO.soundSources[id] = AUDIO.context.createMediaElementSource(AUDIO.players[id]); 
	AUDIO.soundPans[id] = AUDIO.context.createStereoPanner();
	AUDIO.soundSources[id].connect(AUDIO.soundPans[id]).connect(AUDIO.fft);
	return id;
    },

    html5audio_sound_play: function (sound_id, offset) {
        AUDIO.players[sound_id].play(offset);
    },

    html5audio_sound_stop: function (sound_id) {
        AUDIO.players[sound_id].currentTime = 0;
        AUDIO.players[sound_id].pause();
    },

    html5audio_sound_pause: function (sound_id) {
        AUDIO.players[sound_id].pause();
    },

    html5audio_sound_rate: function (sound_id) {
	return AUDIO.players[sound_id].playbackRate;
    },

    html5audio_sound_set_rate: function (sound_id, rate) {
	AUDIO.players[sound_id].playbackRate = rate;
    },

    html5audio_sound_done: function (sound_id) {
	return AUDIO.players[sound_id].done;
    },

    html5audio_sound_duration: function (sound_id) {
	return AUDIO.players[sound_id].duration;
    },

    html5audio_sound_position: function (sound_id) {
        return AUDIO.players[sound_id].currentTime;
    },
    
    html5audio_sound_set_position: function (sound_id, position) {
        AUDIO.players[sound_id].currentTime = position * AUDIO.players[sound_id].duration;
    },

    html5audio_sound_set_loop: function (sound_id, loop) {
        AUDIO.players[sound_id].loop = true;
    },

    html5audio_sound_set_volume: function (sound_id, volume) {
        AUDIO.players[sound_id].volume = volume / 10;
    },

    html5audio_sound_volume: function (sound_id) {
        return AUDIO.players[sound_id].volume * 10;
    },

    html5audio_sound_set_pan: function (sound_id, pan) {
    	AUDIO.soundPans[sound_id].pan.value = pan;
    },

    html5audio_sound_pan: function (sound_id) {
        return AUDIO.soundPans[sound_id].pan.value;
    },
    
    html5audio_sound_free: function (sound_id) {
	if(AUDIO.players[sound_id] != undefined){
	AUDIO.players[sound_id].pause();
	URL.revokeObjectURL(AUDIO.players[sound_id].src);
	}
    },

    html5audio_stream_create: function(bufferSize, inputChannels, outputChannels, inbuffer, outbuffer, callback, userData){
        var stream = AUDIO.context.createScriptProcessor(bufferSize,inputChannels,outputChannels);
        var inbufferArray = Module.HEAPF32.subarray(inbuffer>>2,(inbuffer>>2)+bufferSize*inputChannels);
        var outbufferArray = Module.HEAPF32.subarray(outbuffer>>2,(outbuffer>>2)+bufferSize*outputChannels);

        stream.onaudioprocess = function(event){
            var i, j, c;
            if (inputChannels > 0){
                for (c = 0; c < inputChannels; ++c){
                    var inChannel = event.inputBuffer.getChannelData(c);
                    for (i = 0, j = c; i < bufferSize; ++i, j += inputChannels){
                        inbufferArray[j] = inChannel[i];
                    }
                }
            }

            {{{ makeDynCall('viiii', 'callback') }}}(bufferSize, inputChannels, outputChannels, userData);

            if (outputChannels > 0){
                for (c = 0;c < outputChannels; ++c){
                    var outChannel = event.outputBuffer.getChannelData(c);
                    for (i = 0,j = c;i<bufferSize; ++i, j += outputChannels){
                        outChannel[i] = outbufferArray[j];
                    }
                }
            }
        };

        if (inputChannels > 0){
            navigator.getUserMedia = navigator.getUserMedia ||
            navigator.webkitGetUserMedia ||
            navigator.mozGetUserMedia ||
            navigator.msGetUserMedia;
            
	    if (navigator.getUserMedia){
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
        AUDIO.stream = stream;
    },

    html5audio_stream_free: function () {
        return AUDIO.stream = null;
        return AUDIO.mediaElement = null;
    },

    html5audio_sound_is_loaded: function (sound) {
        if (AUDIO.players[id].src != undefined) {
            return true;
        }
        return false;
    }
}

autoAddDeps(LibraryHTML5Audio, '$AUDIO');
mergeInto(LibraryManager.library, LibraryHTML5Audio);
