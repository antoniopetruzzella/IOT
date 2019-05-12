int SLEEP_MODE_PERIOD=3600000;//UN ORA
int ACTUAL_CONNECTION_TIME=0;
int ACTUAL_IRRIGATION_TIME=0;
int NEXT_CONNECTION_PERIOD=7200000;//DUE ORE
int NEXT_IRRIGATION_PERIOD=86400000;//24 ORE
int IRRIGATION_PERIOD=10000;//10 SECONDI
void setup() {
  delay(3000);
  Serial.begin(9600);
  pinMode(D6,INPUT);//PROBABILMENTE NON SERVE, ANZI QUI DENTRO NON SERVE SICURO...
  Serial.println(ESP.getResetReason());
  if(ESP.getResetReason()=="External System"){
    Serial.println("faccio cose...");
    ESP.deepSleep(5000000);
  }
 
while(Serial){
  Serial.println("sono sveglio");
  ESP.deepSleep(SLEEP_MODE_PERIOD);

}

}

void loop() {
  // put your main code here, to run repeatedly:

}

public class AntoTime {
  
}
