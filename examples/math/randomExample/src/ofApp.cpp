#include "ofApp.h"
#include "ofRandom.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
	
	panel_.setup();
	panel_.add(uniform1_min_);
	panel_.add(uniform1_max_);
	panel_.add(normal1_mean_);
	panel_.add(normal1_stddev_);
	panel_.add(gamma1_alpha_);
	panel_.add(gamma1_beta_);
	panel_.add(poisson1_mean_);
	panel_.add(exponential1_lambda_);

	std::vector<int> v {1,2,3,4,5,6,7,8};
	ofRandomize(v);
	for (auto & i: v) ofLogNotice("1") << i;

	ofShuffle(v);
	for (auto & i: v) ofLogNotice("1") << i;

	of::random::shuffle(v);
	for (auto & i: v) ofLogNotice("1") << i;

	ofLogNotice("X") << ofRandomUniform<float>(3,10);
	ofLogNotice("Y") << ofRandomUniform<glm::vec2>(3,10);
	ofLogNotice("Z") << ofRandomUniform<glm::vec2>({3,10}, {-1,1});

	// cheapo benchmark
	auto now = ofGetElapsedTimef();
	for (size_t i = 0; i < 10'000'000; i++) of::random::uniform(5,10);
	ofLogNotice("a") << ofGetElapsedTimef() - now;
	
	now = ofGetElapsedTimef();
	for (size_t i = 0; i < 5'000'000; i++) {
		of::random::uniform(5,10);
		of::random::uniform(1,2);
	}
	ofLogNotice("a'") << ofGetElapsedTimef() - now;
	
	now = ofGetElapsedTimef();
	for (size_t i = 0; i < 10'000'000; i++) of::random::uniform<double>(5,10);
	ofLogNotice("ofRandUniform<double>(5,10);") << ofGetElapsedTimef()  - now;
	
	now = ofGetElapsedTimef();
	for (size_t i = 0; i < 10'000'000; i++) ofRandom(5,10);
	ofLogNotice("plain ofRandome") << ofGetElapsedTimef()  - now;
	
	now = ofGetElapsedTimef();
	for (size_t i = 0; i < 10'000'000; i++) of::random::uniform<glm::vec3>({0,10,20},{10,20,30});
	ofLogNotice("d") << ofGetElapsedTimef()  - now;
	


	ofLogNotice("of::random::uniform())") << of::random::uniform(5,10);
//	ofLogNotice("ofRandUniform())") << of::random::ofRandUniform();
	ofLogNotice("ofRandUniform<float>") << of::random::uniform<float>(10,20);
	ofLogNotice("ofRandUniform<double>") << of::random::uniform<double>(10,20);
	ofLogNotice("ofRandUniform<int>") << of::random::uniform<int>(10,20);
	ofLogNotice("ofRandUniform<glm::vec2>") << of::random::uniform<glm::vec2>(10,20);
	ofLogNotice("ofRandUniform<glm::vec3>") << of::random::uniform<glm::vec3>(10,20);
	ofLogNotice("ofRandUniform<glm::vec3> with custom ranges") << of::random::uniform<glm::vec3>({0,10,20},{10,20,30});
	
//	of::random::seed(666);
	ofLogNotice("of::random::uniform())") << of::random::uniform(5,10);
	
//	ofSetRandomSeed(1984);
	ofLogNotice("of::random::uniform()) 1") << of::random::uniform(5,10);
	ofLogNotice("of::random::uniform()) 2") << of::random::uniform(5,10);
	ofLogNotice("of::random::uniform()) 3") << of::random::uniform(5,10);

	ofLogNotice("of::random::normal(100,15))") << of::random::normal<int>(100.0,15.0);
	ofLogNotice("of::random::normal(0,1))") << of::random::normal(0,1);
	ofLogNotice("of::random::normal<glm::vec2>())") << of::random::normal<glm::vec2>({100, 0},{15, 5});
	
	for (std::size_t i=0;i<10;i++) ofLogNotice("ofRandomNormalLimits(20,30)") << ofRandomNormalLimits<glm::vec2>({100, 0},{15, 5});


	ofLogNotice("done") << 1;

}

//--------------------------------------------------------------
void ofApp::update(){
	uniform1_.clear();
	normal1_.clear();
	gamma1_.clear();
	poisson1_.clear();
	exponential1_.clear();
	for (std::size_t i=0;i<10000;i++) uniform1_.push_back(of::random::uniform<int>(uniform1_min_,uniform1_max_));
	for (std::size_t i=0;i<10000;i++) normal1_.push_back(of::random::normal<int>(normal1_mean_, normal1_stddev_));
	for (std::size_t i=0;i<10000;i++) gamma1_.push_back(of::random::gamma<int>(gamma1_alpha_, gamma1_beta_));
	for (std::size_t i=0;i<10000;i++) poisson1_.push_back(of::random::poisson<int>(poisson1_mean_));
	for (std::size_t i=0;i<10000;i++) exponential1_.push_back(of::random::exponential<int>(exponential1_lambda_));
	uniform1_.compile();
	normal1_.compile();
	gamma1_.compile();
	poisson1_.compile();
	exponential1_.compile();
}

//--------------------------------------------------------------
void ofApp::draw(){
	uniform1_.draw(220,20,100,100);
	normal1_.draw(220,140,100,100);
	gamma1_.draw(220,260,100,100);
	poisson1_.draw(220,380,100,100);
	exponential1_.draw(220,500,100,100);

	panel_.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
