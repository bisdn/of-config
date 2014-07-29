/*
 * cxmpclient_wrapper.h
 *
 *  Created on: Jul 11, 2014
 *      Author: tobi
 */

#ifndef CXMPCLIENT_WRAPPER_H_
#define CXMPCLIENT_WRAPPER_H_

#include <stdint.h>
#include <libxml/tree.h>

#ifdef  __cplusplus
extern "C" {
#endif

/* structures */
enum operation {
	ADD,
	DELETE,
	// todo what about modify?
};

struct port {
	char *resource_id;
	enum operation op;
};

struct resources {
	struct port *ports;
	unsigned int port_count;
};

struct lsi {
	uint64_t dpid;
	char *dpname;
	struct resources res;
};


void*
new_xmp_client();

void
delete_xmp_client(void* handle);

void
get_port_info(void* handle, xmlNodePtr resources, xmlDocPtr running);

void
get_lsi_info(void* handle, xmlNodePtr lsis, xmlDocPtr running);

int
create_lsi(void* handle, struct lsi *lsi);

#ifdef  __cplusplus
}
#endif



#endif /* CXMPCLIENT_WRAPPER_H_ */
