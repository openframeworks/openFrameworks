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
    unsigned numFrames = 16;
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

    void fillRamp(ofSoundBuffer &buf)
    {

        int nFrames = buf.getNumFrames();
        int nChans = buf.getNumChannels();
        float inc = 1.0f / nFrames;

        for (size_t i=0; i<nChans; i++)
        {
            float x = 0;
            for (int j=0; j < nFrames; j++)
            {
                buf[j*nChans + i] = x;
                x += inc;                
            }
        }
    }

    template <typename T> void substract_vector(std::vector<T>& a, const std::vector<T>& b, std::vector<T>& result)                                                     
    {
        typename std::vector<T>::iterator       it = a.begin();
        typename std::vector<T>::const_iterator it2 = b.begin();
        typename std::vector<T>::iterator       itResult = result.begin();

        while (it < a.end() && it2 < b.end())
        {
            *itResult = *it - *it2;

            it++;   
            it2++;
            itResult++;
        }
    }

    template <typename T> void abs_vector(std::vector<T>& a)
    {
        for(typename std::vector<T>::iterator it = a.begin(); it < a.end(); it++)
        {
            *it = std::abs(*it);
        }
    }

    template <typename T> bool vectorBelowThreshold(std::vector<T>& a, T threshold)
    {
        for(typename std::vector<T>::iterator it = a.begin(); it < a.end(); it++)
        {
            if (*it > threshold)
                return false;
        }
        return true;
    }

    template <typename T> bool isResultWithinTolerance(std::vector<T>& a, const std::vector<T>& b,  T threshold)
    {
        vector<T> errorVec(a.size(), 0);
        substract_vector<T>(a, b, errorVec);
        abs_vector<T>(errorVec);

        ofLogNotice() << "Error Vector: " << ofToString(errorVec);
        return vectorBelowThreshold(errorVec, threshold);
    }

public:
    void run(){

        inBuffer.allocate(numFrames, channels);
        fillSquareWave(inBuffer, numFrames, numFrames/2, 0);
        outBuffer.allocate(numFrames, channels);
        reference = inBuffer;
        
        ofLogNotice() << "testing ofSoundBuffer::linearResampleTo";

        // Test 1 - Resample rate = 1, looping = false, fromFrame=0: Buffers should be the same.
        {
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            unsigned fromFrame = 0;
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, false);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [1] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Output should equal input.");

            // Test 1b - with looping
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [1b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Output should equal input.");
        }

        // Test 2 - Resample rate = 1, looping = false, fromFrame=256: Buffer should be empty.
        {
            unsigned fromFrame = 0;
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
        }
        
        // Test 3 - Resample rate = 1, looping = false, fromFrame=1: Buffer should be  ones followed by all 0s.
        {
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            unsigned fromFrame = 1;
            fillSquareWave(reference, numFrames, numFrames/2-fromFrame, 0);
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, false);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [3] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Output should be advanced by 1 frame.");

            // Test 3b - with looping
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            fromFrame = 1;
            fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [3b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Output should be rotated by 1 frame.");
        }
       
        // Test 4 - Resample rate = 1, looping = false, fromFrame=halfway-1: Buffer should be 1 followed by all 0s.
        {
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            unsigned fromFrame = numFrames/2-1;
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
        }

        // Test 5 - Resample rate = 1, looping = false, fromFrame=half way: Buffer should be 1 followed by all 0s.
        {
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            reference.set(0);
            unsigned fromFrame = numFrames/2;
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, false);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [5] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ": Buffer should be all 0s.");
            
            // Test 5b - with looping
            outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
            fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [5b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Buffer should be half 0s, half 1s.");

            // Test 5c
           outBuffer.fillWithNoise();        // Initialise out buffer with garbage.
           fromFrame = numFrames-1;
           fillSquareWave(reference, numFrames, numFrames/2, -fromFrame);
           inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, 1.0f, true);
           ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [5c] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ": Buffer should be half 0s, half 1s.");
        }

        // Test 6 - Different input and output buffer lengths
        {
            unsigned fromFrame = 0;
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
            inBuffer.linearResampleTo(outBuffer, fromFrame, test6NumFrames, 1.0f, false);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [6] Resample rate = 1, looping = false, fromFrame=" + ofToString(fromFrame) + ", longer output buffer: Buffer should be " + ofToString(channels*numFrames/2) + " ones.");

            // Test 6b
            outBuffer.fillWithNoise();
            fillSquareWave(reference, numFrames, numFrames/2, 0);
            fromFrame = 0;
            inBuffer.linearResampleTo(outBuffer, fromFrame, test6NumFrames, 1.0f, true);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [6b] Resample rate = 1, looping = true, fromFrame=" + ofToString(fromFrame) + ", longer output buffer: Buffer should be " + ofToString(channels*numFrames/2) + " ones.");
        }

        // Test 7 - short output
        {
            size_t test7NumFrames = numFrames-7;
            outBuffer.allocate(test7NumFrames, channels); // Add a prime number to mix it up
            reference.allocate(test7NumFrames, channels);
            outBuffer.fillWithNoise();
            unsigned fromFrame = 0;
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
        }

        // Test 8 - Resampling at half rate
        {
            outBuffer.allocate(numFrames, channels);
            reference.allocate(numFrames, channels);
            outBuffer.fillWithNoise();
            unsigned fromFrame = 0;
            float rate = 0.5f;
            reference.set(1);
            for(size_t i=0; i<channels; i++)
            {   // Last frame is interpolated
                reference[channels*(numFrames-1)+i] = 0.5;
            }
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, rate, false);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [8] Resample rate = "+ofToString(rate)+", looping = false, fromFrame=" + ofToString(fromFrame));

            // Test 8b - with looping
            outBuffer.allocate(numFrames+1, channels);
            reference.allocate(numFrames+1, channels);
            outBuffer.fillWithNoise();
            fromFrame = 0;
            fillSquareWave(reference, numFrames*(1.0f/rate), (numFrames/2)*(1.0f/rate), fromFrame);
            for(size_t i=0; i<channels; i++)
            {  // Last non-zero frame is interpolated
                reference[channels*(numFrames-1)+i] = 0.5;
            }
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames+1, rate, true);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [8b] Resample rate = "+ofToString(rate)+", looping = true, fromFrame=" + ofToString(fromFrame));
        }
        
        // Test 9 - Resampling at twice rate
        {
            outBuffer.allocate(numFrames, channels);
            reference.allocate(numFrames, channels);
            outBuffer.fillWithNoise();
            unsigned fromFrame = 0;
            float rate = 2.0f;
            reference.set(0);
            // Fill first (numFrames/2)/rate with 1s
            for (size_t i=0; i<((size_t)((numFrames/2)/rate))*channels; i++)
            {
                reference[i] = 1.0;
            }
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, rate, false);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [9] Resample rate = "+ofToString(rate)+", looping = false, fromFrame=" + ofToString(fromFrame));
        
            // Test 9b - with looping
            outBuffer.allocate(numFrames+1, channels);
            reference.allocate(numFrames+1, channels);
            outBuffer.fillWithNoise();
            fillSquareWave(reference, numFrames*(1.0f/rate), (numFrames/2)*(1.0f/rate), fromFrame);
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames+1, rate, true);
            ofxTestEq(reference.getBuffer(), outBuffer.getBuffer(), "Test [9b] Resample rate = "+ofToString(rate)+", looping = true, fromFrame=" + ofToString(fromFrame));

            // Test 11 - Arbitrary sample rate
            fillRamp(inBuffer);
            int outBufLen = numFrames-1;
            outBuffer.allocate(outBufLen, channels);
            outBuffer.fillWithNoise();
            reference.allocate(outBufLen, channels);
            fillRamp(reference);
            rate = (float)numFrames/(float)outBufLen;
            inBuffer.linearResampleTo(outBuffer, fromFrame, outBufLen, rate, true);
            bool b = isResultWithinTolerance(outBuffer.getBuffer(), reference.getBuffer(), 1e-5f);
            ofxTestEq(b, true, "Test [11] Resample rate = "+ofToString(rate)+", looping = true, fromFrame=" + ofToString(fromFrame) + ". \nOutput: " + ofToString(outBuffer.getBuffer()));
        }

        // Test 12 - Arbitrary sample rate
        {
            fillRamp(inBuffer);
            size_t outBufLen = numFrames+10;
            outBuffer.allocate(outBufLen, channels);
            outBuffer.fillWithNoise();
            reference.allocate(outBufLen, channels);
            fillRamp(reference);
            for(size_t i=0; i<channels; i++)
            {  // Last non-zero frame is interpolated
                reference[channels*(outBufLen-1)+i] = 0;
            }
            unsigned fromFrame = 0;
            float rate = (float)numFrames/(float)outBufLen; // Last value is 0 when not looping
            inBuffer.linearResampleTo(outBuffer, fromFrame, outBufLen, rate, false);
            bool b = isResultWithinTolerance(outBuffer.getBuffer(), reference.getBuffer(), 1e-5f);
            ofxTestEq(b, true, "Test [12] Resample rate = "+ofToString(rate)+", looping = true, fromFrame=" + ofToString(fromFrame) + ". \nOutput: " + ofToString(outBuffer.getBuffer()));
        }
        
        // Test [13] - Resample sin with phase shift
        {
            unsigned fromFrame = 40;
            numFrames = 64;
            float rate = 1.5;
            inBuffer.allocate(numFrames, 1);
            reference.allocate(numFrames, 1);
            for(size_t i = 0; i < numFrames; i++) {
                //x = sin(2*pi*n*f/Fs)
                inBuffer[i] = 0.5f * sin(TWO_PI*i/(float)numFrames);
                reference[i] = 0.5f * sin(TWO_PI*i*rate/(float)numFrames + TWO_PI*fromFrame/(float)numFrames);
            }
            inBuffer.linearResampleTo(outBuffer, fromFrame, numFrames, rate, true);
            bool b = isResultWithinTolerance(outBuffer.getBuffer(), reference.getBuffer(), 0.001f);
            ofxTestEq(b, true, "Test [13] Resample rate = "+ofToString(rate)+", looping = true, fromFrame=" + ofToString(fromFrame));
        }
        
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

