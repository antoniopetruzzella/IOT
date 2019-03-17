int latchPin = 4;
int clockPin = 3;
int dataPin = 2;
int buttonPin=7;
int buttonsNumber=3;
unsigned char positionByte[4]={17,34,68,136};//136,68,34,17
unsigned char buttonsByte[4]={1,2,4,8};//136,68,34,17

int newMcPosition;

void setup()
{
 Serial.begin(9600);
 pinMode(latchPin, OUTPUT);
 pinMode(clockPin, OUTPUT);
 pinMode(dataPin, OUTPUT);
 pinMode(buttonPin,INPUT);
 newMcPosition=5; 
  
}

void loop()
{

  azzeraShiftRegister();
 
 // foundNewMC();
  buttonClickedEventHandler();
  delay(1000); // Wait for 1000 millisecond(s)
}

void foundNewMC(){

  if(newMcPosition<5){
    while(digitalRead(buttonPin)==0){
      int value=positionByte[newMcPosition];
      digitalWrite(latchPin, LOW);
      Serial.println(value);
      shiftOut(dataPin, clockPin, MSBFIRST, value);
      digitalWrite(latchPin, HIGH);
      //Serial.println("in attesa conferma posizione "+String(newMcPosition));
      Serial.println(digitalRead(buttonPin));
      delay(50);
     
    }
    Serial.println(digitalRead(buttonPin));
      newMcPosition=5;
        Serial.println("ok inserimento posizione "+String(newMcPosition));
      
         
  }

}



void azzeraShiftRegister(){
  int value=8;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    digitalWrite(latchPin, HIGH);
}

void buttonClickedEventHandler(){

  int value=0;
  
  for(int i=0;i<buttonsNumber;i++){
    value=buttonsByte[i];
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    digitalWrite(latchPin, HIGH);
    int buttonState = digitalRead(buttonPin);
      if (buttonState==1) {

        //bool ver3s=verifica3sec();
        //if(ver3s==true){
          Serial.println ("faccio partire l'ordine prodotto in pos:"+String(i));
          
      //}
    }
  }
}

bool verifica3sec(){
 
  int i=0;
  while(i<3){
     Serial.println ("dentro ver 3");
    int buttonState = digitalRead(buttonPin);
    if(buttonState==LOW){
      return false;
    }
    i++;
    delay(1000);
  }
  return true;
}
