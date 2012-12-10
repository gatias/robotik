/*
	2009 Benjamin Reh
	ich@benjaminreh.de
*/
#include "serialline.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <sys/select.h>
#include <strings.h>
#include <iostream>
#include <assert.h>

#include <string>

#include <ncurses.h>

using namespace std;

SerialLine::SerialLine(char* device)
{
	struct termios nconf;
	
	fd = open(device, O_RDWR | O_NOCTTY);	
	bzero(&nconf, sizeof(nconf));
	
	nconf.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	//~ nconf.c_cflag = B9600 | CS8 | CLOCAL | CREAD;


	nconf.c_iflag = IXON;
	nconf.c_oflag = 0;
	nconf.c_lflag = 0; // ICANON;

	nconf.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	nconf.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	nconf.c_cc[VERASE]   = 0;     /* del */
	nconf.c_cc[VKILL]    = 0;     /* @ */
	nconf.c_cc[VEOF]     = 4;     /* Ctrl-d */
	//nconf.c_cc[VSWTC]    = 0;     /* '\0' */
	nconf.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
	nconf.c_cc[VSTOP]    = 0;     /* Ctrl-s */
	nconf.c_cc[VSUSP]    = 0;     /* Ctrl-z */
	nconf.c_cc[VEOL]     = 0;     /* '\0' */
	nconf.c_cc[VREPRINT] = 0;     /* Ctrl-r */
	nconf.c_cc[VDISCARD] = 0;     /* Ctrl-u */
	nconf.c_cc[VWERASE]  = 0;     /* Ctrl-w */
	nconf.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	nconf.c_cc[VEOL2]    = '\0';     /* '\0' */
		
	// read blockiert bis c_cc[MIN] Zeichen empfangen wurden.
	nconf.c_cc[VTIME] = 0; // timeout für read
	nconf.c_cc[VMIN] = 1;
	
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &nconf);	
}
bool SerialLine::isValid()
{
	if (fd<0)
		return false;
	else 
		return true;
}

SerialLine::~SerialLine()
{
	//printw("destruktor");
	//refresh();
	close(fd);
}
bool SerialLine::send(char* buffer, int length)
{
	bool ret=write(fd,buffer,length);
	if (ret)
		return true;
	else
		return false;
}
bool SerialLine::sendchar(char c) {
	char  str[1] = {c};
	return send(str,1);
}

bool SerialLine::dataWaiting() {
	fd_set s;
	struct timeval tv;
	int res = 0;

	FD_ZERO(&s);
	FD_SET(fd, &s);

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	res = select(fd+1, &s, NULL, NULL, &tv);
	return (res == 1) ? true : false;
}
char SerialLine::readchar() {
	unsigned char buf[1];
	int r = read(fd, buf, 1);
	//char str[10];
	//sprintf(str,"<<%i>>",r);
	//printw(str);
	//refresh();
	return buf[0];

}

int SerialLine::clearWaitingData() {
	int count = 0;
	while( dataWaiting() ) {
		readchar();
		++count;
	}
	return count;
}
