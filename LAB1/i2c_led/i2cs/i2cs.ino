#include <Wire.h>
int LED = 8; //define the 8th digital pin for LED brick
void setup()
{
  pinMode(LED,OUTPUT); //set the LED pin for digital output
  Wire.begin(4); 				// join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600); 			// start serial for output
}
void loop()
{
  delay(100);
}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 <Wire.available()) 	// loop through all but the last
  {
    char c =Wire.read(); 		// receive byte as a character
    Serial.print(c); 			// print the character
  }
  
  int x = Wire.read(); 			// receive byte as an integer
  Serial.println(x); 			// print the integer
  if(x == 1)
    digitalWrite(LED,HIGH); // light the LED
  else
    digitalWrite(LED,LOW); // light the LED
}
