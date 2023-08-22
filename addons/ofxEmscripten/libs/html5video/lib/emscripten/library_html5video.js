var LibraryHTML5Video = {
    $VIDEO: {
        player: [],
        playerCounter: 0,

        getNewPlayerId: function() {
          var ret = VIDEO.playerCounter++;
          return ret;
        },

        grabber: [],
        grabberCounter: 0,

        getNewGrabberId: function() {
          var ret = VIDEO.grabberCounter++;
          return ret;
        },

        getUserMedia: function(){
        	return navigator.mediaDevices.getUserMedia ||
        	    navigator.mediaDevices.webkitGetUserMedia ||
        	    navigator.mediaDevices.mozGetUserMedia ||
        	    navigator.mediaDevices.msGetUserMedia;
        },

        update: function(updatePixels, video, context, dstPixels){
        	if((updatePixels || video.pixelFormat!="RGBA") && video.width!=0 && video.height!=0 && dstPixels!=0){
        		try {
	            	context.drawImage( video, 0, 0, video.width, video.height );
	            	imageData = context.getImageData(0,0,video.width,video.height);
	            	srcPixels = imageData.data;
	            	if (video.pixelFormat=="RGBA"){
		            	array.set(imageData.data);
	            	}else if(video.pixelFormat=="RGB"){
		            	array = Module.HEAPU8.subarray(dstPixels, dstPixels+(video.width*video.height*3));
		            	for(var i=0, j=0; i<array.length; ){
		            		array[i++] = srcPixels[j++];
		            		array[i++] = srcPixels[j++];
		            		array[i++] = srcPixels[j++];
		            		++j;
		            	}
		                GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[video.textureId]);
		                GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGB, video.width, video.height, 0, GLctx.RGB, GLctx.UNSIGNED_BYTE, array);
		                GLctx.bindTexture(GLctx.TEXTURE_2D, null);
	            	}else if(video.pixelFormat=="GRAY"){
		            	array = Module.HEAPU8.subarray(dstPixels, dstPixels+(video.width*video.height));
		            	for(var i=0, j=0; i<array.length; ){
		            		array[i++] = (((srcPixels[j++]|0) << 1) + ((srcPixels[j]|0) << 2) + (srcPixels[j++]|0) + (srcPixels[j++]|0)) >> 3;
		            		++j;
		            	}

		                GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[video.textureId]);
		                GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.LUMINANCE, video.width, video.height, 0, GLctx.LUMINANCE, GLctx.UNSIGNED_BYTE, array);
		                GLctx.bindTexture(GLctx.TEXTURE_2D, null);
	            	}
        		}catch(e){console.log(e);}
        	}
        	if (video.pixelFormat=="RGBA"){
                GLctx.bindTexture(GLctx.TEXTURE_2D, GL.textures[video.textureId]);
                GLctx.texImage2D(GLctx.TEXTURE_2D, 0, GLctx.RGBA, GLctx.RGBA, GLctx.UNSIGNED_BYTE, video);
                GLctx.bindTexture(GLctx.TEXTURE_2D, null);
        	}
        }
    },

    html5video_list_devices: function(){
        if (!navigator.mediaDevices.enumerateDevices) {
            console.log("enumerateDevices() not supported.");
        } else {
            // List cameras and microphones.
            navigator.mediaDevices.enumerateDevices()
            .then((devices) => {
                devices.forEach((device) => {
                    if(device.kind == "videoinput"){
                        console.log(`${device.kind}: ${device.label} id = ${device.deviceId}`);
                    }
                });
            })
            .catch((err) => {
                console.error(`${err.name}: ${err.message}`);
            });
        }
    },

   html5video_player_create: function(){
        var video = document.createElement('video');
        video.loop = true;
        video.pixelFormat = "RGBA";
	var player_id = VIDEO.getNewPlayerId();
	VIDEO.player[player_id] = video;
	var source = AUDIO.context.createMediaElementSource(VIDEO.player[player_id]); 
	VIDEO.player[player_id].soundPan = AUDIO.context.createStereoPanner();
	source.connect(VIDEO.player[player_id].soundPan).connect(AUDIO.fft);
	video.onloadedmetadata = function (e){
        	VIDEO.player[player_id].width = this.videoWidth;
        	VIDEO.player[player_id].height = this.videoHeight;
		var videoImage = document.createElement( 'canvas' );
		videoImage.width = this.videoWidth;
		videoImage.height = this.videoHeight;
		var videoImageContext = videoImage.getContext( '2d' );
		// background color if no video present
		videoImageContext.fillStyle = '#000000';
		videoImageContext.fillRect( 0, 0, videoImage.width, videoImage.height );
		VIDEO.player[player_id].playerContext = videoImageContext;
        };
        return player_id;
    },

    html5video_player_delete: function(player_id){
    	VIDEO.player[player_id] = null;
    },
    
    html5video_player_load__deps: ['$GL'],
    html5video_player_load: function(player_id, src) {
        VIDEO.player[player_id].src = UTF8ToString(src);
        var texId = GL.getNewId(GL.textures);
        var texture = GLctx.createTexture();
        texture.name = texId;
        GL.textures[texId] = texture;
        GLctx.bindTexture(GLctx.TEXTURE_2D, texture);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MAG_FILTER, GLctx.LINEAR);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MIN_FILTER, GLctx.LINEAR);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_S, GLctx.CLAMP_TO_EDGE);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_T, GLctx.CLAMP_TO_EDGE);
        VIDEO.player[player_id].textureId = texId;
    },

    html5video_player_pixel_format: function(player_id) {
        var string = VIDEO.player[player_id].pixelFormat;
        var size = lengthBytesUTF8(string) + 1;
        var stringPointer = stackAlloc(size);
        stringToUTF8Array(string, HEAP8, stringPointer, size);
        return stringPointer;
    },

    html5video_player_set_pixel_format: function(player_id, format) {
        VIDEO.player[player_id].pixelFormat = UTF8ToString(format);
    },

    html5video_player_update__deps: ['$GL'],
    html5video_player_update: function(player_id, update_pixels, pixels) {
        var player = VIDEO.player[player_id];
        var array;
        var imageData;
        var data;
        if ( player.readyState === player.HAVE_ENOUGH_DATA ) {
        	VIDEO.update(update_pixels, player, VIDEO.player[player_id].playerContext, pixels);
            return true;
        }else{
        	return false;
        }
    },

    html5video_player_texture_id: function(player_id) {
        return VIDEO.player[player_id].textureId;
    },

    html5video_player_width: function(player_id) {
        return VIDEO.player[player_id].width;
    },

    html5video_player_height: function(player_id) {
        return VIDEO.player[player_id].height;
    },

    html5video_player_play: function(player_id) {
        VIDEO.player[player_id].play();
    },

    html5video_player_pause: function(player_id) {
        VIDEO.player[player_id].pause();
    },

    html5video_player_stop: function(player_id) {
        VIDEO.player[player_id].pause();
    },

    html5video_player_is_paused: function(player_id) {
        return VIDEO.player[player_id].paused;
    },

    html5video_player_ready_state: function(player_id) {
        return VIDEO.player[player_id].readyState;
    },

    html5video_player_duration: function(player_id) {
        if (VIDEO.player[player_id].src != "") {
            return VIDEO.player[player_id].duration;
        } else {
            return 0;
        }
    },

    html5video_player_current_time: function(player_id) {
        if (VIDEO.player[player_id].src != "") {
            return VIDEO.player[player_id].currentTime;
        } else {
            return 0;
        }            
    },

    html5video_player_set_current_time: function(player_id, time) {
	VIDEO.player[player_id].currentTime = time;
    },

    html5video_player_ended: function(player_id) {
        return VIDEO.player[player_id].ended;
    },

    html5video_player_playback_rate: function(player_id) {
        return VIDEO.player[player_id].playbackRate;
    },

    html5video_player_set_playback_rate: function(id,rate) {
        VIDEO.player[id].playbackRate = rate;
    },

    html5video_player_volume: function(player_id) {
        return VIDEO.player[player_id].volume;
    },

    html5video_player_set_volume: function(player_id, volume) {
        VIDEO.player[player_id].volume = volume;
    },

    html5video_player_set_loop: function(player_id, loop) {
        VIDEO.player[player_id].loop = loop;
    },

    html5video_player_loop: function(id) {
        return VIDEO.player[player_id].loop;
    },
    
    html5video_player_set_pan: function (player_id, pan) {
    	VIDEO.player[player_id].soundPan.pan.value = pan;
    },

    html5video_player_pan: function (player_id) {
        return VIDEO.player[player_id].soundPan.pan.value;
    },
    
    html5video_grabber_create: function() {
        var video = document.createElement('video');
        video.autoplay=true;
        video.pixelFormat = "RGB";

        var grabber_id = VIDEO.getNewGrabberId();
        VIDEO.grabber[grabber_id] = video;
	var texId = GL.getNewId(GL.textures);
	var texture = GLctx.createTexture();
	texture.name = texId;
	GL.textures[texId] = texture;
	GLctx.bindTexture(GLctx.TEXTURE_2D, texture);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MAG_FILTER, GLctx.LINEAR);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MIN_FILTER, GLctx.LINEAR);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_S, GLctx.CLAMP_TO_EDGE);
	GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_T, GLctx.CLAMP_TO_EDGE);
	VIDEO.grabber[grabber_id].textureId = texId;
	return grabber_id;
    },

    html5video_grabber_init__deps: ['$GL'],
    html5video_grabber_init: function(grabber_id, w, h, framerate) {
    	if(grabber_id != -1){
        	VIDEO.grabber[grabber_id].width = w;
        	VIDEO.grabber[grabber_id].height = h;

    	    var videoImage = document.createElement( 'canvas' );
    	    videoImage.width = w;
    	    videoImage.height = h;

    	    var videoImageContext = videoImage.getContext( '2d' );
    	    // background color if no video present
    	    videoImageContext.fillStyle = '#000000';
    	    videoImageContext.fillRect( 0, 0, w, h );

    	    VIDEO.grabber[grabber_id].context = videoImageContext;

    		var errorCallback = function(e) {
    			console.log('Couldn\'t init grabber!', e);
    		};

    		if(framerate==-1) {
    			var constraints = {
	    			video: {
		    		    mandatory: {
		    		        maxWidth: w,
		    		        maxHeight: h
		    		    }
	    		    }
    			};
    		} else {
    			var constraints = {
	    			video: {
		    		    mandatory: {
		    		        maxWidth: w,
		    		        maxHeight: h,
		    		    },
    					optional: [
    					    { minFrameRate: framerate }
		    		    ]
	    		    }
    			};
    		}

        navigator.mediaDevices.getUserMedia(constraints)
        .then(function(stream) {
          window.stream = stream;
          VIDEO.grabber[grabber_id].srcObject = stream;
          VIDEO.grabber[grabber_id].onloadedmetadata = function (e) {
            VIDEO.grabber[grabber_id].play();
          }
        })
        .catch(function(err) {
          console.log(e);
        });

    	}
    },

    html5video_grabber_pixel_format: function(grabber_id) {
        var string = VIDEO.grabber[grabber_id].pixelFormat;
        var size = lengthBytesUTF8(string) + 1;
        var stringPointer = stackAlloc(size);
        stringToUTF8Array(string, HEAP8, stringPointer, size);
        return stringPointer;
    },

    html5video_grabber_set_pixel_format: function(grabber_id, format) {
        VIDEO.grabber[grabber_id].pixelFormat = UTF8ToString(format);
    },

    html5video_grabber_update__deps: ['$GL'],
    html5video_grabber_update: function(grabber_id, update_pixels, pixels) {
        var grabber = VIDEO.grabber[grabber_id];
        if ( grabber.readyState == grabber.HAVE_METADATA ) {
        	VIDEO.update(update_pixels, grabber, VIDEO.grabber[grabber_id].context, pixels);
            return true;
        }else{
        	return false;
        }
    },

    html5video_grabber_texture_id: function(grabber_id) {
        return VIDEO.grabber[grabber_id].textureId;
    },

    html5video_grabber_width: function(grabber_id) {
        return VIDEO.grabber[grabber_id].width;
    },

    html5video_grabber_height: function(grabber_id) {
        return VIDEO.grabber[grabber_id].height;
    },

    html5video_grabber_ready_state: function(grabber_id) {
        return VIDEO.grabber[grabber_id].readyState;
    },

}

autoAddDeps(LibraryHTML5Video, '$VIDEO');
mergeInto(LibraryManager.library, LibraryHTML5Video);
