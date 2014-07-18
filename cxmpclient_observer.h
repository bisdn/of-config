/*
 * cxmpclient_observer.h
 *
 *  Created on: Jul 17, 2014
 *      Author: tobi
 */

#ifndef CXMPCLIENT_OBSERVER_H_
#define CXMPCLIENT_OBSERVER_H_

#include "xdpd/xmp/client/cxmpobserver.h"
#include "xdpd/xmp/cxmpmsg.h"

#include <pthread.h>

class cxmpclient_observer : public xdpd::mgmt::protocol::cxmpobserver {
public:
	cxmpclient_observer(pthread_mutex_t *mutex, pthread_cond_t *cv);

	virtual ~cxmpclient_observer();

	virtual void
	notify(const xdpd::mgmt::protocol::cxmpmsg &msg);

	xdpd::mgmt::protocol::cxmpmsg&
	get_msg();

private:
	xdpd::mgmt::protocol::cxmpmsg *msg;
	pthread_mutex_t *mutex;
	pthread_cond_t *cv;
};

#endif /* CXMPCLIENT_OBSERVER_H_ */
