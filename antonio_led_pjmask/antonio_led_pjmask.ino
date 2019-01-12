#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#define PIN D6
#define NUMPIXELS 60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

WiFiManager wifiManager;
struct Pjmask{

  String nome;
  int red;
  int green;
  int blue;
  };
HTTPClient httpclient;  
bool applicationState=1;
int brightness=65;
Pjmask pjmasks[4];
int Showid,Brightness,Speed;
String Pjmaskname,Frompjmask,Topjmask;
DynamicJsonBuffer jsonBuffer;
  

void setup()  {
  Serial.begin(9600);
  wifiManager.setConfigPortalTimeout(10);//QUESTA RIGA EVITA CHE I TENTATIVI DI RICONENSSIONE VENGANO BLOCCATI DALLA RI-CONFIGURAZIONE TRAMITE PORTALE. 10 E' EVIDENTEMENTE UN VALORE DA DEBUG
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");
  Serial.println(WiFi.localIP()); 
  pjmasks[0].nome="gufetta";pjmasks[0].red=255;pjmasks[0].green=0;pjmasks[0].blue=0;
  pjmasks[1].nome="geco";pjmasks[1].red=0;pjmasks[1].green=255;pjmasks[1].blue=0;
  pjmasks[2].nome="gattoboy";pjmasks[2].red=0;pjmasks[2].green=0;pjmasks[2].blue=255;
  pjmasks[3].nome="connection_error";pjmasks[3].red=102;pjmasks[3].green=51;pjmasks[3].blue=0;
  Showid=0;
  Pjmaskname="0";
  Frompjmask="0";
  Topjmask="0";
  Brightness=100;
  Speed=1000;
  pixels.begin();
  pixels.show();
  pixels.setBrightness(20);
  Serial.println("fine setup");
}


void loop() {
   while (WiFi.status() != WL_CONNECTED){
    Serial.println("mancanza d connessione");
    staticPjmask("connection_error",100);
   
   
   }
  if(applicationState==1){

    //fadingPjs(pjmaskname,"gufetta",_speed,200);
    //staticPjmask("gattoboy",lumxcent);
    //pathPjmask(pjmaskname,_speed,_bidir);
    //fillingPjmask(pjmaskname,_speed,_bidir);
    //deflatingPjmask(pjmaskname,_speed,_bidir);
    //fantasy();
    playCurrentShow();
 
 
  }else{
    stopAllShows();
  }
  delay(500);
}

void playCurrentShow(){
  
  int httpcode;
  int showid;
  String pjmaskname;
  int brightness;
  int _speed_;
  String frompjmask;
  String topjmask;
  Serial.println("call");
  while(httpcode!=HTTP_CODE_OK){
  httpclient.begin("http://www.heritagexperience.com/pjmask/controller.php?action=getcurrentshow");
  Serial.println("waiting get");                     
  httpcode=httpclient.GET();
  
  JsonObject& root = jsonBuffer.parseObject(httpclient.getString().c_str());
  
  Serial.println("root returned");
    if(root.size()>0){
    const char* _showid=root["showid"];
    const char* _pjmaskname=root["pjmaskname"];
    const char* _brightness=root["brightness"];
    const char* _speed=root["speed"];
    const char* _frompjmask=root["frompjmask"];
    const char* _topjmask=root["topjmask"];
    showid=(String(_showid)).toInt();
    pjmaskname=String(_pjmaskname);
    brightness=(String(_brightness)).toInt();
    _speed_=(String(_speed)).toInt();
    frompjmask=String(_frompjmask);
    topjmask=String(_topjmask);
    }
  }
  
   
  if(showid!=Showid || pjmaskname!=Pjmaskname || brightness!=Brightness || _speed_!=Speed || frompjmask!=Frompjmask || topjmask!= Topjmask){

    Showid=showid;
    Pjmaskname=pjmaskname;
    Frompjmask=frompjmask;
    Topjmask=topjmask;
    Brightness=brightness;
    Speed=_speed_;
    stopAllShows();
  
  
  
  
  Serial.println("showid "+String(showid));
  httpclient.end();
  /*delete _showid;
  delete _pjmaskname;
  delete _brightness;
  delete _speed;
  delete _frompjmask;
  delete _topjmask;*/
  }
  
  switch(showid){

    case 1:
      staticPjmask(pjmaskname,brightness);
      break;

    case 2:
      pathPjmask(pjmaskname,_speed_,false);
       break;
    
    case 3:
      fillingPjmask(pjmaskname,_speed_,false);  
       break;
    
    case 4:
      deflatingPjmask(pjmaskname,_speed_,false);
      break;  

    case 8:
      //fadingPjs(frompjmask, topjmask, _speed_, 1000);
      fantasy();
      break;   
    }


}

void stopAllShows(){

   for (int x = 0; x < NUMPIXELS; x++) {
      pixels.setPixelColor(x, 0, 0, 0);
    }
    pixels.show();
  
}

void staticPjmask(String PjmaskName, int _lumxcent){
  
  Pjmask pjmask=findPjmaskFromName(PjmaskName);
  for (int x = 0; x < NUMPIXELS; x=x+int(100/_lumxcent)) {
      pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    }
    pixels.show();
   
}

void pathPjmask(String PjmaskName, int _speed, bool bidir){
  Pjmask pjmask=findPjmaskFromName(PjmaskName);
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, 0, 0, 0);
  }
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    if(x>0){
      pixels.setPixelColor(x-1, 0, 0, 0);
    }
    pixels.show();
    delay(_speed);
  }
  if(bidir==true){
    for (int x=NUMPIXELS; x>0; x--){
      pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
      if(x>0){
        pixels.setPixelColor(x+1, 0, 0, 0);
      }
      pixels.show();
      delay(_speed);
    }
  }
  
}

void fillingPjmask(String PjmaskName, int _speed, bool bidir){
  Pjmask pjmask=findPjmaskFromName(PjmaskName);
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, 0, 0, 0);
  }
  pixels.show();
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    pixels.show();
    delay(_speed);
  }
  if(bidir==true){
    for (int x=NUMPIXELS; x>0; x--){
      pixels.setPixelColor(x, 0, 0, 0);
      pixels.show();
      delay(_speed);
    }
  }
  
}

void deflatingPjmask(String PjmaskName, int _speed, bool bidir){
  Pjmask pjmask=findPjmaskFromName(PjmaskName);
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
  }
  pixels.show();
  for (int x=NUMPIXELS; x>0; x--){
      pixels.setPixelColor(x, 0, 0, 0);
      pixels.show();
      delay(_speed);
    }
  if(bidir==true){
    for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    pixels.show();
    delay(_speed);
    }
   
  }
  
}
void fadingPjs(String startPjmaskName, String endPjmaskName, int vel, int passi){
  
  Pjmask startPjmask=findPjmaskFromName(startPjmaskName);
  Pjmask endPjmask=findPjmaskFromName(endPjmaskName);
  fade(startPjmask.red,startPjmask.green,startPjmask.blue,endPjmask.red,endPjmask.green,endPjmask.blue,vel,passi);
  
}

Pjmask findPjmaskFromName(String _nome){

  int i;
  for (i=0;i<=2;i++){
    if(pjmasks[i].nome==_nome){
      return  pjmasks[i];
    }
  }
}



void fade(int r1, int g1, int b1, int r2, int g2, int b2, int fadeRate, int steps) {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  for (int i = 1; i < steps; i++)
  {
    red = (((r1 * (steps - i)) + (r2 * i)) / steps);
    green = (((g1 * (steps - i)) + (g2 * i)) / steps);
    blue = (((b1 * (steps - i)) + (b2 * i)) / steps);
    // Sets the pixels to the color adjusted in the fade
    for (int x = 0; x < NUMPIXELS; x++) {
      pixels.setPixelColor(x, red, green, blue);
    }
    pixels.show();
    delay(fadeRate);
  }
  delay(5000);
}

void fantasy(){
  int red,green,blue;
  for (int x = 0; x < NUMPIXELS; x++) {
    int randompjmask=random(1,4);
    switch(randompjmask){
      case 1:
        red=255;
        green=0;
        blue=0;
        break;
     case 2:
        red=0;
        green=255;
        blue=0;
        break;
     case 3:
        red=0;
        green=0;
        blue=255;
        break;      
    }
    //int randompixel=random(0,255)
      pixels.setPixelColor(x, red, green, blue);
      delay(100);
      pixels.show();
    }
    
    delay(100);
    stopAllShows();
}
