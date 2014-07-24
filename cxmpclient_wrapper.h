/*
 * cxmpclient_wrapper.h
 *
 *  Created on: Jul 11, 2014
 *      Author: tobi
 */

#ifndef CXMPCLIENT_WRAPPER_H_
#define CXMPCLIENT_WRAPPER_H_

#include <libxml/tree.h>

#ifdef  __cplusplus
extern "C" {
#endif

void*
new_xmp_client();

void
delete_xmp_client(void* handle);

void
get_port_info(void* handle, xmlNodePtr resources);

void
get_lsi_info(void* handle, xmlNodePtr lsis);

#ifdef  __cplusplus
}
#endif



#endif /* CXMPCLIENT_WRAPPER_H_ */
