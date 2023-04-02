#include <LiquidCrystal.h>
LiquidCrystal lcd(7,6,5,4,3,2);

int led=13;

int temp=0,i=0,x=0,k=0,bre=0;
char str[200],msg[200];

void setup() 
{
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  lcd.print("GSM Initilizing...");
  gsm_init();
  lcd.setCursor(0,0);
  lcd.print("Wireless Notice");
  lcd.setCursor(0,1);
  lcd.print("    Board      ");
  delay(2000);
  lcd.clear();
  lcd.print("**VVCOE_CSE**");
  delay(1000);
  lcd.setCursor(0,1);
Serial.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  Serial.println("AT+CMGF=1");
  delay(1000);
  digitalWrite(led, LOW);
}

void loop()    
{
  for(unsigned int t=0;t<60000;t++)
  {
    serialEvent();
  if(temp==1)
  {
    x=0,k=0,temp=0;
    while(x<i)
    {
      while(str[x]=='&')
      {
        x++;
        while(str[x]!='$')
        {
          msg[k++]=str[x++];
        }
      }
      x++;
    }
    msg[k]='\0';
    lcd.clear();
   if(msg=="clear" || msg=="CLEAR" || msg=="Clear")
    {
    lcd.clear();
    lcd.print("**VVCOE_CSE**");
    }
   else
    {
      lcd.print(msg);
    }
    delay(1000);
    temp=0;
    i=0;
    x=0;
    k=0;
  }
  }
  lcd.scrollDisplayLeft();
}
void serialEvent()
{
  while(Serial.available())
  {
    char ch=(char)Serial.read();
    str[i++]=ch;
    if(ch == '$')
    {
      temp=1;
      lcd.clear();
      lcd.print("Message Received");
      delay(1000);
    }
  }
}

void gsm_init()
{
  lcd.clear();
  lcd.print("Finding Module..");
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
  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
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

  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    Serial.println("AT+CPIN?");
    while(Serial.available()>0 && bre==0)
    {
      if(Serial.find("+CPIN: READY"))
      net_flag=0;
      else
       {
       delay(1000);
       lcd.clear();
       lcd.print("No network..");
       bre=1;
       }
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  delay(1000);
  lcd.clear();
}
