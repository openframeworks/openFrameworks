#pragma once

#include "ofPath.h"
#include "ofConstants.h"
#include "ofAppRunner.h"

#include <cairo-features.h>
#include <cairo-pdf.h>
#include <cairo-svg.h>
#include <cairo.h>
#include <deque>

enum ofCairoRendererType{
	OF_CAIRO_PDF,
	OF_CAIRO_SVG,
	OF_CAIRO_PNG
};

class ofCairoRenderer: public ofBaseRenderer{
public:
	ofCairoRenderer(ofCairoRendererType type=OF_CAIRO_PDF){
		switch(type){
		case OF_CAIRO_PDF:
			surface = cairo_pdf_surface_create("test.pdf",ofGetWidth(),ofGetHeight());
			break;
		case OF_CAIRO_SVG:
			surface = cairo_svg_surface_create("test.svg",ofGetWidth(),ofGetHeight());
			break;
		}

		cr = cairo_create(surface);
	}
	~ofCairoRenderer(){
		cairo_surface_flush(surface);
		cairo_surface_finish(surface);
		cairo_destroy(cr);
		cairo_surface_destroy(surface);
		surface = NULL;
	}

	void draw(ofPath & path){
		drawPath(path);
	}

	void draw(ofShape & shape){

	}

	bool rendersPathDirectly(){
		return true;
	}

private:

	void drawPath(const ofPath & path,bool is_subpath=false){
		if(!surface) return;
		const vector<ofPath::Command> & commands = path.getCommands();
		if(is_subpath)
			cairo_new_sub_path(cr);
		else
			cairo_new_path(cr);
		for(int i=0; i<commands.size(); i++){
			switch(commands[i].type){
			case ofPath::Command::lineTo:
				curvePoints.clear();
				cairo_line_to(cr,commands[i].to.x,commands[i].to.y);
				break;


			case ofPath::Command::curveTo:
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
				cairo_curve_to(cr,commands[i].cp1().x,commands[i].cp1().y,commands[i].cp2().x,commands[i].cp2().y,commands[i].to.x,commands[i].to.y);
				break;


			case ofPath::Command::arc2D:
				curvePoints.clear();
				// elliptic arcs not directly supported in cairo, lets scale y
				if(commands[i].radiusX()!=commands[i].radiusY()){
					float ellipse_ratio = commands[i].radiusY()/commands[i].radiusX();
					cairo_matrix_t current_matrix;
					cairo_get_matrix(cr,&current_matrix);
					cairo_matrix_t stored_matrix = current_matrix;
					cairo_matrix_translate(&current_matrix,0,-commands[i].to.y*ellipse_ratio);
					cairo_matrix_scale(&current_matrix,1,ellipse_ratio);
					cairo_matrix_translate(&current_matrix,0,commands[i].to.y*1/ellipse_ratio);
					cairo_set_matrix(cr,&current_matrix);
					cairo_arc(cr,commands[i].to.x,commands[i].to.y,commands[i].radiusX(),commands[i].angleBegin(),commands[i].angleEnd());
					cairo_set_matrix(cr,&stored_matrix);
				}else{
					cairo_arc(cr,commands[i].to.x,commands[i].to.y,commands[i].radiusX(),commands[i].angleBegin(),commands[i].angleEnd());
				}
				break;
			}
		}

		if(path.isClosed())
			cairo_close_path(cr);

		const vector<ofPath> &subpaths = path.getSubPaths();
		for(int i=0;i<subpaths.size();i++){
			drawPath(subpaths[i],true);
		}

		cairo_fill_rule_t cairo_poly_mode;
		if(path.getWindingMode()==OF_POLY_WINDING_ODD) cairo_poly_mode=CAIRO_FILL_RULE_EVEN_ODD;
		else cairo_poly_mode=CAIRO_FILL_RULE_WINDING;

		cairo_set_fill_rule(cr,cairo_poly_mode);


		if(path.getStrokeWidth()>0){
			ofColor c = path.getStrokeColor();
			cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
			cairo_set_line_width( cr, path.getStrokeWidth() );
			if(path.isFilled())
				cairo_stroke_preserve( cr );
			else
				cairo_stroke( cr );
		}
		if(path.isFilled()){
			ofColor c = path.getFillColor();
			cairo_set_source_rgba(cr, (float)c.r/255.0, (float)c.g/255.0, (float)c.b/255.0, (float)c.a/255.0);
			cairo_fill( cr );
		}
	}

	deque<ofPoint> curvePoints;
	cairo_t * cr;
	cairo_surface_t * surface;
};
