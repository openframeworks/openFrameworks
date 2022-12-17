var webMidi = null;
var midiIn = null;
var midiOut = null;

function startMidi(){
	function onMidiSuccess(midiAccess){
		webMidi = midiAccess;
		getMidiDevices();
		webMidi.onstatechange = (event) => {
			console.log(event.port.name, event.port.manufacturer, event.port.state);
			getMidiDevices();
		}
	}
	function onMidiFailure(e){
		console.log('no access to midi devices' + e);
	}
	if(navigator.requestMIDIAccess){
		navigator.requestMIDIAccess({sysex: false}).then(onMidiSuccess, onMidiFailure);
	}else{
		alert("no midi support in your browser.");
	}
}

function getMidiDevices(){
		var midiInDevices = "";
		var inputDeviceCount = webMidi.inputs.size;
		if(inputDeviceCount > 0){
			var inputs = webMidi.inputs.values();
			for(var input = inputs.next(); input && !input.done; input = inputs.next()){
				midiInDevices = midiInDevices.concat(",", `${input.value.id}`,",", `${input.value.name}`);
			}
  			Module.loadMidiInDevices(midiInDevices);
		}
		var midiOutDevices = "";
		var outputDeviceCount = webMidi.outputs.size;
		if(outputDeviceCount > 0){
			var outputs = webMidi.outputs.values();
			for(var output = outputs.next(); output && !output.done; output = outputs.next()){
				midiOutDevices = midiOutDevices.concat(",", `${output.value.id}`,",", `${output.value.name}`);
			}
  			Module.loadMidiOutDevices(midiOutDevices);
		}
}
				
function selectMidiIn(file){
	if(midiIn){
		midiIn.onmidimessage = null;
	}
	midiIn = webMidi.inputs.get(file);
	if(midiIn){
        	midiIn.onmidimessage = midiMessageReceived;
	}
}
	
function selectMidiOut(file){	
	if(midiOut){
		midiOut.onmidimessage = null;
	}
	midiOut = webMidi.outputs.get(file);
	if(midiOut){
        	midiOut.onmidimessage = midiMessageReceived;
	}
}	      
        
function midiMessageReceived(midiIn){
	var channel = midiIn.data[0];
	var pitch = midiIn.data[1];
	var velocity = midiIn.data[2];
	console.log("midi in - channel: " + midiIn.data[0] + " pitch: " + midiIn.data[1] + " velocity: " + midiIn.data[2]);
  	Module.midiIn(midiIn.data);
  }
  
function sendMidi(midiOutData){
	console.log("midi out - channel: " + midiOutData[0] + " pitch: " + midiOutData[1] + " velocity: " + midiOutData[2]);
	if(ENVIRONMENT_IS_WEB && midiOut){
		midiOut.send(midiOutData);  
	}
}
