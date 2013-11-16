/*
*
*  LCD GPIO interface
*
*  Author: Arturo Torres
*/

#ifndef _LCD_H
#define _LCD_H

void sendByte(char byteToSend, int isData);
void displayON(int isON);
void clearDisplay();
void initLCD(char *text);

#endif
