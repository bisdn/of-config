/*
 * cxmp_blocking_client_adapter.cc
 *
 *  Created on: Jul 22, 2014
 *      Author: tobi
 */

#include "cxmp_blocking_client_adapter.h"

#include <stddef.h>
#include <assert.h>
#include <libxml/xpath.h>

#include "xml_helper.h"

#include <rofl/datapath/pipeline/switch_port.h>
#include <rofl/datapath/pipeline/openflow/openflow1x/pipeline/of1x_pipeline.h>

extern struct ns_pair namespace_mapping[];

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
		//assert(0);  // fixme actually it should be an assert, but some platforms do not support valid features
		return  BAD_CAST "copper";
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
		// assert(0); // fixme actually it should be an assert, but some platforms do not support valid features
		return BAD_CAST "1Gb-FD";
	}
}

static void
append_all_rates(xmlNodePtr node, const uint32_t features)
{
	if ((features
			& (PORT_FEATURE_10MB_HD | PORT_FEATURE_10MB_FD
					| PORT_FEATURE_100MB_HD | PORT_FEATURE_100MB_FD
					| PORT_FEATURE_1GB_HD | PORT_FEATURE_1GB_FD
					| PORT_FEATURE_10GB_FD | PORT_FEATURE_40GB_FD
					| PORT_FEATURE_100GB_FD | PORT_FEATURE_1TB_FD
					| PORT_FEATURE_OTHER))) {
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
	} else {
		// assert(0); // fixme actually it should be an assert, but some platforms do not support valid features
		xmlNewChild(node, node->ns, BAD_CAST "rate", parse_rate(PORT_FEATURE_1GB_FD));
	}
}

void
cxmp_blocking_client_adapter::add_port_info(xmlNodePtr resources, xmlDocPtr running)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_portinfo;

	pthread_mutex_lock(&client_lock);
	xmp_client->port_info();
	pthread_cond_wait(&client_read_cv, &client_lock);
	pthread_mutex_unlock(&client_lock);

	if (0 == msg->get_xmpies().size()) {
		std::cerr << "no ports found" << std::endl;
		return;
	}

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

		// resource-id is missing?
		if (running) {
			xmlStrPrintf(buf, sizeof(buf), BAD_CAST "/ofc:capable-switch/ofc:resources/ofc:port[ofc:name=%s]/ofc:resource-id", port_info->get_port_num());
			xmlXPathObjectPtr xpath_obj;
			if (NULL == (xpath_obj = get_node(running, namespace_mapping, buf))) {
				// no resource-id, so we set the port as resource-id
				xmlNewChild(port, resources->ns, BAD_CAST "resource-id", BAD_CAST port_info->get_portname().c_str());
			}
		}

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
			xmlNodePtr node = xmlNewChild(features, resources->ns, BAD_CAST "current", NULL);
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
			node = xmlNewChild(features, resources->ns, BAD_CAST "supported", NULL);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:rate (multiple times)
			append_all_rates(node, port_info->get_features_supported());
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:auto-negotiate (xs:boolean)
			ptr = BAD_CAST ((port_info->get_features_supported() & PORT_FEATURE_AUTONEG) ? "true" : "false");
			xmlNewChild(node, resources->ns, BAD_CAST "auto-negotiate", ptr);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:medium (copper|fibre)
			xmlNewChild(node, resources->ns, BAD_CAST "medium", parse_medium(port_info->get_features_supported()));
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:pause (unsupported|symmetric|asymmetric)
			xmlNewChild(node, resources->ns, BAD_CAST "pause", parse_pause(port_info->get_features_supported()));
			xmlNodePtr supported = node; // back up supported node

			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer
			node = xmlNewChild(features, resources->ns, BAD_CAST "advertised-peer", NULL);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:rate (multiple times)
			append_all_rates(node, port_info->get_features_advertised_peer());
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:auto-negotiate (xs:boolean)
			ptr = BAD_CAST ((port_info->get_features_advertised_peer() & PORT_FEATURE_AUTONEG) ? "true" : "false");
			xmlNewChild(node, resources->ns, BAD_CAST "auto-negotiate", ptr);
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:medium (copper|fibre)
			xmlNewChild(node, resources->ns, BAD_CAST "medium", parse_medium(port_info->get_features_advertised_peer()));
			// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:pause (unsupported|symmetric|asymmetric)
			xmlNewChild(node, resources->ns, BAD_CAST "pause", parse_pause(port_info->get_features_advertised_peer()));

			// since the advertised node is not optional we will
			// add it here if it was not configured in the running config
			if (running) {
				xmlStrPrintf(buf, sizeof(buf), BAD_CAST "/ofc:capable-switch/ofc:resources/ofc:port[ofc:name=%s]/ofc:features/ofc:advertised-peer", port_info->get_port_num());
				xmlXPathObjectPtr xpath_obj;
				if (NULL != (xpath_obj = get_node(running, namespace_mapping, buf))) {
					// fixme some parts have been configured... some not, so we will
					// print the supported values only for the not set parts
				} else {
					node = xmlNewChild(features, NULL, BAD_CAST "advertised", NULL);
					// copy supported to advertised
					xmlNodePtr list = xmlCopyNodeList(supported->children);
					xmlAddChildList(node, list);
				}
			}
		}
	}
}

void
cxmp_blocking_client_adapter::add_lsi_info(xmlNodePtr lsis, xmlDocPtr running)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;
	using xdpd::mgmt::protocol::cxmpie_lsiinfo;

	pthread_mutex_lock(&client_lock);
	xmp_client->lsi_info();
	pthread_cond_wait(&client_read_cv, &client_lock);
	pthread_mutex_unlock(&client_lock);

	if (0 == msg->get_xmpies().size()) {
		std::cerr << "no lsi found" << std::endl;
		return;
	}

	assert(true == msg->get_xmpies().has_ie_multipart());

	const std::deque<cxmpie*> & ies = msg->get_xmpies().get_ie_multipart().get_ies();

	std::cerr << "ies.size()=" << ies.size() << std::endl;
	for (std::deque<cxmpie*>::const_iterator iter = ies.begin(); iter != ies.end(); ++iter) {
		cxmpie_lsiinfo* lsi_info = dynamic_cast<cxmpie_lsiinfo*>(*iter);

		std::cerr << lsi_info->get_lsiname() << std::endl;

		xmlChar buf[255];

		xmlNodePtr sw = xmlNewChild(lsis, lsis->ns, BAD_CAST "switch", NULL);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities
		xmlNodePtr caps = xmlNewChild(sw, lsis->ns, BAD_CAST "capabilities", NULL);

		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:max-buffered-packets (type uint32)
		xmlStrPrintf(buf, sizeof(buf), BAD_CAST "%u", lsi_info->get_num_of_buffers()); // fixme get the format right for uint64 llu vs lu
		xmlNewChild(caps, lsis->ns, BAD_CAST "max-buffered-packets", buf);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:max-tables (type uint8)
		xmlStrPrintf(buf, sizeof(buf), BAD_CAST "%u", lsi_info->get_max_tables());
		xmlNewChild(caps, lsis->ns, BAD_CAST "max-tables", buf);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:max-ports (type uint32)
		xmlStrPrintf(buf, sizeof(buf), BAD_CAST "%u", lsi_info->get_max_ports()); // fixme get the format right for uint64 llu vs lu
		xmlNewChild(caps, lsis->ns, BAD_CAST "max-ports", buf);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:flow-statistics (xs:boolean)
		xmlChar *ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_FLOW_STATS) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "flow-statistics", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:table-statistics (xs:boolean)
		ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_TABLE_STATS) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "table-statistics", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:port-statistics (xs:boolean)
		ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_PORT_STATS) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "port-statistics", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-statistics (xs:boolean)
		ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_GROUP_STATS) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "group-statistics", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:queue-statistics (xs:boolean)
		ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_QUEUE_STATS) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "queue-statistics", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reassemble-ip-fragments (xs:boolean)
		ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_IP_REASM) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "reassemble-ip-fragments", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:block-looping-ports (xs:boolean)
		ptr = BAD_CAST ((lsi_info->get_capabilities() & OF1X_CAP_PORT_BLOCKED) ? "true" : "false");
		xmlNewChild(caps, lsis->ns, BAD_CAST "block-looping-ports", ptr);
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reserved-port-types
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reserved-port-types/ofc:type
		// todo
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-types (:ref:)
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-types/ofc:type (all|select|indirect|fast-failover)
		// fixme not yet exported
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-capabilities (:ref:)
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-capabilities/ofc:capability (select-weight|select-liveness|chaining|chaining-check)
		// fixme not yet exported
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:action-types
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:action-types/ofc:type (output|copy-ttl-out|copy-ttl-in|set-mpls-ttl|dec-mpls-ttl|push-vlan|pop-vlan|push-mpls|pop-mpls|set-queue|group|set-nw-ttl|dec-nw-ttl|set-field)
		// todo this is only available per table
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:instruction-types
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:instruction-types/ofc:type (apply-actions|clear-actions|write-actions|write-metadata|goto-table)
		// todo this is only available per table

		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:connection-state
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:current-version
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:supported-versions
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:local-ip-address-in-use
		// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:local-port-in-use
	}
}

int
cxmp_blocking_client_adapter::create_lsi(struct lsi* lsi)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;

	pthread_mutex_lock(&client_lock);
	xmp_client->lsi_create(lsi->dpid, std::string(lsi->dpname));
	pthread_cond_wait(&client_read_cv, &client_lock);
	pthread_mutex_unlock(&client_lock);

	int rv = 0;
	switch (msg->get_type()) {
	case XMPT_REPLY:
		break;
	case XMPT_ERROR:
		rv = -1;
		break;
	case XMPT_REQUEST:
	case XMPT_NOTIFICATION:
	default:
		rv = -2;
		break;
	}

	return rv;
}

int
cxmp_blocking_client_adapter::attach_port(const uint64_t dpid, const char* port_name)
{
	puts(__PRETTY_FUNCTION__);
	using xdpd::mgmt::protocol::cxmpie;

	pthread_mutex_lock(&client_lock);
	xmp_client->port_attach(dpid, std::string(port_name));
	pthread_cond_wait(&client_read_cv, &client_lock);
	pthread_mutex_unlock(&client_lock);

	int rv = 0;
	switch (msg->get_type()) {
	case XMPT_REPLY:
		break;
	case XMPT_ERROR:
		rv = -1;
		break;
	case XMPT_REQUEST:
	case XMPT_NOTIFICATION:
	default:
		rv = -2;
		break;
	}

	return rv;
}
