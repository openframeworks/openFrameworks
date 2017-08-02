#include "ofApp.h"

#include <regex>

// Some explanation on regular expressions
// http://gnosis.cx/publish/programming/regular_expressions.html

// more info
// http://www.regular-expressions.info/reference.html



//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0);
	page = 0;
	searchGoogleImages();
}


//--------------------------------------------------------------
void ofApp::searchGoogleImages() {

	// clear old imges
	images.clear();
	urls.clear();

	// create the google url string
	string term         = "openframeworks";
	string googleImgURL = "http://www.google.com/search?q="+term+"&tbm=isch&sout=1&tbs=isz&&start="+ofToString(page);
	cout << "searching for " << googleImgURL << endl;

	ofHttpResponse res = ofLoadURL(googleImgURL);
	if(res.status > 0) {

		// copy over the response date fromt the url load
		rawData = res.data.getText();

		// first we want to get the search contents tag
		// in the webpage there is a table for all the images. we
		// want to get the content in the table using
		// the <table> (.*?) </table> expression
		std::regex regEx("<table class=\"images_table\" style=\"table-layout:fixed\" [^>]+>(.*?)</table>");
		std::smatch match;
		std::regex_search(rawData, match, regEx);
		int found = match.size();


		// did we find the table tag with all the images
		// if so lets now start pulling out all the img tags
		if(found != 0) {

			// this is just the table content
			string contents = match[0];

			// setup the regex for img tags
			std::regex imgRegEx("<img[^>]*src=\"([^\"]*)");
			std::smatch imgMatch;
			std::regex_search(contents, imgMatch, imgRegEx);

			// now lets search for img tags in the content
			// we keep search till we run out of img tags
			for (size_t i = 0; i < imgMatch.size(); ++i){

				// we get the sub string from the content
				// and then trim the content so that we
				// can continue to search
				string contents = imgMatch[i];

				// setup the regex for src attribute in the img tag
				std::regex srcRegEx("src=\"(.*?).$");
				std::smatch srcMatch;
				std::regex_search(contents, srcMatch, srcRegEx);

				// if we found the src tag lets grab just
				// the url from the src attribute
				if (srcMatch.size()!=0) {

					// save the img url
					string url = srcMatch[0];
					urls.push_back(url);

				}

			}
			// end while

		}
	}

	// load all the images
	for (unsigned int i=0; i<urls.size(); i++) {
		images.push_back(URLImage());
		images.back().url = urls[i];
		images.back().bDoneLoading = false;
	}


	// just clean up for rendering to screen
	ofStringReplace(rawData, "\n", "");
	ofStringReplace(rawData, " ", "");
	ofStringReplace(rawData, "\t", "");
	string str;
	for (unsigned int i=0; i<rawData.size(); i++) {
		str += rawData[i];
		if(i%40==39) str+="\n";
	}
	rawData = str.substr(0, 2000)+"...";
}

//--------------------------------------------------------------
void ofApp::update(){
	for(unsigned int i=0; i<images.size(); i++) {
		if(!images[i].bDoneLoading) {
			images[i].load(images[i].url);
			images[i].bDoneLoading = true;
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// draw the raw data from google
	ofSetColor(255);
	ofDrawBitmapString(rawData, 20, 20);

	// draw the urls
	for(unsigned int i=0; i<urls.size(); i++) {
		ofSetColor(255);
		ofDrawBitmapString(urls[i].substr(0, 30)+"...", 360, 20+(i*20));
	}

	// draw the urls
	for(unsigned int i=0; i<urls.size(); i++) {
		ofSetColor(255);
		ofDrawBitmapString(urls[i].substr(0, 30)+"...", 360, 20+(i*20));
	}

	// draw the images
	float x = 0;
	float y = 0;
	for(unsigned int i=0; i<images.size(); i++) {
		if(images[i].bDoneLoading) {
			ofSetColor(255);
			images[i].draw(640+x, 20+y);

			y += images[i].getHeight()+2;
			if(y > ofGetHeight()-100) {
				y = 0;
				x += 200;
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	page += 22;
	searchGoogleImages();

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
