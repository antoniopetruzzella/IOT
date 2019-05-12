#include <EEPROM.h>
#include <WiFiManager.h>   
int SLEEP_MODE_PERIOD=30;//3600000;//UN ORA
//int ACTUAL_CONNECTION_TIME=0;
//int ACTUAL_IRRIGATION_TIME=0;
//int NEXT_CONNECTION_PERIOD=7200000;//DUE ORE
int NEXT_IRRIGATION_PERIOD=60;//86400000;//24 ORE
int IRRIGATION_PERIOD=10000;//10 SECONDI
int WAKE_UP_COUNT=0; //CONTATORE DI RISVEGLI
int NEXT_CONNECTION_COUNT=3; //CONTATORE DI RISVEGLI NECESSARI PER LA CONNESSIONE 
int MILLIS_COUNTER_FROM_LAST_IRRIG=0; //CONTATORE IN MILLIS DALL'ULTIMA IRRIGAZIONE
int WAKE_UP_COUNT_EEPROM_ADDRESS=0; //INDIRIZZO EEPROM DEL VALORE DELLA VARIABILE
int MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS=1 //INDIRIZZO EEPROM DEL VALORE DELLA VARIABILE
int POMPAPIN=D6;

void setup() {
  // put your setup code here, to run once:
 EEPROM.begin();
 pinMode(POMPAPIN,OUTPUT); 
 Serial.begin(9600); 
 //Serial.println(ESP.getResetReason());
 /* if(ESP.getResetReason()=="External System"){
    //QUANDO DI CARA' NECESSITA' DI UN SENSORE DI WAKE UP SCRIVI QUI QUELLO CHE DEVE SUCCEDERE
    ESP.deepSleep(SLEEP_MODE_PERIOD);
  }*/
  Serial.println("sono sveglio");
  if(EEPROM.get(WAKE_UP_COUNT_EEPROM_ADDRESS)){
    WAKE_UP_COUNT=EEPROM.get(WAKE_UP_COUNT_EEPROM_ADDRESS);
    Serial.println("WAKE_UP_COUNT: "+String(WAKE_UP_COUNT));
  }
  WAKE_UP_COUNT=WAKE_UP_COUNT+1;
  
  if(WAKE_UP_COUNT==NEXT_CONNECTION_COUNT){//SONO PASSATI ABBASTANZA WAKE UP
      Serial.println("SONO PASSATI ABBASTANZA WAKE UP, PROVIAMO A CONNETTERCI..."); 
      WAKE_UP_COUNT=0;
      WiFiManager wifiManager;
      wifiManager.setAPCallback(configModeCallback);
      wifiManager.autoConnect("WebPocket-E036","antonio71");
      if (WiFi.status() == WL_CONNECTED) {//SE C'E' CONNESSIONE
         Serial.println("SONO CONNESSO");        
        if(isTimeToIrrig()){ //SE E' TEMPO DI IRRIGARE
          Serial.println("WEBAPP DICE: E' TEMPO DI IRRIGARE");
          doIrrig();
         
     }else{ //SE NON E' TEMPO DI IRRIGARE
          Serial.println("WEBAP DICE: NON E' TEMPO DI IRRIGARE"); 
          ESP.deepSleep(SLEEP_MODE_PERIOD);
          
     }
      }else{ //SE NON  C' E' CONNESSIONE
          Serial.println("NON SONO CONNESSO"); 
         if(EEPROM.get(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS)){
          MILLIS_COUNTER_FROM_LAST_IRRIG=EEPROM.get(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS)
          MILLIS_COUNTER_FROM_LAST_IRRIG=MILLIS_COUNTER_FROM_LAST_IRRIG+(NEXT_CONNECTION_COUNT*SLEEP_MODE_PERIOD)
          EEPROM.put(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS,MILLIS_COUNTER_FROM_LAST_IRRIG)
          Serial.println("MILLIS_COUNTER_FROM_LAST_IRRIG: "+String(MILLIS_COUNTER_FROM_LAST_IRRIG));
          if(MILLIS_COUNTER_FROM_LAST_IRRIG>=NEXT_IRRIGATION_PERIOD){
            Serial.println("SONO PASSATE 24 ORE, E' TEMPO DI IRRIGARE"); 
            doIrrig();
          }else{
            Serial.println("NON SONO PASSATE 24 ORE, NON E' TEMPO DI IRRIGARE");
          }
        }
        ESP.deepSleep(SLEEP_MODE_PERIOD);
        
      }
    
  }else{// NON SONO PASSATI ABBASTANZA WAKE UP
  Serial.println("NON SONO PASSATI ABBASTANZA WAKE UP, TORNO A DORMIRE");   
  EEPROM.put(WAKE_UP_COUNT_EEPROM_ADDRESS,WAKE_UP_COUNT);
  ESP.deepSleep(SLEEP_MODE_PERIOD);
  }
  
 

}
bool isTimeToIrrig(){
  return true;
  //return false;
}
void doIrrig(){

   digitalWrite(POMPAPIN,1);
   delay(IRRIGATION_PERIOD);  
   digitalWrite(POMPAPIN,0);
   MILLIS_COUNTER_FROM_LAST_IRRIG=0
   EEPROM.put(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS,MILLIS_COUNTER_FROM_LAST_IRRIG)
   ESP.deepSleep(SLEEP_MODE_PERIOD);
}
void configModeCallback (WiFiManager *myWiFiManager) {
ESP.deepSleep(SLEEP_MODE_PERIOD);
}
void loop() {

 
}
