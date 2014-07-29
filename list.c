/*
 * list.c
 *
 *  Created on: Jul 18, 2014
 *      Author: tobi
 */

#include "list.h"

#include <stdlib.h>
#include <assert.h>

struct list {
	struct node *head;
	struct node *tail;
	void
	(*free_fn)(void *data);
};

struct node*
node_new(void *data)
{

	struct node *n = (struct node *) calloc(1, sizeof(struct node));
	n->data = data;
	return n;

}

void
node_delete(struct node *node)
{
	free(node);
}

struct list*
list_new()
{
	struct list* l = (struct list*) calloc(1, sizeof(struct list));
	return l;
}

void
list_delete(struct list *l)
{
	struct node *n = l->head;
	while (n) {
		struct node *tmp = n;
		n = n->next;
		if (l->free_fn) l->free_fn(tmp->data);
		free(tmp);
	}

	free(l);
}

void
list_set_free_fn(struct list *l, void (*free_fn)(void *data))
{
	assert(free_fn);
	l->free_fn = free_fn;
}

void
list_append_node(struct list *l, struct node *node)
{
	l->tail->next = node;
	l->tail = node;
}

int
list_append_data(struct list *l, void *data)
{
	struct node *n = node_new(data);
	if (NULL == n) return -1;

	if (NULL == l->head) {
		assert(NULL == l->tail);
		l->head = n;
		l->tail = n;
	} else {
		l->tail->next = n;
		l->tail = n;
	}

	return 0;
}

struct node*
list_get_head(struct list *l)
{
	assert(l);
	return l->head;
}

void
list_pop_head(struct list *l)
{
	assert(l);
	l->head = l->head->next;
}
