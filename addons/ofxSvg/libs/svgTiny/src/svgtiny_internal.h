/*
 * This file is part of Libsvgtiny
 * Licensed under the MIT License,
 *                http://opensource.org/licenses/mit-license.php
 * Copyright 2008 James Bursa <james@semichrome.net>
 */

#ifndef SVGTINY_INTERNAL_H
#define SVGTINY_INTERNAL_H

#ifndef _MSC_VER
	#include <stdbool.h>
#endif

#ifndef UNUSED
#define UNUSED(x) ((void) (x))
#endif

struct svgtiny_gradient_stop {
	float offset;
	svgtiny_colour color;
};

#define svgtiny_MAX_STOPS 10
#define svgtiny_LINEAR_GRADIENT 0x2000000

struct svgtiny_parse_state {
	struct svgtiny_diagram *diagram;
    Poco::XML::Document *document;

	float viewport_width;
	float viewport_height;

	/* current transformation matrix */
	struct {
		float a, b, c, d, e, f;
	} ctm;

	/*struct css_style style;*/

	/* paint attributes */
	svgtiny_colour fill;
	svgtiny_colour stroke;
	int stroke_width;

	/* gradients */
	unsigned int linear_gradient_stop_count;
	const char *gradient_x1, *gradient_y1, *gradient_x2, *gradient_y2;
	struct svgtiny_gradient_stop gradient_stop[svgtiny_MAX_STOPS];
	bool gradient_user_space_on_use;
	struct {
		float a, b, c, d, e, f;
	} gradient_transform;
};

struct svgtiny_list;

/* svgtiny.c */
float svgtiny_parse_length(const char *s, int viewport_size,
		const struct svgtiny_parse_state state);
void svgtiny_parse_color(const char *s, svgtiny_colour *c,
		struct svgtiny_parse_state *state);
void svgtiny_parse_transform(char *s, float *ma, float *mb,
		float *mc, float *md, float *me, float *mf);
struct svgtiny_shape *svgtiny_add_shape(struct svgtiny_parse_state *state);
void svgtiny_transform_path(float *p, unsigned int n,
		struct svgtiny_parse_state *state);
#if defined(_GNU_SOURCE)
#define HAVE_STRNDUP
#else
#undef HAVE_STRNDUP
char *svgtiny_strndup(const char *s, size_t n);
#define strndup svgtiny_strndup
#endif

/* svgtiny_gradient.c */
void svgtiny_find_gradient(const char *id, struct svgtiny_parse_state *state);
svgtiny_code svgtiny_add_path_linear_gradient(float *p, unsigned int n,
		struct svgtiny_parse_state *state);
//TiXmlElement *svgtiny_find_element_by_id(TiXmlElement *node, const char *id);

/* svgtiny_list.c */
struct svgtiny_list *svgtiny_list_create(size_t item_size);
unsigned int svgtiny_list_size(struct svgtiny_list *list);
svgtiny_code svgtiny_list_resize(struct svgtiny_list *list,
		unsigned int new_size);
void *svgtiny_list_get(struct svgtiny_list *list,
		unsigned int i);
void *svgtiny_list_push(struct svgtiny_list *list);
void svgtiny_list_free(struct svgtiny_list *list);

/* colors.gperf */
const struct svgtiny_named_color *
		svgtiny_color_lookup(register const char *str,
				register unsigned int len);

#endif
