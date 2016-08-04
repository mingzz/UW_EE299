//This is the code for player"X"!!!!

#include <Wire.h>
#include <LiquidCrystal.h>
char pos[8][8]={'.','.','.','.','.','.','.','.',
                  '.','.','.','.','.','.','.','.',
                  '.','.','.','.','.','.','.','.',
                  '.','.','.','X','O','.','.','.',
                  '.','.','.','O','X','.','.','.',
                  '.','.','.','.','.','.','.','.',
                  '.','.','.','.','.','.','.','.',
                  '.','.','.','.','.','.','.','.',};
/*char pos[10][10]={'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','O','O','O','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',
                  'X','X','X','X','X','X','X','X','X','.',};*/
char pos_temp[8][8];
bool btnFlag1 = false;
bool btnFlag2 = false;
boolean _up1=false;
boolean _up2=false;
int Button1 = 11;
int Button2 = 12;
char incomingByte;
bool flag_pos = 0;// decide whether the number input is x-axis or y-axis
int x=0, y=0;// position of the input
int Direction = 0;// means 8 different directions
int temp_x, temp_y;
int i, j, count;
bool flag_valid_position = 0;//shows whether the position you input is valid or not  
bool flag = 1;//1 means you are playing, 0 means the opponent is playing
int flag_judge = 0;
int chess = 4;
char a = 'X', b = 'O';//a means my chess; b means the opponent's chess
int timer1_counter;
int count1=50;
bool flag_buzzer;
int speakerOut1=9;
int flag3=0;
bool flag4=0;
LiquidCrystal lcd(2,3,4,5,6,7,8);
int Mercury_tilt = 12;

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978


//Mario main theme melody
int melody1[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo1[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};


void mov(const char turn);
void sing(int melody[], int tempo[]);
void buzz(int targetPin, long frequency, long length);

int operation(void);
int count_x(void);
int count_o(void);
void endgame(void);
void SetClock();
void LCDupdate();
int button(bool state, int order);
void BroadcastPorcess();
int ReceiveProcess();
int isPosValid(int Posi, int Posj);
void PRINT();

void setup() {
  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  Serial.begin(9600);
  SetClock();
  //Wire.begin(9);
  //Wire.onReceive(receiveEvent);
  lcd.begin(16,2);
  //Wire.begin();
  //pinMode(speakerOut1,OUTPUT);
  //pinMode(Mercury_tilt,OUTPUT);
  //monitor function
  //serial_monitor(pos);
  PRINT();
}

void loop() {
  /*
  if(flag4 == 1)
  {
    transmit();
    flag4=0;
  }
  */
  if(flag == 1)
  {
    flag_valid_position = 0;
    bool donePut = false;
    
    while(donePut!=true){
      mov('x');
      mov('y');
      if(flag4 == 1)break;
      if(pos[x][y]=='.')
      {
        operation();
        if(flag_valid_position == 0)//if there is no 'O' chess having been reversed, invalid input!
        {
          //Serial.println("Invalid position input.");
          flag_judge++;
          pos[x][y] = '.';
        }
        else 
        {
          BroadcastPorcess();
          if(count_x() == 0 || count_o() == 0 || chess == 64)
            endgame();
          flag = 0;//it's the opponent's turn to play
          donePut = true;
        }
      }
      //else 
      //Serial.println("There is already a chess on the position.");// there is already a chess on the position
    }

    if(flag4 == 0 ){
      /*Serial.println("Position received!");
      Serial.print("The position input is (");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.println(").");
      Serial.println(' ');
      serial_monitor(pos);*/
    }else{
      BroadcastPorcess();
      //Serial.println(' ');
      flag4 = 0;
    }
  }
  else
  {
      while(!ReceiveProcess());
  }
  delay(400);
}

void mov(const char turn){
  switch (turn){
    case 'x':
      Serial.println("please input x");
      do 
      {
        if(flag4 == 1)break;
        if(button(digitalRead(Button2),2))
        {
          x = (x+1)%8;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(x);
          lcd.print(" <-");
          lcd.setCursor(8,0);
          lcd.print(y);
          //while(digitalRead(Button2));
        }
      }while(!button(digitalRead(Button1),1));
      break;
    case 'y':
        Serial.println("please input y");
        do 
        {
          if(flag4 == 1)break;
          if(button(digitalRead(Button2),2))
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(x);
            lcd.setCursor(8,0);
            y= (y+1)%8;
            lcd.print(y);
            lcd.print(" <-");
            //while(digitalRead(Button2));
          }
        }while(!button(digitalRead(Button1),1));
        break;
     default:
      break;
  }
}


void sing(int melody[], int tempo[]) {

  //Serial.println(" 'Mario Theme'");
  int siz = sizeof(melody1) / sizeof(int);
  for (int thisNote = 0; thisNote < siz; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / tempo[thisNote];
    //Serial.println(melody[thisNote]);
    buzz(speakerOut1, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    buzz(speakerOut1, 0, noteDuration);
  }
}

void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }

}

int operation(void)
{
  for(Direction=0;Direction<8;Direction++)
      {
        temp_x = x;
        temp_y = y;//find a copy of x & y
        count = 0; // initialize the count
        switch(Direction)
        {
          case 0://consider the right path
            do 
            {
              count++;
              temp_y++;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=y;i<temp_y;i++)
                  pos[temp_x][i]=a;
                flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;
          
          case 1://consider the left path
            do 
            {
              count++;
              temp_y--;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=y;i>temp_y;i--)
                  pos[temp_x][i]=a;
                flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;
          
          case 2://consider the down path
            do 
            {
              count++;
              temp_x++;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=x;i<temp_x;i++)
                  pos[i][temp_y]=a;
                flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;
          
          case 3://consider the up path
            do 
            {
              count++;
              temp_x--;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=x;i>temp_x;i--)
                  pos[i][temp_y]=a;
                flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;
            
          case 4://considering the right-down path
            do 
            {
              temp_y++;
              temp_x++;
              count++;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=0;i<count;i++)
                  pos[x+i][y+i]=a;
                  flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;

          case 5://considering the right-up path
            do 
            {
              temp_y++;
              temp_x--;
              count++;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=0;i<count;i++)
                  pos[x-i][y+i]=a;
                  flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;

          case 6://considering the left-up path
            do 
            {
              temp_y--;
              temp_x--;
              count++;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=0;i<count;i++)
                  pos[x-i][y-i]=a;
                  flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;

          case 7://considering the left-down path
            do 
            {
              temp_y--;
              temp_x++;
              count++;
              if(pos[temp_x][temp_y]==a && count > 1 && temp_x >=0 && temp_x <=7 && temp_y >=0 && temp_y <=9)// reverse all the 'b' between the 2 'a's!!!
              {
                for(i=0;i<count;i++)
                  pos[x+i][y-i]=a;
                  flag_valid_position = 1;
              }
              // if(pos[][]==a && count == 1) or if(pos[][]=='.') nothing happens
            }
            while(pos[temp_x][temp_y]==b);//continue only when getting a 'b' in the path
            break;
          
          default:;
        }
      }
   if(flag_valid_position == 0)//if there is no 'b' chess having been reversed, invalid input!
        pos[x][y] = '.';
   return flag_valid_position;
}

/*void copy(char pos1[8][8], char pos2[8][8])//copy pos1 to pos2
{
  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    {
      pos2[i][j] = pos1[i][j];
    }
  }
}*/

/*int judge(void)
{
  Serial.println("In judge.");
  if(count_x()==0)
    return 0;
  copy(pos,pos_temp);
  int a = 0;// the flag of whether there is valid input
  for(x=0;x<8;i++)
  {
    for(y=0;y<8;j++)
    {
      if(pos[x][y] == '.')
      {
        a = operation();
        if (a == 1)
        {
          copy(pos_temp,pos);
          return 1;
        }
      }
    }
  }
  Serial.println("Out Judge.");
  return 0;
}*/

int count_x(void)
{
  int cnt = 0;//count the number
  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    {
      if(pos[i][j]=='X')
        cnt++;
    }
  }
  return cnt;
}

int count_o(void)
{
  int cnt = 0;//count the number
  for(i=0;i<8;i++)
  {
    for(j=0;j<8;j++)
    {
      if(pos[i][j]=='O')
        cnt++;
    }
  }
  return cnt;
}

void endgame(void)
{
  //serial_monitor(pos);
  if(count_x() > count_o())
  {
    Serial.println("Win!!!");
    sing(melody1,tempo1);
  }
  else if(count_x() < count_o()){
    Serial.println("Lose!!!");
  }
  else
    Serial.println("Draw!!!");
  //Serial.end();
}

void SetClock()
{
  noInterrupts();
  TCCR1A=0;
  TCCR1B=0;
  timer1_counter=3036;
  TCNT1=timer1_counter;
  TCCR1B|=(1<<CS12);
  TIMSK1|=(1<<TOIE1);
  interrupts();
}

ISR(TIMER1_OVF_vect)
{
  TCNT1=timer1_counter;
  if(flag3 == 1)
  {
    digitalWrite(speakerOut1,LOW);
    flag3=0;
  }
  if(flag)
  {
  count1=count1-1;
  if(count1==-1)
  {
  flag=1-flag;
  flag4=1;
  //transmit();
  }
  }
  else
  {
    
  }
  if(flag_buzzer == flag)
  {
    flag_buzzer = flag;  
  }
  else
  {
    count1=50;
    digitalWrite(speakerOut1,HIGH);
    flag3=1;
    flag_buzzer = flag;
  }
  LCDupdate();
  //Serial.println(count);
}

void LCDupdate(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(x);
  lcd.setCursor(8,0);
  lcd.print(y);
  //if(flag)
  //{
  lcd.setCursor(0,1);
  lcd.print("X: ");
  lcd.print(count1);

}

int button(bool state, int order){
  bool *flag = false;
  bool *_up = false;
  if(order == 1){
    flag = &btnFlag1;
    _up = &_up1;
  }else{
    flag = &btnFlag2;
    _up = &_up2;
  }
  if(state== LOW)
    *flag = true;
  if(state == HIGH && (*flag == true))
  {
    *_up = true;
    *flag = false; 
  }
  if(*_up){
    *_up=false;
    return 1;
  }
  else
    return 0;
}

//Broadcast the information
void BroadcastPorcess()
{
    if(flag4 == 1)
    {
        Serial.println('P');
    }
    else
    {
        Serial.print('\t');  //The signal that transmission starts
    
        Serial.println(' ');
        Serial.print('*');
        for(i=0;i<8;i++)
        {
            Serial.print(' ');
            Serial.print(i);
        }
        Serial.println(' ');
        for(i=0;i<8;i++)
        {
            Serial.print(i);
            for(j=0;j<8;j++)
            {
                Serial.print(' ');
                Serial.print(pos[i][j]);
            }
            Serial.println(' ');
        }

        Serial.println("O:");
        Serial.println('\t');  //The signal that transmission end
    } 
}

//During the time when the opponent is playing, show what is needed.
int ReceiveProcess()
{
    int i, j;
    //char TransBuffer
    char ReceiveChar = 0; //store the character received
   // int ReceiveFlag = 0;
    
    if(Serial.available())
    {
        ReceiveChar = Serial.read();
        //Serial.print(ReceiveChar);
    }
    if(ReceiveChar == '\t')
    {
        //Serial.println("Come in");
        i = j = 0;
        while((ReceiveChar = Serial.read()) != '\t')
        {
           // Serial.println("Second floor");
           // if(ReceiveChar != '\t' && ReceiveChar != '\r')
           //     Serial.print(ReceiveChar);  //Print->
            Serial.print(ReceiveChar);  //Print->
            if(ReceiveChar == '.' || ReceiveChar == 'O' || ReceiveChar == 'X')
            {
                if(isPosValid(i, j))
                {
                    pos[i][j] = ReceiveChar;
                    ++j;
                    if(j >= 8 && i < 8)
                    {
                        ++i;
                        j = 0;
                    }
                }  
            }
        }
        flag = 1; //Time to play
        flag_judge = 0;//initialize the times of invalid inputs
        chess++;
        return 1;
    }
    else if(ReceiveChar == 'P')
    {
        flag = 1; //Time to play
        flag_judge = 0;//initialize the times of invalid inputs
        return 1;
    }
    return 0;
}

int isPosValid(int Posi, int Posj)
{
    if(Posi < 0 || Posi >= 8 || Posj < 0 || Posj >= 8)
        return 0;
    else
        return 1;
}


void PRINT()
{
        Serial.println(' ');
        Serial.print('*');
        for(i=0;i<8;i++)
        {
            Serial.print(' ');
            Serial.print(i);
        }
        Serial.println(' ');
        for(i=0;i<8;i++)
        {
            Serial.print(i);
            for(j=0;j<8;j++)
            {
                Serial.print(' ');
                Serial.print(pos[i][j]);
            }
            Serial.println(' ');
        }

        Serial.println("X:");
}

