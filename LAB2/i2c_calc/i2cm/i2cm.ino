#include <Wire.h>
//-----------------------------------------------------------------
//  working variables
int inByte = 0;
char b[15];    // store input expression
int x = 0;     // operand 1
int y = 0;     // operand 2
char op;       // operator
void setup()
{
  Wire.begin();         // join i2c bus (address optional for master)
  Serial.begin(9600);       // start serial for output
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
  // always initialize the array
  for(int j=0; j<15; ++j)
  {
    b[j] = 0;
  }
  // initialize the operands
  x=y=0;
  // working variables for processing input array
  int startPos, opPos, endPos, negPos, negFlag;
  // initialize these local variables
  startPos = 0;
  negFlag = 0;
  opPos = 100;
  negPos = 100;
  // if we get a valid byte, read and display stuff:
  if (Serial.available() > 0) 
  {
    //  read up to 15 characters - put them into array b
    Serial.readBytesUntil('\n', b, 15);
    // whether operand 1 is negative or not 
    if (b[0]=='-')
    {
        // if negative, set startPos and negFlag as 1
        startPos = 1;
        negFlag = 1;
    }
    // print start position of numeric data
    Serial.print("startPos is:");
    Serial.println(startPos);
    // traverse
    for (int i = 0;i<15;i++)
    {
      // if negative, skip
      if(startPos && negFlag )
      {
        negFlag = 0;
        continue; 
      }
      // get the operator
      if ( b[i]=='+'|| 
            b[i]=='-'||
            b[i]=='*'||
            b[i]=='/')
      {
        // detect and store the second '-' when operand 2 is negative 
        if(opPos == 100)
          opPos = i;
        else
          negPos = i;
      }
      // stop signal
      if ( b[i]=='=')
      {
        endPos=i;
      }
    }
    // print operator position and end position
    Serial.print("opPos is:");
    Serial.println(opPos);
    Serial.print("endPos is:");
    Serial.println(endPos);
    // convert char to int and get x
    if(startPos == 1)
    {
        x=CharToInt(1, opPos, x);    //negative
        x = -1 * x;
    }
    else
      x=CharToInt(0, opPos, x);     //positive
    // convert char to int and get y      
    if( negPos != 100)
    {
        y = CharToInt(opPos+2, endPos, y);  // negative
        y = -1 * y;
    }
    else
        y = CharToInt(opPos+1, endPos, y);  // positive
    // get operator
    op = b[opPos];
    // print operands and operator
    Serial.print(x);
    Serial.print(op);      
    Serial.println(y);
    // calculate
    int result = Calc(x,y,op);
    // i2c transmission
    Trans(x,y,op,result);
    //serial output
    Serial.print("The result is : "); 
    Serial.println(result);
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
  for(int k=front; k<final; k++)
  {
    target = 10*target + (b[k]-'0');
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

// tansmit data to the slave
int Trans(const int x,const int y,const  char op,const int result){
  // sign bit is needed to inform the slave
  int x_signal=0;
  int y_signal=0;
  Wire.beginTransmission(4);   // transmit to device #4
  // set sign bit
  if(x<0)
  {
    x_signal=1;
  }
  if(y<0)
  {
    y_signal=1;
  }
  int result_signal = (result<0) ? 1 : 0; 
  Wire.write(x);      
  Wire.write(y);
  Wire.write(result);
  //signal of x, 1 means negative
  Wire.write(x_signal);
  //signal of y
  Wire.write(y_signal); 
  //signal of result
  Wire.write(result_signal); 
  Wire.write(op);       
  Wire.endTransmission();     // stop transmitting
}

