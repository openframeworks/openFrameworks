#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <openssl/evp.h>
#include <openssl/opensslv.h>
#include <sstream>
#include <iomanip>

class ofApp: public ofxUnitTestsApp{
	void run(){
		ofxTest(OPENSSL_VERSION_NUMBER > 0, "OPENSSL_VERSION_NUMBER is defined");

		// SHA-256 of the empty string is a well-known constant -- a good
		// canary that the linked openssl actually implements the digest
		// correctly, not just that the symbols resolve
		unsigned char digest[EVP_MAX_MD_SIZE];
		unsigned int digestLen = 0;
		EVP_MD_CTX* ctx = EVP_MD_CTX_new();
		ofxTest(ctx != nullptr, "EVP_MD_CTX_new() succeeds");

		int initOk = EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
		ofxTestEq(initOk, 1, "EVP_DigestInit_ex(sha256) succeeds");

		int finalOk = EVP_DigestFinal_ex(ctx, digest, &digestLen);
		ofxTestEq(finalOk, 1, "EVP_DigestFinal_ex() succeeds");
		EVP_MD_CTX_free(ctx);

		std::ostringstream hex;
		for(unsigned int i = 0; i < digestLen; i++){
			hex << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
		}
		ofxTestEq(hex.str(), std::string("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"), "SHA-256(\"\") matches the known constant");
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
