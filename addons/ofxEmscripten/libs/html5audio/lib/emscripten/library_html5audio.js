var LibraryHTML5Audio = {
    $AUDIO: {
    	contexts: [],
    	ffts: [],
    	lastContextID: 0,
    	
    	soundBuffers: [],
    	soundSources: [],
    	soundStartTimes: [],
    	soundGains: [],
    	lastSoundID: 0,
    	
    	streams: [],
    	mediaElements: [],
    	lastStreamID: 0,
    	
    	soundPosition: function(sound_id){
    		var source = AUDIO.soundSources[sound_id];
        	if(source!=undefined){
        		var context = source.context;
        		var playTime = context.currentTime - source.startTime;
        		var duration = AUDIO.soundBuffers[sound_id].duration / AUDIO.soundSources[sound_id].playbackRate.value ;
        		return Math.min(duration,playTime);
        	}else{
        		return 0;
        	}	
    	}
    },
    
    html5audio_context_create: function(){
    	try {
			// Fix up for prefixing
			window.AudioContext = window.AudioContext || window.webkitAudioContext;
			var context = new AudioContext();
			var id = AUDIO.lastContextID++;
			AUDIO.contexts[id] = context;
			var fft = context.createAnalyser();
			fft.smoothingTimeConstant = 0;
			fft.connect(AUDIO.contexts[id].destination);
			fft.maxDecibels = 0;
			fft.minDecibels = -100;
			AUDIO.ffts[id] = fft;
			return id;
    	} catch(e) {
    		console.log('Web Audio API is not supported in this browser',e);
    		return -1;
    	}
    },
	
	html5audio_context_spectrum: function(context_id, bands, spectrum){
		AUDIO.ffts[context_id].fftSize = bands*2;
		var spectrumArray = Module.HEAPF32.subarray(spectrum>>2, (spectrum>>2)+bands);
		AUDIO.ffts[context_id].getFloatFrequencyData(spectrumArray);
	},
    
    html5audio_context_samplerate: function(context){
    	return AUDIO.contexts[context_id].sampleRate.value;
    },
    
    html5audio_sound_load: function(context_id, url){
		var request = new XMLHttpRequest();
		request.open('GET', Pointer_stringify(url), true);
		request.responseType = 'arraybuffer';
		
		var id = AUDIO.lastSoundID++;
		AUDIO.soundGains[id] = AUDIO.contexts[context_id].createGain();
		AUDIO.soundGains[id].connect(AUDIO.ffts[context_id]);
		 
		// Decode asynchronously
		request.onload = function() {
			AUDIO.contexts[context_id].decodeAudioData(request.response, 
				function(buffer) {
					AUDIO.soundBuffers[id] = buffer;
				}, 
				function(e){
					console.log("couldn't decode sound " + id, e);
				}
			);
    	};
    	request.send();
    	return id;
    },
    
    html5audio_sound_play: function(context_id, sound_id, offset){
    	if(AUDIO.soundBuffers[sound_id]!=undefined){
    		if(AUDIO.contexts[context_id]!=undefined && AUDIO.contexts[context_id].paused){
    			AUDIO.contexts[context_id].paused = false;
    			AUDIO.contexts[context_id].start(offset);
    		}else{
		    	var source = AUDIO.contexts[context_id].createBufferSource();
		    	source.buffer = AUDIO.soundBuffers[sound_id];
		    	source.connect(AUDIO.soundGains[sound_id]);
		    	source.name = sound_id;
		    	source.done = false;
		    	source.paused = false;
		    	source.onended = function(event){
		    		event.target.done = true;
		    	} 
		    	AUDIO.soundSources[sound_id] = source;
		    	source.startTime = AUDIO.contexts[context_id].currentTime - offset;
	    		source.start(offset);
    		}
    	}
    },

    html5audio_sound_stop: function(sound_id){
    	AUDIO.soundSources[sound_id].stop();
    },

    html5audio_sound_pause: function(sound_id){
    	AUDIO.soundSources[sound_id].stop();
    	AUDIO.soundSources[sound_id].paused = true;
    },
    
    html5audio_sound_rate: function(sound_id){
    	if(AUDIO.soundSources[sound_id]!=undefined){
    		return AUDIO.soundSources[sound_id].playbackRate.value;
    	}
    },
    
    html5audio_sound_set_rate: function(sound_id,rate){
    	var source = AUDIO.soundSources[sound_id];
    	if(source!=undefined){
    		var offset = AUDIO.soundPosition(sound_id);
    		source.startTime = source.context.currentTime - offset;
    		AUDIO.soundSources[sound_id].playbackRate.value = rate;
    	}
    },
    
    html5audio_sound_done: function(sound_id){
    	if(AUDIO.soundSources[sound_id]!=undefined){
    		return AUDIO.soundSources[sound_id].done;
    	}else{
    		return false;
    	}
    },
    
    html5audio_sound_duration: function(sound_id){
    	if(AUDIO.soundBuffers[sound_id]!=undefined){
    		return AUDIO.soundBuffers[sound_id].duration;
    	}else{
    		return 0;
    	}
    },

	html5audio_sound_position: function(sound_id){
		return AUDIO.soundPosition(sound_id);
	},
    
	html5audio_sound_set_loop: function(sound_id, loop){
		AUDIO.soundSources[sound_id].loop = loop;
	},
    
	html5audio_sound_set_gain: function(sound_id, gain){
		AUDIO.soundGains[sound_id].gain = gain;
	},
    
	html5audio_sound_gain: function(sound_id){
		return AUDIO.soundGains[sound_id].gain;
	},
    
	html5audio_sound_free: function(sound_id){
		return AUDIO.soundBuffers[sound_id] = null;
		return AUDIO.soundSources[sound_id] = null;
		return AUDIO.soundStartTimes[sound_id] = 0;
		return AUDIO.soundGains[sound_id] = null;
	},
	
	html5audio_stream_create: function(context_id, bufferSize, inputChannels, outputChannels, inbuffer, outbuffer, callback, userData){
		var stream = AUDIO.contexts[context_id].createScriptProcessor(bufferSize,inputChannels,outputChannels);
		var inbufferArray = Module.HEAPF32.subarray(inbuffer>>2,(inbuffer>>2)+bufferSize*inputChannels);
		var outbufferArray = Module.HEAPF32.subarray(outbuffer>>2,(outbuffer>>2)+bufferSize*outputChannels);
		
		var id = AUDIO.lastStreamID++;
		
		stream.onaudioprocess = function(event){
			var i,j,c;
			if(inputChannels>0){
				for(c=0;c<inputChannels;++c){
					var inChannel = event.inputBuffer.getChannelData(c);
					for(i=0,j=c;i<bufferSize;++i,j+=inputChannels){
						inbufferArray[j] = inChannel[i];
					}
				}
			}
			
			Runtime.dynCall('viiii',callback, [bufferSize,inputChannels,outputChannels,userData]);
			
			if(outputChannels>0){
				for(c=0;c<outputChannels;++c){
					var outChannel = event.outputBuffer.getChannelData(c);
					for(i=0,j=c;i<bufferSize;++i,j+=outputChannels){
						outChannel[i] = outbufferArray[j];
					}
				}
			}
		};
		
		if(inputChannels>0){
			navigator.getUserMedia = navigator.getUserMedia ||
							    	    navigator.webkitGetUserMedia ||
							    	    navigator.mozGetUserMedia ||
							    	    navigator.msGetUserMedia;
			
			if(navigator.getUserMedia){
				navigator.getUserMedia(
					{audio: true}, 
					function(audioIn) {
						var mediaElement = AUDIO.contexts[context_id].createMediaStreamSource(audioIn);
						mediaElement.connect(stream);
						AUDIO.mediaElements[id] = mediaElement;
					},
					function(error){
						console.log("error creating audio in",error);
					}
				);
			}
		}
		
		stream.connect(AUDIO.ffts[context_id]);
		AUDIO.streams[id] = stream;
		return id;
	},
    
	html5audio_stream_free: function(stream_id){
		return AUDIO.streams[stream_id] = null;
		return AUDIO.mediaElements[stream_id] = null;
	}
}


autoAddDeps(LibraryHTML5Audio, '$AUDIO');
mergeInto(LibraryManager.library, LibraryHTML5Audio);