package cc.openframeworks;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioRecord;
import android.media.AudioTrack;
import android.media.MediaRecorder;
import android.util.Log;



public class OFAndroidSoundStream extends OFAndroidObject implements AudioTrack.OnPlaybackPositionUpdateListener, AudioRecord.OnRecordPositionUpdateListener, Runnable{
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
		this.outBufferSize = minBufferSize>bufferSize?minBufferSize:bufferSize;
		
		Log.i("OF","creating out buffer with buffersize: " + this.outBufferSize);
		
		outBuffer = new short[this.outBufferSize*numOuts];

		for(int i=0;i<outBuffer.length;i++){
			outBuffer[i]=0;
		}
		
		oTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, outChannels, AudioFormat.ENCODING_PCM_16BIT, this.outBufferSize*numOuts, AudioTrack.MODE_STREAM);
		
		Log.i("OF","sound output setup with buffersize: " + this.outBufferSize);
	}
	
	private void setupIn(int nInputChannels, int sampleRate, int bufferSize){
		int inChannels = AudioFormat.CHANNEL_IN_STEREO;
		numIns = 2;
		if(nInputChannels==1){
			inChannels = AudioFormat.CHANNEL_IN_MONO;
			numIns = 1;
		}
		int minBufferSize = android.media.AudioRecord.getMinBufferSize(sampleRate, inChannels, AudioFormat.ENCODING_PCM_16BIT)/2;
		this.inBufferSize = minBufferSize>bufferSize?minBufferSize:bufferSize;

		Log.i("OF","creating in buffer with buffersize: " + this.inBufferSize);
		
		inBuffer = new short[this.inBufferSize*numIns];

		for(int i=0;i<inBuffer.length;i++){
			inBuffer[i]=0;
		}
		
		iTrack = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, inChannels, AudioFormat.ENCODING_PCM_16BIT, this.inBufferSize*numIns);
		
		Log.i("OF","sound input setup with buffersize: " + this.inBufferSize);
	}
	
	public void setup(int nOutputChannels, int nInputChannels, int sampleRate, int bufferSize, int nBuffers){
		if(thread!=null)
			return;
		this.sampleRate = sampleRate;
		this.requestedBufferSize = bufferSize;
		this.requestedBuffers = nBuffers;
		
		if(nOutputChannels>0){
			setupOut(nOutputChannels,sampleRate,bufferSize*nBuffers);
		}
		
		if(nInputChannels>0){
			setupIn(nInputChannels,sampleRate,bufferSize*nBuffers);
		}
		
		thread = new Thread(this);
		thread.start();

		
	}
	
	public void start(){
		if(oTrack!=null){
			oTrack.play();
			oTrack.write(outBuffer, 0, outBuffer.length);
		}
		if(iTrack!=null){
			iTrack.startRecording();
			iTrack.read(inBuffer,0, inBuffer.length);
		}
		
		started = true;
	}
	
	@Override
	public void stop(){
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
		if(oTrack!=null){
			oTrack.stop();
		}
		if(iTrack!=null){
			iTrack.stop();
		}
		try {
			thread.join();
		} catch (InterruptedException e) {
			Log.e("OF", "error finishing audio thread ", e);
		}
		thread = null;
	}

	@Override
	protected void resume() {
		if(!started) return;
		switch(state){
		case Paused:
			thread = new Thread(this);
			thread.start();
		case Stopped:
			setup(numOuts,numIns,sampleRate,requestedBufferSize,requestedBuffers);
			break;
		}
	}
	
	private Integer sampleRate;
	private Integer outBufferSize, inBufferSize;
	private Integer requestedBufferSize, requestedBuffers;
	private AudioTrack oTrack;
	private AudioRecord iTrack;
	private short[] outBuffer, inBuffer;
	private Thread thread;
	private int numOuts, numIns;
	private static OFAndroidSoundStream instance;
	private static boolean started;
	
	public void onMarkerReached(AudioTrack arg0) {
		
	}
	
	
	long jni_time=0;
	public void onPeriodicNotification(AudioTrack track) {
		if(audioOut(outBuffer, numOuts, outBufferSize,System.currentTimeMillis(),jni_time)==0){
			jni_time = System.nanoTime();
			track.write(outBuffer, 0, outBuffer.length);
			jni_time = System.nanoTime()-jni_time;
		}
	}

	public void onMarkerReached(AudioRecord recorder) {
		
	}

	public void onPeriodicNotification(AudioRecord recorder) {
		recorder.read(inBuffer, 0, inBuffer.length);
		while(audioIn(inBuffer, numIns, inBufferSize)==1);
	}
	
	public void run() {
		android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
		if(oTrack!=null){
			if(oTrack.setPositionNotificationPeriod(outBufferSize)!=AudioTrack.SUCCESS){
				Log.e("OF","cannot set callback");
			}else{
				oTrack.setPlaybackPositionUpdateListener(this);	
			}
		}
		if(iTrack!=null){
			if(iTrack.setPositionNotificationPeriod(inBufferSize)!=AudioRecord.SUCCESS){
				Log.e("OF","cannot set callback");
			}else{
				iTrack.setRecordPositionUpdateListener(this);	
			}
		}
		start();
	}

    public static native int audioOut(short[] buffer, int numChannels, int bufferSize, long currentTime, long jni_time);
    public static native int audioIn(short[] buffer, int numChannels, int bufferSize);

	
}
