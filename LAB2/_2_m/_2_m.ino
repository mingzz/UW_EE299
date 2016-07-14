#include <Wire.h>
//-----------------------------------------------------------------
//  working variables
int inByte = 0;
char b[10];    //  index 0 is on the lhs
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
  for(int j=0; j<10; ++j)
  {
    b[j] = 0;
  }
  x=y=0;
  int opPos, endPos;
  // if we get a valid byte, read and display stuff:
  if (Serial.available() > 0) 
  {
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
    Serial.print(op);
    Serial.println(y); 
    // get result    
    int result = Calc(x,y,op);
        // i2c transmission
    Trans(x,y,op,result);
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



// tansmit data to the slave
int Trans(int x,int y,const  char op,int result){
  // sign bit is needed to inform the slave
  int x_signal=0;
  int y_signal=0;
  int result_signal=0;
  Wire.beginTransmission(4);   // transmit to device #4
  // set sign bit
  if(x<0)
  {
    x_signal=1;
    x=x*-1;
  }
  if(y<0)
  {
    y_signal=1;
    y=y*-1;
  }
  //int result_signal = (result<0) ? 1 : 0; 
  if(result<0)
  {
    result_signal=1;
    result=result*-1;
  }
  int x_1=x%255;
  int x_2=x/255;
  int y_1=y%255;
  int y_2=y/255;
  int result_1=result%255;
  int result_2=result/255;  
  Wire.write(x_1);
  Wire.write(x_2);       
  Wire.write(y_1);
  Wire.write(y_2); 
  Wire.write(result_1);
  Wire.write(result_2); 
  //signal of x, 1 means negative
  Wire.write(x_signal);
  //signal of y
  Wire.write(y_signal); 
  //signal of result
  Wire.write(result_signal); 
  Wire.write(op);       
  Wire.endTransmission();     // stop transmitting
}


