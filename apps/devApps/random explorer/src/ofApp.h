#pragma once

#include "ofMain.h"

struct Dist {
	
	ofParameterGroup parameters_;
	ofColor color_ {128,128,128};
	std::string info_;
	glm::vec2 range_ { };
	bool discrete_ { };
	size_t underflow_;
	size_t overflow_;
	std::size_t max_ { 0 };
	float cost_ { 0.0f };
	
	virtual auto gen() -> void = 0;
	virtual auto clear() -> void = 0;
	virtual auto compile() -> void = 0;
	virtual auto draw(float x, float y, float w, float h) -> void = 0;
	
	Dist() {};
};

template <typename T>
struct ConcreteDist: public Dist {
	
	std::vector<T> data_;
	std::function<T()> gen_;
	std::vector<T> bins_;
	int autorot_ { 0 };

	ConcreteDist(std::string label,
				 std::string info,
				 std::vector<ofAbstractParameter *> params,
				 std::function<T()> gen,
				 std::size_t num_bins = 101,
				 glm::vec2 range = {-1.0, 1.0},
				 bool discrete = false )
	: gen_(gen)
	{
		info_ = info;
		range_ = range;
		discrete_ = discrete;
		bins_.resize(num_bins);
		parameters_.setName(label);
		for (auto & p: params) parameters_.add(*p);
	}
	
	auto gen() -> void  {
		data_.push_back(gen_());
	}
	
	auto clear() -> void  {
		overflow_ = 0;
		underflow_ = 0;
		data_.clear();
		std::fill(bins_.begin(), bins_.end(), T{0});
		max_ = 0;
	}
	
	auto compile() -> void  {
		for (auto & v: data_) {
			if constexpr (std::is_arithmetic_v<T>) {
				if (v < range_.x) {
					underflow_++;
				} else if (v >= bins_.size()) {
					overflow_++;
				} else {
					bins_.at(v) = bins_.at(v)+1;
					if (bins_.at(v) > max_) max_ = bins_.at(v);
				}
			}
		}
	}
	
	auto draw(float x, float y, float w, float h) -> void  {
		ofPushStyle();
		ofPushMatrix();
		{
			ofTranslate(x,y);
			ofSetColor(color_);
			ofDrawRectangle(0,0,w,h);

			ofSetColor(ofColor::darkRed);
			if (underflow_) ofDrawBitmapString("undershoot: " + ofToString(underflow_), w+5, 48);
			if (overflow_) ofDrawBitmapString("overshoot: " + ofToString(overflow_), w+5, 64);

			ofSetColor(255,255,255,255);
			ofDrawBitmapStringHighlight(parameters_.getName() + " " + ofToString(cost_*1000, 2, 5)+"ms", w+5, 12);
			ofDrawBitmapString(info_, w+5, 30);
	
			if constexpr (std::is_arithmetic_v<T>) {

				auto p = 0.0f;
				auto incr = w/bins_.size();
				auto fact = h/max_;
				if (discrete_) {
					ofTranslate(incr/2, 0);
					for (auto y: bins_) {
						ofDrawLine(0, h, 0, h-float(y)*fact);
						ofDrawCircle(0, h-float(y)*fact, 3);
						ofTranslate(int(incr), 0);
					}
				} else {
					ofPolyline line;
					line.addVertex(0,h-bins_[0]*fact);
					for (auto y: bins_) {
						line.lineTo(p, h-float(y)*fact);
						p+=incr;
					}
					line.draw();
				}
			} else if constexpr (std::is_same_v<T, glm::vec2>) {
				ofSetColor(255,255,255,96);
				for (const auto & d: data_) {
					ofDrawCircle(d, .5);
				}
				
			} else if constexpr (std::is_same_v<T, glm::vec3>) {
				ofSetColor(255,255,255,32);
				of3dPrimitive prim;
				prim.getMesh().getVertices() = data_;
				auto x = ofMap(sin(M_PI*2*fmod(ofGetElapsedTimef(),4.13)/4.13), -1, 1, 0, 1);
				auto y = ofMap(sin(M_PI*2*fmod(ofGetElapsedTimef(),2.13)/2.13), -1, 1, 0, 1);
				auto z = ofMap(sin(M_PI*2*fmod(ofGetElapsedTimef(),3.13)/3.13), -1, 1, 0, 1);
				auto d = ofMap(sin(M_PI*2*fmod(ofGetElapsedTimef(),11.1)/11.1), -1, 1, 33, 70);
				prim.rotateDeg(d,{x,y,z});
				ofPushMatrix();
				{
					ofTranslate(w/3, h/3);
					prim.drawWireframe();
					prim.drawAxes(25);
				}
				ofPopMatrix();
			}
		}
		ofPopMatrix();
		ofPopStyle();
	}
};

#include "ofxGui.h"

class ofApp : public ofBaseApp{

	ofxPanel panel_;
	ofParameter<size_t> size_{"size cube root", 25, 1, 50};
	ofParameter<std::string> size_calc_{"actual num"};

	ofParameter<float> 		rand_min_	{"min", 0, 0, 100};
	ofParameter<float> 		rand_max_	{"max", 100, 0, 100};
	ofParameter<float> 		uni_min_	{"min", 0, 0, 100};
	ofParameter<float> 		uni_max_	{"max", 100, 0, 100};
	ofParameter<int> 		uni_int_min_{"min", 1, 0, 9};
	ofParameter<int> 		uni_int_max_{"max", 6, 1, 10};
	ofParameter<float> 		norm_mean_	{"mean", 100, 0, 100};
	ofParameter<float> 		norm_dev_	{"stddev", 15, 0, 50};
	ofParameter<float> 		bn_min_		{"min", 35, 0, 200};
	ofParameter<float> 		bn_max_		{"max", 165, 0, 200};
	ofParameter<float> 		bn_focus_	{"focus", 1.75, 0.01, 20};
	ofParameter<float> 		gamma_alpha_{"alpha", 3, 1, 20};
	ofParameter<float> 		gamma_beta_	{"beta", 5, 0, 50};
	ofParameter<float> 		poiss_mean_	{"mean", 4, 0, 20};
	ofParameter<float> 		exp_lambda_	{"lambda", .1, 0.01, 0.5};
	ofParameter<glm::vec2> 	vec_min_	{"min", {40,10}, {0,0}, {200,200}};
	ofParameter<glm::vec2> 	vec_max_	{"max", {60,90}, {0,0}, {200,200}};
	ofParameter<glm::vec2> 	vec_mean_	{"mean", {50,50}, {0,0}, {100,100}};
	ofParameter<glm::vec2> 	vec_dev_	{"stddev", {50,15}, {0,0}, {50,50}};
	ofParameter<float> 		vec_gamma_a_{"alpha", 3, 1, 20};
	ofParameter<float> 		vec_gamma_b_{"beta", 5, 0, 50};

	std::vector<std::shared_ptr<Dist>> dists_ {
		std::make_shared<ConcreteDist<float>>("rand()", "old-skool (thread risky) based on srand()", 		std::vector<ofAbstractParameter *>{&rand_min_, &rand_max_},		[&]{ return ofRandom(rand_min_, rand_max_); }),
		std::make_shared<ConcreteDist<float>>("uniform <real>", "modern, thread safe with seeded mt19937",	std::vector<ofAbstractParameter *>{&uni_min_, &uni_max_},		[&]{ return of::random::uniform<float>(uni_min_, uni_max_); }),
		std::make_shared<ConcreteDist<int>>("uniform <int>", "unambiguously includes bounds", 				std::vector<ofAbstractParameter *>{&uni_int_min_, &uni_int_max_},[&]{ return of::random::uniform<int>(uni_int_min_, uni_int_max_); }, 	 11, glm::vec2{0,  10}, true),
		std::make_shared<ConcreteDist<float>>("normal", "aka gaussian", 									std::vector<ofAbstractParameter *>{&norm_mean_, &norm_dev_},	[&]{ return of::random::normal<float>(norm_mean_, norm_dev_); }, 		201, glm::vec2{0, 200}),
		std::make_shared<ConcreteDist<float>>("gamma", "with an alpha of 1, ~equivalent to exponential",	std::vector<ofAbstractParameter *>{&gamma_alpha_, &gamma_beta_},[&]{ return of::random::gamma<int>(gamma_alpha_, gamma_beta_); }),
		std::make_shared<ConcreteDist<int>>("poisson", "for sparse, positive events; mean>12 is ~normal", 	std::vector<ofAbstractParameter *>{&poiss_mean_},				[&]{ return of::random::poisson<int>(poiss_mean_); }, 					 21, glm::vec2{0,  20}, true),
		std::make_shared<ConcreteDist<float>>("exponential", "", 											std::vector<ofAbstractParameter *>{&exp_lambda_},				[&]{ return of::random::exponential<int>(exp_lambda_); })
	};
	
	std::vector<std::shared_ptr<Dist>> of_dists_ {
		std::make_shared<ConcreteDist<float>>("bound normal", "practical min/max, with enforced limits",std::vector<ofAbstractParameter *>{&bn_min_, &bn_max_, &bn_focus_},[&]{ return ofRandomNormalLimits<int>(bn_min_, bn_max_, bn_focus_); }, 	201, glm::vec2{0, 200}),
		std::make_shared<ConcreteDist<glm::vec2>>("vec2 uniform/refined", "all generators can be typed vec2, 3 or 4",std::vector<ofAbstractParameter *>{&vec_min_, &vec_max_},[&]{ return of::random::uniform<glm::vec2>(vec_min_, vec_max_); }, 	201, glm::vec2{0, 200}),
		std::make_shared<ConcreteDist<glm::vec2>>("vec2 normal/refined", "vecN version can be refined to vecN params",std::vector<ofAbstractParameter *>{&vec_mean_, &vec_dev_},[&]{ return of::random::normal<glm::vec2>(vec_mean_, vec_dev_); }, 	201, glm::vec2{0, 200}),
		std::make_shared<ConcreteDist<glm::vec3>>("vec3 gamma", "",std::vector<ofAbstractParameter *>{&vec_gamma_a_, &vec_gamma_b_},[&]{ return of::random::gamma<glm::vec3>(vec_gamma_a_, vec_gamma_b_); }, 	201, glm::vec2{0, 200})
	};
	
public:
	void setup() override;
	void update() override;
	void draw() override;

// MARK: ofApp boilerplate
//	void exit() override;
//	void keyPressed(int key) override;
//	void keyReleased(int key) override;
//	void mouseMoved(int x, int y ) override;
//	void mouseDragged(int x, int y, int button) override;
//	void mousePressed(int x, int y, int button) override;
//	void mouseReleased(int x, int y, int button) override;
//	void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
//	void mouseEntered(int x, int y) override;
//	void mouseExited(int x, int y) override;
//	void windowResized(int w, int h) override;
//	void dragEvent(ofDragInfo dragInfo) override;
//	void gotMessage(ofMessage msg) override;
};
