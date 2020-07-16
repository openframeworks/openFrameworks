/*
 * Test #6378: min/max value coupling between sliders and associated parameter.
 *
 * ofxIntSlider and ofxFloatSlider are not tested, because their parameter
 * references the original directly and hence work 'out of the box'.
 *
 * Tests for changing min/max values via parameters have been disabled since
 * this has not been implemented yet.
 */
#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include "ofxGui.h"

template <typename PT>
struct DimInfo {
    string name;
    PT min;
    PT max;
};

class ofApp : public ofxUnitTestsApp {
    // Test whether the nested slider controls have expected min/max values
    // for the given dimensions.
    template <typename PT>
    bool _testInternalControls(const vector<DimInfo<PT>>& dims, ofxGuiGroup& slider) {
        bool allGood = true;
        for (auto&& dim : dims) {
            bool minEq = (dim.min == ((ofxSlider<PT>*)slider.getControl(dim.name))->getMin());
            bool maxEq = (dim.max == ((ofxSlider<PT>*)slider.getControl(dim.name))->getMax());
            allGood &= minEq && maxEq;
        }
        return allGood;
    }

    // Also valid for ofxVec3Slider, ofxVec4Slider and ofxPointSlider
    void testVec2Slider() {
        static const glm::vec2 minInit = {0, 1}, maxInit = {100, 101};
        static const glm::vec2 min1 = {10, 11}, max1 = {90, 91};
        static const glm::vec2 min2 = {20, 21}, max2 = {80, 81};

        ofParameter<glm::vec2> p("", {40, 41}, minInit, maxInit);
        ofxVec2Slider s(p);

        vector<DimInfo<float>> dimsInit = { { "x", minInit.x, maxInit.x }, { "y", minInit.y, maxInit.y } };
        ofxTest(_testInternalControls(dimsInit, s), "vec2 min/max: initial values (internal controls)");

        p.setMin(min1); p.setMax(max1);  // Set min/max via parameter
        vector<DimInfo<float>> dims1 = { { "x", min1.x, max1.x }, { "y", min1.y, max1.y } };
        // ofxTest(_testInternalControls(dims1, s), "vec2 min/max: change param -> slider (internal controls)");

        s.setMin(min2); s.setMax(max2);  // Set min/max via slider widget
        vector<DimInfo<float>> dims2 = { { "x", min2.x, max2.x }, { "y", min2.y, max2.y } };
        ofxTest(_testInternalControls(dims2, s), "vec2 min/max: change slider (internal controls)");
        ofxTest(p.getMin() == s.getMin() && p.getMax() == s.getMax(), "vec2 min/max: change slider -> param");
    }

    void testRectangleSlider() {
        static const ofRectangle minInit = {0, 1, 5, 6}, maxInit = {100, 101, 105, 106};
        static const ofRectangle min1 = {10, 11, 15, 16}, max1 = {90, 91, 95, 96};
        static const ofRectangle min2 = {20, 21, 25, 26}, max2 = {80, 81, 85, 86};

        ofParameter<ofRectangle> p("", {40, 41, 45, 46}, minInit, maxInit);
        ofxRectangleSlider s(p);

        vector<DimInfo<float>> dimsInit = {
            { "x", minInit.x, maxInit.x }, { "y", minInit.y, maxInit.y },
            { "width", minInit.width, maxInit.width }, { "height", minInit.height, maxInit.height },
        };
        ofxTest(_testInternalControls(dimsInit, s), "rect min/max: initial values (internal controls)");

        p.setMin(min1); p.setMax(max1);  // Set min/max via parameter
        vector<DimInfo<float>> dims1 = {
            { "x", min1.x, max1.x }, { "y", min1.y, max1.y },
            { "width", min1.width, max1.width }, { "height", min1.height, max1.height },
        };
        // ofxTest(_testInternalControls(dims1, s), "rect min/max: change param -> slider (internal controls)");

        s.setMin(min2); s.setMax(max2);  // Set min/max via slider widget
        vector<DimInfo<float>> dims2 = {
            { "x", min2.x, max2.x }, { "y", min2.y, max2.y },
            { "width", min2.width, max2.width }, { "height", min2.height, max2.height },
        };
        ofxTest(_testInternalControls(dims2, s), "rect min/max: change slider (internal controls)");
        ofxTest(p.getMin() == s.getMin() && p.getMax() == s.getMax(), "rect min/max: change slider -> param");
    }

    // Also valid for ofxShortColorSlider and ofxFloatColorSlider
    void testColorSlider() {
        static const ofColor minInit = {0, 1, 5, 6}, maxInit = {100, 101, 105, 106};
        static const ofColor min1 = {10, 11, 15, 16}, max1 = {90, 91, 95, 96};
        static const ofColor min2 = {20, 21, 25, 26}, max2 = {80, 81, 85, 86};

        ofParameter<ofColor> p("", {40, 41, 45, 46}, minInit, maxInit);
        ofxColorSlider s(p);

        vector<DimInfo<unsigned char>> dimsInit = {
            { "r", minInit.r, maxInit.r }, { "g", minInit.g, maxInit.g },
            { "b", minInit.b, maxInit.b }, { "a", minInit.a, maxInit.a },
        };
        ofxTest(_testInternalControls(dimsInit, s), "color min/max: initial values (internal controls)");

        p.setMin(min1); p.setMax(max1);  // Set min/max via parameter
        vector<DimInfo<unsigned char>> dims1 = {
            { "r", min1.r, max1.r }, { "g", min1.g, max1.g },
            { "b", min1.b, max1.b }, { "a", min1.a, max1.a },
        };
        // ofxTest(_testInternalControls(dims1, s), "color min/max: change param -> slider (internal controls)");

        s.setMin(min2); s.setMax(max2);  // Set min/max via slider widget
        vector<DimInfo<unsigned char>> dims2 = {
            { "r", min2.r, max2.r }, { "g", min2.g, max2.g },
            { "b", min2.b, max2.b }, { "a", min2.a, max2.a },
        };
        ofxTest(_testInternalControls(dims2, s), "color min/max: change slider (internal controls)");
        ofxTest(p.getMin() == s.getMin() && p.getMax() == s.getMax(), "color min/max: change slider -> param");
    }

    void run() {
        testVec2Slider();
        testRectangleSlider();
        testColorSlider();
    }
};

int main( ) {
    ofInit();
    auto window = make_shared<ofAppNoWindow>();
    auto app = make_shared<ofApp>();
    ofRunApp(window, app);
    return ofRunMainLoop();
}
