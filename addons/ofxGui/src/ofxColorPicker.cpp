//
//  ofxColorPicker.cpp
//  ofxColorPicker
//
//  Based on ofxColorPicker by Lukasz Karluk on 13/08/2015.
//
//

#include "ofxColorPicker.h"
#include "ofGraphics.h"


namespace {
constexpr int COLOR_WHEEL_RES = 200;

struct PolCord {
	float angle;
	float radius;
};

PolCord getPolarCoordinate(const glm::vec2 & p, float radius){

	float px = p.x - radius;						// point x from center.
	float py = p.y - radius;						// point x from center.
	float pl = sqrt(px * px + py * py);     // point length from center.
	float pa = atan2(px, py);				// point angle around center.

	pa *= RAD_TO_DEG;
	pa -= 90;
	pl /= radius;

	PolCord pc;
	pc.angle = pa;
	pc.radius = pl;

	return pc;
}

glm::vec2 getPoint(float a, float r){
	glm::vec2 p;
	p.x = r * cos(-a * DEG_TO_RAD);
	p.y = r * sin(-a * DEG_TO_RAD);

	return p;
}

float saturate(float v){
	return std::min(1.f, std::max(0.f, v));
}

template<class ColorType>
ofColor_<ColorType> getCircularColor(float angle, float radius, float scale){
	radius = saturate(radius);

	angle = ofWrap(angle, 0, 360);

	float da;

	ofColor_<ColorType> c;

	if(angle < 60) {

		da  = angle / 60;
		c.r = c.limit() * scale;
		c.g = c.limit() * (da + (1 - da) * (1 - radius)) * scale;
		c.b = c.limit() * (1 - radius) * scale;
		c.a = c.limit();

	} else if(angle < 120) {

		da  = (120 - angle) / 60;
		c.r = c.limit() * (da + (1 - da) * (1 - radius)) * scale;
		c.g = c.limit() * scale;
		c.b = c.limit() * (1 - radius) * scale;
		c.a = c.limit();

	} else if(angle < 180) {

		da  = 1 - (180 - angle) / 60;
		c.r = c.limit() * (1 - radius) * scale;
		c.g = c.limit() * scale;
		c.b = c.limit() * (da + (1 - da) * (1 - radius)) * scale;
		c.a = c.limit();

	} else if(angle < 240) {

		da  = (240 - angle) / 60;
		c.r = c.limit() * (1 - radius) * scale;
		c.g = c.limit() * (da + (1 - da) * (1 - radius)) * scale;
		c.b = c.limit() * scale;
		c.a = c.limit();

	} else if(angle < 300) {

		da  = 1 - (300 - angle) / 60;
		c.r = c.limit() * (da + (1 - da) * (1 - radius)) * scale;
		c.g = c.limit() * (1 - radius) * scale;
		c.b = c.limit() * scale;
		c.a = c.limit();

	} else if(angle <= 360) {

		da  = (360 - angle) / 60;
		c.r = c.limit() * scale;
		c.g = c.limit() * (1 - radius) * scale;
		c.b = c.limit() * (da + (1 - da) * (1 - radius)) * scale;
		c.a = c.limit();
	}

	return c;
}

ofMesh rectangle(const ofRectangle & r, const ofFloatColor & c){
	ofMesh mesh;
	mesh.addVertex(r.position);
	mesh.addVertex(glm::vec3(r.x + r.width, r.y, 0));
	mesh.addVertex(glm::vec3(r.x + r.width, r.y + r.height, 0));

	mesh.addVertex(glm::vec3(r.x + r.width, r.y + r.height, 0));
	mesh.addVertex(glm::vec3(r.x, r.y + r.height, 0));
	mesh.addVertex(glm::vec3(r.x, r.y, 0));

	mesh.addColor(c);
	mesh.addColor(c);
	mesh.addColor(c);

	mesh.addColor(c);
	mesh.addColor(c);
	mesh.addColor(c);

	return mesh;
}
}

template<class ColorType>
ofxColorPicker_<ColorType>::ofxColorPicker_() {
	colorScale = 1.0;
	colorRadius = 0;
	colorAngle = 0;

	setShape(b);
}

template<class ColorType>
ofxColorPicker_<ColorType>::ofxColorPicker_(ofParameter<ofColor_<ColorType>> parameter, float w, float h){
	colorScale = 1.0;
	colorRadius = 0;
	colorAngle = 0;

	setup(parameter, w, h);
}

template<class ColorType>
ofxColorPicker_<ColorType> * ofxColorPicker_<ColorType>::setup(ofParameter<ofColor_<ColorType>> parameter, float w, float h){
	this->color.makeReferenceTo(parameter);
    auto colorChanged = [this](const ofColor_<ColorType> & c){
        if(bSettingColor){
            return;
        }
        ofFloatColor cf = c;
        float hue, saturation, brightness;
        cf.getHsb(hue,saturation,brightness);
        colorScale = brightness;
        colorRadius = saturation;
        colorAngle = 360.f * hue;
        setNeedsRedraw();
    };
    listener = color.newListener(colorChanged);
	setShape(b.x, b.y, w, h);
	colorChanged(color.get());//needs this so the color wheel shows the correct color once setup.
	return this;
}
template<class ColorType>
void ofxColorPicker_<ColorType>::setShape(float x, float y, float w, float h) {
	b.x = x;
	b.y = y;
	b.width = w;
	b.height = h;

	setNeedsRedraw();
}

template<class ColorType>
void ofxColorPicker_<ColorType>::setShape(ofRectangle r){
	setShape(r.x, r.y, r.width, r.height);
}

template<class ColorType>
ofMesh ofxColorPicker_<ColorType>::getBackground(){
	return rectangle(rectBackground, thisFillColor);
}

template<class ColorType>
ofMesh ofxColorPicker_<ColorType>::getColorPoint(){
	int x = rectColorWheel.x;
	int y = rectColorWheel.y;
	int w = rectColorWheel.width;
	int h = rectColorWheel.height;

	colorPoint = getPoint(colorAngle, colorRadius * colorWheelRadius) + glm::vec2(colorWheelRadius);
	colorPoint.x += x;
	colorPoint.y += y;

	colorPoint.x = ofClamp( colorPoint.x, x, x + w );
	colorPoint.y = ofClamp( colorPoint.y, y, y + h );

	ofPolyline circle;
	circle.arc(glm::vec3(0), 1, 1, 0, 360, true, 20);
	ofMesh meshColorPoint;

	auto center = glm::vec3(colorPoint, 0);
	for(size_t i=0;i<circle.size();i++){
		auto next = (i + 1) % circle.size();
		meshColorPoint.addVertex(center);
		meshColorPoint.addVertex(center + circle[i] * 4);
		meshColorPoint.addVertex(center + circle[next] * 4);

		meshColorPoint.addColor(ofFloatColor::black);
		meshColorPoint.addColor(ofFloatColor::black);
		meshColorPoint.addColor(ofFloatColor::black);
	}
	for(size_t i=0;i<circle.size();i++){
		auto next = (i + 1) % circle.size();
		meshColorPoint.addVertex(center);
		meshColorPoint.addVertex(center + circle[i] * 2);
		meshColorPoint.addVertex(center + circle[next] * 2);

		meshColorPoint.addColor(ofFloatColor::white);
		meshColorPoint.addColor(ofFloatColor::white);
		meshColorPoint.addColor(ofFloatColor::white);
	}
	return meshColorPoint;
}

template<class ColorType>
ofMesh ofxColorPicker_<ColorType>::getColorWheel() {
	ofPolyline circle;
	circle.arc(glm::vec3(0), 1, 1, 0, 360, false, COLOR_WHEEL_RES);

	ofMesh meshColorWheel;
	auto center = glm::vec3(rectColorWheel.x + colorWheelRadius, rectColorWheel.y + colorWheelRadius, 0);
	for(size_t i=0;i<circle.size();i++){
		auto next = (i + 1) % circle.size();
		meshColorWheel.addVertex(center);
		meshColorWheel.addVertex(center + circle[i] * colorWheelRadius);
		meshColorWheel.addVertex(center + circle[next] * colorWheelRadius);

		int j = i % COLOR_WHEEL_RES;
		float p = j / (float)COLOR_WHEEL_RES;
		float a = p * TWO_PI;

		ofFloatColor c0 = getCircularColor<float>(a * RAD_TO_DEG, 1.0, colorScale);
		meshColorWheel.addColor(ofFloatColor::white);
		meshColorWheel.addColor(c0);
		meshColorWheel.addColor(c0);
	}
    
	return meshColorWheel;
}

template<class ColorType>
ofMesh ofxColorPicker_<ColorType>::getColorScaleBar() {
    int padding = 2;
	int x = rectColorScaleBar.x;
	int y = rectColorScaleBar.y;
	int w = rectColorScaleBar.width;
	int h = rectColorScaleBar.height;


	ofMesh meshColorScaleBar;
	meshColorScaleBar.setMode(OF_PRIMITIVE_TRIANGLES);

	auto borderColor = ofColor(149, 255);
	meshColorScaleBar.append(rectangle(rectColorScaleBar, borderColor));


	int rx, ry;
	rx = x + 2;
	ry = y + 2;

	w = rectColorScaleBar.width - padding * 2;
	h = rectColorScaleBar.height - padding * 2;
	rx = rectColorScaleBar.x + 2;
	ry = rectColorScaleBar.y + 2;
	ofRectangle colorBar(rx, ry, w, h);
	meshColorScaleBar.append(rectangle(colorBar, ofFloatColor::white));
	ofFloatColor c0 = ofFloatColor::black;
	ofFloatColor c1 = getCircularColor<float>(colorAngle, colorRadius, 1.0);

	meshColorScaleBar.setColor(6, c1);
	meshColorScaleBar.setColor(7, c1);
	meshColorScaleBar.setColor(8, c0);

	meshColorScaleBar.setColor(9, c0);
	meshColorScaleBar.setColor(10, c0);
	meshColorScaleBar.setColor(11, c1);

	//--

	int rw = w;
	int rh = h - 4;
	int bx, by;
	int ch = 3;
	int cy = (1. - colorScale) * (rh + ch);

	//-- grey rect for handle. bx and by are border values.
	bx = 4;
	by = 2;
	ofRectangle handle(rx - bx, ry + cy - by, rw + bx * 2, ch + by * 2);
	meshColorScaleBar.append(rectangle(handle, borderColor));

	//-- white rect for handle. bx and by are border values.
	bx = 3;
	by = 1;
	ofRectangle handleBorder(rx - bx, ry + cy - by, rw + bx * 2, ch + by * 2);
	meshColorScaleBar.append(rectangle(handleBorder, ofFloatColor::white));
    
	return meshColorScaleBar;
}


template<class ColorType>
void ofxColorPicker_<ColorType>::setColorScale(float value) {
    if(colorScale == value) {
        return;
    }
	colorScale = value;
	bSettingColor = true;
	color = getCircularColor<ColorType>(colorAngle, colorRadius, colorScale);
	bSettingColor = false;
}

template<class ColorType>
float ofxColorPicker_<ColorType>::getColorScale() {
    return colorScale;
}

template<class ColorType>
void ofxColorPicker_<ColorType>::setColorRadius (float value) {
    colorRadius = value;
	bSettingColor = true;
	color = getCircularColor<ColorType>(colorAngle, colorRadius, colorScale);
	bSettingColor = false;
}

template<class ColorType>
float ofxColorPicker_<ColorType>::getColorRadius() {
    return colorRadius;
}

template<class ColorType>
void ofxColorPicker_<ColorType>::setColorAngle(float value) {
    colorAngle = value * 360;
	bSettingColor = true;
	color = getCircularColor<ColorType>(colorAngle, colorRadius, colorScale);
	bSettingColor = false;
}

template<class ColorType>
float ofxColorPicker_<ColorType>::getColorAngle() {
    return colorAngle / 360.0;
}


template<class ColorType>
ofAbstractParameter & ofxColorPicker_<ColorType>::getParameter(){
	return color;
}

template<class ColorType>
bool ofxColorPicker_<ColorType>::mouseMoved(ofMouseEventArgs &){
	return false;
}

template<class ColorType>
bool ofxColorPicker_<ColorType>::mousePressed(ofMouseEventArgs & mouse){
	if(!isGuiDrawing()){
		return false;
	}

	if(rectColorScaleBar.inside(mouse)){
		state = ChangingScale;
	}else if(rectColorWheel.inside(mouse)){
		state = ChangingWheel;
	}

	return mouseUpdate(mouse);
}

template<class ColorType>
bool ofxColorPicker_<ColorType>::mouseDragged(ofMouseEventArgs & mouse){
	if(!isGuiDrawing()){
        return false;
	}
	return mouseUpdate(mouse);
}

template<class ColorType>
bool ofxColorPicker_<ColorType>::mouseReleased(ofMouseEventArgs & mouse){
	if(!isGuiDrawing()){
		return false;
	}
	bool bReturn = mouseUpdate(mouse);
	state = Waiting;
	return bReturn;
}
template<class ColorType>
bool ofxColorPicker_<ColorType>::mouseUpdate(ofMouseEventArgs& mouse){
	if(rectBackground.inside(mouse)){
		switch (state) {
			case ChangingScale:{
				int relY = mouse.y - rectColorScaleBar.y;
				float scale = 1.f - saturate(relY / rectColorScaleBar.height);
				setColorScale(scale);
				
				setNeedsRedraw();
				break;
			}
			case ChangingWheel:{
				auto p = mouse - glm::vec2(rectColorWheel.position);
				auto pc = getPolarCoordinate(p, colorWheelRadius);
				
				colorAngle	= pc.angle;
				colorRadius	= saturate(pc.radius);
				
				bSettingColor = true;
				color = getCircularColor<ColorType>(colorAngle, colorRadius, colorScale);
				bSettingColor = false;
				setNeedsRedraw();
				break;
			}
			default: return true;
		}
		return true;
	}
	return false;
}

template<class ColorType>
bool ofxColorPicker_<ColorType>::mouseScrolled(ofMouseEventArgs & mouse){
	if(rectColorScaleBar.inside(mouse)){
		setColorScale(saturate(colorScale + mouse.scrollY * 0.001));
		setNeedsRedraw();
		return true;
	}else{
		return false;
	}
}


template<class ColorType>
void ofxColorPicker_<ColorType>::render(){
	geometry.draw();
}

template<class ColorType>
bool ofxColorPicker_<ColorType>::setValue(float mx, float my, bool bCheckBounds){
	return false;
}

template<class ColorType>
void ofxColorPicker_<ColorType>::generateDraw(){
	int minWH = std::min(b.width, b.height);

	colorWheelRadius = (int)std::min(b.width * 0.5f * 0.75f, b.height * 0.9f);
	colorWheelPad = (int)(minWH * 0.05);

	rectBackground.x = (int)(b.x);
	rectBackground.y = (int)(b.y);
	rectBackground.width = (int)(b.width);
	rectBackground.height = (int)(b.height);

	rectColorWheel.x = (int)(b.x + colorWheelPad);
	rectColorWheel.y = (int)(b.y + (rectBackground.height - colorWheelRadius * 2.f) / 2.f);
	rectColorWheel.width = (int)(colorWheelRadius * 2);
	rectColorWheel.height = (int)(colorWheelRadius * 2);

	rectColorScaleBar.width = (int)(colorWheelRadius * 0.2);
	rectColorScaleBar.height = rectColorWheel.height;
	rectColorScaleBar.x = (int)(rectBackground.getMaxX() - rectColorScaleBar.width - colorWheelPad);
	rectColorScaleBar.y	= (int)(rectColorWheel.y);

	geometry.clear();
	geometry.append(getBackground());
	geometry.append(getColorWheel());
	geometry.append(getColorPoint());
	geometry.append(getColorScaleBar());
}


template class ofxColorPicker_<unsigned char>;
template class ofxColorPicker_<unsigned short>;
template class ofxColorPicker_<float>;
