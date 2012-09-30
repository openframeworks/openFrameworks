/**
 * \file audiodecodermediafoundation.h
 * \class AudioDecoderMediaFoundation
 * \brief Decodes MPEG4/AAC audio using the SourceReader interface of the
 * Media Foundation framework included in Windows 7.
 * \author Bill Good <bkgood at gmail dot com>
 * \author Albert Santoni <alberts at mixxx dot org>
 * \date Jan 10, 2011
 */


#ifndef AUDIODECODERMEDIAFOUNDATION_H
#define AUDIODECODERMEDIAFOUNDATION_H

#include "audiodecoderbase.h"

//Force MSVC to generate a .lib file with /implib but without a .def file
//http://msdn.microsoft.com/en-us/library/67wc07b9(v=vs.80).aspx
DllExport int AudioDecoderMediaFoundation = 1;

class IMFSourceReader;
class IMFMediaType;
class IMFMediaSource;

#define SHORT_SAMPLE short

class DllExport AudioDecoderMediaFoundation : public AudioDecoderBase {
  public:
    AudioDecoderMediaFoundation(const std::string filename);
    ~AudioDecoderMediaFoundation();
    int open();
    int seek(int sampleIdx);
    int read(int size, const SAMPLE *buffer);
    inline int numSamples();
    std::vector<std::string> supportedFileExtensions();

  private:
    bool configureAudioStream();
    bool readProperties();
    void copyFrames(short *dest, size_t *destFrames, const short *src,
        size_t srcFrames);
    inline double secondsFromMF(__int64 mf);
    inline __int64 mfFromSeconds(double sec);
    inline __int64 frameFromMF(__int64 mf);
    inline __int64 mfFromFrame(__int64 frame);
    IMFSourceReader *m_pReader;
    IMFMediaType *m_pAudioType;
    wchar_t *m_wcFilename;
    int m_nextFrame;
    short *m_leftoverBuffer;
    size_t m_leftoverBufferSize;
    size_t m_leftoverBufferLength;
    int m_leftoverBufferPosition;
    __int64 m_mfDuration;
    long m_iCurrentPosition;
    bool m_dead;
    bool m_seeking;
	unsigned int m_iBitsPerSample;
	SHORT_SAMPLE m_destBufferShort[8192];
};

#endif // ifndef AUDIODECODERMEDIAFOUNDATION_H
