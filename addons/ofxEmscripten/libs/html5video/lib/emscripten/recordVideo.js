drawTexture__deps = ['$GL'];
var stream;
var mediaParts;
var startTime;
var mediaRecorder;
var data;
var fb;
var canvas2 = document.createElement('canvas');    
var context2 = canvas2.getContext('2d');

function recordVideo(isRecording, recordTexture, textureID) {
	if (isRecording) {
		mediaParts = [];
		if (recordTexture) {
			stream = canvas2.captureStream(30);
			var texture = GL.textures[textureID];
	
			// make a framebuffer
			fb = GLctx.createFramebuffer();

			// make this the current frame buffer
			GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb);

			// attach the texture to the framebuffer.
			GLctx.framebufferTexture2D(
			GLctx.FRAMEBUFFER, GLctx.COLOR_ATTACHMENT0,
			GLctx.TEXTURE_2D, texture, 0);
	
			// Unbind the framebuffer
			GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
		} else {
			stream = canvas.captureStream(30);
		}
		stream.addTrack(AUDIO.contextStream.stream.getAudioTracks()[0]);
		mediaRecorder = new MediaRecorder(stream, {
			audioBitsPerSecond : 128000, // 128 kbps
  			videoBitsPerSecond: 10000000, // 4x the default quality from 2.5Mbps to 10Mbps
			mimeType: 'video/webm'
		});
		mediaRecorder.onstop = function() {
			var duration = Date.now() - startTime;
			var buggyBlob = new Blob(mediaParts, { type: 'video/webm' });
			ysFixWebmDuration(buggyBlob, duration, function(fixedBlob) {
				downloadBlob(fixedBlob);
			});
		};
		mediaRecorder.ondataavailable = function(event) {
			var data = event.data;
			if (data && data.size > 0) {
				mediaParts.push(data);
			}
		};
		mediaRecorder.start();
		startTime = Date.now();
	} else {
		mediaRecorder.stop();
	}
}

function downloadBlob(blob) {
	var url = URL.createObjectURL(blob);
	var tag = document.createElement('a');
	tag.href = url;
	tag.download = 'Video.webm';
	document.body.appendChild(tag);
	tag.click();
	document.body.removeChild(tag);
}

function drawTexture(textureWidth, textureHeight){ 
	// make this the current frame buffer
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, fb);
	
	// read the pixels
	if (canvas2.width != textureWidth || canvas2.height != textureHeight) {
		canvas2.width = textureWidth;
		canvas2.height = textureHeight;
		data = new Uint8Array(textureWidth * textureHeight * 4);
	}
	GLctx.readPixels(0, 0, textureWidth, textureHeight, GLctx.RGBA, GLctx.UNSIGNED_BYTE, data);
	
	// Unbind the framebuffer
	GLctx.bindFramebuffer(GLctx.FRAMEBUFFER, null);
	var imageData = new ImageData(new Uint8ClampedArray(data.buffer), textureWidth, textureHeight);
	context2.putImageData(imageData, 0, 0);
}
