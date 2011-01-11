///*
// *  ofTransformation.cpp
// *  openFrameworksLib
// *
// *  Created by Memo Akten on 10/01/2011.
// *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
// *
// */
//
//#include "ofTransformation3D.h"
//
//ofTransformation3D::ofTransformation3D() {
//	setPosition(ofVec3f());
//	setLookAt(ofVec3f());
//	setOrientation(ofQuaternion());
//	setScale(1);
//}
//
//
//
////----------------------------------------
//void ofTransformation3D::setPosition(float px, float py, float pz) {
//	m44.setTranslation(px, py, pz);
//}
//
////----------------------------------------
//void ofTransformation3D::setPosition(ofVec3f const p) {
//	m44.setTranslation(p);
//}
//
//
////----------------------------------------
//ofVec3f ofTransformation3D::getPosition() const {
//	return ofVec3f(
//}
//
//
//
////----------------------------------------
//void ofTransformation3D::setScale(float s) {
//	setScale(ofVec3f(s, s, s));
//}
//
////----------------------------------------
//void ofTransformation3D::setScale(float sx, float sy, float sz) {
//	setScale(ofVec3f(sx, sy, sz));
//}
//
////----------------------------------------
//void ofTransformation3D::setScale(Vec3f const scale) {
//	matrixIsDirty = true;
//	this->scale = scale;
//}
//
////----------------------------------------
//ofVec3f ofTransformation3D::getScale() const {
//	return scale;
//}
//
//
//
//void ofTransformation3D::updateMatrix() {
//	if(matrixIsDirty) {
//		
//		matrixIsDirty = false;
//	}
//}