/*
	2009 Benjamin Reh
	ich@benjaminreh.de
*/
#include "servoboard.h"
#include <unistd.h>

using namespace std;


bool running = true;
        
using namespace std;

#include <curses.h>
#include <stdlib.h>

#define ANZAHL_SERVOS 32
#define STEPS 0.005

void quit(void)
{
  endwin();
}

int main(void)
{
	Servoboard b(ANZAHL_SERVOS);
	b.connect("/dev/ttyS0");


	//init
	initscr();
	atexit(quit);
	keypad(stdscr, TRUE);
	//  start_color();
	char c;
	int active=0;
	while (c!=27 && c!='q')
	{
		clear();
		int h, w;
		char i;
		getmaxyx(stdscr, h, w);
		mvaddstr(0, 0, "Servo-Test-Programm");
		for (i=0; i<ANZAHL_SERVOS; i++)
		{
			char s[w];
			sprintf(s,"Servo Nr. %i",i);
			mvaddstr(i*2+1,0,s);
			sprintf(s,"Wert: %1.3f",b.servo[i].getPos());
			mvaddstr(i*2+1,20,s);
			if (b.servo[i].haspower()==true)
			{
				int z=0;
				for (z=0;z<w-4;z++)
				{
					if (z== (int) (b.servo[i].getPos()*(w-4)))
						s[z]='|';
					else
						s[z]='=';
				}
				s[w-4]='\0';
				mvaddstr(i*2+2,2,s);
			}
			else
				mvaddstr(i*2+2,w/2-1,"OFF");
		}
		mvaddstr(active*2+2,0,">>");
		mvaddstr(active*2+2,w-2,"<<");
	
		mvaddstr(h-1, 1, "Bedienung durch Pfeiltasten, [LEERTASTE] Servo an/aus,  [ESC] oder 'q' beendet"); 
		refresh();
		c=getch(); 
		if (c==3 && active>0)
			active--;
		if (c==2 && active<ANZAHL_SERVOS-1)
			active++;
		if (c==' ')
			b.servo[active].power(!(b.servo[active].haspower()));
		if(c==5)
			b.servo[active].setPos(b.servo[active].getPos()+STEPS);
		if(c==4)
			b.servo[active].setPos(b.servo[active].getPos()-STEPS);
		b.update();
	}
	b.disconnect();

}	

