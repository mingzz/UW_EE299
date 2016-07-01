//  working variables
int inByte = 0;
char b[15];    //  index 0 is on the lhs
int x = 0;
int y = 0;
char op;
void setup()
{
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
  for(int j=0; j<15; ++j)
  {
    b[j] = 0;
  }
  x=y=0;
  int startPos, opPos, endPos, negPos, negFlag;
  startPos = 0;
  negFlag = 0;
  opPos = 100;
  negPos = 100;
  // if we get a valid byte, read and display stuff:
  if (Serial.available() > 0) 
  {
    //  read up to 3 characters - put them into array b
    Serial.readBytesUntil('\n', b, 15);
    if (b[0]=='-')
    {
        startPos = 1;
        negFlag = 1;
    }

    Serial.print("startPos is:");
    Serial.println(startPos);
    for (int i = 0;i<15;i++)
    {
      if(startPos && negFlag )
      {
        negFlag = 0;
        continue; 
      }
      if ( b[i]=='+'|| 
            b[i]=='-'||
            b[i]=='*'||
            b[i]=='/')
      {
        if(opPos == 100)
          opPos = i;
        else
          negPos = i;
      }
      if ( b[i]=='=')
      {
        endPos=i;
      }
    }
    Serial.print("opPos is:");
    Serial.println(opPos);
    Serial.print("endPos is:");
    Serial.println(endPos);
    if(startPos == 1)
    {
        x=CharToInt(1, opPos, x);
        x = -1 * x;
    }
    else
      x=CharToInt(0, opPos, x);
      
    if( negPos != 100)
    {
        y = CharToInt(opPos+2, endPos, y);
        y = -1 * y;
    }
    else
        y = CharToInt(opPos+1, endPos, y);

    op = b[opPos];
    Serial.print(x);
    Serial.print(op);      
    Serial.println(y);      
    int result = Calc(x,y,op);
    Serial.print("The result is : "); 
    Serial.println(result);
      
     delay(100);
  }  
}

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

int CharToInt(const int front, const int final, int target)
{
  for(int k=front; k<final; k++)
  {
    target = 10*target + (b[k]-'0');
  }
  return target;
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}
