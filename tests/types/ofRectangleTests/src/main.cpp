#include "ofMain.h"
#include "ofAppNoWindow.h"
#include "ofxUnitTests.h"
#include <cfloat>
#include <climits>
#include <cmath>
#include <limits>
#include <random>

class ofApp: public ofxUnitTestsApp{
	// operator!= must always be the negation of operator==, and operator==
	// must give the expected answer.
	void testPair(const ofRectangle & a, const ofRectangle & b, bool wantEqual, const std::string & what){
		ofxTest((a == b) == wantEqual, what + ": operator== should be " + (wantEqual ? "true" : "false"));
		ofxTest((a != b) == !(a == b), what + ": operator!= must be the negation of operator==");
	}

	// Only the complement property, for pairs whose == result is not pinned
	// down by this test (non-finite values, accumulated rounding).
	void testComplement(const ofRectangle & a, const ofRectangle & b, const std::string & what){
		ofxTest((a != b) == !(a == b), what + ": operator!= must be the negation of operator==");
	}

	static ofRectangle withWidth(float w){
		return ofRectangle(0, 0, w, 1);
	}

	void run(){
		ofRectangle a(0, 0, 1, 1);
		ofRectangle b(0, 0, 1, 1);
		ofxTest(a == b, "identical rectangles should compare equal");
		ofxTest(!(a != b), "identical rectangles should not compare unequal");

		// width differs from a's by exactly one float ULP, well inside the
		// epsilon tolerance that operator== uses (ofIsFloatEqual), but
		// operator!= used to compare the raw fields directly, so the two
		// operators disagreed and both returned true for the same pair.
		ofRectangle c(0, 0, std::nextafter(1.0f, 2.0f), 1);
		ofxTest(a == c, "rectangles differing by one float ULP should compare equal under epsilon tolerance");
		ofxTest(!(a != c), "operator!= must be the logical negation of operator==");

		ofRectangle d(0, 0, 2, 2);
		ofxTest(a != d, "clearly different rectangles should compare unequal");
		ofxTest(!(a == d), "clearly different rectangles should not compare equal");

		// each field on its own
		testPair(a, ofRectangle(1, 0, 1, 1), false, "different x");
		testPair(a, ofRectangle(0, 1, 1, 1), false, "different y");
		testPair(a, ofRectangle(0, 0, 1, 2), false, "different height");
		testPair(ofRectangle(-5.5f, -7.25f, 3.125f, 9.75f), ofRectangle(-5.5f, -7.25f, 3.125f, 9.75f), true, "identical negative floats");

		// one ULP at different magnitudes and signs
		testPair(withWidth(1.0f), withWidth(std::nextafter(1.0f, 0.0f)), true, "1 ULP below 1");
		testPair(withWidth(100.0f), withWidth(std::nextafter(100.0f, 200.0f)), true, "1 ULP above 100");
		testPair(withWidth(1920.0f), withWidth(std::nextafter(1920.0f, 0.0f)), true, "1 ULP below 1920");
		testPair(withWidth(-3.5f), withWidth(std::nextafter(-3.5f, 0.0f)), true, "1 ULP from -3.5");
		testPair(withWidth(FLT_MAX), withWidth(std::nextafter(FLT_MAX, 0.0f)), true, "1 ULP below FLT_MAX");
		testPair(withWidth(0.3f), withWidth(0.1f + 0.2f), true, "0.1f + 0.2f vs 0.3f");
		testPair(withWidth(1.0f), withWidth(1.0f + 4 * std::numeric_limits<float>::epsilon()), false, "4 ULP above 1");
		testPair(withWidth(0.1f), withWidth(0.1000001f), false, "0.1 vs 0.1000001");

		// integer coordinates
		for(int v : {0, 1, -1, 7, 255, 1080, 1920, 3840, 65535, 1000000, -1000000}){
			testPair(ofRectangle(v, v, v, v), ofRectangle(v, v, v, v), true, "int " + ofToString(v) + " vs itself");
			testPair(ofRectangle(v, 0, 1, 1), ofRectangle(v + 1, 0, 1, 1), false, "int " + ofToString(v) + " vs +1");
		}
		testPair(ofRectangle(float(INT_MAX), 0, 1, 1), ofRectangle(float(INT_MAX), 0, 1, 1), true, "INT_MAX vs itself");
		testPair(ofRectangle(float(INT_MIN), 0, 1, 1), ofRectangle(float(INT_MIN), 0, 1, 1), true, "INT_MIN vs itself");
		// past 2^24 neighbouring ints are one float ULP apart (or the same float)
		testPair(ofRectangle(float(16777216), 0, 1, 1), ofRectangle(float(16777217), 0, 1, 1), true, "2^24 vs 2^24+1");
		testPair(ofRectangle(float(16777216), 0, 1, 1), ofRectangle(float(16777218), 0, 1, 1), true, "2^24 vs 2^24+2");
		testPair(ofRectangle(float(16777216), 0, 1, 1), ofRectangle(float(16777224), 0, 1, 1), false, "2^24 vs 2^24+8");
		testPair(ofRectangle(float(100000000), 0, 1, 1), ofRectangle(float(100000064), 0, 1, 1), false, "1e8 vs 1e8+64");

		// zero, signed zero and tiny values
		testPair(ofRectangle(0, 0, 0, 0), ofRectangle(-0.0f, -0.0f, -0.0f, -0.0f), true, "+0 vs -0");
		testPair(withWidth(0.0f), withWidth(std::numeric_limits<float>::denorm_min()), false, "0 vs denorm_min");
		testPair(withWidth(FLT_MIN), withWidth(FLT_MIN), true, "FLT_MIN vs itself");

		// non-finite values: ofIsFloatEqual is not meaningful here (inf - inf
		// is NaN), so only require that the two operators agree
		const float inf = std::numeric_limits<float>::infinity();
		const float nan = std::numeric_limits<float>::quiet_NaN();
		testComplement(withWidth(inf), withWidth(inf), "+inf vs +inf");
		testComplement(withWidth(inf), withWidth(-inf), "+inf vs -inf");
		testComplement(withWidth(nan), withWidth(nan), "NaN vs NaN");
		testComplement(withWidth(nan), withWidth(1.0f), "NaN vs 1");

		// accumulated floating point math
		ofRectangle r(12.5f, 33.25f, 640, 480);
		ofRectangle scaled = r;
		scaled.scale(1.1f);
		scaled.scale(1.0f / 1.1f);
		testComplement(r, scaled, "scale 1.1 then 1/1.1");
		ofRectangle moved = r;
		for(int i = 0; i < 10; i++) moved.translate(0.1f, 0.1f);
		moved.translate(-1.0f, -1.0f);
		testComplement(r, moved, "translate 10 x 0.1 then -1");
		ofRectangle centered = r;
		centered.scaleFromCenter(3.0f);
		centered.scaleFromCenter(1.0f / 3.0f);
		testComplement(r, centered, "scaleFromCenter 3 then 1/3");

		// random pairs: exact copies, 1-3 ULP nudges of one field, random
		// floats and random ints, compared in both directions
		std::mt19937 rng(8567);
		std::uniform_real_distribution<float> big(-1e6f, 1e6f), small(-1.0f, 1.0f);
		std::uniform_int_distribution<int> ints(-5000, 5000), ulps(1, 3), pick(0, 3), field(0, 3);
		int pairs = 0, disagreements = 0;
		for(int i = 0; i < 200000; i++){
			float v[4];
			for(float & f : v) f = (i % 5 == 0) ? float(ints(rng)) : ((i & 1) ? big(rng) : small(rng));
			ofRectangle p(v[0], v[1], v[2], v[3]);
			ofRectangle q = p;
			switch(pick(rng)){
			case 0: break;
			case 1:{
				float * f = &q.x + field(rng);
				for(int k = ulps(rng); k > 0; k--) *f = std::nextafter(*f, INFINITY);
				break;
			}
			case 2: q = ofRectangle(big(rng), big(rng), big(rng), big(rng)); break;
			case 3: q = ofRectangle(float(ints(rng)), float(ints(rng)), float(ints(rng)), float(ints(rng))); break;
			}
			pairs += 2;
			if((p != q) == (p == q)) disagreements++;
			if((q != p) == (q == p)) disagreements++;
		}
		ofxTestEq(disagreements, 0, "random pairs where operator!= is not the negation of operator== (" + ofToString(pairs) + " compared)");

		testInside();
	}

	// inside() includes the edge, and intersects(p0, p1) relies on it for
	// segment endpoints (#4871)
	void testInside(){
		ofRectangle r(0, 0, 100, 100);

		ofxTest(r.intersects(glm::vec2(0, 50), glm::vec2(50, 0)), "#4871: segment with both endpoints on the edge intersects");

		ofxTest(r.inside(glm::vec2(0, 0)), "top-left corner is inside");
		ofxTest(r.inside(glm::vec2(100, 0)), "top-right corner is inside");
		ofxTest(r.inside(glm::vec2(0, 100)), "bottom-left corner is inside");
		ofxTest(r.inside(glm::vec2(100, 100)), "bottom-right corner is inside");

		ofxTest(r.inside(glm::vec2(50, 0)), "point on top edge is inside");
		ofxTest(r.inside(glm::vec2(50, 100)), "point on bottom edge is inside");
		ofxTest(r.inside(glm::vec2(0, 50)), "point on left edge is inside");
		ofxTest(r.inside(glm::vec2(100, 50)), "point on right edge is inside");
		ofxTest(r.inside(50.f, 0.f), "inside(x, y) includes the edge");
		ofxTest(r.inside(glm::vec3(100, 50, 7)), "inside(vec3) includes the edge");

		ofxTest(r.inside(glm::vec2(50, 50)), "centre is inside");
		ofxTest(r.inside(glm::vec2(25, 25)), "interior point is inside");

		ofxTest(!r.inside(glm::vec2(-1, 50)), "point left of the rectangle is outside");
		ofxTest(!r.inside(glm::vec2(101, 50)), "point right of the rectangle is outside");
		ofxTest(!r.inside(glm::vec2(50, -1)), "point above the rectangle is outside");
		ofxTest(!r.inside(glm::vec2(50, 101)), "point below the rectangle is outside");

		ofxTest(r.inside(ofRectangle(0, 0, 100, 100)), "a rectangle is inside an identical rectangle");
		ofxTest(r.inside(ofRectangle(0, 10, 50, 50)), "a rectangle sharing an edge is inside");
		ofxTest(!r.inside(ofRectangle(-1, 10, 50, 50)), "a rectangle crossing an edge is not inside");
		ofxTest(r.inside(glm::vec2(0, 0), glm::vec2(100, 100)), "segment between two corners is inside");

		ofxTest(r.intersects(glm::vec2(25, 25), glm::vec2(75, 75)), "segment fully inside intersects");
		ofxTest(r.intersects(glm::vec2(-10, 50), glm::vec2(110, 50)), "segment crossing horizontally intersects");
		ofxTest(r.intersects(glm::vec2(50, -10), glm::vec2(50, 110)), "segment crossing vertically intersects");
		ofxTest(r.intersects(glm::vec2(50, 50), glm::vec2(150, 150)), "segment with one endpoint inside intersects");
		ofxTest(r.intersects(glm::vec2(0, 0), glm::vec2(100, 100)), "diagonal between corners intersects");
		ofxTest(!r.intersects(glm::vec2(-10, -10), glm::vec2(-5, -5)), "segment fully outside does not intersect");
		ofxTest(r.intersects(glm::vec2(0, 0), glm::vec2(100, 0)), "segment along the top edge intersects");
		ofxTest(r.intersects(glm::vec2(20, 100), glm::vec2(60, 100)), "segment along part of the bottom edge intersects");
		ofxTest(r.intersects(glm::vec2(0, 50), glm::vec2(-50, 50)), "segment touching the left edge from outside intersects");
		ofxTest(r.intersects(glm::vec2(100, 100), glm::vec2(150, 150)), "segment touching a corner from outside intersects");

		// rectangle / rectangle intersection is unchanged: sharing only an edge is not an intersection
		ofxTest(!r.intersects(ofRectangle(100, 0, 50, 50)), "rectangles sharing only an edge do not intersect");
		ofxTest(r.intersects(ofRectangle(99, 0, 50, 50)), "overlapping rectangles intersect");
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
