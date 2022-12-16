var webMidi = null;
var midiIn = null;
var midiOut = null;

function getMidiDevices() {
	var midiInDevices = "";
	var midiOutDevices = "";
	function onMIDISuccess(midiAccess) {
		webMidi = midiAccess;
		var inputDeviceCount = midiAccess.inputs.size;
		if(inputDeviceCount > 0) {
			var inputs = midiAccess.inputs.values();
			for ( var input = inputs.next(); input && !input.done; input = inputs.next()) {
				midiInDevices = midiInDevices.concat(",", `${input.value.id}`,",", `${input.value.name}`);
			}
  			Module.loadMidiInDevices(midiInDevices);
		}
		var outputDeviceCount = midiAccess.outputs.size;
		if(outputDeviceCount > 0) {
			var outputs = midiAccess.outputs.values();
			for ( var output = outputs.next(); output && !output.done; output = outputs.next()) {
				midiOutDevices = midiOutDevices.concat(",", `${output.value.id}`,",", `${output.value.name}`);
			}
  			Module.loadMidiOutDevices(midiOutDevices);
		}
	}
	function onMIDIFailure(e) {
		console.log('No access to MIDI devices' + e);
	}
	if(navigator.requestMIDIAccess){
		navigator.requestMIDIAccess({ sysex: false }).then(onMIDISuccess, onMIDIFailure);
	}else{
		alert("No MIDI support in your browser.");
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
	console.log( "Midi in: " + midiIn.data[0] + " " + midiIn.data[1] + " " + midiIn.data[2]);
  	Module.midiIn(midiIn.data);
  }
  
function sendMidi(midiOutData){
	console.log("Midi out: " + midiOutData[0] + " " + midiOutData[1] + " " + midiOutData[2]) ;
	if(ENVIRONMENT_IS_WEB && midiOut){
		midiOut.send(midiOutData);  
	}
}
