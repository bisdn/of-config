proc netconf_connect {} {
	global expect_out spawn_id
	
	# read root password from file
	set f [open ".password"]
	set password [read $f]
	close $f
	
	# run netopeer-cli
	spawn /home/tobi/workspace/netopeer/cli/netopeer-cli
	expect {
		"netconf>"	{}
		timeout		{ exit 1 }
	}
	
	# Send the username, and then wait for a password prompt.
	send "connect --login root localhost\r"
	expect {
		"root@localhost password:"	{}
		timeout						{ exit 1 }
	}
	
	# Send the password, and then wait for a shell prompt.
	send "$password\n"
	expect {
		"netconf>"	{} 
		timeout		{ exit 1 }
	}
}

proc netconf_disconnect {} {
	global expect_out spawn_id
	
	# end session
	send "\x0004"
	expect {
		timeout	{ send_user "failed\n" ; exit 1 }
		eof		{ send_user "\n"; exit 0 } 
	}
}
