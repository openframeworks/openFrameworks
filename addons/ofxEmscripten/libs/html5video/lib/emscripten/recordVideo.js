var mediaParts;
var startTime;
var mediaRecorder;

function recordVideo(isRecording) {
	if (isRecording) {
		mediaParts = [];
		var stream = canvas.captureStream();
		if (typeof AUDIO != 'undefined') {
			stream.addTrack(AUDIO.contextStream.stream.getAudioTracks()[0]);
			mediaRecorder = new MediaRecorder(stream, {
				mimeType: 'video/webm'
			});
		} else {
			mediaRecorder = new MediaRecorder(stream, {
				mimeType: 'video/webm'
			});
		}
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
