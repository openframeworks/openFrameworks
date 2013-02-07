#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void removeIslands(ofPixels& img) {
	int w = img.getWidth(), h = img.getHeight();
	int ia1=-w-1,ia2=-w-0,ia3=-w+1,ib1=-0-1,ib3=-0+1,ic1=+w-1,ic2=+w-0,ic3=+w+1;
	unsigned char* p = img.getPixels();
	for(int y = 1; y + 1 < h; y++) {
		for(int x = 1; x + 1 < w; x++) {
			int i = y * w + x;
			if(p[i]) {
				if(!p[i+ia1]&&!p[i+ia2]&&!p[i+ia3]&&!p[i+ib1]&&!p[i+ib3]&&!p[i+ic1]&&!p[i+ic2]&&!p[i+ic3]) {
					p[i] = 0;
				}
			}
		}
	}
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	
	ofDirectory dir;
	dir.allowExt("png");
	dir.open(".");
	dir.listDir();
	for(int i = 0; i < dir.size(); i++) {
		ofImage cur;
		cur.loadImage(dir.getName(i));
		cur.setImageType(OF_IMAGE_GRAYSCALE);
		input.push_back(cur);
		output.push_back(cur);
		canny.push_back(cur);
	}
	width = input[0].getWidth();
	height = input[0].getHeight();
	
	img.init(width, height);
	
	gui.setup();
	gui.addPanel("Settings");
	gui.addToggle("doFDoG");
	gui.addToggle("doThresh");
	gui.addToggle("doThin");
	gui.addToggle("doCanny");
	gui.addSlider("black", 0, -255, 255, true);
	gui.addSlider("sigma1", 0.4, 0.01, 2.0, false);
	gui.addSlider("sigma2", 3.0, 0.01, 10.0, false);
	gui.addSlider("tau", 0.97, 0.8, 1.0, false);
	gui.addSlider("thresh", 128, 0, 255, false);
	gui.addSlider("halfw", 4, 1, 8, true);
	gui.addSlider("smoothPasses", 2, 1, 4, true);
	gui.addSlider("cannyParam1", 256, 0, 1024, true);
	gui.addSlider("cannyParam2", 256, 0, 1024, true);
	gui.loadSettings("settings.xml");
}

void ofApp::update(){
	bool doFDoG = gui.getValueB("doFDoG");
	bool doCanny = gui.getValueB("doCanny");
	bool doThin = gui.getValueB("doThin");
	bool doThresh = gui.getValueB("doThresh");
	
	int black = gui.getValueI("black");
	float sigma1 = gui.getValueF("sigma1");
	float sigma2 = gui.getValueF("sigma2");
	float tau = gui.getValueF("tau");
	float thresh = gui.getValueF("thresh");
	int halfw = gui.getValueI("halfw");
	int smoothPasses = gui.getValueI("smoothPasses");
	int cannyParam1 = gui.getValueI("cannyParam1");
	int cannyParam2 = gui.getValueI("cannyParam2");
	
	for(int i = 0; i < input.size(); i++) {
		if(doFDoG) {
			int j = 0;
			for(int y = 0; y < height; y++) {
				for(int x = 0; x < width; x++) {
					img[x][y] = input[i].getPixels()[j++] - black;
				}
			}
			etf.init(width, height);
			etf.set(img);
			etf.Smooth(halfw, smoothPasses);
			GetFDoG(img, etf, sigma1, sigma2, tau); 
			//GrayThresholding(img, threshold);
			j = 0;
			for(int y = 0; y < height; y++) {
				for(int x = 0; x < width; x++) {
					output[i].getPixels()[j++] = img[x][y];
				}
			}
			if(doThresh) {
				threshold(output[i], thresh);
			}
			if(doThin) {
				invert(output[i]);
				thin(output[i]);
				removeIslands(output[i].getPixelsRef());
				invert(output[i]);
			}
			output[i].update();
		}
		if(doCanny) {
			Canny(input[i], canny[i], cannyParam1 * 2, cannyParam2 * 2, 5);
			if(doThin) {
				thin(canny[i]);
				removeIslands(canny[i].getPixelsRef());
				invert(canny[i]);
			}
			canny[i].update();
		}
	}
}

void ofApp::draw(){
	for(int i = 0; i < input.size(); i++) {
		input[i].draw(i * 256, 0);
		output[i].draw(i * 256, 256);
		canny[i].draw(i * 256, 512);
	}
}
