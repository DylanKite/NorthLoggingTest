build instructions:

libbsd is requrired in order to compile this program
it is required for the command strlcpy which is a safer version of
strncpy that automatically adds \0 to the end of the string

g++ is used as the compiler

Program was built and tested on a computer running Linux Mint 18.4
and assumes that it will be run on a system with POSIX C support
in order to get the POSIX C socket library to work.


running instructions:
- start the server first
- start the client second
- the client will show a gui with the following 6 options:
	1. send message
	2. read logs
	3. clear logs
	4. change client ID
	5. change log level
	6. exit

bugs:
- currently this program only allows one client at a time. this is due to time constraints.
  with more time each client would have had its own thread spun off that way each client could
  have its own socket with the server.
- commas and spaces can not be in the clients ID, this is due to how information is transmitted
  between the client and the server where a single message is sent over the socket and uses commma
  seperation.
- this program will not takeover a port if it is already in use by another program which can lead to
  issues with the client connecting to the host. currently it uses port 8080. it is defined in the 
  header file logging_server.h and can be changed if needed. NOTE: testing was only done on port 8080
- retreival of the logs can be slow some times
