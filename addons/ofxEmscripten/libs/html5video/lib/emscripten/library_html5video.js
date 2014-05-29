var LibraryHTML5Video = {
    $VIDEO: {
        players: [],
        playersContexts: [],
        playersCounter: 0,
        
        getNewPlayerId: function() {
          var ret = VIDEO.playersCounter++;
          return ret;
        },
        
        grabbers: [],
        grabbersContexts: [],
        grabbersCounter: 0,
        
        getNewGrabberId: function() {
          var ret = VIDEO.grabbersCounter++;
          return ret;
        },
        
        getUserMedia: function(){
        	return navigator.getUserMedia ||
        	    navigator.webkitGetUserMedia ||
        	    navigator.mozGetUserMedia ||
        	    navigator.msGetUserMedia;
        },
        
        update: function(updatePixels, video, context, dstPixels){
        	if((updatePixels || video.pixelFormat!="RGBA") && video.width!=0 && video.height!=0 && dstPixels!=0){
        		try {
	            	context.drawImage( video, 0, 0, video.width, video.height );
	            	imageData = context.getImageData(0,0,video.width,video.height);
	            	srcPixels = imageData.data;
	            	if (video.pixelFormat=="RGBA"){
		            	//TODO: this is faster but under chrome, loop and set_time stop working
		            	//array.set(imageData.data);
		            	array = Module.HEAPU8.subarray(dstPixels, dstPixels+(video.width*video.height*4));
		            	for(var i=0;i<data.length;i++){
		            		array[i] = srcPixels[i];
		            	}
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
    
    html5video_player_create: function(){
        var video = document.createElement('video');
        video.loop = true;
        video.pixelFormat = "RGB";
        //video.crossOrigin = 'anonymous';
        
        var player_id = VIDEO.getNewPlayerId();
        VIDEO.players[player_id] = video;
        video.onloadedmetadata = function (e){
        	console.log(this.videoWidth + 'x' + this.videoHeight);
        	VIDEO.players[player_id].width = this.videoWidth;
        	VIDEO.players[player_id].height = this.videoHeight;
            
    	    var videoImage = document.createElement( 'canvas' );
    	    videoImage.width = this.videoWidth;
    	    videoImage.height = this.videoHeight;

    	    var videoImageContext = videoImage.getContext( '2d' );
    	    // background color if no video present
    	    videoImageContext.fillStyle = '#000000';
    	    videoImageContext.fillRect( 0, 0, videoImage.width, videoImage.height );
    	    
    	    VIDEO.playersContexts[player_id] = videoImageContext;
        };
	    
        return player_id;
    },
    
    html5video_player_delete: function(id){
    	VIDEO.players[id] = null;
    },

    html5video_player_load__deps: ['$GL'],
    html5video_player_load: function(id,src){
        console.log(Pointer_stringify(src));
        VIDEO.players[id].src = Pointer_stringify(src);
        var texId = GL.getNewId(GL.textures);
        var texture = GLctx.createTexture();
        texture.name = texId;
        GL.textures[texId] = texture;
        GLctx.bindTexture(GLctx.TEXTURE_2D, texture);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MAG_FILTER, GLctx.LINEAR);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MIN_FILTER, GLctx.LINEAR);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_S, GLctx.CLAMP_TO_EDGE);
        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_T, GLctx.CLAMP_TO_EDGE);
        VIDEO.players[id].textureId = texId;
        VIDEO.players[id].load();
    },
    
    html5video_player_pixel_format: function(id){
        return allocate(intArrayFromString(VIDEO.players[id].pixelFormat), 'i8', ALLOC_STACK);
    },
    
    html5video_player_set_pixel_format: function(id, format){
        VIDEO.players[id].pixelFormat = Pointer_stringify(format);
    },
    
    html5video_player_update__deps: ['$GL'],
    html5video_player_update: function(id,update_pixels,pixels){
        var player = VIDEO.players[id];
        var array;
        var imageData;
        var data;
        if ( player.readyState === player.HAVE_ENOUGH_DATA ) {
        	VIDEO.update(update_pixels, player, VIDEO.playersContexts[id], pixels);
            return true;
        }else{
        	return false;
        }
    },
    
    html5video_player_texture_id: function(id){
        return VIDEO.players[id].textureId;
    },
    
    html5video_player_width: function(id){
        return VIDEO.players[id].width;
    },
    
    html5video_player_height: function(id){
        return VIDEO.players[id].height;
    },
    
    html5video_player_play: function(id){
        console.log('play');
        VIDEO.players[id].play();
    },
    
    html5video_player_pause: function(id){
        VIDEO.players[id].pause();
    },
    
    html5video_player_stop: function(id){
        VIDEO.players[id].pause();
    },
    
    html5video_player_is_paused: function(id){
        return VIDEO.players[id].paused;
    },
    
    html5video_player_ready_state: function(id){
        return VIDEO.players[id].readyState;
    },
    
    html5video_player_duration: function(id){
        return VIDEO.players[id].duration;
    },
    
    html5video_player_current_time: function(id){
        return VIDEO.players[id].currentTime;
    },
    
    html5video_player_set_current_time: function(id,time){
        VIDEO.players[id].currentTime = time;
    },
    
    html5video_player_ended: function(id){
        return VIDEO.players[id].ended;
    },
    
    html5video_player_playback_rate: function(id){
        return VIDEO.players[id].playbackRate;
    },
    
    html5video_player_set_playback_rate: function(id,rate){
        VIDEO.players[id].playbackRate = rate;
    },
    
    html5video_player_volume: function(id){
        return VIDEO.players[id].volume;
    },
    
    html5video_player_set_volume: function(id,volume){
        VIDEO.players[id].volume = volume;
    },
    
    html5video_player_set_loop: function(id,loop){
        VIDEO.players[id].loop = loop;
    },
    
    html5video_player_loop: function(id){
        return VIDEO.players[id].loop;
    },
    
    html5video_grabber_create: function(){
    	if( VIDEO.getUserMedia() ){
	        var video = document.createElement('video');
			video.autoplay=true;
			video.pixelFormat = "RGB";
	        
	        var grabber_id = VIDEO.getNewGrabberId();
	        VIDEO.grabbers[grabber_id] = video;
	        return grabber_id;
    	}else{
    		console.log("coudln't create grabber");
    		return -1;
    	}
    },

    html5video_grabber_init__deps: ['$GL'],
    html5video_grabber_init: function(id, w, h, framerate){
    	if(id!=-1){
        	VIDEO.grabbers[id].width = w;
        	VIDEO.grabbers[id].height = h;
        	
    	    var videoImage = document.createElement( 'canvas' );
    	    videoImage.width = w;
    	    videoImage.height = h;

    	    var videoImageContext = videoImage.getContext( '2d' );
    	    // background color if no video present
    	    videoImageContext.fillStyle = '#000000';
    	    videoImageContext.fillRect( 0, 0, w, h );
    	    
    	    VIDEO.grabbersContexts[id] = videoImageContext;
        	
    		var errorCallback = function(e) {
    			console.log('Couldn\'t init grabber!', e);
    		};

    		if(framerate==-1){
    			var constraints = {
	    			video: {
		    		    mandatory: {
		    		        maxWidth: w,
		    		        maxHeight: h
		    		    }
	    		    }
    			};
    		}else{
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

    		var getUserMedia = VIDEO.getUserMedia().bind(navigator);
    		getUserMedia(constraints, function(stream) {
				VIDEO.grabbers[id].src = window.URL.createObjectURL(stream);
				var texId = GL.getNewId(GL.textures);
		        var texture = GLctx.createTexture();
		        texture.name = texId;
		        GL.textures[texId] = texture;
		        GLctx.bindTexture(GLctx.TEXTURE_2D, texture);
		        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MAG_FILTER, GLctx.LINEAR);
		        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_MIN_FILTER, GLctx.LINEAR);
		        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_S, GLctx.CLAMP_TO_EDGE);
		        GLctx.texParameteri(GLctx.TEXTURE_2D, GLctx.TEXTURE_WRAP_T, GLctx.CLAMP_TO_EDGE);
		        VIDEO.grabbers[id].textureId = texId;
			}, errorCallback);
    	}
    },
    
    html5video_grabber_pixel_format: function(id){
        return allocate(intArrayFromString(VIDEO.grabbers[id].pixelFormat), 'i8', ALLOC_STACK);
    },
    
    html5video_grabber_set_pixel_format: function(id, format){
        VIDEO.grabbers[id].pixelFormat = Pointer_stringify(format);
    },
    
    html5video_grabber_update__deps: ['$GL'],
    html5video_grabber_update: function(id,update_pixels,pixels){
        var grabber = VIDEO.grabbers[id];
        if ( grabber.readyState >= grabber.HAVE_METADATA ) {
        	VIDEO.update(update_pixels, grabber, VIDEO.grabbersContexts[id], pixels);
            return true;
        }else{
        	return false;
        }
    },
    
    html5video_grabber_texture_id: function(id){
        return VIDEO.grabbers[id].textureId;
    },
    
    html5video_grabber_width: function(id){
        return VIDEO.grabbers[id].width;
    },
    
    html5video_grabber_height: function(id){
        return VIDEO.grabbers[id].height;
    },
    
    html5video_grabber_ready_state: function(id){
        return VIDEO.grabbers[id].readyState;
    },
    
    
}


autoAddDeps(LibraryHTML5Video, '$VIDEO');
mergeInto(LibraryManager.library, LibraryHTML5Video);
