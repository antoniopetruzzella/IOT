#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SERVICE_UUID_1        "4fafc201-1fb5-459e-8fcc-c5c9c331914c"
#define SHOW_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define PJMASKNAME_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a7"  

class ShowCallbacks: public BLECharacteristicCallbacks {
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

class PjmaskCallbacks: public BLECharacteristicCallbacks {
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
 BLEService *pService_1 = pServer->createService(SERVICE_UUID_1);
 BLECharacteristic ShowCharacteristic(SHOW_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ|BLECharacteristic::PROPERTY_WRITE);
 BLECharacteristic PjmaskCharacteristic(PJMASKNAME_CHARACTERISTIC_UUID,BLECharacteristic::PROPERTY_READ |BLECharacteristic::PROPERTY_WRITE);

  ShowCharacteristic.setCallbacks(new ShowCallbacks());
  PjmaskCharacteristic.setCallbacks(new PjmaskCallbacks());
  ShowCharacteristic.setValue("0");
  pService->addCharacteristic(&ShowCharacteristic);
  PjmaskCharacteristic.setValue("1");
  pService_1->addCharacteristic(&PjmaskCharacteristic);
  pService->start();
  pService_1->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

}

void loop() {
  // put your main code here, to run repeatedly:

}
