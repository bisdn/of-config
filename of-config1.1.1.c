/*
 * This is automatically generated callbacks file
 * It contains 3 parts: Configuration callbacks, RPC callbacks and state data callbacks.
 * Do NOT alter function signatures or any structures unless you know exactly what you are doing.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/tree.h>
#include <libnetconf_xml.h>

#include "xml_helper.h"
#include "cxmpclient_wrapper.h"

extern struct ns_pair namespace_mapping[];

/* transAPI version which must be compatible with libnetconf */
int transapi_version = 4;

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
} ofc_state;

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

	puts("create running config");

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

	printf("init done\n");
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

	printf("closed\n");
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
	printf("get_state_data\n");

	printf("erropt=%u\n", erropt);

	xmlDocPtr state;
	xmlNodePtr root;
	xmlNsPtr ns;

	if (running) {
		puts("running config:");
		xmlSaveFormatFileEnc("-", running, "UTF-8", 1);
	} else {
		puts("no running config");
	}

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



	if (state) {
		puts("current state:");
		xmlSaveFormatFileEnc("-", state, "UTF-8", 1);
	} else {
		puts("no state");
	}
	return state;
}
/*
 * Mapping prefixes with namespaces.
 * Do NOT modify this structure!
 */
struct ns_pair namespace_mapping[] = {{"ofc", "urn:onf:of111:config:yang"}, {NULL, NULL}};

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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);

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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(0 == strcmp(node->name, "id"));
	assert(node);
	assert(node->doc);
	assert(node->children);

	if ((XMLDIFF_ADD|XMLDIFF_MOD) & op) {

		xmlChar* text = xmlNodeListGetString(node->doc, node->children, 1);

		printf("got string: \n%s\n", text);

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
		printf("%s: unsupported operation %u\n", __PRETTY_FUNCTION__, op);
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:configuration-points/ofc:configuration-point changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:configuration-points/ofc:configuration-point/ofc:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point_ofc_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:configuration-points/ofc:configuration-point/ofc:uri changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point_ofc_uri (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:configuration-points/ofc:configuration-point/ofc:protocol changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point_ofc_protocol (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:no-receive changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_no_receive (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:no-forward changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_no_forward (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:no-packet-in changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_no_packet_in (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:rate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_rate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:auto-negotiate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_auto_negotiate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:medium changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_medium (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:pause changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_pause (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v4-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v4_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v6-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v6_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:mac-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_mac_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v4-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v4_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v6-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v6_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:mac-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_mac_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:checksum-present changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_checksum_present (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:key-present changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_key_present (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:key changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_key (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:sequence-number-present changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_sequence_number_present (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v4-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v4_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v6-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v6_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:mac-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_mac_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:vni-valid changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_vni_valid (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:vni changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_vni (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:vni-multicast-group changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_vni_multicast_group (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:udp-source-port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_udp_source_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:udp-dest-port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_udp_dest_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:udp-checksum changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_udp_checksum (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v4-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v4_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v6-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v6_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:mac-endpoints changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_mac_endpoints (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:tni changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_tni (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:tni-resv changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_tni_resv (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:tni-multicast-group changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_tni_multicast_group (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:resource-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_resource_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:min-rate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties_ofc_min_rate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:max-rate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties_ofc_max_rate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:experimenter changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties_ofc_experimenter (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:resource-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_resource_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:P changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_P (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:Q changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_Q (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:J changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_J (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:G changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_G (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:Y changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_Y (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:Seed changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_Seed (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:PgenCounter changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_PgenCounter (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa/ofc:RSAKeyValue changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa_ofc_RSAKeyValue (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa/ofc:RSAKeyValue/ofc:Modulus changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa_ofc_RSAKeyValue_ofc_Modulus (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa/ofc:RSAKeyValue/ofc:Exponent changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa_ofc_RSAKeyValue_ofc_Exponent (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:external-certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_external_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:external-certificate/ofc:resource-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_external_certificate_ofc_resource_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:external-certificate/ofc:certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_external_certificate_ofc_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:resource-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_resource_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:max-entries changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_max_entries (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:next-tables changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_next_tables (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:next-tables/ofc:table-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_next_tables_ofc_table_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:instructions changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_instructions (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:instructions/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_instructions_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:matches changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_matches (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:matches/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_matches_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-actions changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_actions (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-actions/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_actions_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-actions changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_actions (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-actions/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_actions_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-setfields changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_setfields (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-setfields/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_setfields_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-setfields changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_setfields (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-setfields/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_setfields_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:wildcards changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_wildcards (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:wildcards/ofc:type changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_wildcards_ofc_type (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:metadata-match changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_metadata_match (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:metadata-write changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_metadata_write (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

static void
lsi_cleanup(struct lsi *data)
{
	if (data->res.port_list) {
		list_delete(data->res.port_list);
		data->res.port_list = NULL;
	}

	free(data->dpname);
	free(data);
}

static void
handle_ports(void *list, xmlNodePtr sw)
{
	// fixme should be changed to first detach and then attach ports
	if (NULL != list) {

		uint64_t dpid = parse_dpid_of_node(find_element(BAD_CAST "datapath-id", sw->children)->children);

		// handle ports
		struct node *n;
		while ((n = list_get_head(list))) {
			list_pop_head(list);

			printf("dpid=%lx port %s with op=%u\n", dpid, ((struct port*) n->data)->resource_id, ((struct port*) n->data)->op);
			if (ADD == ((struct port*) n->data)->op) {
				// attach port
				attach_port(ofc_state.xmp_client_handle, dpid, ((struct port*) n->data)->resource_id);

			} else if (DELETE == ((struct port*) n->data)->op) {
				// detach port
				detach_port(ofc_state.xmp_client_handle, dpid, ((struct port*) n->data)->resource_id);
			} else {
				assert(0);
			}

			xmlFree(((struct port*) n->data)->resource_id);
			node_delete(n);
		}
	}
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	int rv = EXIT_SUCCESS;
	if (XMLDIFF_ADD & op) {
		assert(data);
		assert(*data);
		assert(XMLDIFF_CHAIN & op);

		// todo improve lsi creation (currently the controller cannot be configured)
		printf("create new lsi (dpid=%lu, name=%s)\n", ((struct lsi*) *data)->dpid, ((struct lsi*) *data)->dpname);
		if (create_lsi(ofc_state.xmp_client_handle, *data)) {
			rv = EXIT_FAILURE;
		}

		// handle ports
		handle_ports(((struct lsi*) *data)->res.port_list, node);

	} else if (XMLDIFF_REM& op) {
		assert(data);
		assert(*data);
		assert(XMLDIFF_CHAIN & op);

		// handle ports
		handle_ports(((struct lsi*) *data)->res.port_list, node);

		printf("destroy lsi (dpid=%lu, name=%s)\n", ((struct lsi*) *data)->dpid, ((struct lsi*) *data)->dpname);
		if ( destroy_lsi(ofc_state.xmp_client_handle, ((struct lsi*) *data)->dpid) ) {
			rv = EXIT_FAILURE;
		}
	} else if (XMLDIFF_MOD & op) {
		// direct sub elements changed
		puts("not implemented XMLDIFF_MOD");
		assert(0);
	} else if (XMLDIFF_CHAIN & op) {
		// resources or controllers changed

		// handle ports
		handle_ports(((struct lsi*) *data)->res.port_list, node);
	} else {
		puts("unsupported op");
		assert(0);
	}

	if (*data) {
		// free data
		lsi_cleanup((struct lsi*) *data);
		*data = NULL;
	}

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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(data);
	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if ((XMLDIFF_ADD|XMLDIFF_REM) & op) {

		xmlChar* text = xmlNodeListGetString(node->doc, node->children, 1);
		((struct lsi*) *data)->dpname = strdup(text);

		xmlFree(text);
	} else {
		// todo add operation to modify dpid ?
		puts("not implemented");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	assert(data);
	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if ((XMLDIFF_ADD|XMLDIFF_REM) & op) {
		xmlChar* text = xmlNodeListGetString(node->doc, node->children, 1);
		uint64_t dpid = parse_dpid(text);
		xmlFree(text);

		((struct lsi*) *data)->dpid = dpid;
	} else {
		// todo add operation to modify dpid
		puts("not implemented");
		assert(0);
	}

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:enabled changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_enabled (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:check-controller-certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_check_controller_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:lost-connection-behavior changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_lost_connection_behavior (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:role changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_role (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:local-ip-address changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_local_ip_address (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:local-port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_local_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored");
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	puts("currently ignored"); // todo check if this could be removed
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
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);

	int rv = EXIT_SUCCESS;

	assert(data);
	if (NULL == *data) {
		*data = calloc(1, sizeof(struct lsi));
		assert(*data);
	}

	if (XMLDIFF_ADD & op) {
		if (NULL == ((struct lsi*) *data)->res.port_list) {
			((struct lsi*) *data)->res.port_list = list_new();
			assert(((struct lsi* )*data)->res.port_list);
		}
		struct port *p = calloc(1, sizeof(struct port));
		p->op = ADD;
		p->resource_id = xmlNodeListGetString(node->doc, node->children, 1);
		list_append_data(((struct lsi*) *data)->res.port_list, p);

	} else if (XMLDIFF_REM & op) {
		if (NULL == ((struct lsi*) *data)->res.port_list) {
			((struct lsi*) *data)->res.port_list = list_new();
			assert(((struct lsi* ) *data)->res.port_list);
		}
		struct port *p = calloc(1, sizeof(struct port));
		p->op = DELETE;
		p->resource_id = xmlNodeListGetString(node->doc, node->children, 1);
		list_append_data(((struct lsi*) *data)->res.port_list, p);

	} else {
		// todo implement
		puts("not implemented");
		assert(0);
	}

	return rv;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:queue changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_queue (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:flow-table changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_flow_table (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: data=%p, op=%d\n", __PRETTY_FUNCTION__, data, op);
	print_element_names(node, 0);
	return EXIT_SUCCESS;
}

/*
* Structure transapi_config_callbacks provide mapping between callback and path in configuration datastore.
* It is used by libnetconf library to decide which callbacks will be run.
* DO NOT alter this structure
*/
struct transapi_data_callbacks clbks =  {
	.callbacks_count = 154,
	.data = NULL,
	.callbacks = {
		{.path = "/ofc:capable-switch", .func = callback_ofc_capable_switch},
		{.path = "/ofc:capable-switch/ofc:id", .func = callback_ofc_capable_switch_ofc_id},
		{.path = "/ofc:capable-switch/ofc:configuration-points", .func = callback_ofc_capable_switch_ofc_configuration_points},
		{.path = "/ofc:capable-switch/ofc:configuration-points/ofc:configuration-point", .func = callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point},
		{.path = "/ofc:capable-switch/ofc:configuration-points/ofc:configuration-point/ofc:id", .func = callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point_ofc_id},
		{.path = "/ofc:capable-switch/ofc:configuration-points/ofc:configuration-point/ofc:uri", .func = callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point_ofc_uri},
		{.path = "/ofc:capable-switch/ofc:configuration-points/ofc:configuration-point/ofc:protocol", .func = callback_ofc_capable_switch_ofc_configuration_points_ofc_configuration_point_ofc_protocol},
		{.path = "/ofc:capable-switch/ofc:resources", .func = callback_ofc_capable_switch_ofc_resources},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port", .func = callback_ofc_capable_switch_ofc_resources_ofc_port},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_resource_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:admin-state", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_admin_state},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:no-receive", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_no_receive},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:no-forward", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_no_forward},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:configuration/ofc:no-packet-in", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_configuration_ofc_no_packet_in},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:rate", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_rate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:auto-negotiate", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_auto_negotiate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:medium", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_medium},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:pause", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_features_ofc_advertised_ofc_pause},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v4-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v4_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v6-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v6_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:mac-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_mac_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:tunnel/ofc:tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_tunnel_ofc_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v4-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v4_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v6-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v6_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:mac-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_mac_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:checksum-present", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_checksum_present},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:key-present", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_key_present},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:key", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_key},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:ipgre-tunnel/ofc:ipgre-tunnel/ofc:sequence-number-present", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_ipgre_tunnel_ofc_ipgre_tunnel_ofc_sequence_number_present},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v4-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v4_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v6-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v6_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:mac-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_mac_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:vni-valid", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_vni_valid},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:vni", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_vni},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:vni-multicast-group", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_vni_multicast_group},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:udp-source-port", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_udp_source_port},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:udp-dest-port", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_udp_dest_port},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:vxlan-tunnel/ofc:vxlan-tunnel/ofc:udp-checksum", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_vxlan_tunnel_ofc_vxlan_tunnel_ofc_udp_checksum},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v4-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v4_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:local-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_local_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v4-endpoints/ofc:remote-endpoint-ipv4-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v4_endpoints_ofc_remote_endpoint_ipv4_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v6-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v6_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:local-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_local_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:v6-endpoints/ofc:remote-endpoint-ipv6-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_v6_endpoints_ofc_remote_endpoint_ipv6_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:mac-endpoints", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_mac_endpoints},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:local-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_local_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:endpoints/ofc:mac-endpoints/ofc:remote-endpoint-mac-adress", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_endpoints_ofc_mac_endpoints_ofc_remote_endpoint_mac_adress},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:tni", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_tni},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:tni-resv", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_tni_resv},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel-type/ofc:nvgre-tunnel/ofc:nvgre-tunnel/ofc:tni-multicast-group", .func = callback_ofc_capable_switch_ofc_resources_ofc_port_ofc_tunnel_type_ofc_nvgre_tunnel_ofc_nvgre_tunnel_ofc_tni_multicast_group},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:resource-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_resource_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:id", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:port", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_port},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:min-rate", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties_ofc_min_rate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:max-rate", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties_ofc_max_rate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:experimenter", .func = callback_ofc_capable_switch_ofc_resources_ofc_queue_ofc_properties_ofc_experimenter},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:resource-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_resource_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:certificate", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_certificate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:P", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_P},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:Q", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_Q},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:J", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_J},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:G", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_G},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:Y", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_Y},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:Seed", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_Seed},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:dsa/ofc:DSAKeyValue/ofc:PgenCounter", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_dsa_ofc_DSAKeyValue_ofc_PgenCounter},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa/ofc:RSAKeyValue", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa_ofc_RSAKeyValue},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa/ofc:RSAKeyValue/ofc:Modulus", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa_ofc_RSAKeyValue_ofc_Modulus},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key/ofc:key-type/ofc:rsa/ofc:RSAKeyValue/ofc:Exponent", .func = callback_ofc_capable_switch_ofc_resources_ofc_owned_certificate_ofc_private_key_ofc_key_type_ofc_rsa_ofc_RSAKeyValue_ofc_Exponent},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:external-certificate", .func = callback_ofc_capable_switch_ofc_resources_ofc_external_certificate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:external-certificate/ofc:resource-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_external_certificate_ofc_resource_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:external-certificate/ofc:certificate", .func = callback_ofc_capable_switch_ofc_resources_ofc_external_certificate_ofc_certificate},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:resource-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_resource_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:max-entries", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_max_entries},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:next-tables", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_next_tables},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:next-tables/ofc:table-id", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_next_tables_ofc_table_id},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:instructions", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_instructions},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:instructions/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_instructions_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:matches", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_matches},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:matches/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_matches_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-actions", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_actions},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-actions/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_actions_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-actions", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_actions},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-actions/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_actions_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-setfields", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_setfields},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-setfields/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_write_setfields_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-setfields", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_setfields},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-setfields/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_apply_setfields_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:wildcards", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_wildcards},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:wildcards/ofc:type", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_wildcards_ofc_type},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:metadata-match", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_metadata_match},
		{.path = "/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:metadata-write", .func = callback_ofc_capable_switch_ofc_resources_ofc_flow_table_ofc_metadata_write},
		{.path = "/ofc:capable-switch/ofc:logical-switches", .func = callback_ofc_capable_switch_ofc_logical_switches},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:id", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_id},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:datapath-id", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_datapath_id},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:enabled", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_enabled},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:check-controller-certificate", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_check_controller_certificate},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:lost-connection-behavior", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_lost_connection_behavior},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:id", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_id},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:role", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_role},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:ip-address", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_ip_address},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:port", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_port},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:local-ip-address", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_local_ip_address},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:local-port", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_local_port},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:protocol", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_controllers_ofc_controller_ofc_protocol},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:port", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_port},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:queue", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_queue},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:certificate", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_certificate},
		{.path = "/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:flow-table", .func = callback_ofc_capable_switch_ofc_logical_switches_ofc_switch_ofc_resources_ofc_flow_table}
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

