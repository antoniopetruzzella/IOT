int POMPAPIN=D6;
int MILLIS_IRRIGATION_PERIOD=5000;
void setup() {
  // put your setup code here, to run once:
 
 Serial.begin(9600);
 pinMode(POMPAPIN,OUTPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("giro");
  digitalWrite(POMPAPIN,1);
  delay(MILLIS_IRRIGATION_PERIOD);  
  digitalWrite(POMPAPIN,0);
  delay(MILLIS_IRRIGATION_PERIOD);
}
