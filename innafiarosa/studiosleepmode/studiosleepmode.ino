
void setup() {
  delay(3000);
  Serial.begin(9600);
  pinMode(D6,INPUT);
  Serial.println(ESP.getResetReason());
  if(ESP.getResetReason()=="External System"){
    Serial.println("faccio cose...");
    ESP.deepSleep(5000000);
  }
 
while(Serial){
  Serial.println("sono sveglio");
  ESP.deepSleep(5000000);

}

}

void loop() {
  // put your main code here, to run repeatedly:

}
