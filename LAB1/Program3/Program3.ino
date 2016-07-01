
int LED = 8; //define the 8th digital pin for LED brick
void setup()
{
pinMode(LED,OUTPUT); //set the LED pin for digital output

}
void loop()
{
on(LED,2000);
off(LED,1000);
}

void on(int target,int tm)
{
digitalWrite(target,HIGH); // light the LED
delay(tm);
}

void off(int target,int tm)
{
digitalWrite(target,LOW); // turn off the LED
delay(tm);  
}
