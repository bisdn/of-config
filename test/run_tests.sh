#!/bin/bash

TESTS_TO_RUN="netconf_get \
 netconf_create_lsi \
 netconf_attach_port \
 netconf_create_lsi2 \
 netconf_attach_port_invalid \
 netconf_attach_ports \
 netconf_detach_port \
 netconf_detach_ports \
 netconf_attach_port \
 netconf_attach_ports \
 netconf_move_ports \
 netconf_get_config"


for TEST in $TESTS_TO_RUN
do
	tput bold
	echo "running $TEST"
	tput sgr0
	
	./$TEST.exp
	[ "0" == "$?" ] || {
		echo "test $TEST failed"
		exit 1
	}
done
