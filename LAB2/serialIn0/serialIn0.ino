/*
  Serial Call and Response in ASCII
 Language: Wiring/Arduino
 
 This program sends an ASCII A (byte of value 65) on startup
 and repeats that until it gets some data in.
 Then it waits for a byte in the serial port, and 
 sends three ASCII-encoded, comma-separated sensor values, 
 truncated by a linefeed and carriage return, 
 whenever it gets a byte in.
 
 Thanks to Greg Shakar and Scott Fitzgerald for the improvements
 
  The circuit:
 * potentiometers attached to analog inputs 0 and 1 
 * pushbutton attached to digital I/O 2
 
 
 
 Created 26 Sept. 2005
 by Tom Igoe
 modified 24 Apr 2012
 by Tom Igoe and Scott Fitzgerald
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/SerialCallResponseASCII
 
 */

//  working variables
int inByte = 0;
char b[3];    //  index 0 is on the lhs
int i = 0;
int x = 0;
int y = 0;


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
  // if we get a valid byte, read and display stuff:
  if (Serial.available() > 0) 
  {
    
    //  read up to 3 characters - put them into array b
    Serial.readBytesUntil('\n', b, 3);

    //  Test output
    
      //  convert to integers from characters
      x = b[1]-'0';
      y = b[0]-'0'; 
     
      //  display on the serial monitor
      Serial.print(b[1]);
      Serial.println(b[0]);
      
      Serial.print(x);
      Serial.println(y);
      
      delay(100);
  }
    
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
    delay(300);
  }
}
