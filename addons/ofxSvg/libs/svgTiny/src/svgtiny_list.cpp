/*
 * This file is part of Libsvgtiny
 * Licensed under the MIT License,
 *                http://opensource.org/licenses/mit-license.php
 * Copyright 2008 James Bursa <james@semichrome.net>
 */

/**
 * A svgtiny_list is a managed array of objects. It grows in chunks to reduce
 * calls to realloc(), but keeps wasted space low.
 */

#include <assert.h>
#include "svgtiny.h"
#include "svgtiny_internal.h"


struct svgtiny_list {
	unsigned int size;	/* number of slots used */
	unsigned int allocated;	/* number of slots allocated (>= size) */
	size_t item_size;	/* size of each slot / bytes */
	char *items;		/* array of slots */
};


/**
 * Create an empty svgtiny_list.
 */

struct svgtiny_list *svgtiny_list_create(size_t item_size)
{
	struct svgtiny_list *list = (svgtiny_list *) malloc(sizeof *list);
	if (!list)
		return 0;
	list->size = 0;
	list->allocated = 0;
	list->item_size = item_size;
	list->items = 0;
	return list;
}


/**
 * Return the number of objects in a list.
 */

unsigned int svgtiny_list_size(struct svgtiny_list *list)
{
	return list->size;
}


/**
 * Set the number of objects in a list. If the size is increased, the new
 * objects are not initialized in any way.
 *
 * The allocation size formula is taken from Python's list:
 * http://svn.python.org/view/python/trunk/Objects/listobject.c?view=markup
 *
 * Objects may have moved after this call. Use svgtiny_list_get() to get new
 * pointers.
 */

svgtiny_code svgtiny_list_resize(struct svgtiny_list *list,
		unsigned int new_size)
{
	unsigned int new_allocated;
	void *new_items;

	if (new_size <= list->allocated) {
		list->size = new_size;
		return svgtiny_OK;
	}

	new_allocated = (new_size >> 3) + (new_size < 9 ? 3 : 6) + new_size;
	if (new_size == 0)
		new_allocated = 0;
	new_items = realloc(list->items, new_allocated * list->item_size);
	if (!new_items)
		return svgtiny_OUT_OF_MEMORY;

	list->size = new_size;
	list->allocated = new_allocated;
	list->items = (char *) new_items;

	return svgtiny_OK;
}


/**
 * Return a pointer to an object in a list.
 */

void *svgtiny_list_get(struct svgtiny_list *list,
		unsigned int i)
{
	assert(i < list->size);
	return (void *) (list->items + i * list->item_size);
}


/**
 * Add space for one object to a list and return a pointer to it.
 */

void *svgtiny_list_push(struct svgtiny_list *list)
{
	svgtiny_code code;
	code = svgtiny_list_resize(list, list->size + 1);
	if (code != svgtiny_OK)
		return 0;
	return svgtiny_list_get(list, list->size - 1);
}


/**
 * Free an entire list.
 */

void svgtiny_list_free(struct svgtiny_list *list)
{
	free(list->items);
	free(list);
}

