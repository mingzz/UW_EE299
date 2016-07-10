#include <Wire.h>
#include <LiquidCrystal.h>

int a[8]; // store data get from the master
int cnt = 0; // position cursor of a[8]
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
void setup()
{
  Wire.begin(4);         // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);       // start serial for output
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}
void loop()
{
  delay(100);
}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  // clear screen
  lcd.clear();
  while(1 <Wire.available())  // loop through all but the last
  {
    int c =Wire.read();    // receive byte as a character
    a[cnt++] = c;          // store data
  }
  char x = Wire.read();      // receive byte as an integer

  if(a[3] == 1)
  {
    a[0]=a[0]-256;      // a[0], operand 1 is negative
  }
  if(a[4] == 1)
  {
    a[1]=a[1]-256;      // a[1], operand 2 is negative
  }
  if(a[5] == 1)
  {
    a[2]=a[2]-256;      // a[3], result is negative
  }
  // print result on lcd
  lcd.print(a[0]);
  lcd.print(x);  
  lcd.print(a[1]);     
  lcd.print('=');
  lcd.setCursor(0, 1);
  lcd.print(a[2]);
  // reset data array and count 
  for (int i = 0; i<8; i++){
    a[i]=0;   
  }
  cnt = 0;
}
