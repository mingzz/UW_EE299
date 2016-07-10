//  working variables
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
//-----------------------------------------------------------------
int inByte = 0;
char b[5];    //  index 0 is on the lhs
int x = 0;
int y = 0;
char op;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  establishContact();  // send a byte to establish contact until receiver responds 
}

void loop()
{
  // if we get a valid byte, read and display stuff:
  if (Serial.available() > 0) 
  {
    lcd.clear();
    //  read up to 3 characters - put them into array b
    Serial.readBytesUntil('\n', b, 4);
    // convert char to int
    x = b[0]-'0';
    // print operand 1
    Serial.print(x);
    lcd.print(x);
    op = b[1];
    // print operator
    Serial.print(op); 
    lcd.print(op);  
    // convert char to int   
    y = b[2]-'0'; 
    // print operand 2
    Serial.println(y);
    lcd.print(y);      
    // calculate
    int result = Calc(x,y,op);
    // print
    lcd.print('=');
    Serial.print("The result is : "); 
    Serial.println(result);
    lcd.setCursor(0, 1);
    lcd.print(result);  
    delay(100);
  }  
}

// calculate integar data
int Calc(const int x, const int y, const char op)
{
   if (op == '+')
      return x+y;
   else if(op == '-')
          return x-y;
      else if(op == '*')
            return x*y;
        else if(op == '/')
               return x/y;
}

// establish serial contact
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}
