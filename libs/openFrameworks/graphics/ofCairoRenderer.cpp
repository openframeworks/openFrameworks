#include "ofCairoRenderer.h"
#include "ofConstants.h"
#include "ofUtils.h"
#include "ofMesh.h"
#include "ofImage.h"
#include "of3dPrimitives.h"
#include "ofTrueTypeFont.h"
#include "ofNode.h"
#include "ofGraphics.h"

const string ofCairoRenderer::TYPE="cairo";

_cairo_status ofCairoRenderer::stream_function(void *closure,const unsigned char *data, unsigned int length){
	((ofCairoRenderer*)closure)->streamBuffer.append((const char*)data,length);
	return CAIRO_STATUS_SUCCESS;
}

ofCairoRenderer::ofCairoRenderer()
:graphics3d(this){
	type = PDF;
	surface = NULL;
	cr = NULL;
	bBackgroundAuto = true;
	page = 0;
	multiPage = false;
	b3D = false;
	currentMatrixMode=OF_MATRIX_MODELVIEW;
}

ofCairoRenderer::~ofCairoRenderer(){
	close();
}

void ofCairoRenderer::setup(string _filename, Type _type, bool multiPage_, bool b3D_, ofRectangle _viewport){
	if( _viewport.width == 0 || _viewport.height == 0 ){
		_viewport.set(0, 0, ofGetViewportWidth(), ofGetViewportHeight());
	}

	filename = _filename;
	type = _type;
	streamBuffer.clear();

	if(type == FROM_FILE_EXTENSION){
		string ext = ofFilePath::getFileExt(filename);
		if(ofToLower(ext)=="svg"){
			type = SVG;
		}else if(ofToLower(ext)=="pdf"){
			type = PDF;
		}else{ // default to image
			type = IMAGE;
		}
	}

	if(filename != "") {
		switch(type) {
			case PDF:
			case SVG:
			case IMAGE:
				ofFilePath::createEnclosingDirectory(filename);
			case FROM_FILE_EXTENSION:
				break;
		}
	}

	switch(type){
	case PDF:
		if(filename==""){
			surface = cairo_pdf_surface_create_for_stream(&ofCairoRenderer::stream_function,this,_viewport.width, _viewport.height);
		}else{
			surface = cairo_pdf_surface_create(ofToDataPath(filename).c_str(),_viewport.width, _viewport.height);
		}
		break;
	case SVG:
		if(filename==""){
			surface = cairo_svg_surface_create_for_stream(&ofCairoRenderer::stream_function,this,_viewport.width, _viewport.height);
		}else{
			surface = cairo_svg_surface_create(ofToDataPath(filename).c_str(),_viewport.width, _viewport.height);
		}
		break;
	case IMAGE:
		imageBuffer.allocate(_viewport.width, _viewport.height, 4);
		imageBuffer.set(0);
		surface = cairo_image_surface_create_for_data(imageBuffer.getData(),CAIRO_FORMAT_ARGB32,_viewport.width, _viewport.height,_viewport.width*4);
		break;
	case FROM_FILE_EXTENSION:
		ofLogFatalError("ofCairoRenderer") << "setup(): couldn't determine type from extension for filename: \"" << _filename << "\"!";
		break;
	default:
		ofLogError("ofCairoRenderer") << "setup(): encountered unknown type for filename \"" << _filename << "\"";
		break;
	}

	cr = cairo_create(surface);
	cairo_set_antialias(cr,CAIRO_ANTIALIAS_SUBPIXEL);
	viewportRect = _viewport;
	originalViewport = _viewport;
	viewport(viewportRect);
	page = 0;
	b3D = b3D_;
	multiPage = multiPage_;
	setStyle(currentStyle);
	clear();
}

void ofCairoRenderer::setupMemoryOnly(Type _type, bool multiPage_, bool b3D_, ofRectangle _viewport){
	setup("",_type,multiPage_,b3D_,_viewport);
}

void ofCairoRenderer::flush(){
	if(surface){
		cairo_surface_flush(surface);
	}
}

void ofCairoRenderer::close(){
	if(surface){
		cairo_surface_flush(surface);
		if(type==IMAGE && filename!=""){
			imageBuffer.swapRgb();
			ofSaveImage(imageBuffer,filename);
		}
		cairo_surface_finish(surface);
		cairo_surface_destroy(surface);
		surface = NULL;
	}
	if(cr){
		cairo_destroy(cr);
		cr = NULL;
	}
}


void ofCairoRenderer::startRender(){

}

void ofCairoRenderer::finishRender(){

}

void ofCairoRenderer::update(){
	if(!surface || !cr)
	setStyle(currentStyle);
	cairo_surface_flush(surface);
	if(page==0 || !multiPage){
		page=1;
	}else{
		page++;
		if(getBackgroundAuto()){
			cairo_show_page(cr);
			clear();
		}else{
			cairo_copy_page(cr);
		}
	}
}

void ofCairoRenderer::setStyle(const ofStyle & style){
	//color
	setColor((int)style.color.r, (int)style.color.g, (int)style.color.b, (int)style.color.a);

	//bg color
	setBackgroundColor(style.bgColor);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	setCircleResolution(style.circleResolution);

	setSphereResolution(style.sphereResolution);

	setCurveResolution(style.curveResolution);

	//line width - finally!
	setLineWidth(style.lineWidth);

	//rect mode: corner/center
	setRectMode(style.rectMode);

	//poly mode: winding type
	setPolyMode(style.polyMode);

	//fill
	setFillMode(style.bFill?OF_FILLED:OF_OUTLINE);

	//smoothing
	//setSmoothingEnabled(style.smoothing);

	//blending
	setBlendMode(style.blendingMode);

	//bitmap draw mode
	//setDrawBitmapMode(style.drawBitmapMode);
	currentStyle = style;
}

void ofCairoRenderer::setCurveResolution(int resolution){
	currentStyle.curveResolution = resolution;
	path.setCurveResolution(resolution);
}

void ofCairoRenderer::draw(const ofPath & shape) const{
	cairo_new_path(cr);
	const vector<ofPath::Command> & commands = shape.getCommands();
	for(int i=0;i<(int)commands.size();i++){
		draw(commands[i]);
	}

	cairo_fill_rule_t cairo_poly_mode;
	if(shape.getWindingMode()==OF_POLY_WINDING_ODD) cairo_poly_mode=CAIRO_FILL_RULE_EVEN_ODD;
	else cairo_poly_mode=CAIRO_FILL_RULE_WINDING;

	cairo_set_fill_rule(cr,cairo_poly_mode);


	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = currentStyle.color;
	}

	if(shape.isFilled()){
		if(shape.getUseShapeColor()){
			ofColor c = shape.getFillColor();
			c.a = shape.getFillColor().a;
			cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
		}

		if(shape.hasOutline()){
			cairo_fill_preserve( cr );
		}else{
			cairo_fill(cr);
		}
	}
	if(shape.hasOutline()){
		float lineWidth = currentStyle.lineWidth;
		if(shape.getUseShapeColor()){
			ofColor c = shape.getStrokeColor();
			c.a = shape.getStrokeColor().a;
			cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
		}
		cairo_set_line_width( cr, shape.getStrokeWidth() );
		cairo_stroke( cr );
		cairo_set_line_width( cr, lineWidth );
	}

	if(shape.getUseShapeColor()){
		const_cast<ofCairoRenderer*>(this)->setColor(prevColor);
	}
}

void ofCairoRenderer::draw(const ofPolyline & poly) const{
	cairo_new_path(cr);
	for(int i=0;i<(int)poly.size();i++){
		cairo_line_to(cr,poly.getVertices()[i].x,poly.getVertices()[i].y);
	}
	if(poly.isClosed())
		cairo_close_path(cr);
	cairo_stroke( cr );
}

void ofCairoRenderer::draw(const vector<ofPoint> & vertexData, ofPrimitiveMode drawMode) const{
	if(vertexData.size()==0) return;
	ofCairoRenderer * mut_this = const_cast<ofCairoRenderer*>(this);
	mut_this->pushMatrix();

	cairo_matrix_t matrix;
	cairo_matrix_init_identity(&matrix);
	cairo_new_path(cr);
	//if(indices.getNumIndices()){

		int i = 1;
		ofVec3f v = transform(vertexData[0]);
		ofVec3f v2;
		cairo_move_to(cr,v.x,v.y);
		if(drawMode==OF_PRIMITIVE_TRIANGLE_STRIP){
			v = transform(vertexData[1]);
			cairo_line_to(cr,v.x,v.y);
			v = transform(vertexData[2]);
			cairo_line_to(cr,v.x,v.y);
			i=2;
		}
		for(; i<(int)vertexData.size(); i++){
			v = transform(vertexData[i]);
			switch(drawMode){
			case(OF_PRIMITIVE_TRIANGLES):
				if((i+1)%3==0){
					cairo_line_to(cr,v.x,v.y);
					v2 = transform(vertexData[i-2]);
					cairo_line_to(cr,v2.x,v2.y);
					cairo_move_to(cr,v.x,v.y);
				}else if((i+3)%3==0){
					cairo_move_to(cr,v.x,v.y);
				}else{
					cairo_line_to(cr,v.x,v.y);
				}

			break;
			case(OF_PRIMITIVE_TRIANGLE_STRIP):
					v2 = transform(vertexData[i-2]);
					cairo_line_to(cr,v.x,v.y);
					cairo_line_to(cr,v2.x,v2.y);
					cairo_move_to(cr,v.x,v.y);
			break;
			case(OF_PRIMITIVE_TRIANGLE_FAN):
					/*triangles.addIndex((GLuint)0);
						triangles.addIndex((GLuint)1);
						triangles.addIndex((GLuint)2);
						for(int i = 2; i < primitive.getNumVertices()-1;i++){
							triangles.addIndex((GLuint)0);
							triangles.addIndex((GLuint)i);
							triangles.addIndex((GLuint)i+1);
						}*/
			break;
			default:break;
			}
		}

	cairo_move_to(cr,vertexData[vertexData.size()-1].x,vertexData[vertexData.size()-1].y);
	cairo_stroke( cr );
	mut_this->popMatrix();
}


ofVec3f ofCairoRenderer::transform(ofVec3f vec) const{
	if(!b3D) return vec;
	vec = modelView.preMult(vec);
	vec = projection.preMult(vec);

	//vec.set(vec.x/vec.z*viewportRect.width*0.5-ofGetWidth()*0.5-viewportRect.x,vec.y/vec.z*viewportRect.height*0.5-ofGetHeight()*0.5-viewportRect.y);
	vec.set(vec.x/vec.z*viewportRect.width*0.5,vec.y/vec.z*viewportRect.height*0.5);
	return vec;
}

void ofCairoRenderer::draw(const ofMesh & primitive, bool useColors, bool useTextures, bool useNormals) const{
	if(primitive.getNumVertices() == 0){
		return;
	}
	if(primitive.getNumIndices() == 0){
		ofMesh indexedMesh = primitive;
		indexedMesh.setupIndicesAuto();
		draw(indexedMesh, useColors, useTextures, useNormals);
		return;
	}
	cairo_new_path(cr);

	cairo_matrix_t matrix;
	cairo_matrix_init_identity(&matrix);
	cairo_new_path(cr);

	int i = 1;
	ofVec3f v = transform(primitive.getVertex(primitive.getIndex(0)));
	ofVec3f v2;
	cairo_move_to(cr,v.x,v.y);
	if(primitive.getMode()==OF_PRIMITIVE_TRIANGLE_STRIP){
		v = transform(primitive.getVertex(primitive.getIndex(1)));
		cairo_line_to(cr,v.x,v.y);
		v = transform(primitive.getVertex(primitive.getIndex(2)));
		cairo_line_to(cr,v.x,v.y);
		i=2;
	}
	for(; i<primitive.getNumIndices(); i++){
		v = transform(primitive.getVertex(primitive.getIndex(i)));
		switch(primitive.getMode()){
		case(OF_PRIMITIVE_TRIANGLES):
			if((i+1)%3==0){
				cairo_line_to(cr,v.x,v.y);
				v2 = transform(primitive.getVertex(primitive.getIndex(i-2)));
				cairo_line_to(cr,v2.x,v2.y);
				cairo_move_to(cr,v.x,v.y);
			}else if((i+3)%3==0){
				cairo_move_to(cr,v.x,v.y);
			}else{
				cairo_line_to(cr,v.x,v.y);
			}

		break;
		case(OF_PRIMITIVE_TRIANGLE_STRIP):
				v2 = transform(primitive.getVertex(primitive.getIndex(i-2)));
				cairo_line_to(cr,v.x,v.y);
				cairo_line_to(cr,v2.x,v2.y);
				cairo_move_to(cr,v.x,v.y);
		break;
		case(OF_PRIMITIVE_TRIANGLE_FAN):
				/*triangles.addIndex((GLuint)0);
					triangles.addIndex((GLuint)1);
					triangles.addIndex((GLuint)2);
					for(int i = 2; i < primitive.getNumVertices()-1;i++){
						triangles.addIndex((GLuint)0);
						triangles.addIndex((GLuint)i);
						triangles.addIndex((GLuint)i+1);
					}*/
		break;
		default:break;
		}
	}

	cairo_move_to(cr,primitive.getVertex(primitive.getIndex(primitive.getNumIndices()-1)).x,primitive.getVertex(primitive.getIndex(primitive.getNumIndices()-1)).y);

	if(currentStyle.lineWidth>0){

		cairo_stroke( cr );
	}
}

void ofCairoRenderer::draw(const ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals) const{
    if(useColors || useTextures || useNormals){
        ofLogWarning("ofCairoRenderer") << "draw(): cairo mesh rendering doesn't support colors, textures, or normals. drawing wireframe ...";
    }
	draw(vertexData,false,false,false);
}

//----------------------------------------------------------
void ofCairoRenderer::draw( const of3dPrimitive& model, ofPolyRenderMode renderType  )  const{

	const_cast<ofCairoRenderer*>(this)->pushMatrix();
	const_cast<ofCairoRenderer*>(this)->multMatrix(model.getGlobalTransformMatrix());

    const ofMesh& mesh = model.getMesh();
    draw( mesh, renderType );

	const_cast<ofCairoRenderer*>(this)->popMatrix();

}

void ofCairoRenderer::draw(const ofNode& node) const{
	const_cast<ofCairoRenderer*>(this)->pushMatrix();
	const_cast<ofCairoRenderer*>(this)->multMatrix(node.getGlobalTransformMatrix());
	node.customDraw(this);
	const_cast<ofCairoRenderer*>(this)->popMatrix();
}

void ofCairoRenderer::draw(const ofPath::Command & command) const{
	if(!surface || !cr) return;
	ofCairoRenderer * mut_this = const_cast<ofCairoRenderer*>(this);
	switch(command.type){
	case ofPath::Command::moveTo:
		curvePoints.clear();
		cairo_move_to(cr,command.to.x,command.to.y);
		break;

	case ofPath::Command::lineTo:
		curvePoints.clear();
		cairo_line_to(cr,command.to.x,command.to.y);
		break;


	case ofPath::Command::curveTo:
		curvePoints.push_back(command.to);

		//code adapted from ofxVectorGraphics to convert catmull rom to bezier
		if(curvePoints.size()==4){
			ofPoint p1=curvePoints[0];
			ofPoint p2=curvePoints[1];
			ofPoint p3=curvePoints[2];
			ofPoint p4=curvePoints[3];

			//SUPER WEIRD MAGIC CONSTANT = 1/6 (this works 100% can someone explain it?)
			ofPoint cp1 = p2 + ( p3 - p1 ) * (1.0f/6.f);
			ofPoint cp2 = p3 + ( p2 - p4 ) * (1.0f/6.f);

			cairo_curve_to( cr, cp1.x, cp1.y, cp2.x, cp2.y, p3.x, p3.y );
			curvePoints.pop_front();
		}
		break;


	case ofPath::Command::bezierTo:
		curvePoints.clear();
		cairo_curve_to(cr,command.cp1.x,command.cp1.y,command.cp2.x,command.cp2.y,command.to.x,command.to.y);
		break;

	case ofPath::Command::quadBezierTo:
		curvePoints.clear();
		cairo_curve_to(cr,command.cp1.x,command.cp1.y,command.cp2.x,command.cp2.y,command.to.x,command.to.y);
		break;


	case ofPath::Command::arc:
		curvePoints.clear();
		// elliptic arcs not directly supported in cairo, lets scale y
		if(command.radiusX!=command.radiusY){
			float ellipse_ratio = command.radiusY/command.radiusX;
			mut_this->pushMatrix();
			mut_this->translate(0,-command.to.y*ellipse_ratio);
			mut_this->scale(1,ellipse_ratio);
			mut_this->translate(0,command.to.y/ellipse_ratio);
			cairo_arc(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
			//cairo_set_matrix(cr,&stored_matrix);
			mut_this->popMatrix();
		}else{
			cairo_arc(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
		}
		break;

	case ofPath::Command::arcNegative:
		curvePoints.clear();
		// elliptic arcs not directly supported in cairo, lets scale y
		if(command.radiusX!=command.radiusY){
			float ellipse_ratio = command.radiusY/command.radiusX;
			mut_this->pushMatrix();
			mut_this->translate(0,-command.to.y*ellipse_ratio);
			mut_this->scale(1,ellipse_ratio);
			mut_this->translate(0,command.to.y/ellipse_ratio);
			cairo_arc_negative(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
			//cairo_set_matrix(cr,&stored_matrix);
			mut_this->popMatrix();
		}else{
			cairo_arc_negative(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
		}
	break;

	case ofPath::Command::close:
		cairo_close_path(cr);
		break;

	}


}

//--------------------------------------------
void ofCairoRenderer::draw(const ofPixels & raw, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	bool shouldCrop = sx != 0 || sy != 0 || sw != w || sh != h;
	ofPixels cropped;
	if(shouldCrop) {
		cropped.allocate(sw, sh, raw.getPixelFormat());
		raw.cropTo(cropped, sx, sy, sw, sh);
	}
	const ofPixels & pix = shouldCrop ? cropped : raw;

	ofCairoRenderer * mut_this = const_cast<ofCairoRenderer*>(this);
	mut_this->pushMatrix();
	mut_this->translate(x,y,z);
	mut_this->scale(w/pix.getWidth(),h/pix.getHeight());
	cairo_surface_t *image;
	int stride=0;
	int picsize = pix.getWidth()* pix.getHeight();
	const unsigned char *imgPix = pix.getData();

	vector<unsigned char> swapPixels;

	switch(pix.getImageType()){
	case OF_IMAGE_COLOR:
#ifdef TARGET_LITTLE_ENDIAN
		swapPixels.resize(picsize * 4);

		for(int p= 0; p<picsize; p++) {
			swapPixels[p*4] = imgPix[p*3 +2];
			swapPixels[p*4 +1] = imgPix[p*3 +1];
			swapPixels[p*4 +2] = imgPix[p*3];
		}
#else
		swapPixels.resize(picsize * 4);

		for(int p= 0; p<picsize; p++) {
			swapPixels[p*4] = imgPix[p*3];
			swapPixels[p*4 +1] = imgPix[p*3 +1];
			swapPixels[p*4 +2] = imgPix[p*3 +2];
		}
#endif
		stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, pix.getWidth());
		image = cairo_image_surface_create_for_data(&swapPixels[0], CAIRO_FORMAT_RGB24, pix.getWidth(), pix.getHeight(), stride);
		break;
	case OF_IMAGE_COLOR_ALPHA:
#ifdef TARGET_LITTLE_ENDIAN
		swapPixels.resize(picsize * 4);

		for(int p= 0; p<picsize; p++) {
			swapPixels[p*4] = imgPix[p*4+2];
			swapPixels[p*4 +1] = imgPix[p*4+1];
			swapPixels[p*4 +2] = imgPix[p*4];
			swapPixels[p*4 +3] = imgPix[p*4+3];
		}
		stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, pix.getWidth());
		image = cairo_image_surface_create_for_data(&swapPixels[0], CAIRO_FORMAT_ARGB32, pix.getWidth(), pix.getHeight(), stride);
#else
		stride = cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, pix.getWidth());
		image = cairo_image_surface_create_for_data(pix.getData(), CAIRO_FORMAT_ARGB32, pix.getWidth(), pix.getHeight(), stride);
#endif
		break;
	case OF_IMAGE_GRAYSCALE:
		swapPixels.resize(picsize * 4);

		for(int p= 0; p<picsize; p++) {
			swapPixels[p*4] = imgPix[p];
			swapPixels[p*4 +1] = imgPix[p];
			swapPixels[p*4 +2] = imgPix[p];
		}
		stride = cairo_format_stride_for_width (CAIRO_FORMAT_RGB24, pix.getWidth());
		image = cairo_image_surface_create_for_data(&swapPixels[0], CAIRO_FORMAT_RGB24, pix.getWidth(), pix.getHeight(), stride);
		break;
	case OF_IMAGE_UNDEFINED:
	default:
		ofLogError("ofCairoRenderer") << "draw(): trying to draw undefined image type " << pix.getImageType();
		mut_this->popMatrix();
		return;
		break;
	}
	cairo_set_source_surface (cr, image, 0,0);
	cairo_paint (cr);
	cairo_surface_flush(image);
	cairo_surface_destroy (image);
	mut_this->popMatrix();
}

//--------------------------------------------
void ofCairoRenderer::draw(const ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	draw(img.getPixels(),x,y,z,w,h,sx,sy,sw,sh);
}

//--------------------------------------------
void ofCairoRenderer::draw(const ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	ofPixels tmp = image.getPixels();
	draw(tmp,x,y,z,w,h,sx,sy,sw,sh);
}

//--------------------------------------------
void ofCairoRenderer::draw(const ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const{
	ofPixels tmp = image.getPixels();
	draw(tmp,x,y,z,w,h,sx,sy,sw,sh);
}

//--------------------------------------------
void ofCairoRenderer::draw(const ofBaseVideoDraws & video, float x, float y, float w, float h) const{
	draw(video.getPixels(),x,y,0,w,h,x,y,w,h);
}

ofPath & ofCairoRenderer::getPath(){
	return path;
}

//--------------------------------------------
void ofCairoRenderer::setRectMode(ofRectMode mode){
	currentStyle.rectMode = mode;
}

//--------------------------------------------
ofRectMode ofCairoRenderer::getRectMode(){
	return currentStyle.rectMode;
}

//--------------------------------------------
void ofCairoRenderer::setFillMode(ofFillFlag fill){
	currentStyle.bFill = fill;
	if(currentStyle.bFill){
		path.setFilled(true);
		path.setStrokeWidth(0);
	}else{
		path.setFilled(false);
		path.setStrokeWidth(currentStyle.lineWidth);
	}
}

//--------------------------------------------
ofFillFlag ofCairoRenderer::getFillMode(){
	if(currentStyle.bFill){
		return OF_FILLED;
	}else{
		return OF_OUTLINE;
	}
}

//--------------------------------------------
void ofCairoRenderer::setLineWidth(float lineWidth){
	currentStyle.lineWidth = lineWidth;
	if(!currentStyle.bFill){
		path.setStrokeWidth(lineWidth);
	}
	cairo_set_line_width( cr, lineWidth );
}

//----------------------------------------------------------
void ofCairoRenderer::setDepthTest(bool depthTest) {
	// cairo does not do any depth testing
}

//--------------------------------------------
void ofCairoRenderer::setBlendMode(ofBlendMode blendMode){
	switch (blendMode){
		case OF_BLENDMODE_ALPHA:{
			cairo_set_operator(cr,CAIRO_OPERATOR_OVER);
			break;
		}

		case OF_BLENDMODE_ADD:{
			cairo_set_operator(cr,CAIRO_OPERATOR_ADD);
			break;
		}
#if (CAIRO_VERSION_MAJOR==1 && CAIRO_VERSION_MINOR>=10) || CAIRO_VERSION_MAJOR>1
		case OF_BLENDMODE_MULTIPLY:{
			cairo_set_operator(cr,CAIRO_OPERATOR_MULTIPLY);
			break;
		}

		case OF_BLENDMODE_SCREEN:{
			cairo_set_operator(cr,CAIRO_OPERATOR_SCREEN);
			break;
		}

		case OF_BLENDMODE_SUBTRACT:{
			cairo_set_operator(cr,CAIRO_OPERATOR_DIFFERENCE);
			break;
		}
#endif

		default:
			break;
	}
}

//--------------------------------------------
void ofCairoRenderer::setLineSmoothing(bool smooth){

}


// color options
//--------------------------------------------
void ofCairoRenderer::setColor(int r, int g, int b){
	setColor(r,g,b,255);
};

//--------------------------------------------
void ofCairoRenderer::setColor(int r, int g, int b, int a){
	cairo_set_source_rgba(cr, (float)r/255.0, (float)g/255.0, (float)b/255.0, (float)a/255.0);
};

//--------------------------------------------
void ofCairoRenderer::setColor(const ofColor & c){
	setColor(c.r,c.g,c.b,c.a);
};

//--------------------------------------------
void ofCairoRenderer::setColor(const ofColor & c, int _a){
	setColor(c.r,c.g,c.b,_a);
};

//--------------------------------------------
void ofCairoRenderer::setColor(int gray){
	setColor(gray,gray,gray,255);
};

//--------------------------------------------
void ofCairoRenderer::setHexColor( int hexColor ){
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
};

//--------------------------------------------
// transformations
//our openGL wrappers
ofMatrix4x4 ofCairoRenderer::getCurrentMatrix(ofMatrixMode matrixMode_) const{
	ofLogWarning() << "getCurrentMatrix not yet implemented for Cairo Renderer.";
	return ofMatrix4x4();
}

//----------------------------------------------------------
void ofCairoRenderer::pushMatrix(){
	if(!surface || !cr) return;
	cairo_matrix_t matrix;
	cairo_get_matrix(cr,&matrix);
	matrixStack.push(matrix);

	if(!b3D) return;
	modelViewStack.push(modelView);
}

//----------------------------------------------------------
void ofCairoRenderer::popMatrix(){
	if(!surface || !cr) return;
	cairo_set_matrix(cr,&matrixStack.top());
	matrixStack.pop();

	if(!b3D) return;
	modelView = modelViewStack.top();
	modelViewStack.pop();
}

//----------------------------------------------------------
void ofCairoRenderer::translate(float x, float y, float z ){
	if(!surface || !cr) return;
	cairo_matrix_t matrix;
	cairo_get_matrix(cr,&matrix);
	cairo_matrix_translate(&matrix,x,y);
	cairo_set_matrix(cr,&matrix);

	if(!b3D) return;
	modelView.glTranslate(ofVec3f(x,y,z));

}

//----------------------------------------------------------
void ofCairoRenderer::translate(const ofPoint & p){
	translate(p.x,p.y,p.z);
}

//----------------------------------------------------------
void ofCairoRenderer::scale(float xAmnt, float yAmnt, float zAmnt ){
	if(!surface || !cr) return;
	cairo_matrix_t matrix;
	cairo_get_matrix(cr,&matrix);
	cairo_matrix_scale(&matrix,xAmnt,yAmnt);
	cairo_set_matrix(cr,&matrix);

	if(!b3D) return;
	modelView.glScale(xAmnt,yAmnt,zAmnt);
}

//----------------------------------------------------------
void ofCairoRenderer::rotateZ(float degrees){
    rotate(degrees,0,0,1);
}

//----------------------------------------------------------
void ofCairoRenderer::rotate(float degrees){
	rotateZ(degrees);
}

//----------------------------------------------------------
void ofCairoRenderer::matrixMode(ofMatrixMode mode){
	currentMatrixMode = mode;
}

//----------------------------------------------------------
void ofCairoRenderer::loadIdentityMatrix (void){
	if(!surface || !cr) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		cairo_matrix_t matrix;
		cairo_matrix_init_identity(&matrix);
		cairo_set_matrix(cr,&matrix);
	}

	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView.makeIdentityMatrix();
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection.makeIdentityMatrix();
	}
}

//----------------------------------------------------------
void ofCairoRenderer::loadMatrix (const ofMatrix4x4 & m){
	if(!surface || !cr) return;
	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView = m;
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection = m;
	}
}

//----------------------------------------------------------
void ofCairoRenderer::loadMatrix (const float * m){
	if(!surface || !cr) return;
	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView.set(m);
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection.set(m);
	}

}

//----------------------------------------------------------
void ofCairoRenderer::multMatrix (const ofMatrix4x4 & m){
	if(!surface || !cr) return;
	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView *= m;
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection *= m;
	}
}

//----------------------------------------------------------
void ofCairoRenderer::multMatrix (const float * m){
	if(!surface || !cr) return;
	if(!b3D) return;
	ofMatrix4x4 mat(m);
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView *= mat;
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection *= mat;
	}
}

//----------------------------------------------------------
void ofCairoRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
    if(!surface || !cr) return;

    // we can only do Z-axis rotations via cairo_matrix_rotate.
    if(vecZ == 1.0f) {
    	cairo_matrix_t matrix;
    	cairo_get_matrix(cr,&matrix);
        cairo_matrix_rotate(&matrix,degrees*DEG_TO_RAD);
        cairo_set_matrix(cr,&matrix);
    }

    if(!b3D) return;
    modelView.glRotate(degrees,vecX,vecY,vecZ);
}

//----------------------------------------------------------
void ofCairoRenderer::rotateX(float degrees){
	rotate(degrees,1,0,0);
}
//----------------------------------------------------------
void ofCairoRenderer::rotateY(float degrees){
	rotate(degrees,0,1,0);
}

//----------------------------------------------------------
void ofCairoRenderer::setupScreen(){
	if(!surface || !cr) return;

	setupScreenPerspective();	// assume defaults
}

//----------------------------------------------------------
// screen coordinate things / default gl values
void ofCairoRenderer::pushView(){
	viewportStack.push(viewportRect);
}

//----------------------------------------------------------
void ofCairoRenderer::popView(){
	viewportRect = viewportStack.top();
	viewportStack.pop();
};

//----------------------------------------------------------
// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
void ofCairoRenderer::viewport(ofRectangle v){
	viewport(v.x,v.y,v.width,v.height);
}

//----------------------------------------------------------
void ofCairoRenderer::viewport(float x, float y, float width, float height, bool invertY){
	if(width < 0) width = originalViewport.width;
	if(height < 0) height = originalViewport.height;

	if (invertY){
		y = -y;
	}


	viewportRect.set(x, y, width, height);

	cairo_reset_clip(cr);
	cairo_new_path(cr);
	cairo_move_to(cr,viewportRect.x,viewportRect.y);
	cairo_line_to(cr,viewportRect.x+viewportRect.width,viewportRect.y);
	cairo_line_to(cr,viewportRect.x+viewportRect.width,viewportRect.y+viewportRect.height);
	cairo_line_to(cr,viewportRect.x,viewportRect.y+viewportRect.height);
	cairo_clip(cr);
};

//----------------------------------------------------------
void ofCairoRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist){
	if(!b3D) return;
	if(width < 0) width = viewportRect.width;
	if(height < 0) height = viewportRect.height;
	ofOrientation orientation = ofGetOrientation();

	float viewW = viewportRect.width;
	float viewH = viewportRect.height;

	float eyeX = viewW / 2;
	float eyeY = viewH / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) viewW / viewH;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	projection.makePerspectiveMatrix(fov,aspect,nearDist,farDist);
	modelView.makeLookAtViewMatrix(ofVec3f(eyeX,eyeY,dist),ofVec3f(eyeX,eyeY,0),ofVec3f(0,1,0));


	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	switch(orientation) {
		case OF_ORIENTATION_180:
			modelView.glRotate(-180,0,0,1);
			if(isVFlipped()){
				modelView.glScale(-1,-1,1);
				modelView.glTranslate(width,0,0);
			}else{
				modelView.glTranslate(width,-height,0);
			}

			break;

		case OF_ORIENTATION_90_RIGHT:
			modelView.glRotate(-90,0,0,1);
			if(isVFlipped()){
				modelView.glScale(1,1,1);
			}else{
				modelView.glScale(1,-1,1);
				modelView.glTranslate(-width,-height,0);
			}
			break;

		case OF_ORIENTATION_90_LEFT:
			modelView.glRotate(90,0,0,1);
			if(isVFlipped()){
				modelView.glScale(1,1,1);
				modelView.glTranslate(0,-height,0);
			}else{

				modelView.glScale(1,-1,1);
				modelView.glTranslate(0,0,0);
			}
			break;

		case OF_ORIENTATION_DEFAULT:
		default:
			if(isVFlipped()){
				modelView.glScale(-1,-1,1);
				modelView.glTranslate(-width,-height,0);
			}
			break;
	}
};

//----------------------------------------------------------
void ofCairoRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist){
	if(!b3D) return;
	if(width < 0) width = viewportRect.width;
	if(height < 0) height = viewportRect.height;
	ofOrientation orientation = ofGetOrientation();

	float viewW = viewportRect.width;
	float viewH = viewportRect.height;

	ofSetCoordHandedness(OF_RIGHT_HANDED);

	if(isVFlipped()) {
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}
	projection.makeOrthoMatrix(0, viewW, 0, viewH, nearDist, farDist);

	modelView.makeIdentityMatrix();

	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	switch(orientation) {
		case OF_ORIENTATION_180:
			modelView.glRotate(-180,0,0,1);
			if(isVFlipped()){
				modelView.glScale(-1,-1,1);
				modelView.glTranslate(width,0,0);
			}else{
				modelView.glTranslate(width,-height,0);
			}

			break;

		case OF_ORIENTATION_90_RIGHT:
			modelView.glRotate(-90,0,0,1);
			if(isVFlipped()){
				modelView.glScale(1,1,1);
			}else{
				modelView.glScale(1,-1,1);
				modelView.glTranslate(-width,-height,0);
			}
			break;

		case OF_ORIENTATION_90_LEFT:
			modelView.glRotate(90,0,0,1);
			if(isVFlipped()){
				modelView.glScale(1,1,1);
				modelView.glTranslate(0,-height,0);
			}else{

				modelView.glScale(1,-1,1);
				modelView.glTranslate(0,0,0);
			}
			break;

		case OF_ORIENTATION_DEFAULT:
		default:
			if(isVFlipped()){
				modelView.glScale(-1,-1,1);
				modelView.glTranslate(-width,-height,0);
			}
			break;
	}
};

//----------------------------------------------------------
ofRectangle ofCairoRenderer::getCurrentViewport() const{
	return viewportRect;
};

//----------------------------------------------------------
ofRectangle ofCairoRenderer::getNativeViewport() const{
	return viewportRect;
};

//----------------------------------------------------------
int ofCairoRenderer::getViewportWidth() const{
	return viewportRect.width;
};

//----------------------------------------------------------
int ofCairoRenderer::getViewportHeight() const{
	return viewportRect.height;
};

//----------------------------------------------------------
void ofCairoRenderer::setOrientation(ofOrientation orientation, bool vFlip){
	ofLogError("ofCairoRenderer") << "orientation not supported yet";
}

//----------------------------------------------------------
bool ofCairoRenderer::isVFlipped() const{
	return true;
}

//----------------------------------------------------------
void ofCairoRenderer::loadViewMatrix(const ofMatrix4x4 & m){
	ofLogError("ofCairoRenderer") << "view matrix not supported yet";
}

//----------------------------------------------------------
void ofCairoRenderer::multViewMatrix(const ofMatrix4x4 & m){
	ofLogError("ofCairoRenderer") << "view matrix not supported yet";
}

//----------------------------------------------------------
ofMatrix4x4 ofCairoRenderer::getCurrentViewMatrix() const{
	ofLogError("ofCairoRenderer") << "view matrix not supported yet";
	return ofMatrix4x4::newIdentityMatrix();
}

//----------------------------------------------------------
ofMatrix4x4 ofCairoRenderer::getCurrentNormalMatrix() const{
	ofLogError("ofCairoRenderer") << "normal matrix not supported yet";
	return ofMatrix4x4::newIdentityMatrix();
}

//----------------------------------------------------------
ofMatrix4x4 ofCairoRenderer::getCurrentOrientationMatrix() const{
	ofLogError("ofCairoRenderer") << "orientation matrix not supported yet";
	return ofMatrix4x4::newIdentityMatrix();
}

//----------------------------------------------------------
void ofCairoRenderer::setCircleResolution(int){

}

void ofCairoRenderer::setPolyMode(ofPolyWindingMode mode){
	currentStyle.polyMode = mode;
	path.setPolyWindingMode(mode);
}

//----------------------------------------------------------
void ofCairoRenderer::setCoordHandedness(ofHandednessType handedness){

};

//----------------------------------------------------------
ofHandednessType ofCairoRenderer::getCoordHandedness() const{
	return OF_LEFT_HANDED;
};

//----------------------------------------------------------
void ofCairoRenderer::setupGraphicDefaults(){
	setStyle(ofStyle());
	path.setMode(ofPath::COMMANDS);
	path.setUseShapeColor(false);
};

//----------------------------------------------------------
void ofCairoRenderer::clear(){
	if(!surface || ! cr) return;
	cairo_set_source_rgba(cr,currentStyle.bgColor.r/255., currentStyle.bgColor.g/255., currentStyle.bgColor.b/255., currentStyle.bgColor.a/255.);
	cairo_paint(cr);
}

//----------------------------------------------------------
void ofCairoRenderer::clear(float r, float g, float b, float a) {
	if(!surface || ! cr) return;
	cairo_set_source_rgba(cr,r/255., g/255., b/255., a/255.);
	cairo_paint(cr);

}

//----------------------------------------------------------
void ofCairoRenderer::clear(float brightness, float a) {
	clear(brightness, brightness, brightness, a);
}

//----------------------------------------------------------
void ofCairoRenderer::clearAlpha() {
}


void ofCairoRenderer::setBitmapTextMode(ofDrawBitmapMode mode){
	currentStyle.drawBitmapMode = mode;
}

ofStyle ofCairoRenderer::getStyle() const{
	return currentStyle;
}

void ofCairoRenderer::pushStyle(){
	styleHistory.push_back(currentStyle);
	//if we are over the max number of styles we have set, then delete the oldest styles.
	if( styleHistory.size() > OF_MAX_STYLE_HISTORY ){
		styleHistory.pop_front();
		//should we warn here?
		ofLogWarning("ofGraphics") << "ofPushStyle(): maximum number of style pushes << " << OF_MAX_STYLE_HISTORY << " reached, did you forget to pop somewhere?";
	}
}

void ofCairoRenderer::popStyle(){
	if( styleHistory.size() ){
		setStyle(styleHistory.front());
		styleHistory.pop_back();
	}
}

//----------------------------------------------------------
void ofCairoRenderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofCairoRenderer::getBackgroundAuto(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
void ofCairoRenderer::setBackgroundColor(const ofColor & c){
	currentStyle.bgColor = c;
}

//----------------------------------------------------------
ofColor ofCairoRenderer::getBackgroundColor(){
	return currentStyle.bgColor;
}

//----------------------------------------------------------
void ofCairoRenderer::background(const ofColor & c){
	setBackgroundColor(c);
	clear(c.r,c.g,c.b,c.a);
}

//----------------------------------------------------------
void ofCairoRenderer::background(float brightness) {
	background(ofColor(brightness));
}

//----------------------------------------------------------
void ofCairoRenderer::background(int hexColor, float _a){
	background ( (hexColor >> 16) & 0xff, (hexColor >> 8) & 0xff, (hexColor >> 0) & 0xff, _a);
}

//----------------------------------------------------------
void ofCairoRenderer::background(int r, int g, int b, int a){
	background(ofColor(r,g,b,a));
}


//----------------------------------------------------------
void ofCairoRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) const{
	cairo_new_path(cr);
	cairo_move_to(cr,x1,y1);
	cairo_line_to(cr,x2,y2);

	cairo_stroke( cr );
}

//----------------------------------------------------------
void ofCairoRenderer::drawRectangle(float x, float y, float z, float w, float h) const{

	cairo_new_path(cr);

	if (currentStyle.rectMode == OF_RECTMODE_CORNER){
		cairo_move_to(cr,x,y);
		cairo_line_to(cr,x+w, y);
		cairo_line_to(cr,x+w, y+h);
		cairo_line_to(cr,x, y+h);
	}else{
		cairo_move_to(cr,x-w/2.0f, y-h/2.0f);
		cairo_line_to(cr,x+w/2.0f, y-h/2.0f);
		cairo_line_to(cr,x+w/2.0f, y+h/2.0f);
		cairo_line_to(cr,x-w/2.0f, y+h/2.0f);
	}

	cairo_close_path(cr);

	if(currentStyle.bFill==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

//----------------------------------------------------------
void ofCairoRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) const{
	cairo_new_path(cr);

	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_line_to(cr, x3, y3);


	cairo_close_path(cr);

	if(currentStyle.bFill==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

//----------------------------------------------------------
void ofCairoRenderer::drawCircle(float x, float y, float z, float radius) const{
	cairo_new_path(cr);
	cairo_arc(cr, x,y,radius,0,2*PI);

	cairo_close_path(cr);

	if(currentStyle.bFill==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

//----------------------------------------------------------
void ofCairoRenderer::enableAntiAliasing(){
	cairo_set_antialias(cr,CAIRO_ANTIALIAS_SUBPIXEL);
}

//----------------------------------------------------------
void ofCairoRenderer::disableAntiAliasing(){
	cairo_set_antialias(cr,CAIRO_ANTIALIAS_NONE);
}

//----------------------------------------------------------
void ofCairoRenderer::drawEllipse(float x, float y, float z, float width, float height) const{
	ofCairoRenderer * mutThis = const_cast<ofCairoRenderer*>(this);
	cairo_new_path(cr);
	float ellipse_ratio = height/width;
	mutThis->pushMatrix();
	mutThis->translate(0,-y*ellipse_ratio);
	mutThis->scale(1,ellipse_ratio);
	mutThis->translate(0,y/ellipse_ratio);
	cairo_arc(cr,x,y,width*0.5,0,360);
	mutThis->popMatrix();

	cairo_close_path(cr);


	if(currentStyle.bFill==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

void ofCairoRenderer::drawString(string text, float x, float y, float z) const{
	cairo_select_font_face (cr, "Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 10);
	vector<string> lines = ofSplitString(text, "\n");
	for(int i=0;i<(int)lines.size();i++){
		cairo_move_to (cr, x, y+i*14.3);
		cairo_show_text (cr, lines[i].c_str() );
	}
}

void ofCairoRenderer::drawString(const ofTrueTypeFont & font, string text, float x, float y) const{
	font.drawStringAsShapes(text,x,y);
}

cairo_t * ofCairoRenderer::getCairoContext(){
	return cr;
}

cairo_surface_t * ofCairoRenderer::getCairoSurface(){
	return surface;
}

ofPixels & ofCairoRenderer::getImageSurfacePixels(){
	if(type!=IMAGE){
		ofLogError("ofCairoRenderer") << "getImageSurfacePixels(): can only get pixels from image surface";
	}
	return imageBuffer;
}

ofBuffer & ofCairoRenderer::getContentBuffer(){
	if(filename!="" || (type!=SVG && type!=PDF)){
		ofLogError("ofCairoRenderer") << "getContentBuffer(): can only get buffer from memory allocated renderer for svg or pdf";
	}
	return streamBuffer;
}

const of3dGraphics & ofCairoRenderer::get3dGraphics() const{
	return graphics3d;
}

of3dGraphics & ofCairoRenderer::get3dGraphics(){
	return graphics3d;
}
