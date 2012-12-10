/*
	2009 Benjamin Reh
	ich@benjaminreh.de

	simple class for sending char-arrays via serial Line
*/
#ifndef SERIALLINE_H
#define SERIALLINE_H





class SerialLine {
public:
	SerialLine(char* device);
	~SerialLine();
	bool send(char* buffer, int length);
	bool sendchar(char c);
	char readchar();
	bool isValid();
	bool dataWaiting();
	int clearWaitingData();
private:
	int fd;	

};


#endif
