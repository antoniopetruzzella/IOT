

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
DynamicJsonBuffer jsonBuffer;

void setup()
{
  //DICHIARAZIONI
   EEPROM.begin(10);
  //EEPROM.write(0,0);//RESET FORZATO!!
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
  ledpin=D7;
  buttonPin = D6;
  pinMode(ledpin, OUTPUT);
  digitalWrite(buttonPin,LOW);
  pinMode(buttonPin, INPUT);
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

  }


}

void loop() {
  Serial.println("passo");

  if (WiFi.status() == WL_CONNECTED) {
  digitalWrite(D7,0);
  foundNewmc();
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

  

int buttonState = digitalRead(buttonPin);
Serial.println(String(buttonState));
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState ==LOW) {
    
    //digitalWrite(ledPin, LOW);
    Serial.println("pulsante non premuto");
   
    
    
  }  
  if (buttonState ==HIGH) {
   
    Serial.println("pulsante premuto");
    insertPositionConfirm(1);
    bool result=verifica3sec(buttonPin);
    if(result==true){
      Serial.println ("faccio partire l'ordine");
    }else{
      Serial.println("ordine non partito");
    }
  }
  
}
bool verifica3sec(int pinButton){

  int i=0;
  while(i<3){

    if(digitalRead(pinButton)==HIGH){
      return false;
    }
    i++;
    delay(1000);
  }
  return true;
}

void insertPositionConfirm(int posizione){
   int httpcode=0;
   while(httpcode!=HTTP_CODE_OK){
   httpclient.begin("http://www.heritagexperience.com/mw/index.php?option=com_marketwall&task=marketwalltask.confirmmcinsertion&mwid="+codiceMW+"&posizione="+posizione);
   httpcode=httpclient.GET();
   httpclient.end();
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

      delay(500);

      }

  }
//display.clearDisplay();
//display.display();
}
