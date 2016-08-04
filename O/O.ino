//This is the code for player"O"!!!!

#include <Wire.h>
#include <LiquidCrystal.h>
// working variables
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
bool btnFlag1 = false;
bool btnFlag2 = false;
boolean _up1=false;
boolean _up2=false;
int Button1 = 11;
int Button2 = 12;
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
char a = 'O', b = 'X';//a means my chess; b means the opponent's chess
int timer1_counter;
int count1=50;
bool flag_buzzer;
int speakerOut1=9;
int flag3=0;
bool flag4=0;

void receiveEvent(int howMany);
void mov(const char turn);
void serial_monitor(char pos[8][8]);
void transmit(void);
int operation(void);
int count_x(void);
int count_o(void);
void endgame(void);
void SetClock();
void LCDupdate();
int button(bool state, int order);

void setup() {

  Serial.begin(115200);
  
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  lcd.begin(16,2);
  Wire.begin();
  SetClock();
  //monitor function
  serial_monitor(pos);
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
          Serial.println("Invalid position input.");
          flag_judge++;
          pos[x][y] = '.';
        }
        else 
        {
          transmit();
          if(count_x() == 0 || count_o() == 0 || chess == 64)
            endgame();
          flag = 0;//it's the opponent's turn to play
          donePut = true;
        }
      }
      else Serial.println("There is already a chess on the position.");// there is already a chess on the position
    }
    if(flag4 == 0 ){
      Serial.println("Position received!");
      Serial.print("The position input is (");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.println(").");
      Serial.println(' ');
      serial_monitor(pos);
    }else{
      transmit();
      Serial.println(' ');
      flag4 = 0;
    }
  }
  delay(400);
}

void mov(const char turn){
  switch (turn){
    case 'x':
      Serial.println("Please input x");
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
        Serial.println("Please input y");
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

void receiveEvent(int howMany)
{
  while (0 < Wire.available() )
  {
    char c = Wire.read();
    if(c == '\r'){
      Serial.println("It's your turn to play.");
      flag = 1;//it's time for you to play
      flag_judge = 0;//initialize the times of invalid inputs
    }
    if(c == '\n')//receiving the position graph
    {
      i = Wire.read();//record the line number
      for(j=0;j<8;j++)
      {
        c = Wire.read();
        pos[i][j] = c;
      }
      if(i==7)
      {
        j = Wire.read();
        Serial.print("The opponent's input position is (");
        Serial.print(j);
        j = Wire.read();
        Serial.print(",");
        Serial.print(j);
        Serial.println(").");
        flag = 1;//it's time for you to play
        flag_judge = 0;//initialize the times of invalid inputs
        chess++;
        if(count_x() == 0 || count_o() == 0 || chess == 64)
          endgame();
        serial_monitor(pos);
        Serial.println("It's your turn to play.");
        Serial.flush();
      }
    }
  }
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

void serial_monitor(char pos[8][8])
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
}

void transmit(void)
{
  if(flag4 == 1){
    Wire.beginTransmission(9);
    Wire.write('\r');
    Wire.endTransmission();
  }else
  {
    for(i=0;i<10;i++)
          {
            Wire.beginTransmission(9);
            Wire.write('\n');// the flag for the O_player to recieve data in one line
            Wire.write(i);
            for(j=0;j<8;j++)
            {
              Wire.write(pos[i][j]);
            }
            if(i==7)
            {
              Wire.write(x);
              Wire.write(y);
            }
            Wire.endTransmission();
          }
     chess++;
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
  serial_monitor(pos);
  if(count_x() > count_o())
    Serial.println("Lose!!!");
  else if(count_x() < count_o())
    Serial.println("Win!!!");
  else
    Serial.println("Draw!!!");
  Serial.end();
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
    count1=30;
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
  lcd.setCursor(0,1);
  lcd.print("O: ");
  lcd.print(count1);
  //}
}

