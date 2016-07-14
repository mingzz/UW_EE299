#include <Wire.h>
#include <LiquidCrystal.h>

int a[11]; // store data get from the master
int cnt = 0; // position cursor of a[10]
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
  char op = Wire.read();      // receive byte as an integer

  if(a[6] == 1)
  {
    //a[0]=a[0]-256;      // a[0], operand 1 is negative
    int x=-1*(a[0]+a[1]*255);
    lcd.print(x);
  }
  else
  {
    int x=a[0]+a[1]*255;
    lcd.print(x);
  }
  lcd.print(op);  //print operator
  if(a[7] == 1)
  {
    int y=-1*(a[2]+a[3]*255);
    lcd.print(y);
  }
  else
  {
    int y=a[2]+a[3]*255;
    lcd.print(y);
  }
  lcd.print('=');
  lcd.setCursor(0, 1);
  if(a[8] == 1)
  {
    int z=-1*(a[4]+a[5]*255);
    lcd.print(z);
  }
  else
  {
    int z=a[4]+a[5]*255;
    lcd.print(z);
  }
  // print result on lcd
  // reset data array and count 
  for (int i = 0; i<11; i++){
    a[i]=0;   
  }
  cnt = 0;
}
