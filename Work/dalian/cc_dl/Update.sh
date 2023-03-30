#!/usr/bin/expect -f

set timeout 60
set n [lindex $argv 0]
set userpath [lindex $argv 1]
set db2path [lindex $argv 2]
#set n [exec awk "END{print NR}" /etc/ld.so.conf]
spawn su - root
expect "*:*"
send "111111\r"
expect "*#"

if {$n == 1} {
	send "cp $userpath/.bash_profile .\r"
	expect "cp:*"
	send "y\r"
	expect "*#"
	send ". .bash_profile\r"
	expect "*#"
	send "echo $db2path/lib >> /etc/ld.so.conf\r"
	expect "*#"
	send "ldconfig\r"
	expect "*#"
}
send "cd $userpath/snc\r"
expect "*#"
send "make all\r"
expect "*#"
send "chmod u+s snc\r"
expect "*#"

exit
