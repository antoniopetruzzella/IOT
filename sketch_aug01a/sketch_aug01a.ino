#include <EEPROM.h>
int value;
int read_value;
int MILLI_SLEEP_MODE_PERIOD=1000;
void setup() {
  Serial.begin(9600);
  EEPROM.begin(512);
  read_value=EEPROM.read(0);
  Serial.println("il valore è:"+String(read_value));
  value=read_value+1;
  EEPROM.write(0,value);
  
  EEPROM.commit();
  read_value=EEPROM.read(0);
  Serial.println("il valore dopo commit è:"+String(read_value));
  ESP.deepSleep(MILLI_SLEEP_MODE_PERIOD*1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  
  delay(100);
  
}
