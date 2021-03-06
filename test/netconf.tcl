proc netconf_connect {} {
	global expect_out spawn_id
	
	upvar password passphrase
	
	set password ""
	set passphrase ""
	
	# read root password from file
	if [ file exists ".password" ] {
		set f [open ".password"]
		set password [read $f]
		close $f 
	}
	
	
	# read root passphrase from file
	if [ file exists ".passphrase" ] {
		set f [open ".passphrase"]
		set passphrase [read $f]
		close $f 
	}
		
	# run netopeer-cli
	spawn netopeer-cli
	expect {
		"netconf>"	{}
		timeout		{ exit 1 }
	}
	
	# Send the username, and then wait for a password prompt.
	send "connect --login root localhost\r"
	expect {
		"Enter passphrase for the key" {
			send "$passphrase\n"
			exp_continue
		}
		
		"root@localhost password:"	{
			send "$password\n"
			exp_continue
		}
		"ERROR: Authentication failed."	{ exit 1 }
		"netconf>" {  }
		timeout							{ exit 1 }
	}
	
}

proc netconf_disconnect {} {
	global expect_out spawn_id
	
	# end session
	send "quit\n"
	expect {
		timeout	{ send_user "failed\n" ; exit 1 }
		eof		{ send_user "\n"; exit 0 } 
	}
}

proc netconf_edit_config {xml_file {operation "merge"}} {

	#set f [open $xml_file]
	#set netconf_xml [read $f]
	#close $f
	
	send "edit-config --config=$xml_file --defop=$operation --error=rollback running\r"
	#expect {
	#	"Type the edit configuration data (close editor by Ctrl-D):"	{  }
	#	timeout 	{ exit 1 }
	#}
	
	#send "$netconf_xml"
	#send "\x0004"
}
