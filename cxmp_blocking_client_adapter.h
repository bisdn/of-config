/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef CXMP_BLOCKING_CLIENT_ADAPTER_H_
#define CXMP_BLOCKING_CLIENT_ADAPTER_H_

#include <pthread.h>
#include <libxml/tree.h>
#include <list>

#include <xdpd/xmp/client/cxmpclient.h>

//#include "cxmpclient_wrapper.h"

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
	lsi_create(const uint64_t dpid, const std::string &dpname, const std::list<struct xdpd::mgmt::protocol::controller>& controller);

	int
	lsi_destroy(const uint64_t dpid);

	int
	lsi_connect_to_controller(const uint64_t dpid, const std::list<struct xdpd::mgmt::protocol::controller>& controller);

	int
	lsi_cross_connect(const uint64_t dpid1, const uint64_t port_no1, const uint64_t dpid2, const uint64_t port_no2);

	int
	port_attach(const uint64_t dpid, const char* port_name);

	int
	port_detach(const uint64_t dpid, const char* port_name);

	int
	port_enable(const char* port_name);

	int
	port_disable(const char* port_name);

private:

	static void *
	run(void* arg);
	xdpd::mgmt::protocol::cxmpclient *xmp_client;
	pthread_mutex_t client_lock;
	unsigned int max_time_wait_seconds;

	pthread_t worker;
	pthread_cond_t client_read_cv;
	xdpd::mgmt::protocol::cxmpmsg *msg;

	struct timespec*
	get_wait_time();
};

#endif /* CXMP_BLOCKING_CLIENT_ADAPTER_H_ */
