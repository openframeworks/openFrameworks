#pragma once

#include "ofBaseTypes.h"

class ofRendererCollection: public ofBaseRenderer{
public:
	 ~ofRendererCollection(){}

	 bool rendersPathDirectly(){return true;}
	 void draw(ofPolyline & poly){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(poly);
		 }
	 }
	 void draw(ofShapeTessellation & shape){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(shape);
		 }
	 }
	 void draw(ofShape & shape){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(shape);
		 }
	 }
	 void draw(ofVertexData & vertexData){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->draw(vertexData);
		 }
	 }

	//--------------------------------------------
	// transformations
	 void pushView(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->pushView();
		 }
	 }
	 void popView(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->popView();
		 }
	 }

	// setup matrices and viewport (upto you to push and pop view before and after)
	// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
	// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
	void viewport(ofRectangle viewport){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->viewport(viewport);
		 }
	}
	 void viewport(float x = 0, float y = 0, float width = 0, float height = 0, bool invertY = true){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->viewport(x,y,width,height,invertY);
		 }
	 }
	 void setupScreenPerspective(float width = 0, float height = 0, int orientation=0, bool vFlip = true, float fov = 60, float nearDist = 0, float farDist = 0){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreenPerspective(width,height,orientation,vFlip,fov,nearDist,farDist);
		 }
	 }
	 void setupScreenOrtho(float width = 0, float height = 0, bool vFlip = true, float nearDist = -1, float farDist = 1){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreenOrtho(width,height,vFlip,nearDist,farDist);
		 }
	 }
	 ofRectangle getCurrentViewport(){
		 if(renderers.size()){
			 return renderers[0]->getCurrentViewport();
		 }else{
			 return ofRectangle();
		 }
	 }
	 int getViewportWidth(){
		 if(renderers.size()){
			 return renderers[0]->getViewportWidth();
		 }else{
			 return 0;
		 }
	 }
	 int getViewportHeight(){
		 if(renderers.size()){
			 return renderers[0]->getViewportHeight();
		 }else{
			 return 0;
		 }
	 }

	 void setCoordHandedness(ofHandednessType handedness){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setCoordHandedness(handedness);
		 }
	 }
	 ofHandednessType getCoordHandedness(){
		 if(renderers.size()){
			 return renderers[0]->getCoordHandedness();
		 }else{
			 return OF_LEFT_HANDED;
		 }
	 }

	//our openGL wrappers
	 void pushMatrix(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->pushMatrix();
		 }
	 }
	 void popMatrix(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->popMatrix();
		 }
	 }
	 void translate(float x, float y, float z = 0){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->translate(x,y,z);
		 }
	 }
	 void translate(const ofPoint & p){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->translate(p);
		 }
	 }
	 void scale(float xAmnt, float yAmnt, float zAmnt = 1){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->scale(xAmnt,yAmnt,zAmnt);
		 }
	 }
	 void rotate(float degrees, float vecX, float vecY, float vecZ){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotate(degrees,vecX,vecY,vecZ);
		 }
	 }
	 void rotateX(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotateX(degrees);
		 }
	 }
	 void rotateY(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotateY(degrees);
		 }
	 }
	 void rotateZ(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotateZ(degrees);
		 }
	 }
	 void rotate(float degrees){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->rotate(degrees);
		 }
	 }

	// screen coordinate things / default gl values
	 void setupGraphicDefaults(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupGraphicDefaults();
		 }
	 }
	 void setupScreen(){
		 for(int i=0;i<(int)renderers.size();i++){
			 renderers[i]->setupScreen();
		 }
	 }

	 vector<ofBaseRenderer *> renderers;
};
