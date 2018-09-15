int tempolamp;
int ledpin;

void setup() {
  // put your setup code here, to run once:
  ledpin=D7;
  Serial.begin(115200);

   /* switch on led */
   pinMode(ledpin, OUTPUT);
   tempolamp=1000;

}

void loop() {
  // put your main code here, to run repeatedly:
  
   digitalWrite(ledpin, LOW);
   Serial.println("low");
   delay(tempolamp);
   digitalWrite(ledpin, HIGH);
   Serial.println("high");
   delay(tempolamp);
}
