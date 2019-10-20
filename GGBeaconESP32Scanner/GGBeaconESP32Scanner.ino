/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 2; //In seconds
BLEScan* pBLEScan;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  BLEAdvertisedDevice nearestDevice;
  int majorRSSI=-9999;
  for(int i=0; i<foundDevices.getCount();i++){
    BLEAdvertisedDevice foundDevice=foundDevices.getDevice(i);
    char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)foundDevice.getManufacturerData().data(), foundDevice.getManufacturerData().length());
    String manufacturerData = pHex;
    String UUID = manufacturerData.substring(8, 40);
    
    if(UUID=="c336aa38054bb0483b0ae75027061982"){
      if(foundDevice.getRSSI()>majorRSSI){
        majorRSSI=foundDevice.getRSSI();
        nearestDevice=foundDevice;  
      }
    }
  }
  char *pHexNearest=BLEUtils::buildHexData(nullptr, (uint8_t*)nearestDevice.getManufacturerData().data(), nearestDevice.getManufacturerData().length()); 
  String NearestManufacturerData=pHexNearest;
  String NearestMajor=NearestManufacturerData.substring(42,44);
  //char hexMajorValue[2];
  //hexMajorValue[1]=NearestMajor.substring(0,1);
  //hexMajorValue[2]=NearestMajor.substring(1,2);
  //int NearestMajorInt=strtol(hexMajorValue,0,16);
  int NearestMajorInt=NearestMajor.toInt();
  Serial.println("NEAREST MAJOR: "+ String(NearestMajorInt));  
  
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(2000);
}
