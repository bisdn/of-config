/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

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
find_element(const xmlChar* name, xmlNodePtr node);

uint64_t
parse_dpid(const xmlChar* text);

uint64_t
parse_dpid_of_node(const xmlNodePtr node);

void
print_dpid(const uint64_t dpid, xmlChar* buf, const unsigned int buf_len);

#ifdef  __cplusplus
}
#endif

#endif /* XML_HELPER_H_ */
