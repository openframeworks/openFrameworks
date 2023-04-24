#pragma once

#include "ofSoundBaseTypes.h"
#include "ofSoundBuffer.h"
#include "ofEvents.h"

#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <wrl.h>
#include <xaudio2.h>

// https://github.com/microsoft/DirectXTK/blob/main/Audio/AudioEngine.cpp

namespace of {
	struct MFSourceReaderNotifyCallback {
	public:
		virtual void OnSourceReaderEvent(HRESULT hrStatus, DWORD dwStreamIndex,
			DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample) = 0;
	};
}

class ofMediaFoundationSoundPlayer : public ofBaseSoundPlayer, public of::MFSourceReaderNotifyCallback {
public:

	static void SetMasterVolume(float apct);

	ofMediaFoundationSoundPlayer();
	~ofMediaFoundationSoundPlayer();

	bool load(const of::filesystem::path& fileName, bool stream = false) override;
	void unload() override;

	void play() override;
	void stop() override;

	void setVolume(float vol) override;
	void setPan(float apan) override; // -1 = left, 1 = right
	void setSpeed(float spd) override;
	void setPaused(bool bP) override;
	void setLoop(bool bLp) override;
	void setMultiPlay(bool bMp) override;
	void setPosition(float pct) override; // 0 = start, 1 = end;
	void setPositionMS(int ms) override;

	float getPosition() const override;
	int getPositionMS() const override;
	bool isPlaying() const override;
	float getSpeed() const override;
	float getPan() const override;
	bool isLoaded() const override;
	float getVolume() const override;

	float getDurationSeconds() { return mDurationSeconds; }
	uint32_t getDurationMS() { return mDurationMS; }

protected:

	void OnSourceReaderEvent(HRESULT hrStatus, DWORD dwStreamIndex,
		DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample) override;

	bool mBAddedUpdateEvent = false;
	void update(ofEventArgs& args);
	void addUpdateListener();
	void removeUpdateListener();

	struct MyComDeleterFunctor {
		template <typename T>
		void operator()(T* p) {
			if (p) {
				p->Release();
			}
		}
	};

	struct MyVoiceDeleterFunctor {
		void operator()(IXAudio2SourceVoice* p) {
			if (p) {
				std::ignore = p->Stop(0);
				std::ignore = p->FlushSourceBuffers();
				p->DestroyVoice();
			}
		}
	};


	using UniqueVoice = std::unique_ptr< IXAudio2SourceVoice, MyVoiceDeleterFunctor >;

	void _setPan(IXAudio2SourceVoice* avoice, float apan);
	bool _readToBuffer(IMFSourceReader* areader);

	static int sNumInstances;

	static bool sInitXAudio2();
	static bool sCloseXAudio2();

	static bool sInitMediaFoundation();
	static bool sCloseMediaFoundation();

	static bool sInitAudioSystems();
	static void sCloseAudioSystems();

	bool mBStreaming = false;
	unsigned short mNumChannels = 0;
	unsigned long mSampleRate = 44000;
	std::vector<BYTE> mBuffer;
	WAVEFORMATEX mWaveFormatEx;

	Microsoft::WRL::ComPtr<IMFSourceReader> mSrcReader;
	static Microsoft::WRL::ComPtr< IXAudio2> sXAudio2;
	static std::shared_ptr<IXAudio2MasteringVoice> sXAudioMasteringVoice;

	UniqueVoice mVoice;
	std::vector< std::shared_ptr<IXAudio2SourceVoice> > mExtraVoices;

	bool mBLoaded = false;
	bool mBIsPlaying = false;
	float mVolume = 1.0f;
	float mPan = 0.0f;
	float mSpeed = 1.0f;
	float mPosPct = 0.0f;
	bool mBMultiPlay = false;
	bool mBLoop = false;

	bool mBCanSeek = false;
	double mDurationSeconds = 0;
	uint32_t mDurationMS = 0;

	// 2 = INT_16, 3 = INT_24 and 4 = FLOAT_32
	// TODO: Adjust this based on file loaded 
	unsigned char mBytesPerSample = 2;
	uint64_t mTotalNumFrames = 0;
	size_t mBufferIndex = 0;
	uint64_t mNumSamplesAlreadyPlayed = 0;
	uint64_t mNumSamplesStored = 0;

	unsigned int MAX_BUFFER_COUNT = 3;
	std::vector< std::vector<BYTE> > mStreamBuffers;
	int currentStreamBuffer = 0;


	// https://github.com/walbourn/directx-sdk-samples/blob/main/XAudio2/XAudio2MFStream/XAudio2MFStream.cpp
	struct StreamingVoiceContext : public IXAudio2VoiceCallback {
		STDMETHOD_(void, OnVoiceProcessingPassStart)(UINT32) override {}
		STDMETHOD_(void, OnVoiceProcessingPassEnd)() override {}
		STDMETHOD_(void, OnStreamEnd)() override {}
		STDMETHOD_(void, OnBufferStart)(void*) override {}
		STDMETHOD_(void, OnBufferEnd)(void*) override {
			SetEvent(hBufferEndEvent);
		}
		STDMETHOD_(void, OnLoopEnd)(void*) override {}
		STDMETHOD_(void, OnVoiceError)(void*, HRESULT) override {}
		HANDLE hBufferEndEvent;
		StreamingVoiceContext() :
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
			hBufferEndEvent(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE))
#else
			hBufferEndEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
#endif
		{}
		virtual ~StreamingVoiceContext() {
			CloseHandle(hBufferEndEvent);
		}
	};


	std::shared_ptr<StreamingVoiceContext> mVoiceContext;
	bool mBEndOfStream = false;

	CRITICAL_SECTION m_critSec;

	class SourceReaderCallback : public IMFSourceReaderCallback {
	public:
		STDMETHOD(QueryInterface) (REFIID iid, _COM_Outptr_ void** ppv) override {
			if (!ppv) { return E_POINTER; }
			if (_uuidof(IMFSourceReaderCallback) == iid) {
				*ppv = this;
				return S_OK;
			}
			*ppv = nullptr;
			return E_NOINTERFACE;
		}

		// we are managing this, do don't worry about these
		STDMETHOD_(ULONG, AddRef)() override {return 0;}
		STDMETHOD_(ULONG, Release)() override {return 0;}

		STDMETHOD(OnReadSample)(_In_ HRESULT hrStatus, _In_ DWORD dwStreamIndex, _In_ DWORD dwStreamFlags, _In_ LONGLONG llTimestamp, _In_opt_ IMFSample* pSample) override {
			UNREFERENCED_PARAMETER(dwStreamIndex);
			if (mCB) {
				mCB->OnSourceReaderEvent(hrStatus, dwStreamIndex, dwStreamFlags, llTimestamp, pSample);
			}
			status = hrStatus;
			return S_OK;
		}

		STDMETHOD(OnFlush)(_In_ DWORD) override {return S_OK;}
		STDMETHOD(OnEvent)(_In_ DWORD, _In_ IMFMediaEvent*) override {return S_OK;}

		void setCB(of::MFSourceReaderNotifyCallback* acb) {
			mCB = acb;
		}

		HRESULT             status;
		of::MFSourceReaderNotifyCallback* mCB = nullptr;
		SourceReaderCallback() : status(S_OK) {}
		virtual ~SourceReaderCallback() { mCB = nullptr; }
	};


	std::shared_ptr<SourceReaderCallback> mSrcReaderCallback;
	std::mutex mSrcReaderMutex;
	bool mBRequestNewReaderSample = false;
};