#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SHOW_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define PJMASKNAME_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"  

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
 Serial.begin(115200);
 BLEDevice::init("PjMaskESP32");
 BLEServer *pServer = BLEDevice::createServer();
 BLEService *pService = pServer->createService(SERVICE_UUID);
 BLECharacteristic ShowCharacteristic(SHOW_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
 //BLECharacteristic PjmaskCharacteristic(PJMASKNAME_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_WRITE);

  ShowCharacteristic.setCallbacks(new MyCallbacks());
  //PjmaskCharacteristic.setCallbacks(new MyCallbacks());
  ShowCharacteristic.setValue("0");
  pService->addCharacteristic(&ShowCharacteristic);
  //PjmaskCharacteristic.setValue("1");
  //pService->addCharacteristic(&PjmaskCharacteristic);
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

}

void loop() {
  // put your main code here, to run repeatedly:

}
