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
get_port_list(void* handle, unsigned int *count, struct node *port_list)
{
	puts(__FUNCTION__);
	assert(handle);
	assert(count);
	assert(port_list);
	assert(handle == xmp_client);

	pthread_mutex_lock(&client_lock);

	xmp_client->port_list();
	observer->get_msg();
	pthread_mutex_unlock(&client_lock);

}
