/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "utils.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int
parse_ip_address(const char *buffer, void **out)
{
	assert(buffer);
	assert(out);

	nc_verb_verbose("try parsing %s\n", buffer);

	int ret = 0;

	if (strchr(buffer, '.')) {
		*out = malloc(sizeof(struct in_addr));
		assert(*out);
		inet_pton(AF_INET, buffer, *out);
		ret = AF_INET;
	} else if (strchr(buffer, ':')) {
		*out = malloc(sizeof(struct in6_addr));
		assert(*out);
		inet_pton(AF_INET6, buffer, *out);
		ret = AF_INET6;
	} else {
		assert(0);
	}

	return ret;
}
