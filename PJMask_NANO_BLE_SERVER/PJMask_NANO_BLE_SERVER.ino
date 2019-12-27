#include <ArduinoBLE.h>

BLEService pjmaskService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEUnsignedIntCharacteristic showCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic pjmaskCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite);
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
  BLE.addService(pjmaskService);
  showCharacteristic.setEventHandler(BLEWritten, showCharacteristicWritten);
  pjmaskCharacteristic.setEventHandler(BLEWritten, pjmaskCharacteristicWritten);
  showCharacteristic.setValue(0);
  pjmaskCharacteristic.setValue(0);
  BLE.advertise();
  Serial.println(("Bluetooth device active, waiting for connections..."));
   
}

void loop() {
   BLE.poll();

}
void showCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("modificato Show: ");
  Serial.println(String(showCharacteristic.value()));
}
void pjmaskCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  Serial.print("modificato PJmask: ");
  Serial.println(String(pjmaskCharacteristic.value()));
}
