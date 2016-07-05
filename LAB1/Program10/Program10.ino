#include <LiquidCrystal.h>
int Button1 = 9; //define the 9th digital pin for button brick
int Button2 = 10;
//int LED = 8; //define the 8th digital pin for LED brick
int i = 7;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
void setup()
{
 Serial.begin(9600);
// pinMode(LED,OUTPUT); //set the LED pin for digital output
 pinMode(Button1,INPUT); //set the Button pin for digital input
 pinMode(Button2,INPUT); //set the Button pin for digital input
 lcd.begin(16, 2);
  // Print a message to the LCD.
 lcd.setCursor(7, 0);
 lcd.print("X");
}
void loop()
{
  //Right
 if (digitalRead(Button1)) // if button press
 {
   delay(100);
   if (!digitalRead(Button1))
   {
      //  digitalWrite(LED,HIGH); // light the LED
     // if(i <= 16 )
        i = i + 1;
        Serial.print(i);
        lcd.clear();
        lcd.setCursor(i, 0);
        lcd.print("X");
        if (i >=16) 
        {
          lcd.setCursor(0, 0);
          lcd.print("Right Win!");
          i = 7;
        }
   }
 }

   //Left
 if (digitalRead(Button2)) // if button press
 {
   delay(100);
   if (!digitalRead(Button2))
   {
      //  digitalWrite(LED,HIGH); // light the LED
     // if(i <= 16 )
        i = i - 1;
        Serial.print(i);
        lcd.clear();
        lcd.setCursor(i, 0);
        lcd.print("X");
        if (i <=0) 
        {
          lcd.setCursor(0, 0);
          lcd.print("Left Win!");
          i = 7;
        }
   }
 }

 
   // Turn off the display:
  // Turn on the display:
  lcd.display();
} 
