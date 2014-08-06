/*
 * list.c
 *
 *  Created on: Jul 18, 2014
 *      Author: tobi
 */

#include "list.h"

#include <stdlib.h>
#include <assert.h>


struct node {
	struct node *next;
	void *data;
};

struct list {
	struct node *head;
	struct node *tail;
	struct node *ptr;
};

static struct node*
node_new(void *data)
{
	struct node *n = (struct node *) calloc(1, sizeof(struct node));
	n->data = data;
	return n;
}

static void
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
		free(tmp);
	}

	free(l);
}

static void
list_append_node(struct list *l, struct node *n)
{
	if (NULL == l->head) {
		assert(NULL == l->tail);
		l->head = n;
		l->tail = n;
	} else {
		l->tail->next = n;
		l->tail = n;
	}
}

int
list_append_data(struct list *l, void *data)
{
	assert(l);
	assert(data);

	struct node *n = node_new(data);
	if (NULL == n) return -1;

	list_append_node(l, n);

	return 0;
}

void *
list_pop_head(struct list *l)
{
	assert(l);
	void *data = NULL;
	if (l->head) {
		struct node *n = l->head;
		data = n->data;
		l->head = n->next;

		if (NULL == l->head) {
			l->tail = NULL;
		}

		node_delete(n);
	}

	l->ptr = NULL;
	return data;
}

void *
list_next(struct list *l)
{
	void *data = NULL;
	assert(l);

	if (l->ptr) {
		l->ptr = l->ptr->next;
	} else {
		l->ptr = l->head;
	}
	if (l->ptr) {
		data = l->ptr->data;
	}
	return data;
}
