///*
// *  ofNodeWithTarget.cpp
// *  openFrameworksLib
// *
// *  Created by Memo Akten on 15/01/2011.
// *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
// *
// */
//
//#include "ofNodeWithTarget.h"
//
////----------------------------------------
//ofNodeWithTarget::ofNodeWithTarget() {
//	usingTarget = true;
//}
//
//
////----------------------------------------
//void ofNodeWithTarget::setTarget(const ofVec3f& targetPos) {
//	target.setPosition(targetPos);
//}
//
////----------------------------------------
//void ofNodeWithTarget::setTarget(ofNode& targetNode) {
//	this->target.setPosition(ofVec3f(0, 0, 0));
//	this->target.setParent(targetNode);
//	
////	if(target) delete target;
////	target = &targetNode;
//}
//
//
////----------------------------------------
//ofNode& ofNodeWithTarget::getTarget() {
//	return target;
//}
//
//
////----------------------------------------
//void ofNodeWithTarget::enableTarget() {
//	usingTarget = true;
//}
//
////----------------------------------------
//void ofNodeWithTarget::disableTarget() {
//	usingTarget = false;
//}
//
////----------------------------------------
//bool ofNodeWithTarget::isUsingTarget() {
//	return usingTarget;
//}
//
////----------------------------------------
//void ofNodeWithTarget::onPositionChanged() {
//	if(usingTarget) lookAt(target);
//}