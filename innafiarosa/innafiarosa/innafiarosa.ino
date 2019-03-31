void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(2,OUTPUT);
pinMode(3,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(3)==HIGH){
      digitalWrite(2, HIGH);
}else{
  digitalWrite(2, LOW );
}
Serial.println("uscita:"+String(digitalRead(2)));
Serial.println("entrata:"+String(digitalRead(3)));
delay(500);
}
