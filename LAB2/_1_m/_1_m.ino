#include <Wire.h>
//  working variables
//-----------------------------------------------------------------
int inByte = 0;
char b[5];    //  index 0 is on the lhs
int x = 0;
int y = 0;
char op;

void setup()
{
  Wire.begin();         // join i2c bus (address optional for master)

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
    //  read up to 3 characters - put them into array b
    Serial.readBytesUntil('\n', b, 4);
    // convert char to int
    x = b[0]-'0';
    // print operand 1
    Serial.print(x);
    op = b[1];
    // print operator
    Serial.print(op);  
    // convert char to int   
    y = b[2]-'0'; 
    // print operand 2
    Serial.println(y);     
    // calculate
    int result = Calc(x,y,op);
    // print
    Serial.print("The result is : "); 
    Serial.println(result);
    // i2c transmission
    Trans(x,y,op,result);
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
