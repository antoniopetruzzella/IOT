/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using a WiFi shield.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the WiFi.begin() call accordingly.

 Circuit:
 * WiFi shield attached

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 */


#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;    
const char * host = "192.168.1.143";//"192.168.0.139";
const uint16_t port = 37777;
int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(192,168,0,139);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;
int setThrusterLevelButtonPin=2;
int sensorPin = A1; 
float lastvalueForThruster=0.0;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(setThrusterLevelButtonPin,INPUT);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
 
}

void changeMFD(){
   client.println("ORB:OPENMFD:0:1");
   
   
}

void setThrusterLevel(float value){
 if (client.connect(host, port)) {
      
  if(client.connected()){//GESTIRE TUTTE LE ECCEZIONI POSSIBILI...
    client.println("SHIP:FOCUS:SetEngineGrpLevel:0:"+String(value));
    Serial.println("comando inviato:SHIP:FOCUS:SetEngineGrpLevel:0:"+String(value));
  }
  client.stop();
 }
}

void loop() {

   //client.println("SHIP:FOCUS::EngineGrpLevels");
   //Serial.println("SHIP:FOCUS::EngineGrpLevels:"+client.read());
 /*  if(digitalRead(setThrusterLevelButtonPin)){
    setThrusterLevel(1);
    Serial.println("inviato comando setThrusterLevel a Orbiter");
   }*/
  int analogValue=analogRead(sensorPin); 
  float valueForThruster=float(analogValue)/1024;
  Serial.println("valore letto;"+String(valueForThruster)+" valore precedente:"+String(lastvalueForThruster));
  if(lastvalueForThruster!=valueForThruster){
  setThrusterLevel(valueForThruster);
  lastvalueForThruster=valueForThruster;
  }
  delay(100);
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
