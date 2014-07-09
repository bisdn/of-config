/*
 * This is automatically generated callbacks file
 * It contains 3 parts: Configuration callbacks, RPC callbacks and state data callbacks.
 * Do NOT alter function signatures or any structures unless you know exactly what you are doing.
 */

#include <stdlib.h>
#include <libxml/tree.h>
#include <libnetconf_xml.h>

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


static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}

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
	printf("init done\n");
	return EXIT_SUCCESS;
}

/**
 * @brief Free all resources allocated on plugin runtime and prepare plugin for removal.
 */
void transapi_close(void)
{
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
	return(NULL);
}
/*
 * Mapping prefixes with namespaces.
 * Do NOT modify this structure!
 */
struct ns_pair namespace_mapping[] = {{"ofconfig", "urn:onf:of111:config:yang"}, {NULL, NULL}};

/*
* CONFIGURATION callbacks
* Here follows set of callback functions run every time some change in associated part of running datastore occurs.
* You can safely modify the bodies of all function as well as add new functions for better lucidity of code.
*/

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);

	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:configuration-points/ofconfig:configuration-point changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_configuration_points_ofconfig_configuration_point (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:resources/ofconfig:port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:resources/ofconfig:queue changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_queue (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:resources/ofconfig:owned-certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_owned_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:resources/ofconfig:external-certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_external_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:resources/ofconfig:flow-table changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_flow_table (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:capabilities changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_capabilities (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:datapath-id changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_datapath_id (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:enabled changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_enabled (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:check-controller-certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_check_controller_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:lost-connection-behavior changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_lost_connection_behavior (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:controllers/ofconfig:controller changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_controllers_ofconfig_controller (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:port changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_port (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:queue changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_queue (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:certificate changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_certificate (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/**
 * @brief This callback will be run when node in path /ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:flow-table changes
 *
 * @param[in] data	Double pointer to void. Its passed to every callback. You can share data using it.
 * @param[in] op	Observed change in path. XMLDIFF_OP type.
 * @param[in] node	Modified node. if op == XMLDIFF_REM its copy of node removed.
 * @param[out] error	If callback fails, it can return libnetconf error structure with a failure description.
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
/* !DO NOT ALTER FUNCTION SIGNATURE! */
int callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_flow_table (void ** data, XMLDIFF_OP op, xmlNodePtr node, struct nc_err** error)
{
	printf("%s: op=%d\n", __FUNCTION__, op);
	print_element_names(node);
	return EXIT_SUCCESS;
}

/*
* Structure transapi_config_callbacks provide mapping between callback and path in configuration datastore.
* It is used by libnetconf library to decide which callbacks will be run.
* DO NOT alter this structure
*/
struct transapi_data_callbacks clbks =  {
	.callbacks_count = 20,
	.data = NULL,
	.callbacks = {
		{.path = "/ofconfig:capable-switch/ofconfig:id", .func = callback_ofconfig_capable_switch_ofconfig_id},
		{.path = "/ofconfig:capable-switch/ofconfig:configuration-points/ofconfig:configuration-point", .func = callback_ofconfig_capable_switch_ofconfig_configuration_points_ofconfig_configuration_point},
		{.path = "/ofconfig:capable-switch/ofconfig:resources/ofconfig:port", .func = callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_port},
		{.path = "/ofconfig:capable-switch/ofconfig:resources/ofconfig:queue", .func = callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_queue},
		{.path = "/ofconfig:capable-switch/ofconfig:resources/ofconfig:owned-certificate", .func = callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_owned_certificate},
		{.path = "/ofconfig:capable-switch/ofconfig:resources/ofconfig:external-certificate", .func = callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_external_certificate},
		{.path = "/ofconfig:capable-switch/ofconfig:resources/ofconfig:flow-table", .func = callback_ofconfig_capable_switch_ofconfig_resources_ofconfig_flow_table},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:id", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_id},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:capabilities", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_capabilities},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:datapath-id", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_datapath_id},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:enabled", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_enabled},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:check-controller-certificate", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_check_controller_certificate},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:lost-connection-behavior", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_lost_connection_behavior},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:controllers/ofconfig:controller", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_controllers_ofconfig_controller},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:port", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_port},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:queue", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_queue},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:certificate", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_certificate},
		{.path = "/ofconfig:capable-switch/ofconfig:logical-switches/ofconfig:switch/ofconfig:resources/ofconfig:flow-table", .func = callback_ofconfig_capable_switch_ofconfig_logical_switches_ofconfig_switch_ofconfig_resources_ofconfig_flow_table}
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

