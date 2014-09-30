/*
 * utils.c
 *
 *  Created on: Sep 26, 2014
 *      Author: tobi
 */


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

	printf("try parsing %s\n", buffer);

	int ret = 0;

	if (strchr(buffer, '.')) {
		puts("v4");
		*out = malloc(sizeof(struct in_addr));
		assert(*out);
		inet_pton(AF_INET, buffer, *out);
		ret = AF_INET;
	} else if (strchr(buffer, ':')) {
		puts("v6");
		*out = malloc(sizeof(struct in6_addr));
		assert(*out);
		inet_pton(AF_INET6, buffer, *out);
		ret = AF_INET6;
	} else {
		assert(0);
	}

	return ret;
}
