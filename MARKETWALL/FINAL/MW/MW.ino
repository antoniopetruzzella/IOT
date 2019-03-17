

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
String username;
HTTPClient httpclient;
int ledpin;
int set_led_status;
int buttonPin;
int buttonState;
int blubuttonState;
int bluledpin;
int blubuttonPin;
DynamicJsonBuffer jsonBuffer;

void setup()
{
  //DICHIARAZIONI
   EEPROM.begin(10);
  //EEPROM.write(0,0);//RESET FORZATO!!
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  ledpin=D7;
  bluledpin=D5;
  buttonPin = D6;
  blubuttonPin=D0;
  pinMode(ledpin, OUTPUT);
  pinMode(bluledpin,OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(blubuttonPin, INPUT);
  
  //FINE DICHIARAZIONI
  
  //PRIMO METODO SETUP: CONNETTERSI
  if(getConnection()){
    
    //SECONDO METODO DI SETUP:VERIFICA CHE VI SIA UN CODICE NELLA EEPROM
    if(!checkRegistration()){
      scriviCodice();//ALTRIMENTI  LO GENERA
      codiceMW=leggiCodice();//LO LEGGE
      display.setCursor(10,30);
      display.println("codice da inserire: "+codiceMW);
      display.display();
      
    } //LO LEGGE
    codiceMW=leggiCodice();
    //TERZO METODO DI SETUP. TROvA L'UTENTE DEL MARKETWALL
    utente=getUser(codiceMW);
    username=getUserName(utente);
    display.clearDisplay();
    display.setCursor(10,30);
    display.println("MW ID: "+codiceMW);
    display.setCursor(10,40);
    display.println(" utente "+username);
    display.display();
    delay(5000);
  }


}

void loop() {
  //Serial.println("passo");

if (WiFi.status() == WL_CONNECTED) {
  digitalWrite(bluledpin,0);
  digitalWrite(ledpin,0);
  foundNewmc();
  foundNewPromo();
  display.clearDisplay();
  display.setCursor(10,30);
  display.println("MW ID: "+codiceMW);
  display.setCursor(10,40);
  display.println(" utente "+username);
  display.display();
  buttonClickedEventHandler(); 
}else{

  //display.clearDisplay();
  display.setCursor(10,20);
  display.println("not connected");
  display.display();
}

 delay(1000);

}

void buttonClickedEventHandler(){

  buttonState = digitalRead(buttonPin);
  if (buttonState==HIGH) {
   
    bool ver3s=verifica3sec();
    if(ver3s==true){
      Serial.println ("faccio partire l'ordine");
      insertNewOrdine(1);
    }
  }
  
}
bool verifica1sec(){

  int i=0;
  while(i<1){
    blubuttonState = digitalRead(blubuttonPin);
    if(blubuttonState==LOW){
      return false;
    }
    i++;
    delay(100);
  }
  return true;
}
bool verifica3sec(){
 display.clearDisplay();
 display.setCursor(0,10);
 display.println("TIENI PREMUTO");
  display.setCursor(0,20);
 display.println("PER INVIARE L'ORDINE");
 display.display();
  int i=0;
  while(i<3){
     Serial.println ("dentro ver 3");
    buttonState = digitalRead(buttonPin);
    if(buttonState==LOW){
      return false;
    }
    i++;
    delay(1000);
  }
  return true;
}

void insertPositionConfirm(){
   Serial.println("dentro insertconfirm");
   int httpcode=0;
   while(httpcode!=HTTP_CODE_OK){
   httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.confirmmcinsertion&mwid="+codiceMW);
   httpcode=httpclient.GET();
   httpclient.end();
   }
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("NUOVO MCUBE INSERITO");
    display.setCursor(0,20);
    display.println("PUOI RILASCIARE");
    display.display();
    delay(5000);
}
void insertPromoConfirm(){
   Serial.println("dentro insertconfirm");
   int httpcode=0;
   while(httpcode!=HTTP_CODE_OK){
   httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.confirmmcinsertionpromo&mwid="+codiceMW);
   httpcode=httpclient.GET();
   httpclient.end();
   }
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("fatto");
    display.setCursor(0,20);
    display.println("grazie");
    display.display();
    delay(5000);
}
bool insertNewOrdine(int posizione){
    display.clearDisplay();
    display.setCursor(0,10);
    display.println("ORDINE IN INVIO");
    display.display();
   int httpcode=0;
   String result;
   while(httpcode!=HTTP_CODE_OK){
   httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=gestordini.insert&mwid="+codiceMW+"&posizione="+posizione);
   httpcode=httpclient.GET();
   result=httpclient.getString();
   httpclient.end();
   }
   Serial.println("inserimento: "+result);
   if (result=="1"){
    display.setCursor(0,20);
    display.println("ORDINE INVIATO");
     display.setCursor(0,30);
    display.println("PUOI RILASCIARE");
    display.display();
    delay(1000);
    return true; 
   }else{
    return false;
   }
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
    wifiManager.autoConnect("CONNETTI MARKETWALL");
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
  display.println("cerca tra le tue ");
  display.setCursor(10,30);
  display.println("WIFI:");
  display.setCursor(10,40);
  display.println("CONNETTI MARKETWALL");
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
  Serial.println("codice letto: "+codice_finale.substring(0,7));
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

String getUser(String codiceMW){
int httpcode;
  String utente="";
  while(utente.length()<2 || httpcode!=HTTP_CODE_OK){
  httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.getuser&mwid="+codiceMW);
                           
  httpcode=httpclient.GET();
  //Serial.println("httpcode: "+String(httpcode));
  utente=httpclient.getString();
    httpclient.end();

  }
  return utente;
}

String getUserName(String utente){
int httpcode;
  String username="";
  Serial.println("ID: "+utente);
  while(httpcode!=HTTP_CODE_OK){
  httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.getusername&userid="+utente);
                           
  httpcode=httpclient.GET();
  
  username=httpclient.getString();
    httpclient.end();

  }
  return username;
}
void resetting(){
 
  display.clearDisplay();
  EEPROM.write(0,0);
 WiFi.disconnect();
   display.setCursor(0,0);
  display.println("RESET DONE");
  display.display();
  int i=0;
  while(i<1){
    
  }
  
}
void foundNewmc(){
  String valore;
  int httpcode=0;
  String valori[36];
//VIENE COSTRUITO L'ARRAY DEI NUOVI MC
  
  while(httpcode!=HTTP_CODE_OK){
    //Serial.println(codiceMW);
    int bgn=httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.checkmc&mwid="+codiceMW);
    //Serial.println("getbgn: "+String(bgn));
    httpcode=httpclient.GET();
   
    //Serial.println("httpcode: "+String(httpcode));
    //Serial.println("getString: "+httpclient.getString());
    valore=httpclient.getString().c_str();//MI RACCOMANDO LA CONVERSIONE IN CHAR ALTRIMENTI JSONBUFFER NON FUNZIONA...
    
   
    httpclient.end();

    }

  int i=0;
  
    while(valore.length()>2){
    int cut=valore.indexOf("}")+1;
    valori[i]=valore.substring(valore.indexOf("{"),cut);
    //Serial.println(valore);
    //Serial.println(String(i)+" "+valori[i]);
    valore.remove(0,cut);
    i++;
    }
  
//PER CIASCUN NUOVO MC VIENE ASSOCIATO UN LED E UNA SCRITTA
  for(int j=0;j<i;j++){
    JsonObject& root = jsonBuffer.parseObject(valori[j]);
    //Serial.println("j:"+String(j));
      for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
      if(String(it->value.as<char*>()).substring(0,5)=="reset"){
        resetting();  
      }
      digitalWrite(ledpin,1);  
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("posiziona il nuovo MC presso il led");
      display.setCursor(0,20);
      display.println(String(it->key));
      display.setCursor(0,30);
      display.println(String(it->value.as<char*>()));
      display.display();
      delay(500);

      }
      
  while(buttonState==LOW){
    Serial.println ("in attesa...");
      buttonState = digitalRead(buttonPin);
      delay(500);
  }    
  bool ver1s=verifica1sec();
  if(ver1s==true){
      Serial.println ("posizione inserita");
      insertPositionConfirm();
  }
    

  }
//display.clearDisplay();
//display.display();
}
void foundNewPromo(){

   String valore;
  int httpcode=0;
  String valori[36];
//VIENE COSTRUITO L'ARRAY DEI NUOVI MC
  
  while(httpcode!=HTTP_CODE_OK){
    //Serial.println(codiceMW);
    int bgn=httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.getpromozioni&mwid="+codiceMW);
    //Serial.println("getbgn: "+String(bgn));
    httpcode=httpclient.GET();
   
    //Serial.println("httpcode: "+String(httpcode));
    //Serial.println("getString: "+httpclient.getString());
    valore=httpclient.getString().c_str();//MI RACCOMANDO LA CONVERSIONE IN CHAR ALTRIMENTI JSONBUFFER NON FUNZIONA...
    
   
    httpclient.end();

    }

  int i=0;
  
    while(valore.length()>2){
    int cut=valore.indexOf("}")+1;
    valori[i]=valore.substring(valore.indexOf("{"),cut);
    //Serial.println(valore);
    //Serial.println(String(i)+" "+valori[i]);
    valore.remove(0,cut);
    i++;
    }
  
//PER CIASCUN NUOVO MC VIENE ASSOCIATO UN LED E UNA SCRITTA
  for(int j=0;j<i;j++){
    JsonObject& root = jsonBuffer.parseObject(valori[j]);
    //Serial.println("j:"+String(j));
      for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
      if(String(it->value.as<char*>()).substring(0,5)=="reset"){
        resetting();  
      }
      digitalWrite(ledpin,1);  
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("PROMOZIONE!");
     
      display.setCursor(0,20);
      display.println(String(it->value.as<char*>()));
      display.display();
      delay(500);

      }
      
  while(buttonState==LOW){
    Serial.println ("in attesa...");
      buttonState = digitalRead(buttonPin);
      delay(500);
  }    
  bool ver1s=verifica1sec();
  if(ver1s==true){
      Serial.println ("grazie");
      insertPromoConfirm();
  }
    

  }
//display.clearDisplay();
//display.display();
  
}
