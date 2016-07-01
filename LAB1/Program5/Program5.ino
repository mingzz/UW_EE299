int Button = 9; //define the 9th digital pin for button brick
int LED = 8; //define the 8th digital pin for LED brick
void setup()
{
pinMode(LED,OUTPUT); //set the LED pin for digital output
pinMode(Button,INPUT); //set the Button pin for digital input
}
void loop()
{
if (digitalRead(Button)) // if button press
digitalWrite(LED,HIGH); // light the LED
else // if not press
digitalWrite(LED,LOW); // turn off the LED
}
