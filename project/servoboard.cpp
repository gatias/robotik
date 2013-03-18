/*
	2009 Benjamin Reh
	ich@benjaminreh.de
*/
#include "servoboard.h"
#include <stdint.h>
#include <sstream>
#include <iostream>
using namespace std;

Servoboard::Servoboard(int new_nr)
{
	nr=new_nr;
	connected=false;
	servo=new Servo[nr];
}

Servoboard::~Servoboard()
{
	all_stop();
	disconnect();
	delete[] servo;
}
bool Servoboard::connect(char* device)
{
	if (!(connected))
	{
		s= new SerialLine(device);
		if (!(s->isValid()))
			return false;
		connected=true;
		for (int i=0; i< nr; i++)
			servo[i].init(i, s);
		return true;
	}
	return false;
}
bool Servoboard::disconnect()
{
	if ((connected))
	{
		delete s;
		connected=false;
		return true;
	}
	else
		return false;
}
void Servoboard::force_update()
{
	if (connected)
		for (int i=0; i< nr; i++)
			servo[i].force_update();
}
void Servoboard::update()
{
	if (connected)
		for (int i=0; i< nr; i++)
			servo[i].update();
}

void Servoboard::updateSimultaneously(int timeInMs) {
	if(connected) {
		stringstream toSend;
		for(int i=0; i< nr; i++) {
			float pos = servo[i].getPos();
			toSend << "#" << i << " P" << (int)(pos*2000.0+500.0) << " ";
			servo[i].clear_update();
			//~ string str = toSend.str();
			//~ s->send((char*) str.c_str(), str.length());
		}
		toSend << "T"<< timeInMs << " \n\r";
		string str = toSend.str();
//		std::cout << "tosend=" << str << "|||" << std::endl;
		//if(!s->dataWaiting())
		bool status = s->send(const_cast<char*>(str.c_str()), str.length());
	}
}

void Servoboard::all_stop()
{
	if(connected) {
		stringstream toSend;
		for (int i=0; i< nr; i++) {
			servo[i].power(false);
			toSend.clear();
			toSend << "STOP " << i << " \n\r";
			string str = toSend.str();
		    s->send((char*) str.c_str(), str.length());
		}
	}
}

bool Servoboard::setDiscreteOutput(int ch, bool value) {
	if(!connected || ch < 0 || ch > 32) {
		return 0;
	} 	
	stringstream toSend;
	toSend << "#" << ch;
	if( value ) { 
		toSend << "H\n\r";
	} else {
		toSend << "L\n\r";
	}
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
}


bool Servoboard::setByteOutput(int bank, char byte) {
	if(!connected || bank < 0 || bank > 4) {
		return 0;
	} 	
	stringstream toSend;
	toSend << "#" << bank << ":" << (int)byte << "\n\r";
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
}


int Servoboard::getInput(int number) {
	if(!connected || number < 0 || number > 4) {
		return -1;
	} 
	stringstream toSend;
	toSend << "" << (char)(number+65) << " \n\r";
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
	// wait
	char val = 0;
	if( s->dataWaiting() ) { val = s->readchar(); } else { return -2; }
	return (char)val == '1';
}

int Servoboard::getLatchedInput(int number) {
	if(!connected || number < 0 || number > 4) {
		return -1;
	} 
	stringstream toSend;
	toSend << "" << (char)(number+65) << "L \n\r";
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
	// wait
	char val = 0;
	if( s->dataWaiting() ) { val = s->readchar(); } else { return -2; }
	return (char)val == '1';
}

unsigned char Servoboard::getAnalogInput(int number) {
	if(!connected || number < 0 || number > 4) {
		return 0;
	} 
	stringstream toSend;
	toSend << "V" << (char)(number+65) << " \n\r";
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
	// wait
	unsigned char val = 0;
	if( s->dataWaiting() ) { val = (unsigned char)s->readchar(); } else { return -2; }
	return val;
}

bool Servoboard::readInputs(bool *A, bool *B, bool *C, bool *D) {
	if(!connected || !A || !B || !C || !D) {
		return 0;
	}
	stringstream toSend;
	toSend << "A B C D \n\r";
	string str = toSend.str();

	s->send((char*) str.c_str(), str.length());
	// wait
	if( s->dataWaiting() ) { (*A) = ( s->readchar() == '1' ); } else { return 0; }
	if( s->dataWaiting() ) { (*B) = ( s->readchar() == '1' ); } else { return 0; }
	if( s->dataWaiting() ) { (*C) = ( s->readchar() == '1' ); } else { return 0; }
	if( s->dataWaiting() ) { (*D) = ( s->readchar() == '1' ); } else { return 0; }
	return 1;
}

bool Servoboard::readLatchedInputs(bool *A, bool *B, bool *C, bool *D) {
	if(!connected || !A || !B || !C || !D ) {
		return 0;
	} 
	stringstream toSend;
	toSend << "AL BL CL DL \n\r";
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
	// wait
	if( s->dataWaiting() ) { (*A) = ( s->readchar() == '1' ); } else { return 0; }
	if( s->dataWaiting() ) { (*B) = ( s->readchar() == '1' ); } else { return 0; }
	if( s->dataWaiting() ) { (*C) = ( s->readchar() == '1' ); } else { return 0; }
	if( s->dataWaiting() ) { (*D) = ( s->readchar() == '1' ); } else { return 0; }
	return 1;
}

bool Servoboard::readAnalogInputs(unsigned char *A, unsigned char *B, unsigned char *C, unsigned char *D) {
	if(!connected || !A || !B || !C || !D ) {
		return 0;
	} 
	stringstream toSend;
	toSend << "VA VB VC VD \n\r";
	string str = toSend.str();
	s->send((char*) str.c_str(), str.length());
	// wait
	if( s->dataWaiting() ) { (*A) = (unsigned char)s->readchar(); } else { return 0; }
	if( s->dataWaiting() ) { (*B) = (unsigned char)s->readchar(); } else { return 0; }
	if( s->dataWaiting() ) { (*C) = (unsigned char)s->readchar(); } else { return 0; }
	if( s->dataWaiting() ) { (*D) = (unsigned char)s->readchar(); } else { return 0; }
	return 1;
}

