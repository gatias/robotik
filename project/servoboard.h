/*
	2009 Benjamin Reh
	ich@benjaminreh.de
	class implementing a set of Servos and a connection
	(for details see servo.h)

	2011 Edward-Robert Tyercha
	tyercha1@googlemail.com
	Specification for SCC32
*/
#ifndef SERVOBOARD_H
#define  SERVOBOARD_H
#include "servo.h"
#include "serialline.h"

class Servoboard
{
	public:
	//Constructor. user 'new_nr' servos 
	Servoboard(int new_nr);
	//Destructor. All servos will be switched off 
	~Servoboard();
	//Device, such as "/dev/ttyS0" "/dev/ttyS1" "/dev/ttyUSB0" ...
	bool connect(char* device);
	//closes port
	bool disconnect();

	inline int getNumberOfServos() { return this->nr; }

	//Sends the new values to the servoboard
	//only values that have been changend are transfered
	void update();
	void updateSimultaneously(int timeInMs) ;
	
	inline bool isConnected() { return connected; }

	//Updates in any case, even if the values are unchanged
	//This is usefull, if Servoboard has lost power
	void force_update();
	//Kind of emergency stop
	void all_stop();
	//Acces to the Servos themselves
	Servo* servo;

	// =========== Specific Operations for SCC32 =========

	// All servo channels can be used as regular logical outputs (TTL, ~25mA 	
	bool setDiscreteOutput(int ch, bool value);
	
	// There are four banks that can be used as byte outputs
	// Servo Banks (8bit)	Bank0 = channels 0-7,
	//			Bank1 = 8-15,
	//			Bank2 = 16-23,
	//			Bank3 = 24-31. 	
	bool setByteOutput(int bank, char byte);

	// There are 4 digital/analog debounced inputs, with pullup resitors (~50k).
	// Read and get only a single value; Note: Default all inputs are digital pullups.
	int getInput(int number);
	
	// Return only one latched input = true iff value has not changed from true to false since the last read.
        // Terutns only false if the intput is false and has not changed from false to true since the last read.
	// To reset a latch, call this method once again.
	int getLatchedInput(int number);

	// Read the input as a analog value (0-255 resolution from 0V to 5V)
	// First read contains no valid data since per default they are pullups.
	// All inputs are handeled individually, so it is possible to have combinatinos of analog, pullups and latches.
	unsigned char getAnalogInput(int number);

	// Return all 4 inputs at once
	bool readInputs(bool *A, bool *B, bool *c, bool *D);
	bool readLatchedInputs(bool *A, bool *B, bool *c, bool *D);
	bool readAnalogInputs(unsigned char *A,unsigned char *B,unsigned char *c,unsigned char *D);
	
	private:
	int nr;
	bool connected;
	
	public:
	SerialLine* s;




};
#endif
