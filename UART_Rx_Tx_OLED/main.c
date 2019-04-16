
#include "lcd.h"
#include "uart.h"
#include "lpc17xx.h"
#include "delay.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * 
 */
 struct formDisplayString{
	 int _screen;
	 int _hour;
	 int _min;
	 int _battery;
	 char* _songs[5];
	 char* _currentlyPlaying;
	 int _durationMin;
	 int _durationSec;
 };
 /*
 char* convChar(int i){
	 char* c;
	 char k;
	 sprintf(k, "%d", i);
	 return c;
 }*/
 char* parseString(struct formDisplayString *fd){
	 char temp[20];
	 char* t;
	 char* finalString ;
	 sprintf(temp, "%d", fd->_screen);
	 t = temp;
	 strcpy(finalString, "screen:");
	 strcat(finalString, temp); 
	 
	 strcat(finalString, ";time:");
	 sprintf(temp, "%d", fd->_hour);
	 strcat(finalString, temp);
	 strcat(finalString, ":");
	 sprintf(temp, "%d", fd->_min);
	 strcat(finalString, temp);
	 strcat(finalString, ";battery:");
	 sprintf(temp, "%d", fd->_battery);
	 strcat(finalString, temp);
	 strcat(finalString, ";");
	 strcat(finalString, fd->_songs[0]);
	 strcat(finalString, ",");
	 strcat(finalString, fd->_songs[1]);
	 strcat(finalString, ",");	 
	 strcat(finalString, fd->_songs[2]);
	 strcat(finalString, ",");
	 strcat(finalString, fd->_songs[3]);
	 strcat(finalString, ",");	 
	 strcat(finalString, fd->_songs[4]);
	 strcat(finalString, ";currentlyplaying:");
	 strcat(finalString, fd->_currentlyPlaying);
	 strcat(finalString, ";duration:");
	 sprintf(temp, "%d", fd->_durationMin);
	 strcat(finalString, temp);
	 strcat(finalString, ":");
	 sprintf(temp, "%d", fd->_durationSec);
	 strcat(finalString, temp);
	 strcat(finalString, ";#\n");
	 
	 return finalString;
 }

int main() 
{
	  struct formDisplayString str;
    //char ch;
		char* formString = "screen:1;time:2:15;battery:42;s1,s2,s3,s4,s5;currentlyplaying:s2;duration:4:15;#\n";
		SystemInit();
		UART0_Init(4800);
		UART3_Init(4800);
		
		str._screen = 1;
		str._battery = 42;
		str._hour = 2;
		str._min = 15;
		str._songs[0] = "s1";
		str._songs[1] = "s2";
		str._songs[2] = "s3";
		str._songs[3] = "s4";
		str._songs[4] = "s5";
		str._currentlyPlaying = "s1";
	  str._durationMin = 4;
	  str._durationSec = 15;
    while(1)
    {
			//if(UART0_RxChar()){
			 UART0_TxString(parseString(&str));
       UART3_TxString(parseString(&str));
			//}
			 DELAY_ms(1000);
    }
    
    
}

