/*
 * QUESTO SKETCH CREA UN SEMPLICISSIMO BLE ADVERTISER SERVER PER LA APP
 *  *****   ANTOBLESCAN ****
 */

#include <ArduinoBLE.h>


void setup() {
  Serial.begin(9600);
  //while (!Serial);


  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("GGBN2");//GGBN1-GGBN2
  
 BLE.advertise();

  //Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
   
}
