#include <EEPROM.h>

#define CONF_HEADER "BsOsuKeyboard2"

struct _Config
{
  byte sm1;
  byte sm2;
  byte sm3;
  byte sm4;
  byte btn1;
  byte btn2;
  static _Config MakeDefault()
  {
    _Config ret;
    ret.sm1=96;   //Tilde
    ret.sm2=194;  //F1
    ret.sm3=195;  //F2
    ret.sm4=196;  //F3
    ret.btn1='x';
    ret.btn2='z';
    return ret;
  }
};

class _CFG
{
  private:
  _Config cfg;
  void savecfg()
  {
    EEPROM.put(strlen(CONF_HEADER), cfg);
  }
  public:
  void begin()
  {
    Serial.begin(9600);
    unsigned int addr;
    String header;
    for(addr=0;addr<strlen(CONF_HEADER);addr++)
    {
      header+=(char)EEPROM.read(addr);
    }
    if(header!=CONF_HEADER)
    {
      header=CONF_HEADER;
      for(addr=0;addr<strlen(CONF_HEADER);addr++)
      {
        EEPROM.write(addr,header[addr]);
      }
      EEPROM.put(addr,cfg.MakeDefault());
    }
    addr=strlen(CONF_HEADER);
    EEPROM.get(addr,cfg);
  }

  byte getSM1(){
    return cfg.sm1;
  }
  byte getSM2(){
    return cfg.sm2;
  }
  byte getSM3(){
    return cfg.sm3;
  }
  byte getSM4(){
    return cfg.sm4;
  }
  byte getBTN1(){
    return cfg.btn1;
  }
  byte getBTN2(){
    return cfg.btn2;
  }

  void ConfigProc()
  {
    int datasize=Serial.available();
    String cmd;
    for(int i=0;i<datasize;i++)
    {
      cmd+=(char)Serial.read();
    }
    if(cmd=="bss--ping")
      Serial.println("bsr--hello");
    else if(cmd=="bss--stat")
    {
      String stat="bsr--";
      stat+=cfg.sm1;
      stat+=",";
      stat+=cfg.sm2;
      stat+=",";
      stat+=cfg.sm3;
      stat+=",";
      stat+=cfg.sm4;
      stat+=",";
      stat+=cfg.btn1;
      stat+=",";
      stat+=cfg.btn2;
      Serial.println(stat);
    }
    else if(cmd.substring(0,9)=="bss--SM1 ")
    {
      cfg.sm1=cmd.substring(9).toInt();
      savecfg();
      Serial.println("bsr--done");
    }
    else if(cmd.substring(0,9)=="bss--SM2 ")
    {
      cfg.sm2=cmd.substring(9).toInt();
      savecfg();
      Serial.println("bsr--done");
    }
    else if(cmd.substring(0,9)=="bss--SM3 ")
    {
      cfg.sm3=cmd.substring(9).toInt();
      savecfg();
      Serial.println("bsr--done");
    }
    else if(cmd.substring(0,9)=="bss--SM4 ")
    {
      cfg.sm4=cmd.substring(9).toInt();
      savecfg();
      Serial.println("bsr--done");
    }
    else if(cmd.substring(0,9)=="bss--BT1 ")
    {
      cfg.btn1=cmd.substring(9).toInt();
      savecfg();
      Serial.println("bsr--done");
    }
    else if(cmd.substring(0,9)=="bss--BT2 ")
    {
      cfg.btn2=cmd.substring(9).toInt();
      savecfg();
      Serial.println("bsr--done");
    }
    else if(cmd=="bsst--reset")
    {
      unsigned int addr;
      String header=CONF_HEADER;
      for(addr=0;addr<strlen(CONF_HEADER);addr++)
      {
        EEPROM.write(addr,header[addr]);
      }
      EEPROM.put(addr,cfg.MakeDefault());
      cfg=cfg.MakeDefault();
      savecfg();
      Serial.println("bsrt--CONFIG RESET IS DONE");
    }
    else if(cmd=="bsst--eeprom")
    {
      int sizecfg=strlen(CONF_HEADER)+sizeof(_Config);
      for(int i=0;i<sizecfg;i++)
      {
        Serial.print(i);
        Serial.print("\t: ");
        Serial.println(EEPROM.read(i)); 
      }
    }
    else
      Serial.println("bss--unknown command");
  }
}Config;

