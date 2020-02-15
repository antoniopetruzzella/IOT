//QUESTO SKETCH RICEVE PARAMETRI TRAMITE LE CHARACTERISTICS DEI SERVICES CON NFRC CONNECT
// PER ARDUINO NANO 33 BLR SENSE
// I VALORI SI LEGGONO NELLA SER
#include <ArduinoBLE.h>
//QUESTO SKETCH FA FUNZIONARE LA ADAFRUIT LED STRIP SU ARDUINO NANO 33 BLE SENSE
// Adafruit NeoPixel - Version: Latest 
#include <Adafruit_NeoPixel.h>

/*
ATTENZIONE: LA LIBRERIA ADAFRUIT E' STATA MODIFICATA SECONDO QUANTO FIXED QUI:
https://github.com/adafruit/Adafruit_NeoPixel/pull/218/files
PER POTER FUNZIONARE SU ARDUINO NANO VA COMMENTATA LA r. 1419 PERCHE' PWM[device]->ENABLE E' SEMPRE 1 SU NANO
IN OGNI CASO SENZA RIMUOVERE IL COMMENTO LA LIBRERIA FUNZIONA PERFETTAMENTE ANCHE SU WIFI1010
*/
#define PIN 6
#define NUMPIXELS 60
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
BLEService pjmaskService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEUnsignedIntCharacteristic showCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic pjmaskCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic brightnessCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic speedCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite);
struct Pjmask{

  String nome;
  int red;
  int green;
  int blue;
  };
bool applicationState=1;
int brightness=65;
Pjmask pjmasks[4];
int Showid,Brightness,Speed,Pjmaskindex;

void setup() {
  Serial.begin(9600);
   if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  BLE.setLocalName("PJMASKNANO");
  BLE.setAdvertisedService(pjmaskService);
  pjmaskService.addCharacteristic(showCharacteristic);
  pjmaskService.addCharacteristic(pjmaskCharacteristic);
  pjmaskService.addCharacteristic(brightnessCharacteristic);
  pjmaskService.addCharacteristic(speedCharacteristic);
  BLE.addService(pjmaskService);
  showCharacteristic.setEventHandler(BLEWritten, showCharacteristicWritten);
  pjmaskCharacteristic.setEventHandler(BLEWritten, pjmaskCharacteristicWritten);
  brightnessCharacteristic.setEventHandler(BLEWritten, brightnessCharacteristicWritten);
  speedCharacteristic.setEventHandler(BLEWritten,speedCharacteristicWritten);
  showCharacteristic.setValue(0);
  pjmaskCharacteristic.setValue(0);
  BLE.advertise();
  Serial.println(("Bluetooth device active, waiting for connections..."));
  pjmasks[0].nome="gufetta";pjmasks[0].red=255;pjmasks[0].green=0;pjmasks[0].blue=0;
  pjmasks[1].nome="geco";pjmasks[1].red=0;pjmasks[1].green=255;pjmasks[1].blue=0;
  pjmasks[2].nome="gattoboy";pjmasks[2].red=0;pjmasks[2].green=0;pjmasks[2].blue=255;
  pjmasks[3].nome="connection_error";pjmasks[3].red=102;pjmasks[3].green=51;pjmasks[3].blue=0;
  Showid=0; 
  Brightness=100;
  Speed=30;
  pixels.begin();
  pixels.show();
  pixels.setBrightness(20);
  Serial.println("fine setup");
}

void loop() {
   BLE.poll();
  if(applicationState==1){

     playCurrentShow();
 
 
  }else{
    stopAllShows();
  }
  delay(500);
}
void playCurrentShow(){
   

  
  switch(Showid){

    case 1:
      staticPjmask();
      break;

    case 2:
      pathPjmask(false);
       break;
    
    case 3:
      fillingPjmask(false);  
       break;
    
    case 4:
      deflatingPjmask(false);
      break;  

    case 8:
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

void staticPjmask(){
  
  pixels.show();
  Pjmask pjmask=pjmasks[Pjmaskindex];
  for (int x = 0; x < NUMPIXELS; x=x+int(100/Brightness)) {
      pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    }
  pixels.show();
  delay(1000);
}
void pathPjmask(bool bidir){
  
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, 0, 0, 0);
  }
  for (int x=0; x<NUMPIXELS; x++){
    Pjmask pjmask=pjmasks[Pjmaskindex];
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    if(x>0){
      pixels.setPixelColor(x-1, 0, 0, 0);
    }
    pixels.show();
    delay(Speed);
    BLE.poll();
  }
  if(bidir==true){
    for (int x=NUMPIXELS; x>0; x--){
      Pjmask pjmask=pjmasks[Pjmaskindex];
      pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
      if(x>0){
        pixels.setPixelColor(x+1, 0, 0, 0);
      }
      pixels.show();
      delay(Speed);
      BLE.poll();
    }
  }
  
}
void fillingPjmask(bool bidir){
  ;
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, 0, 0, 0);
  }
  pixels.show();
  for (int x=0; x<NUMPIXELS; x++){
    Pjmask pjmask=pjmasks[Pjmaskindex];
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    pixels.show();
    delay(Speed);
    BLE.poll();
  }
  if(bidir==true){
    for (int x=NUMPIXELS; x>0; x--){
      Pjmask pjmask=pjmasks[Pjmaskindex];
      pixels.setPixelColor(x, 0, 0, 0);
      pixels.show();
      delay(Speed);
      BLE.poll();
    }
  }
  
}
void deflatingPjmask(bool bidir){
  Pjmask pjmask=pjmasks[Pjmaskindex]; 
  for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
  }
  pixels.show();
  for (int x=NUMPIXELS; x>0; x--){
      pixels.setPixelColor(x, 0, 0, 0);
      pixels.show();
      delay(Speed);
      BLE.poll();
    }
  if(bidir==true){
    for (int x=0; x<NUMPIXELS; x++){
    pixels.setPixelColor(x, pjmask.red, pjmask.green, pjmask.blue);
    pixels.show();
    delay(Speed);
    BLE.poll();
    }
   
  }
  
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

void showCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("modificato Show: ");
  Serial.println(String(showCharacteristic.value()));
  Showid=showCharacteristic.value();
  playCurrentShow();//SHOWID E' L'UNICO VALORE CHE SE CAMBIATO DA REMOTO, RIAVVIA LO SHOW
}
void pjmaskCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("modificato PJmask: ");
  Serial.println(String(pjmaskCharacteristic.value()));
  Pjmaskindex=pjmaskCharacteristic.value();
    
}
void brightnessCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("modificata Brightness: ");
  Serial.println(String(brightnessCharacteristic.value()));
  Brightness=brightnessCharacteristic.value();
  if(Showid==1){
    stopAllShows();//
  }
  
}
void speedCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("modificata Speed: ");
  Serial.println(String(speedCharacteristic.value()));
  Speed=speedCharacteristic.value();
  
}
