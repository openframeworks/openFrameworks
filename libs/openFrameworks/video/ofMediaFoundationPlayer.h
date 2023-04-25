#pragma once

#include <mfmediaengine.h>
#include <d3d11_1.h>
#include <wrl.h>
#include <wincodec.h>
#include "ofConstants.h"
#include "ofVideoBaseTypes.h"
#include "ofPixels.h"
#include "ofFbo.h"
#include "ofEvent.h"

#include <mutex>
#include <queue>

namespace of {
    struct MediaEngineNotifyCallback {
    public:
        virtual void OnMediaEngineEvent(DWORD meEvent, DWORD_PTR param1, DWORD param2) = 0;
    };
}

class ofMediaFoundationPlayer : public ofBaseVideoPlayer, public of::MediaEngineNotifyCallback {
protected:
    friend class ofMediaFoundationSoundPlayer;
    
    // MediaEngineNotify: Implements the callback for Media Engine event notification.
    class ofMEEventProcessor : public IMFMediaEngineNotify {
    public:

        STDMETHODIMP QueryInterface(REFIID riid, void** ppv) {
            if (__uuidof(IMFMediaEngineNotify) == riid) {
                *ppv = static_cast<IMFMediaEngineNotify*>(this);
            } else {
                *ppv = nullptr;
                return E_NOINTERFACE;
            }
            AddRef();
            return S_OK;
        }

        // EventNotify is called when the Media Engine sends an event.
        STDMETHODIMP EventNotify(DWORD meEvent, DWORD_PTR param1, DWORD param2) {
            if (meEvent == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE) {
                SetEvent(reinterpret_cast<HANDLE>(param1));
            } else {
                mCB->OnMediaEngineEvent(meEvent, param1, param2);
            }
            return S_OK;
        }

        // we are going to store and manage this, so don't worry about these functions
        STDMETHODIMP_(ULONG) AddRef() {return 0;}
        STDMETHODIMP_(ULONG) Release() {return 0;}

        void setCB(MediaEngineNotifyCallback* acb) {
            mCB = acb;
        }

    protected:
        MediaEngineNotifyCallback* mCB = nullptr;
    };

public:
    static std::string MFEventToString(MF_MEDIA_ENGINE_EVENT aevent);
    static std::string MFErrorToString(MF_MEDIA_ENGINE_ERR aerror);

    ofMediaFoundationPlayer();
    ~ofMediaFoundationPlayer();

    static void setDurationHackEnabled(bool ab);

    bool                load(std::string name) override;
    void				loadAsync(std::string name) override;
    void                close() override;

    bool                isInitialized() const override;

    void                update() override;
    bool                isFrameNew() const override;
    void                play() override;
    void                stop() override;
    void                setPaused(bool bPause) override;

    bool                isLoaded() const override;
    bool                isPlaying() const override;

    float               getWidth() const override;
    float               getHeight() const override;

    ofTexture*          getTexturePtr();

    bool                isPaused() const override;

    void                setPosition(float pct) override;
    void                setSpeed(float speed) override;
    void 				setVolume(float volume) override;

    void				setFrame(int frame) override;
    int					getCurrentFrame() const override;
    int					getTotalNumFrames() const override;

    void 				setLoopState(ofLoopType state) override;
    ofLoopType			getLoopState() const override;

    float 				getPosition() const override;
    float 				getSpeed() const override;
    float 				getDuration() const override;
    bool				getIsMovieDone() const override;

    bool                hasAudio();
    bool                hasVideo();

    void				firstFrame() override;
    void				nextFrame() override;
    void				previousFrame() override;

    bool                setPixelFormat(ofPixelFormat pixelFormat) override;
    ofPixelFormat       getPixelFormat() const override;

    ofPixels&           getPixels() override;
    const ofPixels&     getPixels() const override;

    void setUsingHWAccel(bool ab) { mBUseHWAccel = ab; };
    bool isUsingHWAccel() { return mBUseHWAccel; }

    float getFrameRate() { return mFramerate; }

    ofEvent<MF_MEDIA_ENGINE_EVENT> MFEngineEvent;
    ofEvent<MF_MEDIA_ENGINE_ERR> MFErrorEvent;

protected:
    bool _load(std::string name, bool abAsync);
    void OnMediaEngineEvent(DWORD aEvent, DWORD_PTR param1, DWORD param2) override;

    class MEDXDeviceManager {
    public:
        MEDXDeviceManager();
        ~MEDXDeviceManager();

        bool createDX11Device();
        bool isHWSupported() { return mBUseDX; }

        IMFDXGIDeviceManager* getDXGIManagerPtr() { return m_spDXGIManager.Get(); }
        ID3D11Device* getD11Device() { return m_spDX11Device.Get(); }
        HANDLE getGLHandleD3D() { return gl_handleD3D; }
        ID3D11DeviceContext* getContext() {return m_spDX11DeviceContext.Get();}

    protected:
        bool mBUseDX = true;
        Microsoft::WRL::ComPtr<IMFDXGIDeviceManager> m_spDXGIManager;
        Microsoft::WRL::ComPtr<ID3D11Device> m_spDX11Device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_spDX11DeviceContext;

        UINT mResetToken = 0;
        HANDLE gl_handleD3D;
    };


public:
    static std::shared_ptr<MEDXDeviceManager> getDxDeviceManager();

    class METexture {
    public:
        virtual bool allocate(ofPixelFormat afmt, int aw, int ah);
        virtual bool transferFrame(IMFMediaEngine* aengine) = 0;
        virtual bool create(DXGI_FORMAT aDxFormat) = 0; 
        virtual bool isValid() = 0;
        virtual bool draw(ofPixels& apix) = 0;
        virtual bool updatePixels(ofTexture& aSrcTex, ofPixels& apix, ofPixelFormat aTargetPixFormat) = 0;
        
        int getWidth() { return mWidth; }
        int getHeight() { return mHeight; }

    protected:
        bool _swapPixelsFromSrc4ChannelTo3(ofPixels& aDstPix);

        unsigned int mWidth = 0;
        unsigned int mHeight = 0;

        MFARGB bgColor{ 0, 0, 0, 0 };
        MFVideoNormalizedRect mNormalizedVidRect{ 0.0f, 0.0f, 1.0f, 1.0f };
        ofPixelFormat mOfPixFmt;
        std::shared_ptr<ofTexture> mOfTex;
        ofPixels mSrcPixels;
    };

protected:
    
    void handleMEEvent(DWORD aevent);
    void updateDuration();

    std::shared_ptr<METexture> mMeTexture;

    bool mBUseHWAccel = true;
    static bool sBAllowDurationHack;// = true;

    bool mBReady = false;
    bool mBLoaded = false;
    bool mBNewFrame = false;
    mutable double mDuration = 0.0;
    float mWidth = 0.f;
    float mHeight = 0.f;
    bool mBDone = false;
    bool mBPlaying = false;
    bool mBCanSeek = false;
    float mFramerate = 1.f / 30.f;
    int mEstimatedNumFrames = 1;
    ofPixelFormat mPixFormat;
    ofLoopType mLoopType = OF_LOOP_NONE;
    float mTargetSeekPercent = -1.0;

    double mTimeStartedSeek = 0.0;
    double mTimePlayback = 0.0;

    std::shared_ptr<ofMEEventProcessor> mEventProcessor;

    static std::shared_ptr<MEDXDeviceManager> sDeviceManager;

    DXGI_FORMAT m_d3dFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    
    // Media Engine related
    Microsoft::WRL::ComPtr<IMFMediaEngine> m_spMediaEngine;
    Microsoft::WRL::ComPtr<IMFMediaEngineEx> m_spEngineEx;

    CRITICAL_SECTION m_critSec;

    //static int sNumInstances;

    //static bool sInitMediaFoundation();
    //static void sCloseMediaFoundation();

    std::queue<DWORD> mEventsQueue;
    std::mutex mMutexEvents;
    // needed to copy the pixels while in lock()
    // also an easy color conversion 
    ofFbo mFbo;
    ofTexture mCopyTex;
    ofPixels mPixels;
    mutable bool mBUpdatePixels = false;

    //std::mutex mMutexLoad;
    bool mBLoadAsync = false;
    std::atomic_bool mBIsDoneAtomic;
    std::atomic_bool mBIsClosedAtomic;
    std::condition_variable mWaitCondition;
    
};