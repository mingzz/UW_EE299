//This is the code for player"X"!!!!

#include <Wire.h>
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
int x=-1, y=-1;// position of the input
int Direction = 0;// means 8 different directions
int temp_x, temp_y;
int i, j, count;
bool flag_valid_position = 0;//shows whether the position you input is valid or not  
bool flag = 0;//1 means you are playing, 0 means the opponent is playing
int flag_judge = 0;
int chess = 4;
char a = 'O', b = 'X';//a means my chess; b means the opponent's chess

void serial_monitor(char pos[8][8])
{
  Serial.print('*');
    for(i=0;i<8;i++)
    {
      Serial.print("  ");
      Serial.print(i);
    }
    Serial.println(' ');
    for(i=0;i<8;i++)
      {
        Serial.print(i);
        for(j=0;j<8;j++)
        {
          Serial.print("  ");
          Serial.print(pos[i][j]);
        }
        Serial.println(' ');
      }
    Serial.println(' ');
    Serial.println(' ');
    Serial.println(' ');
    Serial.print("X  ");
    Serial.println(count_x());
    Serial.print("O  ");
    Serial.println(count_o());
    Serial.println(' ');
}

void transmit(void)
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

void setup() {

  Serial.begin(9600);
  
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  
  Wire.begin();
 
  //monitor function
  serial_monitor(pos);
}

void loop() {
  if(flag == 1)
  {
    if(flag_judge == 3)
    {
      transmit();
      if(count_x() == 0 || count_o() == 0 || chess == 64)
        endgame();
      flag = 0;//it's the opponent's turn to play
    }
    if(Serial.available()>0)//it only works when it's your turn to play
  {
    flag_valid_position = 0;
    do incomingByte = Serial.read();
    while(incomingByte == NULL);// problem! may cause memory problem. always loop 
    flag_valid_position = 0;//initialize the flag of valid position
    if((incomingByte-'0')<=7 && (incomingByte-'0')>=0)
    {
      if(flag_pos==0)
      {
        x = incomingByte-'0';
        flag_pos = 1;
      }
      else 
      {
        y = incomingByte-'0';
        flag_pos = 0;
        Serial.println("Position received!");
        Serial.print("The position input is (");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.println(").");
      }
    }

    if(flag_pos == 0)//put the chess only when both of the x-axis and y-axis have been prompted
    {
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
      }
    }
    else Serial.println("There is already a chess on the position.");// there is already a chess on the position

    //moniter  function
    serial_monitor(pos);
    }
  }
  }
  
}

void receiveEvent(int howMany)
{
  while (0 < Wire.available() )
  {
    char c = Wire.read();
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
        
      }
    }
  }
}

