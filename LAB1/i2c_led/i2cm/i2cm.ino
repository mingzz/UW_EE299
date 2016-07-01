#include <Wire.h>
int Button = 8; //define the 9th digital pin for button brick
void setup()
{
  pinMode(Button,INPUT); //set the Button pin for digital input
  Wire.begin(); 				// join i2c bus (address optional for master)
  Serial.begin(9600); 			// start serial for output
}

byte x = 0;

void loop()
{
  Wire.beginTransmission(4);   // transmit to device #4
  if (digitalRead(Button)) // if button press
  {
      Wire.write("LED is ON");       // sends five bytes
      Wire.write(1);        // sends one byte
      Serial.println("LED is ON");
  } 
  else
  {
      Wire.write("LED is OFF");       // sends five bytes
      Wire.write(0);        // sends one byte
      Serial.println("LED is OFF");
  }

  Wire.endTransmission(); 		// stop transmitting
  delay(500);
}
