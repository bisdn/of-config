/*
 * cxmpclient_observer.cc
 *
 *  Created on: Jul 17, 2014
 *      Author: tobi
 */

#include "cxmpclient_observer.h"

cxmpclient_observer::cxmpclient_observer(pthread_mutex_t *mutex,
		pthread_cond_t *cv) :
		msg(NULL), mutex(mutex), cv(cv)
{
	// TODO Auto-generated constructor stub

}

cxmpclient_observer::~cxmpclient_observer()
{
	// TODO Auto-generated destructor stub
}

void
cxmpclient_observer::notify(const xdpd::mgmt::protocol::cxmpmsg& msg)
{
	pthread_mutex_lock(mutex);
	if (this->msg) delete this->msg;
	this->msg = new xdpd::mgmt::protocol::cxmpmsg(msg);
	pthread_cond_signal(cv);
	pthread_mutex_unlock(mutex);
}

const xdpd::mgmt::protocol::cxmpmsg&
cxmpclient_observer::get_msg()
{
	pthread_cond_wait(cv, mutex);

	return *msg;
}
