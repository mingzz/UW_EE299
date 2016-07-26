#include <Wire.h>
#include <LiquidCrystal.h>
// working variables
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
int Entrance = -1; // legal:0, 3, 12, 15

// define plot structure on the map
struct Plot{
  int mine; //0 for no mine, 1 for mine
  int used; //0 for no used, 1 for used
  char show;
};
// Map used to play game on
Plot Map[4][4];
int cx, cy; // current coordinates
int ex, ey; // destination coordiantes
int HP = 2; // health point

int MineBuffer[4];  // store four mines
byte SendBuffer[4]; // send mines using byte data type
enum action{actPending, setMine, setEntrance, Play, End}actEvent; // system state event
enum sending{sndPending, Mine_TX, Entrnce_TX, Win, Lose}sndEvent; // sending state event
boolean gameResult; // game result on this side

void receiveEvent(int howMany);
void requestEvent();
void Initialization();
int GetEntrance();
int Move();
void Left();
void Right();
void Up();
void Down();
void reset (const int masterResult);
void LCDupdate();
void Line();
void Display();

void setup() {  
  Serial.begin(9600);
  Wire.begin(4);        
  Wire.onReceive(receiveEvent); // receive event
  Wire.onRequest(requestEvent); // request event
  lcd.begin(16, 2);
  // initialize two events
  sndEvent = sndPending;
  actEvent = actPending;
}

void loop() {
  switch(actEvent){
    case actPending:// do nothing
      break;
    case setMine:{  // set mines
      Initialization();
      sndEvent = Mine_TX;
      actEvent = actPending;
      Serial.println("setMine complete");
      break;
    }
    case setEntrance:{  // set entrance and start game
      while(Entrance == -1)
      {
        Entrance = GetEntrance();   //add the wait for the other people
      }
      Map[cx][cy].used = 1;
      sndEvent = Entrnce_TX;
      actEvent = Play;
      Serial.println("Start");
      LCDupdate();
      if(Map[cx][cy].mine == 1){
        HP-=1;
        //Map[cx][cy].mine = 0;
        //Map[cx][cy].show = 'o';
        Serial.println(".....You are born with a mine. It's cool.");
        LCDupdate();
      }
      Line();
      Display(); 
      break;
    }
    case Play:{ // playing
      if(Serial.available() > 0){
        if(Move() == 0){
          Line();
          Display();
        }
      }
      // two situations
      if (HP == 0){// lose
        gameResult = false;
        actEvent = End;  
      }
      else if (cx == ex && cy == ey){// win
        gameResult = true;
        actEvent = End;  
      }
      break;
  }
  case End:{ // game has ended
    if (gameResult){
      reset(false);
      sndEvent = Win;
      actEvent = actPending;
    }
    else{
      reset(true);
      sndEvent = Lose;
      actEvent = actPending;
    }
  }
  }
  delay(400);
}


void receiveEvent(int howMany)
{
  int i = 0;
  char c = Wire.read(); // receive byte as a character
  switch(c){
    case 'I':{
      while(Wire.available()>0){
        MineBuffer[i] = Wire.read();    // receive byte as an integer
        i++;
      }
      actEvent = setMine;
      break;
    }
    case 'E': actEvent = setEntrance; break;
    case 'W': reset(1); actEvent = actPending; break;// master win
    case 'L': reset(0);actEvent = actPending; break; // master lose
    case 'Z': reset(2);actEvent = actPending; break; // timeout, all lose
    default: break;
  }
  delay(400);
}

void requestEvent(){
  switch (sndEvent){
    case sndPending: break;
    case Mine_TX: {// send four mines
      Wire.write(SendBuffer,4);
      break;
    }
    case Entrnce_TX:Wire.write('E'); break; // slave has set entrance, start game
    case Win:Wire.write('W'); break;  // slave win
    case Lose:Wire.write('L'); break; // slave has been bumped up
    default:break;
  }
  sndEvent = sndPending;// reset send event every time
}



// Output: Entrance:0, 3, 12, 15
int GetEntrance(){
  Line();
  int en = 0;
  char ReceiveBuffer[2];
  int num = 0;// entrance number
  //Print the map to show the position of entrance 
  for(int i = 0; i < 4; ++i){
    for(int j = 0; j < 4; ++j){
      if(i == 0 && j == 0)  Serial.print("1 ");
      else if(i == 0 && j == 3) Serial.print("2 ");
      else if(i == 3 && j == 0) Serial.print("3 ");
      else if(i == 3 && j == 3) Serial.print("4 ");
      else Serial.print("0 ");
    }
    Serial.println(" ");
  }
  Serial.println(" ");
  Serial.println("Input the entrance: ");

  while (!Serial.available() > 0);  //wait user's input
  Serial.readBytesUntil('\n', ReceiveBuffer, 2);

  num = ReceiveBuffer[0] - '0';
  // change entrance number into exact coordinates
  if(1 <= num && num <= 4)
  {
    switch(num)
    {
    case 1: cx = 0; cy = 0; ex = 3; ey = 3; en = 0; break;
    case 2: cx = 0; cy = 3; ex = 3; ey = 0; en = 3; break;
    case 3: cx = 3; cy = 0; ex = 0; ey = 3; en = 12; break;
    case 4: cx = 3; cy = 3; ex = 0; ey = 0; en = 15; break;
    default: en = -1; break;
    }
  }
  else{
    en = -1;
  }
  return en;
}

// Set four mines in the Metrix
void Initialization(){
  int i=0;
  int tmpMine = -1; // record if mines are on the same place
  int sameflag = 0;
  // initialize the map  
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      Map[i][j].mine = 0;
      Map[i][j].used = 0;
      Map[i][j].show = 'o';      
    }
  }
  // set mines on the map
  for (int p = 0;p<4;p++){
    int row = MineBuffer[p]%4;
    int column = MineBuffer[p]-row*4;
    Map[row][column].mine = 1;  
    Map[row][column].show = '*';    
  }
  
  Serial.println("0   1   2   3");
  Serial.println("4   5   6   7");
  Serial.println("8   9   10  11");
  Serial.println("12  13  14  15");
  Serial.println("Please choose four numbers as mines:");

    for(i=0;i<4;i++){
      while(1)
      {
        sameflag = 0;
        Serial.print("Please input mine ");
        Serial.println(i);
        while (!Serial.available() > 0);  //wait user's input
        tmpMine = Serial.parseInt();
        if(tmpMine < 0 || tmpMine > 15)
        {
          Serial.println("Invalid input, Please input a number of 0-15");
        }
        else
        {
          //the mine cannot be placed at the same place
          for(int j =0; j < i; ++j) 
          {
            if(tmpMine == MineBuffer[j])
            {
              sameflag = 1;
            }
          }
          //The number is different from the others
          if(sameflag == 0)
          {
            break;
          }
          else
          {
             Serial.println("Invalid input, it can't be the same as the other mines");
          }
        }
      }
      MineBuffer[i] = tmpMine;
    }
  for(i=0;i<4;i++){
    SendBuffer[i]=(byte)MineBuffer[i];
  }
}

int Move(){
  char current[1];
  Serial.readBytesUntil('\n', current, 1);
  switch(current[0]){
    case 'w': case 'W':Up(); break;
    case 'a': case 'A':Left(); break;
    case 's': case 'S':Down(); break;
    case 'd': case 'D':Right(); break;
    default: return -1;  
  }
  Map[cx][cy].used = 1;
  if(Map[cx][cy].mine == 1){
    HP-=1;
    //Map[cx][cy].mine = 0;
    //Map[cx][cy].show = 'o';
    Serial.println("IDIOTS!!!!!!!!!");
    LCDupdate();
  }
  return 0;
  //Line();
  //Display();
}
void Up(){
  if (cx != 0){
    cx-=1;
  }
}
void Left(){
  if (cy != 0){
    cy-=1;
  }
}
void Down(){
  if (cx != 3){
    cx+=1;
  }
}
void Right(){
  if (cy != 3){
    cy+=1;
  }  
}
// display current map(also the progress)
void Display(){
  for(int i = 0; i < 4; ++i){
    for(int j = 0; j < 4; ++j){
      if(cx == i && cy == j)  Serial.print("M ");
      else if(Map[i][j].used == 0) Serial.print("0 ");
      else if(Map[i][j].mine == 0) Serial.print("1 ");
      else if(Map[i][j].mine == 1) Serial.print("* ");
    }
    Serial.println(" ");
  }
  Serial.println(" ");
  //Serial.println("Here we go->");
}
// display a line
void Line(){
  for(int i = 0; i < 60; ++i)
  {
    Serial.print("-");
  }
  Serial.println(" ");
}
// update hp on lcd
void LCDupdate(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HP: ");
  lcd.print(HP);
}

// reset when a game has ended
void reset (const int masterResult){
  if(masterResult == 1){
    Serial.println("You Lose");
  }
  else if(masterResult == 0){
    Serial.println("You Win");
  }
  else if(masterResult == 2){
    Serial.println("Time Out! Game Over");
  }
  cx = cy = ex = ey = 0;
  HP = 2;
  Entrance = -1;
}

