#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Dist.hpp"

class ofApp : public ofBaseApp {

	ofxPanel panel_;
	ofParameter<size_t> size_{"size cube root", 25, 1, 50};
    ofParameter<std::string> size_calc_{"actual num"};
    ofParameter<unsigned long> seed_{"seed", 0, 0, 1000};
    ofParameter<void> reinit_{"re-init engine"};
    
    std::array<uint8_t, 8> dna_;
    std::vector<uint8_t> shuffle_{1,2,3,4,5,6,7,8};
    std::string dna_string_;
    std::string shuffle_string_;

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
	ofParameter<float> 		vec_gamma_a_{"alpha", 1.5, 1, 20};
	ofParameter<float> 		vec_gamma_b_{"beta", 10, 0, 50};

	std::vector<std::shared_ptr<Dist>> dists_ {
        
		std::make_shared<ConcreteDist<float>>("rand()", "old-skool (thread risky) based on srand()",
                                              std::vector<ofAbstractParameter *>{&rand_min_, &rand_max_}, [&]
                                              { return ofRandom(rand_min_, rand_max_); }),
        
		std::make_shared<ConcreteDist<float>>("uniform <real>", "modern, thread safe with seeded mt19937",
                                              std::vector<ofAbstractParameter *>{&uni_min_, &uni_max_}, [&]
                                              { return of::random::uniform<float>(uni_min_, uni_max_); }),
        
		std::make_shared<ConcreteDist<int>>("uniform <int>", "unambiguously includes bounds",
                                            std::vector<ofAbstractParameter *>{&uni_int_min_, &uni_int_max_}, [&]
                                            { return of::random::uniform<int>(uni_int_min_, uni_int_max_); }, 11, glm::vec2{0,  10}, true),
        
		std::make_shared<ConcreteDist<float>>("normal", "aka gaussian",
                                              std::vector<ofAbstractParameter *>{&norm_mean_, &norm_dev_},[&]
                                              { return of::random::normal<float>(norm_mean_, norm_dev_); }, 201, glm::vec2{0, 200}),
        
		std::make_shared<ConcreteDist<float>>("gamma", "with an alpha of 1, ~equivalent to exponential",
                                              std::vector<ofAbstractParameter *>{&gamma_alpha_, &gamma_beta_}, [&]
                                              { return of::random::gamma<int>(gamma_alpha_, gamma_beta_); }),
        
		std::make_shared<ConcreteDist<int>>("poisson", "for sparse, positive events; mean>12 is ~normal",
                                            std::vector<ofAbstractParameter *>{&poiss_mean_}, [&]
                                            { return of::random::poisson<int>(poiss_mean_); }, 21, glm::vec2{0,  20}, true),
        
		std::make_shared<ConcreteDist<float>>("exponential", "",
                                              std::vector<ofAbstractParameter *>{&exp_lambda_},	[&]
                                              { return of::random::exponential<int>(exp_lambda_); })
	};
	
	std::vector<std::shared_ptr<Dist>> of_dists_ {
        
		std::make_shared<ConcreteDist<float>>("bound normal", "practical min/max, with enforced limits",
                                              std::vector<ofAbstractParameter *>{&bn_min_, &bn_max_, &bn_focus_}, [&]
                                              { return ofRandomNormalLimits<int>(bn_min_, bn_max_, bn_focus_); }, 201, glm::vec2{0, 200}),
        
		std::make_shared<ConcreteDist<glm::vec2>>("vec2 uniform/refined", "all generators can be typed vec2, 3 or 4",
                                                  std::vector<ofAbstractParameter *>{&vec_min_, &vec_max_}, [&]
                                                  { return of::random::uniform<glm::vec2>(vec_min_, vec_max_); }, 201, glm::vec2{0, 200}),
        
		std::make_shared<ConcreteDist<glm::vec2>>("vec2 normal/refined", "vecN version can be refined to vecN params",
                                                  std::vector<ofAbstractParameter *>{&vec_mean_, &vec_dev_}, [&]
                                                  { return of::random::normal<glm::vec2>(vec_mean_, vec_dev_); }, 201, glm::vec2{0, 200}),
        
		std::make_shared<ConcreteDist<glm::vec3>>("vec3 gamma", "",
                                                  std::vector<ofAbstractParameter *>{&vec_gamma_a_, &vec_gamma_b_}, [&]
                                                  { return of::random::gamma<glm::vec3>(vec_gamma_a_, vec_gamma_b_); }, 201, glm::vec2{0, 200})
	};
	
public:
    
    auto rebuild_signatures() {
        dna_string_= "origin numbers:  ";
        for (size_t i = 0; i < dna_.size(); i++) {
            dna_[i] = of::random::uniform<uint8_t>(0,255);
            dna_string_ += ofToUpper(ofToHex(dna_[i])) + " ";
        };
        
        shuffle_string_= "shuffled vector: ";
        shuffle_ = {1,2,3,4,5,6,7,8}; // put back
        of::random::shuffle(shuffle_);
        for (size_t i = 0; i < shuffle_.size(); i++) {
            shuffle_string_ += ofToUpper(ofToHex(shuffle_[i])) + " ";
        }
    }
    
    auto seed (unsigned long & seed) {
        of::random::engine()->seed(seed);
        rebuild_signatures();
        panel_.getControlNames();
        panel_.getControl("seed")->setTextColor(ofColor::green);
    }

    auto reinit() {
        seed_ = 0;
        of::random::Engine::destruct();
        of::random::Engine::construct();
        rebuild_signatures();
        panel_.getControl("seed")->setTextColor(ofColor::gray);
    }
    
	void setup() override;
	void update() override;
	void draw() override;

};
