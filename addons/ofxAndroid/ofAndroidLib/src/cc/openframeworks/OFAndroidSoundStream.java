package cc.openframeworks;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioRecord.OnRecordPositionUpdateListener;
import android.media.AudioTrack;
import android.media.AudioTrack.OnPlaybackPositionUpdateListener;
import android.media.MediaRecorder;
import android.util.Log;

// NOTE: the OFAndroidSoundStream class is controlled from the JNI layer (ofxAndroidSoundStream.cpp / h)
public class OFAndroidSoundStream extends OFAndroidObject implements Runnable, OnRecordPositionUpdateListener, OnPlaybackPositionUpdateListener{
	
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
		return android.media.AudioTrack.getMinBufferSize(samplerate, outChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
	}
	
	static public int getMinInBufferSize(int samplerate, int nchannels){
		int inChannels = AudioFormat.CHANNEL_IN_STEREO;
		if(nchannels==1){
			inChannels = AudioFormat.CHANNEL_IN_MONO;
		}
		return android.media.AudioRecord.getMinBufferSize(samplerate, inChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
	}
	
	private void setupOut(int nOutputChannels, int sampleRate){
		int outChannels = AudioFormat.CHANNEL_OUT_STEREO;
		numOuts = 2;
		if(nOutputChannels==1){
			outChannels = AudioFormat.CHANNEL_OUT_MONO;
			numOuts = 1;
		}
		
		int minBufferSize = android.media.AudioTrack.getMinBufferSize(sampleRate, outChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
		int outBufferSize = minBufferSize>requestedBufferSize?minBufferSize:requestedBufferSize;
		int bufferSizeInBytes = outBufferSize*2;
		
		outBuffer = new short[outBufferSize*numOuts];

		for(int i=0;i<outBuffer.length;i++){
			outBuffer[i]=0;
		}
		
		oTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, outChannels, 
								AudioFormat.ENCODING_PCM_16BIT, bufferSizeInBytes, AudioTrack.MODE_STREAM);
		
		Log.i("OF","sound output setup with buffersize: " + minBufferSize);
	}
	
	private void setupIn(int nInputChannels, int sampleRate){
		int inChannels = AudioFormat.CHANNEL_IN_STEREO;
		numIns = 2;
		if(nInputChannels==1){
			inChannels = AudioFormat.CHANNEL_IN_MONO;
			numIns = 1;
		}
		
		int minBufferSize = android.media.AudioRecord.getMinBufferSize(sampleRate, inChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
		int inBufferSize = requestedBufferSize;//minBufferSize>requestedBufferSize?minBufferSize:requestedBufferSize;
		int bufferSizeInBytes = (minBufferSize>requestedBufferSize?minBufferSize:requestedBufferSize)*2;
		
		iTrack = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, inChannels, AudioFormat.ENCODING_PCM_16BIT, bufferSizeInBytes);
		
		
		inBuffer = new short[inBufferSize*numIns];
		
		Log.i("OF","sound input setup with buffersize: " + minBufferSize);
	}
	
	public void setup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers){
		Log.i("OF","SoundStream setup");
		if(thread!=null)
			return;
		this.sampleRate = sampleRate;
		this.requestedBufferSize = bufferSize;
		this.requestedBuffers = nBuffers;
		
		if(nOutputChannels>0){
			setupOut(nOutputChannels,sampleRate);
		}
		
		if(nInputChannels>0){
			setupIn(nInputChannels,sampleRate);
		}else{
			Log.i("OF","no input channels");
		}

        if(broadcastReceiver==null){
        	broadcastReceiver = new HeadphonesReceiver();
        	activity.registerReceiver(broadcastReceiver, new IntentFilter(Intent.ACTION_HEADSET_PLUG ));
        }
		
		thread = new Thread(this);
		thread.start();

		
	}
	
	@Override
	public void appStop(){
		threadRunning = false;
		
		if(iTrack!=null){
			iTrack.stop();
		}
		
		try {
			thread.join();
		} catch (InterruptedException e) {
			Log.e("OF", "error finishing audio thread ", e);
		}
		thread = null;
		

		if(oTrack!=null){
			oTrack.stop();
			oTrack.release();
			oTrack = null;
		}
		if(iTrack!=null){
			iTrack.release();
			iTrack = null;
		}
		
	}

	@Override
	protected void appPause() {
		if(broadcastReceiver != null){
			try{
				activity.unregisterReceiver(broadcastReceiver);
			}catch(Exception e){
				
			}finally{
				broadcastReceiver = null;
			}
		}
		appStop();
	}

	@Override
	protected void appResume() {
		if(!started) return;
		
		setup(numOuts,numIns,sampleRate,requestedBufferSize,requestedBuffers);
	}

	
	public void start(){
		if(iTrack!=null && iTrack.getState()!=AudioRecord.STATE_UNINITIALIZED){
			if(iTrack.setPositionNotificationPeriod(inBuffer.length/numIns)!=AudioRecord.SUCCESS){
				Log.e("OF","OFAndroidSoundStream: cannot set callback");
			}else{
				iTrack.setRecordPositionUpdateListener(this);
				iTrack.startRecording();
				onPeriodicNotification(iTrack);
				Log.i("OF","OFAndroidSoundStream: input started");
			}
		}else{
			Log.i("OF","no audio input");
		}

		/*if(oTrack!=null && oTrack.getState()!=AudioTrack.STATE_UNINITIALIZED){
			if(oTrack.setPositionNotificationPeriod(outBuffer.length/numOuts/2)!=AudioTrack.SUCCESS){
				Log.e("OF","cannot set callback");
			}else{
				oTrack.setPlaybackPositionUpdateListener(this);
				oTrack.play();
				onPeriodicNotification(oTrack);
			}
		}*/
		if(oTrack!=null){
			oTrack.play();
		}
		started = true;
	}
	
	public void stop(){
		if(oTrack!=null){
			oTrack.stop();
		}
		if(iTrack!=null){
			iTrack.stop();
		}
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
	
	public void onMarkerReached(AudioRecord recorder) {

	}

	public void onPeriodicNotification(AudioRecord recorder) {
		int samplesRead = 0;
		
		while(samplesRead<inBuffer.length && threadRunning ){
			int err = recorder.read(inBuffer, samplesRead, inBuffer.length - samplesRead);
			if(err==AudioRecord.ERROR_INVALID_OPERATION){
				Log.e("OF","AudioRecord error ERROR_INVALID_OPERATION");
				return;
			}
			if(err==AudioRecord.ERROR_BAD_VALUE){
				Log.e("OF","AudioRecord error ERROR_BAD_VALUE");
				return;
			}
			if(err==0){
				Log.e("OF","AudioRecord error 0 samples read");
				return;
			}
			samplesRead += err;
		}
		if(samplesRead>0) while(audioIn(inBuffer, numIns, samplesRead/numIns)==1);
	}

	public void onMarkerReached(AudioTrack track) {
		// TODO Auto-generated method stub
		
	}

	public void onPeriodicNotification(AudioTrack track) {
		if(audioOut(outBuffer, numOuts, outBuffer.length/numOuts)==0){
			track.write(outBuffer, 0, outBuffer.length);
		}
	}
	
	public void run() {
		android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
		threadRunning = true;
		start();
		while(threadRunning){
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
    			Log.i("OF","Headphones disconnected " + intent.getIntExtra("state",0));
    			headphonesConnected(false);
    		}else{
    			Log.i("OF","Headphones connected " + intent.getIntExtra("state",0));
    			headphonesConnected(true);
    		}
    	}
    	
    }

	
}
