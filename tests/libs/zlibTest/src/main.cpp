#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <zlib.h>
#include <cstring>

class ofApp: public ofxUnitTestsApp{
	void run(){
		// the version string is compiled into the lib we downloaded/built —
		// a mismatch here usually means download_libs deployed the wrong
		// platform's archive, or a stale one
		ofxTest(strlen(zlibVersion()) > 0, "zlibVersion() returns a non-empty string");

		const std::string original = "openFrameworks zlib smoke test - compress then decompress this exact string and expect it back unchanged.";

		uLongf compressedSize = compressBound((uLong)original.size());
		std::vector<Bytef> compressed(compressedSize);
		int cres = compress(compressed.data(), &compressedSize, (const Bytef*)original.data(), (uLong)original.size());
		ofxTestEq(cres, Z_OK, "compress() succeeds");

		uLongf decompressedSize = (uLongf)original.size();
		std::vector<Bytef> decompressed(decompressedSize);
		int dres = uncompress(decompressed.data(), &decompressedSize, compressed.data(), compressedSize);
		ofxTestEq(dres, Z_OK, "uncompress() succeeds");

		std::string roundTripped((char*)decompressed.data(), decompressedSize);
		ofxTestEq(roundTripped, original, "round-tripped data matches original");
	}
};

int main(){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
