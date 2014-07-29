/*
 * cxmpclient_wrapper.c
 *
 *  Created on: Jul 11, 2014
 *      Author: tobi
 */

#include "cxmpclient_wrapper.h"
#include "cxmp_blocking_client_adapter.h"
#include <xdpd/xmp/client/cxmpclient.h>

#include <assert.h>


static cxmp_blocking_client_adapter *xmp_client = NULL;


void*
new_xmp_client()
{
	puts(__FUNCTION__);
	if (NULL != xmp_client) return xmp_client;

	rofl::logging::init();
	rofl::logging::set_debug_level(rofl::logging::DBG);

	xmp_client = new cxmp_blocking_client_adapter();

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
get_port_info(void* handle, xmlNodePtr resources, xmlDocPtr running)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_portinfo;

	assert(handle);
	assert(resources);
	assert(handle == xmp_client);

	xmp_client->add_port_info(resources, running);

}

void
get_lsi_info(void* handle, xmlNodePtr lsis, xmlDocPtr running)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_portinfo;

	assert(handle);
	assert(lsis);
	assert(handle == xmp_client);

	xmp_client->add_lsi_info(lsis, running);
}

int
create_lsi(void* handle, struct lsi* lsi)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_portinfo;

	assert(handle);
	assert(lsi);
	assert(handle == xmp_client);

	return xmp_client->create_lsi(lsi);
}
