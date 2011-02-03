/*
 * ofxAndroidSoundStream.cpp
 *
 *  Created on: 04/07/2010
 *      Author: arturo
 */

#include <jni.h>
#include "ofBaseApp.h"
#include "ofSoundStream.h"
#include "ofUtils.h"
#include "ofxAndroidUtils.h"
#include "ofSoundUnit.h"
#include <deque>
#include <set>

static short * out_buffer, * in_buffer;
static float * out_float_buffer=NULL, * in_float_buffer=NULL;
static int outBufferSize=0, outChannels=0, inBufferSize=0, inChannels=0;
static ofBaseApp * OFApp;
static long unsigned long			tickCount;
static int	sampleRate;
static int  requestedBufferSize;

// TODO: can we label this better?
static float * working = NULL;

static vector< ofSoundSource* > soundSources;
static vector< ofSoundSink* > soundSinks;



void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA, int _sampleRate, int bufferSize, int nBuffers){
	sampleRate			=  _sampleRate;
	OFApp = OFSA;
	tickCount			=  0;
	requestedBufferSize =  bufferSize;
	if(!ofGetJavaVMPtr()){
		ofLog(OF_LOG_ERROR,"ofSoundStreamSetup: Cannot find java virtual machine");
		return;
	}
	JNIEnv *env;
	if (ofGetJavaVMPtr()->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		ofLog(OF_LOG_ERROR,"Failed to get the environment using GetEnv()");
		return;
	}
	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidSoundStream");

	if(javaClass==0){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream java class");
		return;
	}

	jmethodID soundStreamSingleton = env->GetStaticMethodID(javaClass,"getInstance","()Lcc/openframeworks/OFAndroidSoundStream;");
	if(!soundStreamSingleton){
		ofLog(OF_LOG_ERROR,"cannot find OFAndroidSoundStream singleton method");
		return;
	}
	jobject javaObject = env->CallStaticObjectMethod(javaClass,soundStreamSingleton);
	jmethodID javaSetup = env->GetMethodID(javaClass,"setup","(IIIII)V");
	if(javaObject && javaSetup)
		env->CallVoidMethod(javaObject,javaSetup,nOutputChannels,nInputChannels,sampleRate,bufferSize,nBuffers);
	else
		ofLog(OF_LOG_ERROR, "cannot get OFAndroidSoundStream instance or setup method");
}

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, ofBaseApp * OFSA){
	ofSoundStreamSetup(nOutputChannels,nInputChannels,OFSA,44100,256,2);
}

void ofSoundStreamSetup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers){
	ofSoundStreamSetup(nOutputChannels,nInputChannels,NULL,sampleRate,bufferSize,nBuffers);
}

void ofSoundStreamStop(){

}

void ofSoundStreamStart(){

}

void ofSoundStreamClose(){
}


void ofSoundStreamListDevices(){

}

int walkDSPGraphLookingForOutputBranches();


// Add the given ofSoundSource as an input source for the ofSoundStream system.
void ofSoundStreamAddSoundSource( ofSoundSource* source )
{
	source->setSampleRate( sampleRate );
	soundSources.push_back( source );
	walkDSPGraphLookingForOutputBranches();
}
// Add the given ofSoundSink as a sink for incoming audio data (eg from a microphone)
void ofSoundStreamAddSoundSink( ofSoundSink* sink )
{
	soundSinks.push_back( sink );
}


// Remove the given ofSoundSource
void ofSoundStreamRemoveSoundSource( ofSoundSource* source )
{
	vector<ofSoundSource*>::iterator it = find( soundSources.begin(), soundSources.end(), source );
	if ( it != soundSources.end() )
		soundSources.erase( it );
}
// Remove the given ofSoundSink
void ofSoundStreamRemoveSoundSink( ofSoundSink* sink )
{
	vector<ofSoundSink*>::iterator it = find( soundSinks.begin(), soundSinks.end(), sink );
	if ( it != soundSinks.end() )
		soundSinks.erase( it );
}


// return all sound sources
vector<ofSoundSource*> ofSoundStreamGetAllSoundSources() {
	return soundSources;
}


long unsigned long ofSoundStreamGetCurrentTick() {
	return tickCount;
}


// Walk up the tree looking for branches on output, outputting a message for each. Return
// the number of branches found, or 0 if none found.
int walkDSPGraphLookingForOutputBranches()
{
	// if a unit is visited more than once, then it is used as an input to multiple ofSoundSinks.
	// set<ofSoundUnit*> visited;
	// map between source nodes and the outputs that they branch output to.
	map<ofSoundUnit*,vector<ofSoundUnit*> > outputs;

	// do a depth-first traversal of the graph
	vector<ofSoundSource*> sources = ofSoundStreamGetAllSoundSources();
	deque<ofSoundSource*> stack;
	set<ofSoundUnit*> visited;
	copy( sources.begin(), sources.end(), back_inserter( stack ) );
	while ( !stack.empty() ) {
		ofSoundUnit* u = stack.back();
		stack.pop_back();
		// already visited?
		if ( visited.find( u ) != visited.end() )
			continue;
		visited.insert( u );

		// store
		// fetch all immediate upstream neighbours
		vector<ofSoundSource*> upstream = u->getInputs();
	//	ofLog(OF_LOG_WARNING, "inputs to '%s' %x:", u->getName().c_str(), u );
		for ( int i=0; i<(int)upstream.size(); i++ ) {
	//		ofLog(OF_LOG_WARNING, " - '%s' %x", upstream[i]->getName().c_str(), upstream[i] );
			// recording u as an output for all upstream branches
			outputs[upstream[i]].push_back( u );
			// also push upstream[i] to the traversal stack
			stack.push_back( upstream[i] );
		}
	}

	// iterate through outputs to find branching outputs
	int branchCount = 0;
	for ( map<ofSoundUnit*,vector<ofSoundUnit*> >::iterator it = outputs.begin(); it != outputs.end(); ++it ) {
		ofSoundUnit* source = (*it).first;
		vector<ofSoundUnit*>& outputs = (*it).second;
		if ( outputs.size() > 1 && source->getName() != OF_SOUND_SOURCE_MULTIPLEXOR_NAME ) {
			ofLog( OF_LOG_ERROR, "ofSoundStream: output branching detected: the following %i units take input from '%s' (%x):",
				  outputs.size(), source->getName().c_str(), source );
			for ( int j=0; j<(int)outputs.size(); j++ ) {
				ofLog( OF_LOG_ERROR, "                 - '%s' (%x)", outputs[j]->getName().c_str(), outputs[j] );
			}
			ofLog( OF_LOG_ERROR, "ofSoundStream: output branching will cause odd behaviour in upstream units (eg, clicks in oscillators, sound advancing too fast); "
				  "please use ofSoundUnitMultiplexor if you want to branch outputs" );
			branchCount++;
		}
	}

	return branchCount;
}


extern "C"{

/*
void
Java_cc_openframeworks_OFAndroidSoundStream_initAudioOutput(JNIEnv*  env, jobject  thiz, jobject javaSoundStream){
	jclass javaClass = env->FindClass("cc.openframeworks.OFAndroidSoundStream");
	if(javaClass!=0){
		javaObject = javaSoundStream;
		javaSetup = env->GetMethodID(javaClass,"setup","(IIIII)V");
	}
	javaEnv = env;
}*/

/*static int time_one_frame = 0;
static int acc_time = 0;
static int num_frames = 0;
static int time_prev_out = 0;*/

jint
Java_cc_openframeworks_OFAndroidSoundStream_audioRequested(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	tickCount++;
	if(!out_float_buffer || numChannels!=outChannels || bufferSize!=outBufferSize){
		if(out_float_buffer) delete[] out_float_buffer;
		out_float_buffer = new float[bufferSize*numChannels];
		outBufferSize = bufferSize;
		outChannels   = numChannels;
	}

	out_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
	if(!out_buffer) return 1;
	if (numChannels > 0) {
		if (working == NULL){
			working = new float[requestedBufferSize*numChannels];
			memset( working, 0, sizeof(float)*requestedBufferSize*numChannels );
		}
		float * out_buffer_ptr = out_float_buffer;
		memset( out_float_buffer, 0, sizeof(float)*bufferSize*numChannels );
		for(int t=0;t<bufferSize/requestedBufferSize;t++){
			if(OFApp){
				OFApp->audioRequested(out_buffer_ptr,requestedBufferSize,numChannels);
			}

			// fetch and add from ofSoundSources
			for ( int i=0; i<(int)soundSources.size(); i++ ) {
				soundSources[i]->audioRequested( working, requestedBufferSize, numChannels );
				// sum
				for ( int j=0; j<requestedBufferSize*numChannels; j++ ) {
					out_buffer_ptr[j] += working[j];
				}
			}
			out_buffer_ptr+=requestedBufferSize;
		}
		//time_one_frame = ofGetSystemTime();
		for(int i=0;i<bufferSize*numChannels;i++){
			short tempf = (out_float_buffer[i] * 32767.5f) - 0.5;
			out_buffer[i]=tempf;//lrintf( tempf - 0.5 );
		}
	}

	env->ReleasePrimitiveArrayCritical(array,out_buffer,0);
	return 0;
}

static float conv_factor = 1/32767.5f;

jint
Java_cc_openframeworks_OFAndroidSoundStream_audioReceived(JNIEnv*  env, jobject  thiz, jshortArray array, jint numChannels, jint bufferSize){
	if(!in_float_buffer || numChannels!=inChannels || bufferSize!=inBufferSize){
		if(in_float_buffer) delete[] in_float_buffer;
		in_float_buffer = new float[bufferSize*numChannels];
		inBufferSize = bufferSize;
		inChannels   = numChannels;
	}
	if(OFApp){
		in_buffer = (short*)env->GetPrimitiveArrayCritical(array, NULL);
		if(!in_buffer) return 1;
		for(int i=0;i<bufferSize*numChannels;i++){
			in_float_buffer[i] = (float(in_buffer[i]) + 0.5) * conv_factor;
		}
		OFApp->audioReceived(in_float_buffer,bufferSize,numChannels);
		env->ReleasePrimitiveArrayCritical(array,in_buffer,0);
	}

	return 0;
}
}
