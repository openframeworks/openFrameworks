#include "ofMain.h"
#include "ofxUnitTests.h"
#include "ofAppNoWindow.h"

//bool aprox_eq(const ofSoundBuffer & v1, const ofSoundBuffer & v2){
//    
//    for (size_t i=0; size_t < v1.)
//    
//    return eq;
//}

class ofApp: public ofxUnitTestsApp{
private:
    unsigned channels = 2;
    unsigned numFrames = 10;
    ofSoundBuffer inBuffer;
    ofSoundBuffer outBuffer;
    ofSoundBuffer reference;

    void fillSquareWave(ofSoundBuffer &buf, int waveLength, int dutyLength, int offset)
    {
        int nFrames = buf.getNumFrames();
        int nChans = buf.getNumChannels();
        buf.set(0);

        for (size_t i=0; i<nChans; i++)
        {
            for (int j=0; j < nFrames; j++)
            {
                int k = (j-offset) % waveLength;
                if(std::abs(k) < dutyLength)
                {
                    buf[j*nChans + i] = 1;
                }
            }
        }
    }

public:
    void run(){
        unsigned fromFrame;

        inBuffer.allocate(numFrames, channels);
        fillSquareWave(inBuffer, numFrames, numFrames/2, 0);
        outBuffer.allocate(numFrames, channels);
        reference = inBuffer;
        
        ofLogNotice() << "testing ofSoundBuffer::linearResampleTo";

        // Test 1 - Resample rate = 1, looping = false, fromFrame=0: Buffers should be the same.
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fromFrame = 0;
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [1] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Output should equal input.");

        // Test 1b - with looping
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fromFrame = 0;
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [1b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Output should equal input.");

        // Test 2 - Resample rate = 1, looping = false, fromFrame=256: Buffer should be empty.
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        inBuffer.linearResampleTo(outBuffer, numFrames/2, numFrames, 1.0f, false);
        reference.set(0);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [2] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Output should be empty.");

        // Test 2b - with looping
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fromFrame = numFrames/2;
        fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [2b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Output should be half zeros, followed by ones.");

        // Test 3 - Resample rate = 1, looping = false, fromFrame=1: Buffer should be  ones followed by all 0s.
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fromFrame = 1;
        fillSquareWave(reference, numFrames, numFrames/2-fromFrame, 0);
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [3] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Output should be advanced by 1 frame.");

        // Test 3b - with looping
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fromFrame = 1;
        fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [3b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Output should be rotated by 1 frame.");
       
        // Test 4 - Resample rate = 1, looping = false, fromFrame=halfway-1: Buffer should be 1 followed by all 0s.
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fromFrame = numFrames/2-1; 
        reference.set(0);
        for (size_t i = 0; i<channels; i++)
        {
            reference[i] = 1.0f;
        }        
        inBuffer.linearResampleTo(outBuffer, numFrames/2-1, numFrames, 1.0f, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [4] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Buffer should be 1 followed by all 0s.");

        // Test 4b - with looping
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
        inBuffer.linearResampleTo(outBuffer, numFrames/2-1, numFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [4b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Buffer should be rotated to half way - 1.");

        // Test 5 - Resample rate = 1, looping = false, fromFrame=half way: Buffer should be 1 followed by all 0s.
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        reference.set(0);
        fromFrame = numFrames/2;
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [5] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Buffer should be all 0s.");
        
        // Test 5b - with looping
        outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
        fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [5b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Buffer should be half 0s, half 1s.");
                
        // Test 6 - Different input and output buffer lengths
        size_t additionalOutputSamples = 31;
        size_t test6NumFrames = numFrames+additionalOutputSamples;
        outBuffer.allocate(test6NumFrames, channels); // Add a prime number to mix it up
        reference.allocate(test6NumFrames, channels);
        outBuffer.fillWithNoise();
        reference.set(0);
        for (size_t i = 0; i<channels*numFrames/2; i++)
        {
            reference[i] = 1.0f;
        }     
        fromFrame = 0;
        inBuffer.linearResampleTo(outBuffer, fromFrame, test6NumFrames, 1.0f, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [6] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ", longer output buffer: Buffer should be " + ofToString(channels*numFrames/2) + " ones.");

        // Test 6b
        outBuffer.fillWithNoise();
        fillSquareWave(reference, numFrames, numFrames/2, 0);
        fromFrame = 0;
        inBuffer.linearResampleTo(outBuffer, fromFrame, test6NumFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [6b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ", longer output buffer: Buffer should be " + ofToString(channels*numFrames/2) + " ones.");

        // Test 7 - short output
        size_t test7NumFrames = numFrames-7;
        outBuffer.allocate(test7NumFrames, channels); // Add a prime number to mix it up
        reference.allocate(test7NumFrames, channels);
        outBuffer.fillWithNoise();
        fromFrame = 0;
        fillSquareWave(reference, numFrames, numFrames/2, fromFrame);
        inBuffer.linearResampleTo(outBuffer, fromFrame, test7NumFrames, 1.0f, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [7] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ", shorter output buffer: Buffer should be " + ofToString(channels*numFrames/2) + " ones with additional looped.");

        // Test 7b
        outBuffer.allocate(test7NumFrames, channels);
        reference.allocate(test7NumFrames, channels);
        outBuffer.fillWithNoise();
        fillSquareWave(reference, numFrames, numFrames/2, fromFrame);
        inBuffer.linearResampleTo(outBuffer, fromFrame, test7NumFrames, 1.0f, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [7b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ", shorter output buffer: Buffer should be " + ofToString(channels*numFrames/2) + " ones.");

        // Test 8 - Resampling at half rate
        outBuffer.allocate(numFrames, channels);
        reference.allocate(numFrames, channels);        
        outBuffer.fillWithNoise();
        fromFrame = 0;
        float rate = 0.5f;
        reference.set(1);
        for(size_t i=0; i<channels; i++)
        {
            reference[channels*(numFrames-1)+i] = 0.5;
        }
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, rate, false);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [8] Resample rate = "+ofToString(rate)+", looping = false, fromFrame=" + ofToString(fromFrame));

        // Test 8b - with looping
        outBuffer.fillWithNoise();
        fromFrame = 0;
        rate = 0.5f;
        // reference.set(1.0f);
        inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, rate, true);
        ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [8b] Resample rate = "+ofToString(rate)+", looping = true, fromFrame=" + ofToString(fromFrame));
        
        ofLogNotice() << "end testing";
    }
};

//========================================================================
int main( ){
    ofInit();
    auto window = make_shared<ofAppNoWindow>();
    auto app = make_shared<ofApp>();
    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(window, app);
    return ofRunMainLoop();


}

