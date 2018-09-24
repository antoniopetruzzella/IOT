

#include <ArduinoJson.h>

#include <EEPROM.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(-1);
String codiceMW;
String utente;
HTTPClient httpclient;
int ledpin;
int set_led_status;
DynamicJsonBuffer jsonBuffer;

void setup()
{
   EEPROM.begin(10);
  //EEPROM.write(0,0);//RESET FORZATO!!
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  ledpin=D7;
   pinMode(ledpin, OUTPUT);
  if(getConnection()){
    if(!checkRegistration()){//VERIFICA CHE VI SIA UN CODICE NELLA EEPROM, ALTRIMENTI
      scriviCodice();// LO GENERA
      codiceMW=leggiCodice();//LO LEGGE
      display.setCursor(10,30);
      display.println("codice da inserire: "+codiceMW);
      display.display();
      utente=verificaCodice(codiceMW);
    }
    codiceMW=leggiCodice();
    utente=verificaCodice(codiceMW);
    display.clearDisplay();
    display.setCursor(10,30);
    display.println("MW ID: "+codiceMW+" utente "+utente);
    display.display();   
   
  }


}

void loop() {
  display.clearDisplay();
  if (WiFi.status() == WL_CONNECTED) {
  digitalWrite(D7,0);
  foundNewmc();
  
}else{

  //display.clearDisplay();
  display.setCursor(10,20);
  display.println("not connected");
  display.display();
}
 
 //delay(3600000);

}

bool getConnection(){
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10,10);
    display.clearDisplay();
    display.println("...connecting...");
    display.display();
    //Serial.println("stop displaying");
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("AutoConnectAP");
    //Serial.println("connected...yeey :)");
    //Serial.println(WiFi.localIP()); 
  display.clearDisplay();
  display.setCursor(10,20);
  display.println("connected");
  display.display();   
    return true; 
}
void configModeCallback (WiFiManager *myWiFiManager) {
 display.clearDisplay();
  display.setCursor(10,20);
  display.println("entra nella APP per la configurazione");
  display.display();

}

void scriviCodice(){
  int a=random(1000000);
  //Serial.println("codice scritto "+String(a));
  String str=String(a);
  char codice[8];
  String zeri="0000000";
  str=zeri.substring(0,7-str.length())+str;
  //Serial.println("str= "+str);
  str.toCharArray(codice,8);
  EEPROM.write(0,1);
  for(int i=0;i<7;i++){
  EEPROM.write(i+1,codice[i]);
  //Serial.println(String(i)+" - "+ codice[i]);
  
  }
EEPROM.commit();
}

String leggiCodice(){
  char buf[8];
  for (int ii=0;ii<7;ii++){
  buf[ii]=(char)EEPROM.read(ii+1);
  //Serial.println(String(ii)+" - "+buf[ii]);
  }
  String codice_finale=(String)buf;
  //Serial.println("codice letto: "+codice_finale.substring(0,7));
  return codice_finale.substring(0,7);
}

bool checkRegistration(){

  if(EEPROM.read(0)==1){
    //Serial.println("codice presente");
    return true;
  }else{
    //Serial.println("codice non presente");
    return false;
  }
}

String verificaCodice(String codiceMW){

  String utente="";
  while(utente.length()<2){
  httpclient.begin("http://www.heritagexperience.com/marketwall/checkmw.php?id_mw="+codiceMW);
  
  int httpcode=httpclient.GET();
  utente=httpclient.getString();
  //Serial.println("id= "+utente+" URL= "+"http://www.heritagexperience.com/marketwall/checkmw.php?id_mw="+codiceMW+" http code:"+String(httpcode)+" len: "+utente.length());
  httpclient.end();
  
  }
  return utente;
}

void foundNewmc(){
  String valore;
  int httpcode=0;
  String valori[36];
  while(httpcode!=HTTP_CODE_OK){
    httpclient.begin("http://www.heritagexperience.com/marketwall/checkmc.php?mwid="+codiceMW);
    httpcode=httpclient.GET();
    valore=httpclient.getString().c_str();//MI RACCOMANDO LA CONVERSIONE IN CHAR ALTRIMENTI JSONBUFFER NON FUNZIONA...
   //Serial.println(valore);
    httpclient.end();
    
    }
  
  int i=0;  
  while(valore.length()>1){  
  int cut=valore.indexOf("}")+1;  
  valori[i]=valore.substring(valore.indexOf("{"),cut);
  //Serial.println(valore);
  Serial.println(String(i)+" "+valori[i]);
  valore.remove(0,cut);
  i++;
  }
  
  for(int j=0;j<i;j++){
    JsonObject& root = jsonBuffer.parseObject(valori[j]);
    Serial.println("j:"+String(j));
      for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
      
      
        if((String(it->key))=="posizione" && String(it->value.as<char*>())=="1"){ //ASSOCIA POSIZIONE E LED
          int led=D7;
        
        digitalWrite(led,1);
        }
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("posiziona il nuovo MC presso il led");
      display.setCursor(0,20);
      display.println(String(it->key));
      display.setCursor(0,30);
      display.println(String(it->value.as<char*>()));
      display.display(); 
      
      delay(1000);
 
      }

  }

}
