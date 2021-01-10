


//needed for library
#include <WiFi.h>
     
#include <HTTPClient.h>
#include <ArduinoJson.h>
String valori[36];
void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    //WiFiManager wifiManager;
    WiFi.begin("WebPocket-E036","antonio71");
    Serial.println("connected...yeey :)");

}

void loop() {
    // put your main code here, to run repeatedly:  
  HTTPClient http;
  DynamicJsonBuffer jsonBuffer; 
  http.begin("http://demo.ggallery.it/GGARBACK/index.php?option=com_ggne&task=remotecontroller.getstatus&RemoteDeviceID=1");
  int httpCode = http.GET(); 
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString();   //Get the request response payload
    //Serial.println(payload);            //Print the response payload
    int cut=payload.indexOf("}")+1; //QUESTA RIGA E LA SUCCESSIVA SERVONO A TOGLIERE LE DUE [] CON LE QUALI RISPONDE IL TASK SU JOOMLA
    String _payload=payload.substring(payload.indexOf("{"),cut);
    //Serial.println(_payload);
    JsonObject& root = jsonBuffer.parseObject(_payload.c_str());
    if (!root.success()) {   //Check for errors in parsing
      Serial.println("Parsing failed");
      delay(5000);
      return;
    }
      
    for (JsonObject::iterator it=root.begin(); it!=root.end(); ++it) {
        //Serial.println(String(it->key));
        if(String(it->key)=="ActualContent"){
          Serial.println(String(it->value.as<char*>()));
        }
    }
  delay(1000);
  }
 
    http.end(); 
// Serial.println(WiFi.localIP());
}
