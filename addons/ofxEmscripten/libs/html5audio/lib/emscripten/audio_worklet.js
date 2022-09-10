// This file is the main bootstrap script for Wasm Audio Worklets loaded in an Emscripten application.
// Build with -sAUDIO_WORKLET=1 linker flag to enable targeting Audio Worklets.

// AudioWorkletGlobalScope does not have a onmessage/postMessage() functionality at the global scope, which
// means that after creating an AudioWorkletGlobalScope and loading this script into it, we cannot
// postMessage() information into it like one would do with Web Workers.

// Instead, we must create an AudioWorkletProcessor class, then instantiate a Web Audio graph node from it
// on the main thread. Using its message port and the node constructor's
// "processorOptions" field, we can share the necessary bootstrap information from the main thread to
// the AudioWorkletGlobalScope.

function createWasmAudioWorkletProcessor(audioParams) {
  class WasmAudioWorkletProcessor extends AudioWorkletProcessor {
    constructor(args) {
      super();

      // Copy needed stack allocation functions from the Module object
      // to global scope, these will be accessed in hot paths, so maybe
      // they'll be a bit faster to access directly, rather than referencing
      // them as properties of the Module object.
      globalThis.stackAlloc = Module['stackAlloc'];
      globalThis.stackSave = Module['stackSave'];
      globalThis.stackRestore = Module['stackRestore'];
      globalThis.HEAPU32 = Module['HEAPU32'];
      globalThis.HEAPF32 = Module['HEAPF32'];

      // Capture the Wasm function callback to invoke.
      let opts = args.processorOptions;
      this.bufferSize = opts['bufferSize'];
      this.inputChannels = opts['inputChannels'];
      this.outputChannels = opts['outputChannels'];
      this.userData2 = opts['userData2'];
      this.inbuffer = opts['inbuffer'];
      this.outbuffer = opts['outbuffer'];
      this.callback = Module['wasmTable'].get(opts['cb2']);
    }

    static get parameterDescriptors() {
      return audioParams;
    }

	process(inputList, outputList, parameters) {
		this.callback(this.bufferSize,this.inputChannels,this.outputChannels,this.userData2);
		const input = inputList[0];
		const output = outputList[0];
		if (this.outputChannels > 0) {
			for (let c = 0; c < this.outputChannels; ++c) {
				var outChannel = output[c];
				for (let i = 0, j = c; i < this.bufferSize; ++i, j += this.outputChannels) {
					outChannel[i] = HEAPF32.subarray(this.outbuffer >> 2 + this.bufferSize * 2, (this.outbuffer >> 2) + this.bufferSize * this.outputChannels)[j]
				}
			}
		}
		if(this.inputChannels > 0){
			for(let c = 0; c < input.length; ++c){
				var inChannel = input[c];
				for(let i = 0, j = c; i < this.bufferSize; ++i, j += this.inputChannels){
					HEAPF32.subarray(this.inbuffer >> 2, (this.inbuffer >> 2) + this.bufferSize * this.inputChannels)[j] = inChannel[i];
				}
			}
		}  
	return true
	}
  }
  return WasmAudioWorkletProcessor;
}

// Specify a worklet processor that will be used to receive messages to this AudioWorkletGlobalScope.
// We never connect this initial AudioWorkletProcessor to the audio graph to do any audio processing.
class BootstrapMessages extends AudioWorkletProcessor {
  constructor(arg) {
    super();
    // Initialize the global Emscripten Module object that contains e.g. the Wasm Module and Memory objects.
    // After this we are ready to load in the main application JS script, which the main thread will addModule()
    // to this scope.
    globalThis.Module = arg['processorOptions'];
#if !MINIMAL_RUNTIME
    // Default runtime relies on an injected instantiateWasm() function to initialize the Wasm Module.
    globalThis.Module['instantiateWasm'] = (info, receiveInstance) => {
      var instance = new WebAssembly.Instance(Module['wasm'], info);
      receiveInstance(instance, Module['wasm']);
      return instance.exports;
    };
#endif
#if WEBAUDIO_DEBUG
    console.log('AudioWorklet global scope looks like this:');
    console.dir(globalThis);
#endif
    // Listen to messages from the main thread. These messages will ask this scope to create the real
    // AudioWorkletProcessors that call out to Wasm to do audio processing.
    let p = this.port;
    p.onmessage = (msg) => {
      let d = msg.data;
#if MODULARIZE
      // Instantiate the MODULARIZEd Module function, which is stored for us under the special global
      // name AudioWorkletModule in MODULARIZE+AUDIO_WORKLET builds.
      if (globalThis.AudioWorkletModule) {
        AudioWorkletModule(Module); // This populates the Module object with all the Wasm properties
        delete globalThis.AudioWorkletModule; // We have now instantiated the Module function, can discard it from global scope
      }
#endif
      // Register a real AudioWorkletProcessor that will actually do audio processing.
      registerProcessor(d['name'], createWasmAudioWorkletProcessor(d['audioParams']));
#if WEBAUDIO_DEBUG
      console.log(`Registered a new WasmAudioWorkletProcessor "${d['name']}" with AudioParams: ${d['audioParams']}`);
#endif
      // Post a message back telling that we have now registered the AudioWorkletProcessor class.
      // This message does not need to contain any information - just need to let the main thread know that
      // the processor can now be instantiated.
      p.postMessage(0);
    }
  }

  // No-op, not doing audio processing in this processor. It is just for receiving bootstrap messages.
  // However browsers require it to still be present. It should never be called because we never add a
  // node to the graph with this processor, although it does look like Chrome does still call this function.
  process() {
    // keep this function a no-op. Chrome redundantly wants to call this even though this processor is never added to the graph.
  }
};

// Register the dummy processor that will just receive messages.
registerProcessor("message", BootstrapMessages);
