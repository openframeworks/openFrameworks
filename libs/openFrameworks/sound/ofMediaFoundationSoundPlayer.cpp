
#include "ofMediaFoundationSoundPlayer.h"
#include "ofLog.h"

#include <condition_variable>
#include <propvarutil.h>
#include <xaudio2.h>
#include <mmreg.h>

// standard speaker geometry configurations, used with X3DAudioInitialize
#if !defined(SPEAKER_MONO)
#define SPEAKER_MONO             SPEAKER_FRONT_CENTER
#define SPEAKER_STEREO           (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
#define SPEAKER_2POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY)
#define SPEAKER_SURROUND         (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_BACK_CENTER)
#define SPEAKER_QUAD             (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define SPEAKER_4POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define SPEAKER_5POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define SPEAKER_7POINT1          (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_FRONT_LEFT_OF_CENTER | SPEAKER_FRONT_RIGHT_OF_CENTER)
#define SPEAKER_5POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_SIDE_LEFT  | SPEAKER_SIDE_RIGHT)
#define SPEAKER_7POINT1_SURROUND (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT | SPEAKER_FRONT_CENTER | SPEAKER_LOW_FREQUENCY | SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT | SPEAKER_SIDE_LEFT  | SPEAKER_SIDE_RIGHT)
#endif

using namespace Microsoft::WRL;

int ofMediaFoundationSoundPlayer::sNumInstances = 0;

ComPtr<IXAudio2> ofMediaFoundationSoundPlayer::sXAudio2 = nullptr;
std::shared_ptr<IXAudio2MasteringVoice> ofMediaFoundationSoundPlayer::sXAudioMasteringVoice;


int ofMediaFoundationUtils::sNumMFInstances = 0;
// media foundation utils
//----------------------------------------------
bool ofMediaFoundationUtils::InitMediaFoundation() {
    if (sNumMFInstances == 0) {
        HRESULT hr = MFStartup(MF_VERSION);
        // TODO: This is called by ofMediaFoundationPlayer also
        // uses CoInitializeEx
        // using the coinit in case ofDirectShowPlayer is also being used
        hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        //hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        //HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED | COINIT_DISABLE_OLE1DDE);
        ofLogVerbose("ofMediaFoundationUtils :: CoInitializeEx : init ok ") << SUCCEEDED(hr);
    }
    sNumMFInstances++;
    return sNumMFInstances > 0;
}

//----------------------------------------------
bool ofMediaFoundationUtils::CloseMediaFoundation() {
    sNumMFInstances--;
    HRESULT hr = S_OK;
    if (sNumMFInstances <= 0) {
        CoUninitialize();
        ofLogVerbose("ofMediaFoundationUtils") << " calling MFShutdown.";
        // Shut down Media Foundation.
        hr = MFShutdown();
    }
    if (sNumMFInstances < 0) {
        sNumMFInstances = 0;
    }
    return (hr == S_OK);
}

//----------------------------------------------
int ofMediaFoundationUtils::GetNumInstances() {
    return sNumMFInstances;
}

//----------------------------------------------
void ofMediaFoundationUtils::CallAsyncBlocking(std::function<void()> aCallBack) {
    std::mutex lock;
    std::condition_variable wait;
    std::atomic_bool isDone(false);

    HRESULT hr = S_OK;

    ComPtr<AsyncCallback> pCB(
        new AsyncCallback(
            [&] {
        aCallBack();
        isDone.store(true);
        wait.notify_one();
    }
    ));

    hr = MFPutWorkItem(MFASYNC_CALLBACK_QUEUE_MULTITHREADED, pCB.Get(), NULL);
    if (hr == S_OK) {
        std::unique_lock lk{ lock };
        wait.wait(lk, [&] { return isDone.load(); });
    } else {
        aCallBack();
        if (pCB) {
            pCB->Release();
            pCB = nullptr;
        }
    }
}

//----------------------------------------------
void ofMediaFoundationSoundPlayer::SetMasterVolume(float apct) {
    sInitAudioSystems();
    if (sXAudioMasteringVoice) {
        sXAudioMasteringVoice->SetVolume(std::clamp(apct, 0.f, 1.0f));
    }
}


//----------------------------------------------
bool ofMediaFoundationSoundPlayer::sInitXAudio2() {

    if (sXAudio2 == nullptr) {
        UINT32 flags = 0;
#if defined(TARGET_WINVS)
        HRESULT hr = XAudio2Create(sXAudio2.GetAddressOf(), 0U);
#else
        HRESULT hr = XAudio2Create(sXAudio2.GetAddressOf(), 0U, XAUDIO2_DEFAULT_PROCESSOR );
#endif
        if (sXAudio2) {
            sXAudio2->StartEngine();
        }
        if (hr == S_OK) {
            IXAudio2MasteringVoice* pMVoice = sXAudioMasteringVoice.get();
            hr = sXAudio2->CreateMasteringVoice(&pMVoice);
            if (hr != S_OK) {
                ofLogError("ofMediaFoundationSoundPlayer :: sInitXAudio2") << " error creating master voice.";
                sCloseXAudio2();
                return false;
            }
        }
    }
    return true;
}

//----------------------------------------------
bool ofMediaFoundationSoundPlayer::sCloseXAudio2() {

    if (sXAudioMasteringVoice) {
        sXAudioMasteringVoice->DestroyVoice();
        sXAudioMasteringVoice.reset();
    }

    if (sXAudio2 != nullptr) {
       sXAudio2->StopEngine();
       sXAudio2.Reset();
    }

    sXAudio2 = nullptr;
    return true;
}



//----------------------------------------------
bool ofMediaFoundationSoundPlayer::sInitAudioSystems() {
    ofMediaFoundationUtils::InitMediaFoundation();
    if (sNumInstances == 0) {
        sInitXAudio2();
    }
    sNumInstances++;
    return sNumInstances > 0;
}

//----------------------------------------------
void ofMediaFoundationSoundPlayer::sCloseAudioSystems() {
    sNumInstances--;
    if (sNumInstances <= 0) {
        ofLogVerbose("ofMediaFoundationSoundPlayer") << " closing XAudio2.";
        sCloseXAudio2(); 
    }
    ofMediaFoundationUtils::CloseMediaFoundation();
    if (sNumInstances < 0) {
        sNumInstances = 0;
    }
}

ofMediaFoundationSoundPlayer::ofMediaFoundationSoundPlayer() {
    InitializeCriticalSectionEx(&m_critSec, 0, 0);
    sInitAudioSystems();
}
ofMediaFoundationSoundPlayer::~ofMediaFoundationSoundPlayer() {
    unload();
    sCloseAudioSystems();
    DeleteCriticalSection(&m_critSec);
}

//--------------------
bool ofMediaFoundationSoundPlayer::load(const of::filesystem::path& fileName, bool stream) {
    unload();
    
    std::string fileStr = fileName.string();
    bool bStream = false;
    bStream = bStream || ofIsStringInString(fileStr, "http://");
    bStream = bStream || ofIsStringInString(fileStr, "https://");
    bStream = bStream || ofIsStringInString(fileStr, "rtsp://");
    bStream = bStream || ofIsStringInString(fileStr, "rtmp://");

    of::filesystem::path absPath{ fileStr };

    if (!bStream) {
        if (ofFile::doesFileExist(absPath)) {
            absPath = ofFilePath::getAbsolutePath(absPath, true);
        } else {
            ofLogError("ofMediaFoundationSoundPlayer") << " file does not exist! " << absPath;
            return false;
        }
    }

    mBStreaming = (bStream || stream);

    ComPtr<IMFAttributes> attributes;
    HRESULT hr = MFCreateAttributes(&attributes, mBStreaming ? 2 : 1);
    //hr = attributes->SetUINT32(MF_LOW_LATENCY, TRUE);
    if (mBStreaming) {
        #if defined MF_LOW_LATENCY
        hr = attributes->SetUINT32(MF_LOW_LATENCY, TRUE);
        #endif
        mSrcReaderCallback = std::make_shared< SourceReaderCallback>();
        mSrcReaderCallback->setCB(this);
        hr = attributes->SetUnknown(MF_SOURCE_READER_ASYNC_CALLBACK, mSrcReaderCallback.get());
    }


    LPCWSTR path = absPath.c_str();
    

    hr = MFCreateSourceReaderFromURL(
        path,
        attributes.Get(),
        mSrcReader.GetAddressOf());

    if (hr != S_OK) {
        ofLogError("ofMediaFoundationSoundPlayer::load") << " unable to load from: " << absPath;
        unload();
        return false;
    }

    ofLogVerbose("ofMediaFoundationSoundPlayer::load") << " created the source reader " << absPath;
    // Select only the audio stream
    hr = mSrcReader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, false);
    if (hr == S_OK) {
        hr = mSrcReader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true);
    }

    if (hr != S_OK) {
        unload();
        return false;
    }


    IMFMediaType* nativeType;
    WAVEFORMATEX* nativeFormat;
    UINT32 formatSize;
    hr = mSrcReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &nativeType);

    auto nativeTypePtr = std::unique_ptr<IMFMediaType, MyComDeleterFunctor>(nativeType);
    // get a wave format 
    hr = MFCreateWaveFormatExFromMFMediaType(nativeType, &nativeFormat, &formatSize);
    
    mNumChannels = nativeFormat->nChannels;
    mSampleRate = nativeFormat->nSamplesPerSec;

    CoTaskMemFree(nativeFormat);
    

    ComPtr<IMFMediaType> mediaType;
    hr = MFCreateMediaType(mediaType.GetAddressOf());
    if (hr != S_OK) {
        return false;
    }

    hr = mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    if (hr != S_OK) {
        return false;
    }

    hr = mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    if (FAILED(hr)) {
        return false;
    }

    hr = mSrcReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, mediaType.Get() );
    if (hr != S_OK) {
        return false;
    }

    ComPtr<IMFMediaType> outputMediaType;
    hr = mSrcReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, outputMediaType.GetAddressOf());
    if (hr != S_OK) {
        return false;
    }


    UINT32 waveFormatSize = 0;
    WAVEFORMATEX* waveFormat = nullptr;
    hr = MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &waveFormatSize);
    if (hr != S_OK) {
        return false;
    }

    memcpy_s(&mWaveFormatEx, sizeof(mWaveFormatEx), waveFormat, waveFormatSize);
    ofLogVerbose("ofMediaFoundationSoundPlayer::load") << "waveFormatEx num channels: " << mWaveFormatEx.nChannels << std::endl;

    if (!mBStreaming) {
        _readToBuffer(mSrcReader.Get());
    }

    CoTaskMemFree(waveFormat);



    if (hr != S_OK) {
        unload();
        return false;
    }

    // get seconds:
    if (mBStreaming) {
        PROPVARIANT durationProp;
        hr = mSrcReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &durationProp);
        if (hr == S_OK) {
            mDurationSeconds = (double)durationProp.uhVal.QuadPart / 10000000.0;
            mDurationMS = (double)durationProp.uhVal.QuadPart / 10000.0;
            ofLogVerbose("ofMediaFoundationSoundPlayer::load") << "Reader duration seconds: " << (double)durationProp.uhVal.QuadPart / 10000000.0 << " millis: " << mDurationMS << std::endl;
        }
    }

    PROPVARIANT seekProp;
    hr = mSrcReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_SOURCE_READER_MEDIASOURCE_CHARACTERISTICS, &seekProp);
    if (hr == S_OK) {
        ULONG flags = seekProp.ulVal;
        mBCanSeek = ((flags & MFMEDIASOURCE_CAN_SEEK) == MFMEDIASOURCE_CAN_SEEK);
        ofLogVerbose("ofMediaFoundationSoundPlayer::load") << "mBCanSeek: " << mBCanSeek << std::endl;
    }

    ofLogVerbose("ofMediaFoundationSoundPlayer::load") << "made it all the way to the end.";
    
    if (!mBStreaming) {
        mSrcReader.Reset();
        mSrcReader = nullptr;
    }

    {

        // create stream context for listening to voice 
        mVoiceContext = std::make_shared<StreamingVoiceContext>();
        // Create the source voice
        IXAudio2SourceVoice* pSourceVoice = nullptr;
        // setting max freq ratio to 3, though it may need to be higher to play at a 
        // faster pitch
        if (mBStreaming) {
            hr = sXAudio2->CreateSourceVoice(
                &pSourceVoice,
                &mWaveFormatEx,
                0U, 3.0f,
                mVoiceContext.get()
            );
        } else {
            hr = sXAudio2->CreateSourceVoice(&pSourceVoice, &mWaveFormatEx);
        }
        if (hr != S_OK) {
            ofLogError("ofMediaFoundationSoundPlayer :: load") << "error creating voice. hr: " << hr;
            unload();
            return false;
        }

        mVoice.reset(pSourceVoice);
    }

    if (hr != S_OK) {
        unload();
        return false;
    }

    mBLoaded = true;

    return mBLoaded;
};

//--------------------
void ofMediaFoundationSoundPlayer::unload() {
    removeUpdateListener();

    EnterCriticalSection(&m_critSec);

    if (mSrcReader) {
        {
            std::unique_lock<std::mutex> lk(mSrcReaderMutex);
            mBRequestNewReaderSample = false;
        }

        ofMediaFoundationUtils::CallAsyncBlocking(
            [&] {
                //mSrcReader->Flush(MF_SOURCE_READER_FIRST_AUDIO_STREAM);
                mSrcReader.Reset();
            }
        );
    }
    mSrcReader = nullptr;

    if (mSrcReaderCallback) {
        ofMediaFoundationUtils::CallAsyncBlocking(
            [&] { mSrcReaderCallback.reset(); }
        );
    }

    if (mVoice) {
        mVoice.reset();
    }

    if (mVoiceContext) {
        mVoiceContext.reset();
    }


    mBStreaming = false;

    _clearExtraVoices();

    mStreamBuffers.clear();
    mBuffer.clear();
	mPosPct = 0.f;
	mBIsPlaying = false;
	mBLoaded = false;
    mBCanSeek = false;
    mDurationSeconds = 0.f;
    mDurationMS = 0;

    mTotalNumFrames = 0;
    mNumSamplesAlreadyPlayed = 0;
    mBRequestNewReaderSample = false;
    
    LeaveCriticalSection(&m_critSec);
};

//--------------------
void ofMediaFoundationSoundPlayer::update(ofEventArgs& args) {
    if (mVoice) {
        if (mBStreaming) {
            if (isPlaying()) {
                XAUDIO2_VOICE_STATE xstate;
                mVoice->GetState(&xstate);
                mNumSamplesAlreadyPlayed += xstate.SamplesPlayed - mNumSamplesStored;
                double seconds = (double)mNumSamplesAlreadyPlayed / (double)mSampleRate;
                mPosPct = seconds / mDurationSeconds;
                mNumSamplesStored = xstate.SamplesPlayed;
                //std::cout << "SamplesPlayed: " << SamplesPlayed << " - " << SamplesPlayed / mSampleRate << " -- " << seconds << " / " << mDurationSeconds << " pos: " << mPosPct << std::endl;

                {
                    bool bRequestStop = false;
                    {
                        std::unique_lock<std::mutex> lk(mSrcReaderMutex);
                        if (mBEndOfStream) {
                            if (!xstate.BuffersQueued) {
                                if (mBLoop) {
                                    mBEndOfStream = false;
                                    if (mVoice) {
                                        mNumSamplesAlreadyPlayed = 0;
                                        // Restart loop
                                        PROPVARIANT var = {};
                                        var.vt = VT_I8;
                                        HRESULT hr = mSrcReader->SetCurrentPosition(GUID_NULL, var);
                                        hr = PropVariantClear(&var);
                                    }
                                    mBRequestNewReaderSample = true;
                                } else {
                                    bRequestStop = true;
                                    // we need to request stop outside of the scope of the lock
                                    // since stop() also locks to set vars 
                                }
                            }
                        }
                    }
                    if (bRequestStop) {
                        stop();
                    }
                }

                if (mSrcReader) {
                    std::unique_lock<std::mutex> lk(mSrcReaderMutex);
                    if (mBRequestNewReaderSample) {
                        mBRequestNewReaderSample = false;
                        HRESULT hr = mSrcReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, nullptr, nullptr, nullptr);
                    }
                }
            }
        } else {
            if (isPlaying()) {
                XAUDIO2_VOICE_STATE xstate;
                mVoice->GetState(&xstate);
                mNumSamplesAlreadyPlayed += xstate.SamplesPlayed - mNumSamplesStored;
                double seconds = (double)mNumSamplesAlreadyPlayed / (double)mSampleRate;
                mPosPct = seconds / mDurationSeconds;
                mNumSamplesStored = xstate.SamplesPlayed;

                if (!xstate.BuffersQueued && mExtraVoices.size() < 1) {
                    // we have reached the end //
                    if (mBLoop) {
                        // set isPlaying to false, so that it will create a new instance 
                        // for mVoice and not an Extra Voice
                        mPosPct = 0.0f;
                        mBIsPlaying = false;
                        play();
                    } else {
                        stop();
                    }
                }
            }
        }
    }

    for (auto it = mExtraVoices.begin(); it != mExtraVoices.end(); ) {
        XAUDIO2_VOICE_STATE xstate;
        it->second->GetState(&xstate, XAUDIO2_VOICE_NOSAMPLESPLAYED);
        if (!xstate.BuffersQueued) {
            std::ignore = it->second->Stop(0);
            it->second->DestroyVoice();
            it = mExtraVoices.erase(it);
        } else {
            ++it;
        }
    }
}

//--------------------
void ofMediaFoundationSoundPlayer::play() {
    if (!mVoice) {
        ofLogError("ofMediaFoundationSoundPlayer :: play") << "error creating sXAudio2.";
        return;
    }

    // don't want a ton of loops going on here 
    if (mBLoop) {
        stop();
    }
    if (!mBMultiPlay) {
        stop();
    }

    if (mBStreaming) {
        // just in case, multiplay is not supported for streams 
        _clearExtraVoices();
    }

    if (mBMultiPlay && isPlaying()) {
        unsigned int voiceKey = 0;
        // get next available key //
        if (mExtraVoices.size() > 0) {
            bool bOkGotAGoodOne = false;
            int numAttempts = 0;
            while (!bOkGotAGoodOne && (numAttempts < 1024)) {
                bool bOneIsAlreadyKey = false;
                for (auto& it : mExtraVoices) {
                    if (it.first == voiceKey) {
                        bOneIsAlreadyKey = true;
                        break;
                    }
                }
                if (!bOneIsAlreadyKey) {
                    bOkGotAGoodOne = true;
                } else {
                    voiceKey++;
                    numAttempts++;
                }
            }

            if (!bOkGotAGoodOne) {
                ofLogError("ofMediaFoundationSoundPlayer::play") << " error finding key for multi play";
                return;
            }
        }

        IXAudio2SourceVoice* pSourceVoice;
        HRESULT hr = sXAudio2->CreateSourceVoice(&pSourceVoice, &mWaveFormatEx);
        if (hr != S_OK) {
            ofLogError("ofMediaFoundationSoundPlayer::play") << " error creating extra multi play sounds.";
            return;
        }

        // ok, now lets add some extra players //
        // Submit the wave sample data using an XAUDIO2_BUFFER structure
        XAUDIO2_BUFFER buffer = {};
        buffer.pAudioData = mBuffer.data();
        // tell the source voice not to expect any data after this buffer
        buffer.Flags = XAUDIO2_END_OF_STREAM;  
        buffer.AudioBytes = mBuffer.size();

        hr = pSourceVoice->SubmitSourceBuffer(&buffer);
        pSourceVoice->SetVolume(mVolume);
        pSourceVoice->SetFrequencyRatio(mSpeed);
        _setPan(pSourceVoice, mPan);
        mExtraVoices.emplace_back(std::make_pair(voiceKey, pSourceVoice));
        //mExtraVoices.push_back(uptr);
        hr = pSourceVoice->Start(0);
    } else {

        if (mBStreaming && mSrcReader) {
            mBRequestNewReaderSample = true;
        } else {
            XAUDIO2_BUFFER buffer = {};
            buffer.pAudioData = mBuffer.data();
            // tell the source voice not to expect any data after this buffer
            buffer.Flags = XAUDIO2_END_OF_STREAM;
            buffer.AudioBytes = mBuffer.size();

            mVoice->SubmitSourceBuffer(&buffer);
           
        }

        mVoice->SetVolume(mVolume);
        mVoice->SetFrequencyRatio(mSpeed);
        _setPan(mVoice.get(), mPan);

        mVoice->Start(0);
        mNumSamplesAlreadyPlayed = 0;
    }
    addUpdateListener();

    mBIsPlaying = true;
};

//--------------------
void ofMediaFoundationSoundPlayer::stop() {
    _clearExtraVoices();

    if (mBStreaming && mSrcReader) {
        ofMediaFoundationUtils::CallAsyncBlocking(
            [&] { mSrcReader->Flush(MF_SOURCE_READER_FIRST_AUDIO_STREAM); }
        );
    }

    if (mVoice) {
        mVoice->Stop();
        mVoice->FlushSourceBuffers();
    }
    {
        std::unique_lock<std::mutex> lk(mSrcReaderMutex);
        mBEndOfStream = false;
        mBRequestNewReaderSample = false;
    }
    mPosPct = 0.0f;
    mBIsPlaying = false;
    mNumSamplesAlreadyPlayed = 0;
};

//--------------------
void ofMediaFoundationSoundPlayer::setVolume(float vol) {
    mVolume = vol;
    if (mVoice) {
        mVoice->SetVolume(mVolume);
    }
    for (auto& it : mExtraVoices) {
        if (it.second) {
            it.second->SetVolume(mVolume);
        }
    }
};

// https://learn.microsoft.com/en-us/windows/win32/xaudio2/how-to--pan-a-sound
//--------------------
void ofMediaFoundationSoundPlayer::setPan(float apan) {
    if (!sXAudioMasteringVoice) {
        return;
    }

    apan = std::clamp(apan, -1.f, 1.f);

    if (mPan == apan) {
        return;
    }

    _setPan(mVoice.get(), apan);

    mPan = apan;
};

//--------------------
void ofMediaFoundationSoundPlayer::setSpeed(float spd) {
    if (mVoice) {
        mVoice->SetFrequencyRatio(spd);
    }
    mSpeed = spd;
};

//--------------------
void ofMediaFoundationSoundPlayer::setPaused(bool bP) {
    if (bP) {
        if (mVoice) {
            mVoice->Stop();
        }
        for (auto& it : mExtraVoices) {
            if (it.second) {
                it.second->Stop();
            }
        }
    } else {
        if (mVoice) {
            mVoice->Start();
        }

        for (auto& it : mExtraVoices) {
            if (it.second) {
                it.second->Start();
            }
        }
    }
    mBIsPlaying = !bP;
};

//--------------------
void ofMediaFoundationSoundPlayer::setLoop(bool bLp) {
    if (bLp) {
        // we don't want a lot of looping iterations 
        _clearExtraVoices();
    }
    mBLoop = bLp;
};

//--------------------
void ofMediaFoundationSoundPlayer::setMultiPlay(bool bMp) {
    if (mBStreaming) {
        ofLogWarning("ofMediaFoundationSoundPlayer::setMultiPlay") << "multiplay not supported for streams.";
        mBMultiPlay = false;
        return;
    }
    if (!mBMultiPlay) {
        _clearExtraVoices();
    }
    mBMultiPlay = bMp;
};

//--------------------
void ofMediaFoundationSoundPlayer::setPosition(float pct) {
    if (isPlaying()) {
        double dpct = static_cast<double>(pct);
        dpct = std::clamp(dpct, 0.0, 1.0);

        if (mBStreaming && !mBCanSeek) {
            ofLogWarning("ofMediaFoundationSoundPlayer::setPosition") << " unable to seek.";
            return;
        }
        
        // ok we need to kill buffers and resubmit a buffer
        if (mVoice) {
            std::ignore = mVoice->Stop();
            std::ignore = mVoice->FlushSourceBuffers();

            if (mBStreaming) {
                if (mSrcReader && mBCanSeek) {
                    std::unique_lock<std::mutex> lk(mSrcReaderMutex);
                    double tseconds = dpct * (double)mDurationSeconds;
                    PROPVARIANT seekVar;
                    HRESULT hr = InitPropVariantFromInt64((LONGLONG)(tseconds * 10000000.0), &seekVar);
                    if (hr == S_OK) {
                        hr = mSrcReader->SetCurrentPosition(GUID_NULL, seekVar);
                        hr = PropVariantClear(&seekVar);
                    }
                    uint32_t desiredSample = (dpct * mDurationSeconds) * (double)mSampleRate;
                    mNumSamplesAlreadyPlayed = desiredSample;

                    mBRequestNewReaderSample = true;
                }
                mVoice->Start();
            } else {
                XAUDIO2_BUFFER buffer = {};
                buffer.pAudioData = mBuffer.data();
                // tell the source voice not to expect any data after this buffer
                buffer.Flags = XAUDIO2_END_OF_STREAM;
                buffer.AudioBytes = mBuffer.size();
                uint32_t desiredSample = dpct * (double)mTotalNumFrames;
                mNumSamplesAlreadyPlayed = desiredSample;
                // First sample in this buffer to be played.
                buffer.PlayBegin = desiredSample;
                mVoice->SubmitSourceBuffer(&buffer);
                mVoice->Start();
            }
        }
    }
};

//--------------------
void ofMediaFoundationSoundPlayer::setPositionMS(int ms) {
    setPosition((double)ms / (double)mDurationMS);
};

//--------------------
float ofMediaFoundationSoundPlayer::getPosition() const { 
	return mPosPct;
};

//--------------------
int ofMediaFoundationSoundPlayer::getPositionMS() const {
	return (double)getPosition() * (double)mDurationMS;
};

//--------------------
bool ofMediaFoundationSoundPlayer::isPlaying() const {
	return mBIsPlaying;
};

//--------------------
float ofMediaFoundationSoundPlayer::getSpeed() const {
	return mSpeed;
};

//--------------------
float ofMediaFoundationSoundPlayer::getPan() const {
	return mPan;
};

//--------------------
bool ofMediaFoundationSoundPlayer::isLoaded() const {
	return mBLoaded;
};

//--------------------
float ofMediaFoundationSoundPlayer::getVolume() const {
	return mVolume;
};

//--------------------
void ofMediaFoundationSoundPlayer::OnSourceReaderEvent(HRESULT hrStatus, DWORD dwStreamIndex,
    DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample) {
    HRESULT hr = S_OK;
    if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
        ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "End of stream";
        // set to the buffer and complete //
        std::unique_lock<std::mutex> lk(mSrcReaderMutex);
        mBEndOfStream = true;
        return;
    }
    if (dwStreamFlags & MF_SOURCE_READERF_NEWSTREAM) {
        ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "New stream";
    }
    if (dwStreamFlags & MF_SOURCE_READERF_NATIVEMEDIATYPECHANGED) {
        ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "Native type changed";
    }
    if (dwStreamFlags & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED) {
        ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "Current type changed";
    }

    if (!pSample) {
        ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "Null audio sample.";
        std::unique_lock<std::mutex> lk(mSrcReaderMutex);
        mBRequestNewReaderSample = true;
    } else {
        EnterCriticalSection(&m_critSec);
        ComPtr<IMFMediaBuffer> mediaBuffer;
        hr = pSample->ConvertToContiguousBuffer(mediaBuffer.GetAddressOf());
        if (hr != S_OK) {
            ofLogError("ofMediaFoundationSoundPlayer::OnSourceReaderEvent : ") << "error converting to contiguous buffer";
            LeaveCriticalSection(&m_critSec);
            std::unique_lock<std::mutex> lk(mSrcReaderMutex);
            mBRequestNewReaderSample = true;
            return;
        }

        BYTE* audioData = nullptr;
        DWORD sampleBufferLength = 0;

        hr = mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength);
        if (hr != S_OK) {
            LeaveCriticalSection(&m_critSec);
            std::unique_lock<std::mutex> lk(mSrcReaderMutex);
            mBRequestNewReaderSample = true;
            return;
        }

        {
            if (mStreamBuffers.size() < MAX_BUFFER_COUNT) {
                mStreamBuffers.resize(MAX_BUFFER_COUNT);
            }
            if (mStreamBuffers[currentStreamBuffer].size() < sampleBufferLength) {
                mStreamBuffers[currentStreamBuffer].assign(sampleBufferLength,0);
            }

            //memcpy(buffers[currentStreamBuffer].data(), audioData, sampleBufferLength * sizeof(BYTE) );
            memcpy_s(mStreamBuffers[currentStreamBuffer].data(), sampleBufferLength, audioData, sampleBufferLength);
        }

        hr = mediaBuffer->Unlock();
        
        if (mVoice && mVoiceContext) {
            XAUDIO2_VOICE_STATE state;

            while(mVoice) {
                mVoice->GetState(&state);
                if (state.BuffersQueued < MAX_BUFFER_COUNT-1) {
                    break;
                }
                //WaitForSingleObject(mVoiceContext->hBufferEndEvent, INFINITE);
                WaitForSingleObject(mVoiceContext->hBufferEndEvent, 50);
            }

            if (mVoice) {
                XAUDIO2_BUFFER buf = {};
                buf.AudioBytes = sampleBufferLength;
                buf.pAudioData = mStreamBuffers[currentStreamBuffer].data();
                mVoice->SubmitSourceBuffer(&buf);

                currentStreamBuffer++;
                currentStreamBuffer %= MAX_BUFFER_COUNT;
            }
        }

        LeaveCriticalSection(&m_critSec);
        {
            std::unique_lock<std::mutex> lk(mSrcReaderMutex);
            mBRequestNewReaderSample = true;
        }
        
    }
}

//--------------------
void ofMediaFoundationSoundPlayer::addUpdateListener() {
    if (!mBAddedUpdateEvent) {
        ofAddListener(ofEvents().update, this, &ofMediaFoundationSoundPlayer::update);
    }
    mBAddedUpdateEvent = true;
}

//--------------------
void ofMediaFoundationSoundPlayer::removeUpdateListener() {
    if (mBAddedUpdateEvent) {
        ofRemoveListener(ofEvents().update, this, &ofMediaFoundationSoundPlayer::update);
    }
    mBAddedUpdateEvent = false;
}

//--------------------
void ofMediaFoundationSoundPlayer::_clearExtraVoices() {
    for (auto& it : mExtraVoices) {
        if (it.second) {
            std::ignore = it.second->Stop();
            std::ignore = it.second->FlushSourceBuffers();
            it.second->DestroyVoice();
        }
    }
    mExtraVoices.clear();
}

//--------------------
void ofMediaFoundationSoundPlayer::_setPan(IXAudio2SourceVoice* avoice, float apan) {
    if (!sXAudioMasteringVoice) {
        return;
    }

    DWORD dwChannelMask;
    sXAudioMasteringVoice->GetChannelMask(&dwChannelMask);

    float outputMatrix[8];
    for (int i = 0; i < 8; i++) outputMatrix[i] = 0;

    // pan of -1.0 indicates all left speaker, 
// 1.0 is all right speaker, 0.0 is split between left and right
    float left = 0.5f - apan / 2;
    float right = 0.5f + apan / 2;

    switch (dwChannelMask) {
    case SPEAKER_MONO:
        outputMatrix[0] = 1.0;
        break;
    case SPEAKER_STEREO:
    case SPEAKER_2POINT1:
    case SPEAKER_SURROUND:
        outputMatrix[0] = left;
        outputMatrix[1] = right;
        break;
    case SPEAKER_QUAD:
        outputMatrix[0] = outputMatrix[2] = left;
        outputMatrix[1] = outputMatrix[3] = right;
        break;
    case SPEAKER_4POINT1:
        outputMatrix[0] = outputMatrix[3] = left;
        outputMatrix[1] = outputMatrix[4] = right;
        break;
    case SPEAKER_5POINT1:
    case SPEAKER_7POINT1:
    case SPEAKER_5POINT1_SURROUND:
        outputMatrix[0] = outputMatrix[4] = left;
        outputMatrix[1] = outputMatrix[5] = right;
        break;
    case SPEAKER_7POINT1_SURROUND:
        outputMatrix[0] = outputMatrix[4] = outputMatrix[6] = left;
        outputMatrix[1] = outputMatrix[5] = outputMatrix[7] = right;
        break;
    }

    // Assuming pVoice sends to pMasteringVoice

    // TODO: Cache this 
    XAUDIO2_VOICE_DETAILS MasterVoiceDetails;
    sXAudioMasteringVoice->GetVoiceDetails(&MasterVoiceDetails);

    XAUDIO2_VOICE_DETAILS VoiceDetails;
    avoice->GetVoiceDetails(&VoiceDetails);
    avoice->SetOutputMatrix(NULL, VoiceDetails.InputChannels, MasterVoiceDetails.InputChannels, outputMatrix);
}

//--------------------
bool ofMediaFoundationSoundPlayer::_readToBuffer(IMFSourceReader* areader) {
    bool bKeepOnReadin = true;

    ofLogVerbose("ofMediaFoundationSoundPlayer::_readToBuffer") << "num channels: " << mNumChannels << " sample rate: " << mSampleRate << std::endl;

    unsigned int totalFrames = 0;

    uint64_t bytes64 = uint64_t(mBytesPerSample);
    uint64_t numChannels64 = uint64_t(mNumChannels);

    while (bKeepOnReadin) {
        // figure out a better way to process this //
        ComPtr<IMFSample> audioSample;
        DWORD streamIndex, flags = 0;
        LONGLONG llAudioTimeStamp;

        HRESULT hr = areader->ReadSample(
            MF_SOURCE_READER_FIRST_AUDIO_STREAM,
            0,                              // Flags.
            &streamIndex,                   // Receives the actual stream index. 
            &flags,                         // Receives status flags.
            &llAudioTimeStamp,              // Receives the time stamp.
            &audioSample                    // Receives the sample or NULL.
        );

        if (flags & MF_SOURCE_READERF_ENDOFSTREAM) {
            ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "End of stream";
            // set to the buffer and complete //
            bKeepOnReadin = false;
            break;
        }

        if (!audioSample) {
            ofLogVerbose("ofMediaFoundationSoundPlayer::update") << "Null audio sample.";
            //audioSample->Release();
        } else {
            ComPtr<IMFMediaBuffer> mediaBuffer;
            hr = audioSample->ConvertToContiguousBuffer(mediaBuffer.GetAddressOf());
            if (hr != S_OK) {
                continue;
            }

            BYTE* audioData = nullptr;
            DWORD sampleBufferLength = 0;

            hr = mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength);
            if (hr != S_OK) {
                continue;
            }
            
            size_t numFramesRead = uint64_t(sampleBufferLength) / (bytes64 * numChannels64);
            ofLogVerbose("ofMediaFoundationSoundPlayer::_readToBuffer") << "sampleBufferLength : " << sampleBufferLength << " num frames: " << numFramesRead << std::endl;
            totalFrames += numFramesRead;
            std::vector<BYTE> tempBuffer;
            tempBuffer.resize(sampleBufferLength, 0);
            memcpy_s(tempBuffer.data(), sampleBufferLength, audioData, sampleBufferLength);
            // add into the main buffer? 
            mBuffer.insert(mBuffer.end(), tempBuffer.begin(), tempBuffer.end());

            hr = mediaBuffer->Unlock();
            if (hr != S_OK) {
                continue;
            }
        }
    }
    mTotalNumFrames = (mBuffer.size() / uint64_t(mNumChannels)) / uint64_t(mBytesPerSample);
    mDurationSeconds = (double)mTotalNumFrames / (double)mSampleRate;
    mDurationMS = mTotalNumFrames * uint64_t(1000) / uint64_t(mSampleRate);
    auto durMillis = mTotalNumFrames * uint64_t(1000) / uint64_t(mSampleRate);
    double durSeconds = (double)durMillis / 1000.0;
    ofLogVerbose("ofMediaFoundationSoundPlayer::_readToBuffer") << "Total frames read: " << (totalFrames) << " mTotalNumFrames: " << mTotalNumFrames << " dur millis: " << durMillis << " dur seconds: " << durSeconds << std::endl;
    return mBuffer.size() > 0;
}