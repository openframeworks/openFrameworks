//
//  of3dGraphics.cpp
//  openFrameworksLib
//
//  Created by Nick Hardeman on 9/14/12.
//
//

#include "of3dGraphics.h"
#include "ofAppRunner.h"
#include "ofVboMesh.h"
#include <map>



enum of3dPrimitiveType {
	OF_3D_PRIMITIVE_PLANE,
    OF_3D_PRIMITIVE_SPHERE,
    OF_3D_PRIMITIVE_ICO_SPHERE,
	OF_3D_PRIMITIVE_BOX,
	OF_3D_PRIMITIVE_CONE,
    OF_3D_PRIMITIVE_CYLINDER,
    OF_3D_PRIMITIVE_BOX_WIREFRAME
};


of3dGraphics::of3dGraphics(ofBaseRenderer * renderer)
:renderer(renderer)
,plane(1.0f, 1.0f, 6, 4)
,sphere(1.0f, 20)
,icoSphere(1.0f, 2)
,box(1.f, 1.f, 1.f)
,cone( 1.f, 1.f, 9, 3, 2 )
,cylinder(1.f, 1.f, 8, 4, 2, true)
,boxWireframe(1.f, 1.f, 1.f )
,axis(ofMesh::axis())
{

    ofMesh* boxWireframeMesh = boxWireframe.getMeshPtr();
	boxWireframeMesh->clear();
	boxWireframeMesh->setMode( OF_PRIMITIVE_LINES );

	boxWireframeMesh->addVertex(ofVec3f(-.5, -.5, -.5));
	boxWireframeMesh->addVertex(ofVec3f(.5, -.5, -.5));
	boxWireframeMesh->addVertex(ofVec3f(.5, .5, -.5));
	boxWireframeMesh->addVertex(ofVec3f(-.5, .5, -.5));

	boxWireframeMesh->addVertex(ofVec3f(-.5, -.5, .5));
	boxWireframeMesh->addVertex(ofVec3f(.5, -.5, .5));
	boxWireframeMesh->addVertex(ofVec3f(.5, .5, .5));
	boxWireframeMesh->addVertex(ofVec3f(-.5, .5, .5));

	// front face
	boxWireframeMesh->addIndex(0);
	boxWireframeMesh->addIndex(1);

	boxWireframeMesh->addIndex(1);
	boxWireframeMesh->addIndex(2);

	boxWireframeMesh->addIndex(2);
	boxWireframeMesh->addIndex(3);

	boxWireframeMesh->addIndex(3);
	boxWireframeMesh->addIndex(0);

	// back face
	boxWireframeMesh->addIndex(4);
	boxWireframeMesh->addIndex(5);

	boxWireframeMesh->addIndex(5);
	boxWireframeMesh->addIndex(6);

	boxWireframeMesh->addIndex(6);
	boxWireframeMesh->addIndex(7);

	boxWireframeMesh->addIndex(7);
	boxWireframeMesh->addIndex(4);


	boxWireframeMesh->addIndex(0);
	boxWireframeMesh->addIndex(4);

	boxWireframeMesh->addIndex(1);
	boxWireframeMesh->addIndex(5);

	boxWireframeMesh->addIndex(2);
	boxWireframeMesh->addIndex(6);

	boxWireframeMesh->addIndex(3);
	boxWireframeMesh->addIndex(7);

}

//----------------------------------------------------------
void of3dGraphics::renderCached3dPrimitive( const of3dPrimitive& model ) const{
	if(renderer->getFillMode() == OF_FILLED) {
		renderer->draw(model,OF_MESH_FILL);
	} else {
		renderer->draw(model,OF_MESH_WIREFRAME);
	}
}

// Plane //
//----------------------------------------------------------
void of3dGraphics::setPlaneResolution( int columns, int rows ) {
    plane.setResolution(columns, rows);
}

// returns columns as x value of vector and rows as y value
//----------------------------------------------------------
ofVec2f of3dGraphics::getPlaneResolution() const{
    return plane.getResolution();
}

//----------------------------------------------------------
void of3dGraphics::drawPlane(float x, float y, float width, float height) const{
	of3dGraphics::drawPlane( x, y, 0, width, height);
}

//----------------------------------------------------------
void of3dGraphics::drawPlane(float x, float y, float z, float width, float height) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,1);
	m.translate(x,y,z);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( plane );
    renderer->popMatrix();
}

//----------------------------------------------------------
void of3dGraphics::drawPlane(ofPoint& position, float width, float height) const{
    drawPlane(position.x,position.y,position.z,width, height);
}

//----------------------------------------------------------
void of3dGraphics::drawPlane( float width, float height ) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,1);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( plane );
    renderer->popMatrix();
}


// UV SPHERE //
//----------------------------------------------------------
void of3dGraphics::setSphereResolution(int res) {
    if(ofGetSphereResolution() != res) {
       sphere.setResolution(res);
    }
}

//---------------------------------------------------------
int of3dGraphics::getSphereResolution() const{
    return sphere.getResolution();
}

//----------------------------------------------------------
void of3dGraphics::drawSphere(float x, float y, float z, float radius) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
	m.translate(x,y,z);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( sphere );
    renderer->popMatrix();
}

//----------------------------------------------------------
void of3dGraphics::drawSphere(float x, float y, float radius) const{
    drawSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void of3dGraphics::drawSphere(const ofPoint& position, float radius) const{
    drawSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void of3dGraphics::drawSphere(float radius) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( sphere );
    renderer->popMatrix();
}


// ICO SPHERE //
//----------------------------------------------------------
void of3dGraphics::setIcoSphereResolution( int res ) {
    if(ofGetIcoSphereResolution() != res) {
        icoSphere.setResolution(res);
    }
}

//----------------------------------------------------------
int of3dGraphics::getIcoSphereResolution() const{
    return icoSphere.getResolution();
}

//----------------------------------------------------------
void of3dGraphics::drawIcoSphere(float x, float y, float z, float radius) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
	m.translate(x,y,z);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( icoSphere );
    renderer->popMatrix();
}

//----------------------------------------------------------
void of3dGraphics::drawIcoSphere(float x, float y, float radius) const{
    drawIcoSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void of3dGraphics::drawIcoSphere(const ofPoint& position, float radius) const{
    drawIcoSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void of3dGraphics::drawIcoSphere(float radius) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,radius,radius);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( icoSphere );
    renderer->popMatrix();
}


// Cylinder //
//----------------------------------------------------------
void of3dGraphics::setCylinderResolution( int radiusSegments, int heightSegments, int capSegments ) {
    if(ofGetCylinderResolution() != ofVec3f( radiusSegments, heightSegments, capSegments )) {
        cylinder.setResolution(radiusSegments, heightSegments, capSegments);
    }
}

//----------------------------------------------------------
ofVec3f of3dGraphics::getCylinderResolution() const{
    return cylinder.getResolution();
}

//----------------------------------------------------------
void of3dGraphics::drawCylinder(float x, float y, float radius, float height) const{
    drawCylinder( x, y, 0, radius, height );
}

//----------------------------------------------------------
void of3dGraphics::drawCylinder(float x, float y, float z, float radius, float height) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
	m.translate(x,y,z);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( cylinder );
    renderer->popMatrix();
}

//----------------------------------------------------------
void of3dGraphics::drawCylinder(const ofPoint& position, float radius, float height) const{
    drawCylinder( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void of3dGraphics::drawCylinder(float radius, float height) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( cylinder );
    renderer->popMatrix();
}



// CONE //
//----------------------------------------------------------
void of3dGraphics::setConeResolution( int radiusSegments, int heightSegments, int capSegments){
    if(ofGetConeResolution() != ofVec3f( radiusSegments, heightSegments, capSegments )) {
        cone.setResolution(radiusSegments, heightSegments, capSegments);
    }
}

//----------------------------------------------------------
ofVec3f of3dGraphics::getConeResolution() const{
    return cone.getResolution();
}

//----------------------------------------------------------
void of3dGraphics::drawCone(float x, float y, float z, float radius, float height) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
	m.translate(x,y,z);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( cone );
    renderer->popMatrix();
}

//----------------------------------------------------------
void of3dGraphics::drawCone(float x, float y, float radius, float height) const{
	drawCone( x, y, 0, radius, height );
}

//----------------------------------------------------------
void of3dGraphics::drawCone(const ofPoint& position, float radius, float height) const{
	drawCone( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void of3dGraphics::drawCone(float radius, float height) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(radius,height,radius);
    renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( cone );
    renderer->popMatrix();
}



// BOX //
//----------------------------------------------------------
void of3dGraphics::setBoxResolution( int res ) {
    setBoxResolution(res, res, res);
}

//----------------------------------------------------------
void of3dGraphics::setBoxResolution( int resWidth, int resHeight, int resDepth ) {
    if(ofGetBoxResolution() != ofVec3f( resWidth, resHeight, resDepth )) {
        box.setResolution(resWidth, resHeight, resDepth);
    }
}

//----------------------------------------------------------
ofVec3f of3dGraphics::getBoxResolution() const{
    return box.getResolution();
}

//----------------------------------------------------------
void of3dGraphics::drawBox( float x, float y, float z, float width, float height, float depth) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(width,height,depth);
	m.translate(x,y,z);

    renderer->pushMatrix();
    renderer->multMatrix(m);
    if(renderer->getFillMode() == OF_FILLED) {
        renderCached3dPrimitive( box );
    } else {
        renderCached3dPrimitive( boxWireframe );
    }
    renderer->popMatrix();
}

//----------------------------------------------------------
void of3dGraphics::drawBox(float x, float y, float z, float size) const{
	drawBox( x, y, z, size, size, size );
}

//----------------------------------------------------------
void of3dGraphics::drawBox(const ofPoint& position, float width, float height, float depth) const{
	drawBox( position.x, position.y, position.z, width, height, depth );
}

//----------------------------------------------------------
void of3dGraphics::drawBox(const ofPoint& position, float size) const{
	drawBox( position.x, position.y, position.z, size, size, size );
}

//----------------------------------------------------------
void of3dGraphics::drawBox(float size) const{
	drawBox( size, size, size );
}

//----------------------------------------------------------
void of3dGraphics::drawBox( float width, float height, float depth ) const{
	drawBox(0,0,0,width,height,depth);
}


void of3dGraphics::drawAxis(float size) const{
	ofMatrix4x4 m;
	m.makeScaleMatrix(size,size,size);
	renderer->pushMatrix();
    renderer->multMatrix(m);
    renderCached3dPrimitive( axis );
    renderer->popMatrix();
}

//--------------------------------------------------------------
void of3dGraphics::drawGrid(float stepSize, size_t numberOfSteps, bool labels, bool x, bool y, bool z) const{

	ofColor c;
	ofColor prevColor = renderer->getStyle().color;

	if (x) {
		c.setHsb(0.0f, 200.0f, 255.0f);
		renderer->setColor(c);
		drawGridPlane(stepSize, numberOfSteps, labels);
	}
	if (y) {
		c.setHsb(255.0f / 3.0f, 200.0f, 255.0f);
		renderer->setColor(c);
		ofMatrix4x4 m;
		m.makeRotationMatrix(90,0,0,-1);
		renderer->pushMatrix();
		renderer->multMatrix(m);
		drawGridPlane(stepSize, numberOfSteps, labels);
		renderer->popMatrix();
	}
	if (z) {
		c.setHsb(255.0f * 2.0f / 3.0f, 200.0f, 255.0f);
		renderer->setColor(c);
		ofMatrix4x4 m;
		m.makeRotationMatrix(90,0,1,0);
		renderer->pushMatrix();
		renderer->multMatrix(m);
		drawGridPlane(stepSize, numberOfSteps, labels);
		renderer->popMatrix();
	}

	if (labels) {
		ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;
		renderer->setColor(255, 255, 255);
		float labelPos = stepSize * (numberOfSteps + 0.5);
		renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);
		renderer->drawString("x", labelPos, 0, 0);
		renderer->drawString("y", 0, labelPos, 0);
		renderer->drawString("z", 0, 0, labelPos);
		renderer->setBitmapTextMode(mode);
	}
	renderer->setColor(prevColor);
}


//--------------------------------------------------------------
void of3dGraphics::drawGridPlane(float stepSize, size_t numberOfSteps, bool labels) const{
	float scale = stepSize * numberOfSteps;
	float lineWidth = renderer->getStyle().lineWidth;

	for (int iDimension=0; iDimension<2; iDimension++)
	{
		for (size_t i=0; i <= numberOfSteps; i++)
		{
			float yz = i * stepSize;

			if (i == numberOfSteps || i == 0)
				renderer->setLineWidth(2);   // central axis or cap line
			else if ( i % 2 == 0){
				renderer->setLineWidth(1.5); // major
			} else {
				renderer->setLineWidth(1);   // minor
			}

			if (iDimension == 0 ) {
				renderer->drawLine(0, yz, -scale, 0, yz, scale);
				if (yz !=0) renderer->drawLine(0, -yz, -scale, 0, -yz, scale);
			} else {
				renderer->drawLine(0, -scale, yz, 0, scale, yz);
				if (yz !=0) renderer->drawLine(0, -scale, -yz, 0, scale, -yz);
			}
		}
	}
	renderer->setLineWidth(lineWidth);

	if (labels) {
		//draw numbers on axes
		ofColor prevColor = renderer->getStyle().color;
		ofDrawBitmapMode mode = renderer->getStyle().drawBitmapMode;

		renderer->setColor(255, 255, 255);
		renderer->setBitmapTextMode(OF_BITMAPMODE_MODEL_BILLBOARD);

		renderer->drawString(ofToString(0), 0, 0, 0);

		for (float i = 1; i <= numberOfSteps; i++)
		{
			float yz = i * stepSize;
			renderer->drawString(ofToString(yz), 0, yz, 0);
			renderer->drawString(ofToString(-yz), 0, -yz, 0);
			renderer->drawString(ofToString(yz), 0, 0, yz);
			renderer->drawString(ofToString(-yz), 0, 0, -yz);
		}

		renderer->setColor(prevColor);
		renderer->setBitmapTextMode(mode);
	}

}

//--------------------------------------------------------------
void of3dGraphics::drawArrow(const ofVec3f& start, const ofVec3f& end, float headSize) const{

	//draw line
	renderer->drawLine(start.x,start.y,start.z, end.x,end.y,end.z);

	//draw cone
	ofMatrix4x4 mat;
	mat.makeRotationMatrix( ofVec3f(0,1,0), start - end );
	mat.translate(end + ofVec3f(0,headSize*0.5,0));
	renderer->pushMatrix();
	renderer->multMatrix(mat.getPtr());
    drawCone(headSize, headSize*2.);
    renderer->popMatrix();
}

//--------------------------------------------------------------
void of3dGraphics::drawRotationAxes(float radius, float stripWidth, int circleRes) const{

	ofMesh axisXMesh;
	axisXMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	ofMesh axisYMesh;
	axisYMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	ofMesh axisZMesh;
	axisZMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	for (int j = 0; j<=circleRes; j++) {
		float x = cos(TWO_PI * j/circleRes);
		float y = sin(TWO_PI * j/circleRes);
		axisXMesh.addColor(ofFloatColor(ofFloatColor::red));
		axisXMesh.addVertex(ofVec3f(-stripWidth, x*radius, y*radius));
		axisXMesh.addColor(ofFloatColor(ofFloatColor::red));
		axisXMesh.addVertex(ofVec3f( stripWidth, x*radius, y*radius));

		axisYMesh.addColor(ofFloatColor(ofFloatColor::green));
		axisYMesh.addVertex(ofVec3f(x*radius, -stripWidth, y*radius));
		axisYMesh.addColor(ofFloatColor(ofFloatColor::green));
		axisYMesh.addVertex(ofVec3f(x*radius,  stripWidth, y*radius));

		axisZMesh.addColor(ofFloatColor(ofFloatColor::blue));
		axisZMesh.addVertex(ofVec3f(x*radius, y*radius, -stripWidth));
		axisZMesh.addColor(ofFloatColor(ofFloatColor::blue));
		axisZMesh.addVertex(ofVec3f(x*radius, y*radius,  stripWidth));
	}

	axisXMesh.draw();
	axisYMesh.draw();
	axisZMesh.draw();
	drawAxis(radius);
}


//----------------------------------------------------------
void ofSetPlaneResolution( int columns, int rows ){
	ofGetCurrentRenderer()->setPlaneResolution(columns,rows);
}

//----------------------------------------------------------
ofVec2f ofGetPlaneResolution(){
	return ofGetCurrentRenderer()->getPlaneResolution();
}

//----------------------------------------------------------
void ofDrawPlane(float x, float y, float width, float height){
	ofGetCurrentRenderer()->drawPlane(x,y,width,height);
}

//----------------------------------------------------------
void ofDrawPlane(float x, float y, float z, float width, float height){
	ofGetCurrentRenderer()->drawPlane(x,y,z,width,height);
}

//----------------------------------------------------------
void ofDrawPlane(ofPoint& position, float width, float height){
	ofGetCurrentRenderer()->drawPlane(position,width,height);
}

//----------------------------------------------------------
void ofDrawPlane( float width, float height ){
	ofGetCurrentRenderer()->drawPlane(width,height);
}


// UV Sphere
//----------------------------------------------------------
void ofSetSphereResolution(int res){
	ofGetCurrentRenderer()->setSphereResolution(res);
}

//----------------------------------------------------------
int ofGetSphereResolution(){
	return ofGetCurrentRenderer()->getSphereResolution();
}

//----------------------------------------------------------
void ofDrawSphere(float x, float y, float radius){
	ofGetCurrentRenderer()->drawSphere(x,y,radius);
}

//----------------------------------------------------------
void ofDrawSphere(float x, float y, float z, float radius){
	ofGetCurrentRenderer()->drawSphere(x,y,z,radius);
}

//----------------------------------------------------------
void ofDrawSphere(const ofPoint& position, float radius){
	ofGetCurrentRenderer()->drawSphere(position,radius);
}

//----------------------------------------------------------
void ofDrawSphere(float radius){
	ofGetCurrentRenderer()->drawSphere(radius);
}


// Ico Sphere
//----------------------------------------------------------
void ofSetIcoSphereResolution( int res ){
	ofGetCurrentRenderer()->setIcoSphereResolution(res);
}

//----------------------------------------------------------
int ofGetIcoSphereResolution(){
	return ofGetCurrentRenderer()->getIcoSphereResolution();
}

//----------------------------------------------------------
void ofDrawIcoSphere(float x, float y, float z, float radius){
	ofGetCurrentRenderer()->drawIcoSphere(x,y,z,radius);
}

//----------------------------------------------------------
void ofDrawIcoSphere(float x, float y, float radius){
	ofGetCurrentRenderer()->drawIcoSphere(x,y,radius);
}

//----------------------------------------------------------
void ofDrawIcoSphere(const ofPoint& position, float radius){
	ofGetCurrentRenderer()->drawIcoSphere(position,radius);
}

//----------------------------------------------------------
void ofDrawIcoSphere(float radius){
	ofGetCurrentRenderer()->drawIcoSphere(radius);
}


// Cylinder //
//----------------------------------------------------------
void ofSetCylinderResolution( int radiusSegments, int heightSegments, int capSegments ){
	ofGetCurrentRenderer()->setCylinderResolution(radiusSegments,heightSegments,capSegments);
}

//----------------------------------------------------------
ofVec3f ofGetCylinderResolution(){
	return ofGetCurrentRenderer()->getCylinderResolution();
}

//----------------------------------------------------------
void ofDrawCylinder(float x, float y, float radius, float height){
	ofGetCurrentRenderer()->drawCylinder(x,y,radius,height);
}

//----------------------------------------------------------
void ofDrawCylinder(float x, float y, float z, float radius, float height){
	ofGetCurrentRenderer()->drawCylinder(x,y,z,radius,height);
}

//----------------------------------------------------------
void ofDrawCylinder(const ofPoint& position, float radius, float height){
	ofGetCurrentRenderer()->drawCylinder(position,radius,height);
}

//----------------------------------------------------------
void ofDrawCylinder(float radius, float height){
	ofGetCurrentRenderer()->drawCylinder(radius,height);
}

//----------------------------------------------------------
void ofSetConeResolution( int radiusSegments, int heightSegments, int capSegments){
	ofGetCurrentRenderer()->setConeResolution(radiusSegments,heightSegments,capSegments);
}

//----------------------------------------------------------
ofVec3f ofGetConeResolution(){
	return ofGetCurrentRenderer()->getConeResolution();
}

//----------------------------------------------------------
void ofDrawCone(float x, float y, float z, float radius, float height){
	ofGetCurrentRenderer()->drawCone(x,y,z,radius,height);
}

//----------------------------------------------------------
void ofDrawCone(float x, float y, float radius, float height){
	ofGetCurrentRenderer()->drawCone(x,y,radius,height);
}

//----------------------------------------------------------
void ofDrawCone(const ofPoint& position, float radius, float height){
	ofGetCurrentRenderer()->drawCone(position,radius,height);
}

//----------------------------------------------------------
void ofDrawCone(float radius, float height){
	ofGetCurrentRenderer()->drawCone(radius,height);
}

//----------------------------------------------------------
void ofSetBoxResolution( int res ){
	ofGetCurrentRenderer()->setBoxResolution(res);
}

//----------------------------------------------------------
void ofSetBoxResolution( int resWidth, int resHeight, int resDepth ){
	ofGetCurrentRenderer()->setBoxResolution(resWidth,resHeight,resDepth);
}

//----------------------------------------------------------
ofVec3f ofGetBoxResolution(){
	return ofGetCurrentRenderer()->getBoxResolution();
}

//----------------------------------------------------------
void ofDrawBox( float x, float y, float z, float width, float height, float depth){
	ofGetCurrentRenderer()->drawBox(x,y,z,width,height,depth);
}

//----------------------------------------------------------
void ofDrawBox(float x, float y, float z, float size){
	ofGetCurrentRenderer()->drawBox(x,y,z,size);
}

//----------------------------------------------------------
void ofDrawBox(const ofPoint& position, float width, float height, float depth){
	ofGetCurrentRenderer()->drawBox(position,width,height,depth);
}

//----------------------------------------------------------
void ofDrawBox(const ofPoint& position, float size){
	ofGetCurrentRenderer()->drawBox(position,size);
}

//----------------------------------------------------------
void ofDrawBox(float size){
	ofGetCurrentRenderer()->drawBox(size);
}

void ofDrawBox( float width, float height, float depth ){
	ofGetCurrentRenderer()->drawBox(width,height,depth);
}


// Deprecated methods - for compatability with previous versions of OF //
//----------------------------------------------------------
void ofSphere(float x, float y, float z, float radius) {
    ofDrawSphere(x, y, z, radius);
}

//----------------------------------------------------------
void ofSphere(float x, float y, float radius) {
    ofDrawSphere(x, y, 0, radius);
}

//----------------------------------------------------------
void ofSphere(const ofPoint& position, float radius) {
    ofDrawSphere(position.x,position.y,position.z,radius);
}

//----------------------------------------------------------
void ofSphere(float radius) {
    ofDrawSphere(radius);
}

//----------------------------------------------------------
void ofCone(float x, float y, float z, float radius, float height) {
    ofDrawCone(x, y, z, radius, height);
}

//----------------------------------------------------------
void ofCone(float x, float y, float radius, float height) {
    ofDrawCone( x, y, 0, radius, height );
}

//----------------------------------------------------------
void ofCone(const ofPoint& position, float radius, float height) {
    ofDrawCone( position.x, position.y, position.z, radius, height );
}

//----------------------------------------------------------
void ofCone(float radius, float height) {
    ofDrawCone(radius, height );
}



// deprecated methods //
//----------------------------------------------------------
void ofBox( float x, float y, float z, float width, float height, float depth) {
    ofDrawBox(x,y,z,width,height,depth);
}

//----------------------------------------------------------
void ofBox(float x, float y, float z, float size) {
    ofDrawBox( x, y, z, size, size, size );
}

//----------------------------------------------------------
void ofBox(const ofPoint& position, float width, float height, float depth) {
    ofDrawBox( position.x, position.y, position.z, width, height, depth );
}

//----------------------------------------------------------
void ofBox(const ofPoint& position, float size) {
    ofDrawBox( position.x, position.y, position.z, size, size, size );
}

//----------------------------------------------------------
void ofBox(float size) {
    ofDrawBox( size, size, size );
}

//----------------------------------------------------------
void ofBox( float width, float height, float depth ) {
    ofDrawBox(width,height,depth);
}








