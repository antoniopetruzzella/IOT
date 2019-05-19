#include <EEPROM.h>
#include <WiFiManager.h> 
#include <ESP8266HTTPClient.h>  
#include <ArduinoJson.h>
int MILLI_SLEEP_MODE_PERIOD=10000;//3600000;//UN ORA
//int ACTUAL_CONNECTION_TIME=0;
//int ACTUAL_IRRIGATION_TIME=0;
//int NEXT_CONNECTION_PERIOD=7200000;//DUE ORE
int MILLIS_NEXT_IRRIGATION_PERIOD=60000;//86400000;//24 ORE
int MILLIS_IRRIGATION_PERIOD=10000;//10 SECONDI
int WAKE_UP_COUNT=0; //CONTATORE DI RISVEGLI
int NEXT_CONNECTION_COUNT=3; //CONTATORE DI RISVEGLI NECESSARI PER LA CONNESSIONE 
int MILLIS_COUNTER_FROM_LAST_IRRIG=0; //CONTATORE IN MILLIS DALL'ULTIMA IRRIGAZIONE
int WAKE_UP_COUNT_EEPROM_ADDRESS=0; //INDIRIZZO EEPROM DEL VALORE DELLA VARIABILE
int MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS=5; //INDIRIZZO EEPROM DEL VALORE DELLA VARIABILE
int POMPAPIN=D6;

void setup() {
  // put your setup code here, to run once:
 
 Serial.begin(9600);
 Serial.println("sono sveglio");
 EEPROM.begin(10);
 pinMode(POMPAPIN,OUTPUT); 
  
 //Serial.println(ESP.getResetReason());
 /* if(ESP.getResetReason()=="External System"){
    //QUANDO DI CARA' NECESSITA' DI UN SENSORE DI WAKE UP SCRIVI QUI QUELLO CHE DEVE SUCCEDERE
    ESP.deepSleep(MILLI_SLEEP_MODE_PERIOD);
  }*/
  EEPROM.get(WAKE_UP_COUNT_EEPROM_ADDRESS,WAKE_UP_COUNT);
  if(WAKE_UP_COUNT>10){
    WAKE_UP_COUNT=0;
  }
  
 
  WAKE_UP_COUNT=WAKE_UP_COUNT+1;
  Serial.println("WAKE_UP_COUNT: "+String(WAKE_UP_COUNT)+" LO CONFRONTO CON: NEXT_CONNECTION_COUNT: "+String(NEXT_CONNECTION_COUNT));
 
  if(WAKE_UP_COUNT==NEXT_CONNECTION_COUNT){//SONO PASSATI ABBASTANZA WAKE UP
      Serial.println("SONO PASSATI ABBASTANZA WAKE UP, PROVIAMO A CONNETTERCI..."); 
      WAKE_UP_COUNT=0;
      EEPROM.put(WAKE_UP_COUNT_EEPROM_ADDRESS,WAKE_UP_COUNT);
      EEPROM.commit();
      WiFiManager wifiManager;
      //wifiManager.setAPCallback(configModeCallback);
      WiFi.mode(WIFI_STA);
      //wifiManager.setConfigPortalTimeout(10);
      //wifiManager.autoConnect("WebPocket-E036","antonio71");
      int connRes = WiFi.waitForConnectResult();
      
      //WiFi.begin("WebPocket-E036","antonio71");
      if (WiFi.status() == WL_CONNECTED) {//SE C'E' CONNESSIONE
         Serial.println("SONO CONNESSO");        
        if(isTimeToIrrig()){ //SE E' TEMPO DI IRRIGARE
          Serial.println("WEBAPP DICE: E' TEMPO DI IRRIGARE");
          doIrrig();
         
     }else{ //SE NON E' TEMPO DI IRRIGARE
          Serial.println("WEBAPP DICE: NON E' TEMPO DI IRRIGARE, ENRTO IN DEEP SLEEP"); 
          ESP.deepSleep(MILLI_SLEEP_MODE_PERIOD*1000);
          
     }
      }else{ //SE NON  C' E' CONNESSIONE
          Serial.println("NON SONO CONNESSO"); 
          EEPROM.get(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS,MILLIS_COUNTER_FROM_LAST_IRRIG);
          Serial.println("MILLIS_COUNTER_FROM_LAST_IRRIG DA LETTURA:"+String(MILLIS_COUNTER_FROM_LAST_IRRIG));
          Serial.println("NEXT_CONNECTION_COUNT:"+String(NEXT_CONNECTION_COUNT));
          
          MILLIS_COUNTER_FROM_LAST_IRRIG=MILLIS_COUNTER_FROM_LAST_IRRIG+(NEXT_CONNECTION_COUNT*MILLI_SLEEP_MODE_PERIOD);
           Serial.println("MILLIS_COUNTER_FROM_LAST_IRRIG DOPO FORMULA:"+String(MILLIS_COUNTER_FROM_LAST_IRRIG));
          EEPROM.put(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS,MILLIS_COUNTER_FROM_LAST_IRRIG);
          EEPROM.commit();
          Serial.println("MILLIS_COUNTER_FROM_LAST_IRRIG: "+String(MILLIS_COUNTER_FROM_LAST_IRRIG)+" LO CONFRONTO CON: MILLIS_NEXT_IRRIGATION_PERIOD: "+String(MILLIS_NEXT_IRRIGATION_PERIOD));
          
          if(MILLIS_COUNTER_FROM_LAST_IRRIG>=MILLIS_NEXT_IRRIGATION_PERIOD){
            Serial.println("SONO PASSATE 24 ORE, E' TEMPO DI IRRIGARE"); 
            doIrrig();
          }else{
            Serial.println("NON SONO PASSATE 24 ORE, NON E' TEMPO DI IRRIGARE");
          }
        
        ESP.deepSleep(MILLI_SLEEP_MODE_PERIOD*1000);
        
      }
    
  }else{// NON SONO PASSATI ABBASTANZA WAKE UP
  Serial.println("NON SONO PASSATI ABBASTANZA WAKE UP, TORNO A DORMIRE");  
  Serial.println("MA PRIMA SCRIVO WAKE_UP_COUNT: "+String(WAKE_UP_COUNT));
  EEPROM.put(WAKE_UP_COUNT_EEPROM_ADDRESS,WAKE_UP_COUNT);
  EEPROM.commit();
 
  ESP.deepSleep(MILLI_SLEEP_MODE_PERIOD*1000);
  }
  
 

}
bool isTimeToIrrig(){
  HTTPClient httpclient;
  DynamicJsonBuffer jsonBuffer; 
  int httpcode;
  const char* result;
  while(httpcode!=HTTP_CODE_OK){
    Serial.println("call: http://www.heritagexperience.com/iot/innaffiarosa.php?MILLIS_NEXT_IRRIGATION_PERIOD="+String(MILLIS_NEXT_IRRIGATION_PERIOD));
    httpclient.begin("http://www.heritagexperience.com/iot/innaffiarosa.php?MILLIS_NEXT_IRRIGATION_PERIOD="+String(MILLIS_NEXT_IRRIGATION_PERIOD));
    Serial.println("waiting get");                     
    httpcode=httpclient.GET();
    JsonObject& root = jsonBuffer.parseObject(httpclient.getString().c_str());
    
    
    if(root.size()>0){
      result=root["result"];
      Serial.println("root returned "+String(result));  
     }else{
      Serial.println("root is null");
      return false;
    }
  }
  if(String(result)=="true"){
    return true;
  }else{
    return false;  
  }
  //return true;
  
}
void doIrrig(){
   Serial.println("INIZIO AD IRRIGARE");
   digitalWrite(POMPAPIN,1);
   delay(MILLIS_IRRIGATION_PERIOD);  
   digitalWrite(POMPAPIN,0);
   MILLIS_COUNTER_FROM_LAST_IRRIG=0;
   EEPROM.put(MILLIS_COUNTER_FROM_LAST_IRRIG_EEPROM_ADDRESS,MILLIS_COUNTER_FROM_LAST_IRRIG);
   EEPROM.commit();
   Serial.println("HO FINITO D IRRIGARE, TORNO A DORMIRE");
   ESP.deepSleep(MILLI_SLEEP_MODE_PERIOD*1000);
}

void loop() {

 
}
