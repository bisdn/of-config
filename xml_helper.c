/*
 * xml_helper.c
 *
 *  Created on: Jul 25, 2014
 *      Author: tobi
 */

#include "xml_helper.h"
#include <assert.h>
#include <string.h>

xmlXPathObjectPtr
get_node(xmlDocPtr doc, const struct ns_pair const *namespace_mapping, xmlChar *xpath)
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	assert(doc);
	assert(xpath);

	context = xmlXPathNewContext(doc);
	assert(context);

	while (namespace_mapping && namespace_mapping->prefix) {
		xmlXPathRegisterNs(context, namespace_mapping->prefix, namespace_mapping->href);
		++namespace_mapping;
	}

	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	assert(result);
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
		xmlXPathFreeObject(result);
		return NULL;
	}

	return result;
}

void
print_element_names(xmlNode * a_node, int d)
{
	xmlNode *cur_node = NULL;
	if (0 == d) {
		printf("xml subtree:\n");
	}

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			int i;
			for (i = 0; i < d; ++i) {
				printf("\t");
			}
			printf("%s\n", cur_node->name);
		} else if (cur_node->type == XML_TEXT_NODE) {
			int i;
			for (i = 0; i < d; ++i) {
				printf("\t");
			}
			printf("value=%s\n", XML_GET_CONTENT(cur_node));
		}

		print_element_names(cur_node->children, d + 1);
	}
}

xmlNodePtr
find_element(const xmlChar* name, xmlNodePtr node)
{
	assert(name);
	assert(node);

	while (node) {
		if (XML_ELEMENT_NODE == node->type && xmlStrEqual(name, node->name)) {
			break;
		}
		node = node->next;
	}
	return node;
}

uint64_t
parse_dpid(const xmlChar* text)
{
	assert(text);
	xmlChar *mytext = xmlStrdup(text);
	assert(mytext);

	printf("%s: text=%s\n", __PRETTY_FUNCTION__,  text);

	uint64_t dpid = 0;
	unsigned int run = 8;

	char *tok = strtok(mytext, ":");
	while (tok) {
		run--;
		dpid |= strtoul(tok, NULL, 16) << (run * 8);
		tok = strtok(NULL, ":");
	}
	xmlFree(mytext);
	assert(0 == run);

	return dpid;
}

uint64_t
parse_dpid_of_node(const xmlNodePtr node)
{
	assert(node);
	assert(node->children);
	return parse_dpid(XML_GET_CONTENT(node->children));
}

void
print_dpid(const uint64_t dpid, xmlChar* buf, const unsigned int buf_len)
{
	assert(buf);
	assert(buf_len > 24);

	xmlStrPrintf(buf, buf_len, "%x:%x:%x:%x:%x:%x:%x:%x",
			dpid >> 7*8,
			dpid >> 6*8,
			dpid >> 5*8,
			dpid >> 4*8,
			dpid >> 3*8,
			dpid >> 2*8,
			dpid >> 1*8,
			dpid >> 0*8
	);
}
