/*
 * cxmp_blocking_client_adapter.cc
 *
 *  Created on: Jul 22, 2014
 *      Author: tobi
 */

#include "cxmp_blocking_client_adapter.h"

#include <stddef.h>
#include <assert.h>

#include <rofl/datapath/pipeline/switch_port.h>

static void *
run(void* arg)
{
	puts(__FUNCTION__);

	assert(arg);
	cxmp_blocking_client_adapter* handle = (cxmp_blocking_client_adapter *) arg;

	try {
		handle->xmp_client = new xdpd::mgmt::protocol::cxmpclient();
		handle->xmp_client->register_observer(handle);
		handle->xmp_client->set_auto_exit(false);
		handle->xmp_client->run();
	} catch (...) {
		// todo log error
		std::cerr << "got error" << std::endl;
	}

	delete handle->xmp_client;
	handle->xmp_client = NULL;

	pthread_exit(NULL);
}

cxmp_blocking_client_adapter::cxmp_blocking_client_adapter() :
		xmp_client(NULL), worker(0), msg(NULL)
{
	pthread_mutex_init(&client_lock, NULL);
	pthread_cond_init(&client_read_cv, NULL);

	int rv = pthread_create(&worker, NULL, &run, this);
	printf("rv=%d\n", rv);

	sleep(1);

}

cxmp_blocking_client_adapter::~cxmp_blocking_client_adapter()
{
	// TODO Auto-generated destructor stub
}

void
cxmp_blocking_client_adapter::notify(const xdpd::mgmt::protocol::cxmpmsg& msg)
{
	puts(__PRETTY_FUNCTION__);
	pthread_mutex_lock(&client_lock);
	if (this->msg) delete this->msg;

	std::cerr << "msg: " << msg;

	this->msg = new xdpd::mgmt::protocol::cxmpmsg(msg);
	pthread_cond_signal(&client_read_cv);
	pthread_mutex_unlock(&client_lock);

	std::cerr << "save msg: " << *this->msg << std::endl;
}

static inline xmlChar*
parse_medium(const uint32_t features)
{
	switch (features & (PORT_FEATURE_COPPER | PORT_FEATURE_FIBER)) {
	case PORT_FEATURE_COPPER:
		return BAD_CAST "copper";
	case PORT_FEATURE_FIBER:
		return BAD_CAST "fibre";
	default:
		//assert(0);
		return NULL;
	}
}

static inline xmlChar*
parse_pause(const uint32_t features)
{
	switch (features & (PORT_FEATURE_PAUSE | PORT_FEATURE_PAUSE_ASYM)) {
	case PORT_FEATURE_PAUSE:
		return BAD_CAST "symmetric";
	case (PORT_FEATURE_PAUSE|PORT_FEATURE_PAUSE_ASYM):
	case PORT_FEATURE_PAUSE_ASYM:
		return BAD_CAST "asymmetric";
	default:
		return BAD_CAST "unsupported";
	}
}

static inline xmlChar*
parse_rate(const uint32_t features)
{
	switch (features
			& (PORT_FEATURE_10MB_HD | PORT_FEATURE_10MB_FD
					| PORT_FEATURE_100MB_HD | PORT_FEATURE_100MB_FD
					| PORT_FEATURE_1GB_HD | PORT_FEATURE_1GB_FD
					| PORT_FEATURE_10GB_FD | PORT_FEATURE_40GB_FD
					| PORT_FEATURE_100GB_FD | PORT_FEATURE_1TB_FD
					| PORT_FEATURE_OTHER)) {
	case PORT_FEATURE_10MB_HD:
		return BAD_CAST "10Mb-HD";
	case PORT_FEATURE_10MB_FD:
		return BAD_CAST "10Mb-FD";
	case PORT_FEATURE_100MB_HD:
		return BAD_CAST "100Mb-HD";
	case PORT_FEATURE_100MB_FD:
		return BAD_CAST "100Mb-FD";
	case PORT_FEATURE_1GB_HD:
		return BAD_CAST "1Gb-HD";
	case PORT_FEATURE_1GB_FD:
		return BAD_CAST "1Gb-FD";
	case PORT_FEATURE_10GB_FD:
		return BAD_CAST "10Gb";
	case PORT_FEATURE_40GB_FD:
		return BAD_CAST "40Gb";
	case PORT_FEATURE_100GB_FD:
		return BAD_CAST "100Gb";
	case PORT_FEATURE_1TB_FD:
		return BAD_CAST "1Tb";
	case PORT_FEATURE_OTHER:
		return BAD_CAST "other";
		break;
	default:
		// assert(0);
		return NULL;
	}
}

static void
append_all_rates(xmlNodePtr node, const uint32_t features)
{
	if (features & PORT_FEATURE_10MB_HD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_10MB_HD));
	if (features & PORT_FEATURE_10MB_HD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_10MB_HD));
	if (features & PORT_FEATURE_10MB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_10MB_FD));
	if (features & PORT_FEATURE_100MB_HD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_100MB_HD));
	if (features & PORT_FEATURE_100MB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_100MB_FD));
	if (features & PORT_FEATURE_1GB_HD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_1GB_HD));
	if (features & PORT_FEATURE_1GB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_1GB_FD));
	if (features & PORT_FEATURE_10GB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_10GB_FD));
	if (features & PORT_FEATURE_40GB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_40GB_FD));
	if (features & PORT_FEATURE_100GB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_100GB_FD));
	if (features & PORT_FEATURE_1TB_FD) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_1TB_FD));
	if (features & PORT_FEATURE_OTHER) xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_OTHER));
}

void
cxmp_blocking_client_adapter::add_port_info(xmlNodePtr resources)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_portinfo;

	pthread_mutex_lock(&client_lock);
	xmp_client->port_info();
	pthread_cond_wait(&client_read_cv, &client_lock);
	pthread_mutex_unlock(&client_lock);

	assert(true == msg->get_xmpies().has_ie_multipart());

	const std::deque<cxmpie*> & ies =
			msg->get_xmpies().get_ie_multipart().get_ies();

	std::cerr << "ies.size()=" << ies.size() << std::endl;
	for (std::deque<cxmpie*>::const_iterator iter = ies.begin();
			iter != ies.end(); ++iter) {

		cxmpie_portinfo* port_info = dynamic_cast<cxmpie_portinfo*>(*iter);
		if (NULL == port_info) continue;

		std::cerr << "append " << port_info->get_portname() << std::endl;

		xmlNodePtr port = xmlNewChild(resources, resources->ns, BAD_CAST "port", NULL);
		xmlChar buf[255];

		// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:number
		xmlStrPrintf(buf, sizeof(buf), BAD_CAST "%u", port_info->get_port_num());
		xmlNewChild(port, resources->ns, BAD_CAST "number", buf);

		// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:name
		xmlNewChild(port, resources->ns, BAD_CAST "name", BAD_CAST port_info->get_portname().c_str());


		{
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state
			xmlNodePtr state = xmlNewChild(port, resources->ns, BAD_CAST "state", NULL);

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state/ofc:oper-state (up|down)
			xmlChar *ptr = BAD_CAST ((port_info->get_state() & PORT_STATE_LINK_DOWN) ? "down" : "up");
			xmlNewChild(state, resources->ns, BAD_CAST "oper-state", ptr);

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state/ofc:blocked (xs:boolean)
			ptr = BAD_CAST ((port_info->get_state() & PORT_STATE_BLOCKED) ? "true" : "false");
			xmlNewChild(state, resources->ns, BAD_CAST "blocked", ptr);

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state/ofc:live (xs:boolean)
			ptr = BAD_CAST ((port_info->get_state() & PORT_STATE_LIVE) ? "true" : "false");
			xmlNewChild(state, resources->ns, BAD_CAST "live", ptr);
		}

		{
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current
			xmlNodePtr features = xmlNewChild(port, resources->ns, BAD_CAST "features", NULL);

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current
			xmlNodePtr node = xmlNewChild(port, resources->ns, BAD_CAST "current", NULL);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:rate
			xmlNodePtr rate = xmlNewNode(resources->ns, BAD_CAST "rate");
			if (port_info->get_features_current() & PORT_FEATURE_OTHER) {
				xmlAddChild(rate, xmlNewText(BAD_CAST "other"));

				// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:current-rate [kbit/s]
				xmlStrPrintf(buf, sizeof(buf), BAD_CAST "%u", port_info->get_current_rate()); // fixme get the format right for uint64 llu vs lu
				xmlNewChild(port, resources->ns, BAD_CAST "current-rate", buf);

				// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:max-rate [kbit/s]
				xmlStrPrintf(buf, sizeof(buf), BAD_CAST "%u", port_info->get_current_rate_max()); // fixme get the format right for uint64 llu vs lu
				xmlNewChild(port, resources->ns, BAD_CAST "max-rate", buf);

			} else {
				// for the current port there is only one valid rate allowed
				xmlAddChild(rate, xmlNewText(parse_rate(port_info->get_features_current())));
			}
			xmlAddChild(node, rate);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:auto-negotiate (xs:boolean)
			xmlChar *ptr = BAD_CAST ((port_info->get_features_current() & PORT_FEATURE_AUTONEG) ? "true" : "false");
			xmlNewChild(node, resources->ns, BAD_CAST "auto-negotiate", ptr);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:medium (copper|fibre)
			xmlNewChild(node, resources->ns, BAD_CAST "medium", parse_medium(port_info->get_features_current()));
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:pause (unsupported|symmetric|asymmetric)
			xmlNewChild(node, resources->ns, BAD_CAST "pause", parse_pause(port_info->get_features_current()));

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported
			node = xmlNewChild(port, resources->ns, BAD_CAST "supported", NULL);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:rate (multiple times)
			append_all_rates(node, port_info->get_features_supported());
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:auto-negotiate (xs:boolean)
			ptr = BAD_CAST ((port_info->get_features_supported() & PORT_FEATURE_AUTONEG) ? "true" : "false");
			xmlNewChild(node, resources->ns, BAD_CAST "auto-negotiate", ptr);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:medium (copper|fibre)
			xmlNewChild(node, resources->ns, BAD_CAST "medium", parse_medium(port_info->get_features_supported()));
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:pause (unsupported|symmetric|asymmetric)
			xmlNewChild(node, resources->ns, BAD_CAST "pause", parse_pause(port_info->get_features_supported()));

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer
			node = xmlNewChild(port, resources->ns, BAD_CAST "advertised-peer", NULL);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:rate (multiple times)
			append_all_rates(node, port_info->get_features_advertised_peer());
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:auto-negotiate (xs:boolean)
			ptr = BAD_CAST ((port_info->get_features_advertised_peer() & PORT_FEATURE_AUTONEG) ? "true" : "false");
			xmlNewChild(node, resources->ns, BAD_CAST "auto-negotiate", ptr);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:medium (copper|fibre)
			xmlNewChild(node, resources->ns, BAD_CAST "medium", parse_medium(port_info->get_features_advertised_peer()));
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:pause (unsupported|symmetric|asymmetric)
			xmlNewChild(node, resources->ns, BAD_CAST "pause", parse_pause(port_info->get_features_advertised_peer()));
		}
	}
}
