/*
 * This file is part of Libsvgtiny
 * Licensed under the MIT License,
 *                http://opensource.org/licenses/mit-license.php
 * Copyright 2008 James Bursa <james@semichrome.net>
 */

#include <assert.h>
#include <math.h>
#include <string.h>
#include "svgtiny.h"
#include "svgtiny_internal.h"
#include <stdio.h>

#undef GRADIENT_DEBUG

static svgtiny_code svgtiny_parse_linear_gradient(Poco::XML::Element *linear,
		struct svgtiny_parse_state *state);
static float svgtiny_parse_gradient_offset(const char *s);
static void svgtiny_path_bbox(float *p, unsigned int n,
		float *x0, float *y0, float *x1, float *y1);
static void svgtiny_invert_matrix(float *m, float *inv);


/**
 * Find a gradient by id and parse it.
 */

void svgtiny_find_gradient(const char *id, struct svgtiny_parse_state *state)
{
	Poco::XML::Element *gradient;

	fprintf(stderr, "svgtiny_find_gradient: id \"%s\"\n", id);

	state->linear_gradient_stop_count = 0;
	state->gradient_x1 = "0%";
	state->gradient_y1 = "0%";
	state->gradient_x2 = "100%";
	state->gradient_y2 = "0%";
	state->gradient_user_space_on_use = false;
	state->gradient_transform.a = 1;
	state->gradient_transform.b = 0;
	state->gradient_transform.c = 0;
	state->gradient_transform.d = 1;
	state->gradient_transform.e = 0;
	state->gradient_transform.f = 0;

    Poco::XML::Document *d = state->document;
    
	//gradient = svgtiny_find_element_by_id( d->documentElement(), id);
    
    std::string s(id);
    //gradient = d->getElementById(s);
    gradient = d->getElementById( s, "id");
    
	//fprintf(stderr, "gradient %p\n", (void *) gradient);
	if (!gradient) {
		fprintf(stderr, "gradient \"%s\" not found\n", id);
		return;
	}

	//fprintf(stderr, "gradient name \"%s\"\n", gradient->Value());
	//if (strcmp((const char *) gradient->Value(), "linearGradient") == 0) {
    if (gradient->localName().compare("linearGradient") == 0) {
		svgtiny_parse_linear_gradient(gradient, state);
	}
}


/**
 * Parse a <linearGradient> element node.
 *
 * http://www.w3.org/TR/SVG11/pservers#LinearGradients
 */

svgtiny_code svgtiny_parse_linear_gradient(Poco::XML::Element *linear,
		struct svgtiny_parse_state *state)
{
	unsigned int i = 0;
    
	//Poco::XML::Attribute *href = xmlHasProp(linear, (const xmlChar *) "href");
	//if (href && href->children->content[0] == '#')
	//	svgtiny_find_gradient((const char *) href->children->content
	//			+ 1, state);
    
    
    //Poco::XML::Element *href = (Poco::XML::Element *) linear->FirstChild("href");
    Poco::XML::Element *href = linear->getChildElement("href");
    if (href && href->nodeValue()[0] == '#') {
        svgtiny_find_gradient((const char *) &href->nodeValue().c_str()[0], state);
    }
    

	//for (attr = linear->properties; attr; attr = attr->next) {
    
    //for( attr = linear->FirstAttribute(); attr; attr = attr->Next() ) {
    
    
    //Poco::XML::NodeIterator it(linear, Poco::XML::NodeFilter::SHOW_ELEMENT | Poco::XML::NodeFilter::SHOW_TEXT);
    //Poco::XML::Node* pNode = it.nextNode();
    
    Poco::XML::NamedNodeMap *map = linear->attributes();
    Poco::XML::Node *node;
    i = 0;
    while (i < map->length())
    {
    
		//const char *name = (const char *) attr->name;
		//const char *content = (const char *) attr->children->content;
        
        //const char *name = (const char *) attr->Name();
		//const char *content = (const char *) attr->Value();
        
        node = map->item(i);
        
		/*if (strcmp(name, "x1") == 0)
			state->gradient_x1 = content;
		else if (strcmp(name, "y1") == 0)
			state->gradient_y1 = content;
		else if (strcmp(name, "x2") == 0)
			state->gradient_x2 = content;
		else if (strcmp(name, "y2") == 0)
			state->gradient_y2 = content;
		else if (strcmp(name, "gradientUnits") == 0)
			state->gradient_user_space_on_use =
					strcmp(content, "userSpaceOnUse") == 0;
		else if (strcmp(name, "gradientTransform") == 0) {
			float a = 1, b = 0, c = 0, d = 1, e = 0, f = 0;
			char *s = strdup(content);
			if (!s)
				return svgtiny_OUT_OF_MEMORY;
			svgtiny_parse_transform(s, &a, &b, &c, &d, &e, &f);
			free(s);
            std::cout << "transform " << a << " " << b << " " << c << " " << d << " " << e << " " << f << std::endl;
			state->gradient_transform.a = a;
			state->gradient_transform.b = b;
			state->gradient_transform.c = c;
			state->gradient_transform.d = d;
			state->gradient_transform.e = e;
			state->gradient_transform.f = f;
		}*/
        
        if (node->localName().compare("x1") == 0)
			state->gradient_x1 = node->getNodeValue().c_str();
		else if (node->localName().compare("y1") == 0)
			state->gradient_y1 = node->getNodeValue().c_str(); //state->gradient_y1 = content;
		else if (node->localName().compare("x2") == 0)
			state->gradient_x2 = node->getNodeValue().c_str();
		else if (node->localName().compare("y2") == 0)
			state->gradient_y2 = node->getNodeValue().c_str();
		else if (node->localName().compare("gradientUnits") == 0)
			state->gradient_user_space_on_use = node->getNodeValue().compare("userSpaceOnUse") == 0;
		else if (node->localName().compare("gradientTransform") == 0) {
			float a = 1, b = 0, c = 0, d = 1, e = 0, f = 0;
			
            // we'll just assume this won't happen (dangerous, I know)
            char *s = strdup(node->getNodeValue().c_str());
            
			//if (!s)
			//	return svgtiny_OUT_OF_MEMORY;
            
			svgtiny_parse_transform(s, &a, &b, &c, &d, &e, &f);
			
            free(s);
            
            
            std::cout << "transform " << a << " " << b << " " << c << " " << d << " " << e << " " << f << std::endl;
			state->gradient_transform.a = a;
			state->gradient_transform.b = b;
			state->gradient_transform.c = c;
			state->gradient_transform.d = d;
			state->gradient_transform.e = e;
			state->gradient_transform.f = f;
		}
        
        i++;
    }

	//for (stop = linear->children; stop; stop = stop->next) {
    //for( stop = (Poco::XML::Element*) linear->FirstChild( false ); stop; stop = (Poco::XML::Element*) stop->NextSibling( false ) ) {
    
    Poco::XML::Element *pNode;
    for( pNode = (Poco::XML::Element*) linear->firstChild(); pNode; pNode = (Poco::XML::Element*) pNode->nextSibling() )
    {
        //pNode = (Poco::XML::Element *) cnl->item(i);
        
        std::cout << pNode->localName() << std::endl;
    
		float offset = -1;
		svgtiny_colour color = svgtiny_TRANSPARENT;

		//if (stop->type != XML_ELEMENT_NODE)
		//	continue;
        
		if (pNode->localName().compare("stop") != 0)
			continue;

		//for (attr = stop->properties; attr;
		//		attr = attr->next) {
        
        //for( attr = stop->FirstAttribute(); attr; attr = attr->Next() ) {
        
        Poco::XML::NamedNodeMap *map = pNode->attributes();
        for( i = 0; i < map->length(); i++ ) {
        
        
			//const char *name = (const char *) attr->name;
			//const char *content =
			//		(const char *) attr->children->content;
            
            //const char *name = (const char *) attr->Name();
            //const char *content = (const char *) attr->Value();
            
            const char *name = (const char *) map->item(i)->localName().c_str();
            const char *content = (const char *) map->item(i)->getNodeValue().c_str();
            
			if (strcmp(name, "offset") == 0)
				offset = svgtiny_parse_gradient_offset(content);
			else if (strcmp(name, "stop-color") == 0)
				svgtiny_parse_color(content, &color, state);
			else if (strcmp(name, "style") == 0) {
				const char *s;
				char *value;
				if ((s = strstr(content, "stop-color:"))) {
					s += 11;
					while (*s == ' ')
						s++;
					value = strndup(s, strcspn(s, "; "));
					svgtiny_parse_color(value, &color,
							state);
					free(value);
				}
			}
		}

		if (offset != -1 && color != svgtiny_TRANSPARENT) {
			fprintf(stderr, "stop %g %x\n", offset, color);
			state->gradient_stop[i].offset = offset;
			state->gradient_stop[i].color = color;
			i++;
		}
        
        //pNode = it.nextNode();

		if (i == svgtiny_MAX_STOPS)
			break;
	}

	if (i)
		state->linear_gradient_stop_count = i;

	return svgtiny_OK;
}


float svgtiny_parse_gradient_offset(const char *s)
{
	int num_length = strspn(s, "0123456789+-.");
	const char *unit = s + num_length;
	float n = atof((const char *) s);

	if (unit[0] == 0)
		;
	else if (unit[0] == '%')
		n /= 100.0;
	else
		return -1;

	if (n < 0)
		n = 0;
	if (1 < n)
		n = 1;
	return n;
}


/**
 * Add a path with a linear gradient fill to the svgtiny_diagram.
 */

svgtiny_code svgtiny_add_path_linear_gradient(float *p, unsigned int n,
		struct svgtiny_parse_state *state)
{
	struct grad_point {
		float x, y, r;
	};
	float object_x0, object_y0, object_x1, object_y1;
	float gradient_x0, gradient_y0, gradient_x1, gradient_y1,
	      gradient_dx, gradient_dy;
	float trans[6];
	unsigned int steps = 10;
	float x0 = 0, y0 = 0, x0_trans, y0_trans, r0; /* segment start point */
	float x1, y1, x1_trans, y1_trans, r1; /* segment end point */
	/* segment control points (beziers only) */
	float c0x = 0, c0y = 0, c1x = 0, c1y = 0;
	float gradient_norm_squared;
	struct svgtiny_list *pts;
	float min_r = 1000;
	unsigned int min_pt = 0;
	unsigned int j;
	unsigned int stop_count;
	unsigned int current_stop;
	float last_stop_r;
	float current_stop_r;
	int red0, green0, blue0, red1, green1, blue1;
	unsigned int t, a, b;

	/* determine object bounding box */
	svgtiny_path_bbox(p, n, &object_x0, &object_y0, &object_x1, &object_y1);
	#ifdef GRADIENT_DEBUG
	fprintf(stderr, "object bbox: (%g %g) (%g %g)\n",
			object_x0, object_y0, object_x1, object_y1);
	#endif

	/* compute gradient vector */
	fprintf(stderr, "x1 %s, y1 %s, x2 %s, y2 %s\n",
			state->gradient_x1, state->gradient_y1,
			state->gradient_x2, state->gradient_y2);
	if (!state->gradient_user_space_on_use) {
		gradient_x0 = object_x0 +
				svgtiny_parse_length(state->gradient_x1,
					object_x1 - object_x0, *state);
		gradient_y0 = object_y0 +
				svgtiny_parse_length(state->gradient_y1,
					object_y1 - object_y0, *state);
		gradient_x1 = object_x0 +
				svgtiny_parse_length(state->gradient_x2,
					object_x1 - object_x0, *state);
		gradient_y1 = object_y0 +
				svgtiny_parse_length(state->gradient_y2,
					object_y1 - object_y0, *state);
	} else {
		gradient_x0 = svgtiny_parse_length(state->gradient_x1,
				state->viewport_width, *state);
		gradient_y0 = svgtiny_parse_length(state->gradient_y1,
				state->viewport_height, *state);
		gradient_x1 = svgtiny_parse_length(state->gradient_x2,
				state->viewport_width, *state);
		gradient_y1 = svgtiny_parse_length(state->gradient_y2,
				state->viewport_height, *state);
	}
	gradient_dx = gradient_x1 - gradient_x0;
	gradient_dy = gradient_y1 - gradient_y0;
	#ifdef GRADIENT_DEBUG
	fprintf(stderr, "gradient vector: (%g %g) => (%g %g)\n",
			gradient_x0, gradient_y0, gradient_x1, gradient_y1);
	#endif

	/* show theoretical gradient strips for debugging */
	/*unsigned int strips = 10;
	for (unsigned int z = 0; z != strips; z++) {
		float f0, fd, strip_x0, strip_y0, strip_dx, strip_dy;
		f0 = (float) z / (float) strips;
		fd = (float) 1 / (float) strips;
		strip_x0 = gradient_x0 + f0 * gradient_dx;
		strip_y0 = gradient_y0 + f0 * gradient_dy;
		strip_dx = fd * gradient_dx;
		strip_dy = fd * gradient_dy;
		fprintf(stderr, "strip %i vector: (%g %g) + (%g %g)\n",
				z, strip_x0, strip_y0, strip_dx, strip_dy);

		float *p = malloc(13 * sizeof p[0]);
		if (!p)
			return svgtiny_OUT_OF_MEMORY;
		p[0] = svgtiny_PATH_MOVE;
		p[1] = strip_x0 + (strip_dy * 3);
		p[2] = strip_y0 - (strip_dx * 3);
		p[3] = svgtiny_PATH_LINE;
		p[4] = p[1] + strip_dx;
		p[5] = p[2] + strip_dy;
		p[6] = svgtiny_PATH_LINE;
		p[7] = p[4] - (strip_dy * 6);
		p[8] = p[5] + (strip_dx * 6);
		p[9] = svgtiny_PATH_LINE;
		p[10] = p[7] - strip_dx;
		p[11] = p[8] - strip_dy;
		p[12] = svgtiny_PATH_CLOSE;
		svgtiny_transform_path(p, 13, state);
		struct svgtiny_shape *shape = svgtiny_add_shape(state);
		if (!shape) {
			free(p);
			return svgtiny_OUT_OF_MEMORY;
		}
		shape->path = p;
		shape->path_length = 13;
		shape->fill = svgtiny_TRANSPARENT;
		shape->stroke = svgtiny_RGB(0, 0xff, 0);
		state->diagram->shape_count++;
	}*/

	/* invert gradient transform for applying to vertices */
	svgtiny_invert_matrix(&state->gradient_transform.a, trans);
	fprintf(stderr, "inverse transform %g %g %g %g %g %g\n",
			trans[0], trans[1], trans[2], trans[3],
			trans[4], trans[5]);

	/* compute points on the path for triangle vertices */
	/* r, r0, r1 are distance along gradient vector */
	gradient_norm_squared = gradient_dx * gradient_dx +
	                              gradient_dy * gradient_dy;
	pts = svgtiny_list_create(
			sizeof (struct grad_point));
	if (!pts)
		return svgtiny_OUT_OF_MEMORY;
	for (j = 0; j != n; ) {
		int segment_type = (int) p[j];
		struct grad_point *point;
		unsigned int z;

		if (segment_type == svgtiny_PATH_MOVE) {
			x0 = p[j + 1];
			y0 = p[j + 2];
			j += 3;
			continue;
		}

		assert(segment_type == svgtiny_PATH_CLOSE ||
				segment_type == svgtiny_PATH_LINE ||
				segment_type == svgtiny_PATH_BEZIER);

		/* start point (x0, y0) */
		x0_trans = trans[0]*x0 + trans[2]*y0 + trans[4];
		y0_trans = trans[1]*x0 + trans[3]*y0 + trans[5];
		r0 = ((x0_trans - gradient_x0) * gradient_dx +
				(y0_trans - gradient_y0) * gradient_dy) /
				gradient_norm_squared;
		point = (grad_point *) svgtiny_list_push(pts);
		if (!point) {
			svgtiny_list_free(pts);
			return svgtiny_OUT_OF_MEMORY;
		}
		point->x = x0;
		point->y = y0;
		point->r = r0;
		if (r0 < min_r) {
			min_r = r0;
			min_pt = svgtiny_list_size(pts) - 1;
		}

		/* end point (x1, y1) */
		if (segment_type == svgtiny_PATH_LINE) {
			x1 = p[j + 1];
			y1 = p[j + 2];
			j += 3;
		} else if (segment_type == svgtiny_PATH_CLOSE) {
			x1 = p[1];
			y1 = p[2];
			j++;
		} else /* svgtiny_PATH_BEZIER */ {
			c0x = p[j + 1];
			c0y = p[j + 2];
			c1x = p[j + 3];
			c1y = p[j + 4];
			x1 = p[j + 5];
			y1 = p[j + 6];
			j += 7;
		}
		x1_trans = trans[0]*x1 + trans[2]*y1 + trans[4];
		y1_trans = trans[1]*x1 + trans[3]*y1 + trans[5];
		r1 = ((x1_trans - gradient_x0) * gradient_dx +
				(y1_trans - gradient_y0) * gradient_dy) /
				gradient_norm_squared;

		/* determine steps from change in r */
		steps = ceilf(fabsf(r1 - r0) / 0.05);
		if (steps == 0)
			steps = 1;
		fprintf(stderr, "r0 %g, r1 %g, steps %i\n",
				r0, r1, steps);

		/* loop through intermediate points */
		for (z = 1; z != steps; z++) {
			float t, x, y, x_trans, y_trans, r;
			struct grad_point *point;
			t = (float) z / (float) steps;
			if (segment_type == svgtiny_PATH_BEZIER) {
				x = (1-t) * (1-t) * (1-t) * x0 +
					3 * t * (1-t) * (1-t) * c0x +
					3 * t * t * (1-t) * c1x +
					t * t * t * x1;
				y = (1-t) * (1-t) * (1-t) * y0 +
					3 * t * (1-t) * (1-t) * c0y +
					3 * t * t * (1-t) * c1y +
					t * t * t * y1;
			} else {
				x = (1-t) * x0 + t * x1;
				y = (1-t) * y0 + t * y1;
			}
			x_trans = trans[0]*x + trans[2]*y + trans[4];
			y_trans = trans[1]*x + trans[3]*y + trans[5];
			r = ((x_trans - gradient_x0) * gradient_dx +
					(y_trans - gradient_y0) * gradient_dy) /
					gradient_norm_squared;
			fprintf(stderr, "(%g %g [%g]) ", x, y, r);
			point = (grad_point *) svgtiny_list_push(pts);
			if (!point) {
				svgtiny_list_free(pts);
				return svgtiny_OUT_OF_MEMORY;
			}
			point->x = x;
			point->y = y;
			point->r = r;
			if (r < min_r) {
				min_r = r;
				min_pt = svgtiny_list_size(pts) - 1;
			}
		}
		fprintf(stderr, "\n");

		/* next segment start point is this segment end point */
		x0 = x1;
		y0 = y1;
	}
	fprintf(stderr, "pts size %i, min_pt %i, min_r %.3f\n",
			svgtiny_list_size(pts), min_pt, min_r);

	/* render triangles */
	stop_count = state->linear_gradient_stop_count;
	assert(2 <= stop_count);
	current_stop = 0;
	last_stop_r = 0;
	current_stop_r = state->gradient_stop[0].offset;
	red0 = red1 = svgtiny_RED(state->gradient_stop[0].color);
	green0 = green1 = svgtiny_GREEN(state->gradient_stop[0].color);
	blue0 = blue1 = svgtiny_BLUE(state->gradient_stop[0].color);
	t = min_pt;
	a = (min_pt + 1) % svgtiny_list_size(pts);
	b = min_pt == 0 ? svgtiny_list_size(pts) - 1 : min_pt - 1;
	while (a != b) {
		struct grad_point *point_t = (grad_point *) svgtiny_list_get(pts, t);
		struct grad_point *point_a = (grad_point *) svgtiny_list_get(pts, a);
		struct grad_point *point_b = (grad_point *) svgtiny_list_get(pts, b);
		float mean_r = (point_t->r + point_a->r + point_b->r) / 3;
		float *p;
		struct svgtiny_shape *shape;
		/*fprintf(stderr, "triangle: t %i %.3f a %i %.3f b %i %.3f "
				"mean_r %.3f\n",
				t, pts[t].r, a, pts[a].r, b, pts[b].r,
				mean_r);*/
		while (current_stop != stop_count && current_stop_r < mean_r) {
			current_stop++;
			if (current_stop == stop_count)
				break;
			red0 = red1;
			green0 = green1;
			blue0 = blue1;
			red1 = svgtiny_RED(state->
					gradient_stop[current_stop].color);
			green1 = svgtiny_GREEN(state->
					gradient_stop[current_stop].color);
			blue1 = svgtiny_BLUE(state->
					gradient_stop[current_stop].color);
			last_stop_r = current_stop_r;
			current_stop_r = state->
					gradient_stop[current_stop].offset;
		}
		p = (float *) malloc(10 * sizeof p[0]);
		if (!p)
			return svgtiny_OUT_OF_MEMORY;
		p[0] = svgtiny_PATH_MOVE;
		p[1] = point_t->x;
		p[2] = point_t->y;
		p[3] = svgtiny_PATH_LINE;
		p[4] = point_a->x;
		p[5] = point_a->y;
		p[6] = svgtiny_PATH_LINE;
		p[7] = point_b->x;
		p[8] = point_b->y;
		p[9] = svgtiny_PATH_CLOSE;
		svgtiny_transform_path(p, 10, state);
		shape = svgtiny_add_shape(state);
		if (!shape) {
			free(p);
			return svgtiny_OUT_OF_MEMORY;
		}
		shape->path = p;
		shape->path_length = 10;
		/*shape->fill = svgtiny_TRANSPARENT;*/
		if (current_stop == 0)
			shape->fill = state->gradient_stop[0].color;
		else if (current_stop == stop_count)
			shape->fill = state->
					gradient_stop[stop_count - 1].color;
		else {
			float stop_r = (mean_r - last_stop_r) /
				(current_stop_r - last_stop_r);
			shape->fill = svgtiny_RGB(
				(int) ((1 - stop_r) * red0 + stop_r * red1),
				(int) ((1 - stop_r) * green0 + stop_r * green1),
				(int) ((1 - stop_r) * blue0 + stop_r * blue1));
		}
		shape->stroke = svgtiny_TRANSPARENT;
		#ifdef GRADIENT_DEBUG
		shape->stroke = svgtiny_RGB(0, 0, 0xff);
		#endif
		state->diagram->shape_count++;
		if (point_a->r < point_b->r) {
			t = a;
			a = (a + 1) % svgtiny_list_size(pts);
		} else {
			t = b;
			b = b == 0 ? svgtiny_list_size(pts) - 1 : b - 1;
		}
	}

	/* render gradient vector for debugging */
	#ifdef GRADIENT_DEBUG
	{
		float *p = malloc(7 * sizeof p[0]);
		if (!p)
			return svgtiny_OUT_OF_MEMORY;
		p[0] = svgtiny_PATH_MOVE;
		p[1] = gradient_x0;
		p[2] = gradient_y0;
		p[3] = svgtiny_PATH_LINE;
		p[4] = gradient_x1;
		p[5] = gradient_y1;
		p[6] = svgtiny_PATH_CLOSE;
		svgtiny_transform_path(p, 7, state);
		struct svgtiny_shape *shape = svgtiny_add_shape(state);
		if (!shape) {
			free(p);
			return svgtiny_OUT_OF_MEMORY;
		}
		shape->path = p;
		shape->path_length = 7;
		shape->fill = svgtiny_TRANSPARENT;
		shape->stroke = svgtiny_RGB(0xff, 0, 0);
		state->diagram->shape_count++;
	}
	#endif

	/* render triangle vertices with r values for debugging */
	#ifdef GRADIENT_DEBUG
	for (unsigned int i = 0; i != pts->size; i++) {
		struct grad_point *point = svgtiny_list_get(pts, i);
		struct svgtiny_shape *shape = svgtiny_add_shape(state);
		if (!shape)
			return svgtiny_OUT_OF_MEMORY;
		char *text = malloc(20);
		if (!text)
			return svgtiny_OUT_OF_MEMORY;
		sprintf(text, "%i=%.3f", i, point->r);
		shape->text = text;
		shape->text_x = state->ctm.a * point->x +
				state->ctm.c * point->y + state->ctm.e;
		shape->text_y = state->ctm.b * point->x +
				state->ctm.d * point->y + state->ctm.f;
		shape->fill = svgtiny_RGB(0, 0, 0);
		shape->stroke = svgtiny_TRANSPARENT;
		state->diagram->shape_count++;
	}
	#endif

	/* plot actual path outline */
	if (state->stroke != svgtiny_TRANSPARENT) {
		struct svgtiny_shape *shape;
		svgtiny_transform_path(p, n, state);

		shape = svgtiny_add_shape(state);
		if (!shape) {
			free(p);
			return svgtiny_OUT_OF_MEMORY;
		}
		shape->path = p;
		shape->path_length = n;
		shape->fill = svgtiny_TRANSPARENT;
		state->diagram->shape_count++;
	} else {
		free(p);
	}

	svgtiny_list_free(pts);

	return svgtiny_OK;
}


/**
 * Get the bounding box of path.
 */

void svgtiny_path_bbox(float *p, unsigned int n,
		float *x0, float *y0, float *x1, float *y1)
{
	unsigned int j;

	*x0 = *x1 = p[1];
	*y0 = *y1 = p[2];

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
			float x = p[j], y = p[j + 1];
			if (x < *x0)
				*x0 = x;
			else if (*x1 < x)
				*x1 = x;
			if (y < *y0)
				*y0 = y;
			else if (*y1 < y)
				*y1 = y;
			j += 2;
		}
	}
}


/**
 * Invert a transformation matrix.
 */
void svgtiny_invert_matrix(float *m, float *inv)
{
	float determinant = m[0]*m[3] - m[1]*m[2];
	inv[0] = m[3] / determinant;
	inv[1] = -m[1] / determinant;
	inv[2] = -m[2] / determinant;
	inv[3] = m[0] / determinant;
	inv[4] = (m[2]*m[5] - m[3]*m[4]) / determinant;
	inv[5] = (m[1]*m[4] - m[0]*m[5]) / determinant;
}



// don't need this any more since Poco XML provides that
/**
 * Find an element in the document by id.
 */
/*
Poco::XML::Element *svgtiny_find_element_by_id(Poco::XML::Element *node, const char *id)
{
	Poco::XML::Element *child;
	Poco::XML::Element *found;

	//for (child = node->children; child; child = child->next) {
    for( child = node->FirstChild( false ); child; child = child->NextSibling( false ) ) {
    
		Poco::XML::Attr *attr;
		//if (child->type != XML_ELEMENT_NODE)
		//	continue;
        
		//attr = xmlHasProp(child, (const xmlChar *) "id");
        
        // it could be an attribute
        for( attr = child->ToElement()->FirstAttribute(); attr; attr = attr->Next() ) {
            if(strcmp(attr->Name(), id) == 0) {
                return child->ToElement();
            }
        }
        
		//if (attr && strcmp(id, (const char *) attr->children->content) == 0)
		//	return child;
        
        
        // recurse
		found = svgtiny_find_element_by_id(child->ToElement(), id);
		if (found)
			return found;
	}

	return 0;
}
 */

