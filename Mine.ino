// working variables
int Entrance = 0; // legal:0, 3, 12, 15
int Reset = 1;
struct Plot{
  int mine; //0 for no mine, 1 for mine
  int used; //0 for no used, 1 for not used
  char show;
};
Plot Map[4][4];
int cx;
int cy;
int timer1_counter;
int count;

// function declare
int GetEntrance();
void Initilization(const int entrance);
void SetClock();
void Move();
void Display();


void setup() {
  // put your setup code here, to run once:
  SetClock();
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  while(Entrance == 0){
    Entrance = GetEntrance();
    // print wrong input
    // clear
  }
  Initilization(Entrance);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (Reset == 1){
    GetEntrance();
    Initialization(Entrance);
    Reset = 0;
  }
  Move();
  Display();
  if (Time expire || HP ==0){
    Reset = 1;
    //.....
  }
  if (posistion == destination){
    Reset = 1;
    //....
  }*/
}

// Output: Entrance:0, 3, 12, 15
int GetEntrance(){
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
  Serial.print("Input the entrance: ");

  while (!Serial.available() > 0);  //wait user's input
  Serial.readBytesUntil('\n', ReceiveBuffer, 2);

  num = ReceiveBuffer[0] - '0';
  if(1 <= num && num <= 4)
  {
    switch(num)
    {
    case 1: return 0;
    case 2: return 3;
    case 3: return 12;
    case 4: return 15;
    }
    return 0;
  }
  else return 0;
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
  for(q;q<4:q++){
    RandNum[q] = -1;
  }
  q = 0;
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
  for (int p = 0;p<4:p++){
    int row = RandNum[p]%4;
    int column = RandNum[p]-row*4;
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

// Listen and execute direction input 
void Move(){
  
}

ISR(TIMER1_OVF_vect)
{
  TCNT1=timer1_counter;
  count=count+1;
  Serial.println(count);
  if(count==60)
  {
    count=0;
    Serial.print("Game Over");
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
}

void Clear(){
  for(int i = 0; i < 20; ++i){
    Serial.println(" ");
  }
}




