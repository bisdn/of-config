/*
 * xml_helper.h
 *
 *  Created on: Jul 25, 2014
 *      Author: tobi
 */

#ifndef XML_HELPER_H_
#define XML_HELPER_H_

#include <libxml/xpath.h>
#include <libnetconf/transapi.h>

#ifdef  __cplusplus
extern "C" {
#endif

xmlXPathObjectPtr
get_node(xmlDocPtr doc, const struct ns_pair *namespace_mapping, xmlChar *xpath);

xmlNodePtr
find_element(xmlChar* name, xmlNodePtr node);

uint64_t
parse_dpid(xmlChar* text);

uint64_t
parse_dpid_of_node(xmlNodePtr node);

void
print_dpid(const uint64_t dpid, xmlChar* buf, const unsigned int buf_len);

#ifdef  __cplusplus
}
#endif

#endif /* XML_HELPER_H_ */
