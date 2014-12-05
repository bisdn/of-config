/*
 * This is automatically generated callbacks file
 * It contains 3 parts: Configuration callbacks, RPC callbacks and state data callbacks.
 * Do NOT alter function signatures or any structures unless you know exactly what you are doing.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libnetconf_xml.h>

#include "xml_helper.h"
#include "cxmpclient_wrapper.h"
#include "utils.h"

extern struct ns_pair namespace_mapping[];

/* transAPI version which must be compatible with libnetconf */
int transapi_version = 5;

/* Signal to libnetconf that configuration data were modified by any callback.
 * 0 - data not modified
 * 1 - data have been modified
 */
int config_modified = 0;

/*
 * Determines the callbacks order.
 * Set this variable before compilation and DO NOT modify it in runtime.
 * TRANSAPI_CLBCKS_LEAF_TO_ROOT (default)
 * TRANSAPI_CLBCKS_ROOT_TO_LEAF
 */
const TRANSAPI_CLBCKS_ORDER_TYPE callbacks_order = TRANSAPI_CLBCKS_ORDER_DEFAULT;

/* Do not modify or set! This variable is set by libnetconf to announce edit-config's error-option
Feel free to use it to distinguish module behavior for different error-option values.
 * Possible values:
 * NC_EDIT_ERROPT_STOP - Following callback after failure are not executed, all successful callbacks executed till
                         failure point must be applied to the device.
 * NC_EDIT_ERROPT_CONT - Failed callbacks are skipped, but all callbacks needed to apply configuration changes are executed
 * NC_EDIT_ERROPT_ROLLBACK - After failure, following callbacks are not executed, but previous successful callbacks are
                         executed again with previous configuration data to roll it back.
 */
NC_EDIT_ERROPT_TYPE erropt = NC_EDIT_ERROPT_NOTSET;

/* status structure */
struct of_config__status {
	void *xmp_client_handle;
	char *capable_switch_id;

	struct list *lsi_list;
} ofc_state;

static void *__data = NULL;

#define LSI(data) ((struct lsi*) *data)
#define CONTROLLER(data) ((struct controller*) data)

/**
 * @brief Initialize plugin after loaded and before any other functions are called.

 * This function should not apply any configuration data to the controlled device. If no
 * running is returned (it stays *NULL), complete startup configuration is consequently
 * applied via module callbacks. When a running configuration is returned, libnetconf
 * then applies (via module's callbacks) only the startup configuration data that
 * differ from the returned running configuration data.

 * Please note, that copying startup data to the running is performed only after the
 * libnetconf's system-wide close - see nc_close() function documentation for more
 * information.

 * @param[out] running	Current configuration of managed device.

 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int transapi_init(xmlDocPtr * running)
{
	assert(running);
	assert(NULL == *running);

	memset(&ofc_state, 0, sizeof(struct of_config__status));
	ofc_state.xmp_client_handle = new_xmp_client();

	// since we currently cannot query the switch_id we set it manually
	ofc_state.capable_switch_id = strdup("xdpd-switch");

	// create running config as following:
	//	   +--rw capable-switch
	//	      +--rw id                      inet:uri
	//	      +--rw configuration-points
	//	      +--rw resources
	//	      +--rw logical-switches

	nc_verb_verbose("create running config");

	*running = xmlNewDoc(BAD_CAST "1.0");
	assert(*running);
	xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "capable-switch");
	assert(root);
	xmlDocSetRootElement(*running, root);
	xmlNsPtr ns = xmlNewNs(root, BAD_CAST namespace_mapping[0].href, NULL);
	assert(ns);
	xmlSetNs(root, ns);

	xmlNodePtr id = xmlNewChild(root, ns, BAD_CAST "id", BAD_CAST ofc_state.capable_switch_id);
	assert(id);
	xmlNodePtr config_points = xmlNewChild(root, ns, BAD_CAST "configuration-points", NULL);
	assert(config_points);
	xmlNodePtr resources = xmlNewChild(root, ns, BAD_CAST "resources", NULL);
	assert(resources);
	get_resources(ofc_state.xmp_client_handle, resources);
	xmlNodePtr lsis = xmlNewChild(root, ns, BAD_CAST "logical-switches", NULL);
	assert(lsis);
	get_lsi_config(ofc_state.xmp_client_handle, lsis);


	xmlSaveFormatFileEnc("-", *running, "UTF-8", 1);

	nc_verb_verbose("init done\n");
	return EXIT_SUCCESS;
}

/**
 * @brief Free all resources allocated on plugin runtime and prepare plugin for removal.
 */
void transapi_close(void)
{
	delete_xmp_client(ofc_state.xmp_client_handle);
	ofc_state.xmp_client_handle = NULL;

	if(ofc_state.capable_switch_id) {
		free(ofc_state.capable_switch_id);
		ofc_state.capable_switch_id = NULL;
	}

	nc_verb_verbose("closed\n");
	return;
}

/**
 * @brief Retrieve state data from device and return them as XML document
 *
 * @param model	Device data model. libxml2 xmlDocPtr.
 * @param running	Running datastore content. libxml2 xmlDocPtr.
 * @param[out] err  Double pointer to error structure. Fill error when some occurs.
 * @return State data as libxml2 xmlDocPtr or NULL in case of error.
 */
xmlDocPtr get_state_data (xmlDocPtr model, xmlDocPtr running, struct nc_err **err)
{
	nc_verb_verbose("get_state_data\n");

	nc_verb_verbose("erropt=%u\n", erropt);

	xmlDocPtr state;
	xmlNodePtr root;
	xmlNsPtr ns;

	state = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewDocNode(state, NULL, BAD_CAST "capable-switch", NULL);
	xmlDocSetRootElement(state, root);
	ns = xmlNewNs(root, BAD_CAST "urn:onf:of111:config:yang", NULL);
	xmlSetNs(root, ns);

	// state that should be queried here
	// ### base
	// #/ofc:capable-switch/ofc:config-version
	xmlNewChild(root, ns, BAD_CAST "config-version", BAD_CAST "1.1.1");
//	// ### configuration points
//	// ### Resources
//
	xmlNodePtr resources = xmlNewChild(root, NULL, BAD_CAST "resources", NULL);

	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:number
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:current-rate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:max-rate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state/ofc:oper-state
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state/ofc:blocked
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:state/ofc:live
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:rate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:auto-negotiate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:medium
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:current/ofc:pause
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:rate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:auto-negotiate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:medium
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:pause
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:rate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:auto-negotiate
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:medium
	// #/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:pause
	get_port_info(ofc_state.xmp_client_handle, resources, running);

	xmlNodePtr lsis = xmlNewChild(root, NULL, BAD_CAST "logical-switches", NULL);
	// ### LSIs
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:max-buffered-packets
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:max-tables
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:max-ports
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:flow-statistics
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:table-statistics
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:port-statistics
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-statistics
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:queue-statistics
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reassemble-ip-fragments
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:block-looping-ports
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reserved-port-types
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reserved-port-types/ofc:type
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-types
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-types/ofc:type
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-capabilities
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-capabilities/ofc:capability
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:action-types
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:action-types/ofc:type
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:instruction-types
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:instruction-types/ofc:type
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:connection-state
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:current-version
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:supported-versions
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:local-ip-address-in-use
	// #/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:local-port-in-use
	get_lsi_info(ofc_state.xmp_client_handle, lsis, running);

	return state;
}
/*
 * Mapping prefixes with namespaces.
 * Do NOT modify this structure!
 */
struct ns_pair namespace_mapping[] = {{"ofc", "urn:onf:of111:config:yang"}, {"xdpd-mgmt", "urn:xdpd:mgmt:yang"}, {NULL, NULL}};

/*
* CONFIGURATION callbacks
* Here follows set of callback functions run every time some change in associated part of running datastore occurs.
* You can safely modify the bodies of all function as well as add new functions for better lucidity of code.
*/

/**
 * @brief This callback will be run when node in path /ofc:capable-switch changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);

	xmlSaveFormatFileEnc("-", node->doc, "UTF-8", 1);

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(0 == strcmp(node->name, "id"));
	assert(node);
	assert(node->doc);
	assert(node->children);

	if ((XMLDIFF_ADD|XMLDIFF_MOD) & op) {

		xmlChar* text = xmlNodeListGetString(node->doc, node->children, 1);

		nc_verb_verbose("got string: \n%s\n", text);

		if (ofc_state.capable_switch_id) {
			free(ofc_state.capable_switch_id);
		}
		ofc_state.capable_switch_id = strdup(text);
		xmlFree(text);
	} else if (XMLDIFF_REM & op) {
		if (ofc_state.capable_switch_id) {
			free(ofc_state.capable_switch_id);
			ofc_state.capable_switch_id = NULL;
		}
	} else {
		// todo anything else?
		nc_verb_verbose("%s: unsupported operation %u\n", __PRETTY_FUNCTION__, op);
		assert(0);
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:configuration-points changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_configuration_points (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_resource_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:admin-state changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_admin_state (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	int rv = EXIT_SUCCESS;
	int down = 0;	// default is up

	if ((XMLDIFF_ADD|XMLDIFF_MOD) & op) {

		if (xmlStrEqual(XML_GET_CONTENT(node->children), BAD_CAST "down")) {
			down = 1;
		}

		// sanity check... if the content is not "down", it has to be "up"
		assert(down || xmlStrEqual(XML_GET_CONTENT(node->children), BAD_CAST "up"));

		// currently the resource-id is the port name (even if the name is not set
		xmlNodePtr tmp = find_element(BAD_CAST "resource-id",  node->parent->parent->children);
		assert(tmp);

		if (down) {
			// set interface down
			nc_verb_verbose("set interface %s down\n", tmp->children->content);
			if (port_disable(ofc_state.xmp_client_handle, tmp->children->content)) {
				rv = EXIT_FAILURE;
			}

		} else {
			// set interface up
			nc_verb_verbose("set interface %s up\n", tmp->children->content);
			if (port_enable(ofc_state.xmp_client_handle, tmp->children->content)) {
				rv = EXIT_FAILURE;
			}
		}

	} else if (XMLDIFF_REM & op) {
		// setting interface up
	} else {
		nc_verb_error("unsupported op");
		assert(0);
	}


	return rv;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:features changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

static void
lsi_cleanup(struct lsi *data)
{
	if (data->res.port_list_add) {
		list_delete(data->res.port_list_add);
		data->res.port_list_add = NULL;
	}

	if (data->res.port_list_del) {
		list_delete(data->res.port_list_del);
		data->res.port_list_del = NULL;
	}

	if (data->controller_list_add) {
		list_delete(data->controller_list_add);
		data->res.port_list_del = NULL;
	}

	if (data->controller_list_del) {

		list_delete(data->controller_list_del);
		data->res.port_list_del = NULL;
	}

	free(data->dpname);
	free(data);
}

static void
handle_ports(void *list)
{
	if (NULL != list) {

		// handle ports
		struct port *p;
		while ((p = list_pop_head(list))) {

			nc_verb_verbose("dpid=%lx port %s with op=%u\n", p->dpid, p->resource_id, p->op);
			if (ADD == p->op) {
				// attach port
				port_attach(ofc_state.xmp_client_handle, p->dpid, p->resource_id);
			} else if (DELETE == p->op) {
				// detach port
				port_detach(ofc_state.xmp_client_handle, p->dpid, p->resource_id);
			} else {
				assert(0);
			}

			xmlFree(p->resource_id);
			free(p);
		}
	}
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	int rv = EXIT_SUCCESS;
	if (NULL != node->children) {

		assert(ofc_state.lsi_list);
		assert(XMLDIFF_CHAIN & op);

		if ((XMLDIFF_ADD|XMLDIFF_REM|XMLDIFF_MOD|XMLDIFF_CHAIN) & op) {

			struct lsi *current_lsi;
			while ( (current_lsi = list_next(ofc_state.lsi_list)) ) {
				handle_ports(current_lsi->res.port_list_del);
			}

			while ( (current_lsi = list_pop_head(ofc_state.lsi_list)) ) {
				handle_ports(current_lsi->res.port_list_add);
				lsi_cleanup(current_lsi);
			}

		} else {
			nc_verb_error("unsupported op");
			assert(0);
		}

		list_delete(ofc_state.lsi_list);
		ofc_state.lsi_list = NULL;
	}

	return rv;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	if (NULL == ofc_state.lsi_list) {
		ofc_state.lsi_list = list_new();
		assert(ofc_state.lsi_list);
	}

	assert(data);
	assert(*data);

	if (!(XMLDIFF_REM & op)) {
		list_append_data(ofc_state.lsi_list, *data);
	}

	int rv = EXIT_SUCCESS;
	if (XMLDIFF_ADD & op) {
		assert(XMLDIFF_CHAIN & op);

		nc_verb_verbose("create new lsi (dpid=%lu, name=%s)\n", LSI(data)->dpid, LSI(data)->dpname);
		if (lsi_create(ofc_state.xmp_client_handle, *data)) {
			rv = EXIT_FAILURE;
		}

	} else if (XMLDIFF_REM& op) {
		assert(XMLDIFF_CHAIN & op);

		nc_verb_verbose("destroy lsi (dpid=%lu, name=%s)\n", LSI(data)->dpid, LSI(data)->dpname);
		if ( lsi_destroy(ofc_state.xmp_client_handle, LSI(data)->dpid) ) {
			rv = EXIT_FAILURE;
		}

		// cannot have a port add during a lsi destroy
		assert(NULL == LSI(data)->res.port_list_add);

		// check if there were ports attached, then clean the list, because detachment takes place during lsi destruction
		if (NULL != LSI(data)->res.port_list_del) {
			struct port *p;
			while ((p = list_pop_head(LSI(data)->res.port_list_del))) {
				xmlFree(p->resource_id);
				free(p);
			}
		}

		// no need to deal with controllers seperately here

		lsi_cleanup(*data);


	} else if (XMLDIFF_MOD & op) {
		// direct sub elements changed
		nc_verb_error("not implemented XMLDIFF_MOD");
		assert(0);
	} else if (XMLDIFF_CHAIN & op) {
		// resources or controllers changed (attachment of ports handled in parent)

		nc_verb_verbose("XMLDIFF_CHAIN\n");

		// check dpid
		if (0 == LSI(data)->dpid) {
			xmlNodePtr tmp = find_element(BAD_CAST "datapath-id",  node->children);
			assert(tmp);
			uint64_t dpid = parse_dpid(tmp->children->content);

			if (LSI(data)->dpid != dpid) {
				LSI(data)->dpid = dpid;
			}
		}

		if (LSI(data)->controller_list_add) {
			lsi_connect_to_controller(ofc_state.xmp_client_handle, LSI(data));
		}

		if (LSI(data)->controller_list_del) {
			// fixme implement
			// assert(0);
		}

	} else {
		nc_verb_error("unsupported op");
		assert(0);
	}

	*data = NULL;

	return rv;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(data);
	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if ((XMLDIFF_ADD|XMLDIFF_REM) & op) {

		xmlChar* text = xmlNodeListGetString(node->doc, node->children, 1);
		LSI(data)->dpname = strdup(text);

		xmlFree(text);
	} else {
		// todo add operation to modify dpid ?
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:datapath-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_datapath_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(NULL == __data);
	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if ((XMLDIFF_ADD|XMLDIFF_REM) & op) {
		xmlChar* text = xmlNodeListGetString(node->doc, node->children, 1);
		uint64_t dpid = parse_dpid(text);
		xmlFree(text);

		LSI(data)->dpid = dpid;
	} else {
		// todo add operation to modify dpid
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	nc_verb_verbose("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if ((XMLDIFF_ADD) & op) {
		if (NULL == LSI(data)->controller_list_add) {
			LSI(data)->controller_list_add = list_new();
			assert(LSI(data)->controller_list_add);
		}

		list_append_data(LSI(data)->controller_list_add, __data);
		__data = NULL;

	} else if ((XMLDIFF_REM) & op) {
		if (NULL == LSI(data)->controller_list_del) {
			LSI(data)->controller_list_del = list_new();
			assert(LSI(data)->controller_list_del);
		}

		list_append_data(LSI(data)->controller_list_del, __data);
		__data = NULL;
	} else {
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);

	assert(NULL == __data);

	if ((XMLDIFF_ADD|XMLDIFF_REM) & op) {
		__data = calloc(1, sizeof(struct controller));

		CONTROLLER(__data)->id = strdup(XML_GET_CONTENT(node->children));
		assert(CONTROLLER(__data)->id);

	} else {
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:ip-address changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_ip_address (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(__data);

	if ((XMLDIFF_ADD) & op) {

		// fixme handle zone in address (see http://www.netconfcentral.org/modules/ietf-inet-types)
		CONTROLLER(__data)->ip_domain = parse_ip_address(XML_GET_CONTENT(node->children), &CONTROLLER(__data)->ip);

	} else if ((XMLDIFF_REM) & op) {
	} else {
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);

	assert(__data);

	if ((XMLDIFF_ADD) & op) {

		CONTROLLER(__data)->port = strtoul(XML_GET_CONTENT(node->children), NULL, 10);

	} else if ((XMLDIFF_REM) & op) {
	} else {
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:protocol changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_protocol (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);

	assert(__data);

	if ((XMLDIFF_ADD) & op) {

		// enum "tcp";
		// enum "tls";
		CONTROLLER(__data)->proto = strdup(XML_GET_CONTENT(node->children));

	} else if ((XMLDIFF_REM) & op) {
	} else {
		nc_verb_error("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);

	print_element_names(node, 0);

	// retrieve the dpid of this twig
	xmlNodePtr tmp = find_element(BAD_CAST "datapath-id",  node->parent->parent->children);
	assert(tmp);
	uint64_t dpid = parse_dpid(tmp->children->content);

	int rv = EXIT_SUCCESS;

	// fixme depending on the erropt (e.g. NC_EDIT_ERROPT_ROLLBACK we might still have *data set)
	assert(data);
	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if (XMLDIFF_ADD & op) {

		xmlChar buf[255];
		// check if port is already attached
		xmlStrPrintf(buf, sizeof(buf), "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:port[text()='%s']", XML_GET_CONTENT(node->children));
		xmlXPathObjectPtr xpath_obj_ptr = get_node(node->doc, namespace_mapping, buf);
		assert(xpath_obj_ptr);
		assert(xpath_obj_ptr->nodesetval);

		if (1 == xpath_obj_ptr->nodesetval->nodeNr) {

			if (NULL == LSI(data)->res.port_list_add) {
				LSI(data)->res.port_list_add = list_new();
				assert(((struct lsi* )*data)->res.port_list_add);
			}
			struct port *p = calloc(1, sizeof(struct port));
			p->resource_id = xmlNodeListGetString(node->doc, node->children, 1);
			p->op = ADD;
			p->dpid = dpid;
			list_append_data(LSI(data)->res.port_list_add, p);

			nc_verb_verbose("added to list: dpid=%lx port %s with op=%u\n", p->dpid, p->resource_id, p->op);
		} else {
			// nodeNr > 1 ==> already attached port
			nc_verb_verbose("attachment failed dpid=%lx port %s: port already attached.\n", dpid, XML_GET_CONTENT(node->children));
			rv = EXIT_FAILURE;
		}

		xmlXPathFreeObject(xpath_obj_ptr);

	} else if (XMLDIFF_REM & op) {

		if (NULL == LSI(data)->res.port_list_del) {
			LSI(data)->res.port_list_del = list_new();
			assert(((struct lsi* ) *data)->res.port_list_del);
		}
		struct port *p = calloc(1, sizeof(struct port));
		p->resource_id = xmlNodeListGetString(node->doc, node->children, 1);
		p->op = DELETE;
		p->dpid = dpid;
		list_append_data(LSI(data)->res.port_list_del, p);

		nc_verb_verbose("added to list: dpid=%lx port %s with op=%u\n", p->dpid, p->resource_id, p->op);

	} else {
		// todo implement
		nc_verb_error("not implemented");
		assert(0);
	}

	return rv;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/xdpd-mgmt:cross-connections/xdpd-mgmt:cross-connection changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_xdpd_mgmt_cross_connections_xdpd_mgmt_cross_connection (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	nc_verb_verbose("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	uint64_t dpid_1 = 0;
	uint64_t dpid_2 = 0;
	uint64_t port_no1 = 0;
	uint64_t port_no2 = 0;

	if (XMLDIFF_ADD & op) {
		int i=0;
		xmlNodePtr lsi;
		for (lsi = node->children->next; NULL != lsi; lsi = lsi->next, ++i) {
			assert(xmlStrEqual(lsi->name, BAD_CAST "switch"));

			// resolve dpid
			char buf[255];
			xmlStrPrintf(buf, sizeof(buf), "/ofc:capable-switch/ofc:logical-switches/ofc:switch[ofc:id='%s']", XML_GET_CONTENT(lsi->children->children));
			xmlXPathObjectPtr xpath_obj_ptr = get_node(lsi->doc, namespace_mapping, buf);
			assert(xpath_obj_ptr);
			assert(xpath_obj_ptr->nodesetval);


			// there can only be one lsi with this id
			if (1 == xpath_obj_ptr->nodesetval->nodeNr) {

				xmlNodePtr dpid_node = find_element(BAD_CAST "datapath-id", xpath_obj_ptr->nodesetval->nodeTab[0]->children);
				assert(dpid_node);

				if (0 == i) {
					dpid_1 = parse_dpid(XML_GET_CONTENT(dpid_node->children));
				} else {
					dpid_2 = parse_dpid(XML_GET_CONTENT(dpid_node->children));
				}

			} else {
				// otherwise something is really screwed
				assert(0);
			}
			xmlXPathFreeObject(xpath_obj_ptr);

			xmlNodePtr requested_portnum = find_element(BAD_CAST "requested-of-port-number", lsi->children);
			if (NULL != requested_portnum) {

				if (0 == i) {
					port_no1 = strtoul(XML_GET_CONTENT(requested_portnum->children), NULL, 10);
				} else {
					port_no2 = strtoul(XML_GET_CONTENT(requested_portnum->children), NULL, 10);
				}
			}
		}

		nc_verb_verbose("dpid_1 = %lx, dpid_2 = %lx\n", dpid_1, dpid_2);
		lsi_cross_connect(ofc_state.xmp_client_handle, dpid_1, port_no1, dpid_2, port_no2);
	}

	return EXIT_SUCCESS;
}

/*
* Structure transapi_config_callbacks provide mapping between callback and path in configuration datastore.
* It is used by libnetconf library to decide which callbacks will be run.
* DO NOT alter this structure
*/
struct transapi_data_callbacks clbks =  {
	.callbacks_count = 21,
	.data = NULL,
	.callbacks = {
		{.path = "/ofc:capable-switch", .func = callback_ofc_capable_switch},
		{.path = "/ofc:capable-switch/ofc:id", .func = callback_ofc_capable_switch_ofc_id},
		{.path = "/ofc:capable-switch/ofc:configuration-points", .func = callback_ofc_capable_switch_ofc_configuration_points},
		{.path = "/ofc:capable-switch/ofc:resources", .func = callback_ofc_capable_switch_ofc_resources},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port", .func = callback_ofc_capable_switch_ofc_resources_ofc_port},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_resource_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:admin-state", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_admin_state},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features},
		{.path = "/ofc:capable-switch/ofc:logical-switches", .func = callback_ofc_capable_switch_ofc_logical_switches},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:id", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_id},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:datapath-id", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_datapath_id},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:id", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_id},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:ip-address", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_ip_address},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:port", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_port},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:protocol", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_protocol},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:port", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_port},
		{.path = "/ofc:capable-switch/xdpd-mgmt:cross-connections/xdpd-mgmt:cross-connection", .func = callback_ofc_capable_switch_xdpd_mgmt_cross_connections_xdpd_mgmt_cross_connection}
	}
};

/*
* RPC callbacks
* Here follows set of callback functions run every time RPC specific for this device arrives.
* You can safely modify the bodies of all function as well as add new functions for better lucidity of code.
* Every function takes array of inputs as an argument. On few first lines they are assigned to named variables. Avoid accessing the array directly.
* If input was not set in RPC message argument in set to NULL.
*/

/*
* Structure transapi_rpc_callbacks provide mapping between callbacks and RPC messages.
* It is used by libnetconf library to decide which callbacks will be run when RPC arrives.
* DO NOT alter this structure
*/
struct transapi_rpc_callbacks rpc_clbks = {
	.callbacks_count = 0,
	.callbacks = {
	}
};

