/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CXMPCLIENT_WRAPPER_H_
#define CXMPCLIENT_WRAPPER_H_

#include <stdint.h>
#include <libxml/tree.h>
#include "list.h"

#ifdef  __cplusplus
extern "C" {
#endif

/* structures */// TODO move structures to a more suitable file
enum operation {
	ADD,
	DELETE,
	MODIFY
};

struct port {
	uint64_t dpid;
	char *resource_id;
	enum operation op;
};

struct resources {
	void *port_list_add;
	void *port_list_del;
};

struct controller {
	char *id;
	void *ip;
	uint8_t ip_domain;
	uint16_t port;
	char *proto;
};

struct lsi { // todo operation per changed item?
	uint64_t dpid;
	char *dpname;
	struct resources res;
	void *controller_list_add;
	void *controller_list_del;
	enum operation op;
};


void*
new_xmp_client();

void
delete_xmp_client(void* handle);

void
get_resources(void* handle, xmlNodePtr resources);

void
get_port_info(void* handle, xmlNodePtr resources, xmlDocPtr running);

void
get_lsi_info(void* handle, xmlNodePtr lsis, xmlDocPtr running);

void
get_lsi_config(void* handle, xmlNodePtr lsis);

int
lsi_create(void* handle, struct lsi *lsi);

int
lsi_destroy(void* handle, const uint64_t dpid);

int
lsi_connect_to_controller(void* handle, struct lsi *lsi);

int
lsi_cross_connect(void* handle, const uint64_t dpid1, const uint64_t port_no1, const uint64_t dpid2, const uint64_t port_no2);

int
port_attach(void* handle, uint64_t dpid, char* port_name);

int
port_detach(void* handle, uint64_t dpid, char* port_name);

int
port_enable(void* handle, const char* port_name);

int
port_disable(void* handle, const char* port_name);

#ifdef  __cplusplus
}
#endif



#endif /* CXMPCLIENT_WRAPPER_H_ */
