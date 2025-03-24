#include "ofApp.h"

using namespace of::random; // OK in implementation files
using namespace glm;

void ofApp::setup() {

	panel_.setup("Engine Seeding");
	panel_.add(seed_);
	panel_.add(reconstruct_);

	seed_.addListener(this, &ofApp::seed);
	reconstruct_.addListener(this, &ofApp::reconstruct);
	reconstruct();
	
	vec4 aRandom(gaussian<vec2>(10.0, 1.0), exponential<vec2>({0.1, 0.5}));
	ofLogNotice("assembled random vec4") << aRandom;
}

void ofApp::draw() {

	panel_.draw();

	ofDrawBitmapStringHighlight(shuffled_string_, 230, 80);
	if (of::random::engine()->is_deterministic()) {
		ofDrawBitmapStringHighlight("engine is deterministic (seeded)", 230, 40, ofColor::black, ofColor::green);
	} else {
		ofDrawBitmapStringHighlight("engine is non-deterministic", 230, 40, ofColor::black, ofColor::white);
	}
	ofDrawBitmapString("the 8 numbers are randomly shuffled by the engine immediately after seeding, \nso for a given seed, will always procure the same order. when \nnon-deterministic, the order will appear to be random", 230, 100);
	ofDrawBitmapString("hit spacebar and watch the console for more", 20, 320);
}

void ofApp::perform() {
	{
		
		// ofRandom is always float, to be as close as possible to original behaviour
		ofLogNotice("ofRandom()") << ofRandom();
		ofLogNotice("ofRandom(5)") << ofRandom(5);
		ofLogNotice("ofRandom(5.0)") << ofRandom(5.0);
		ofLogNotice("ofRandom(5,10)") << ofRandom(5, 10);
		ofLogNotice("ofRandom(-1,1)") << ofRandom(-1,1);
		ofLogNotice("ofRandom(5.0,10.0)") << ofRandom(5.0, 10.0);

		// ofRandomUniform honors type of args
		ofLogNotice("uniform()") << of::random::uniform();
		ofLogNotice("uniform(5)") << uniform(5);
		ofLogNotice("uniform(-10,10)") << uniform(-10,10);
		ofLogNotice("uniform(5.0)") << uniform(5.0);
		ofLogNotice("uniform(5,10)") << uniform(5, 10);
		ofLogNotice("uniform(5.0,10.0)") << uniform(5.0, 10.0);
		ofLogNotice("uniform(-5.0,-10.0)") << uniform(5.0, 10.0);

		// but can be forced by specifying the <type>
		ofLogNotice("uniform<double>()") << uniform<double>();
		ofLogNotice("uniform<double>(5)") << uniform<double>(5);
		ofLogNotice("uniform<double>(5,10)") << uniform<double>(5, 10);
		// ofLogNotice("uniform<int>()") << uniform<int>(); // will not compile (makes no sense: always 0)
		ofLogNotice("uniform<int>(5)") << uniform<int>(5);
		ofLogNotice("uniform<int>(5,10)") << uniform<int>(5, 10);
		// <T> wins over args:
		ofLogNotice("uniform<int>(5.0)") << uniform<int>(5.0);
		ofLogNotice("uniform<int>(5.0,6.0)") << uniform<int>(5.0, 6.0);
		ofLogNotice("uniform<vec2>()") << uniform<vec2>();
		ofLogNotice("uniform<vec2>(5)") << uniform<vec2>(5);
		ofLogNotice("uniform<vec2>(5,10)") << uniform<vec2>(5, 10);
		ofLogNotice("uniform<vec2>({0,10},{10,20})") << uniform<vec2>({ 0, 10 }, { 10, 20 });
		ofLogNotice("uniform<vec3>()") << uniform<vec3>();
		ofLogNotice("uniform<vec4>(5)") << uniform<vec4>(5);
		ofLogNotice("uniform<vec3>(5,10)") << uniform<vec3>(5, 10);
		ofLogNotice("uniform<vec3>(-1,1)") << uniform<vec3>(-1,1);
		ofLogNotice("uniform<vec3>({0,10,20},{10,20,30})") << uniform<vec3>({ 0, 10, 20 }, { 10, 20, 30 });
		ofLogNotice("uniform<vec4>({0,10,20,30},{10,20,30, 40})") << uniform<vec4>({ 0, 10, 20, 30 }, { 10, 20, 30, 40 });

		// ofRandomUniform honors type of args
		ofLogNotice("ofRandomUniform()") << ofRandomUniform();
		ofLogNotice("ofRandomUniform(5)") << ofRandomUniform(5);
		ofLogNotice("ofRandomUniform(5.0)") << ofRandomUniform(5.0);
		ofLogNotice("ofRandomUniform(5,10)") << ofRandomUniform(5, 10);
		ofLogNotice("ofRandomUniform(5.0,10.0)") << ofRandomUniform(5.0, 10.0);

		// but can be forced by specifying the <type>
		ofLogNotice("ofRandomUniform<double>(5)") << ofRandomUniform<double>(5);
		ofLogNotice("ofRandomUniform<double>(5,10)") << ofRandomUniform<double>(5, 10);
		//	ofLogNotice("ofRandomUniform<int>()") << ofRandomUniform<int>(); // will not compile (makes no sense: always 0)
		ofLogNotice("ofRandomUniform<int>(5)") << ofRandomUniform<int>(5);
		ofLogNotice("ofRandomUniform<int>(5,10)") << ofRandomUniform<int>(5, 10);
		ofLogNotice("ofRandomUniform<vec2>()") << ofRandomUniform<vec2>();
		ofLogNotice("ofRandomUniform<vec2>(5)") << ofRandomUniform<vec2>(5);
		ofLogNotice("ofRandomUniform<vec2>(5,10)") << ofRandomUniform<vec2>(5, 10);
		ofLogNotice("ofRandomUniform<vec2>({0,10},{10,20})") << ofRandomUniform<vec2>({ 0, 10 }, { 10, 20 });
		ofLogNotice("ofRandomUniform<vec3>()") << ofRandomUniform<vec3>();
		ofLogNotice("ofRandomUniform<vec3>(5)") << ofRandomUniform<vec3>(5);
		ofLogNotice("ofRandomUniform<vec3>(5,10)") << ofRandomUniform<vec3>(5, 10);
		ofLogNotice("ofRandomUniform<vec3>({0,10,20},{10,20,30})") << ofRandomUniform<vec3>({ 0, 10, 20 }, { 10, 20, 30 });

		// normal / gaussian requires 2 args (no defaults) // return type defaults to float
		// "gaussian" is preferred as non-qualified-"normal" (when using namespace of::random as here) will clash with a macOS define

		ofLogNotice("normal(10, 2)") << of::random::normal(10, 2);
		ofLogNotice("normal<double>(10, 2)") << of::random::normal<double>(10, 2);
		ofLogNotice("normal<int>(10, 2)") << of::random::normal<int>(10, 2);
		ofLogNotice("normal<vec2>(10, 2)") << of::random::normal<vec2>(10, 2);
		ofLogNotice("normal<vec2>({10, 20},{2, 1})") << of::random::normal<vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("normal<vec3>(10,2)") << of::random::normal<vec3>(10, 2);
		ofLogNotice("normal<vec3>({10,20,30},{2,1,.5})") << of::random::normal<vec3>({ 10, 20, 30 }, { 2, 1, .5 });
		ofLogNotice("normal<vec4>({10,20,30, 40},{2,1,.5, .25})") << of::random::normal<vec4>({ 10, 20, 30, 40 }, { 2, 1, .5, .25 });
		ofLogNotice("ofRandomNormal(10, 2)") << ofRandomNormal(10, 2);
		ofLogNotice("ofRandomNormal<double>(10, 2)") << ofRandomNormal<double>(10, 2);
		ofLogNotice("ofRandomNormal<int>(10, 2)") << ofRandomNormal<int>(10, 2);
		ofLogNotice("ofRandomNormal<vec2>(10, 2)") << ofRandomNormal<vec2>(10, 2);
		ofLogNotice("ofRandomNormal<vec2>({10, 20},{2, 1})") << ofRandomNormal<vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("ofRandomNormal<vec3>(10,2)") << ofRandomNormal<vec3>(10, 2);
		ofLogNotice("ofRandomNormal<vec3>({10,20,30},{2,1,.5})") << ofRandomNormal<vec3>({ 10, 20, 30 }, { 2, 1, .5 });
		ofLogNotice("ofRandomNormal<vec4>({10,20,30, 40},{2,1,.5, .25})") << ofRandomNormal<vec4>({ 10, 20, 30, 40 }, { 2, 1, .5, .25 });

		// testing the forwarding
		ofLogNotice("ofRandomGaussian(10, 2)") << ofRandomGaussian(10, 2);
		ofLogNotice("ofRandomGaussian<double>(10, 2)") << ofRandomGaussian<double>(10, 2);
		ofLogNotice("ofRandomGaussian<int>(10, 2)") << ofRandomGaussian<int>(10, 2);
		ofLogNotice("ofRandomGaussian<vec2>(10, 2)") << ofRandomGaussian<vec2>(10, 2);
		ofLogNotice("ofRandomGaussianvec2>({10, 20},{2, 1})") << ofRandomGaussian<vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("ofRandomGaussian<vec3>(10,2)") << ofRandomGaussian<vec3>(10, 2);
		ofLogNotice("ofRandomGaussian<vec3>({10,20,30},{2,1,.5})") << ofRandomGaussian<vec3>({ 10, 20, 30 }, { 2, 1, .5 });

		ofLogNotice("gaussian aka normal(10, 2)") << of::random::gaussian(10, 2);
		ofLogNotice("gaussian aka normal<double>(10, 2)") << gaussian<double>(10, 2);
		ofLogNotice("gaussian aka normal<int>(10, 2)") << gaussian<int>(10, 2);
		ofLogNotice("gaussian aka normal<vec2>(10, 2)") << gaussian<vec2>(10, 2);
		ofLogNotice("gaussian aka normal<vec2>({10, 20},{2, 1})") << gaussian<vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("gaussian aka normal<vec3>(10,2)") << gaussian<vec3>(10, 2);
		ofLogNotice("gaussian aka normal<vec3>({10,20,30},{2,1,.5})") << gaussian<vec3>({ 10, 20, 30 }, { 2, 1, .5 });

		// exponential requires 1 args (no defaults)

		ofLogNotice("exponential(2.5)") << of::random::exponential(2.5);
		ofLogNotice("exponential<double>(2.5)") << exponential<double>(2.5);
		ofLogNotice("exponential<int>(2.5)") << exponential<int>(2.5);
		ofLogNotice("exponential<vec2>(2.5)") << exponential<vec2>(2.5);
		ofLogNotice("exponential<vec4>(2.5)") << exponential<vec4>(2.5);
		ofLogNotice("exponential<vec2>({2.5, 10})") << exponential<vec2>({ 2.5, 10 });
		ofLogNotice("exponential<vec3>(2.5, 10, 0.1") << exponential<vec3>(2.5);
		ofLogNotice("exponential<vec3>({2.5, 10, 0.1}") << exponential<vec3>({ 2.5, 10, 0.1 });
		ofLogNotice("exponential<vec4>({2.5, 10, 0.1, 5}") << exponential<vec4>({ 2.5, 10, 0., 5 });

		ofLogNotice("ofRandomExponential(2.5)") << ofRandomExponential(2.5);
		ofLogNotice("ofRandomExponential<double>(2.5)") << ofRandomExponential<double>(2.5);
		ofLogNotice("ofRandomExponential<int>(2.5)") << ofRandomExponential<int>(2.5);
		ofLogNotice("ofRandomExponential<vec2>(2.5)") << ofRandomExponential<vec2>(2.5);
		ofLogNotice("ofRandomExponential<vec2>({2.5, 10})") << ofRandomExponential<vec2>({ 2.5, 10 });
		ofLogNotice("ofRandomExponential<vec3>(2.5, 10, 0.1") << ofRandomExponential<vec3>(2.5);
		ofLogNotice("ofRandomExponential<vec3>({2.5, 10, 0.1}") << ofRandomExponential<vec3>({ 2.5, 10, 0.1 });

		// poisson requires 1 arg (no defaults), and return type is integer-only
		// these conditions are enforced by the std::distributions themselves
		// however to facilitate glm usage (even if the use case is strange), they are cast to float in glm templates

		ofLogNotice("poisson(10)") << of::random::poisson(10);
//		 ofLogNotice("poisson<float>(2.5)") << poisson<float>(5.5);   expected not to compile by libcpp static assert
		ofLogNotice("poisson<int>(2.5)") << poisson<long long>(5.5);
		ofLogNotice("poisson<vec2>(2.5)") << poisson<vec2>(5.5);
		ofLogNotice("poisson<vec2>({2.5, 10})") << poisson<vec2>({ 5.5, 10 });
		ofLogNotice("poisson<vec3>(2.5, 10, 0.1") << poisson<vec3>(5.5);
		ofLogNotice("poisson<vec3>({2.5, 10, 0.1}") << poisson<vec3>({ 5.5, 10, 0.1 });
		ofLogNotice("poisson<vec4>({2.5, 10, 0.1, 5}") << poisson<vec4>({ 5.5, 10, 0.1, 5 });

		ofLogNotice("ofRandomPoisson(10)") << ofRandomPoisson(10);
		// ofLogNotice("ofRandomPoisson<float>(2.5)") << ofRandomPoisson<float>(5.5);  // expected not to compile by libcpp static assert
		ofLogNotice("ofRandomPoisson<int>(2.5)") << ofRandomPoisson<long long>(5.5);
		ofLogNotice("ofRandomPoisson<vec2>(2.5)") << ofRandomPoisson<vec2>(5.5);
		ofLogNotice("ofRandomPoisson<vec2>({2.5, 10})") << ofRandomPoisson<vec2>({ 5.5, 10 });
		ofLogNotice("ofRandomPoisson<vec3>(2.5, 10, 3") << ofRandomPoisson<vec3>(5.5);
		ofLogNotice("ofRandomPoisson<vec3>({2.5, 10, 3}") << ofRandomPoisson<vec3>({ 5.5, 10, 0.1 });

		// bernoulli requires 1 arg (no defaults), and return type is bool
		// again, glm usage case is cast to float

		ofLogNotice("bernoulli<vec3>(.33)") << of::random::bernoulli<vec3>(.33);
		ofLogNotice("bernoulli<vec2>(.5)") << bernoulli<vec2>(.5);
		ofLogNotice("bernoulli<float>(.5)") << bernoulli<float>(.5);
		ofLogNotice("bernoulli<bool>(.5)") << bernoulli<bool>(.5);
		ofLogNotice("bernoulli(.5)") << bernoulli(.5);

		ofLogNotice("ofRandomBernoulli<vec3>({.1,.5,.9})") << ofRandomBernoulli<vec3>({.1,.5,.9});
		ofLogNotice("ofRandomBernoulli<vec3>(.33)") << ofRandomBernoulli<vec3>(.33);
		ofLogNotice("ofRandomBernoulli<vec2>(.5)") << ofRandomBernoulli<vec2>(.5);
		ofLogNotice("ofRandomBernoulli<float>(.5)") << ofRandomBernoulli<float>(.5);
		ofLogNotice("ofRandomBernoulli<bool>(.5)") << ofRandomBernoulli<bool>(.5);
		ofLogNotice("ofRandomBernoulli(.5)") << ofRandomBernoulli(.5);

		ofLogNotice("sometimes aka bernoulli<vec3>(.33)") << of::random::sometimes<vec3>(.33);
		ofLogNotice("sometimes aka bernoulli<vec2>(.5)") << sometimes<vec2>(.5);
		ofLogNotice("sometimes aka bernoulli<float>(.5)") << sometimes<float>(.5);
		ofLogNotice("sometimes aka bernoulli<bool>(.5)") << sometimes<bool>(.5);
		ofLogNotice("sometimes aka bernoulli(.5)") << sometimes(.5);

		ofLogNotice("ofRandomSometimes aka bernoulli<vec3>(.33)") << ofRandomSometimes<vec3>(.33);
		ofLogNotice("ofRandomSometimes aka bernoulli<vec2>(.5)") << ofRandomSometimes<vec2>(.5);
		ofLogNotice("ofRandomSometimes aka bernoulli<float>(.5)") << ofRandomSometimes<float>(.5);
		ofLogNotice("ofRandomSometimes aka bernoulli<bool>(.5)") << ofRandomSometimes<bool>(.5);
		ofLogNotice("ofRandomSometimes aka bernoulli(.5)") << ofRandomSometimes(.5);

		// lognormal requires 2 args (no defaults)

		ofLogNotice("lognormal(10, 2)") << of::random::lognormal(10, 2);
		ofLogNotice("lognormal<double>(10, 2)") << lognormal<double>(10, 2);
		ofLogNotice("lognormal<int>(10, 2)") << lognormal<int>(10, 2);
		ofLogNotice("lognormal<vec2>(10, 2)") << lognormal<vec2>(10, 2);
		ofLogNotice("lognormal<vec2>({10, 20},{2, 1})") << lognormal<vec2>({ 10, 2 }, { 2, .5 });
		ofLogNotice("lognormal<vec3>(10,2)") << lognormal<vec3>(10, 2);
		ofLogNotice("lognormal<vec3>({10,20,30},{2,1,.5})") << lognormal<vec3>({ 10, 2, 1 }, { 2, .5, .25 });
		ofLogNotice("lognormal<vec4>({10,20,30, 40},{2,1,.5, .25})") << lognormal<vec4>({ 10, 2, 1, 30 }, { 2,1, .5, .25 });

		ofLogNotice("ofRandomLognormal(10, 2)") << ofRandomLognormal(10, 2);
		ofLogNotice("ofRandomLognormal<double>(10, 2)") << ofRandomLognormal<double>(10, 2);
		ofLogNotice("ofRandomLognormal<int>(10, 2)") << ofRandomLognormal<int>(10, 2);
		ofLogNotice("ofRandomLognormal<vec2>(10, 2)") << ofRandomLognormal<vec2>(10, 2);
		ofLogNotice("ofRandomLognormal<vec2>({10, 20},{2, 1})") << ofRandomLognormal<vec2>({ 10, 2 }, { 2, .5 });
		ofLogNotice("ofRandomLognormal<vec3>(10,2)") << ofRandomLognormal<vec3>(10, 2);
		ofLogNotice("ofRandomLognormal<vec3>({10,2,1},{2,.5,.25})") << ofRandomLognormal<vec3>({ 10, 2, 1 }, { 2, .5, .25 });

		// gamma requires 2 args (no defaults)

		ofLogNotice("gamma(3, 5)") << of::random::gamma(3, 5);
		ofLogNotice("gamma<double>(3, 5)") << gamma<double>(3, 5);
		ofLogNotice("gamma<int>(3, 5)") << gamma<int>(3, 5);
		ofLogNotice("gamma<vec2>(3, 5)") << gamma<vec2>(3, 5);
		ofLogNotice("gamma<vec2>({3, 20},{5, 1})") << gamma<vec2>({ 3, 20 }, { 5, 1 });
		ofLogNotice("gamma<vec3>(3, 5)") << gamma<vec3>(3, 5);
		ofLogNotice("gamma<vec3>({3,20, 1},{5,1,.5})") << gamma<vec3>({ 3, 20, 1 }, { 5, 1, .5 });
		ofLogNotice("gamma<vec4>({3,20, 1, .5},{5,1,.5, .25})") << gamma<vec4>({ 3, 20, 1 , .5}, { 5, 1, .5, .25 });

		ofLogNotice("ofRandomGamma(3, 5)") << ofRandomGamma(3, 5);
		ofLogNotice("ofRandomGamma<double>(3, 5)") << ofRandomGamma<double>(3, 5);
		ofLogNotice("ofRandomGamma<int>(3, 5)") << ofRandomGamma<int>(3, 5);
		ofLogNotice("ofRandomGamma<vec2>(3, 5)") << ofRandomGamma<vec2>(3, 5);
		ofLogNotice("ofRandomGamma<vec2>({3, 20},{5, 1})") << ofRandomGamma<vec2>({ 3, 20 }, { 5, 1 });
		ofLogNotice("ofRandomGamma<vec3>(3, 5)") << ofRandomGamma<vec3>(3, 5);
		ofLogNotice("ofRandomGamma<vec3>({3,20, 1},{5,1,.5})") << ofRandomGamma<vec3>({ 3, 20, 1 }, { 5, 1, .5 });

		// chi-squared requires 1 arg (no defaults)

		ofLogNotice("ofRandomChiSquared(4)") << ofRandomChiSquared(4);
		ofLogNotice("ofRandomChiSquared<double>(4)") << ofRandomChiSquared<double>(4);
		ofLogNotice("ofRandomChiSquared<int>(4)") << ofRandomChiSquared<int>(4);
		ofLogNotice("ofRandomChiSquared<vec2>(4)") << ofRandomChiSquared<vec2>(4);
		ofLogNotice("ofRandomChiSquared<vec2>({4, 10})") << ofRandomChiSquared<vec2>({ 4, 10 });
		ofLogNotice("ofRandomChiSquared<vec3>(4)") << ofRandomChiSquared<vec3>(4);
		ofLogNotice("ofRandomChiSquared<vec4>(4)") << ofRandomChiSquared<vec4>(4);
		ofLogNotice("ofRandomChiSquared<vec3>({4,10,100})") << ofRandomChiSquared<vec3>({ 4, 10, 100 });
		ofLogNotice("ofRandomChiSquared<vec4>({4,10,100, 50})") << ofRandomChiSquared<vec4>({ 4, 10, 100, 50 });

		ofLogNotice("chi_squared(4)") << of::random::chi_squared(4);
		ofLogNotice("chi_squared<double>(4)") << chi_squared<double>(4);
		ofLogNotice("chi_squared<int>(4)") << chi_squared<int>(4);
		ofLogNotice("chi_squared<vec2>(4)") << chi_squared<vec2>(4);
		ofLogNotice("chi_squared<vec2>({4, 10})") << chi_squared<vec2>({ 4, 10 });
		ofLogNotice("chi_squared<vec3>(4)") << chi_squared<vec3>(4);
		ofLogNotice("chi_squared<vec3>({4,10,100})") << chi_squared<vec3>({ 4, 10, 100 });

		// binomial requires 2 args (no defaults)
		// returns integers

		ofLogNotice("binomial(20, .5)") << of::random::binomial(20, .5);
		ofLogNotice("binomial<double>(20, .5)") << binomial<double>(20, .5);
		ofLogNotice("binomial<int>(20, .5)") << binomial<int>(20, .5);
		ofLogNotice("binomial<vec2>(20, .5)") << binomial<vec2>(20, .5);
		ofLogNotice("binomial<vec2>({20, 20},{.5, .1})") << binomial<vec2>({ 20, 20 }, { .5, .1 });
		ofLogNotice("binomial<vec3>(20, .5)") << binomial<vec3>(20, .5);
		ofLogNotice("binomial<vec4>(20, .5)") << binomial<vec4>(20, .5);
		ofLogNotice("binomial<vec3>({20,20, 1},{.5,.1,.05})") << binomial<vec3>({ 20, 20, 1 }, { .5, .1, .05 });
		ofLogNotice("binomial<vec4>({20,20, 1, 1},{.5,.1,.05, .1})") << binomial<vec4>({ 20, 20, 1, 1 }, { .5, .1, .05, .1 });

		ofLogNotice("ofRandomBinomial(20, .5)") << ofRandomBinomial(20, .5);
		ofLogNotice("ofRandomBinomial<double>(20, .5)") << ofRandomBinomial<double>(20, .5);
		ofLogNotice("ofRandomBinomial<int>(20, .5)") << ofRandomBinomial<int>(20, .5);
		ofLogNotice("ofRandomBinomial<vec2>(20, .5)") << ofRandomBinomial<vec2>(20, .5);
		ofLogNotice("ofRandomBinomial<vec2>({20, 20},{.5, .1})") << ofRandomBinomial<vec2>({ 20, 20 }, { .5, .1 });
		ofLogNotice("ofRandomBinomial<vec3>(20, .5)") << ofRandomBinomial<vec3>(20, .5);
		ofLogNotice("ofRandomBinomial<vec3>({20,20, 1},{.5,.1,.05})") << ofRandomBinomial<vec3>({ 20, 20, 1 }, { .5, .1, .05 });

		// geometric takes no arg

		ofLogNotice("geometric()") << of::random::geometric();
		ofLogNotice("geometric<double>()") << geometric<double>();
		ofLogNotice("geometric<int>()") << geometric<int>();
		ofLogNotice("geometric<vec2>()") << geometric<vec2>();
		ofLogNotice("geometric<vec3>()") << geometric<vec3>();
		ofLogNotice("geometric<vec4>()") << geometric<vec4>();

		ofLogNotice("ofRandomGeometric()") << ofRandomGeometric();
		ofLogNotice("ofRandomGeometric<double>()") << ofRandomGeometric<double>();
		ofLogNotice("ofRandomGeometric<int>()") << ofRandomGeometric<int>();
		ofLogNotice("ofRandomGeometric<vec2>()") << ofRandomGeometric<vec2>();
		ofLogNotice("ofRandomGeometric<vec3>()") << ofRandomGeometric<vec3>();
		
		ofLogNotice("ofRandomBoundNormal(10,20;") << ofRandomBoundNormal(10,20);
		ofLogNotice("ofRandomBoundNormal<glm::vec2>(10, 20)") << ofRandomBoundNormal<glm::vec2>(10, 20);
		ofLogNotice("ofRandomBoundNormal<glm::vec2>({100, 200},{110, 210})") << ofRandomBoundNormal<glm::vec2>({100, 200},{110, 210});
		ofLogNotice("ofRandomBoundNormal<glm::vec3>(10, 20)") << ofRandomBoundNormal<glm::vec3>(10, 20);
		ofLogNotice("ofRandomBoundNormal<glm::vec3>({100 ,200, 300},{110, 210, 310})") << ofRandomBoundNormal<glm::vec3>({100 ,200, 300},{110, 210, 310});
		ofLogNotice("ofRandomBoundNormal<glm::vec4>(10, 20)") << ofRandomBoundNormal<glm::vec4>(10, 20);
		ofLogNotice("ofRandomBoundNormal<glm::vec4>({100 ,200, 300, 400},{110, 210, 310, 410})") << ofRandomBoundNormal<glm::vec4>({100 ,200, 300, 400},{110, 210, 310, 410});

		ofLogNotice("======= alternate engines test =====");
		std::random_device rd {};
		std::seed_seq seq { rd(), rd(), rd(), rd() };
		std::mt19937 alternate_twister { seq };
		ofLogNotice("alternate MT");
		ofLogNotice("  of::random::uniform<vec2>({5,5}, {10,10} — rand seed") << of::random::uniform<vec2>({5,5}, {10,10}, alternate_twister);
		alternate_twister.seed(10);
		ofLogNotice("  of::random::uniform<vec2>({5,5}, {10,10} — seed(10) predict => 8.8566, 6.49381") << of::random::uniform<vec2>({5,5}, {10,10}, alternate_twister);

		ofLogNotice("alternate default");
		std::default_random_engine default_engine;
		ofLogNotice("  of::random::uniform<vec2>({5,5}, {10,10} - rand seed") << of::random::uniform<vec2>({5,5}, {10,10}, default_engine);
		default_engine.seed(10);
		ofLogNotice("  of::random::uniform<vec2>({5,5}, {10,10} — seed(10) predict => 5.00112, 9.25162") << of::random::uniform<vec2>({5,5}, {10,10}, default_engine);

	}
}

void ofApp::keyPressed(int key) {
	if (key == ' ') perform();
}
