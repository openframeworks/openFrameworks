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


#pragma once


#include "Poco/BasicEvent.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Delegate.h"
#include "Poco/File.h"
#include "Poco/SharedPtr.h"
#include "Poco/SingletonHolder.h"
#include "Poco/Net/Context.h"
#include "Poco/Net/NetSSL.h"
#include "Poco/Net/PrivateKeyPassphraseHandler.h"
#include "Poco/Net/InvalidCertificateHandler.h"
#include "Poco/Net/SSLManager.h"
#include "ofUtils.h"
#include "ofEvents.h"


/// \class ofSSLManager
/// \brief A class to simplify client and server SSL Context management.
///
/// Default OpenSSL security contexts are represented by the Poco::Net:Context
/// class and managed by the managed by the Poco::Net::SSLManager class.  Poco
/// Sockets, Streams and Sessions can be explicity instantiated with a
/// Poco::Net::Context or they can (more conveniently) employ a default SSL
/// Client or Server Context.  For applications that implement
/// Poco::Util::Application, default Context setup and configuration trivially
/// done via config files. But, since ofBaseApp does not extend
/// Poco::Util::Application, special care must be taken when creating and using
/// Poco's default Client and Server Contexts.
///
/// ofSSLManager adds a layer between the openFrameworks user and
/// the Poco::Net::SSLManager to ensure that the default contexts are configured
/// in an openFrameworks-friendly way (e.g. providing a default location for
/// certificate authority files, SSL certificate and Private Key files, the
/// default security settings and mechanisms for registering SSL verification
/// event callbacks.
///
/// ofSSLManager allows the user to either accept the default ofSSLManager
/// Client and Server Contexts or provide a custom Context during application
/// setup by calling:
///
///     ofSSLManager::initializeClient(...)
///
/// or:
///
///     ofSSLManager::initializeServer(...)
///
/// The user must to interact with the default SSL Context management system via
/// the ofSSLManager singleton, rather than calling Poco::Net::SSLManager
/// directly.
///
/// Finally, in order to simplify the certificate verification process,
/// ofSSLManager configures Contexts to seek their verification and private key
/// passwords via an event callback. Thus, users wishing to manually verify
/// Client and Server private key passwords, SSL certificates that are not
/// automatically accepted based on the certificate authority settings, etc must
/// add listeners for SSL verification callbacks BEFORE any calls to any of the
/// following methods:
///
///     ofSSLManager::defaultServerContext();
///     ofSSLManager::defaultServerContext();
///     ofSSLManager::initializeServer(...);
///     ofSSLManager::initializeClient(...);
///
/// Further information about registering these event callbacks can be found in
/// the ofSSLManager::registerAllEvents(), ofSSLManager::registerClientEvents()
/// and ofSSLManager::registerServerEvents() method documentation.
class ofSSLManager
{
public:
    /// \brief Get the default Server Context via the ofSSLManager.
    ///
    /// This is the same context that is returned via:
    ///
    ///     Poco::Net::SSLManager::instance().defaultServerContext();
    ///
    /// But unlike that call, the ofSSLManager::defaultServerContext() call
    /// ensures that ofSSLManager::initializeServer() is called prior to the
    /// Poco::Net::SSLManager call.  This ensures that the default Server
    /// Context has been configured first by ofSSLManager, rather than
    /// Poco::Net::SSLManager.
    ///
    /// \returns A pointer to the default Server Context.
    static Poco::Net::Context::Ptr getDefaultServerContext();

    /// \brief Get the default Server Context via the ofSSLManager.
    ///
    /// This is the same context that is returned via:
    ///
    ///     Poco::Net::SSLManager::instance().defaultClientContext();
    ///
    /// But unlike that call, the ofSSLManager::defaultClientContext() call
    /// ensures that ofSSLManager::initializeClient() is called prior to the
    /// Poco::Net::SSLManager call.  This ensures that the default Client
    /// Context has been configured first by ofSSLManager, rather than
    /// Poco::Net::SSLManager.
    ///
    /// \returns A pointer to the default Client Context.
    static Poco::Net::Context::Ptr getDefaultClientContext();

    /// \brief Initialize a SSL Server Context.
    ///
    /// If the user wishes to use non-default Context (see Poco::Net::Context
    /// for a list of options), the user must create a Context and initialize it
    /// with this function immediately in the ofBaseApp::setup() function.
    /// Otherwise, ofSSLManager::initializeSlient() will be configured with the
    /// ofSSLManager default settings.
    ///
    /// \param pContext A Poco::Net::Context::Ptr to set as the default.
    static void initializeServer(Poco::Net::Context::Ptr pContext = 0);

    /// \brief Initialize a SSL Client Context.
    ///
    /// If the user wishes to use non-default Context (see Poco::Net::Context
    /// for a list of options), the user must create a Context and initialize it
    /// with this function immediately in the ofBaseApp::setup() function.
    /// Otherwise, ofSSLManager::initializeClient() will be configured with the
    /// ofSSLManager default settings.
    ///
    /// \param pContext A Poco::Net::Context::Ptr to set as the default.
    static void initializeClient(Poco::Net::Context::Ptr pContext = 0);


    /// \brief Register the listener class for all Client and Server SSL events.
    /// Applications that do not implement these callbacks will not be given the
    /// opportunity to manually approve SSL cert errors.  Listening classes must
    /// have callbacks in the form:
    ///
    ///     void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLPrivateKeyPassphraseRequired(std::string& args);
    ///
    ///
    /// A listener might call this class in the ofBaseApp::setup() method like
    /// this:
    ///
    ///    ofSSLManager::registerAllEvents(this);
    ///
    /// \param listener A pointer to the class containing all callbacks.
    template<class ListenerClass>
    static void registerAllEvents(ListenerClass* listener);

    /// \brief Unregister the listener for all Client and Server SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    template<class ListenerClass>
    static void unregisterAllEvents(ListenerClass* listener);

    /// \brief Register the listener class for all Client SSL events.
    ///
    /// Applications that do not implement these callbacks will not be given the
    /// opportunity to manually approve SSL cert errors.  Listening classes must
    /// have callbacks in the form:
    ///
    ///     void onSSLClientVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLPrivateKeyPassphraseRequired(std::string& args);
    ///
    /// A listener might call this class in the ofBaseApp::setup() method like
    /// this:
    ///
    ///    ofSSLManager::registerClientEvents(this);
    ///
    /// \param listener A pointer to the class containing client callbacks.
    template<class ListenerClass>
    static void registerClientEvents(ListenerClass* listener);

    /// \brief Unregister the listener class for all Client SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    template<class ListenerClass>
    static void unregisterClientEvents(ListenerClass* listener);

    /// \brief Register the listener class for all Server SSL events.
    ///
    /// Applications that do not implement these callbacks will not be
    /// given the opportunity to manually approve SSL cert errors. Listening
    /// classes must have callbacks in the form:
    ///
    ///     void onSSLServerVerificationError(Poco::Net::VerificationErrorArgs& args);
    ///     void onSSLPrivateKeyPassphraseRequired(std::string& args);
    ///
    ///
    /// A listener might call this class in the ofBaseApp::setup() method like
    /// this:
    ///
    ///    ofSSLManager::registerServerEvents(this);
    ///
    /// \param listener A pointer to the class containing client callbacks.
    template<class ListenerClass>
    static void registerServerEvents(ListenerClass* listener);

    /// \brief Unregister the listener class for all Server SSL events.
    /// \param listener A pointer to the class containing all callbacks.
    template<class ListenerClass>
    static void unregisterServerEvents(ListenerClass* listener);

    /// \brief The default location of the certificate authority bundle.
    ///
    /// The certificate authority bundle can be extracted from the cURL website.
    /// More information is here: http://curl.haxx.se/docs/caextract.html
    static const std::string DEFAULT_CA_LOCATION;

    /// \brief The default location of the private key pem file.
    ///
    /// SSL Certificate files and their private key counterparts can be
    /// purchased or self-signed.  More information on generating self-signed
    /// certificates is here:
    /// https://devcenter.heroku.com/articles/ssl-certificate-self
    static const std::string DEFAULT_PRIVATE_KEY_FILE;

    /// \brief The default location of the private key pem file.
    ///
    /// SSL Certificate files and their private key counterparts can be
    /// purchased or self-signed.  More information on generating self-signed
    /// certificates is here:
    /// https://devcenter.heroku.com/articles/ssl-certificate-self
    static const std::string DEFAULT_CERTIFICATE_FILE;

private:
    ofSSLManager();
    ofSSLManager(const ofSSLManager&);
    ofSSLManager& operator = (const ofSSLManager&);

    /// \brief Destroys the ofSSLManager.
    ~ofSSLManager();

    /// \brief An instance of the ofSSLManager.
    ///
    /// All static methods access the singleton via this method. Usually there
    /// is no need for a user to call this method.
    ///
    /// \returns A reference to the singleton.
    static ofSSLManager& instance();

    bool _clientContextInitialized;
        ///< \brief True iff ofSSLManager initialized its own Client Context.

    bool _serverContextInitialized;
        ///< \brief True iff ofSSLManager initialized its own Server Context.

    friend class Poco::SingletonHolder<ofSSLManager>;
        ///< \brief Allow Poco::SingletonHolder to call the private constructor.

};


template<class ListenerClass>
void ofSSLManager::registerAllEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.ClientVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired += Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template<class ListenerClass>
void ofSSLManager::unregisterAllEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.ClientVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired -= Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template<class ListenerClass>
void ofSSLManager::registerClientEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ClientVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired += Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template<class ListenerClass>
void ofSSLManager::unregisterClientEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ClientVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLClientVerificationError);
    manager.PrivateKeyPassphraseRequired -= Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template<class ListenerClass>
void ofSSLManager::registerServerEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      += Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.PrivateKeyPassphraseRequired += Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


template<class ListenerClass>
void ofSSLManager::unregisterServerEvents(ListenerClass* listener)
{
    ofSSLManager::instance();
    Poco::Net::SSLManager& manager = Poco::Net::SSLManager::instance();
    manager.ServerVerificationError      -= Poco::delegate(listener, &ListenerClass::onSSLServerVerificationError);
    manager.PrivateKeyPassphraseRequired -= Poco::delegate(listener, &ListenerClass::onSSLPrivateKeyPassphraseRequired);
}


/// \brief Convert a Poco::Net::VerificationErrorArgs to a std::string.
///
/// \param args The Poco::Net::VerificationErrorArgs to convert.
/// \returns An easy-to-read representation of the error arguments.
template <>
inline std::string ofToString(const Poco::Net::VerificationErrorArgs& args)
{
    std::stringstream os;

    static int width = 15;
    static std::string fmt = "%dd %H:%M:%S.%i";

    os << "======================================================" << std::endl;
    os << "Error: " << std::endl;
    os << std::setw(width) << "Message: " << args.errorMessage() << std::endl;
    os << std::setw(width) << "#: " << args.errorNumber() << std::endl;
    os << std::setw(width) << "Depth: " << args.errorDepth() << std::endl;
    os << "======================================================" << std::endl;
    const Poco::Net::X509Certificate& cert = args.certificate();
    os << "Certificate: " << std::endl;
    os << std::setw(width) << "Issued By: " << cert.issuerName() << std::endl;
    os << std::setw(width) << "Subject Name: " << cert.subjectName() << std::endl;
    os << std::setw(width) << "Common Name: " << cert.commonName() << std::endl;
    os << std::setw(width) << "Valid From: " << Poco::DateTimeFormatter::format(cert.validFrom(), fmt) << std::endl;
    os << std::setw(width) << "Expires On: " << Poco::DateTimeFormatter::format(cert.expiresOn(), fmt) << std::endl;
    os << "======================================================" << std::endl;

    return os.str();
}
