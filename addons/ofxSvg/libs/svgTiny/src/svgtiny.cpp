/*
 * This file is part of Libsvgtiny
 * Licensed under the MIT License,
 *                http://opensource.org/licenses/mit-license.php
 * Copyright 2008-2009 James Bursa <james@semichrome.net>
 */

#include <assert.h>
#include <math.h>
#include <setjmp.h>
#ifndef _MSC_VER
	#include <stdbool.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <libxml/parser.h>
//#include <libxml/debugXML.h>
#include "svgtiny.h"
#include "svgtiny_internal.h"

using namespace std;

#ifdef _MSC_VER
long lroundf (float x) {
	long xl = (long) x;
	return x - xl > .5 ? xl + 1 : xl;
}  
#endif

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define KAPPA		0.5522847498

static svgtiny_code svgtiny_parse_svg(Poco::XML::Element *svg,
		struct svgtiny_parse_state state);
static svgtiny_code svgtiny_parse_path(Poco::XML::Element *path,
		struct svgtiny_parse_state state);
static svgtiny_code svgtiny_parse_rect(Poco::XML::Element *rect,
		struct svgtiny_parse_state state);
static svgtiny_code svgtiny_parse_circle(Poco::XML::Element *circle,
		struct svgtiny_parse_state state);
static svgtiny_code svgtiny_parse_ellipse(Poco::XML::Element *ellipse,
		struct svgtiny_parse_state state);
static svgtiny_code svgtiny_parse_line(Poco::XML::Element *line,
		struct svgtiny_parse_state state);
static svgtiny_code svgtiny_parse_poly(Poco::XML::Element *poly,
		struct svgtiny_parse_state state, bool polygon);
static svgtiny_code svgtiny_parse_text(Poco::XML::Element *text,
		struct svgtiny_parse_state state);
static void svgtiny_parse_position_attributes(const Poco::XML::Element *node,
		const struct svgtiny_parse_state state,
		float *x, float *y, float *width, float *height);
static void svgtiny_parse_paint_attributes(const Poco::XML::Element *node,
		struct svgtiny_parse_state *state);
static void svgtiny_parse_font_attributes(const Poco::XML::Element *node,
		struct svgtiny_parse_state *state);
static void svgtiny_parse_transform_attributes(Poco::XML::Element *node,
		struct svgtiny_parse_state *state);
static svgtiny_code svgtiny_add_path(float *p, unsigned int n,
		struct svgtiny_parse_state *state);


/**
 * Create a new svgtiny_diagram structure.
 */

struct svgtiny_diagram *svgtiny_create(void)
{
	struct svgtiny_diagram *diagram;

	diagram = (svgtiny_diagram *) malloc(sizeof *diagram);
	if (!diagram)
		return 0;

	diagram->shape = 0;
	diagram->shape_count = 0;
	diagram->error_line = 0;
	diagram->error_message = 0;

	return diagram;
}


/**
 * Parse a block of memory into a svgtiny_diagram.
 */

svgtiny_code svgtiny_parse(struct svgtiny_diagram *diagram,
		const char *buffer, size_t size, const char *url,
		int viewport_width, int viewport_height)
{
	
    Poco::XML::Document *document;
	//Poco::XML::Element *svg;
    Poco::XML::Element *svg;
	struct svgtiny_parse_state state;
	float x, y, width, height;
	svgtiny_code code;

	assert(diagram);
	assert(buffer);
	assert(url);
    
    std::string str(buffer);
    
    Poco::XML::DOMParser parser;
    document = parser.parseString(str);

    svg = document->documentElement();
    
    //std::cout << svg->localName() << std::endl;
    
	if (!svg)
		return svgtiny_NOT_SVG;
    if (svg->localName().compare("svg") != 0)
		return svgtiny_NOT_SVG;

	/* get graphic dimensions */
	state.diagram = diagram;
	state.document = document;
	state.viewport_width = viewport_width;
	state.viewport_height = viewport_height;
	svgtiny_parse_position_attributes(svg, state, &x, &y, &width, &height);
	diagram->width = width;
	diagram->height = height;

	/* set up parsing state */
	state.viewport_width = width;
	state.viewport_height = height;
	state.ctm.a = 1; /*(float) viewport_width / (float) width;*/
	state.ctm.b = 0;
	state.ctm.c = 0;
	state.ctm.d = 1; /*(float) viewport_height / (float) height;*/
	state.ctm.e = 0; /*x;*/
	state.ctm.f = 0; /*y;*/
	/*state.style = css_base_style;
	state.style.font_size.value.length.value = option_font_size * 0.1;*/
	state.fill = 0x000000;
	state.stroke = svgtiny_TRANSPARENT;
	state.stroke_width = 1;
	state.linear_gradient_stop_count = 0;

	/* parse tree */
	code = svgtiny_parse_svg(svg, state);

	/* free XML tree */
	//xmlFreeDoc(document);

	return code;
}


/**
 * Parse a <svg> or <g> element node.
 */

svgtiny_code svgtiny_parse_svg(Poco::XML::Element *svg,
		struct svgtiny_parse_state state)
{
	float x, y, width, height;
	Poco::XML::Attr *view_box;
	Poco::XML::Element *child;

	svgtiny_parse_position_attributes(svg, state, &x, &y, &width, &height);
	svgtiny_parse_paint_attributes(svg, &state);
	svgtiny_parse_font_attributes(svg, &state);
    
    view_box = svg->getAttributeNode("viewBox");
    
	if (view_box) {
		//const char *s = (const char *) view_box->children->content;
        const char *s = (const char *) view_box->getValue().c_str();
		float min_x, min_y, vwidth, vheight;
		if (sscanf(s, "%f,%f,%f,%f",
				&min_x, &min_y, &vwidth, &vheight) == 4 ||
				sscanf(s, "%f %f %f %f",
				&min_x, &min_y, &vwidth, &vheight) == 4) {
			state.ctm.a = (float) state.viewport_width / vwidth;
			state.ctm.d = (float) state.viewport_height / vheight;
			state.ctm.e += -min_x * state.ctm.a;
			state.ctm.f += -min_y * state.ctm.d;
		}
	}

	svgtiny_parse_transform_attributes(svg, &state);


    // this is how this should work, but it doesn't
    //Poco::XML::NodeIterator it(svg, Poco::XML::NodeFilter::SHOW_ELEMENT | Poco::XML::NodeFilter::SHOW_TEXT);
    //Poco::XML::Node* pNode = it.nextNode();
    //while (pNode)
    
    
    // Note: this should be using the NodeIterator, but it doesn't seem to work as advertised when using
    // a Node as the root for the iterator constructor. Really weird.
    Poco::XML::ChildNodesList *cnl = ( Poco::XML::ChildNodesList *) svg->childNodes();
    int i  = 0, l = cnl->length();
    while( i < l )
    {
    
		svgtiny_code code = svgtiny_OK;
        
        child = (Poco::XML::Element *) cnl->item(i);
        
        //std::cout << child->localName() << std::endl;

        // I think this can't happen?
		if (child->nodeType() == Poco::XML::Element::ELEMENT_NODE) {
			const char *name = (const char *) child->localName().c_str();
            
			if (strcmp(name, "svg") == 0)
				code = svgtiny_parse_svg(child, state);
			else if (strcmp(name, "g") == 0)
				code = svgtiny_parse_svg(child, state);
			else if (strcmp(name, "a") == 0)
				code = svgtiny_parse_svg(child, state);
			else if (strcmp(name, "path") == 0)
				code = svgtiny_parse_path(child, state);
			else if (strcmp(name, "rect") == 0)
				code = svgtiny_parse_rect(child, state);
			else if (strcmp(name, "circle") == 0)
				code = svgtiny_parse_circle(child, state);
			else if (strcmp(name, "ellipse") == 0)
				code = svgtiny_parse_ellipse(child, state);
			else if (strcmp(name, "line") == 0)
				code = svgtiny_parse_line(child, state);
			else if (strcmp(name, "polyline") == 0)
				code = svgtiny_parse_poly(child, state, false);
			else if (strcmp(name, "polygon") == 0)
				code = svgtiny_parse_poly(child, state, true);
            else if (strcmp(name, "text") == 0)
				code = svgtiny_parse_text(child, state);
            
            // not sure about this
        } else if (child->nodeType() == Poco::XML::Element::TEXT_NODE) {
            
            const char *name = (const char *) child->localName().c_str();
            
			if (strcmp(name, "text") == 0)
				code = svgtiny_parse_text(child, state);
		}
        
        //pNode = it.nextNode();
        i++;

		if (code != svgtiny_OK)
			return code;
	}
	return svgtiny_OK;
}



/**
 * Parse a <path> element node.
 *
 * http://www.w3.org/TR/SVG11/paths#PathElement
 */

svgtiny_code svgtiny_parse_path(Poco::XML::Element *path,
		struct svgtiny_parse_state state)
{
	char *s, *path_d;
	float *p;
	unsigned int i;
	float last_x = 0, last_y = 0;
	float last_cubic_x = 0, last_cubic_y = 0;
	float last_quad_x = 0, last_quad_y = 0;

	svgtiny_parse_paint_attributes(path, &state);
	svgtiny_parse_transform_attributes(path, &state);

	/* read d attribute */
	//s = path_d = (char *) xmlGetProp(path, (const xmlChar *) "d");
    //s = path_d = (char *) path->Attribute("d");
    s = path_d = (char *) path->getAttribute("d").c_str();
	if (!s) {
		//state.diagram->error_line = path->line;
		state.diagram->error_message = "path: missing d attribute";
		return svgtiny_SVG_ERROR;
	}

	/* allocate space for path: it will never have more elements than d */
	p = (float*) malloc(sizeof p[0] * strlen(s));
	if (!p)
		return svgtiny_OUT_OF_MEMORY;

	/* parse d and build path */
	for (i = 0; s[i]; i++)
		if (s[i] == ',')
			s[i] = ' ';
	i = 0;
	while (*s) {
		char command[2];
		int plot_command;
		float x, y, x1, y1, x2, y2, rx, ry, rotation, large_arc, sweep;
		int n;

		/* moveto (M, m), lineto (L, l) (2 arguments) */
		if (sscanf(s, " %1[MmLl] %f %f %n", command, &x, &y, &n) == 3) {
			/*LOG(("moveto or lineto"));*/
			if (*command == 'M' || *command == 'm')
				plot_command = svgtiny_PATH_MOVE;
			else
				plot_command = svgtiny_PATH_LINE;
			do {
				p[i++] = plot_command;
				if ('a' <= *command) {
					x += last_x;
					y += last_y;
				}
				p[i++] = last_cubic_x = last_quad_x = last_x
						= x;
				p[i++] = last_cubic_y = last_quad_y = last_y
						= y;
				s += n;
				plot_command = svgtiny_PATH_LINE;
			} while (sscanf(s, "%f %f %n", &x, &y, &n) == 2);

		/* closepath (Z, z) (no arguments) */
		} else if (sscanf(s, " %1[Zz] %n", command, &n) == 1) {
			/*LOG(("closepath"));*/
			p[i++] = svgtiny_PATH_CLOSE;
			s += n;

		/* horizontal lineto (H, h) (1 argument) */
		} else if (sscanf(s, " %1[Hh] %f %n", command, &x, &n) == 2) {
			/*LOG(("horizontal lineto"));*/
			do {
				p[i++] = svgtiny_PATH_LINE;
				if (*command == 'h')
					x += last_x;
				p[i++] = last_cubic_x = last_quad_x = last_x
						= x;
				p[i++] = last_cubic_y = last_quad_y = last_y;
				s += n;
			} while (sscanf(s, "%f %n", &x, &n) == 1);

		/* vertical lineto (V, v) (1 argument) */
		} else if (sscanf(s, " %1[Vv] %f %n", command, &y, &n) == 2) {
			/*LOG(("vertical lineto"));*/
			do {
				p[i++] = svgtiny_PATH_LINE;
				if (*command == 'v')
					y += last_y;
				p[i++] = last_cubic_x = last_quad_x = last_x;
				p[i++] = last_cubic_y = last_quad_y = last_y
						= y;
				s += n;
			} while (sscanf(s, "%f %n", &x, &n) == 1);

		/* curveto (C, c) (6 arguments) */
		} else if (sscanf(s, " %1[Cc] %f %f %f %f %f %f %n", command,
				&x1, &y1, &x2, &y2, &x, &y, &n) == 7) {
			/*LOG(("curveto"));*/
			do {
				p[i++] = svgtiny_PATH_BEZIER;
				if (*command == 'c') {
					x1 += last_x;
					y1 += last_y;
					x2 += last_x;
					y2 += last_y;
					x += last_x;
					y += last_y;
				}
				p[i++] = x1;
				p[i++] = y1;
				p[i++] = last_cubic_x = x2;
				p[i++] = last_cubic_y = y2;
				p[i++] = last_quad_x = last_x = x;
				p[i++] = last_quad_y = last_y = y;
				s += n;
			} while (sscanf(s, "%f %f %f %f %f %f %n",
					&x1, &y1, &x2, &y2, &x, &y, &n) == 6);

		/* shorthand/smooth curveto (S, s) (4 arguments) */
		} else if (sscanf(s, " %1[Ss] %f %f %f %f %n", command,
				&x2, &y2, &x, &y, &n) == 5) {
			/*LOG(("shorthand/smooth curveto"));*/
			do {
				p[i++] = svgtiny_PATH_BEZIER;
				x1 = last_x + (last_x - last_cubic_x);
				y1 = last_y + (last_y - last_cubic_y);
				if (*command == 's') {
					x2 += last_x;
					y2 += last_y;
					x += last_x;
					y += last_y;
				}
				p[i++] = x1;
				p[i++] = y1;
				p[i++] = last_cubic_x = x2;
				p[i++] = last_cubic_y = y2;
				p[i++] = last_quad_x = last_x = x;
				p[i++] = last_quad_y = last_y = y;
				s += n;
			} while (sscanf(s, "%f %f %f %f %n",
					&x2, &y2, &x, &y, &n) == 4);

		/* quadratic Bezier curveto (Q, q) (4 arguments) */
		} else if (sscanf(s, " %1[Qq] %f %f %f %f %n", command,
				&x1, &y1, &x, &y, &n) == 5) {
			/*LOG(("quadratic Bezier curveto"));*/
			do {
				p[i++] = svgtiny_PATH_BEZIER;
				last_quad_x = x1;
				last_quad_y = y1;
				if (*command == 'q') {
					x1 += last_x;
					y1 += last_y;
					x += last_x;
					y += last_y;
				}
				p[i++] = 1./3 * last_x + 2./3 * x1;
				p[i++] = 1./3 * last_y + 2./3 * y1;
				p[i++] = 2./3 * x1 + 1./3 * x;
				p[i++] = 2./3 * y1 + 1./3 * y;
				p[i++] = last_cubic_x = last_x = x;
				p[i++] = last_cubic_y = last_y = y;
				s += n;
			} while (sscanf(s, "%f %f %f %f %n",
					&x1, &y1, &x, &y, &n) == 4);

		/* shorthand/smooth quadratic Bezier curveto (T, t)
		   (2 arguments) */
		} else if (sscanf(s, " %1[Tt] %f %f %n", command,
				&x, &y, &n) == 3) {
			/*LOG(("shorthand/smooth quadratic Bezier curveto"));*/
			do {
				p[i++] = svgtiny_PATH_BEZIER;
				x1 = last_x + (last_x - last_quad_x);
				y1 = last_y + (last_y - last_quad_y);
				last_quad_x = x1;
				last_quad_y = y1;
				if (*command == 't') {
					x1 += last_x;
					y1 += last_y;
					x += last_x;
					y += last_y;
				}
				p[i++] = 1./3 * last_x + 2./3 * x1;
				p[i++] = 1./3 * last_y + 2./3 * y1;
				p[i++] = 2./3 * x1 + 1./3 * x;
				p[i++] = 2./3 * y1 + 1./3 * y;
				p[i++] = last_cubic_x = last_x = x;
				p[i++] = last_cubic_y = last_y = y;
				s += n;
			} while (sscanf(s, "%f %f %n",
					&x, &y, &n) == 2);

		/* elliptical arc (A, a) (7 arguments) */
		} else if (sscanf(s, " %1[Aa] %f %f %f %f %f %f %f %n", command,
				&rx, &ry, &rotation, &large_arc, &sweep,
				&x, &y, &n) == 8) {
			do {
				p[i++] = svgtiny_PATH_LINE;
				if (*command == 'a') {
					x += last_x;
					y += last_y;
				}
				p[i++] = last_cubic_x = last_quad_x = last_x
						= x;
				p[i++] = last_cubic_y = last_quad_y = last_y
						= y;
				s += n;
			} while (sscanf(s, "%f %f %f %f %f %f %f %n",
				&rx, &ry, &rotation, &large_arc, &sweep,
				&x, &y, &n) == 7);

		} else {
			fprintf(stderr, "parse failed at \"%s\"\n", s);
			break;
		}
	}

	//xmlFree(path_d);
    if(path_d) {
        //delete path_d;
    }

	if (i <= 4) {
		/* no real segments in path */
		free(p);
		return svgtiny_OK;
	}

	return svgtiny_add_path(p, i, &state);
}


/**
 * Parse a <rect> element node.
 *
 * http://www.w3.org/TR/SVG11/shapes#RectElement
 */

svgtiny_code svgtiny_parse_rect(Poco::XML::Element *rect,
		struct svgtiny_parse_state state)
{
	float x, y, width, height;
	float *p;

	svgtiny_parse_position_attributes(rect, state,
			&x, &y, &width, &height);
	svgtiny_parse_paint_attributes(rect, &state);
	svgtiny_parse_transform_attributes(rect, &state);

	p =(float *) malloc(13 * sizeof p[0]);
	if (!p)
		return svgtiny_OUT_OF_MEMORY;

	p[0] = svgtiny_PATH_MOVE;
	p[1] = x;
	p[2] = y;
	p[3] = svgtiny_PATH_LINE;
	p[4] = x + width;
	p[5] = y;
	p[6] = svgtiny_PATH_LINE;
	p[7] = x + width;
	p[8] = y + height;
	p[9] = svgtiny_PATH_LINE;
	p[10] = x;
	p[11] = y + height;
	p[12] = svgtiny_PATH_CLOSE;

	return svgtiny_add_path(p, 13, &state);
}


/**
 * Parse a <circle> element node.
 */

svgtiny_code svgtiny_parse_circle(Poco::XML::Element *circle,
		struct svgtiny_parse_state state)
{
	float x = 0, y = 0, r = -1;
	float *p;

	//for (attr = circle->properties; attr; attr = attr->next) {
    //for( attr = circle->FirstAttribute(); attr; attr = attr->Next() ) {
    
    Poco::XML::NamedNodeMap *map = circle->attributes();
    for( int i = 0; i < map->length(); i++ ) {
    
		//const char *name = (const char *) attr->name;
		//const char *content = (const char *) attr->children->content;
        
        //const char *name = (const char *) attr->Name();
		//const char *content = (const char *) attr->Value();
        
        const char *name = (const char *) map->item(i)->localName().c_str();
		const char *content = (const char *) map->item(i)->getNodeValue().c_str();
        
		if (strcmp(name, "cx") == 0)
			x = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "cy") == 0)
			y = svgtiny_parse_length(content,
					state.viewport_height, state);
		else if (strcmp(name, "r") == 0)
			r = svgtiny_parse_length(content,
					state.viewport_width, state);
        }
	svgtiny_parse_paint_attributes(circle, &state);
	svgtiny_parse_transform_attributes(circle, &state);

	if (r < 0) {
        // how to figure?
		//state.diagram->error_line = circle->line;
        
		state.diagram->error_message = "circle: r missing or negative";
		return svgtiny_SVG_ERROR;
	}
	if (r == 0)
		return svgtiny_OK;

	p = (float *) malloc(32 * sizeof p[0]);
	if (!p)
		return svgtiny_OUT_OF_MEMORY;

	p[0] = svgtiny_PATH_MOVE;
	p[1] = x + r;
	p[2] = y;
	p[3] = svgtiny_PATH_BEZIER;
	p[4] = x + r;
	p[5] = y + r * KAPPA;
	p[6] = x + r * KAPPA;
	p[7] = y + r;
	p[8] = x;
	p[9] = y + r;
	p[10] = svgtiny_PATH_BEZIER;
	p[11] = x - r * KAPPA;
	p[12] = y + r;
	p[13] = x - r;
	p[14] = y + r * KAPPA;
	p[15] = x - r;
	p[16] = y;
	p[17] = svgtiny_PATH_BEZIER;
	p[18] = x - r;
	p[19] = y - r * KAPPA;
	p[20] = x - r * KAPPA;
	p[21] = y - r;
	p[22] = x;
	p[23] = y - r;
	p[24] = svgtiny_PATH_BEZIER;
	p[25] = x + r * KAPPA;
	p[26] = y - r;
	p[27] = x + r;
	p[28] = y - r * KAPPA;
	p[29] = x + r;
	p[30] = y;
	p[31] = svgtiny_PATH_CLOSE;
	
	return svgtiny_add_path(p, 32, &state);
}


/**
 * Parse an <ellipse> element node.
 */

svgtiny_code svgtiny_parse_ellipse(Poco::XML::Element *ellipse,
		struct svgtiny_parse_state state)
{
	float x = 0, y = 0, rx = -1, ry = -1;
	float *p;

	//for (attr = ellipse->properties; attr; attr = attr->next) {
    //for( attr = ellipse->FirstAttribute(); attr; attr = attr->Next() ) {
    
    Poco::XML::NamedNodeMap *map = ellipse->attributes();
    for( int i = 0; i < map->length(); i++ ) {
    
		//const char *name = (const char *) attr->name;
		//const char *content = (const char *) attr->children->content;
        
        //const char *name = (const char *) attr->Name();
		//const char *content = (const char *) attr->Value();
        
        const char *name = (const char *) map->item(i)->localName().c_str();
		const char *content = (const char *) map->item(i)->getNodeValue().c_str();
        
		if (strcmp(name, "cx") == 0)
			x = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "cy") == 0)
			y = svgtiny_parse_length(content,
					state.viewport_height, state);
		else if (strcmp(name, "rx") == 0)
			rx = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "ry") == 0)
			ry = svgtiny_parse_length(content,
					state.viewport_width, state);
        }
	svgtiny_parse_paint_attributes(ellipse, &state);
	svgtiny_parse_transform_attributes(ellipse, &state);

	if (rx < 0 || ry < 0) {
		//state.diagram->error_line = ellipse->line;
		state.diagram->error_message = "ellipse: rx or ry missing "
				"or negative";
		return svgtiny_SVG_ERROR;
	}
	if (rx == 0 || ry == 0)
		return svgtiny_OK;

	p = (float*) malloc(32 * sizeof p[0]);
	if (!p)
		return svgtiny_OUT_OF_MEMORY;

	p[0] = svgtiny_PATH_MOVE;
	p[1] = x + rx;
	p[2] = y;
	p[3] = svgtiny_PATH_BEZIER;
	p[4] = x + rx;
	p[5] = y + ry * KAPPA;
	p[6] = x + rx * KAPPA;
	p[7] = y + ry;
	p[8] = x;
	p[9] = y + ry;
	p[10] = svgtiny_PATH_BEZIER;
	p[11] = x - rx * KAPPA;
	p[12] = y + ry;
	p[13] = x - rx;
	p[14] = y + ry * KAPPA;
	p[15] = x - rx;
	p[16] = y;
	p[17] = svgtiny_PATH_BEZIER;
	p[18] = x - rx;
	p[19] = y - ry * KAPPA;
	p[20] = x - rx * KAPPA;
	p[21] = y - ry;
	p[22] = x;
	p[23] = y - ry;
	p[24] = svgtiny_PATH_BEZIER;
	p[25] = x + rx * KAPPA;
	p[26] = y - ry;
	p[27] = x + rx;
	p[28] = y - ry * KAPPA;
	p[29] = x + rx;
	p[30] = y;
	p[31] = svgtiny_PATH_CLOSE;
	
	return svgtiny_add_path(p, 32, &state);
}


/**
 * Parse a <line> element node.
 */

svgtiny_code svgtiny_parse_line(Poco::XML::Element *line,
		struct svgtiny_parse_state state)
{
	float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	float *p;

	//for (attr = line->properties; attr; attr = attr->next) {
    //for( attr = line->FirstAttribute(); attr; attr = attr->Next() ) {
    
    Poco::XML::NamedNodeMap *map = line->attributes();
    for( int i = 0; i < map->length(); i++ ) {
    
		//const char *name = (const char *) attr->name;
		//const char *content = (const char *) attr->children->content;
        
        //const char *name = (const char *) attr->Name();
		//const char *content = (const char *) attr->Value();
        
        const char *name = (const char *) map->item(i)->localName().c_str();
		const char *content = (const char *) map->item(i)->getNodeValue().c_str();
        
		if (strcmp(name, "x1") == 0)
			x1 = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "y1") == 0)
			y1 = svgtiny_parse_length(content,
					state.viewport_height, state);
		else if (strcmp(name, "x2") == 0)
			x2 = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "y2") == 0)
			y2 = svgtiny_parse_length(content,
					state.viewport_height, state);
        }
	svgtiny_parse_paint_attributes(line, &state);
	svgtiny_parse_transform_attributes(line, &state);

	p = (float*) malloc(7 * sizeof p[0]);
	if (!p)
		return svgtiny_OUT_OF_MEMORY;

	p[0] = svgtiny_PATH_MOVE;
	p[1] = x1;
	p[2] = y1;
	p[3] = svgtiny_PATH_LINE;
	p[4] = x2;
	p[5] = y2;
	p[6] = svgtiny_PATH_CLOSE;

	return svgtiny_add_path(p, 7, &state);
}


/**
 * Parse a <polyline> or <polygon> element node.
 *
 * http://www.w3.org/TR/SVG11/shapes#PolylineElement
 * http://www.w3.org/TR/SVG11/shapes#PolygonElement
 */

svgtiny_code svgtiny_parse_poly(Poco::XML::Element *poly,
		struct svgtiny_parse_state state, bool polygon)
{
	char *s, *points;
	float *p;
	unsigned int i;

	svgtiny_parse_paint_attributes(poly, &state);
	svgtiny_parse_transform_attributes(poly, &state);

	/* read points attribute */
	//s = points = (char *) xmlGetProp(poly, (const xmlChar *) "points");
    
    s = points = (char *) poly->getAttribute("points").c_str();
    
	if (!s) {
		//state.diagram->error_line = poly->line;
		state.diagram->error_message =
				"polyline/polygon: missing points attribute";
		return svgtiny_SVG_ERROR;
	}

	/* allocate space for path: it will never have more elements than s */
	p = (float*) malloc(sizeof p[0] * strlen(s));
	if (!p) {
        //xmlFree(points);
		return svgtiny_OUT_OF_MEMORY;
	}

	/* parse s and build path */
	for (i = 0; s[i]; i++)
		if (s[i] == ',')
			s[i] = ' ';
	i = 0;
	while (*s) {
		float x, y;
		int n;

		if (sscanf(s, "%f %f %n", &x, &y, &n) == 2) {
			if (i == 0)
				p[i++] = svgtiny_PATH_MOVE;
			else
				p[i++] = svgtiny_PATH_LINE;
			p[i++] = x;
			p[i++] = y;
			s += n;
                } else {
                	break;
                }
        }
        if (polygon)
		p[i++] = svgtiny_PATH_CLOSE;

	//xmlFree(points);

	return svgtiny_add_path(p, i, &state);
}


/**
 * Parse a <text> or <tspan> element node.
 */

svgtiny_code svgtiny_parse_text(Poco::XML::Element *text,
		struct svgtiny_parse_state state)
{
	float x, y, width, height;
	float px, py;
	Poco::XML::Element *child;

	svgtiny_parse_position_attributes(text, state,
			&x, &y, &width, &height);
	svgtiny_parse_font_attributes(text, &state);
	svgtiny_parse_transform_attributes(text, &state);

	px = state.ctm.a * x + state.ctm.c * y + state.ctm.e;
	py = state.ctm.b * x + state.ctm.d * y + state.ctm.f;
    
/* 	state.ctm.e = px - state.origin_x; */
/* 	state.ctm.f = py - state.origin_y; */

	/*struct css_style style = state.style;
	style.font_size.value.length.value *= state.ctm.a;*/

	//for (child = text->children; child; child = child->next) {
    //for( child = (Poco::XML::Element*) text->FirstChild( false ); child; child = (Poco::XML::Element*) child->NextSibling( false ) ) {
    
    
    Poco::XML::NodeIterator it(text, Poco::XML::NodeFilter::SHOW_ELEMENT | Poco::XML::NodeFilter::SHOW_TEXT);
    Poco::XML::Node* pNode = it.nextNode();
    while (pNode) {
    
		svgtiny_code code = svgtiny_OK;

		if (pNode->getNodeValue().compare("text") == 0) 
        {
			struct svgtiny_shape *shape = svgtiny_add_shape(&state);
			
            if (!shape)
				return svgtiny_OUT_OF_MEMORY;
            
			//shape->text = strdup((const char *) child->content);
            
            shape->text = strdup((const char *) pNode->getNodeValue().c_str());
            
			shape->text_x = px;
			shape->text_y = py;
			state.diagram->shape_count++;

		} 
        //else if (strcmp((const char *) child->Value(), "tspan") == 0) 
        else if (pNode->getNodeValue().compare("tspan") == 0) 
        {
			code = svgtiny_parse_text(child, state);
		}
    
        pNode = it.nextNode();

	if ((!code) != svgtiny_OK)
			return code;
	}

	return svgtiny_OK;
}


/**
 * Parse x, y, width, and height attributes, if present.
 */

void svgtiny_parse_position_attributes(const Poco::XML::Element *node,
		const struct svgtiny_parse_state state,
		float *x, float *y, float *width, float *height)
{
	*x = 0;
	*y = 0;
	*width = state.viewport_width;
	*height = state.viewport_height;

	//for (attr = node->properties; attr; attr = attr->next) {
    //for( attr = node->FirstAttribute(); attr; attr = attr->Next() ) {
    Poco::XML::NamedNodeMap *map = node->attributes();
    //for( attr = node->FirstAttribute(); attr; attr = attr->Next() ) {
    for( int i = 0; i < map->length(); i++ ) {
    
    
		//const char *name = (const char *) attr->name;
		//const char *content = (const char *) attr->children->content;
        
        // good god this is ugly
        const char *name = (const char *) map->item(i)->localName().c_str();
		const char *content = (const char *) map->item(i)->getNodeValue().c_str();
        
		if (strcmp(name, "x") == 0)
			*x = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "y") == 0)
			*y = svgtiny_parse_length(content,
					state.viewport_height, state);
		else if (strcmp(name, "width") == 0)
			*width = svgtiny_parse_length(content,
					state.viewport_width, state);
		else if (strcmp(name, "height") == 0)
			*height = svgtiny_parse_length(content,
					state.viewport_height, state);
	}
}


/**
 * Parse a length as a number of pixels.
 */

float svgtiny_parse_length(const char *s, int viewport_size,
		const struct svgtiny_parse_state state)
{
	int num_length = strspn(s, "0123456789+-.");
	const char *unit = s + num_length;
	float n = atof((const char *) s);
	float font_size = 20; /*css_len2px(&state.style.font_size.value.length, 0);*/

	UNUSED(state);

	if (unit[0] == 0) {
		return n;
	} else if (unit[0] == '%') {
		return n / 100.0 * viewport_size;
	} else if (unit[0] == 'e' && unit[1] == 'm') {
		return n * font_size;
	} else if (unit[0] == 'e' && unit[1] == 'x') {
		return n / 2.0 * font_size;
	} else if (unit[0] == 'p' && unit[1] == 'x') {
		return n;
	} else if (unit[0] == 'p' && unit[1] == 't') {
		return n * 1.25;
	} else if (unit[0] == 'p' && unit[1] == 'c') {
		return n * 15.0;
	} else if (unit[0] == 'm' && unit[1] == 'm') {
		return n * 3.543307;
	} else if (unit[0] == 'c' && unit[1] == 'm') {
		return n * 35.43307;
	} else if (unit[0] == 'i' && unit[1] == 'n') {
		return n * 90;
	}

	return 0;
}


/**
 * Parse paint attributes, if present.
 */

void svgtiny_parse_paint_attributes(const Poco::XML::Element *node,
		struct svgtiny_parse_state *state)
{
	//const xmlAttr *attr;
    const Poco::XML::Attr *attr;

	//for (attr = node->properties; attr; attr = attr->next) {
    
    //for( attr = node->FirstAttribute(); attr; attr = attr->Next() ) {
    Poco::XML::NamedNodeMap *map = node->attributes();
    for( int i = 0; i < map->length(); i++ ) {
    
		//const char *name = (const char *) attr->name;
		//const char *content = (const char *) attr->children->content;
        
        //const char *name = (const char *) attr->Name();
		//const char *content = (const char *) attr->Value();
        attr = (Poco::XML::Attr*) map->item(i);
        const char *name = (const char *) attr->localName().c_str();
		const char *content = (const char *) attr->getNodeValue().c_str();
        
		if (strcmp(name, "fill") == 0)
			svgtiny_parse_color(content, &state->fill, state);
		else if (strcmp(name, "stroke") == 0)
			svgtiny_parse_color(content, &state->stroke, state);
		else if (strcmp(name, "stroke-width") == 0)
			state->stroke_width = svgtiny_parse_length(content,
					state->viewport_width, *state);
		else if (strcmp(name, "style") == 0) {
			//const char *style = (const char *) attr->children->content;
            
            const char *style = attr->getValue().c_str();
            
			const char *s;
			char *value;
			if ((s = strstr(style, "fill:"))) {
				s += 5;
				while (*s == ' ')
					s++;
				value = strndup(s, strcspn(s, "; "));
				svgtiny_parse_color(value, &state->fill, state);
				free(value);
			}
			if ((s = strstr(style, "stroke:"))) {
				s += 7;
				while (*s == ' ')
					s++;
				value = strndup(s, strcspn(s, "; "));
				svgtiny_parse_color(value, &state->stroke, state);
				free(value);
			}
			if ((s = strstr(style, "stroke-width:"))) {
				s += 13;
				while (*s == ' ')
					s++;
				value = strndup(s, strcspn(s, "; "));
				state->stroke_width = svgtiny_parse_length(value,
						state->viewport_width, *state);
				free(value);
			}
		}
	}
}


/**
 * Parse a colour.
 */

void svgtiny_parse_color(const char *s, svgtiny_colour *c,
		struct svgtiny_parse_state *state)
{
	unsigned int r, g, b;
	float rf, gf, bf;
	size_t len = strlen(s);
	char *id = 0, *rparen;

	if (len == 4 && s[0] == '#') {
		if (sscanf(s + 1, "%1x%1x%1x", &r, &g, &b) == 3)
			*c = svgtiny_RGB(r | r << 4, g | g << 4, b | b << 4);

	} else if (len == 7 && s[0] == '#') {
		if (sscanf(s + 1, "%2x%2x%2x", &r, &g, &b) == 3)
			*c = svgtiny_RGB(r, g, b);

	} else if (10 <= len && s[0] == 'r' && s[1] == 'g' && s[2] == 'b' &&
			s[3] == '(' && s[len - 1] == ')') {
		if (sscanf(s + 4, "%u,%u,%u", &r, &g, &b) == 3)
			*c = svgtiny_RGB(r, g, b);
		else if (sscanf(s + 4, "%f%%,%f%%,%f%%", &rf, &gf, &bf) == 3) {
			b = bf * 255 / 100;
			g = gf * 255 / 100;
			r = rf * 255 / 100;
			*c = svgtiny_RGB(r, g, b);
		}

	} else if (len == 4 && strcmp(s, "none") == 0) {
		*c = svgtiny_TRANSPARENT;

	} else if (5 < len && s[0] == 'u' && s[1] == 'r' && s[2] == 'l' &&
			s[3] == '(') {
		if (s[4] == '#') {
			id = strdup(s + 5);
			if (!id)
				return;
			rparen = strchr(id, ')');
			if (rparen)
				*rparen = 0;
			svgtiny_find_gradient(id, state);
			free(id);
			fprintf(stderr, "linear_gradient_stop_count %i\n",
					state->linear_gradient_stop_count);
			if (state->linear_gradient_stop_count == 0)
				*c = svgtiny_TRANSPARENT;
			else if (state->linear_gradient_stop_count == 1)
				*c = state->gradient_stop[0].color;
			else
				*c = svgtiny_LINEAR_GRADIENT;
		}

	} else {
		const struct svgtiny_named_color *named_color;
		named_color = svgtiny_color_lookup(s, strlen(s));
		if (named_color)
			*c = named_color->color;
	}
}


/**
 * Parse font attributes, if present.
 */

void svgtiny_parse_font_attributes(const Poco::XML::Element *node,
		struct svgtiny_parse_state *state)
{
	UNUSED(state);

	//for (attr = node->properties; attr; attr = attr->next) {
        
		/*if (strcmp((const char *) attr->Name(), "font-size") == 0) {
			if (css_parse_length(
					(const char *) attr->children->content,
					&state->style.font_size.value.length,
					true, true)) {
				state->style.font_size.size =
						CSS_FONT_SIZE_LENGTH;
			}*/
    
        //if (node->Attribute("font-size")) {
        //    if (css_parse_length( (const char *) node->Value(), &state->style.font_size.value.length, true, true)) 
        //    {
        //        state->style.font_size.size = CSS_FONT_SIZE_LENGTH;
        //    }
        //}
    
    
		//}
    //}
}


/**
 * Parse transform attributes, if present.
 *
 * http://www.w3.org/TR/SVG11/coords#TransformAttribute
 */

void svgtiny_parse_transform_attributes(Poco::XML::Element *node,
		struct svgtiny_parse_state *state)
{
	char *transform;

	/* parse transform */
	//transform = (char *) xmlGetProp(node, (const xmlChar *) "transform");
    
    transform = (char *) node->getAttribute("transform").c_str();
    
	if (transform) {
		svgtiny_parse_transform(transform, &state->ctm.a, &state->ctm.b,
				&state->ctm.c, &state->ctm.d,
				&state->ctm.e, &state->ctm.f);
		//xmlFree(transform);
	}
}


/**
 * Parse a transform string.
 */

void svgtiny_parse_transform(char *s, float *ma, float *mb,
		float *mc, float *md, float *me, float *mf)
{
	float a, b, c, d, e, f;
	float za, zb, zc, zd, ze, zf;
	float angle, x, y;
	int n;
	unsigned int i;

	for (i = 0; s[i]; i++)
		if (s[i] == ',')
			s[i] = ' ';

	while (*s) {
		a = d = 1;
		b = c = 0;
		e = f = 0;
		if (sscanf(s, "matrix (%f %f %f %f %f %f) %n",
					&a, &b, &c, &d, &e, &f, &n) == 6)
			;
		else if (sscanf(s, "translate (%f %f) %n",
					&e, &f, &n) == 2)
			;
		else if (sscanf(s, "translate (%f) %n",
					&e, &n) == 1)
			;
		else if (sscanf(s, "scale (%f %f) %n",
					&a, &d, &n) == 2)
			;
		else if (sscanf(s, "scale (%f) %n",
					&a, &n) == 1)
			d = a;
		else if (sscanf(s, "rotate (%f %f %f) %n",
					&angle, &x, &y, &n) == 3) {
			angle = angle / 180 * M_PI;
			a = cos(angle);
			b = sin(angle);
			c = -sin(angle);
			d = cos(angle);
			e = -x * cos(angle) + y * sin(angle) + x;
			f = -x * sin(angle) - y * cos(angle) + y;
		} else if (sscanf(s, "rotate (%f) %n",
					&angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			a = cos(angle);
			b = sin(angle);
			c = -sin(angle);
			d = cos(angle);
		} else if (sscanf(s, "skewX (%f) %n",
					&angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			c = tan(angle);
		} else if (sscanf(s, "skewY (%f) %n",
					&angle, &n) == 1) {
			angle = angle / 180 * M_PI;
			b = tan(angle);
		} else
			break;
		za = *ma * a + *mc * b;
		zb = *mb * a + *md * b;
		zc = *ma * c + *mc * d;
		zd = *mb * c + *md * d;
		ze = *ma * e + *mc * f + *me;
		zf = *mb * e + *md * f + *mf;
		*ma = za;
		*mb = zb;
		*mc = zc;
		*md = zd;
		*me = ze;
		*mf = zf;
		s += n;
	}
}


/**
 * Add a path to the svgtiny_diagram.
 */

svgtiny_code svgtiny_add_path(float *p, unsigned int n,
		struct svgtiny_parse_state *state)
{
	struct svgtiny_shape *shape;

	if (state->fill == svgtiny_LINEAR_GRADIENT)
		return svgtiny_add_path_linear_gradient(p, n, state);

	svgtiny_transform_path(p, n, state);

	shape = svgtiny_add_shape(state);
	if (!shape) {
		free(p);
		return svgtiny_OUT_OF_MEMORY;
	}
	shape->path = p;
	shape->path_length = n;
	state->diagram->shape_count++;

	return svgtiny_OK;
}


/**
 * Add a svgtiny_shape to the svgtiny_diagram.
 */

struct svgtiny_shape *svgtiny_add_shape(struct svgtiny_parse_state *state)
{
	struct svgtiny_shape *shape = (svgtiny_shape *) realloc(state->diagram->shape,
			(state->diagram->shape_count + 1) * sizeof (state->diagram->shape[0]));
	if (!shape)
		return 0;
	state->diagram->shape = shape;

	shape += state->diagram->shape_count;
	shape->path = 0;
	shape->path_length = 0;
	shape->text = 0;
	shape->fill = state->fill;
	shape->stroke = state->stroke;
	shape->stroke_width = lroundf((float) state->stroke_width *
			(state->ctm.a + state->ctm.d) / 2.0);
	if (0 < state->stroke_width && shape->stroke_width == 0)
		shape->stroke_width = 1;

	return shape;
}


/**
 * Apply the current transformation matrix to a path.
 */

void svgtiny_transform_path(float *p, unsigned int n,
		struct svgtiny_parse_state *state)
{
	unsigned int j;

	for (j = 0; j != n; ) {
		unsigned int points = 0;
		unsigned int k;
		switch ((int) p[j]) {
		case svgtiny_PATH_MOVE:
		case svgtiny_PATH_LINE:
			points = 1;
			break;
		case svgtiny_PATH_CLOSE:
			points = 0;
			break;
		case svgtiny_PATH_BEZIER:
			points = 3;
			break;
		default:
			assert(0);
		}
		j++;
		for (k = 0; k != points; k++) {
			float x0 = p[j], y0 = p[j + 1];
			float x = state->ctm.a * x0 + state->ctm.c * y0 +
				state->ctm.e;
			float y = state->ctm.b * x0 + state->ctm.d * y0 +
				state->ctm.f;
			p[j] = x;
			p[j + 1] = y;
			j += 2;
		}
	}
}


/**
 * Free all memory used by a diagram.
 */

void svgtiny_free(struct svgtiny_diagram *svg)
{
	unsigned int i;
	assert(svg);

	for (i = 0; i != svg->shape_count; i++) {
		free(svg->shape[i].path);
		free(svg->shape[i].text);
	}
	
	free(svg->shape);

	free(svg);
}

#ifndef HAVE_STRNDUP
char *svgtiny_strndup(const char *s, size_t n)
{
	size_t len;
	char *s2;

	for (len = 0; len != n && s[len]; len++)
		continue;

	s2 = (char*) malloc(len + 1);
	if (s2 == NULL)
		return NULL;

	memcpy(s2, s, len);
	s2[len] = '\0';

	return s2;
}
#endif

