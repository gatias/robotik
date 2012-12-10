/*
	2009 Benjamin Reh
	ich@benjaminreh.de
*/
#include "servo.h"
#include <stdint.h>
#include <sstream>


using namespace std;
Servo::Servo()
{
	speed=0.95;
	pos=0.5;
	active=false;
	pos_update=false;
}
void Servo::init(int nr_new, SerialLine* s_new)
{
	pos_update=false;
	nr=nr_new;
	s=s_new;
}
Servo::~Servo(void)
{
}


bool Servo::setPos(float newpos)
{
	if (newpos<0.0 || newpos>1.0)
		return false;
	pos=newpos;
	pos_update=true;
	active=true;
	return true;
}
bool Servo::setSpeed(float newspeed)
{
	if (newspeed<=0.0 || newspeed>1.0)
		return false;
	power(true);
	speed=newspeed;
	speed_update=true;
	return true;
}
float Servo::getSpeed(void)
{
	return speed;
}
float Servo::getPos(void)
{
	return pos;
}
bool Servo::force_update()
{
	speed_update=true;
	pos_update=true;
	update();
}

bool Servo::clear_update() {
	speed_update=false;
	pos_update=false;
}

bool Servo::update()
{
	// if( !pos_update ) { return true; }

	stringstream buffer;
	buffer << "#" << nr << " P" << (int) (pos*2000.0+500.0) << " T"<< (int) ((1.0-speed)*65535) << " \n\r";
	speed_update=false;
	pos_update=false;
	string str = buffer.str();
	s->send((char*) str.c_str(), str.length());
	
	//~ if (speed_update)
	//~ {
		//~ char buffer[5]={ 0x80, 0x01, 0x01, 0x00, 0x00};
		//~ buffer[1]=(nr/8)+1;	//Board-ID
		//~ buffer[3]= nr % 8;	//Servo-Nr
		//~ buffer[4]=(uint8_t) (speed*126.0);
		//~ speed_update=false;
		//~ s->send(buffer, 5);
	//~ }
//~ 
	//~ if (pos_update)
	//~ {
		//~ if (active)
		//~ {
			//~ char buffer[6]={ 0x80, 0x01, 0x04, 0x00, 0xEE, 0xEE};
			//~ buffer[1]=(nr/8)+1;	//Board-ID
			//~ buffer[3]= nr % 8;	//Servo-Nr
			//~ uint16_t value=((uint16_t) (pos*5000.0))+500;
			//~ buffer[4]=(uint8_t)(value>>7);
			//~ buffer[5]=(uint8_t)(value & 0x007F);
			//~ pos_update=false;
			//~ s->send(buffer, 6);
		//~ }
		//~ else
		//~ {
		//~ char buffer[5]={ 0x80, 0x01, 0x00, 0x00, 0x00};
		//~ buffer[1]=(nr/8)+1;	//Board-ID
		//~ buffer[3]= nr % 8;	//Servo-Nr
		//~ pos_update=false;
		//~ s->send(buffer, 5);
			//~ 
		//~ }
	//~ }
	
	return true;
}
void Servo::power(bool status)
{
	active=status;
	pos_update=true;
}
bool Servo::haspower()
{
	return active;
}


