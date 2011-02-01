/*
 * ofCairoRenderer.cpp
 *
 *  Created on: 26/01/2011
 *      Author: arturo
 */

#include "ofCairoRenderer.h"
#include "ofGraphics.h"
#include "ofConstants.h"
#include "ofAppRunner.h"

void
helper_quadratic_to (cairo_t *cr,
                     double x1, double y1,
                     double x2, double y2)
{
  double x0, y0;
  cairo_get_current_point (cr, &x0, &y0);
  cairo_curve_to (cr,
                  2.0 / 3.0 * x1 + 1.0 / 3.0 * x0,
                  2.0 / 3.0 * y1 + 1.0 / 3.0 * y0,
                  2.0 / 3.0 * x1 + 1.0 / 3.0 * x2,
                  2.0 / 3.0 * y1 + 1.0 / 3.0 * y2,
                  y1, y2);
}

ofCairoRenderer::ofCairoRenderer(){
	type = PDF;
	surface = NULL;
	cr = NULL;
}

ofCairoRenderer::~ofCairoRenderer(){
	close();
}

void ofCairoRenderer::setup(string filename, Type type, bool multiPage_, bool b3D_){
	switch(type){
	case PDF:
		surface = cairo_pdf_surface_create(ofToDataPath(filename).c_str(),ofGetWidth(),ofGetHeight());
		break;
	case SVG:
		surface = cairo_svg_surface_create(ofToDataPath(filename).c_str(),ofGetWidth(),ofGetHeight());
		break;
	}
	viewportRect.set(0,0,ofGetWidth(),ofGetHeight());
	cr = cairo_create(surface);
	page = 0;
	b3D = b3D_;
	multiPage = multiPage_;
}

void ofCairoRenderer::close(){
	if(surface){
		cairo_surface_flush(surface);
		cairo_surface_finish(surface);
		cairo_surface_destroy(surface);
		surface = NULL;
	}
	if(cr){
		cairo_destroy(cr);
		cr = NULL;
	}
}

void ofCairoRenderer::draw(ofShape & path){
	drawPath(path);
}

void ofCairoRenderer::draw(ofPolyline & poly){
	ofPushStyle();
	cairo_new_path(cr);
	for(int i=0;i<poly.size();i++){
		cairo_line_to(cr,poly.getVertices()[i].x,poly.getVertices()[i].y);
	}
	if(poly.isClosed())
		cairo_close_path(cr);
	ofPopStyle();
}

void ofCairoRenderer::draw(ofShapeTessellation & shape){
	for(int i=0;i<shape.getTessellation().size();i++){
		draw(shape.getTessellation()[i]);
	}
	for(int i=0;i<shape.getOutline().size();i++){
		draw(shape.getOutline()[i]);
	}
}

ofVec3f ofCairoRenderer::transform(ofVec3f vec){
	if(!b3D) return vec;
	vec = modelView.preMult(vec);
	vec = projection.preMult(vec);

	//vec.set(vec.x/vec.z*viewportRect.width*0.5-ofGetWidth()*0.5-viewportRect.x,vec.y/vec.z*viewportRect.height*0.5-ofGetHeight()*0.5-viewportRect.y);
	vec.set(vec.x/vec.z*ofGetWidth()*0.5,vec.y/vec.z*ofGetHeight()*0.5);
	return vec;
}

void ofCairoRenderer::draw(ofVertexData & vertexData){
	if(vertexData.getNumVertices()==0) return;
	pushMatrix();
	cairo_matrix_init_identity(getCairoMatrix());
	cairo_new_path(cr);
	//if(indices.getNumIndices()){

		int i = 1;
		ofVec3f v = transform(vertexData.getVertex(vertexData.getIndex(0)));
		ofVec3f v2;
		cairo_move_to(cr,v.x,v.y);
		if(vertexData.getMode()==OF_TRIANGLE_STRIP_MODE){
			v = transform(vertexData.getVertex(vertexData.getIndex(1)));
			cairo_line_to(cr,v.x,v.y);
			v = transform(vertexData.getVertex(vertexData.getIndex(2)));
			cairo_line_to(cr,v.x,v.y);
			i=2;
		}
		for(; i<vertexData.getNumIndices(); i++){
			v = transform(vertexData.getVertex(vertexData.getIndex(i)));
			switch(vertexData.getMode()){
			case(OF_TRIANGLES_MODE):
				if((i+1)%3==0){
					cairo_line_to(cr,v.x,v.y);
					v2 = transform(vertexData.getVertex(vertexData.getIndex(i-2)));
					cairo_line_to(cr,v2.x,v2.y);
					cairo_move_to(cr,v.x,v.y);
				}else if((i+3)%3==0){
					cairo_move_to(cr,v.x,v.y);
				}else{
					cairo_line_to(cr,v.x,v.y);
				}

			break;
			case(OF_TRIANGLE_STRIP_MODE):
					v2 = transform(vertexData.getVertex(vertexData.getIndex(i-2)));
					cairo_line_to(cr,v.x,v.y);
					cairo_line_to(cr,v2.x,v2.y);
					cairo_move_to(cr,v.x,v.y);
			break;
			case(OF_TRIANGLE_FAN_MODE):
					/*triangles.addIndex((GLuint)0);
						triangles.addIndex((GLuint)1);
						triangles.addIndex((GLuint)2);
						for(int i = 2; i < vertexData.getNumVertices()-1;i++){
							triangles.addIndex((GLuint)0);
							triangles.addIndex((GLuint)i);
							triangles.addIndex((GLuint)i+1);
						}*/
			break;
			default:break;
			}
		}

	cairo_move_to(cr,vertexData.getVertex(vertexData.getIndex(vertexData.getNumIndices()-1)).x,vertexData.getVertex(vertexData.getIndex(vertexData.getNumIndices()-1)).y);

	if(ofGetStyle().lineWidth>0){
		ofColor c = ofGetStyle().color;
		cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
		cairo_set_line_width( cr, ofGetStyle().lineWidth );

		cairo_stroke( cr );
	}
	popMatrix();
}

void ofCairoRenderer::drawPath(const ofShape & path,bool is_subpath){
	if(!surface || !cr) return;
	const vector<ofShape::Command> & commands = path.getCommands();
	if(is_subpath)
		cairo_new_sub_path(cr);
	else
		cairo_new_path(cr);
	for(int i=0; i<commands.size(); i++){
		switch(commands[i].type){
		case ofShape::Command::line2DTo:
		case ofShape::Command::line3DTo:
			curvePoints.clear();
			cairo_line_to(cr,commands[i].to.x,commands[i].to.y);
			break;


		case ofShape::Command::curve2DTo:
			curvePoints.push_back(commands[i].to);

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


		case ofShape::Command::bezier2DTo:
			curvePoints.clear();
			cairo_curve_to(cr,commands[i].cp1.x,commands[i].cp1.y,commands[i].cp2.x,commands[i].cp2.y,commands[i].to.x,commands[i].to.y);
			break;

		case ofShape::Command::quadBezier2DTo:
			curvePoints.clear();
			cairo_curve_to(cr,commands[i].cp1.x,commands[i].cp1.y,commands[i].cp2.x,commands[i].cp2.y,commands[i].to.x,commands[i].to.y);
			break;


		case ofShape::Command::arc2D:
			curvePoints.clear();
			// elliptic arcs not directly supported in cairo, lets scale y
			if(commands[i].radiusX!=commands[i].radiusY){
				float ellipse_ratio = commands[i].radiusY/commands[i].radiusX;
				pushMatrix();
				translate(0,-commands[i].to.y*ellipse_ratio);
				scale(1,ellipse_ratio);
				translate(0,commands[i].to.y*1/ellipse_ratio);
				cairo_arc(cr,commands[i].to.x,commands[i].to.y,commands[i].radiusX,commands[i].angleBegin,commands[i].angleEnd);
				//cairo_set_matrix(cr,&stored_matrix);
				popMatrix();
			}else{
				cairo_arc(cr,commands[i].to.x,commands[i].to.y,commands[i].radiusX,commands[i].angleBegin,commands[i].angleEnd);
			}
			break;
		}
	}

	if(path.isClosed()){
		cairo_close_path(cr);
	}

	const vector<ofShape> &subpaths = path.getSubShapes();
	for(int i=0;i<(int)subpaths.size();i++){
		drawPath(subpaths[i],true);
	}

	cairo_fill_rule_t cairo_poly_mode;
	if(path.getWindingMode()==OF_POLY_WINDING_ODD) cairo_poly_mode=CAIRO_FILL_RULE_EVEN_ODD;
	else cairo_poly_mode=CAIRO_FILL_RULE_WINDING;

	cairo_set_fill_rule(cr,cairo_poly_mode);


	if(path.getStrokeWidth()>0){
		ofColor c = path.getStrokeColor() * ofGetStyle().color;
		cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
		cairo_set_line_width( cr, path.getStrokeWidth() );
		if(path.isFilled())
			cairo_stroke_preserve( cr );
		else
			cairo_stroke( cr );
	}
	if(path.isFilled()){
		ofColor c = path.getFillColor() * ofGetStyle().color;
		cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
		cairo_fill( cr );
	}
}

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
	if(!surface || !cr) return;
	cairo_matrix_rotate(getCairoMatrix(),degrees*DEG_TO_RAD);
	setCairoMatrix();

	if(!b3D) return;
	modelView.glRotate(180,0,0,1);
}

void ofCairoRenderer::rotate(float degrees){
	rotateZ(degrees);
}


void ofCairoRenderer::setupScreen(){
	if(!surface || !cr) return;
	if(page==0 || !multiPage){
		page=1;
		//TODO: clear page if not bg auto?
		cairo_surface_flush(surface);
		return;
	}else{
		page++;
		if(ofbClearBg()){
			cairo_show_page(cr);
		}else{
			cairo_copy_page(cr);
		}
	}
	ofSetupScreenPerspective();	// assume defaults
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
void ofCairoRenderer::viewport(float x, float y, float width, float height, bool invertY){
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();

	if (invertY){
		y = ofGetHeight() - (y + height);
	}

	viewportRect.set(x, y, width, height);
};

void ofCairoRenderer::setupScreenPerspective(float width, float height, int orientation, bool vFlip, float fov, float nearDist, float farDist){
	if(!b3D) return;
	if(width == 0) width = ofGetWidth();
	if(height == 0) height = ofGetHeight();
	if( orientation == 0 ) orientation = ofGetOrientation();

	float w = width;
	float h = height;

	//we do this because ofGetWidth and ofGetHeight return orientated widths and height
	//for the camera we need width and height of the actual screen
	if( orientation == OF_ORIENTATION_90_LEFT || orientation == OF_ORIENTATION_90_RIGHT ){
		h = width;
		w = height;
	}

	float eyeX = w / 2;
	float eyeY = h / 2;
	float halfFov = PI * fov / 360;
	float theTan = tanf(halfFov);
	float dist = eyeY / theTan;
	float aspect = (float) w / h;

	if(nearDist == 0) nearDist = dist / 10.0f;
	if(farDist == 0) farDist = dist * 10.0f;

	projection.makePerspectiveMatrix(fov,aspect,nearDist,farDist);
	modelView.makeLookAtViewMatrix(ofVec3f(eyeX,eyeY,dist),ofVec3f(eyeX,eyeY,0),ofVec3f(0,1,0));


	//note - theo checked this on iPhone and Desktop for both vFlip = false and true
	switch(orientation) {
		case OF_ORIENTATION_180:
			modelView.glRotate(-180,0,0,1);
			//glRotatef(-180, 0, 0, 1);
			if(vFlip){
				modelView.glScale(-1,-1,1);
				modelView.glTranslate(width,0,0);
				//glScalef(1, -1, 1);
				//glTranslatef(-width, 0, 0);
			}else{
				modelView.glTranslate(width,-height,0);
				//glTranslatef(-width, -height, 0);
			}

			break;

		case OF_ORIENTATION_90_RIGHT:
			//glRotatef(-90, 0, 0, 1);
			modelView.glRotate(-90,0,0,1);
			if(vFlip){
				//glScalef(-1, 1, 1);
				modelView.glScale(1,1,1);
			}else{
				//glScalef(-1, -1, 1);
				//glTranslatef(0, -height, 0);
				modelView.glScale(1,-1,1);
				modelView.glTranslate(-width,-height,0);
			}
			break;

		case OF_ORIENTATION_90_LEFT:
			//glRotatef(90, 0, 0, 1);
			modelView.glRotate(90,0,0,1);
			if(vFlip){
				//glScalef(-1, 1, 1);
				//glTranslatef(-width, -height, 0);
				modelView.glScale(1,1,1);
				modelView.glTranslate(0,-height,0);
			}else{
				//glScalef(-1, -1, 1);
				//glTranslatef(-width, 0, 0);

				modelView.glScale(1,-1,1);
				modelView.glTranslate(0,0,0);
			}
			break;

		case OF_ORIENTATION_DEFAULT:
		default:
			if(vFlip){
				//glScalef(1, -1, 1);
				//glTranslatef(0, -height, 0);
				modelView.glScale(-1,-1,1);
				modelView.glTranslate(-width,-height,0);
			}
			break;
	}
};

void ofCairoRenderer::setupScreenOrtho(float width, float height, bool vFlip, float nearDist, float farDist){
	if(!b3D) return;
	if(width == 0) width = ofGetViewportWidth();
	if(height == 0) height = ofGetViewportHeight();

	ofSetCoordHandedness(OF_RIGHT_HANDED);

	if(vFlip) {
		projection.makeOrthoMatrix(0, width, height, 0, nearDist, farDist);
		ofSetCoordHandedness(OF_LEFT_HANDED);
	}else{
		projection.makeOrthoMatrix(0, width, 0, height, nearDist, farDist);
	}
	modelView.makeIdentityMatrix();
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

void ofCairoRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){
	if(!b3D) return;
	modelView.glRotate(degrees,vecX,vecY,vecZ);
}

void ofCairoRenderer::rotateX(float degrees){
	if(!b3D) return;
	rotate(degrees,1,0,0);
}
void ofCairoRenderer::rotateY(float degrees){
	if(!b3D) return;
	rotate(degrees,0,1,0);
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
