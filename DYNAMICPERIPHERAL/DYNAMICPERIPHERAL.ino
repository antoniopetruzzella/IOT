//QUESTO SKETCH RICEVE PARAMETRI TRAMITE LE CHARACTERISTICS DEI SERVICES CON NFRC CONNECT
// PER ARDUINO NANO 33 BLR SENSE
// I VALORI SI LEGGONO NELLA SER
#include <ArduinoBLE.h>
BLEService firstService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEUnsignedIntCharacteristic firstCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic secondCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic thirdCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic fourtyCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite);
void ConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}
void DisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  BLE.advertise();
}
void setup() {
  Serial.begin(9600);
   if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  BLE.setEventHandler(BLEConnected, ConnectHandler);
  BLE.setEventHandler(BLEDisconnected, DisconnectHandler);
  BLE.setLocalName("ANTO_PERIPH");
  BLE.setAdvertisedService(firstService);
  firstService.addCharacteristic(firstCharacteristic);
  firstService.addCharacteristic(secondCharacteristic);
  firstService.addCharacteristic(thirdCharacteristic);
  firstService.addCharacteristic(fourtyCharacteristic);
  BLE.addService(firstService);
  firstCharacteristic.setEventHandler(BLEWritten, firstCharacteristicWritten);
  secondCharacteristic.setEventHandler(BLEWritten, secondCharacteristicWritten);
  thirdCharacteristic.setEventHandler(BLEWritten, thirdCharacteristicWritten);
  fourtyCharacteristic.setEventHandler(BLEWritten,fourtyCharacteristicWritten);

  BLE.advertise();
  Serial.println(("Bluetooth device active, waiting for connections..."));
  Serial.println("fine setup");
}

void loop() {
   BLE.poll();

  delay(500);
}


void firstCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
 
}
void secondCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {

    
}
void thirdCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {

  
  
}
void fourtyCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
 
  
}
