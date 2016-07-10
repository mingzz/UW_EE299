//  working variables
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
//-----------------------------------------------------------------
//  working variables
int inByte = 0;
char b[10];    //  index 0 is on the lhs
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
  for(int j=0; j<10; ++j)
  {
    b[j] = 0;
  }
  x=y=0;
  int opPos, endPos;
  // if we get a valid byte, read and display stuff:
  if (Serial.available() > 0) 
  {
    lcd.clear();
    //  read up to 3 characters - put them into array b
    Serial.readBytesUntil('\n', b, 15);
    // traverse
    for (int i = 0;i<15;i++)
    {
      //  get operator
      if ( b[i]=='+'|| 
            b[i]=='-'||
            b[i]=='*'||
            b[i]=='/')
      {
        opPos=i;
      }
      if ( b[i]=='=')
      {
        endPos=i;
      }
    }
    // convert char to int
    x=CharToInt(0, opPos, x);
    y=CharToInt(opPos+1, endPos, y);
    op = b[opPos];
    // print on monitor
    Serial.print(x);
    lcd.print(x);
    Serial.print(op);
    lcd.print(op);      
    Serial.println(y);
    lcd.print(y);  
    // get result    
    int result = Calc(x,y,op);
    Serial.print("The result is : "); 
    Serial.println(result);
    lcd.print('=');
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

// an atoi() implementation, convert char to int
int CharToInt(const int front, const int final, int target)
{
  for(int i=front; i<final; i++)
  {
    target = 10*target + (b[i]-'0');
  }
  return target;
}

// establish serial contact
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}
