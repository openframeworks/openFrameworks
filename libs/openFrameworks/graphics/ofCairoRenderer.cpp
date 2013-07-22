#include "ofCairoRenderer.h"
#include "ofGraphics.h"
#include "ofConstants.h"
#include "ofAppRunner.h"
#include "ofUtils.h"
#include "ofMesh.h"
#include "ofImage.h"
#include "of3dPrimitives.h"


const string ofCairoRenderer::TYPE="cairo";

_cairo_status ofCairoRenderer::stream_function(void *closure,const unsigned char *data, unsigned int length){
	((ofCairoRenderer*)closure)->streamBuffer.append((const char*)data,length);
	return CAIRO_STATUS_SUCCESS;
}

ofCairoRenderer::ofCairoRenderer(){
	type = PDF;
	surface = NULL;
	cr = NULL;
	bBackgroundAuto = true;
	rectMode = OF_RECTMODE_CORNER;
	bSmoothHinted = false;
	page = 0;
	multiPage = false;
	bFilled = OF_FILLED;
	b3D = false;
	currentMatrixMode=OF_MATRIX_MODELVIEW;
}

ofCairoRenderer::~ofCairoRenderer(){
	close();
}

void ofCairoRenderer::setup(string _filename, Type _type, bool multiPage_, bool b3D_, ofRectangle _viewport){
	if( _viewport.width == 0 || _viewport.height == 0 ){
		_viewport.set(0, 0, ofGetWidth(), ofGetHeight());
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
		surface = cairo_image_surface_create_for_data(imageBuffer.getPixels(),CAIRO_FORMAT_ARGB32,_viewport.width, _viewport.height,_viewport.width*4);
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
	viewport(viewportRect);
	page = 0;
	b3D = b3D_;
	multiPage = multiPage_;
	setStyle(ofGetStyle());
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

void ofCairoRenderer::update(){
	if(!surface || !cr)
	cairo_surface_flush(surface);
	if(page==0 || !multiPage){
		page=1;
	}else{
		page++;
		if(bClearBg()){
			cairo_show_page(cr);
		}else{
			cairo_copy_page(cr);
		}
	}
	setStyle(ofGetStyle());
}

void ofCairoRenderer::setStyle(const ofStyle & style){
	//color
	setColor((int)style.color.r, (int)style.color.g, (int)style.color.b, (int)style.color.a);

	//bg color
	//setBackgroundColor(style.bgColor);

	//circle resolution - don't worry it only recalculates the display list if the res has changed
	setCircleResolution(style.circleResolution);

	setSphereResolution(style.sphereResolution);

	//setCurveResolution(style.curveResolution);

	//line width - finally!
	setLineWidth(style.lineWidth);

	//rect mode: corner/center
	setRectMode(style.rectMode);

	//poly mode: winding type
	//setPolyMode(style.polyMode);

	//fill
	setFillMode(style.bFill?OF_FILLED:OF_OUTLINE);

	//smoothing
	//setSmoothingEnabled(style.smoothing);

	//blending
	setBlendMode(style.blendingMode);

	//bitmap draw mode
	//setDrawBitmapMode(style.drawBitmapMode);
}

void ofCairoRenderer::draw(ofPath & shape){
	cairo_new_path(cr);
	vector<ofPath::Command> & commands = shape.getCommands();
	for(int i=0;i<(int)commands.size();i++){
		draw(commands[i]);
	}

	cairo_fill_rule_t cairo_poly_mode;
	if(shape.getWindingMode()==OF_POLY_WINDING_ODD) cairo_poly_mode=CAIRO_FILL_RULE_EVEN_ODD;
	else cairo_poly_mode=CAIRO_FILL_RULE_WINDING;

	cairo_set_fill_rule(cr,cairo_poly_mode);


	ofColor prevColor;
	if(shape.getUseShapeColor()){
		prevColor = ofGetStyle().color;
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
		float lineWidth = ofGetStyle().lineWidth;
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
		setColor(prevColor);
	}
}

void ofCairoRenderer::draw(ofPolyline & poly){
	cairo_new_path(cr);
	for(int i=0;i<(int)poly.size();i++){
		cairo_line_to(cr,poly.getVertices()[i].x,poly.getVertices()[i].y);
	}
	if(poly.isClosed())
		cairo_close_path(cr);
	cairo_stroke( cr );
}

void ofCairoRenderer::draw(vector<ofPoint> & vertexData, ofPrimitiveMode drawMode){
	if(vertexData.size()==0) return;
	pushMatrix();
	cairo_matrix_init_identity(getCairoMatrix());
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
	popMatrix();
}


ofVec3f ofCairoRenderer::transform(ofVec3f vec){
	if(!b3D) return vec;
	vec = modelView.preMult(vec);
	vec = projection.preMult(vec);

	//vec.set(vec.x/vec.z*viewportRect.width*0.5-ofGetWidth()*0.5-viewportRect.x,vec.y/vec.z*viewportRect.height*0.5-ofGetHeight()*0.5-viewportRect.y);
	vec.set(vec.x/vec.z*ofGetWidth()*0.5,vec.y/vec.z*ofGetHeight()*0.5);
	return vec;
}

void ofCairoRenderer::draw(ofMesh & primitive, bool useColors, bool useTextures, bool useNormals){
	if(primitive.getNumVertices() == 0){
		return;
	}
	if(primitive.getNumIndices() == 0){
		ofMesh indexedMesh = primitive;
		indexedMesh.setupIndicesAuto();
		draw(indexedMesh, useColors, useTextures, useNormals);
		return;
	}

	pushMatrix();
	cairo_matrix_init_identity(getCairoMatrix());
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

	if(ofGetStyle().lineWidth>0){

		cairo_stroke( cr );
	}
	popMatrix();
}

void ofCairoRenderer::draw(ofMesh & vertexData, ofPolyRenderMode mode, bool useColors, bool useTextures, bool useNormals){
    if(useColors || useTextures || useNormals){
        ofLogWarning("ofCairoRenderer") << "draw(): cairo mesh rendering doesn't support colors, textures, or normals. drawing wireframe ...";
    }
	draw(vertexData,false,false,false);
}

//----------------------------------------------------------
void ofCairoRenderer::draw( of3dPrimitive& model, ofPolyRenderMode renderType  ) {

    if(model.hasScaling()) {
        ofLogWarning("ofCairoRenderer") << "draw(): cairo mesh rendering doesn't support scaling";
        //glEnable( GL_NORMALIZE );
        //glPushMatrix();
        //ofVec3f scale = model.getScale();
        //glScalef( scale.x, scale.y, scale.z);
    }

    ofMesh& mesh = model.getMesh();
    draw( mesh, renderType, mesh.usingColors(), mesh.usingTextures(), mesh.usingNormals() );

    if(model.hasScaling()) {
        //glPopMatrix();
        //glDisable( GL_NORMALIZE );
    }

}

void ofCairoRenderer::draw(ofPath::Command & command){
	if(!surface || !cr) return;
	switch(command.type){
	case ofPath::Command::moveTo:
		curvePoints.clear();
		cairo_new_sub_path(cr);
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
			ofPoint cp1 = p2 + ( p3 - p1 ) * (1.0/6);
			ofPoint cp2 = p3 + ( p2 - p4 ) * (1.0/6);

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
			pushMatrix();
			translate(0,-command.to.y*ellipse_ratio);
			scale(1,ellipse_ratio);
			translate(0,command.to.y/ellipse_ratio);
			cairo_arc(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
			//cairo_set_matrix(cr,&stored_matrix);
			popMatrix();
		}else{
			cairo_arc(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
		}
		break;

	case ofPath::Command::arcNegative:
		curvePoints.clear();
		// elliptic arcs not directly supported in cairo, lets scale y
		if(command.radiusX!=command.radiusY){
			float ellipse_ratio = command.radiusY/command.radiusX;
			pushMatrix();
			translate(0,-command.to.y*ellipse_ratio);
			scale(1,ellipse_ratio);
			translate(0,command.to.y/ellipse_ratio);
			cairo_arc_negative(cr,command.to.x,command.to.y,command.radiusX,command.angleBegin*DEG_TO_RAD,command.angleEnd*DEG_TO_RAD);
			//cairo_set_matrix(cr,&stored_matrix);
			popMatrix();
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
void ofCairoRenderer::draw(ofImage & img, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	ofPixelsRef raw = img.getPixelsRef();
	bool shouldCrop = sx != 0 || sy != 0 || sw != w || sh != h;
	ofPixels cropped;
	if(shouldCrop) {
		cropped.allocate(sw, sh, raw.getImageType());
		raw.cropTo(cropped, sx, sy, sw, sh);
	}
	ofPixelsRef pix = shouldCrop ? cropped : raw;

	pushMatrix();
	translate(x,y,z);
	scale(w/pix.getWidth(),h/pix.getHeight());
	cairo_surface_t *image;
	int stride=0;
	int picsize = pix.getWidth()* pix.getHeight();
	unsigned char *imgPix = pix.getPixels();

	static vector<unsigned char> swapPixels;

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
		image = cairo_image_surface_create_for_data(pix.getPixels(), CAIRO_FORMAT_ARGB32, pix.getWidth(), pix.getHeight(), stride);
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
		popMatrix();
		return;
		break;
	}
	cairo_set_source_surface (cr, image, 0,0);
	cairo_paint (cr);
	cairo_surface_flush(image);
	cairo_surface_destroy (image);
	popMatrix();
}

//--------------------------------------------
void ofCairoRenderer::draw(ofFloatImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	ofImage tmp = image;
	draw(tmp,x,y,z,w,h,sx,sy,sw,sh);
}

//--------------------------------------------
void ofCairoRenderer::draw(ofShortImage & image, float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh){
	ofImage tmp = image;
	draw(tmp,x,y,z,w,h,sx,sy,sw,sh);
}

//--------------------------------------------
void ofCairoRenderer::setRectMode(ofRectMode mode){
	rectMode = mode;
}

//--------------------------------------------
ofRectMode ofCairoRenderer::getRectMode(){
	return rectMode;
}

//--------------------------------------------
void ofCairoRenderer::setFillMode(ofFillFlag fill){
	bFilled = fill;
}

//--------------------------------------------
ofFillFlag ofCairoRenderer::getFillMode(){
	return bFilled;
}

//--------------------------------------------
void ofCairoRenderer::setLineWidth(float lineWidth){
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
void ofCairoRenderer::pushMatrix(){
	if(!surface || !cr) return;
	cairo_get_matrix(cr,&tmpMatrix);
	matrixStack.push(tmpMatrix);

	if(!b3D) return;
	modelViewStack.push(modelView);
}

void ofCairoRenderer::popMatrix(){
	if(!surface || !cr) return;
	cairo_set_matrix(cr,&matrixStack.top());
	matrixStack.pop();

	if(!b3D) return;
	modelView = modelViewStack.top();
	modelViewStack.pop();
}

void ofCairoRenderer::translate(float x, float y, float z ){
	if(!surface || !cr) return;
	cairo_matrix_translate(getCairoMatrix(),x,y);
	setCairoMatrix();

	if(!b3D) return;
	modelView.glTranslate(ofVec3f(x,y,z));

}

void ofCairoRenderer::translate(const ofPoint & p){
	translate(p.x,p.y,p.z);
}

void ofCairoRenderer::scale(float xAmnt, float yAmnt, float zAmnt ){
	if(!surface || !cr) return;
	cairo_matrix_scale(getCairoMatrix(),xAmnt,yAmnt);
	setCairoMatrix();

	if(!b3D) return;
	modelView.glScale(xAmnt,yAmnt,zAmnt);
}

void ofCairoRenderer::rotateZ(float degrees){
    rotate(degrees,0,0,1);
}

void ofCairoRenderer::rotate(float degrees){
	rotateZ(degrees);
}

void ofCairoRenderer::matrixMode(ofMatrixMode mode){
	currentMatrixMode = mode;
}

void ofCairoRenderer::loadIdentityMatrix (void){
	if(!surface || !cr) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		cairo_matrix_init_identity(getCairoMatrix());
		setCairoMatrix();
	}

	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView.makeIdentityMatrix();
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection.makeIdentityMatrix();
	}
}

void ofCairoRenderer::loadMatrix (const ofMatrix4x4 & m){
	if(!surface || !cr) return;
	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView = m;
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection = m;
	}
}

void ofCairoRenderer::loadMatrix (const float * m){
	if(!surface || !cr) return;
	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView.set(m);
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection.set(m);
	}

}

void ofCairoRenderer::multMatrix (const ofMatrix4x4 & m){
	if(!surface || !cr) return;
	if(!b3D) return;
	if(currentMatrixMode==OF_MATRIX_MODELVIEW){
		modelView *= m;
	}else if(currentMatrixMode==OF_MATRIX_PROJECTION){
		projection *= m;
	}
}

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

void ofCairoRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
    if(!surface || !cr) return;

    // we can only do Z-axis rotations via cairo_matrix_rotate.
    if(vecZ == 1.0f) {
        cairo_matrix_rotate(getCairoMatrix(),degrees*DEG_TO_RAD);
        setCairoMatrix();
    }

    if(!b3D) return;
    modelView.glRotate(degrees,vecX,vecY,vecZ);
}

void ofCairoRenderer::rotateX(float degrees){
	rotate(degrees,1,0,0);
}
void ofCairoRenderer::rotateY(float degrees){
	rotate(degrees,0,1,0);
}

void ofCairoRenderer::setupScreen(){
	if(!surface || !cr) return;

	setupScreenPerspective();	// assume defaults
}

// screen coordinate things / default gl values
void ofCairoRenderer::pushView(){
	viewportStack.push(viewportRect);
}

void ofCairoRenderer::popView(){
	viewportRect = viewportStack.top();
	viewportStack.pop();
};

// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
void ofCairoRenderer::viewport(ofRectangle v){
	viewport(v.x,v.y,v.width,v.height);
}

void ofCairoRenderer::viewport(float x, float y, float width, float height, bool invertY){
	if(width == 0) width = ofGetWindowWidth();
	if(height == 0) height = ofGetWindowHeight();

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

void ofCairoRenderer::setupScreenPerspective(float width, float height, float fov, float nearDist, float farDist){
	if(!b3D) return;
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
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

void ofCairoRenderer::setupScreenOrtho(float width, float height, float nearDist, float farDist){
	if(!b3D) return;
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
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

ofRectangle ofCairoRenderer::getCurrentViewport(){
	return viewportRect;
};

int ofCairoRenderer::getViewportWidth(){
	return viewportRect.width;
};

int ofCairoRenderer::getViewportHeight(){
	return viewportRect.height;
};

void ofCairoRenderer::setCoordHandedness(ofHandednessType handedness){

};

ofHandednessType ofCairoRenderer::getCoordHandedness(){
	return OF_LEFT_HANDED;
};

void ofCairoRenderer::setupGraphicDefaults(){
};

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

//----------------------------------------------------------
void ofCairoRenderer::setBackgroundAuto(bool bAuto){
	bBackgroundAuto = bAuto;
}

//----------------------------------------------------------
bool ofCairoRenderer::bClearBg(){
	return bBackgroundAuto;
}

//----------------------------------------------------------
ofFloatColor & ofCairoRenderer::getBgColor(){
	return bgColor;
}

//----------------------------------------------------------
void ofCairoRenderer::background(const ofColor & c){
	bgColor = c;
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
void ofCairoRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2){
	cairo_new_path(cr);
	cairo_move_to(cr,x1,y1);
	cairo_line_to(cr,x2,y2);

	cairo_stroke( cr );
}

//----------------------------------------------------------
void ofCairoRenderer::drawRectangle(float x, float y, float z, float w, float h){

	cairo_new_path(cr);

	if (ofGetStyle().rectMode == OF_RECTMODE_CORNER){
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

	if(bFilled==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

//----------------------------------------------------------
void ofCairoRenderer::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
	cairo_new_path(cr);

	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_line_to(cr, x3, y3);


	cairo_close_path(cr);

	if(bFilled==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

//----------------------------------------------------------
void ofCairoRenderer::drawCircle(float x, float y, float z, float radius){
	cairo_new_path(cr);
	cairo_arc(cr, x,y,radius,0,2*PI);

	cairo_close_path(cr);

	if(bFilled==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

//----------------------------------------------------------
void ofCairoRenderer::setSphereResolution(int res) {
	int n = res * 2;
	float ndiv2=(float)n/2;
	int stripVerts = (ndiv2*((n+1)*2));

	if((int)sphereVerts.size() != stripVerts ) {
		sphereVerts.clear();
		sphereVerts.resize( (ndiv2*((n+1)*2)) );
	} else {
		return;
	}

	/*
	 Original code by Paul Bourke
	 A more efficient contribution by Federico Dosil (below)
	 Draw a point for zero radius spheres
	 Use CCW facet ordering
	 http://paulbourke.net/texture_colour/texturemap/
	 */

	float theta2 = TWO_PI;
	float phi1 = -HALF_PI;
	float phi2 = HALF_PI;
	float radius = 1.f; // normalize the verts //

	int i, j;
	float j1divn,idivn,dosdivn,unodivn=1/(float)n,t1,t2,t3,cost1,cost2,cte1,cte3;
	cte3 = (theta2)/n;
	cte1 = (phi2-phi1)/ndiv2;
	dosdivn = 2*unodivn;
	ofVec3f e,p,e2,p2;

	// Handle special cases //
	if (n < 0){
		n = -n;
		ndiv2 = -ndiv2;
	}
	if (n < 4) {
		ofLogWarning("ofCairoRenderer") << "setSphereResolution(): ignoring low sphere resolution: " << n;
	}

	t2=phi1;
	cost2=cos(phi1);
	j1divn=0;

	int cindex = 0; // current index //

	for (j=0;j<ndiv2;j++) {
		t1 = t2;
		t2 += cte1;
		t3 = -cte3;
		cost1 = cost2;
		cost2 = cos(t2);
		e.y = sin(t1);
		e2.y = sin(t2);
		p.y = radius * e.y;
		p2.y = radius * e2.y;

		idivn=0;
		j1divn+=dosdivn;
		for (i=0;i<=n;i++) {
			t3 += cte3;
			e.x = cost1 * cos(t3);
			e.z = cost1 * sin(t3);
			p.x = radius * e.x;
			p.z = radius * e.z;

			cindex = (j * (n+1) + i) * 2;
			sphereVerts[cindex].set(p.x,p.y,p.z);

			e2.x = cost2 * cos(t3);
			e2.z = cost2 * sin(t3);
			p2.x = radius * e2.x;
			p2.z = radius * e2.z;
			cindex = (j * (n+1) + i) * 2 + 1;
			sphereVerts[cindex].set(p2.x,p2.y,p2.z);

			idivn += unodivn;
		}
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
void ofCairoRenderer::drawSphere(float x, float y, float z, float radius) {
	int n = ofGetStyle().sphereResolution * 2;
	float ndiv2=(float)n/2;
	int cindex = 0;

	if(sphereVerts.size() < 1) {
		// double check to make sure that setSphereResolution has been called at least once //
		setSphereResolution( ofGetStyle().sphereResolution );
	}

	spherePoints.resize( (n+1) * 2 );

	ofVec3f sp;
	int i, j;
	for (j=0;j<ndiv2;j++) {
		for (i=0;i<=n;i++) {
			cindex = (j * (n+1) + i) * 2;
			sp = sphereVerts[cindex] * radius;

			spherePoints[i*2+0].set( sp.x+x, sp.y+y, sp.z+z );

			cindex = (j * (n+1) + i) * 2 + 1;
			sp = sphereVerts[cindex] * radius;
			spherePoints[i*2+1].set( sp.x+x, sp.y+y, sp.z+z );
		}
		draw(spherePoints, OF_PRIMITIVE_TRIANGLE_STRIP);
	}

}

//----------------------------------------------------------
void ofCairoRenderer::drawEllipse(float x, float y, float z, float width, float height){
	cairo_new_path(cr);
	float ellipse_ratio = height/width;
	pushMatrix();
	translate(0,-y*ellipse_ratio);
	scale(1,ellipse_ratio);
	translate(0,y/ellipse_ratio);
	cairo_arc(cr,x,y,width*0.5,0,360);
	popMatrix();

	cairo_close_path(cr);


	if(bFilled==OF_FILLED){
		cairo_fill( cr );
	}else{
		cairo_stroke( cr );
	}
}

void ofCairoRenderer::drawString(string text, float x, float y, float z, ofDrawBitmapMode mode){
	cairo_select_font_face (cr, "Mono", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 10);
	vector<string> lines = ofSplitString(text, "\n");
	for(int i=0;i<(int)lines.size();i++){
		cairo_move_to (cr, x, y+i*14.3);
		cairo_show_text (cr, lines[i].c_str() );
	}
}

cairo_t * ofCairoRenderer::getCairoContext(){
	return cr;
}

cairo_surface_t * ofCairoRenderer::getCairoSurface(){
	return surface;
}

cairo_matrix_t * ofCairoRenderer::getCairoMatrix(){
	cairo_get_matrix(cr,&tmpMatrix);
	return &tmpMatrix;
}

void ofCairoRenderer::setCairoMatrix(){
	cairo_set_matrix(cr,&tmpMatrix);
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
