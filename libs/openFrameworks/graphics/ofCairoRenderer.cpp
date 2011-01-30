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

void ofCairoRenderer::setup(string filename, Type type){
	switch(type){
	case PDF:
		surface = cairo_pdf_surface_create(ofToDataPath(filename).c_str(),ofGetWidth(),ofGetHeight());
		break;
	case SVG:
		surface = cairo_svg_surface_create(ofToDataPath(filename).c_str(),ofGetWidth(),ofGetHeight());
		break;
	}

	cr = cairo_create(surface);
	page = 0;
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

void ofCairoRenderer::draw(ofPath & path){
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

void ofCairoRenderer::draw(ofShape & shape){
	for(int i=0;i<shape.getTessellation().size();i++){
		draw(shape.getTessellation()[i]);
	}
	for(int i=0;i<shape.getOutline().size();i++){
		draw(shape.getOutline()[i]);
	}
}

void ofCairoRenderer::draw(ofVertexData & vertexData){
	cairo_new_path(cr);
	for(int i=0; i<vertexData.getNumVertices(); i++){
		cairo_line_to(cr,vertexData.getVertex(i).x,vertexData.getVertex(i).y);
	}
}

void ofCairoRenderer::drawPath(const ofPath & path,bool is_subpath){
	if(!surface || !cr) return;
	const vector<ofPath::Command> & commands = path.getCommands();
	if(is_subpath)
		cairo_new_sub_path(cr);
	else
		cairo_new_path(cr);
	for(int i=0; i<commands.size(); i++){
		switch(commands[i].type){
		case ofPath::Command::line2DTo:
		case ofPath::Command::line3DTo:
			curvePoints.clear();
			cairo_line_to(cr,commands[i].to.x,commands[i].to.y);
			break;


		case ofPath::Command::curve2DTo:
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


		case ofPath::Command::bezier2DTo:
			curvePoints.clear();
			cairo_curve_to(cr,commands[i].cp1.x,commands[i].cp1.y,commands[i].cp2.x,commands[i].cp2.y,commands[i].to.x,commands[i].to.y);
			break;

		case ofPath::Command::quadBezier2DTo:
			curvePoints.clear();
			cairo_curve_to(cr,commands[i].cp1.x,commands[i].cp1.y,commands[i].cp2.x,commands[i].cp2.y,commands[i].to.x,commands[i].to.y);
			break;


		case ofPath::Command::arc2D:
			curvePoints.clear();
			// elliptic arcs not directly supported in cairo, lets scale y
			if(commands[i].radiusX!=commands[i].radiusY){
				float ellipse_ratio = commands[i].radiusY/commands[i].radiusX;
				/*cairo_matrix_t current_matrix;
				cairo_get_matrix(cr,&current_matrix);
				cairo_matrix_t stored_matrix = current_matrix;
				cairo_matrix_translate(&current_matrix,0,-commands[i].to.y*ellipse_ratio);
				cairo_matrix_scale(&current_matrix,1,ellipse_ratio);
				cairo_matrix_translate(&current_matrix,0,commands[i].to.y*1/ellipse_ratio);
				cairo_set_matrix(cr,&current_matrix);*/
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

	const vector<ofPath> &subpaths = path.getSubPaths();
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
}

void ofCairoRenderer::popMatrix(){
	if(!surface || !cr) return;
	cairo_set_matrix(cr,&matrixStack.top());
	matrixStack.pop();
}

void ofCairoRenderer::translate(float x, float y, float z ){
	if(!surface || !cr) return;
	cairo_get_matrix(cr,&tmpMatrix);
	cairo_matrix_translate(&tmpMatrix,x,y);
	cairo_set_matrix(cr,&tmpMatrix);
}

void ofCairoRenderer::translate(const ofPoint & p){
	translate(p.x,p.y,p.z);
}

void ofCairoRenderer::scale(float xAmnt, float yAmnt, float zAmnt ){
	if(!surface || !cr) return;
	cairo_get_matrix(cr,&tmpMatrix);
	cairo_matrix_scale(&tmpMatrix,xAmnt,yAmnt);
	cairo_set_matrix(cr,&tmpMatrix);
}

void ofCairoRenderer::rotateZ(float degrees){
	if(!surface || !cr) return;
	cairo_get_matrix(cr,&tmpMatrix);
	cairo_matrix_rotate(&tmpMatrix,degrees*DEG_TO_RAD);
	cairo_set_matrix(cr,&tmpMatrix);
}

void ofCairoRenderer::rotate(float degrees){
	rotateZ(degrees);
}


void ofCairoRenderer::setupScreen(){
	if(!surface || !cr) return;
	if(page==0){
		page=1;
		return;
	}else{
		page++;
		if(ofbClearBg()){
			cairo_show_page(cr);
		}else{
			cairo_copy_page(cr);
		}
	}
}

// screen coordinate things / default gl values
void ofCairoRenderer::pushView(){}
void ofCairoRenderer::popView(){};

// setup matrices and viewport (upto you to push and pop view before and after)
// if width or height are 0, assume windows dimensions (ofGetWidth(), ofGetHeight())
// if nearDist or farDist are 0 assume defaults (calculated based on width / height)
void ofCairoRenderer::viewport(float x, float y, float width, float height, bool invertY){};
void ofCairoRenderer::setupScreenPerspective(float width, float height, bool vFlip, float fov, float nearDist, float farDist){};

void ofCairoRenderer::setupScreenOrtho(float width, float height, bool vFlip, float nearDist, float farDist){};

ofRectangle ofCairoRenderer::getCurrentViewport(){return ofRectangle();};
int ofCairoRenderer::getViewportWidth(){return ofGetWidth();};
int ofCairoRenderer::getViewportHeight(){return ofGetHeight();};

void ofCairoRenderer::setCoordHandedness(ofHandednessType handedness){};
ofHandednessType ofCairoRenderer::getCoordHandedness(){return OF_LEFT_HANDED;};

void ofCairoRenderer::setupGraphicDefaults(){};


void ofCairoRenderer::rotate(float degrees, float vecX, float vecY, float vecZ){}
void ofCairoRenderer::rotateX(float degrees){}
void ofCairoRenderer::rotateY(float degrees){}


cairo_t * ofCairoRenderer::getCairoContext(){
	return cr;
}

cairo_surface_t * ofCairoRenderer::getCairoSurface(){
	return surface;
}
