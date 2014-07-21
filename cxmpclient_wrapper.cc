/*
 * cxmpclient_wrapper.c
 *
 *  Created on: Jul 11, 2014
 *      Author: tobi
 */

#include "cxmpclient_wrapper.h"
#include "cxmpclient_observer.h"
#include <xdpd/xmp/client/cxmpclient.h>

#include <pthread.h>
#include <assert.h>

static pthread_t worker = 0;
static xdpd::mgmt::protocol::cxmpclient *xmp_client = NULL;
static cxmpclient_observer *observer = NULL;

static pthread_mutex_t client_lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t client_read_cv;

void *
run(void* arg)
{
	puts(__FUNCTION__);
	assert(observer);

	try {
		xmp_client = new xdpd::mgmt::protocol::cxmpclient();
		xmp_client->register_observer(observer);
		xmp_client->set_auto_exit(false);
		xmp_client->run();
	} catch (...) {
		// todo log error
		std::cerr << "got error" << std::endl;
	}

	delete xmp_client;
	xmp_client = NULL;

	pthread_exit(NULL);
}

void*
new_xmp_client()
{
	puts(__FUNCTION__);
	if (NULL != xmp_client) return xmp_client;

	rofl::logging::init();
	rofl::logging::set_debug_level(rofl::logging::DBG);

	observer = new cxmpclient_observer(&client_lock, &client_read_cv);

	pthread_create(&worker, NULL, &run, NULL);

	sleep(1);

	assert(NULL != xmp_client);

	return reinterpret_cast<void*>(xmp_client);
}

void
delete_xmp_client(void* data)
{
	puts(__FUNCTION__);
	assert(NULL != data);
	assert(xmp_client == data);
	delete reinterpret_cast<xdpd::mgmt::protocol::cxmpclient*>(xmp_client);
	xmp_client = NULL;
}

void
get_port_list(void* handle, struct list *port_list)
{
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_portname;

	puts(__FUNCTION__);
	assert(handle);
	assert(port_list);
	assert(handle == xmp_client);

	pthread_mutex_lock(&client_lock);

	xmp_client->port_list();
	xdpd::mgmt::protocol::cxmpmsg &msg = observer->get_msg();
	pthread_mutex_unlock(&client_lock);

	assert(true == msg.get_xmpies().has_ie_multipart());

	const std::deque<cxmpie*> & ies =
			msg.get_xmpies().get_ie_multipart().get_ies();

	std::cerr << "ies.size()=" << ies.size() << std::endl;
	for (std::deque<cxmpie*>::const_iterator iter = ies.begin();
			iter != ies.end(); ++iter) {

		cxmpie_portname* port = dynamic_cast<cxmpie_portname*>(*iter);
		if (NULL == port) continue;


		std::cerr << "append " <<  port->get_portname() << std::endl;

		list_append_data(port_list, strdup(port->get_portname().c_str()));
	}
	list_set_free_fn(port_list, free);
}
