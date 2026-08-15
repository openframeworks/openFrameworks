#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"

// Round-trips a synthetic test pattern through FreeImage (via ofImage) for a
// handful of formats and checks the reloaded pixels actually match the
// pattern, not just that width/height came back right.
class ofApp: public ofxUnitTestsApp{

	static const int kBlockSize = 8;
	static const int kNumColors = 8;

	// small, fixed palette so the pattern survives GIF's 256-color quantize
	ofColor paletteColor(int i){
		static const ofColor palette[kNumColors] = {
			ofColor(230, 60, 60), ofColor(60, 230, 60), ofColor(60, 60, 230), ofColor(230, 230, 60),
			ofColor(230, 60, 230), ofColor(60, 230, 230), ofColor(250, 250, 250), ofColor(20, 20, 20)
		};
		return palette[i % kNumColors];
	}

	ofColor patternColor(int x, int y){
		int block = (x / kBlockSize) + (y / kBlockSize);
		return paletteColor(block);
	}

	ofImage makeTestImage(int w, int h){
		ofImage img;
		img.setUseTexture(false);
		img.allocate(w, h, OF_IMAGE_COLOR);
		for(int y = 0; y < h; y++){
			for(int x = 0; x < w; x++){
				img.setColor(x, y, patternColor(x, y));
			}
		}
		return img;
	}

	bool colorsClose(const ofColor & a, const ofColor & b, int tolerance){
		return abs((int)a.r - (int)b.r) <= tolerance
			&& abs((int)a.g - (int)b.g) <= tolerance
			&& abs((int)a.b - (int)b.b) <= tolerance;
	}

	void testFormat(const string & ext, int tolerance){
		const int w = 64, h = 48;
		ofImage img = makeTestImage(w, h);

		string path = "test_pattern." + ext;
		if(!ofxTest(img.save(path), "save " + ext)){
			return;
		}

		ofImage loaded;
		loaded.setUseTexture(false);
		if(!ofxTest(loaded.load(path), "load " + ext)){
			return;
		}

		ofxTestEq((int)loaded.getWidth(), w, "width " + ext);
		ofxTestEq((int)loaded.getHeight(), h, "height " + ext);

		// sample block centers (avoids compression artifacts at block edges)
		bool pixelsOk = true;
		for(int by = kBlockSize / 2; by < h; by += kBlockSize){
			for(int bx = kBlockSize / 2; bx < w; bx += kBlockSize){
				ofColor expected = patternColor(bx, by);
				ofColor actual = loaded.getColor(bx, by);
				if(!colorsClose(expected, actual, tolerance)){
					pixelsOk = false;
					ofLogError() << ext << " pixel(" << bx << "," << by << ") expected "
						<< (int)expected.r << "," << (int)expected.g << "," << (int)expected.b
						<< " got " << (int)actual.r << "," << (int)actual.g << "," << (int)actual.b;
				}
			}
		}
		ofxTest(pixelsOk, "pixel content matches pattern " + ext);
	}

	void run(){
		testFormat("png", 0);   // lossless
		testFormat("bmp", 0);   // lossless
		testFormat("tif", 0);   // lossless
		testFormat("tga", 0);   // lossless
		testFormat("gif", 8);   // 256-color quantize, small slack
		testFormat("jpg", 24);  // lossy DCT, sampled at block centers
	}
};

//========================================================================
int main( ){
	ofInit();
	auto window = std::make_shared<ofAppNoWindow>();
	auto app = std::make_shared<ofApp>();
	ofRunApp(window, app);
	return ofRunMainLoop();
}
