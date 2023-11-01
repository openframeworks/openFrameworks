#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetFrameRate(60);

	#ifdef _USE_LIVE_VIDEO
        vidGrabber.setVerbose(true);
        vidGrabber.setup(1280,720);
	#else
        vidPlayer.load("Hand3s.mov");
        vidPlayer.play();
        vidPlayer.setLoopState(OF_LOOP_NORMAL);
	#endif
	
	blurAmount = 11;
	bMirror = true;
	cvDownScale = 8;
	bContrastStretch = false;
	// store a minimum squared value to apply flow velocity
	minLengthSquared = 0.5 * 0.5;
	mode = MODE_SPIN_CUBES;
	bDrawOptiFlowVectors = false;
	
	// lets make a mesh that's an arrow
	arrowMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	
	// these 4 vertices will be the rectangle
	arrowMesh.addVertex( glm::vec3(0.0, -0.22, 0.0 ));
	arrowMesh.addVertex( glm::vec3(1.0, -0.22, 0.0 ));
	arrowMesh.addVertex( glm::vec3(1.0, 0.22, 0.0 ));
	arrowMesh.addVertex( glm::vec3(0.0, 0.22, 0.0 ));
	
	arrowMesh.addIndex(1);
	arrowMesh.addIndex(0);
	arrowMesh.addIndex(3);
	
	arrowMesh.addIndex(1);
	arrowMesh.addIndex(3);
	arrowMesh.addIndex(2);
	
	// these 3 vertices will be the arrow head
	arrowMesh.addVertex( glm::vec3(1.0, -0.6, 0.0 )); // 4
	arrowMesh.addVertex( glm::vec3(1.5, 0.0, 0.0 )); // 5
	arrowMesh.addVertex( glm::vec3(1.0, 0.6, 0.0 )); // 6
	
	arrowMesh.addIndex(6);
	arrowMesh.addIndex(5);
	arrowMesh.addIndex(4);
	
	arrowMesh.setUsage(GL_STATIC_DRAW);
	
	ofSetWindowTitle("Spin Cubes");
}

//--------------------------------------------------------------
void ofApp::update(){
	ofBackground(100,100,100);

    bool bNewFrame = false;
	int sourceWidth = 320;
	int sourceHeight = 240;

	#ifdef _USE_LIVE_VIDEO
		vidGrabber.update();
		bNewFrame = vidGrabber.isFrameNew();
		sourceWidth = vidGrabber.getWidth();
		sourceHeight = vidGrabber.getHeight();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
		sourceWidth = vidPlayer.getWidth();
		sourceHeight = vidPlayer.getHeight();
	#endif
	
	// divide the source width by cvDownScale to create a smaller image to use for opencv
	int scaledWidth = sourceWidth / cvDownScale;
	int scaledHeight = sourceHeight / cvDownScale;
	
	if( currentImage.getWidth() != scaledWidth || currentImage.getHeight() != scaledHeight ){
		ofLogNotice() << "Allocating current image to: " << scaledWidth << " x " << scaledHeight << " | " << ofGetFrameNum() << endl;
		currentImage.clear();
		// allocate the ofxCvGrayscaleImage currentImage
		currentImage.allocate(scaledWidth, scaledHeight);
		currentImage.set(0);
		// free up the previous cv::Mat
		previousMat.release();
		// copy the contents of the currentImage to previousMat
		// this will also allocate the previousMat
		currentImage.getCvMat().copyTo(previousMat);
		// free up the flow cv::Mat
		flowMat.release();
		// notice that the argument order is height and then width
		// store as floats
		flowMat = cv::Mat(scaledHeight, scaledWidth, CV_32FC2);
		
		ofSetWindowShape(MIN(sourceWidth, 1920), MIN(sourceHeight, 1200));
		
		// clear the previous vectors
		spinCubes.clear();
		particles.clear();
		flowLines.clear();
		
		float spacing = 30;
		int numx = sourceWidth / spacing;
		int numy = sourceHeight / spacing;
		
		// store a mesh for drawing on the face of cubes in the draw function
		planeMesh = ofMesh::plane( spacing, spacing );
		
		for( int x = 0; x < numx; x++ ){
			for( int y = 0; y < numy; y++ ){
				glm::vec2 pos(spacing * 0.5f + (float)x * spacing, spacing * 0.5f + (float)y * spacing );
				SpinCube cube;
				cube.pos = pos;
				cube.size = spacing;
				spinCubes.push_back(cube);
				
				Particle particle;
				particle.pos = pos;
				particle.size = spacing;
				particle.basePos = particle.pos;
				particles.push_back( particle );
				
				FlowLine line;
				line.pos = pos;
				line.size = spacing;
				line.targetAngle = ofRandom(-PI, PI);
				line.angle = atan2(y, x);
				flowLines.push_back(line);
			}
		}
	}
	
	

	if (bNewFrame){

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels());
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels());
        #endif
		grayImage = colorImg;
		
		if(bMirror) {
			// flip the image horizontally
			grayImage.mirror(false, true);
		}
		// call this function if you would like to use the texture
//		grayImage.updateTexture();
		
		// scale down the grayImage into the smaller sized currentImage
		currentImage.scaleIntoMe(grayImage);
		
		if(bContrastStretch) {
			currentImage.contrastStretch();
		}
		
		if(blurAmount > 0 ) {
			currentImage.blurGaussian(blurAmount);
		}
		
		// to perform the optical flow, we will be using cv::Mat
		// so grab the cv::Mat from the current image and store in currentMat
		cv::Mat currentMat = currentImage.getCvMat();
		// calculate the optical flow
		// we pass in the previous mat, the curent one and the flowMat where the opti flow data will be stored
		cv::calcOpticalFlowFarneback(previousMat,
									 currentMat,
									 flowMat,
									 0.5, // pyr_scale
									 3, // levels
									 15, // winsize
									 3, // iterations
									 7, // poly_n
									 1.5, // poly_sigma
									 cv::OPTFLOW_FARNEBACK_GAUSSIAN);
		
		// copy over the current mat into the previous mat
		// so that the optical flow function can calculate the difference
		currentMat.copyTo(previousMat);
	}
	
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 5000.f, 1.f / 5.f );
	
	if( mode == MODE_SPIN_CUBES ){
		for( size_t i = 0; i < spinCubes.size(); i++ ){
			auto& cube = spinCubes[i];
			float percentX = cube.pos.x / sourceWidth;
			float percentY = cube.pos.y / sourceHeight;
			glm::vec2 flowForce = getOpticalFlowValueForPercent(percentX, percentY);
			// divide the yAxisAngle so that it will eventually return to 0
			cube.yAxisAngle /= 1.f + deltaTime;
			// slow down the acceleration
			cube.yAngleAcc /= (1.f+deltaTime);
			// add the flow force.x value to the angle around the y axis
			cube.yAngleAcc += flowForce.x * 20.0 * deltaTime;
			cube.yAxisAngle += cube.yAngleAcc;
			cube.yAxisAngle = ofWrapDegrees(cube.yAxisAngle);
			
			cube.xAxisAngle /= 1.f + deltaTime;
			cube.xAngleAcc /= (1.f+deltaTime);
			cube.xAngleAcc += -flowForce.y * 20.0 * deltaTime;
			cube.xAxisAngle += cube.xAngleAcc;
			cube.xAxisAngle = ofWrapDegrees(cube.xAxisAngle);
			
		}
	} else if( mode == MODE_PARTICLES ){
		size_t numParticles = particles.size();
		for( size_t i = 0; i < numParticles; i++ ){
			auto& particle = particles[i];
			float percentX = particle.pos.x / sourceWidth;
			float percentY = particle.pos.y / sourceHeight;
			glm::vec2 flowForce = getOpticalFlowValueForPercent(percentX, percentY);
			float len2 = glm::length2( flowForce );
			
			particle.vel /= 1.f + deltaTime;
			if( len2 > minLengthSquared ){
				// ok lets add some velocity
				particle.vel += flowForce * (50.0f * deltaTime);
				if( particle.bAtBasePos ) {
					particle.timeNotTouched = 0.0f;
				}
				particle.bAtBasePos = false;
				
			} else {
				particle.timeNotTouched += deltaTime;
			}
			if( particle.timeNotTouched > 2.0 ){
				particle.timeNotTouched = 0.0;
				if( !particle.bAtBasePos ) {
					particle.pos = particle.basePos;
					particle.vel = {0,0};
				}
				particle.bAtBasePos = true;
			}
			particle.pos += particle.vel * (10.0f * deltaTime);
			
		}
	}else if( mode == MODE_LINES ){
		size_t numFlowLines = flowLines.size();
		for( size_t i = 0; i < numFlowLines; i++ ){
			auto& line = flowLines[i];
			float percentX = line.pos.x / sourceWidth;
			float percentY = line.pos.y / sourceHeight;
			glm::vec2 flowForce = getOpticalFlowValueForPercent(percentX, percentY);
			
			if( glm::length2(flowForce) > minLengthSquared ){
				line.target = glm::normalize(flowForce);
				line.targetAngle = atan2(line.target.y, line.target.x );
				
			}
			line.flow = glm::mix( line.flow, glm::normalize(line.target), deltaTime );
			line.angle = ofLerpRadians(line.angle, line.targetAngle, deltaTime * 4.0 );
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor(0), ofColor(40));
	float deltaTime = ofClamp( ofGetLastFrameTime(), 1.f / 5000.f, 1.f / 5.f );
	
	ofSetColor( 255 );
	float scale = grayImage.getWidth() / MAX(1,currentImage.getWidth());
	
	
	if(grayImage.bAllocated){
		if( mode == MODE_SPIN_CUBES ){
			ofEnableDepthTest();
			const ofPixels& vpix = grayImage.getPixels();
			for( auto& cube : spinCubes ) {
				ofPushMatrix();
				ofFloatColor vcolor = vpix.getColor(cube.pos.x, cube.pos.y );
				ofFloatColor color;
				float hue = ofWrapDegrees( ofRadToDeg( fabs(cube.yAxisAngle) * 0.0075 + fabs(cube.xAxisAngle)*0.0075)-90,0, 180);
				color.setHsb( hue/360.f, 1.f-vcolor.getBrightness(), 1.0);//vcolor.getBrightness() * 0.5 + 0.5 );
				glm::mat4 ymat(1.0);
				ymat = glm::translate(ymat, glm::vec3(cube.pos.x, cube.pos.y, 0.0f));
				glm::quat rotation = glm::angleAxis(ofDegToRad(cube.yAxisAngle), glm::vec3(0,1,0) ) * glm::angleAxis(ofDegToRad(cube.xAxisAngle), glm::vec3(1,0,0));
				ymat = ymat * glm::mat4_cast(rotation);
				ofMultMatrix(ymat);
				float lerpPercent = ofClamp((fabs(cube.yAngleAcc)+fabs(cube.xAngleAcc)), 0.0, 1.0);
				ofFloatColor color2 = color;
				color2.setBrightness( ofClamp(color.getBrightness() * ((1.f-lerpPercent)*0.2f+0.8f), 0.0, 1.0) );
				ofSetColor(color2);
				ofDrawBox(0, 0, 0, cube.size*0.9);
				ofTranslate(0, 0, cube.size*0.5+1);
				ofSetColor( color );
				planeMesh.draw();
				ofPopMatrix();
			}
			ofDisableDepthTest();
		} else if( mode == MODE_PARTICLES ){
			ofSetColor(80);
			currentImage.draw(0, 0, grayImage.getWidth(), grayImage.getHeight() );
			
			size_t numParticles = particles.size();
			const ofPixels& vpix = colorImg.getPixels();
			
			for( size_t i = 0; i < numParticles; i++ ) {
				auto& particle = particles[i];
				int samplex = particle.pos.x;
				if( bMirror ) {
					samplex = colorImg.getWidth() - samplex;
				}
				ofFloatColor vcolor = vpix.getColor(samplex, particle.pos.y );
				ofFloatColor color(1,1,1,1);
				color = vcolor;
				ofSetColor(color);
				ofPushMatrix();
				ofTranslate( particle.pos );
				ofDrawCircle(0,0, particle.size * 0.5 * (vcolor.getBrightness() * 0.7f + 0.3f) );
				ofPopMatrix();
				
			}
		} else if( mode == MODE_LINES ){
			ofSetColor(80);
			currentImage.draw(0, 0, grayImage.getWidth(), grayImage.getHeight() );
			const ofPixels& vpix = grayImage.getPixels();
			for( auto& flowLine : flowLines ) {
				ofFloatColor vcolor = vpix.getColor( flowLine.pos.x, flowLine.pos.y );
				ofFloatColor color;
				float hue = ofWrapDegrees( ofRadToDeg( flowLine.angle),0, 360);
				color.setHueAngle(hue);
				color.setHsb( hue/360.f, 1.0, 1.0 );
				ofSetColor( color );
				ofPushMatrix();
				ofTranslate( flowLine.pos );
				ofRotateRad(flowLine.angle);
				float targetBrightness = vcolor.getBrightness();
				flowLine.brightness = ofLerp( flowLine.brightness, targetBrightness, 1.5 * deltaTime );
				float tsize = flowLine.size * 0.5f * (flowLine.brightness * 0.7 + 0.3);
				ofScale(tsize, tsize, 1.0);
				arrowMesh.draw();
				ofPopMatrix();
			}
		}
	}
	
	ofSetColor( 255 );
	if(currentImage.bAllocated ){
		currentImage.draw(20, 120);
	}
	
	ofSetColor(230, 40, 210 );
	
	int numCols = flowMat.cols;
	int numRows = flowMat.rows;
	
	
	if(bDrawOptiFlowVectors){
		ofPushMatrix();
		ofScale( scale, scale );
		ofMesh drawMesh;
		drawMesh.setMode(OF_PRIMITIVE_LINES);
		for( int x = 0; x < numCols; x++ ) {
			for( int y = 0; y < numRows; y++ ) {
				const cv::Point2f& fxy = flowMat.at<cv::Point2f>(y, x);
				glm::vec2 flowVector( fxy.x, fxy.y );
				if( glm::length2(flowVector) > minLengthSquared ) {
					ofFloatColor color( ofClamp(flowVector.x, 0, 1), fabs(flowVector.y), fabs(ofClamp(flowVector.x, -1, 0)) );
					drawMesh.addVertex(glm::vec3(x,y,0));
					drawMesh.addVertex(glm::vec3(x+flowVector.x * 2.0, y+flowVector.y * 2.0, 0.0f));
					drawMesh.addColor(color);
					drawMesh.addColor(color);
				}
			}
		}
		drawMesh.draw();
		ofPopMatrix();
	}
	
	string outString = "Downscale (up / down): " + ofToString(cvDownScale, 0);
	outString += "\nBlur (left / right): " + ofToString(blurAmount);
	outString += "\nMirror (m): " + ofToString(bMirror);
	outString += "\nContrast Stretch (c): " + ofToString(bContrastStretch);
	outString += "\nDraw Opti Flow (d): " +ofToString(bDrawOptiFlowVectors);
	outString += "\nMode (spacebar): " + ofToString(mode+1) + " / 3 ";
	outString += "\nFramerate: "+ofToString(ofGetFrameRate(), 0);
	ofDrawBitmapStringHighlight(outString, 20,20);
}

//--------------------------------------------------------------
glm::vec2 ofApp::getOpticalFlowValueForPercent( float xpct, float ypct ){
	glm::vec2 flowVector(0,0);
	
	if( flowMat.empty() || !grayImage.bAllocated) {
		return flowVector;
	}
	
	int tx = xpct * (float)flowMat.cols;
	int ty = ypct * (float)flowMat.rows;
	// make sure to clamp the values to the width and height of the image
	// note that flowMat.cols == width
	if( tx >= flowMat.cols ){
		tx = flowMat.cols-1;
	}
	
	// note that flowMat.rows == height
	if( ty >= flowMat.rows ){
		ty = flowMat.rows-1;
	}
	if( tx < 0 ) tx = 0;
	if( ty < 0 ) ty = 0;
	
	const cv::Point2f& fxy = flowMat.at<cv::Point2f>(ty, tx);
	flowVector = glm::vec2(fxy.x, fxy.y);
	if( glm::length2(flowVector) > minLengthSquared ){
		return flowVector;
	}
	return glm::vec2(0.0, 0.0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key){
		case OF_KEY_UP:
			cvDownScale += 1.f;
			break;
		case OF_KEY_DOWN:
			cvDownScale -= 1.0f;
			if( cvDownScale < 2) {
				cvDownScale = 2;
			}
			break;
		case 'm':
			bMirror = !bMirror;
			break;
		case 'c':
			bContrastStretch = !bContrastStretch;
			break;
		case 'd':
			bDrawOptiFlowVectors = !bDrawOptiFlowVectors;
			break;
		case OF_KEY_RIGHT:
			blurAmount ++;
			if (blurAmount > 255) blurAmount = 255;
			break;
		case OF_KEY_LEFT:
			blurAmount --;
			if (blurAmount < 0) blurAmount = 0;
			break;
		case ' ':
			mode += 1;
			if( mode > 2 ) {
				mode = 0;
			}
			string modeString = "Spin Cubes";
			if( mode == MODE_PARTICLES ) {
				modeString = "Particles";
			} else if( mode == MODE_LINES ) {
				modeString = "Flow Lines";
			}
			ofSetWindowTitle(modeString);
			break;
	}
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
