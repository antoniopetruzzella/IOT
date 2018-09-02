
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(-1);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    
    
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10,10);
    display.clearDisplay();
    display.println("...connecting...");
    display.display();
     Serial.println("stop displaying");
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
   wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
     wifiManager.setAPCallback(configModeCallback);
    wifiManager.autoConnect("AutoConnectAP");
    //WiFi.begin("WebPocket-E036","antonio71");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

   
    
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    Serial.println(WiFi.localIP()); 
    
   
}

void loop() {
if (WiFi.status() == WL_CONNECTED) {
  display.clearDisplay();
  display.setCursor(0,30);
  display.println("connected");
  display.display();   
}else{

  display.clearDisplay();
  display.setCursor(0,30);
  display.println("not connected");
  display.display();
}
    
 delay(5000);


}

void configModeCallback (WiFiManager *myWiFiManager) {
 display.clearDisplay();
  display.setCursor(0,30);
  display.println("entra nella APP per la configurazione");
  display.display();

}

