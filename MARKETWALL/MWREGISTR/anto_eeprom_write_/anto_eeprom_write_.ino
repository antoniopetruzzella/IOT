/*
 * EEPROM Write
 *
 * Stores values read from analog input 0 into the EEPROM.
 * These values will stay in the EEPROM when the board is
 * turned off and may be retrieved later by another sketch.
 */

#include <EEPROM.h>

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
int addr = 0;

void setup()
{
  EEPROM.begin(512);
  Serial.begin(9600);
  int a=random(1000000);
  Serial.println("codice scritto "+String(a));
  String str=String(a);
  
  char codice[7];
  str.toCharArray(codice,7);
  for(int i=0;codice[i];i++){

  //  Serial.println(codice[i]);
    EEPROM.write(addr+i,(uint8_t)codice[i]);
    
  }
EEPROM.commit();
char buf[7];

for (int ii=0;codice[ii];ii++){
buf[ii]=(char)EEPROM.read(ii);
//Serial.println(buf[ii]);

}

String codice_finale=(String)buf;

Serial.println("codice letto: "+codice_finale.substring(0,str.length()));
  //
  //char ch[9]='123456789';
  //int xVal=3456;
  //EEPROM.write(addr, highByte(xVal)); //write the first half
  //EEPROM.write(addr+1, lowByte(xVal)); //write the second half
  /*int byte0=xVal&255; // con un prodotto logico per 255 ottengo gli 8 bit meno significativi. 
  int byte1=(xVal>>8)&255; //con uno spostamento a sinistra ed il prodotto logico ottengo gli 8 bit più 
  int byte2=(xVal>>16)&255;
  int byte3=(xVal>>32)&255;
  EEPROM.write(addr, byte0);
  EEPROM.write(addr+1, byte1);
  EEPROM.write(addr+2, byte2);
  EEPROM.write(addr+3, byte3);*/
  //intoa(xVal,ch,10);
  /*
  for(i=0;ch[i],i++){
    Serial.println(ch[i]);
    
    }
  }*/
  //
}

void loop()
{


  //delay(10000);
   //
  /*
  //byte high = EEPROM.read(addr); //read the first half
  //byte low = EEPROM.read(addr+1); //read the second half
  int byte0=EEPROM.read(addr);
  int byte1=EEPROM.read(addr+1);
  int byte2=EEPROM.read(addr+1);
  int byte3=EEPROM.read(addr+1);
  int xVal=(byte3<<32)|(byte2<<16)|(byte1<<8)|byte0; // somma logica del valore in byte1 spostato a sinistra di 8 bit e byte0 *
  
  //int xVal = (high << 8) + low; //reconstruct the integer
  Serial.println(byte0);
  Serial.println(byte1);
  Serial.println(byte2);
  Serial.println(byte3);

  delay(1000);*/
}
