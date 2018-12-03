#include <Adafruit_NeoPixel.h>
#define PIN D6
#define NUMPIXELS 60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Pjmask{

  String nome;
  int red;
  int green;
  int blue;
  };
bool applicationState=1;
int brightness=125;
int lumxcent=5;
Pjmask pjmasks[3];  

void setup()  {
  Serial.begin(9600);
  pjmasks[0].nome="gufetta";pjmasks[0].red=255;pjmasks[0].green=0;pjmasks[0].blue=0;
  pjmasks[1].nome="geco";pjmasks[1].red=0;pjmasks[1].green=255;pjmasks[1].blue=0;
  pjmasks[2].nome="gattoboy";pjmasks[2].red=0;pjmasks[2].green=0;pjmasks[2].blue=255;
  pixels.begin();
  pixels.show();
  pixels.setBrightness(brightness);
  Serial.println("fine setup");
}

void loop() {
  if(applicationState==1){
    
     Serial.println(String(lumxcent));
    //fadingPjs("gattoboy","gufetta",25,200);
    staticPjmask("geco",lumxcent);
    //fade(255,0,0,0,255,0,50,100);     //r1, g1, b1, r2, g2 ,b2 , fade rate , steps
    //fadeAll(0,255,0,255,0,0,50,100);
  }else{
    stopAllShows();
  }
  lumxcent=lumxcent+20;
  if(lumxcent>100){
    lumxcent=100;
  }
  delay (1000);
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
  for (int i = 1; i < steps; i++)
  {
    uint8_t red = (((r1 * (steps - i)) + (r2 * i)) / steps);
    uint8_t green = (((g1 * (steps - i)) + (g2 * i)) / steps);
    uint8_t blue = (((b1 * (steps - i)) + (b2 * i)) / steps);
    // Sets the pixels to the color adjusted in the fade
    for (int x = 0; x < NUMPIXELS; x++) {
      pixels.setPixelColor(x, red, green, blue);
    }
    pixels.show();
    delay(fadeRate);
  }
}
