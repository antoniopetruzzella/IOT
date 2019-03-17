int latchPin = 4;
int clockPin = 3;
int dataPin = 2;
int buttonPin=7;
int buttonsNumber=6;
unsigned char positionByte[4]={17,34,68,136};//136,68,34,17
unsigned char buttonsByte[6]={1,2,4,8,16,32};//136,68,34,17

int newMcPosition;

void setup()
{
 Serial.begin(9600);
 pinMode(latchPin, OUTPUT);
 pinMode(clockPin, OUTPUT);
 pinMode(dataPin, OUTPUT);
 pinMode(buttonPin,INPUT);
 newMcPosition=1; 
 //setShiftRegister(4);
  
}

void loop()
{

  azzeraShiftRegister();

  buttonClickedEventHandler();
  delay(10); // Wait for 1000 millisecond(s)
}


void setShiftRegister(int value){
  
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    digitalWrite(latchPin, HIGH);
}


void azzeraShiftRegister(){
  int value=0;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    digitalWrite(latchPin, HIGH);
}

void buttonClickedEventHandler(){

  int value=0;
  
  for(int i=0;i<buttonsNumber;i++){
    value=buttonsByte[i];
    Serial.println(value);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, value);
    digitalWrite(latchPin, HIGH);

    
    delay(2000);
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
