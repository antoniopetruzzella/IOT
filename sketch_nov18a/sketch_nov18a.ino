#include <SPI.h>
#include "MFRC522.h"
#define RST_PIN D3 // RST-PIN for RC522 - RFID - SPI - Modul GPIO15 
#define SS_PIN  D8  // SDA-PIN for RC522 - RFID - SPI - Modul GPIO2
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance
void setup() {
  //pinMode(D8,INPUT_PULLUP);
  Serial.begin(9600);    // Initialize serial communications
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522 
  Serial.println(F("Read personal data on a MIFARE PICC:"));
 
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    //Serial.print(F("no card detected"));
    //ShowReaderDetails();
    //byte mem = SPI.transfer(0xB0);
    //Serial.println(mem);
    delay(500);
    
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
   Serial.println(F("**Card Detected:**"));
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }

}

void ShowReaderDetails() {
  // Get the MFRC522 software version
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown),probably a chinese clone?"));
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
    Serial.println(F("SYSTEM HALTED: Check connections."));
    // Visualize system is halted
    
    while (true); // do not go further
  }
}
