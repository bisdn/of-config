/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef UTILS_H_
#define UTILS_H_

/**
 *
 * @param buffer
 * @param out
 * @return
 */
int
parse_ip_address(const char *buffer, void **out);

#endif /* UTILS_H_ */
