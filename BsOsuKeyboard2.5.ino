#include <Keyboard.h>
#include "Config.h"

/*
 * SM = Small Btn
 * LED= LED For BTN
 * BTN= Cherry MX Switch
*/
#define SM1 2
#define SM2 3
#define SM3 4
#define SM4 9
#define LED1 6
#define LED2 5
#define BTN1 8 
#define BTN2 7
#define KEYCOUNT 6  //there is 4 push btn, and 2 cherry mx
#define LEDCOUNT 2  //2 buttons's LED
#define LED_DELAY 1 //led delay

int key[KEYCOUNT]={BTN1,BTN2,SM1,SM2,SM3,SM4};
int lastkeystate[KEYCOUNT];
int led[]={LED1,LED2};
int ledbrightness[]={0,0};
unsigned long lastleddecrement[]={millis(),millis()};

void setup() {
  // put your setup code here, to run once:
  //button
  for(int i=0;i<KEYCOUNT;i++)
  {
    pinMode(key[i],INPUT);
    digitalWrite(key[i],HIGH);
    lastkeystate[i]=HIGH;
  }
  //LED
  for(int i=0;i<LEDCOUNT;i++)
  {
    pinMode(led[i],OUTPUT);
    analogWrite(led[i],ledbrightness[i]);
  }
  Config.begin();
  Keyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0)
  {
    Config.ConfigProc();
  }
  int nowkeystat[KEYCOUNT];
  for(int i=0;i<KEYCOUNT;i++)
  {
    nowkeystat[i]=digitalRead(key[i]);
    if(nowkeystat[i]!=lastkeystate[i])
    {
      lastkeystate[i]=nowkeystat[i];
      if(lastkeystate[i]==LOW)
      {
        switch(key[i])
        {
          case BTN1:
            Keyboard.press(Config.getBTN1());
            break;
          case BTN2:
            Keyboard.press(Config.getBTN2());
            break;
          case SM1:
            Keyboard.press(Config.getSM1());
            break;
          case SM2:
            Keyboard.press(Config.getSM2());
            break;
          case SM3:
            Keyboard.press(Config.getSM3());
            break;
          case SM4:
            Keyboard.press(Config.getSM4());
            break;
        }
      }
      else
      {
        switch(key[i])
        {
          case BTN1:
            Keyboard.release(Config.getBTN1());
            break;
          case BTN2:
            Keyboard.release(Config.getBTN2());
            break;
          case SM1:
            Keyboard.release(Config.getSM1());
            break;
          case SM2:
            Keyboard.release(Config.getSM2());
            break;
          case SM3:
            Keyboard.release(Config.getSM3());
            break;
          case SM4:
            Keyboard.release(Config.getSM4());
            break;
        } 
      }
    }
  }
  unsigned long now=millis();
  for(int i=0;i<LEDCOUNT;i++)
  {
    if(lastkeystate[i]==LOW)
    {
      ledbrightness[i]=255;
      lastleddecrement[i]=now;
    }
    if(ledbrightness[i]>0&&(now-lastleddecrement[i])>LED_DELAY)
    {
      ledbrightness[i]--;
      lastleddecrement[i]=now;
    }
    analogWrite(led[i],ledbrightness[i]);
  }
  //disable TX RX LED
  TXLED0;
  RXLED0;
}
