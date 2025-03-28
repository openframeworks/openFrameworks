#include "ofAppRunner.h"
#include "ofURLFileLoader.h"
#include "ofUtils.h"
#include "ofFileUtils.h"
#include "ofLog.h"

using std::map;
using std::set;
using std::string;

#if !defined(TARGET_IMPLEMENTS_URL_LOADER)
#include <curl/curl.h>
#include "ofThreadChannel.h"
#include "ofThread.h"
static bool curlInited = false;
#define MAX_POSTFIELDS_SIZE (1024 * 1024)
#if !defined(NO_OPENSSL)
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <iostream>
#include <fstream>
#define CERTIFICATE_FILE "ssl/cacert.pem"
#define PRIVATE_KEY_FILE "ssl/cacert.key"
#endif
#endif

int ofHttpRequest::nextID = 0;

ofEvent<ofHttpResponse> & ofURLResponseEvent() {
	static ofEvent<ofHttpResponse> * event = new ofEvent<ofHttpResponse>;
	return *event;
}

#if !defined(TARGET_IMPLEMENTS_URL_LOADER)
std::mutex responseMutex;
class ofURLFileLoaderImpl : public ofThread, public ofBaseURLFileLoader {
public:
	ofURLFileLoaderImpl();
	~ofURLFileLoaderImpl();
	ofHttpResponse get(const string & url);
	int getAsync(const string & url, const string & name = ""); // returns id
	ofHttpResponse saveTo(const string & url, const of::filesystem::path & path);
	int saveAsync(const string & url, const of::filesystem::path & path);
	void remove(int id);
	void clear();
	void stop();
#if !defined(NO_OPENSSL)
	bool checkValidCertifcate(const std::string& cert_file);
	void createSSLCertificate();
#endif
	ofHttpResponse handleRequest(const ofHttpRequest & request);
	int handleRequestAsync(const ofHttpRequest & request); // returns id

protected:
	// threading -----------------------------------------------
	void threadedFunction();
	void start();
	void update(ofEventArgs & args); // notify in update so the notification is thread safe

private:
	// perform the requests on the thread

	ofThreadChannel<ofHttpRequest> requests;
	ofThreadChannel<ofHttpResponse> responses;
	ofThreadChannel<int> cancelRequestQueue;
	set<int> cancelledRequests;
};

ofURLFileLoaderImpl::ofURLFileLoaderImpl() {
	if (!curlInited) {
		curl_global_init(CURL_GLOBAL_ALL);
	}
}

ofURLFileLoaderImpl::~ofURLFileLoaderImpl() {
	clear();
	stop();
}

ofHttpResponse ofURLFileLoaderImpl::get(const string & url) {
	ofHttpRequest request(url, url);
	return handleRequest(request);
}

int ofURLFileLoaderImpl::getAsync(const string & url, const string & name) {
	ofHttpRequest request(url, name.empty() ? url : name);
	requests.send(request);
	start();
	return request.getId();
}

ofHttpResponse ofURLFileLoaderImpl::saveTo(const string & url, const of::filesystem::path & path) {
	ofHttpRequest request(url, path.string(), true);
	return handleRequest(request);
}

int ofURLFileLoaderImpl::saveAsync(const string & url, const of::filesystem::path & path) {
	ofHttpRequest request(url, path.string(), true);
	requests.send(request);
	start();
	return request.getId();
}

void ofURLFileLoaderImpl::remove(int id) {
	cancelRequestQueue.send(id);
}

void ofURLFileLoaderImpl::clear() {
	ofHttpResponse resp;
	ofHttpRequest req;
	while (requests.tryReceive(req)) { }
	while (responses.tryReceive(resp)) { }
}

void ofURLFileLoaderImpl::start() {
	if (!isThreadRunning()) {
		ofAddListener(ofEvents().update, this, &ofURLFileLoaderImpl::update);
		startThread();
	}
}

void ofURLFileLoaderImpl::stop() {
	stopThread();
	requests.close();
	responses.close();
	waitForThread();
	curl_global_cleanup();
}

#if !defined(NO_OPENSSL)
bool ofURLFileLoaderImpl::checkValidCertifcate(const std::string & cert_file) {
	try {
		FILE * fp = fopen(cert_file.c_str(), "r");
		if (!fp) return false;
		X509 * cert = PEM_read_X509(fp, NULL, NULL, NULL);
		fclose(fp);
		if (!cert) return false;
		time_t current_time = time(NULL);
		int notBefore = X509_cmp_time(X509_get0_notBefore(cert), &current_time);
		int notAfter = X509_cmp_time(X509_get0_notAfter(cert), &current_time);
		X509_free(cert);
		return (notBefore <= 0 && notAfter >= 0);
	} catch (const std::exception & e) {
		ofLogError("ofURLFileLoader") << "Exception in checkValidCertifcate: " << e.what();
		return false;
	} catch (...) {
		ofLogError("ofURLFileLoader") << "Unknown error occurred in checkValidCertifcate.";
		return false;
	}
}


void ofURLFileLoaderImpl::createSSLCertificate() {
	try {
		EVP_PKEY * pkey = nullptr;
		X509 * x509 = nullptr;
		EVP_PKEY_CTX * pkey_ctx = EVP_PKEY_CTX_new_from_name(NULL, "RSA", NULL);
		if (!pkey_ctx) {
			throw std::runtime_error("Error initializing key generation context");
		}
		if (EVP_PKEY_keygen_init(pkey_ctx) <= 0 || EVP_PKEY_CTX_set_rsa_keygen_bits(pkey_ctx, 2048) <= 0 || EVP_PKEY_keygen(pkey_ctx, &pkey) <= 0) {
			EVP_PKEY_CTX_free(pkey_ctx);
			throw std::runtime_error("Error generating RSA key");
		}
		EVP_PKEY_CTX_free(pkey_ctx);
		x509 = X509_new();
		if (!x509) {
			EVP_PKEY_free(pkey);
			throw std::runtime_error("Failed to create new X509 certificate");
		}
		ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
		X509_gmtime_adj(X509_get_notBefore(x509), 0);
		X509_gmtime_adj(X509_get_notAfter(x509), 31536000L); // 1 year
		X509_set_pubkey(x509, pkey);
		X509_NAME * name = X509_get_subject_name(x509);
		if (!name) {
			X509_free(x509);
			EVP_PKEY_free(pkey);
			throw std::runtime_error("Failed to get subject name from X509 certificate");
		}
		X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"US", -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"Local Machine", -1, -1, 0);
		X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"Local Root CA", -1, -1, 0);
		X509_set_issuer_name(x509, name);
		if (X509_sign(x509, pkey, EVP_sha256()) == 0) {
			X509_free(x509);
			EVP_PKEY_free(pkey);
			throw std::runtime_error("Error signing the certificate");
		}
		BIO * keyBio = BIO_new(BIO_s_mem());
		BIO * certBio = BIO_new(BIO_s_mem());
		if (!keyBio || !certBio) {
			if (keyBio) BIO_free(keyBio);
			if (certBio) BIO_free(certBio);
			X509_free(x509);
			EVP_PKEY_free(pkey);
			throw std::runtime_error("Failed to create BIO objects for key/cert storage");
		}
		PEM_write_bio_PrivateKey(keyBio, pkey, nullptr, nullptr, 0, nullptr, nullptr);
		PEM_write_bio_X509(certBio, x509);
		char * keyData = nullptr;
		long keyLen = BIO_get_mem_data(keyBio, &keyData);
		std::string keyStr(keyData, keyLen);
		char * certData = nullptr;
		long certLen = BIO_get_mem_data(certBio, &certData);
		std::string certStr(certData, certLen);
		ofBuffer keyBuffer, certBuffer;
		keyBuffer.set(keyStr.c_str(), keyLen);
		certBuffer.set(certStr.c_str(), certLen);
		if (!ofDirectory::createDirectory("ssl")) {
			ofLogWarning("ofURLFileLoader") << "Could not create ssl directory";
		}
		if (!ofBufferToFile(ofToDataPath(PRIVATE_KEY_FILE), keyBuffer)) {
			throw std::runtime_error("Failed to save private key to file");
		}
		if (!ofBufferToFile(ofToDataPath(CERTIFICATE_FILE), certBuffer)) {
			throw std::runtime_error("Failed to save certificate to file");
		}
		BIO_free(keyBio);
		BIO_free(certBio);
		EVP_PKEY_free(pkey);
		X509_free(x509);
		ofLogNotice("ofURLFileLoader") << "Root certificate and private key generated and saved";
	} catch (const std::exception & e) {
		ofLogError("ofURLFileLoader") << "Exception in createSSLCertificate: " << e.what();
	} catch (...) {
		ofLogError("ofURLFileLoader") << "Unknown error occurred in createSSLCertificate.";
	}
}
#endif


void ofURLFileLoaderImpl::threadedFunction() {
	setThreadName("ofURLFileLoader " + ofToString(getThreadId()));
	while (isThreadRunning()) {
		int cancelled = 0;
		while (cancelRequestQueue.tryReceive(cancelled)) {
			cancelledRequests.insert(cancelled);
		}
		ofHttpRequest request;
		if (requests.receive(request)) {
			if (cancelledRequests.find(request.getId()) == cancelledRequests.end()) {
				ofHttpResponse response(handleRequest(request));
				int status = response.status;
				if (!responses.send(std::move(response))) {
					break;
				}
				if (status == -1) {
					// retry
					requests.send(request);
				}
			} else {
				cancelledRequests.erase(cancelled);
			}
		} else {
			break;
		}
	}
}

namespace {
size_t saveToFile_cb(void * buffer, size_t size, size_t nmemb, void * userdata) {
	std::lock_guard<std::mutex> lock(responseMutex);
	auto saveTo = (ofFile *)userdata;
	saveTo->write((const char *)buffer, size * nmemb);
	return size * nmemb;
}

size_t saveToMemory_cb(void * buffer, size_t size, size_t nmemb, void * userdata) {
	std::lock_guard<std::mutex> lock(responseMutex);
	auto response = (ofHttpResponse *)userdata;
	response->data.append((const char *)buffer, size * nmemb);
	return size * nmemb;
}

size_t readBody_cb(void * ptr, size_t size, size_t nmemb, void * userdata) {
	std::lock_guard<std::mutex> lock(responseMutex);
	auto body = (std::string *)userdata;
	if (size * nmemb < 1) {
		return 0;
	}
	if (!body->empty()) {
		auto sent = std::min(size * nmemb, body->size());
		memcpy(ptr, body->c_str(), sent);
		*body = body->substr(sent);
		return sent;
	}
	return 0; /* no more data left to deliver */
}
}

ofHttpResponse ofURLFileLoaderImpl::handleRequest(const ofHttpRequest & request) {
	std::unique_ptr<CURL, void (*)(CURL *)> curl = std::unique_ptr<CURL, void (*)(CURL *)>(curl_easy_init(), curl_easy_cleanup);
	if (!curl) {
		ofLogError("ofURLFileLoader") << "curl_easy_init() failed!";
		return ofHttpResponse(request, -1, "CURL initialization failed");
	}
	curl_slist * headers = nullptr;
	curl_version_info_data *version = curl_version_info( CURLVERSION_NOW );
	if(request.verbose) {
		CURLcode ret = curl_easy_setopt(curl.get(), CURLOPT_VERBOSE, 1L);
		if (ret != CURLE_OK) {
			ofLogWarning() << "cURL error: " << curl_easy_strerror(ret);
		}
		if (version) {
			std::string userAgent = std::string("curl/") + version->version;
			curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, userAgent.c_str());
		} else {
			curl_easy_setopt(curl.get(), CURLOPT_USERAGENT, "curl/unknown");
		}
	}
	if(version->features & CURL_VERSION_SSL) {
#if !defined(NO_OPENSSL)
		const std::string caPath = "ssl";
		const std::string caFile = "ssl/cacert.pem";
		if (ofFile::doesFileExist(ofToDataPath(CERTIFICATE_FILE)) && checkValidCertifcate(ofToDataPath(CERTIFICATE_FILE))) {
			ofLogVerbose("ofURLFileLoader") << "SSL valid certificate found";
		} else {
			ofLogVerbose("ofURLFileLoader") << "SSL certificate not found - generating";
			createSSLCertificate();
		}
		curl_easy_setopt(curl.get(), CURLOPT_CAPATH, ofToDataPath(caPath, true).c_str());
		curl_easy_setopt(curl.get(), CURLOPT_CAINFO, ofToDataPath(caFile, true).c_str());
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, false);
#else
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 1L);
#endif
		curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 2);
	}
	curl_easy_setopt(curl.get(), CURLOPT_URL, request.url.c_str());
	curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl.get(), CURLOPT_MAXREDIRS, 20L);

	if (request.contentType != "") {
		headers = curl_slist_append(headers, ("Content-Type: " + request.contentType).c_str());
	}
	if(request.close)
		headers = curl_slist_append(headers, "Connection: close");
	if(version->features & CURL_VERSION_BROTLI) {
		headers = curl_slist_append(headers, "Accept-Encoding: br");
	}
	if(version->features & CURL_VERSION_LIBZ) {
		headers = curl_slist_append(headers, "Accept-Encoding: gzip");
	}
	for (map<string, string>::const_iterator it = request.headers.cbegin(); it != request.headers.cend(); it++) {
		headers = curl_slist_append(headers, (it->first + ": " + it->second).c_str());
	}

	if (headers) {
		curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headers);
	}
	std::string body = request.body;
	if (!request.body.empty()) {
		if (request.method == ofHttpRequest::PUT || request.body.size() > MAX_POSTFIELDS_SIZE) { // If request is an upload (e.g., file upload)
			curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 1L);
			curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, readBody_cb);
			curl_easy_setopt(curl.get(), CURLOPT_READDATA, &body);
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, 0L);
		} else { // If request is a normal POST
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, request.body.size());
			curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, request.body.c_str());
		}
	} else {
		curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDSIZE, 0L);
		curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, "");
		curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, nullptr);
		curl_easy_setopt(curl.get(), CURLOPT_READDATA, nullptr);
	}
	if (request.method == ofHttpRequest::GET) {
		curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 1L);
		curl_easy_setopt(curl.get(), CURLOPT_POST, 0L);
		curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 0L);
	}
	else if (request.method == ofHttpRequest::PUT) {
		curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl.get(), CURLOPT_POST, 0L);
		curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 0L);
	}
	else if (request.method == ofHttpRequest::POST) {
		curl_easy_setopt(curl.get(), CURLOPT_POST, 1L);
		curl_easy_setopt(curl.get(), CURLOPT_UPLOAD, 0L);
		curl_easy_setopt(curl.get(), CURLOPT_HTTPGET, 0L);
	}

	if (request.timeoutSeconds > 0) {
		curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, request.timeoutSeconds);
	}
	if (request.headerOnly) {
		curl_easy_setopt(curl.get(), CURLOPT_NOBODY, 1L);
	}

	// start request and receive response
	ofHttpResponse response(request, 0, "");
	CURLcode err = CURLE_OK;
	if (request.saveTo) {
		ofFile saveTo(request.name, ofFile::WriteOnly, true);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &saveTo);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, saveToFile_cb);
		err = curl_easy_perform(curl.get());
	} else {
		curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, saveToMemory_cb);
		err = curl_easy_perform(curl.get());
	}
	if (err == CURLE_OK) {
		long http_code = 0;
		curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &http_code);
		response.status = http_code;
	} else {
		response.error = curl_easy_strerror(err);
		response.status = -1;
	}

	if (headers) {
		curl_slist_free_all(headers);
	}

	return response;
}

int ofURLFileLoaderImpl::handleRequestAsync(const ofHttpRequest & request) {
	requests.send(request);
	start();
	return request.getId();
}

void ofURLFileLoaderImpl::update(ofEventArgs & args) {
	ofHttpResponse response;
	while (responses.tryReceive(response)) {
		try {
			std::lock_guard<std::mutex> lock(responseMutex);
			if (response.request.done) {
				response.request.done(response);
			} 
		} catch (...) {
		}

		ofNotifyEvent(ofURLResponseEvent(), response);
	}
}

ofURLFileLoader::ofURLFileLoader()
	: impl(new ofURLFileLoaderImpl) { }
#endif

#ifdef TARGET_EMSCRIPTEN
	#include "ofxEmscriptenURLFileLoader.h"
ofURLFileLoader::ofURLFileLoader()
	: impl(new ofxEmscriptenURLFileLoader) { }
#endif

#ifdef TARGET_OF_TVOS
#include "ofxtvOSURLFileLoader.h"
ofURLFileLoader::ofURLFileLoader()
: impl(new ofxtvOSURLFileLoader) { }
#endif

ofHttpResponse ofURLFileLoader::get(const string & url) {
	return impl->get(url);
}

int ofURLFileLoader::getAsync(const string & url, const string & name) {
	return impl->getAsync(url, name);
}

ofHttpResponse ofURLFileLoader::saveTo(const string & url, const of::filesystem::path & path) {
	return impl->saveTo(url, path);
}

int ofURLFileLoader::saveAsync(const string & url, const of::filesystem::path & path) {
	return impl->saveAsync(url, path);
}

void ofURLFileLoader::remove(int id) {
	impl->remove(id);
}

void ofURLFileLoader::clear() {
	impl->clear();
}

void ofURLFileLoader::stop() {
	impl->stop();
}

ofHttpResponse ofURLFileLoader::handleRequest(const ofHttpRequest & request) {
	return impl->handleRequest(request);
}

int ofURLFileLoader::handleRequestAsync(const ofHttpRequest & request) {
	return impl->handleRequestAsync(request);
}

static bool initialized = false;
static ofURLFileLoader & getFileLoader() {
	static ofURLFileLoader * fileLoader = new ofURLFileLoader;
	initialized = true;
	return *fileLoader;
}

ofHttpRequest::ofHttpRequest()
	: saveTo(false)
	, method(GET)
	, id(nextID++) {
}

ofHttpRequest::ofHttpRequest(const string & url, const string & name, bool saveTo, bool autoClose, bool verbose)
	: url(url)
	, name(name)
	, saveTo(saveTo)
	, method(GET)
	, id(nextID++)
	, close(autoClose)
	, verbose(verbose){
}

int ofHttpRequest::getId() const {
	return id;
}

int ofHttpRequest::getID() {
	return id;
}

ofHttpResponse::ofHttpResponse()
	: status(0) {
}

ofHttpResponse::ofHttpResponse(const ofHttpRequest & request, const ofBuffer & data, int status, const string & error)
	: request(request)
	, data(data)
	, status(status)
	, error(error) {
}

ofHttpResponse::ofHttpResponse(const ofHttpRequest & request, int status, const string & error)
	: request(request)
	, status(status)
	, error(error) {
}

ofHttpResponse::operator ofBuffer &() {
	return data;
}

ofHttpResponse ofLoadURL(const string & url) {
	return getFileLoader().get(url);
}

int ofLoadURLAsync(const string & url, const string & name) {
	return getFileLoader().getAsync(url, name);
}

ofHttpResponse ofSaveURLTo(const string & url, const of::filesystem::path & path) {
	return getFileLoader().saveTo(url, path);
}

int ofSaveURLAsync(const string & url, const of::filesystem::path & path) {
	return getFileLoader().saveAsync(url, path);
}

void ofRemoveURLRequest(int id) {
	getFileLoader().remove(id);
}

void ofRemoveAllURLRequests() {
	getFileLoader().clear();
}

void ofStopURLLoader() {
	getFileLoader().stop();
}

void ofURLFileLoaderShutdown() {
	if (initialized) {
		ofRemoveAllURLRequests();
		ofStopURLLoader();
	}
}
