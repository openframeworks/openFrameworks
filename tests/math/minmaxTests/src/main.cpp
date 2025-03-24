#include "ofMain.h"
#include "ofxUnitTests.h"
#include "ofAppNoWindow.h"

class ofApp: public ofxUnitTestsApp {
public:
    void run(){
		// testing the std:: pass-through (same types compared)
		ofxTest(ofMin(static_cast<int32_t>(-5),static_cast<int32_t>(5)) == -5, "min) I32");
		ofxTest(ofMin(static_cast<uint32_t>(1),static_cast<uint32_t>(5)) == 1, "min) U32");
		ofxTest(ofMin(static_cast<char>(3),static_cast<char>(5)) == 3, "min) char");
		ofxTest(ofMin(static_cast<size_t>(20),static_cast<size_t>(std::numeric_limits<size_t>::max())) == 20, "min) size_t");
		ofxTest(ofMin(static_cast<float>(-5.1),static_cast<float>(5)) == static_cast<float>(-5.1), "min) F32");
		ofxTest(ofMin(static_cast<double>(23),static_cast<double>(std::numeric_limits<double>::min())) == std::numeric_limits<double>::min(), "min) U32");

		ofxTest(ofMax(static_cast<int32_t>(-5),static_cast<int32_t>(5)) == 5, "max) I32");
		ofxTest(ofMax(static_cast<uint32_t>(1),static_cast<uint32_t>(5)) == 5, "max) U32");
		ofxTest(ofMax(static_cast<char>(3),static_cast<char>(5)) == 5, "max) char");
		ofxTest(ofMax(static_cast<size_t>(20),static_cast<size_t>(std::numeric_limits<size_t>::max())) == std::numeric_limits<size_t>::max(), "max) size_t");
		ofxTest(ofMax(static_cast<float>(-5.1),static_cast<float>(5.1)) == static_cast<float>(5.1), "max) F32");
		ofxTest(ofMax(static_cast<double>(23),static_cast<double>(std::numeric_limits<double>::max())) == std::numeric_limits<double>::max(), "max) U32");

		// testing the heterogenous types, both sides (implementation is mirrored)
		ofxTest(ofMin(static_cast<int>(4), static_cast<float>(-4.0)) == -4, "min) I32 vs F32");
		ofxTest(ofMin(static_cast<float>(4), static_cast<int>(std::numeric_limits<int>::lowest())) == std::numeric_limits<int>::lowest(), "min) F32 vs I32.lowest");
		ofxTest(ofMin(static_cast<int>(4), static_cast<double>(std::numeric_limits<double>::lowest())) == std::numeric_limits<double>::lowest(), "min) I32 vs F64.lowest");
		ofxTest(ofMin(static_cast<float>(std::numeric_limits<float>::lowest()), static_cast<double>(std::numeric_limits<double>::lowest())) == std::numeric_limits<double>::lowest(), "min) F32.lowest vs F64.lowest");
		ofxTest(ofMin(static_cast<float>(-1), static_cast<uint16_t>(-4.0)) == -1, "min) F32 vs U16");
		ofxTest(ofMin(static_cast<float>(-1), static_cast<uint64_t>(-4.0)) == -1, "min) F32 vs U64");
		ofxTest(ofMin(static_cast<uint64_t>(-4.0), static_cast<float>(-1) ) == -1, "min) U64 vs F32");
		ofxTest(ofMin(static_cast<size_t>(10), static_cast<float>(-1) ) == -1, "min) size_t vs F32");
		ofxTest(ofMin(static_cast<size_t>(10), static_cast<float>(11) ) == 10, "min) size_t vs F32");

		ofxTest(ofMax(static_cast<int>(4), static_cast<float>(-4.0)) == 4, "max) I32 vs F32");
		ofxTest(ofMax(static_cast<float>(4), static_cast<int>(std::numeric_limits<int>::max())) == std::numeric_limits<int>::max(), "max) F32 vs I32.max");
		ofxTest(ofMax(static_cast<int>(4), static_cast<double>(std::numeric_limits<double>::max())) == std::numeric_limits<double>::max(), "max) I32 vs F64.max");
		ofxTest(ofMax(static_cast<float>(std::numeric_limits<float>::max()), static_cast<double>(std::numeric_limits<double>::max())) == std::numeric_limits<double>::max(), "max) F32.max vs F64.max");
		ofxTest(ofMax(static_cast<float>(std::numeric_limits<float>::lowest()), static_cast<double>(std::numeric_limits<double>::lowest())) == std::numeric_limits<float>::lowest(), "max) F32.min vs F64.min");
		ofxTest(ofMax(static_cast<float>(1), static_cast<uint16_t>(4.0)) == 4, "max) F32 vs U16");
		ofxTest(ofMax(static_cast<float>(10), static_cast<uint64_t>(4.0)) == 10, "max) F32 vs U64");
		ofxTest(ofMax(static_cast<uint64_t>(12), static_cast<float>(-1) ) == 12, "max) U64 vs F32");
		ofxTest(ofMax(static_cast<size_t>(10), static_cast<float>(1) ) == 10, "max) size_t vs F32");
		ofxTest(ofMax(static_cast<size_t>(10), static_cast<float>(11) ) == 11, "max) size_t vs F32");
    }
};

int main( ){
    ofInit();
    auto window = std::make_shared<ofAppNoWindow>();
    auto app = std::make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
