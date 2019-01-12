#include <Wire.h>
#include <LiquidCrystal_I2C.h>//https://github.com/agnunez/ESP8266-I2C-LCD1602 !!!!
 
char row1[]="I'm Arduino";
char row2[]="Hello world!"; 
int t = 500;
// Library initialization with 16x2 size settings
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27
//LiquidCrystal_I2C lcd(0x27) ;
void setup(){
   lcd.begin(4,5);
   lcd.backlight();
   
   
   lcd.setCursor(0,1);
   lcd.print("PANTOPRAZOLO 40"); 
   lcd.setCursor(0,0);
   lcd.print("devi prendere:");
 
}
 
void loop()
{
 lcd.setCursor(0,1);
 lcd.print("PANTOPRAZOLO 40"); 
 lcd.setCursor(0,0);
 lcd.print("devi prendere:");
 delay(500);
 lcd.clear();
 delay(500);
 }
