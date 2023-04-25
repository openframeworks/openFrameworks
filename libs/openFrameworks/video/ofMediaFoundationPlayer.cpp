
#include "ofPixels.h"
#include "ofMediaFoundationPlayer.h"
#include "ofLog.h"
#include <string.h>
#include <mfapi.h>
#include <mferror.h>
#include "ofTexture.h"
#include "ofGLUtils.h"
#include "ofGraphics.h"
#include "ofEventUtils.h"

// declares some shared Media Foundation code
#include "ofMediaFoundationSoundPlayer.h"

using namespace Microsoft::WRL;

std::shared_ptr<ofMediaFoundationPlayer::MEDXDeviceManager> ofMediaFoundationPlayer::sDeviceManager;

bool ofMediaFoundationPlayer::sBAllowDurationHack = true;

//----------------------------------------------
void ofMediaFoundationPlayer::setDurationHackEnabled(bool ab) {
    sBAllowDurationHack = ab;
}

//----------------------------------------------
ofMediaFoundationPlayer::MEDXDeviceManager::MEDXDeviceManager() {
    gl_handleD3D = nullptr;
}

//----------------------------------------------
ofMediaFoundationPlayer::MEDXDeviceManager::~MEDXDeviceManager() {
    if (gl_handleD3D != nullptr) {
        ofLogVerbose("ofMEDXDeviceManager") << " closing gl handleD3D.";
        wglDXCloseDeviceNV(gl_handleD3D);
        gl_handleD3D = nullptr;
    }
    mBUseDX = false;
    m_spDXGIManager = nullptr;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::MEDXDeviceManager::createDX11Device() {
    static const D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    D3D_FEATURE_LEVEL FeatureLevel;
    HRESULT hr = S_OK;

    mBUseDX = true;

    hr = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_VIDEO_SUPPORT | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        levels,
        ARRAYSIZE(levels),
        D3D11_SDK_VERSION,
        &m_spDX11Device,
        &FeatureLevel,
        &m_spDX11DeviceContext
    );

    if (FAILED(hr)) {
        ofLogError("ofMEDXDeviceManager::CreateDX11Device()") << " unable to use hw accel.";
        mBUseDX = false;
        return mBUseDX;
    }

    ComPtr<ID3D10Multithread> spMultithread;
    if (SUCCEEDED(m_spDX11Device.Get()->QueryInterface(IID_PPV_ARGS(&spMultithread)))) {
        spMultithread->SetMultithreadProtected(TRUE);
    } else {
        ofLogError("ofMEDXDeviceManager :: CreateDX11Device") << " unable to set multi thread.";
        mBUseDX = false;
        return mBUseDX;
    }


    hr = MFCreateDXGIDeviceManager(&mResetToken, &m_spDXGIManager);
    if (FAILED(hr)) {
        ofLogError("ofMEDXDeviceManager :: CreateDX11Device") << " unable to create DXGIDeviceManager.";
        mBUseDX = false;
        return mBUseDX;
    }

    hr = m_spDXGIManager->ResetDevice(m_spDX11Device.Get(), mResetToken);
    if (FAILED(hr)) {
        ofLogError("ofMEDXDeviceManager :: CreateDX11Device") << " unable to ResetDevice.";
        mBUseDX = false;
        return mBUseDX;
    }

    if (mBUseDX) {
        if (SUCCEEDED(hr)) {
            gl_handleD3D = wglDXOpenDeviceNV(m_spDX11Device.Get());
        }
    }

    if (gl_handleD3D == nullptr) {
        ofLogError("ofMEDXDeviceManager :: CreateDX11Device") << " error creating GL D3D Handle.";
        mBUseDX = false;
    }

    return mBUseDX;
}

class BstrURL {
public:
    BstrURL(std::string aurl) {
        std::wstring ws = std::wstring(aurl.begin(), aurl.end());
        assert(!ws.empty());
        _bstrStr = SysAllocStringLen(ws.data(), ws.size());
    }
    ~BstrURL() {
        SysReleaseString(_bstrStr);
        _bstrStr = nullptr;
    }

    operator BSTR() const { 
        return _bstrStr;
    }
private:
    BSTR _bstrStr = nullptr;
};

//----------------------------------------------
bool ofMediaFoundationPlayer::METexture::allocate( ofPixelFormat afmt, int aw, int ah) {
    if (mOfTex && ((int)mOfTex->getWidth() != aw || (int)mOfTex->getHeight() != ah)) {
        mOfTex.reset();
    }
    if (!mOfTex) {
        mOfTex = std::make_shared<ofTexture>();
    }
    mWidth = aw;
    mHeight = ah;
    mOfPixFmt = afmt;
    //auto glFormat = ofGetGLInternalFormatFromPixelFormat(OF_PIXELS_BGRA);
    auto glFormat = ofGetGLInternalFormatFromPixelFormat(afmt);
    // make a GL_TEXTURE2D
    mOfTex->allocate(mWidth, mHeight, glFormat, false);
    return true;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::METexture::_swapPixelsFromSrc4ChannelTo3(ofPixels& aDstPix) {
    const auto targetPixFormat = aDstPix.getPixelFormat();
    const auto srcPixFormat = mSrcPixels.getPixelFormat();

    bool bNeedsSwap = (srcPixFormat == OF_PIXELS_BGRA && targetPixFormat == OF_PIXELS_RGB);
    if (srcPixFormat == OF_PIXELS_RGBA && targetPixFormat == OF_PIXELS_BGR) {
        bNeedsSwap = true;
    }

    if (bNeedsSwap) {
        auto srcPixels = mSrcPixels.getPixelsIter();
        auto dstPixels = aDstPix.getPixelsIter();
        auto srcPixel = srcPixels.begin();
        auto dstPixel = dstPixels.begin();
        auto endPixel = srcPixels.end();
        for (; srcPixel != srcPixels.end(); srcPixel++, dstPixel++) {
            dstPixel[0] = srcPixel[2];
            dstPixel[1] = srcPixel[1];
            dstPixel[2] = srcPixel[0];
        }
    } else {
        // straight copy, removing the alpha channel
        auto srcPixels = mSrcPixels.getPixelsIter();
        auto dstPixels = aDstPix.getPixelsIter();
        auto srcPixel = srcPixels.begin();
        auto dstPixel = dstPixels.begin();
        auto endPixel = srcPixels.end();
        for (; srcPixel != srcPixels.end(); srcPixel++, dstPixel++) {
            dstPixel[0] = srcPixel[0];
            dstPixel[1] = srcPixel[1];
            dstPixel[2] = srcPixel[2];
        }
    }
    return true;
}

//----------------------------------------------
class SharedDXGLTexture : public ofMediaFoundationPlayer::METexture {
public:
    SharedDXGLTexture() { mGLDX_Handle = nullptr; }
    ~SharedDXGLTexture();

    bool allocate(ofPixelFormat afmt, int aw, int ah);
    bool create(DXGI_FORMAT aDxFormat) override;
    bool transferFrame(IMFMediaEngine* aengine) override;
    bool isValid() override { return mBValid; }
    HANDLE getGLDXHandle() { return mGLDX_Handle; }
    ID3D11Texture2D* getDXTexture() { return mDXTex.Get(); }

    bool draw(ofPixels& apix) override;
    bool updatePixels(ofTexture& aSrcTex, ofPixels& apix, ofPixelFormat aTargetPixFormat) override;

    bool lock();
    bool unlock();
    bool isLocked();

protected:
    ComPtr<ID3D11Texture2D> mDXTex{ nullptr };
    ComPtr<ID3D11Texture2D> stagingTexture{ nullptr };
    bool mBValid = false;
    HANDLE mGLDX_Handle;
    bool mBLocked = false;
};

//----------------------------------------------
class WICTextureManager : public ofMediaFoundationPlayer::METexture {
public:
    bool isValid() override { return mBValid; }

    bool allocate(ofPixelFormat afmt, int aw, int ah) override;
    bool create(DXGI_FORMAT aDxFormat) override;

    bool transferFrame(IMFMediaEngine* aengine) override;
    bool draw(ofPixels& apix) override;
    bool updatePixels(ofTexture& aSrcTex, ofPixels& apix, ofPixelFormat aTargetPixFormat) override;

protected:
    Microsoft::WRL::ComPtr<IWICBitmap> mWicBitmap = nullptr;
    Microsoft::WRL::ComPtr<IWICImagingFactory> mWicFactory = nullptr;
    bool mBValid = false;
};

//----------------------------------------------
bool SharedDXGLTexture::allocate(ofPixelFormat afmt, int aw, int ah) {
    ofPixelFormat outfmt = OF_PIXELS_BGRA;
    if (afmt == OF_PIXELS_RGBA || afmt == OF_PIXELS_RGB) {
        outfmt = OF_PIXELS_RGBA;
    }
    return METexture::allocate(outfmt, aw, ah);
}

//----------------------------------------------
bool SharedDXGLTexture::create(DXGI_FORMAT aDxFormat) {
    unsigned int tw = static_cast<unsigned int>(getWidth());
    unsigned int th = static_cast<unsigned int>(getHeight());

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = tw;
    desc.Height = th;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = aDxFormat;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET;
    desc.Usage = D3D11_USAGE_DEFAULT;

    auto dxMan = ofMediaFoundationPlayer::getDxDeviceManager();

    if (SUCCEEDED(dxMan->getD11Device()->CreateTexture2D(&desc, nullptr, mDXTex.GetAddressOf()))) {
        mGLDX_Handle = wglDXRegisterObjectNV(
            dxMan->getGLHandleD3D(), 
            mDXTex.Get(),
            mOfTex->getTextureData().textureID,
            GL_TEXTURE_2D, 
            WGL_ACCESS_READ_ONLY_NV);

        D3D11_TEXTURE2D_DESC desc2;
        desc2.Width = desc.Width;
        desc2.Height = desc.Height;
        desc2.MipLevels = desc.MipLevels;
        desc2.ArraySize = desc.ArraySize;
        desc2.Format = desc.Format;
        desc2.SampleDesc = desc.SampleDesc;
        desc2.Usage = D3D11_USAGE_STAGING;
        desc2.BindFlags = 0;
        desc2.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc2.MiscFlags = 0;

        HRESULT hr = dxMan->getD11Device()->CreateTexture2D(&desc2, nullptr, stagingTexture.GetAddressOf());
        if (FAILED(hr)) {
            ofLogError("ofMEVideoPlayer :: SharedDXGLTexture :: create") << " Failed to create staging texture";
            return false;
        }

        mBValid = (mGLDX_Handle != nullptr);
    } else {
        ofLogError("SharedDXGLTexture :: createSharedTexture") << " ERROR Creating shared texture.";
        mBValid = false;
    }
    return mBValid;
}

//----------------------------------------------
bool SharedDXGLTexture::transferFrame(IMFMediaEngine* aengine) {
    if (!mBValid || !mGLDX_Handle) {
        return false;
    }
    RECT targetRect{ 0, 0, mWidth, mHeight };
    if ((aengine->TransferVideoFrame(
        getDXTexture(),
        &mNormalizedVidRect, &targetRect, &bgColor)) == S_OK) {
        return true;
    }
    return false;
}

//----------------------------------------------
bool SharedDXGLTexture::lock() {
    if (mBLocked) return false;
    if (!mBValid) return false;
    mBLocked = wglDXLockObjectsNV(ofMediaFoundationPlayer::getDxDeviceManager()->getGLHandleD3D(), 1, &mGLDX_Handle );
    return mBLocked;
}

//----------------------------------------------
bool SharedDXGLTexture::unlock() {
    if (!mBLocked) return false;
    if (wglDXUnlockObjectsNV(ofMediaFoundationPlayer::getDxDeviceManager()->getGLHandleD3D(), 1, &mGLDX_Handle )) {
        mBLocked = false;
        return true;
    }
    return false;
}

//----------------------------------------------
bool SharedDXGLTexture::isLocked() {
    return mBLocked;
}

//----------------------------------------------
bool SharedDXGLTexture::draw(ofPixels& apix) {
    if (lock()) {
        mOfTex->draw(0, 0);
        unlock();
        return true;
    }
    return false;
}

//----------------------------------------------
bool SharedDXGLTexture::updatePixels(ofTexture& aSrcTex, ofPixels& apix, ofPixelFormat aTargetPixFormat) {

    auto deviceMan = ofMediaFoundationPlayer::getDxDeviceManager();
    auto immediateContext = deviceMan->getContext();
    ID3D11Texture2D* lDestImage = getDXTexture();
    auto d3device = deviceMan->getD11Device();

    // Copy GPU Resource to CPU
    D3D11_TEXTURE2D_DESC desc;
    lDestImage->GetDesc(&desc);
    D3D11_MAPPED_SUBRESOURCE mapInfo;

    HRESULT hr = S_OK;
    immediateContext->CopyResource(stagingTexture.Get(), lDestImage);
    // copy the texture to a staging resource
    if (!stagingTexture) {
        ofLogError("ofMediaFoundationPlayer :: SharedDXGLTexture :: updatePixels") << " ERROR copying staging texture.";
        return false;
    }

    // now, map the staging resource
    hr = immediateContext->Map(
        stagingTexture.Get(),
        0,
        D3D11_MAP_READ,
        0,
        &mapInfo);
    if (hr != S_OK) {
        ofLogError("ofMediaFoundationPlayer :: SharedDXGLTexture :: updatePixels") << " Failed to map staging texture.";
        return false;
    }
    immediateContext->Unmap(stagingTexture.Get(), 0);

    if (FAILED(hr)) {
        ofLogVerbose("ofMediaFoundationPlayer :: SharedDXGLTexture :: updatePixels") << " unable to map hw dx texture.";
        aSrcTex.readToPixels(apix);
        return apix.getWidth() > 0;
    }
    // the mOfPixFmt is always going to be BGRA || RGBA
    bool bSetStraightOnPix = (mOfPixFmt == aTargetPixFormat);
    if (mOfPixFmt == OF_PIXELS_BGRA && aTargetPixFormat == OF_PIXELS_RGBA) {
        bSetStraightOnPix = true;
    }
    if (mOfPixFmt == OF_PIXELS_RGBA && aTargetPixFormat == OF_PIXELS_BGRA) {
        bSetStraightOnPix = true;
    }
    if (bSetStraightOnPix) {
        apix.setFromPixels(reinterpret_cast<unsigned char*>(mapInfo.pData), getWidth(), getHeight(), mOfPixFmt);
        if (aTargetPixFormat != mOfPixFmt) {
            apix.swapRgb();
        }
    } else {
        mSrcPixels.setFromPixels(reinterpret_cast<unsigned char*>(mapInfo.pData), getWidth(), getHeight(), mOfPixFmt);

        apix.allocate(mSrcPixels.getWidth(), mSrcPixels.getHeight(), aTargetPixFormat);
        _swapPixelsFromSrc4ChannelTo3(apix);
    }
    return apix.getWidth() > 0 && apix.getHeight() > 0;
}

//----------------------------------------------
SharedDXGLTexture::~SharedDXGLTexture() {
    // release the handle 
    if (mGLDX_Handle != nullptr) {
        if (wglGetCurrentContext() != nullptr) {
            if (isLocked()) {
                unlock();
            }
            wglDXUnregisterObjectNV(ofMediaFoundationPlayer::getDxDeviceManager()->getGLHandleD3D(), mGLDX_Handle);
            mGLDX_Handle = nullptr;
        }
    }
}

//----------------------------------------------
bool WICTextureManager::allocate(ofPixelFormat afmt, int aw, int ah) {
    ofLogVerbose("ofMediaFoundationVideoPlayer :: WICTextureManager") << " allocate.";
    METexture::allocate(OF_PIXELS_BGRA, aw, ah);
    mBValid = false;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mWicFactory));
    if (hr == S_OK && mWicFactory) {
        mBValid = true;
        ofLogVerbose("ofMediaFoundationVideoPlayer :: WICTextureManager") << " created CLSID_WICImagingFactory.";
    } else {
        ofLogError("ofMediaFoundationVideoPlayer :: WICTextureManager") << " CLSID_WICImagingFactory.";
        mWicFactory = nullptr;
    }
    return mBValid;
}

//----------------------------------------------
bool WICTextureManager::create(DXGI_FORMAT aDxFormat) {
    
    if (mBValid && mWicFactory) {
        unsigned int tw = static_cast<unsigned int>(getWidth());
        unsigned int th = static_cast<unsigned int>(getHeight());

        if (!mWicBitmap || mWidth != tw || mHeight != th) {
            if (mWicBitmap) {
                mWicBitmap->Release();
            }
            mWidth = tw;
            mHeight = th;

            GUID wicPixFmt = GUID_WICPixelFormat32bppBGRA;
            //GUID_WICPixelFormat32bppRGBA
            HRESULT hr = mWicFactory->CreateBitmap(tw, th, wicPixFmt, WICBitmapCacheOnDemand,
                mWicBitmap.GetAddressOf());
            if (hr == S_OK) {

            } else {
                ofLogError("ofMediaFoundationVideoPlayer :: WICTextureManager") << " ERROR CreateBitmap.";
                mBValid = false;
                mWicBitmap = nullptr;
            }
        }
    }

    return mBValid;
}

//----------------------------------------------
bool WICTextureManager::transferFrame(IMFMediaEngine* aengine) {
    if (!mBValid || !mWicBitmap) {
        return false;
    }
    RECT targetRect{ 0, 0, mWidth, mHeight };
    if ((aengine->TransferVideoFrame(
        mWicBitmap.Get(),
        &mNormalizedVidRect, &targetRect, &bgColor)) == S_OK) {
        return true;
    }
    return false;
}

//----------------------------------------------
bool WICTextureManager::draw(ofPixels& apix) {

    ComPtr<IWICBitmapLock> lockedData;
    DWORD flags = WICBitmapLockRead;
    WICRect srcRect{ 0, 0, mWidth, mHeight };

    if (FAILED(mWicBitmap->Lock(&srcRect, flags, lockedData.GetAddressOf()))) {
        return false;
    }

    UINT stride{ 0 };
    if (FAILED(lockedData->GetStride(&stride))) {
        return false;
    }

    UINT bufferSize{ 0 };
    unsigned char* data{ nullptr };
    if (FAILED(lockedData->GetDataPointer(&bufferSize, &data))) {
        return false;
    }

    mSrcPixels.setFromAlignedPixels(data, getWidth(), getHeight(), mOfPixFmt, stride);
    mOfTex->loadData(apix);
    //mOfTex->loadData(data, mWidth, mHeight, GL_BGRA);
    mOfTex->draw(0, 0);

    lockedData->Release();

    return true;
}

//----------------------------------------------
bool WICTextureManager::updatePixels(ofTexture& aSrcTex, ofPixels& apix, ofPixelFormat aTargetPixFormat) {
    // always has mOfPixFmt == OF_PIXELS_BGRA
    bool bSetStraightOnPix = (mOfPixFmt == aTargetPixFormat);
    if (mOfPixFmt == OF_PIXELS_BGRA && aTargetPixFormat == OF_PIXELS_RGBA) {
        bSetStraightOnPix = true;
    }
    if (bSetStraightOnPix) {
        apix = mSrcPixels;
        if (aTargetPixFormat != mOfPixFmt) {
            apix.swapRgb();
        }
    } else {
        // swap around pixels 
        apix.allocate(mSrcPixels.getWidth(), mSrcPixels.getHeight(), aTargetPixFormat);
        _swapPixelsFromSrc4ChannelTo3(apix);
    }
    return true;
}

//----------------------------------------------
std::string ofMediaFoundationPlayer::MFEventToString(MF_MEDIA_ENGINE_EVENT aevent) {
    static std::unordered_map<MF_MEDIA_ENGINE_EVENT, std::string> sMFMessages =
    {
        { MF_MEDIA_ENGINE_EVENT_LOADSTART, "MF_MEDIA_ENGINE_EVENT_LOADSTART" },
            { MF_MEDIA_ENGINE_EVENT_PROGRESS, "MF_MEDIA_ENGINE_EVENT_PROGRESS" },
            { MF_MEDIA_ENGINE_EVENT_SUSPEND, "MF_MEDIA_ENGINE_EVENT_SUSPEND" },
            { MF_MEDIA_ENGINE_EVENT_ABORT, "MF_MEDIA_ENGINE_EVENT_ABORT" },
            { MF_MEDIA_ENGINE_EVENT_ERROR, "MF_MEDIA_ENGINE_EVENT_ERROR" },
            { MF_MEDIA_ENGINE_EVENT_EMPTIED, "MF_MEDIA_ENGINE_EVENT_EMPTIED" },
            { MF_MEDIA_ENGINE_EVENT_STALLED, "MF_MEDIA_ENGINE_EVENT_STALLED" },
            { MF_MEDIA_ENGINE_EVENT_PLAY, "MF_MEDIA_ENGINE_EVENT_PLAY" },
            { MF_MEDIA_ENGINE_EVENT_PAUSE, "MF_MEDIA_ENGINE_EVENT_PAUSE" },
            { MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA, "MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA" },
            { MF_MEDIA_ENGINE_EVENT_LOADEDDATA, "MF_MEDIA_ENGINE_EVENT_LOADEDDATA" },
            { MF_MEDIA_ENGINE_EVENT_WAITING, "MF_MEDIA_ENGINE_EVENT_WAITING" },
            { MF_MEDIA_ENGINE_EVENT_PLAYING, "MF_MEDIA_ENGINE_EVENT_PLAYING" },
            { MF_MEDIA_ENGINE_EVENT_CANPLAY, "MF_MEDIA_ENGINE_EVENT_CANPLAY" },
            { MF_MEDIA_ENGINE_EVENT_CANPLAYTHROUGH, "MF_MEDIA_ENGINE_EVENT_CANPLAYTHROUGH" },
            { MF_MEDIA_ENGINE_EVENT_SEEKING, "MF_MEDIA_ENGINE_EVENT_SEEKING" },
            { MF_MEDIA_ENGINE_EVENT_SEEKED, "MF_MEDIA_ENGINE_EVENT_SEEKED" },
            { MF_MEDIA_ENGINE_EVENT_TIMEUPDATE, "MF_MEDIA_ENGINE_EVENT_TIMEUPDATE" },
            { MF_MEDIA_ENGINE_EVENT_ENDED, "MF_MEDIA_ENGINE_EVENT_ENDED" },
            { MF_MEDIA_ENGINE_EVENT_RATECHANGE, "MF_MEDIA_ENGINE_EVENT_RATECHANGE" },
            { MF_MEDIA_ENGINE_EVENT_DURATIONCHANGE, "MF_MEDIA_ENGINE_EVENT_DURATIONCHANGE" },
            { MF_MEDIA_ENGINE_EVENT_VOLUMECHANGE, "MF_MEDIA_ENGINE_EVENT_VOLUMECHANGE" },
            { MF_MEDIA_ENGINE_EVENT_FORMATCHANGE, "MF_MEDIA_ENGINE_EVENT_FORMATCHANGE" },
            { MF_MEDIA_ENGINE_EVENT_PURGEQUEUEDEVENTS, "MF_MEDIA_ENGINE_EVENT_PURGEQUEUEDEVENTS" },
            { MF_MEDIA_ENGINE_EVENT_TIMELINE_MARKER, "MF_MEDIA_ENGINE_EVENT_TIMELINE_MARKER" },
            { MF_MEDIA_ENGINE_EVENT_BALANCECHANGE, "MF_MEDIA_ENGINE_EVENT_BALANCECHANGE" },
            { MF_MEDIA_ENGINE_EVENT_DOWNLOADCOMPLETE, "MF_MEDIA_ENGINE_EVENT_DOWNLOADCOMPLETE" },
            { MF_MEDIA_ENGINE_EVENT_BUFFERINGSTARTED, "MF_MEDIA_ENGINE_EVENT_BUFFERINGSTARTED" },
            { MF_MEDIA_ENGINE_EVENT_BUFFERINGENDED, "MF_MEDIA_ENGINE_EVENT_BUFFERINGENDED" },
            { MF_MEDIA_ENGINE_EVENT_FRAMESTEPCOMPLETED, "MF_MEDIA_ENGINE_EVENT_FRAMESTEPCOMPLETED" },
            { MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE, "MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE" },
            { MF_MEDIA_ENGINE_EVENT_FIRSTFRAMEREADY, "MF_MEDIA_ENGINE_EVENT_FIRSTFRAMEREADY" },
            { MF_MEDIA_ENGINE_EVENT_TRACKSCHANGE, "MF_MEDIA_ENGINE_EVENT_TRACKSCHANGE" },
            { MF_MEDIA_ENGINE_EVENT_OPMINFO, "MF_MEDIA_ENGINE_EVENT_OPMINFO" },
            { MF_MEDIA_ENGINE_EVENT_RESOURCELOST, "MF_MEDIA_ENGINE_EVENT_RESOURCELOST" },
            { MF_MEDIA_ENGINE_EVENT_DELAYLOADEVENT_CHANGED, "MF_MEDIA_ENGINE_EVENT_DELAYLOADEVENT_CHANGED" },
            { MF_MEDIA_ENGINE_EVENT_STREAMRENDERINGERROR, "MF_MEDIA_ENGINE_EVENT_STREAMRENDERINGERROR" },
            { MF_MEDIA_ENGINE_EVENT_SUPPORTEDRATES_CHANGED, "MF_MEDIA_ENGINE_EVENT_SUPPORTEDRATES_CHANGED" },
            { MF_MEDIA_ENGINE_EVENT_AUDIOENDPOINTCHANGE, "MF_MEDIA_ENGINE_EVENT_AUDIOENDPOINTCHANGE" }
    };

    if (sMFMessages.count(aevent) > 0) {
        return sMFMessages.at(aevent);
    }
    return std::to_string(aevent);
}

//----------------------------------------------
std::string ofMediaFoundationPlayer::MFErrorToString(MF_MEDIA_ENGINE_ERR aerror) {
    static std::unordered_map<MF_MEDIA_ENGINE_ERR, std::string> sMFErrorMessages =
    {
        {MF_MEDIA_ENGINE_ERR_NOERROR, "MF_MEDIA_ENGINE_ERR_NOERROR" },
        {MF_MEDIA_ENGINE_ERR_ABORTED, "MF_MEDIA_ENGINE_ERR_ABORTED"},
        {MF_MEDIA_ENGINE_ERR_NETWORK, "MF_MEDIA_ENGINE_ERR_NETWORK"},
        {MF_MEDIA_ENGINE_ERR_DECODE, "MF_MEDIA_ENGINE_ERR_DECODE"},
        {MF_MEDIA_ENGINE_ERR_SRC_NOT_SUPPORTED, "MF_MEDIA_ENGINE_ERR_SRC_NOT_SUPPORTED"},
        {MF_MEDIA_ENGINE_ERR_ENCRYPTED, "MF_MEDIA_ENGINE_ERR_ENCRYPTED"}
    };

    if (sMFErrorMessages.count(aerror) > 0) {
        return sMFErrorMessages.at(aerror);
    }

    return std::to_string(aerror);
}

//----------------------------------------------
ofMediaFoundationPlayer::ofMediaFoundationPlayer() {
    //sInitMediaFoundation();
    // TODO: this is currently located in ofMediaFoundationSoundPlayer
    ofMediaFoundationUtils::InitMediaFoundation();
	InitializeCriticalSectionEx(&m_critSec, 0, 0);
    mPixFormat = OF_PIXELS_RGB;
}

//----------------------------------------------
ofMediaFoundationPlayer::~ofMediaFoundationPlayer() {
    close();
    ofMediaFoundationUtils::CloseMediaFoundation();
    DeleteCriticalSection(&m_critSec);
}

//----------------------------------------------
std::shared_ptr<ofMediaFoundationPlayer::MEDXDeviceManager> ofMediaFoundationPlayer::getDxDeviceManager() {
    if (!sDeviceManager) {
        sDeviceManager = std::make_shared<MEDXDeviceManager>();
        sDeviceManager->createDX11Device();
    }
    return sDeviceManager;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::load(std::string name) {
    return _load(name, false);
}

//----------------------------------------------
void ofMediaFoundationPlayer::loadAsync(std::string name) {
    _load(name, true);
}

//----------------------------------------------
bool ofMediaFoundationPlayer::_load(std::string name, bool abAsync) {
    close();

    mBLoadAsync = abAsync;

    bool bStream = false;
    bStream = bStream || ofIsStringInString(name, "http://");
    bStream = bStream || ofIsStringInString(name, "https://");
    bStream = bStream || ofIsStringInString(name, "rtsp://");
    bStream = bStream || ofIsStringInString(name, "rtmp://");

    std::string absPath = name;

    if (!bStream) {
        if (ofFile::doesFileExist(absPath)) {
            absPath = ofFilePath::getAbsolutePath(absPath, true);
        } else {
            ofLogError("ofMediaFoundationPlayer") << " file does not exist! " << absPath;
            return false;
        }
    }


    EnterCriticalSection(&m_critSec);

    // init device manager if not created
    if (isUsingHWAccel()) {
        setUsingHWAccel(getDxDeviceManager()->isHWSupported());
    }

    if (isUsingHWAccel()) {
        ofLogVerbose("ofMediaFoundationPlayer::load") << " utilizing hardware acceleration.";
    } else {
        ofLogVerbose("ofMediaFoundationPlayer::load") << " utilizing software decoding.";
    }


    ComPtr<IMFMediaEngineClassFactory> spFactory;
    ComPtr<IMFAttributes> spAttributes;

    HRESULT hr = S_OK;

    hr = CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&spFactory));

    if (FAILED(hr)) {
        ofLogError("ofMediaFoundationPlayer::load") << " unable to create Media Engine Class Factory.";
    }
    if (SUCCEEDED(hr)) {
        mEventProcessor = std::make_shared<ofMEEventProcessor>();
        mEventProcessor->setCB(this);
        hr = MFCreateAttributes(&spAttributes, 1);
        if (FAILED(hr)) {
            ofLogError("ofMediaFoundationPlayer::load") << " unable to MFCreateAttributes.";
        }
    }

    if (SUCCEEDED(hr)) {
        if (isUsingHWAccel()) {
            hr = spAttributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER, (IUnknown*)getDxDeviceManager()->getDXGIManagerPtr());
            if (FAILED(hr)) {
                ofLogError("ofMediaFoundationPlayer::load") << " unable to set device Manager.";
            }
        }
    }

    if (SUCCEEDED(hr)) {
        hr = spAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, (IUnknown*)mEventProcessor.get());
        if (FAILED(hr)) {
            ofLogError("ofMediaFoundationPlayer::load") << " unable to set media engine callback.";
        }
    }

    if (SUCCEEDED(hr)) {
        hr = spAttributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, m_d3dFormat);
        if (FAILED(hr)) {
            ofLogError("ofMediaFoundationPlayer::load") << " unable to set Video Output Format.";
        }
    }

    if (SUCCEEDED(hr)) {
        //const DWORD flags = MF_MEDIA_ENGINE_WAITFORSTABLE_STATE;
        const DWORD flags = MF_MEDIA_ENGINE_REAL_TIME_MODE;
        hr = spFactory->CreateInstance(flags, spAttributes.Get(), &m_spMediaEngine);
        if (FAILED(hr)) {
            ofLogError("ofMediaFoundationPlayer::load") << " unable to create media engine.";
        }
    }

    m_spMediaEngine->SetAutoPlay(FALSE);

    // now lets make a BSTR 
    m_spMediaEngine->SetSource(BstrURL(absPath));

    hr = m_spMediaEngine->Load();

    //mBLoaded = (hr == S_OK);

    if (hr == S_OK) {
        HRESULT hr2 = m_spMediaEngine.Get()->QueryInterface(__uuidof(IMFMediaEngine), (void**)&m_spEngineEx);
        if (FAILED(hr2)) {
            ofLogError("ofMediaFoundationPlayer::load") << " unable to create media engine ex.";
        }
    }

    LeaveCriticalSection(&m_critSec);

    if (hr != S_OK) {
        close();
    }

    if (!mBLoadAsync) {
        if (hr == S_OK) {
            mBIsDoneAtomic.store(false);
            mBIsClosedAtomic.store(false);

            std::mutex lock;
            std::unique_lock lk{ lock };
            mWaitCondition.wait(lk, [&] { return mBIsDoneAtomic.load() || mBIsClosedAtomic.load();  });
        }
    }

    //mBLoaded = SUCCEEDED(hr);

    return (hr == S_OK);
}



//----------------------------------------------
void ofMediaFoundationPlayer::close() {

    mBIsClosedAtomic.store(true);
    mBIsDoneAtomic.store(false);
    mWaitCondition.notify_all();

    EnterCriticalSection(&m_critSec);

    if (m_spMediaEngine) {
        ofMediaFoundationUtils::CallAsyncBlocking(
            [&] { m_spMediaEngine->Shutdown(); }
        );
    }

    m_spMediaEngine = nullptr;

    // clear out the events 
    {
        std::unique_lock<std::mutex> lk(mMutexEvents);
        if (!mEventsQueue.empty()) {
            std::queue<DWORD> tempty;
            std::swap(mEventsQueue, tempty);
        }
    }

    mTargetSeekPercent = -1.0f;
    mBLoaded = false;
    mBNewFrame = false;
    mDuration = 0.f;
    mWidth = 0.f;
    mHeight = 0.f;
    mBReady = false;
    mBDone = false;
    mBPlaying = false;
    mBCanSeek = false;
    mFramerate = 1.f / 30.f;
    mEstimatedNumFrames = 1;
    mBUpdatePixels = false;

    if (mMeTexture) {
        mMeTexture.reset();
    }

    if (mEventProcessor) {
        mEventProcessor.reset();
    }

    mFbo.clear();

    LeaveCriticalSection(&m_critSec);

}

//----------------------------------------------
bool ofMediaFoundationPlayer::isInitialized() const {
    return mBReady;
}

//----------------------------------------------
void ofMediaFoundationPlayer::OnMediaEngineEvent(DWORD aEvent, DWORD_PTR param1, DWORD param2) {
    if (aEvent == MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA) {
        if (!mBLoadAsync) {
            mBIsDoneAtomic.store(true);
            mWaitCondition.notify_one();
        }
        mBLoaded = true;
    } else if (aEvent == MF_MEDIA_ENGINE_EVENT_ERROR) {
        // not sure if we should handle this here
        ofLogVerbose("engine event: ofMediaFoundationPlayer") << " ERRROR";
        // clear out the mutex so that we no longer wait and the close event can be fired
        if (!mBLoadAsync) {
            mBIsDoneAtomic.store(false);
            mBIsClosedAtomic.store(true);
            mWaitCondition.notify_one();
        }
    }
    // lets not overload the events, query similar with isFrameNew()
    if (aEvent != MF_MEDIA_ENGINE_EVENT_TIMEUPDATE) {
        std::unique_lock<std::mutex> tt(mMutexEvents);
        mEventsQueue.push(aEvent);
    }


}

//----------------------------------------------
void ofMediaFoundationPlayer::update() {
    mBNewFrame = false;
    mBDone = false;
    if (mBLoaded && hasVideo() ) {
        LONGLONG time;
        if(m_spMediaEngine->OnVideoStreamTick(&time) == S_OK) {
            if (mMeTexture && mMeTexture->isValid()) {
                if ( mMeTexture->transferFrame(m_spMediaEngine.Get())) {
                    ofPushStyle(); {
                        ofSetColor(255);

                        bool bValidDraw = false;
                        mFbo.begin(); {
                            bValidDraw = mMeTexture->draw(mPixels);
                        } mFbo.end();

                        if (bValidDraw) {
                            mCopyTex = mFbo.getTexture();
                            if (mBUpdatePixels) {
                                mMeTexture->updatePixels(mCopyTex, mPixels,mPixFormat);
                            }
                            mBNewFrame = true;
                        }
                    } ofPopStyle();
                }
            }
        }
    }

    if (m_spMediaEngine) {
        if (!mBLoaded) {
            mTargetSeekPercent = -1.f;
        }
        if (mTargetSeekPercent > -0.5f && isInitialized()) {
            setPosition(mTargetSeekPercent);
        }
    }

    if (ofMediaFoundationPlayer::sBAllowDurationHack && m_spMediaEngine) {
        if (mBLoaded) {
            //not sure why the GetDuration() method returns inaccurate values,
            // but this will update the duration if the current time is larger
            double ctime = m_spMediaEngine->GetCurrentTime();
            if (ctime > mDuration) {
                mDuration = ctime;
                if (mFramerate > 0.0) {
                    mEstimatedNumFrames = mDuration / (1.f / mFramerate);
                }
            }
        }
        if (mBNewFrame) {
            if (mFramerate > 0.0) {
                mEstimatedNumFrames = mDuration / (1.f / mFramerate);
            }
        }
    }

    unsigned int mMaxEventsToProcess = 1000;
    unsigned int numEventsProcessed = 0;
    // now lets update the events in the queue
    bool bHasEvent = true;
    DWORD tevent;
    while (bHasEvent && (numEventsProcessed < mMaxEventsToProcess) ) { 
        bHasEvent = false;
        {
            std::unique_lock<std::mutex> lk(mMutexEvents);
            if (!mEventsQueue.empty()) {
                tevent = mEventsQueue.front();
                mEventsQueue.pop();
                bHasEvent = true;
            }
        }
        if (bHasEvent) {
            // handle the event //
            handleMEEvent(tevent);
            numEventsProcessed++;
        }
    }
}

//----------------------------------------------
bool ofMediaFoundationPlayer::isFrameNew() const {
    return mBNewFrame;
}

//----------------------------------------------
void ofMediaFoundationPlayer::play() {
    if (m_spMediaEngine) {
        if (mBDone) {
            setPosition(0.f);   
        }
        m_spMediaEngine->Play();
        mBDone = false;
    }
}

//----------------------------------------------
void ofMediaFoundationPlayer::stop() {
    if (m_spMediaEngine) {
        if (isPlaying()) {
            setPosition(0.f);
            setPaused(true);
            mBDone = false;
        }
    }
}

//----------------------------------------------
void ofMediaFoundationPlayer::setPaused(bool bPause) {
    if (m_spMediaEngine) {
        if (bPause) {
            m_spMediaEngine->Pause();
            mBPlaying = false;
        } else {
            play();
        }
    }
}

//----------------------------------------------
bool ofMediaFoundationPlayer::isLoaded() const {
    return mBLoaded;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::isPlaying() const {
    return mBPlaying;
}

//----------------------------------------------
float ofMediaFoundationPlayer::getWidth() const {
    return mWidth;
}

//----------------------------------------------
float ofMediaFoundationPlayer::getHeight() const {
    return mHeight;
}

//----------------------------------------------
ofTexture* ofMediaFoundationPlayer::getTexturePtr() {
    return &mCopyTex;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::isPaused() const {
    if (m_spMediaEngine) {
        return m_spMediaEngine->IsPaused();
    }
    return false;
}

//----------------------------------------------
void ofMediaFoundationPlayer::setPosition(float pct) {
    if (m_spMediaEngine) {
        if (!mBCanSeek) {
            ofLogError("ofMediaFoundationPlayer :: setPosition") << " seeking is not supported.";
            return;
        }

        if (mDuration > 0.0 && isInitialized() && pct >= 0.f && pct <= 1.0f) {
            // does not like when we are seeking //
            double ttime = static_cast<double>(pct) * mDuration;
            if (ttime >= mDuration) {
                ttime = mDuration;
            }
            if (ttime < 0.0) {
                ttime = 0.0;
            }
            if (!m_spMediaEngine->IsSeeking()) {
                if (m_spEngineEx) {
                    // MF_MEDIA_ENGINE_SEEK_MODE_APPROXIMATE
                    m_spEngineEx->SetCurrentTimeEx(ttime, MF_MEDIA_ENGINE_SEEK_MODE_NORMAL);
                } else if (m_spMediaEngine) {
                    m_spMediaEngine->SetCurrentTime(ttime);
                }
                mTargetSeekPercent = -1.f;
                //callAsync([&] {m_spMediaEngine->SetCurrentTime(ttime); });
            } else {
                mTargetSeekPercent = pct;
            }
        }
    }
}

//----------------------------------------------
void ofMediaFoundationPlayer::setSpeed(float speed) {
    if (m_spMediaEngine && m_spEngineEx) {
        if (m_spEngineEx->IsPlaybackRateSupported(static_cast<double>(speed))) {
            HRESULT hr = m_spMediaEngine->SetPlaybackRate(static_cast<double>(speed));
            if (hr != S_OK) {
                ofLogVerbose("ofMediaFoundationPlayer :: setSpeed : Unable to set speed to ") << speed << ".";
            }
        }
    }
}

//----------------------------------------------
void ofMediaFoundationPlayer::setVolume(float volume) {
    if (m_spMediaEngine) {
        ofMediaFoundationUtils::CallAsyncBlocking(
            [&] {m_spMediaEngine->SetVolume(static_cast<double>(volume)); 
        });
    }
}

//----------------------------------------------
void ofMediaFoundationPlayer::setFrame(int frame) {
    setPosition((float)frame / (float)getTotalNumFrames());
}

//----------------------------------------------
int ofMediaFoundationPlayer::getCurrentFrame() const {
    return getPosition() * mEstimatedNumFrames;
}

//----------------------------------------------
int ofMediaFoundationPlayer::getTotalNumFrames() const {
    return mEstimatedNumFrames;
}

//----------------------------------------------
void ofMediaFoundationPlayer::setLoopState(ofLoopType state) {
    if (state == OF_LOOP_NONE || state == OF_LOOP_NORMAL ) {
        if (m_spMediaEngine) {
            BOOL loop = (state == OF_LOOP_NORMAL) ? TRUE : FALSE;
            m_spMediaEngine->SetLoop( loop );
        }
        mLoopType = state;
    } else {
        ofLogError("ofMediaFoundationPlayer") << " cannot set loop of type palindrome.";
    }
}

//----------------------------------------------
ofLoopType ofMediaFoundationPlayer::getLoopState() const {
    return mLoopType;
}

//----------------------------------------------
float ofMediaFoundationPlayer::getPosition() const {
    if (m_spMediaEngine && mDuration > 0.0 ) {
        //return static_cast<float>(m_spMediaEngine->GetCurrentTime()) / mDuration;
        const double ctime = m_spMediaEngine->GetCurrentTime();
        if (ctime > mDuration) {
            mDuration = ctime;
        }
        return (ctime) / mDuration;
    }
    return 0.f;
}

//----------------------------------------------
float ofMediaFoundationPlayer::getSpeed() const {
    if (m_spMediaEngine) {
        return static_cast<float>(m_spMediaEngine->GetPlaybackRate());
    }
    return 1.f;
}

//----------------------------------------------
float ofMediaFoundationPlayer::getDuration() const {
    return mDuration;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::getIsMovieDone() const {
    return mBDone;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::hasAudio() {
    if (m_spMediaEngine) {
        return m_spMediaEngine->HasAudio();
    }
    return false;
}

//----------------------------------------------
bool ofMediaFoundationPlayer::hasVideo() {
    if (m_spMediaEngine) {
        return m_spMediaEngine->HasVideo();
    }
    return false;
}

//----------------------------------------------
void ofMediaFoundationPlayer::firstFrame() {
    setPosition(0.0f);
}

//----------------------------------------------
void ofMediaFoundationPlayer::nextFrame() {
    if (m_spEngineEx) {
        m_spEngineEx->FrameStep(TRUE);
    }
}

//----------------------------------------------
void ofMediaFoundationPlayer::previousFrame() {
    if (m_spEngineEx) {
        m_spEngineEx->FrameStep(FALSE);
    }
}

//----------------------------------------------
bool ofMediaFoundationPlayer::setPixelFormat(ofPixelFormat pixelFormat) {
    if (pixelFormat == OF_PIXELS_BGRA || pixelFormat == OF_PIXELS_BGR ) {
        m_d3dFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    } else if(pixelFormat == OF_PIXELS_RGBA || pixelFormat == OF_PIXELS_RGB ) {
        m_d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    switch (pixelFormat) {
    case OF_PIXELS_RGB:
    case OF_PIXELS_BGR:
    case OF_PIXELS_BGRA:
    case OF_PIXELS_RGBA:
        mPixFormat = pixelFormat;
        return true;
    default:
        return false;
    }
    return false;
}

//----------------------------------------------
ofPixelFormat ofMediaFoundationPlayer::getPixelFormat() const {
    return mPixFormat;
}

//----------------------------------------------
ofPixels& ofMediaFoundationPlayer::getPixels() {
    if (!mBUpdatePixels && mFbo.isAllocated()) {
        mFbo.readToPixels(mPixels);
    }
    mBUpdatePixels = true;
    return mPixels;
}

//----------------------------------------------
const ofPixels& ofMediaFoundationPlayer::getPixels() const {
    mBUpdatePixels = true;
    return mPixels;
}

//----------------------------------------------
void ofMediaFoundationPlayer::handleMEEvent(DWORD aevent) {
    if (aevent != MF_MEDIA_ENGINE_EVENT_TIMEUPDATE) {
        ofLogVerbose("ofMediaFoundationPlayer") << MFEventToString(static_cast<MF_MEDIA_ENGINE_EVENT>(aevent));
    }

    switch (aevent) {
        case MF_MEDIA_ENGINE_EVENT_LOADEDMETADATA:
        {
            //mDuration = static_cast<float>(m_spMediaEngine->GetDuration());
            updateDuration();
            if (mDuration != mDuration || mDuration == std::numeric_limits<float>::infinity()) {
                mDuration = 0.f;
            } else {
                DWORD caps = 0;
                if (m_spEngineEx) {
                    m_spEngineEx->GetResourceCharacteristics(&caps);
                    mBCanSeek = (caps & MFMEDIASOURCE_CAN_SEEK) > 0;
                }
            }
            mBDone = false;
            mWidth = 0.f;
            mHeight = 0.f;
            DWORD w, h;
            if (SUCCEEDED(m_spMediaEngine->GetNativeVideoSize(&w, &h))) {
                mWidth = w;
                mHeight = h;

                if (mMeTexture) {
                    if (mMeTexture->getWidth() != mWidth || mMeTexture->getHeight() != mHeight) {
                        mMeTexture.reset();
                        mFbo.clear();
                    }
                }

                if (!mMeTexture) {
                    if (isUsingHWAccel()) {
                        ofLogVerbose(" ofMediaFoundationPlayer::handleMEEvent") << " creating a shared texture that is hw supported.";
                        mMeTexture = std::make_shared<SharedDXGLTexture>();
                    } else {
                        ofLogVerbose(" ofMediaFoundationPlayer::handleMEEvent") << " creating a WIC Texture manager.";
                        mMeTexture = std::make_shared<WICTextureManager>();
                    }
                    mMeTexture->allocate(mPixFormat, getWidth(), getHeight());
                    mMeTexture->create(m_d3dFormat);

                    ofFbo::Settings fsettings;
                    fsettings.internalformat = ofGetGLInternalFormatFromPixelFormat(mPixFormat);
                    fsettings.useDepth = false;
                    fsettings.useStencil = false;
                    fsettings.width = mWidth;
                    fsettings.height = mHeight;
                    fsettings.numSamples = 0;
                    mFbo.allocate(fsettings);
                    mFbo.begin(); {
                        ofClear(0, 0, 0, 255);
                    } mFbo.end();
                    mCopyTex = mFbo.getTexture();
                }
            }
            // in case this was called before load //
            setLoopState(getLoopState());
            //mBReady = true;
            mBLoaded = true;
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_LOADEDDATA:
        {
            //IMFMediaEngineEx::GetStreamAttribute
            //HRESULT GetStreamAttribute(
            //    [in]  DWORD       dwStreamIndex,
            //    [in]  REFGUID     guidMFAttribute,
            //    [out] PROPVARIANT * pvValue
            //);
            // MF_MT_FRAME_RATE
            DWORD nstreams;
            
            if (m_spEngineEx && SUCCEEDED(m_spEngineEx->GetNumberOfStreams(&nstreams)) ) {
                if (nstreams > 0) {

                    //MF_MT_FRAME_RATE                {UINT64 (HI32(Numerator),LO32(Denominator))}
                    {
                        PROPVARIANT pvar;
                        HRESULT hr = m_spEngineEx->GetStreamAttribute(0, MF_MT_FRAME_RATE, &pvar);
                        if (hr == S_OK) {
                            auto numerator = pvar.hVal.HighPart / pvar.hVal.LowPart;
                            auto denom = pvar.uhVal.HighPart / pvar.uhVal.LowPart;
                            mFramerate = (static_cast<float>(pvar.hVal.HighPart) / static_cast<float>(pvar.hVal.LowPart));
                        } else {
                            mFramerate = 1.f / 30.f;
                        }
                        PropVariantClear(&pvar);
                    }
                    updateDuration();
                }
            }

            //mBReady = true;
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_CANPLAY:
        {
            // Start the Playback
            //play();
            //stop();
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_PLAY:
        {
            mBPlaying = true;
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_PAUSE:
        {
            mBPlaying = false;
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_ENDED:
        {
            mBDone = true;
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_TIMEUPDATE:
        {
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_SEEKING:
        {
            mTimeStartedSeek = m_spMediaEngine->GetCurrentTime();
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_SEEKED:
        {
            auto ctime = m_spMediaEngine->GetCurrentTime();
            // looping videos don't fire off an ended event, so try here
            if (ctime < 0.1 && (ctime - mTimeStartedSeek < 0.05)) {
                mBDone = true;
            }

            break;
        }
        case MF_MEDIA_ENGINE_EVENT_ERROR:
        {
            if (m_spMediaEngine) {
                ComPtr<IMFMediaError> error;
                if (m_spMediaEngine->GetError(&error) == S_OK) {
                    USHORT errorCode = error->GetErrorCode();
                    MF_MEDIA_ENGINE_ERR meError = static_cast<MF_MEDIA_ENGINE_ERR>(errorCode);
                    ofLogError("ofMediaFoundationPlayer") << MFErrorToString(meError);
                    ofNotifyEvent(MFErrorEvent, meError, this);
                    close();
                }
            }
            break;

        }
        case MF_MEDIA_ENGINE_EVENT_DURATIONCHANGE:
        {
            updateDuration();
            break;
        }
        case MF_MEDIA_ENGINE_EVENT_FIRSTFRAMEREADY:
        {
            mBReady = true;
        }
    }
    MF_MEDIA_ENGINE_EVENT mfEvent = static_cast<MF_MEDIA_ENGINE_EVENT>(aevent);
    ofNotifyEvent(MFEngineEvent, mfEvent, this);
    
}

//-----------------------------------------
void ofMediaFoundationPlayer::updateDuration() {
    if (m_spMediaEngine) {
        mDuration = (m_spMediaEngine->GetDuration());
        ofLogVerbose("ofMediaFoundationPlayer") << " update duration: " << mDuration;
        if (mDuration != mDuration || mDuration == std::numeric_limits<double>::infinity()) {
            mDuration = 0.0;
        }
        if (mDuration > 0.0 && mFramerate > 0.f) {
            mEstimatedNumFrames = mDuration / (1.f / mFramerate);
        } else {
            mEstimatedNumFrames = 1;
        }
    }
}

