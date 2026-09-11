var mediaElement = null;

function getAudioDevices(){
	var audioInDevices = "";
	var audioOutDevices = "";
	if(!navigator.mediaDevices.enumerateDevices){
		console.log("enumerateDevices() not supported.");
	}else{
		navigator.mediaDevices.getUserMedia({audio: true, video: false}).then(s => {
			navigator.mediaDevices.enumerateDevices().then((devices) => {
				devices.forEach((device) => {
					if(device.kind == "audioinput"){
						audioInDevices = audioInDevices.concat(",", `${device.deviceId}`,",", `${device.label}`);
					}
					if(device.kind == "audiooutput"){
						audioOutDevices = audioOutDevices.concat(",", `${device.groupId}`,",", `${device.label}`);                 
					}
				});
				Module.loadAudioInDevices(audioInDevices);
				Module.loadAudioOutDevices(audioOutDevices);
			})
			.catch((err) => {
				console.error(`${err.name}: ${err.message}`);
			});
		});
        }
}
				
function selectAudioIn(file){
	if (navigator.getUserMedia){
		navigator.getUserMedia(
		{ audio: { deviceId: { exact: file } } },
		function (audioIn){
			if(mediaElement){
				mediaElement.disconnect();
			}
		mediaElement = AUDIO.context.createMediaStreamSource(audioIn);
		mediaElement.connect(AUDIO.stream);
		},
		function (error){
			console.log("error creating audio in",error);
		});
	}
}
	
function selectAudioOut(file){	
	//AUDIO.context.setSinkId(file);
}	      
