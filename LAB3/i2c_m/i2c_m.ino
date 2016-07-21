#include <Wire.h>
#include <LiquidCrystal.h>

#define SLAVE_ADDRESS 4
// working variables
LiquidCrystal lcd(2, 3, 4, 5, 6, 7, 8);
int Entrance = -1; // legal:0, 3, 12, 15
int Reset = 1;
struct Plot{
  int mine; //0 for no mine, 1 for mine
  int used; //0 for no used, 1 for used
  char show;
};
Plot Map[4][4];
int cx, cy;
int ex, ey;
int timer1_counter;
int count;
int HP = 2;

char transGet;

void setup() {
  Wire.begin();         // join i2c bus (address optional for master)
  SetClock();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void loop() {
  if(Reset == 1){
    //Game reset
    cx = cy = ex = ey = 0;
    HP = 2;
    Entrance = -1;
    Reset = 0;
    Initialization(); //set Mines
    while(Entrance == -1)
    {
       Entrance = GetEntrance();   //add the wait for the other people
    }
    Wire.beginTransmission(4);
    Wire.write('E');
    Wire.endTransmission();
    //wait for the other player choose the entrance
  do{
    Wire.requestFrom(4, 1);
    transGet = Wire.read();
  }while(transGet != 'E');
    Map[cx][cy].used = 1;
    Serial.println("Start");
    if(Map[cx][cy].mine == 1){
      HP-=1;
      //Map[cx][cy].mine = 0;
      //Map[cx][cy].show = 'o';
      Serial.println(".....You are born with a mine. It's cool.");
      LCDupdate();
    }
    Line();
    Display();
    Reset = 0;
    count = 0;
    lcd.display();   
  }
  if(Serial.available() > 0){
    if(Move() == 0){
      Line();
      Display();
    }
  }
  Wire.requestFrom(4, 1);
  char result = Wire.read();
  if(result == 'W'){
    Serial.println("You Lose");
    GameOver();
  }
  else if(result == 'L'){
    Serial.println("You Win");
    GameOver();
  }
  //how to win
  if ((count == 60 || HP == 0) && Reset == 0){
    //Reset = 1;
    if(HP == 0){
      Serial.println("You die! Game Over");
      Wire.beginTransmission(4);
      Wire.write('L');
      Wire.endTransmission();
    }
    else{
      Serial.println("Time Out! Game Over");
      Wire.beginTransmission(4);
      Wire.write('Z');
      Wire.endTransmission();
    }
    GameOver();
  }
  else if (cx == ex && cy == ey){
    //Reset = 1;
    Serial.println("Victory!");
    Wire.beginTransmission(4);
    Wire.write('W');
    Wire.endTransmission();
    //send
    GameOver();
  }
  delay(400);
}

// Output: Entrance:0, 3, 12, 15
int GetEntrance(){
  Line();
  int en = 0;
  char ReceiveBuffer[2];
  int num = 0;
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
  int MineBuffer[5];
  int i=0;
  int transFlag = 0;
  int tmpMine = -1;
  int sameflag = 0;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      Map[i][j].mine = 0;
      Map[i][j].used = 0;
      Map[i][j].show = 'o';      
    }
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
  // need to prohibit wrong input here
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write('I');
  for(i=0;i<4;i++){
    Wire.write(MineBuffer[i]);
    //Serial.print(MineBuffer[i]);
    //Serial.print(" ");
    MineBuffer[i]=0;
  }
  //Serial.println(" ");
  Wire.endTransmission();    // stop transmitting
  do{
    Wire.requestFrom(4, 4);
    transFlag = 0;
    for(i=0;i<4;i++){
      MineBuffer[i]=Wire.read();
      if (MineBuffer[i]>=0 && MineBuffer[i]<=15)
        transFlag=transFlag+1;
    }
  }while(transFlag != 4);
  /*
  Serial.println("Through!!!!!!");
     for(i=0;i<4;i++){
      Serial.println(MineBuffer[i]);
     }
  Serial.println("HERE HERE HERE");*/
  for (int p = 0;p<4;p++){
    int row = MineBuffer[p]%4;
    int column = MineBuffer[p]-row*4;
    Map[row][column].mine = 1;  
    Map[row][column].show = '*';    
  }
}

// set 1 second frequency divider and initial count number 
void SetClock(){
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
  count=count+1;
  //Serial.println(count);
  if(Reset == 0){
      LCDupdate();
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

void Line(){
  for(int i = 0; i < 60; ++i)
  {
    Serial.print("-");
  }
  Serial.println(" ");
}

void GameOver(){
  lcd.clear();
  Reset = -1;
  char Buffer[2];
  Serial.println("Do you want to try again? Y/N");
  while (!Serial.available() > 0);  //wait user's input
  Serial.readBytesUntil('\n', Buffer, 2);
  
  if(Buffer[0] == 'Y'){
    Reset = 1;
    Serial.println("Reset");
  }
  else{
    //Game over
    Serial.println(":-)");
    Serial.end();
  }
}

void LCDupdate(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HP: ");
  lcd.print(HP);
  lcd.setCursor(0, 1);
  lcd.print("Time Left: ");
  if(Reset == 0){
    lcd.print(60 - count);
  }
  else{
    lcd.print(60);
  }
    
}


