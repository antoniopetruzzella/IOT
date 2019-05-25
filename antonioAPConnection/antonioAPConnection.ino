
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>

int MILLIS_NEXT_IRRIGATION_PERIOD=10000;//10 SECONDI
HTTPClient httpclient;
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
   // wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
        //wifiManager.autoConnect("AutoConnectAP");
        WiFi.begin("WebPocket-E036","antonio71");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();
   //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
Serial.println(WiFi.localIP()); 
    
   
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    
  int httpcode;
   while(httpcode!=HTTP_CODE_OK){
    
    httpclient.begin("http://www.heritagexperience.com/raspberryWS/raspberryloaddataWS.php?action=insertdata&descrizione=temperatura-camera&valueFloat=23");
    Serial.println("waiting get");                     
    httpcode=httpclient.GET();
                   
   }
   Serial.println(httpcode);                    
  delay(1000);


  }
}
