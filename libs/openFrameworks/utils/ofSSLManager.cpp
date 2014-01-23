// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofSSLManager.h"


const std::string ofSSLManager::DEFAULT_CA_LOCATION       = "ssl/cacert.pem";
const std::string ofSSLManager::DEFAULT_PRIVATE_KEY_FILE  = "ssl/privateKey.pem";
const std::string ofSSLManager::DEFAULT_CERTIFICATE_FILE  = "ssl/certificate.pem";


ofSSLManager::ofSSLManager():
    _clientContextInitialized(false),
    _serverContextInitialized(false)
{
    Poco::Net::SSLManager::instance();
}


ofSSLManager::~ofSSLManager()
{
    Poco::Net::SSLManager::instance().shutdown();
}


Poco::Net::Context::Ptr ofSSLManager::getDefaultServerContext()
{
    initializeServer(); // make sure it's initialized with something
    return Poco::Net::SSLManager::instance().defaultServerContext();
}


Poco::Net::Context::Ptr ofSSLManager::getDefaultClientContext()
{
    initializeClient(); // make sure it's initialized with something
    return Poco::Net::SSLManager::instance().defaultClientContext();
}


void ofSSLManager::initializeClient(Poco::Net::Context::Ptr pContext)
{
    ofSSLManager& manager = ofSSLManager::instance();

    if(0 != pContext)
    {
        Poco::Net::SSLManager::instance().initializeClient(0, 0, pContext);
        manager._clientContextInitialized = true;
    }
    else if (!manager._clientContextInitialized)
    {
        std::string caLocation = ofToDataPath(DEFAULT_CA_LOCATION, true);

        Poco::File caLocationFile(caLocation);

        if(!caLocationFile.exists())
        {
            caLocation = "";
        }

        Poco::Net::Context::Ptr pContext = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE,
                                                                  caLocation);

        Poco::Net::SSLManager::instance().initializeClient(0, 0, pContext);
        manager._clientContextInitialized = true;
    }
}


void ofSSLManager::initializeServer(Poco::Net::Context::Ptr pContext)
{
    ofSSLManager& manager = ofSSLManager::instance();

    if(0 != pContext)
    {
        Poco::Net::SSLManager::instance().initializeServer(0, 0, pContext);
        manager._serverContextInitialized = true;
    }
    else if (!manager._serverContextInitialized)
    {
        std::string privateKeyFile = ofToDataPath(DEFAULT_PRIVATE_KEY_FILE, true);
        std::string certificateFile = ofToDataPath(DEFAULT_CERTIFICATE_FILE, true);
        std::string caLocation = ofToDataPath(DEFAULT_CA_LOCATION, true);

        Poco::File caLocationFile(caLocation);

        if(!caLocationFile.exists())
        {
            caLocation = "";
        }

        Poco::Net::Context::Ptr pContext = new Poco::Net::Context(Poco::Net::Context::SERVER_USE,
                                                                  privateKeyFile,
                                                                  certificateFile,
                                                                  caLocation);

        Poco::Net::SSLManager::instance().initializeServer(0, 0, pContext);
        manager._serverContextInitialized = true;
    }
}


namespace
{
    static Poco::SingletonHolder<ofSSLManager> singleton;
    // We keep this in an anonymous namespace to ensure that everything is
    // shut down correctly at the very end of the application lifecycle.
}


ofSSLManager& ofSSLManager::instance()
{
    return *singleton.get();
}
