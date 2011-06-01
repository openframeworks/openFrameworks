package cc.openframeworks;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.util.Log;



public class OFAndroidSoundStream extends OFAndroidObject implements Runnable{
	
	boolean threadRunning;
	public OFAndroidSoundStream(){
	}
	
	static public boolean isInitialized(){
		return instance!=null;
	}
	
	static public OFAndroidSoundStream getInstance(){
		if(instance==null)
			instance = new OFAndroidSoundStream();
		return instance;
	}
	
	static public int getMinOutBufferSize(int samplerate, int nchannels){
		int outChannels = AudioFormat.CHANNEL_OUT_STEREO;
		if(nchannels==1){
			outChannels = AudioFormat.CHANNEL_OUT_MONO;
		}
		int minBuff = android.media.AudioTrack.getMinBufferSize(samplerate, outChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
		Log.i("OF","min buffer size: " + minBuff);
		return minBuff;
	}
	
	static public int getMinInBufferSize(int samplerate, int nchannels){
		int inChannels = AudioFormat.CHANNEL_IN_STEREO;
		if(nchannels==1){
			inChannels = AudioFormat.CHANNEL_IN_MONO;
		}
		return android.media.AudioRecord.getMinBufferSize(samplerate, inChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
	}
	
	private void setupOut(int nOutputChannels, int sampleRate, int bufferSize){
		int outChannels = AudioFormat.CHANNEL_OUT_STEREO;
		numOuts = 2;
		if(nOutputChannels==1){
			outChannels = AudioFormat.CHANNEL_OUT_MONO;
			numOuts = 1;
		}
		
		int minBufferSize = android.media.AudioTrack.getMinBufferSize(sampleRate, outChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
		int outBufferSize = minBufferSize>requestedBufferSize?minBufferSize:requestedBufferSize;
		
		outBuffer = new short[requestedBufferSize*numOuts];

		for(int i=0;i<outBuffer.length;i++){
			outBuffer[i]=0;
		}
		
		oTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, outChannels, AudioFormat.ENCODING_PCM_16BIT, outBufferSize*numOuts, AudioTrack.MODE_STREAM);
		
		Log.i("OF","sound output setup with buffersize: " + minBufferSize);
	}
	
	private void setupIn(int nInputChannels, int sampleRate, int bufferSize){
		int inChannels = AudioFormat.CHANNEL_IN_STEREO;
		numIns = 2;
		if(nInputChannels==1){
			inChannels = AudioFormat.CHANNEL_IN_MONO;
			numIns = 1;
		}
		
		int minBufferSize = android.media.AudioRecord.getMinBufferSize(sampleRate, inChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
		int inBufferSize = minBufferSize>requestedBufferSize?minBufferSize:requestedBufferSize;
		
		iTrack = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, inChannels, AudioFormat.ENCODING_PCM_16BIT, inBufferSize*numIns);
		
		
		inBuffer = new short[requestedBufferSize*numIns];

		for(int i=0;i<inBuffer.length;i++){
			inBuffer[i]=0;
		}
		
		Log.i("OF","sound input setup with buffersize: " + minBufferSize);
	}
	
	public void setup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers){
		if(thread!=null)
			return;
		this.sampleRate = sampleRate;
		this.requestedBufferSize = bufferSize;
		this.requestedBuffers = nBuffers;
		
		if(nOutputChannels>0){
			setupOut(nOutputChannels,sampleRate,bufferSize);
		}
		
		if(nInputChannels>0){
			setupIn(nInputChannels,sampleRate,bufferSize);
		}

        
        broadcastReceiver = new HeadphonesReceiver();
		activity.registerReceiver(broadcastReceiver, new IntentFilter(Intent.ACTION_HEADSET_PLUG ));
		
		thread = new Thread(this);
		thread.start();

		
	}
	
	public void start(){
		if(oTrack!=null && oTrack.getState()!=AudioTrack.STATE_UNINITIALIZED){
			oTrack.play();
		}
		if(iTrack!=null && iTrack.getState()!=AudioRecord.STATE_UNINITIALIZED){
			iTrack.startRecording();
		}
		
		started = true;
	}
	
	@Override
	public void stop(){

		threadRunning = false;
		try {
			thread.join();
		} catch (InterruptedException e) {
			Log.e("OF", "error finishing audio thread ", e);
		}
		thread = null;
		
		if(oTrack!=null){
			oTrack.release();
			oTrack = null;
		}
		if(iTrack!=null){
			iTrack.release();
			iTrack = null;
		}
	}

	@Override
	protected void pause() {
		stop();
	}

	@Override
	protected void resume() {
		if(!started) return;
		
		setup(numOuts,numIns,sampleRate,requestedBufferSize,requestedBuffers);
	}
	
	private Integer sampleRate;
	private Integer requestedBufferSize, requestedBuffers;
	private AudioTrack oTrack;
	private AudioRecord iTrack;
	private short[] outBuffer, inBuffer;
	private Thread thread;
	private int numOuts, numIns;
	private static OFAndroidSoundStream instance;
	private static boolean started;
	
	
	public void run() {
		android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
		threadRunning = true;

		start();
		while(threadRunning){
			if(numIns>0){
				int samplesRead = iTrack.read(inBuffer,0, requestedBufferSize*numIns);
				if(samplesRead>0) audioIn(inBuffer, numIns, samplesRead/numIns);
			}
			if(numOuts>0 && audioOut(outBuffer, numOuts, requestedBufferSize)==0){
				oTrack.write(outBuffer, 0, requestedBufferSize*numOuts);
			}
		}
	}

    public static native int audioOut(short[] buffer, int numChannels, int bufferSize);
    public static native int audioIn(short[] buffer, int numChannels, int bufferSize);
    public static native void headphonesConnected(boolean connected);

	HeadphonesReceiver broadcastReceiver;

    class HeadphonesReceiver extends BroadcastReceiver{

    	@Override
    	public void onReceive(Context context, Intent intent) {
    		
    		if(intent.getIntExtra("state",0)==0){
    			Log.i("OF","Headphones disconnected" + intent.getStringExtra("state"));
    			headphonesConnected(false);
    		}else{
    			Log.i("OF","Headphones connected" + intent.getStringExtra("state"));
    			headphonesConnected(true);
    		}
    	}
    	
    }
	
}
