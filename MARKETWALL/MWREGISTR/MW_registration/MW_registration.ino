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


void setup()
{
  EEPROM.begin(10);
  //EEPROM.write(0,0);//RESET FORZATO!!
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  if(getConnection()){
    if(!checkRegistration()){//VERIFICA CHE VI SIA UN CODICE NELLA EEPROM, ALTRIMENTI
      scriviCodice();// LO GENERA
      codiceMW=leggiCodice();//LO LEGGE
      display.setCursor(10,30);
      display.println("codice: "+codiceMW);
      display.display();
      utente=verificaCodice(codiceMW);
    }
    codiceMW=leggiCodice();
    utente=verificaCodice(codiceMW);
    display.setCursor(10,30);
    display.println("codice: "+codiceMW+" utente "+utente);
    display.display();   
   
  }


}

void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
  //display.clearDisplay();
  display.setCursor(10,20);
  display.println("connected");
  display.display();   
}else{

  //display.clearDisplay();
  display.setCursor(10,20);
  display.println("not connected");
  display.display();
}
    
 delay(5000);

}

bool getConnection(){
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10,10);
    display.clearDisplay();
    display.println("...connecting...");
    display.display();
    Serial.println("stop displaying");
    WiFiManager wifiManager;
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");
    Serial.println(WiFi.localIP()); 
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
  Serial.println("codice scritto "+String(a));
  String str=String(a);
  char codice[8];
  String zeri="0000000";
  str=zeri.substring(0,7-str.length())+str;
  Serial.println("str= "+str);
  str.toCharArray(codice,8);
  EEPROM.write(0,1);
  for(int i=0;i<7;i++){
  EEPROM.write(i+1,codice[i]);
  Serial.println(String(i)+" - "+ codice[i]);
  
  }
EEPROM.commit();
}

String leggiCodice(){
  char buf[8];
  for (int ii=0;ii<7;ii++){
  buf[ii]=(char)EEPROM.read(ii+1);
  Serial.println(String(ii)+" - "+buf[ii]);
  }
  String codice_finale=(String)buf;
  Serial.println("codice letto: "+codice_finale.substring(0,7));
  return codice_finale.substring(0,7);
}

bool checkRegistration(){

  if(EEPROM.read(0)==1){
    Serial.println("codice presente");
    return true;
  }else{
    Serial.println("codice non presente");
    return false;
  }
}

String verificaCodice(String codiceMW){

  String utente="";
  while(utente==""){
  httpclient.begin("http://www.heritagexperience.com/marketwall/checkmw.php?id_mw="+codiceMW);
  
  int httpcode=httpclient.GET();
  utente=httpclient.getString();
  Serial.println(utente);
  httpclient.end();
  
  }
  return utente;
}

