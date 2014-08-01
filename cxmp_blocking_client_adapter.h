/*
 * cxmp_blocking_client_adapter.h
 *
 *  Created on: Jul 22, 2014
 *      Author: tobi
 */

#ifndef CXMP_BLOCKING_CLIENT_ADAPTER_H_
#define CXMP_BLOCKING_CLIENT_ADAPTER_H_

#include <pthread.h>
#include <libxml/tree.h>

#include <xdpd/xmp/client/cxmpclient.h>

#include "cxmpclient_wrapper.h"

class cxmp_blocking_client_adapter : public xdpd::mgmt::protocol::cxmpobserver {
public:
	cxmp_blocking_client_adapter();
	virtual ~cxmp_blocking_client_adapter();

	virtual void
	notify(const xdpd::mgmt::protocol::cxmpmsg &msg);

	void
	get_resources(xmlNodePtr resources);

	void
	get_all_ports(xmlNodePtr resources);

	void
	add_port_info(xmlNodePtr resources, xmlDocPtr running);

	void
	add_lsi_info(xmlNodePtr lsis, xmlDocPtr running);

	void
	get_lsi_config(xmlNodePtr lsis);

	void
	get_lsi_ports(const uint64_t dpid, xmlNodePtr resources);

	int
	create_lsi(struct lsi *lsi);

	int
	destroy_lsi(const uint64_t dpid);

	int
	attach_port(const uint64_t dpid, const char* port_name);

	int
	detach_port(const uint64_t dpid, const char* port_name);

	xdpd::mgmt::protocol::cxmpclient *xmp_client;
private:

	pthread_mutex_t client_lock;
	pthread_t worker;
	pthread_cond_t client_read_cv;
	xdpd::mgmt::protocol::cxmpmsg *msg;

};

#endif /* CXMP_BLOCKING_CLIENT_ADAPTER_H_ */
