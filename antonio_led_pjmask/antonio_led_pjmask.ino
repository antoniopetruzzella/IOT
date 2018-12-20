#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#define PIN D6
#define NUMPIXELS 60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Pjmask{

  String nome;
  int red;
  int green;
  int blue;
  };
HTTPClient httpclient;  
bool applicationState=1;
int brightness=125;
Pjmask pjmasks[4];
int Showid,Brightness,Speed;
String Pjmaskname,Frompjmask,Topjmask;
DynamicJsonBuffer jsonBuffer;
  

void setup()  {
  Serial.begin(9600);
  WiFiManager wifiManager;
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
  pixels.setBrightness(brightness);
  Serial.println("fine setup");
}


void loop() {
  /* if (WiFi.status() != WL_CONNECTED){
    staticPjmask("connection_error",255);
   }*/
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
  delay(Speed);
}

void playCurrentShow(){
  
  int httpcode;
  int showid;
  String pjmaskname;
  int brightness;
  int _speed_;
  String frompjmask;
  String topjmask;
  while(httpcode!=HTTP_CODE_OK){
  httpclient.begin("http://www.heritagexperience.com/pjmask/controller.php?action=getcurrentshow");
                       
  httpcode=httpclient.GET();
  //Serial.println("httpcode: "+String(httpcode));
  //getCurrentShow_=getJsonObjectToParse(httpclient.getString());
 JsonObject& root = jsonBuffer.parseObject(httpclient.getString().c_str());
 
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
   
  if(showid!=Showid || pjmaskname!=Pjmaskname || brightness!=Brightness || _speed_!=Speed || frompjmask!=Frompjmask || topjmask!= Topjmask){

    Showid=showid;
    Pjmaskname=pjmaskname;
    Frompjmask=frompjmask;
    Topjmask=topjmask;
    Brightness=brightness;
    Speed=_speed_;
    stopAllShows();
  }
  
  
  
  Serial.println("showid "+showid);
  httpclient.end();

  }
  
  switch(showid){

    case 1:
  
      staticPjmask(pjmaskname,brightness);
      break;
    
    }
  
}

String getStaticPjmaskname(){

  int httpcode;
  String pjmaskname;
  while(httpcode!=HTTP_CODE_OK){
  httpclient.begin("http://www.heritagexperience.com/pjmask/controller.php?action=getstaticpjmask");
  httpcode=httpclient.GET();
  pjmaskname=httpclient.getString();
  httpclient.end();
  
  }
   
  return pjmaskname;
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

void fantasy(){
  int r1=125;int g1=178; int b1=253; int r2=12; int g2=78; int b2=99;
  fade(r1,g1,b1,r2,g2,b2,5,50);
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
