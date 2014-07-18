/*
 * list.h
 *
 *  Created on: Jul 18, 2014
 *      Author: tobi
 */

#ifndef LIST_H_
#define LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

struct list;

struct node {
	struct node *next;
	void *data;
};

struct node*
node_new(void *data);

void
node_delete(struct node **node);

void
list_set_free_fn(struct list *l, void (*free_fn)(void *data));

struct list*
list_new();

void
list_delete(struct list *l);

void
list_append_node(struct list *l, struct node *node);

int
list_append_data(struct list *l, void *data);

struct node*
list_get_head(struct list *l);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H_ */
