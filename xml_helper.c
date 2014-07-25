/*
 * xml_helper.c
 *
 *  Created on: Jul 25, 2014
 *      Author: tobi
 */

#include "xml_helper.h"
#include <assert.h>

xmlXPathObjectPtr
get_node(xmlDocPtr doc, const struct ns_pair const *namespace_mapping, xmlChar *xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	assert(doc);
	assert(xpath);

	context = xmlXPathNewContext(doc);
	assert(context);

	while(namespace_mapping && namespace_mapping->prefix) {
		xmlXPathRegisterNs(context, namespace_mapping->prefix, namespace_mapping->href);
		++namespace_mapping;
	}

	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	assert(result);
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		return NULL;
	}

	return result;
}
