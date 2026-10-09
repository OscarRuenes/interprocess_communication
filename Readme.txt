Files without .cpp are executable files (excluding echo.log) and can be run using ./<filename>
echo_s requires at least one port number to run, and echo_c allows for a "-u" argument for UDP style of communication (TCP is normal)
Known bugs:
	The first entry in TCP communication does not appear in echo.log
	To access the second port number given in echo_s, you must first have a connection to the first given port number

I'm not sure if these are bugs, but echo_c does not receive anything from netcat and UDP communications through echo_s only support one message

The echo.log shows the date, message, and ip address of the machine which sends the message ONLY if you activate the server by executing ./log_s
