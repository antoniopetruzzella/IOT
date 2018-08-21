#include <DHT_U.h>
#include <DHT.h>

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#define DHTTYPE DHT11
// DHT Sensor
const int DHTPin = 5;
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);
static char celsiusTemp[7];


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
        wifiManager.autoConnect("AutoConnectAP");
        //WiFi.begin("WebPocket-E036","antonio71");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    dht.begin();
    
   
}

void loop() {
    // put your main code here, to run repeatedly:
     delay(300000);
     String temperatura(dht.readTemperature());
     String umidita(dht.readHumidity());
     //Serial.println(dht.readTemperature(),6,2,celsiusTemp);
     Serial.println(temperatura);  
     Serial.println(umidita);     
      HTTPClient http;
      
      http.begin("http://www.heritagexperience.com/raspberryWS/raspberryloaddataWS.php?action=insertdata&descrizione=temperatura-camera&valueFloat="+temperatura);
      int httpCode = http.GET(); 
      if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
     http.end(); 
     http.begin("http://www.heritagexperience.com/raspberryWS/raspberryloaddataWS.php?action=insertdata&descrizione=umidita-camera&valueFloat="+umidita);
      int httpCode2 = http.GET(); 
      if (httpCode2 > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }
 
    http.end(); 
// Serial.println(WiFi.localIP());
}
