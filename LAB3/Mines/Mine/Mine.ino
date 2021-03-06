#include <LiquidCrystal.h>
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

void setup() {
  SetClock();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  randomSeed(analogRead(0));
}

void loop() {
  if (Reset == 1){
    cx = cy = ex = ey = 0;
    HP = 2;
    Entrance = -1;
    while(Entrance == -1){
    Entrance = GetEntrance();
    }
    Initilization(Entrance);
    Line();
    Display();
    Reset = 0;
    count = 0;
    lcd.display();
    //Serial.println(Reset);
  }
  if(Serial.available() > 0){
    if(Move() == 0){
      Line();
      Display();
    }
  }
  if ((count == 60 || HP == 0) && Reset == 0){
    //Reset = 1;
    if(HP == 0){
      Serial.println("You die! Game Over");
    }
    else{
      Serial.println("Time Out! Game Over");
    }
    GameOver();
  }
  else if (cx == ex && cy == ey){
    //Reset = 1;
    Serial.println("Victory!");
    GameOver();
  }
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

// Set Metrix with four random mines 
void Initilization(const int entrance){
  // No mines on entrance and exit
  int BlockOne = entrance;
  int BlockTwo = 15 - entrance;
  for(int i=0;i<4;i++){
    for(int j=0;j<4;j++){
      Map[i][j].mine = 0;
      Map[i][j].used = 0;
      Map[i][j].show = 'o';      
    }
  }
  int RandNum[4];
  for(int q=0;q<4;q++){
    RandNum[q] = -1;
  }
  // Get 4 non-repeating random numbers.Legal ones
  for(int q=0;q<4;q++){
    RandNum[q] = random(16);

    while(RandNum[q] == BlockOne || RandNum[q] == BlockTwo ){
      RandNum[q] = random(16);
    }
    
    int m = 0;
    while(RandNum[q] == RandNum[m] &&  m<q ){
      RandNum[q] = random(16);
      m++;
    }
  }
  for (int p = 0;p<4;p++){
    int row = RandNum[p]%4;
    int column = RandNum[p]-row*4;
    Map[row][column].mine = 1;  
    Map[row][column].show = '*';    
  }
  Map[cx][cy].used = 1;
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
    Serial.println("OOOOOPS!!!!!!!!!");
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
  lcd.print(60 - count);
}


