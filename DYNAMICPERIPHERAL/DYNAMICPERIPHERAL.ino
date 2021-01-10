//QUESTO SKETCH RICEVE PARAMETRI TRAMITE LE CHARACTERISTICS DEI SERVICES CON NFRC CONNECT
// PER ARDUINO NANO 33 BLR SENSE
// I VALORI SI LEGGONO NELLA SER
#include <ArduinoBLE.h>
BLEService firstService("19B10000-E8F2-537E-4F6C-D104768A1214"); 
BLEUnsignedIntCharacteristic firstCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);// valore dello show
BLECharacteristic secondCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1215", BLERead | BLEWrite,20);//username
BLEUnsignedIntCharacteristic thirdCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1216", BLERead | BLEWrite);
BLEUnsignedIntCharacteristic fourtyCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1217", BLERead | BLEWrite);
unsigned long TempoInizioConnessione;
unsigned long TempoTimeOut;
String CentralExhibitAddress;
BLEDevice ConnectedCentrals[2];
int ConnectedCentralCount; //numero di central connessi ECCETTO EXHIBIT

void ConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  if (central.address()!=CentralExhibitAddress){
    ConnectedCentralCount++;
    Serial.println("central connessi: "+ String(ConnectedCentralCount));
    if(ConnectedCentralCount>1){
      central.disconnect();
      Serial.println("passo dalla disconnessione di..."+central.address());
      
    }else{
    ConnectedCentrals[ConnectedCentralCount]=central;
    TempoInizioConnessione=millis();
    }
  } else {
    Serial.println("central Exhibit Connected!!: "+ central.address());
  }
  BLE.advertise();
}
void DisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  if (central.address()!=CentralExhibitAddress){
    ConnectedCentralCount--;
  }
  Serial.println("central connessi: "+ String(ConnectedCentralCount));
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

  TempoTimeOut=(30*60*1000);
  
  CentralExhibitAddress="b8:27:eb:e2:25:ed";
  ConnectedCentralCount=0;
}

void loop() {
  BLE.poll();

  delay(500);
  unsigned long TempoNow=millis();
  //Serial.println("tempo alla fine connessione: "+String(TempoInizioConnessione+TempoTimeOut-TempoNow));
  if(TempoNow>TempoInizioConnessione+TempoTimeOut){
     
    for (int i=0;i<ConnectedCentralCount+1;i++){ // i connected central sono contati solo se non exhibit, allora devo aggiungere uno
      if(ConnectedCentrals[i].address()!=CentralExhibitAddress){
        ConnectedCentrals[i].disconnect();
      }
    }
  }
}


void firstCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {//QUI VIENE SETTATO LO SHOW

    char* val=(char*)(firstCharacteristic.value());
    String valore =String(val);
    Serial.println("cambiato show: "+valore);
}
void secondCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {//QUI VIENE SETTATO LO USERNAME
   
    char* val=(char*)(secondCharacteristic.value());
    String valore =String(val);
    
    //Serial.println(valore.substring(1,20));
}
void thirdCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {

  
  
}
void fourtyCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
 
  
}
