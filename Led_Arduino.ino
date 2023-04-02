#include<SPI.h>
#include<DMD.h>
#include<TimerOne.h>
#include "SystemFont5x7.h"
#include "Arial_black_16.h"
#define DISPLAY_ACROSS 1
#define DISPLAY_DOWN 1
DMD dmd(DISPLAY_ACROSS,DISPLAY_DOWN);
int temp=0,i=0,x=0,k=0;
char str[400],msg[400];
char r_char;
byte index=0;
char greeting[]="VVCOE_CSE ";
void ScanDMD()
{
dmd.scanDisplayBySPI();
}
void setup()
{
Timer1.initialize ( 5000 );
Timer1.attachInterrupt ( ScanDMD );
dmd.clearScreen(true);
Serial.begin(9600);
strcpy(msg,greeting);
gsm_init();
Serial.println("AT+CNMI=2,2,0,0,0");
delay(500);
Serial.println("AT+CMGF=1");
delay(1000);
}
void loop()
{
for(unsigned int t=0;t<60000;t++)
{
serialEvent();
if(temp==1)
{
  dmd.clearScreen(true);
  x=0,k=0,temp=0;
  while(x<i)
  {
   while(str[x]=='&')
   {
      x++;
      while(str[x]!='@')
      {
       msg[k++]=str[x++];
      }
    }
    x++;
}
msg[k]='\0';
Serial.println(msg);
delay(1000);
temp=0;
i=0;
x=0;
k=0;
}
}
dmd.selectFont(Arial_Black_16);
dmd.drawMarquee(msg,32,(32*DISPLAY_ACROSS)-1,0);
long start=millis();
long timer=start;
boolean ret=false;
while(!ret)
{
if((timer+30)<millis())
{
ret=dmd.stepMarquee(-1,0);
timer=millis();
}
}
}
void serialEvent()
{
while(Serial.available())
{
char ch=(char)Serial.read();
str[i++]=ch;
if(ch=='*')
{
temp=1;
Serial.print("Message Received");
delay(1000);
}
}
}
void gsm_init()
{
boolean at_flag=1;
while(at_flag)
{
Serial.println("AT");
  while(Serial.available()>0)
  {
   if(Serial.find("OK"))
   at_flag=0;
  }
  delay(1000);
}
boolean echo_flag=1;
while(echo_flag)
{
Serial.println("ATE0");
  while(Serial.available()>0)
  {
   if(Serial.find("OK"))
   echo_flag=0;
  }
  delay(1000);
}
boolean net_flag=1;
while(net_flag)
{
Serial.println("AT+CPIN?");
  while(Serial.available()>0)
  {
   if(Serial.find("+CPIN: READY"))
   net_flag=0;
  }
  delay(1000);
}
}
