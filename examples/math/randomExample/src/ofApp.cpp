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
	ofDrawBitmapString("hit spacebar and watch the console", 20, 200);
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
		ofLogNotice("uniform<glm::vec2>()") << uniform<glm::vec2>();
		ofLogNotice("uniform<glm::vec2>(5)") << uniform<glm::vec2>(5);
		ofLogNotice("uniform<glm::vec2>(5,10)") << uniform<glm::vec2>(5, 10);
		ofLogNotice("uniform<glm::vec2>({0,10},{10,20})") << uniform<glm::vec2>({ 0, 10 }, { 10, 20 });
		ofLogNotice("uniform<glm::vec3>()") << uniform<glm::vec3>();
		ofLogNotice("uniform<glm::vec4>(5)") << uniform<glm::vec4>(5);
		ofLogNotice("uniform<glm::vec3>(5,10)") << uniform<glm::vec3>(5, 10);
		ofLogNotice("uniform<glm::vec3>(-1,1)") << uniform<glm::vec3>(-1,1);
		ofLogNotice("uniform<glm::vec3>({0,10,20},{10,20,30})") << uniform<glm::vec3>({ 0, 10, 20 }, { 10, 20, 30 });
		ofLogNotice("uniform<glm::vec4>({0,10,20,30},{10,20,30, 40})") << uniform<glm::vec4>({ 0, 10, 20, 30 }, { 10, 20, 30, 40 });

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
		ofLogNotice("ofRandomUniform<glm::vec2>()") << ofRandomUniform<glm::vec2>();
		ofLogNotice("ofRandomUniform<glm::vec2>(5)") << ofRandomUniform<glm::vec2>(5);
		ofLogNotice("ofRandomUniform<glm::vec2>(5,10)") << ofRandomUniform<glm::vec2>(5, 10);
		ofLogNotice("ofRandomUniform<glm::vec2>({0,10},{10,20})") << ofRandomUniform<glm::vec2>({ 0, 10 }, { 10, 20 });
		ofLogNotice("ofRandomUniform<glm::vec3>()") << ofRandomUniform<glm::vec3>();
		ofLogNotice("ofRandomUniform<glm::vec3>(5)") << ofRandomUniform<glm::vec3>(5);
		ofLogNotice("ofRandomUniform<glm::vec3>(5,10)") << ofRandomUniform<glm::vec3>(5, 10);
		ofLogNotice("ofRandomUniform<glm::vec3>({0,10,20},{10,20,30})") << ofRandomUniform<glm::vec3>({ 0, 10, 20 }, { 10, 20, 30 });

		// normal / gaussian requires 2 args (no defaults) // return type defaults to float
		// "gaussian" is preferred as "normal" (using namespace of::random) will clash with a macOS define

		ofLogNotice("normal(10, 2)") << of::random::normal(10, 2);
		ofLogNotice("normal<double>(10, 2)") << of::random::normal<double>(10, 2);
		ofLogNotice("normal<int>(10, 2)") << of::random::normal<int>(10, 2);
		ofLogNotice("normal<glm::vec2>(10, 2)") << of::random::normal<glm::vec2>(10, 2);
		ofLogNotice("normal<glm::vec2>({10, 20},{2, 1})") << of::random::normal<glm::vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("normal<glm::vec3>(10,2)") << of::random::normal<glm::vec3>(10, 2);
		ofLogNotice("normal<glm::vec3>({10,20,30},{2,1,.5})") << of::random::normal<glm::vec3>({ 10, 20, 30 }, { 2, 1, .5 });
		ofLogNotice("normal<glm::vec4>({10,20,30, 40},{2,1,.5, .25})") << of::random::normal<glm::vec4>({ 10, 20, 30, 40 }, { 2, 1, .5, .25 });
		ofLogNotice("ofRandomNormal(10, 2)") << ofRandomNormal(10, 2);
		ofLogNotice("ofRandomNormal<double>(10, 2)") << ofRandomNormal<double>(10, 2);
		ofLogNotice("ofRandomNormal<int>(10, 2)") << ofRandomNormal<int>(10, 2);
		ofLogNotice("ofRandomNormal<glm::vec2>(10, 2)") << ofRandomNormal<glm::vec2>(10, 2);
		ofLogNotice("ofRandomNormal<glm::vec2>({10, 20},{2, 1})") << ofRandomNormal<glm::vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("ofRandomNormal<glm::vec3>(10,2)") << ofRandomNormal<glm::vec3>(10, 2);
		ofLogNotice("ofRandomNormal<glm::vec3>({10,20,30},{2,1,.5})") << ofRandomNormal<glm::vec3>({ 10, 20, 30 }, { 2, 1, .5 });
		ofLogNotice("ofRandomNormal<glm::vec4>({10,20,30, 40},{2,1,.5, .25})") << ofRandomNormal<glm::vec4>({ 10, 20, 30, 40 }, { 2, 1, .5, .25 });

		// testing the forwarding
		ofLogNotice("ofRandomGaussian(10, 2)") << ofRandomGaussian(10, 2);
		ofLogNotice("ofRandomGaussian<double>(10, 2)") << ofRandomGaussian<double>(10, 2);
		ofLogNotice("ofRandomGaussian<int>(10, 2)") << ofRandomGaussian<int>(10, 2);
		ofLogNotice("ofRandomGaussian<glm::vec2>(10, 2)") << ofRandomGaussian<glm::vec2>(10, 2);
		ofLogNotice("ofRandomGaussianglm::vec2>({10, 20},{2, 1})") << ofRandomGaussian<glm::vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("ofRandomGaussian<glm::vec3>(10,2)") << ofRandomGaussian<glm::vec3>(10, 2);
		ofLogNotice("ofRandomGaussian<glm::vec3>({10,20,30},{2,1,.5})") << ofRandomGaussian<glm::vec3>({ 10, 20, 30 }, { 2, 1, .5 });

		ofLogNotice("gaussian aka normal(10, 2)") << gaussian(10, 2);
		ofLogNotice("gaussian aka normal<double>(10, 2)") << gaussian<double>(10, 2);
		ofLogNotice("gaussian aka normal<int>(10, 2)") << gaussian<int>(10, 2);
		ofLogNotice("gaussian aka normal<glm::vec2>(10, 2)") << gaussian<glm::vec2>(10, 2);
		ofLogNotice("gaussian aka normal<glm::vec2>({10, 20},{2, 1})") << gaussian<glm::vec2>({ 10, 20 }, { 2, 1 });
		ofLogNotice("gaussian aka normal<glm::vec3>(10,2)") << gaussian<glm::vec3>(10, 2);
		ofLogNotice("gaussian aka normal<glm::vec3>({10,20,30},{2,1,.5})") << gaussian<glm::vec3>({ 10, 20, 30 }, { 2, 1, .5 });

		// exponential requires 1 args (no defaults)

		ofLogNotice("exponential(2.5)") << exponential(2.5);
		ofLogNotice("exponential<double>(2.5)") << exponential<double>(2.5);
		ofLogNotice("exponential<int>(2.5)") << exponential<int>(2.5);
		ofLogNotice("exponential<glm::vec2>(2.5)") << exponential<glm::vec2>(2.5);
		ofLogNotice("exponential<glm::vec4>(2.5)") << exponential<glm::vec4>(2.5);
		ofLogNotice("exponential<glm::vec2>({2.5, 10})") << exponential<glm::vec2>({ 2.5, 10 });
		ofLogNotice("exponential<glm::vec3>(2.5, 10, 0.1") << exponential<glm::vec3>(2.5);
		ofLogNotice("exponential<glm::vec3>({2.5, 10, 0.1}") << exponential<glm::vec3>({ 2.5, 10, 0.1 });
		ofLogNotice("exponential<glm::vec4>({2.5, 10, 0.1, 5}") << exponential<glm::vec4>({ 2.5, 10, 0., 5 });

		ofLogNotice("ofRandomExponential(2.5)") << ofRandomExponential(2.5);
		ofLogNotice("ofRandomExponential<double>(2.5)") << ofRandomExponential<double>(2.5);
		ofLogNotice("ofRandomExponential<int>(2.5)") << ofRandomExponential<int>(2.5);
		ofLogNotice("ofRandomExponential<glm::vec2>(2.5)") << ofRandomExponential<glm::vec2>(2.5);
		ofLogNotice("ofRandomExponential<glm::vec2>({2.5, 10})") << ofRandomExponential<glm::vec2>({ 2.5, 10 });
		ofLogNotice("ofRandomExponential<glm::vec3>(2.5, 10, 0.1") << ofRandomExponential<glm::vec3>(2.5);
		ofLogNotice("ofRandomExponential<glm::vec3>({2.5, 10, 0.1}") << ofRandomExponential<glm::vec3>({ 2.5, 10, 0.1 });

		// poisson requires 1 arg (no defaults), and return type is integer-only
		// these conditions are enforced by the std::distributions themselves
		// however to facilitate glm usage (even if the use case strange), they are cast to float in glm templates

		ofLogNotice("poisson(10)") << poisson(10);
		// ofLogNotice("poisson<float>(2.5)") << poisson<float>(5.5);  // expected not to compile
		ofLogNotice("poisson<int>(2.5)") << poisson<long long>(5.5);
		ofLogNotice("poisson<glm::vec2>(2.5)") << poisson<glm::vec2>(5.5);
		ofLogNotice("poisson<glm::vec2>({2.5, 10})") << poisson<glm::vec2>({ 5.5, 10 });
		ofLogNotice("poisson<glm::vec3>(2.5, 10, 0.1") << poisson<glm::vec3>(5.5);
		ofLogNotice("poisson<glm::vec3>({2.5, 10, 0.1}") << poisson<glm::vec3>({ 5.5, 10, 0.1 });
		ofLogNotice("poisson<glm::vec4>({2.5, 10, 0.1, 5}") << poisson<glm::vec4>({ 5.5, 10, 0.1, 5 });

		ofLogNotice("ofRandomPoisson(10)") << ofRandomPoisson(10);
		// ofLogNotice("ofRandomPoisson<float>(2.5)") << ofRandomPoisson<float>(5.5);  // expected not to compile
		ofLogNotice("ofRandomPoisson<int>(2.5)") << ofRandomPoisson<long long>(5.5);
		ofLogNotice("ofRandomPoisson<glm::vec2>(2.5)") << ofRandomPoisson<glm::vec2>(5.5);
		ofLogNotice("ofRandomPoisson<glm::vec2>({2.5, 10})") << ofRandomPoisson<glm::vec2>({ 5.5, 10 });
		ofLogNotice("ofRandomPoisson<glm::vec3>(2.5, 10, 0.1") << ofRandomPoisson<glm::vec3>(5.5);
		ofLogNotice("ofRandomPoisson<glm::vec3>({2.5, 10, 0.1}") << ofRandomPoisson<glm::vec3>({ 5.5, 10, 0.1 });

		// bernoulli requires 1 arg (no defaults), and return type is bool
		// again, glm usage case is cast to float

		ofLogNotice("bernoulli<glm::vec3>(.33)") << bernoulli<glm::vec3>(.33);
		ofLogNotice("bernoulli<glm::vec2>(.5)") << bernoulli<glm::vec2>(.5);
		ofLogNotice("bernoulli<float>(.5)") << bernoulli<float>(.5);
		ofLogNotice("bernoulli<bool>(.5)") << bernoulli<bool>(.5);
		ofLogNotice("bernoulli(.5)") << bernoulli(.5);

		ofLogNotice("ofRandomBernoulli<glm::vec3>({.1,.5,.9})") << ofRandomBernoulli<glm::vec3>({.1,.5,.9});
		ofLogNotice("ofRandomBernoulli<glm::vec3>(.33)") << ofRandomBernoulli<glm::vec3>(.33);
		ofLogNotice("ofRandomBernoulli<glm::vec2>(.5)") << ofRandomBernoulli<glm::vec2>(.5);
		ofLogNotice("ofRandomBernoulli<float>(.5)") << ofRandomBernoulli<float>(.5);
		ofLogNotice("ofRandomBernoulli<bool>(.5)") << ofRandomBernoulli<bool>(.5);
		ofLogNotice("ofRandomBernoulli(.5)") << ofRandomBernoulli(.5);

		ofLogNotice("yes aka bernoulli<glm::vec3>(.33)") << yes<glm::vec3>(.33);
		ofLogNotice("yes aka bernoulli<glm::vec2>(.5)") << yes<glm::vec2>(.5);
		ofLogNotice("yes aka bernoulli<float>(.5)") << yes<float>(.5);
		ofLogNotice("yes aka bernoulli<bool>(.5)") << yes<bool>(.5);
		ofLogNotice("yes aka bernoulli(.5)") << yes(.5);

		ofLogNotice("ofRandomYes aka bernoulli<glm::vec3>(.33)") << ofRandomYes<glm::vec3>(.33);
		ofLogNotice("ofRandomYes aka bernoulli<glm::vec2>(.5)") << ofRandomYes<glm::vec2>(.5);
		ofLogNotice("ofRandomYes aka bernoulli<float>(.5)") << ofRandomYes<float>(.5);
		ofLogNotice("ofRandomYes aka bernoulli<bool>(.5)") << ofRandomYes<bool>(.5);
		ofLogNotice("ofRandomYes aka bernoulli(.5)") << ofRandomYes(.5);

		// lognormal requires 2 args (no defaults)

		ofLogNotice("lognormal(10, 2)") << lognormal(10, 2);
		ofLogNotice("lognormal<double>(10, 2)") << lognormal<double>(10, 2);
		ofLogNotice("lognormal<int>(10, 2)") << lognormal<int>(10, 2);
		ofLogNotice("lognormal<glm::vec2>(10, 2)") << lognormal<glm::vec2>(10, 2);
		ofLogNotice("lognormal<glm::vec2>({10, 20},{2, 1})") << lognormal<glm::vec2>({ 10, 2 }, { 2, .5 });
		ofLogNotice("lognormal<glm::vec3>(10,2)") << lognormal<glm::vec3>(10, 2);
		ofLogNotice("lognormal<glm::vec3>({10,20,30},{2,1,.5})") << lognormal<glm::vec3>({ 10, 2, 1 }, { 2, .5, .25 });
		ofLogNotice("lognormal<glm::vec4>({10,20,30, 40},{2,1,.5, .25})") << lognormal<glm::vec4>({ 10, 2, 1, 30 }, { 2,1, .5, .25 });

		ofLogNotice("ofRandomLognormal(10, 2)") << ofRandomLognormal(10, 2);
		ofLogNotice("ofRandomLognormal<double>(10, 2)") << ofRandomLognormal<double>(10, 2);
		ofLogNotice("ofRandomLognormal<int>(10, 2)") << ofRandomLognormal<int>(10, 2);
		ofLogNotice("ofRandomLognormal<glm::vec2>(10, 2)") << ofRandomLognormal<glm::vec2>(10, 2);
		ofLogNotice("ofRandomLognormal<glm::vec2>({10, 20},{2, 1})") << ofRandomLognormal<glm::vec2>({ 10, 2 }, { 2, .5 });
		ofLogNotice("ofRandomLognormal<glm::vec3>(10,2)") << ofRandomLognormal<glm::vec3>(10, 2);
		ofLogNotice("ofRandomLognormal<glm::vec3>({10,2,1},{2,.5,.25})") << ofRandomLognormal<glm::vec3>({ 10, 2, 1 }, { 2, .5, .25 });

		// gamma requires 2 args (no defaults)

		ofLogNotice("gamma(3, 5)") << gamma(3, 5);
		ofLogNotice("gamma<double>(3, 5)") << gamma<double>(3, 5);
		ofLogNotice("gamma<int>(3, 5)") << gamma<int>(3, 5);
		ofLogNotice("gamma<glm::vec2>(3, 5)") << gamma<glm::vec2>(3, 5);
		ofLogNotice("gamma<glm::vec2>({3, 20},{5, 1})") << gamma<glm::vec2>({ 3, 20 }, { 5, 1 });
		ofLogNotice("gamma<glm::vec3>(3, 5)") << gamma<glm::vec3>(3, 5);
		ofLogNotice("gamma<glm::vec3>({3,20, 1},{5,1,.5})") << gamma<glm::vec3>({ 3, 20, 1 }, { 5, 1, .5 });
		ofLogNotice("gamma<glm::vec4>({3,20, 1, .5},{5,1,.5, .25})") << gamma<glm::vec4>({ 3, 20, 1 , .5}, { 5, 1, .5, .25 });

		ofLogNotice("ofRandomGamma(3, 5)") << ofRandomGamma(3, 5);
		ofLogNotice("ofRandomGamma<double>(3, 5)") << ofRandomGamma<double>(3, 5);
		ofLogNotice("ofRandomGamma<int>(3, 5)") << ofRandomGamma<int>(3, 5);
		ofLogNotice("ofRandomGamma<glm::vec2>(3, 5)") << ofRandomGamma<glm::vec2>(3, 5);
		ofLogNotice("ofRandomGamma<glm::vec2>({3, 20},{5, 1})") << ofRandomGamma<glm::vec2>({ 3, 20 }, { 5, 1 });
		ofLogNotice("ofRandomGamma<glm::vec3>(3, 5)") << ofRandomGamma<glm::vec3>(3, 5);
		ofLogNotice("ofRandomGamma<glm::vec3>({3,20, 1},{5,1,.5})") << ofRandomGamma<glm::vec3>({ 3, 20, 1 }, { 5, 1, .5 });

		// chi-squared requires 1 arg (no defaults)

		ofLogNotice("ofRandomChiSquared(4)") << ofRandomChiSquared(4);
		ofLogNotice("ofRandomChiSquared<double>(4)") << ofRandomChiSquared<double>(4);
		ofLogNotice("ofRandomChiSquared<int>(4)") << ofRandomChiSquared<int>(4);
		ofLogNotice("ofRandomChiSquared<glm::vec2>(4)") << ofRandomChiSquared<glm::vec2>(4);
		ofLogNotice("ofRandomChiSquared<glm::vec2>({4, 10})") << ofRandomChiSquared<glm::vec2>({ 4, 10 });
		ofLogNotice("ofRandomChiSquared<glm::vec3>(4)") << ofRandomChiSquared<glm::vec3>(4);
		ofLogNotice("ofRandomChiSquared<glm::vec4>(4)") << ofRandomChiSquared<glm::vec4>(4);
		ofLogNotice("ofRandomChiSquared<glm::vec3>({4,10,100})") << ofRandomChiSquared<glm::vec3>({ 4, 10, 100 });
		ofLogNotice("ofRandomChiSquared<glm::vec4>({4,10,100, 50})") << ofRandomChiSquared<glm::vec4>({ 4, 10, 100, 50 });

		ofLogNotice("chi_squared(4)") << chi_squared(4);
		ofLogNotice("chi_squared<double>(4)") << chi_squared<double>(4);
		ofLogNotice("chi_squared<int>(4)") << chi_squared<int>(4);
		ofLogNotice("chi_squared<glm::vec2>(4)") << chi_squared<glm::vec2>(4);
		ofLogNotice("chi_squared<glm::vec2>({4, 10})") << chi_squared<glm::vec2>({ 4, 10 });
		ofLogNotice("chi_squared<glm::vec3>(4)") << chi_squared<glm::vec3>(4);
		ofLogNotice("chi_squared<glm::vec3>({4,10,100})") << chi_squared<glm::vec3>({ 4, 10, 100 });

		// binomial requires 2 args (no defaults)
		// returns integers

		ofLogNotice("binomial(20, .5)") << binomial(20, .5);
		ofLogNotice("binomial<double>(20, .5)") << binomial<double>(20, .5);
		ofLogNotice("binomial<int>(20, .5)") << binomial<int>(20, .5);
		ofLogNotice("binomial<glm::vec2>(20, .5)") << binomial<glm::vec2>(20, .5);
		ofLogNotice("binomial<glm::vec2>({20, 20},{.5, .1})") << binomial<glm::vec2>({ 20, 20 }, { .5, .1 });
		ofLogNotice("binomial<glm::vec3>(20, .5)") << binomial<glm::vec3>(20, .5);
		ofLogNotice("binomial<glm::vec4>(20, .5)") << binomial<glm::vec4>(20, .5);
		ofLogNotice("binomial<glm::vec3>({20,20, 1},{.5,.1,.05})") << binomial<glm::vec3>({ 20, 20, 1 }, { .5, .1, .05 });
		ofLogNotice("binomial<glm::vec4>({20,20, 1, 1},{.5,.1,.05, .1})") << binomial<glm::vec4>({ 20, 20, 1, 1 }, { .5, .1, .05, .1 });

		ofLogNotice("ofRandomBinomial(20, .5)") << ofRandomBinomial(20, .5);
		ofLogNotice("ofRandomBinomial<double>(20, .5)") << ofRandomBinomial<double>(20, .5);
		ofLogNotice("ofRandomBinomial<int>(20, .5)") << ofRandomBinomial<int>(20, .5);
		ofLogNotice("ofRandomBinomial<glm::vec2>(20, .5)") << ofRandomBinomial<glm::vec2>(20, .5);
		ofLogNotice("ofRandomBinomial<glm::vec2>({20, 20},{.5, .1})") << ofRandomBinomial<glm::vec2>({ 20, 20 }, { .5, .1 });
		ofLogNotice("ofRandomBinomial<glm::vec3>(20, .5)") << ofRandomBinomial<glm::vec3>(20, .5);
		ofLogNotice("ofRandomBinomial<glm::vec3>({20,20, 1},{.5,.1,.05})") << ofRandomBinomial<glm::vec3>({ 20, 20, 1 }, { .5, .1, .05 });

		// geometric takes no arg

		ofLogNotice("geometric()") << geometric();
		ofLogNotice("geometric<double>()") << geometric<double>();
		ofLogNotice("geometric<int>()") << geometric<int>();
		ofLogNotice("geometric<glm::vec2>()") << geometric<glm::vec2>();
		ofLogNotice("geometric<glm::vec3>()") << geometric<glm::vec3>();
		ofLogNotice("geometric<glm::vec4>()") << geometric<glm::vec4>();

		ofLogNotice("ofRandomGeometric()") << ofRandomGeometric();
		ofLogNotice("ofRandomGeometric<double>()") << ofRandomGeometric<double>();
		ofLogNotice("ofRandomGeometric<int>()") << ofRandomGeometric<int>();
		ofLogNotice("ofRandomGeometric<glm::vec2>()") << ofRandomGeometric<glm::vec2>();
		ofLogNotice("ofRandomGeometric<glm::vec3>()") << ofRandomGeometric<glm::vec3>();
	}
}

void ofApp::keyPressed(int key) {
	if (key == ' ') perform();
}
