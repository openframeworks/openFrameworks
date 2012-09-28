/*
 * libaudiodecoder - Native Portable Audio Decoder Library
 * libaudiodecoder API Header File
 * Latest version available at: http://www.oscillicious.com/libaudiodecoder
 *
 * Copyright (c) 2010-2012 Albert Santoni, Bill Good, RJ Ryan  
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire libaudiodecoder license; however, 
 * the Oscillicious community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included aint with the 
 * license above.
 */


#ifndef __AUDIODECODERBASE_H__
#define __AUDIODECODERBASE_H__

#include <string>
#include <vector>

#ifdef _WIN32
#define DllExport   __declspec( dllexport )
#else
#define DllExport
#endif

//Types
typedef float SAMPLE;

//Error codes
#define AUDIODECODER_ERROR -1
#define AUDIODECODER_OK     0


class DllExport AudioDecoderBase
{
    public:
        AudioDecoderBase(const std::string filename);
        virtual ~AudioDecoderBase();

        /** Opens the file for decoding */
        int open() { return 0; };

        /** Seek to a sample in the file */
        int seek(int filepos) { return 0l; };

        /** Read a maximum of 'size' samples of audio into buffer. 
            Samples are always returned as 32-bit floats, with stereo interlacing.
            Returns the number of samples read. */
        int read(int size, const SAMPLE *buffer) { return 0u; };

        /** Get the number of samples in the audio file */
        inline int    numSamples()        const { return m_iNumSamples; };

        /** Get the number of channels in the audio file */
        inline int    channels()          const { return m_iChannels; };

        /** Get the sample rate of the audio file (samples per second) */
        inline int    sampleRate()        const { return m_iSampleRate; };

        /** Get the duration of the audio file (seconds) */
        inline float  duration()          const { return m_fDuration; };

        /** Get the current playback position in samples */
        inline int    positionInSamples() const { return m_iPositionInSamples; };

        /** Get a list of the filetypes supported by the decoder, by extension */
        static std::vector<std::string> supportedFileExtensions()
        { 
            return std::vector<std::string>();
        };

    protected:
        std::string     m_filename;
        int   m_iNumSamples;
        int   m_iChannels;
        int   m_iSampleRate;
        float m_fDuration; // in seconds
        int   m_iPositionInSamples; // in samples;
};

#endif //__AUDIODECODERBASE_H__
