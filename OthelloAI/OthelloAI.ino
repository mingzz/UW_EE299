#define infinity 100
char board[8][8];
// plot on the board
typedef struct plot{
    int x;
    int y;
};
// Game Tree node
typedef struct node{
  char graph[8][8];
  struct plot mov;
  struct node *firstChild;
  struct node *nextSibling;
};
// root of tree
struct node *root;
bool reverse[8][8];// modified each time when ismoveOnceOnceLegal is called. Storing opponents' pieces which can be reversed
int rtn = 0;
int mode;

bool turn = 0;    
int roundCount = 0;

void initial();// initiate the game
int moveOnce(const int mode); // make a moveOnce
bool isMoveLegal(char board[8][8], const int row, const int col, const char player, const char opponent); // analyze if this moveOnce is legal
void show(void); // display the current map
int gameOver(void); // if game is over
int canMove(char board[8][8], const char player, const char opponent);  // if the player can moveOnce
struct plot MinMax(int depth, const char player, const char opponent);  // MiniMax Algorithm entrance
void findPossibleMoves(struct node *parent, const char player, const char opponent); // find all possible moves and will make a game tree
struct node *insertTree(const int row, const int col, struct node *parent, struct node *preSibling, const char player, const char opponent); // insert a node into the tree
int mini(struct node *root, int depth, const char player, const char opponent, int alpha, int beta); // Minimize the opponent's benefit
int maxi(struct node *root, int depth, const char player, const char opponent, int alpha, int beta); // Maximum the player's benefit
int evaluate(struct node *current, const char player, const char opponent); // evaluate the board on the player side
void clean(struct node *parent);  // delete tree

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  initial();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(0 == rtn)
    rtn = moveOnce(mode);
  else{
    char choice = -1;
    if(choice == -1){
          Serial.println("Do you want to play again?(Y/Others)");
          while(!Serial.available()>0);
          choice = Serial.read();
          if('Y' == choice){
            rtn = 0;
            initial();
          }
    }
  }
  delay(400);
}


void initial(){
    turn = 0;
    roundCount = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if((3==i && 4==j)||(4==i && 3==j))
               board[i][j] = 'x';// black
            else if((3==i && 3==j)||(4==i && 4==j))
                board[i][j] = 'o';// white
            else
                board[i][j] = '.';
        }
    }
    root=(struct node*)malloc(sizeof(struct node));
    root->firstChild = NULL;
    root->nextSibling = NULL;
    root->mov.x = -1;
    root->mov.y = -1;
    Serial.println("1.User VS AI 2.User VS User 3.AI VS AI");
    Serial.println("Choose mode:");
    while (!Serial.available() > 0);
    mode = Serial.read()-'0';
    Serial.println(mode);
    show();
}

int moveOnce(const int mode){
    int row = 0;
    int col = 0;
    char rcvBuffer[2];
    roundCount++;
    if(roundCount>40){
        if(gameOver()){
            return -1;
        }
        if(-1==canMove(board,'x','o'))
            turn = true;
        if(-1==canMove(board,'o','x'))
            turn = false;
    }

    if(1 == mode || 2 == mode){
        if(false==turn){// black
            Serial.println("Black, it's your turn:");
            while (Serial.available() <= 0);  
            Serial.readBytesUntil('\n', rcvBuffer, 2);
            row = rcvBuffer[0]-'0';
            col = rcvBuffer[1]-'0';
            while(!isMoveLegal(board,row,col,'x','o')){
                Serial.println("You cannot move here.\nEnter again:");
                while (Serial.available() <= 0);  
                Serial.readBytesUntil('\n', rcvBuffer, 2);
                row = rcvBuffer[0]-'0';
                col = rcvBuffer[1]-'0';
            }
            Serial.print("OK! You move to ");
            Serial.print(row);
            Serial.println(col);
            board[row][col]='x';
            for(int i = 0;i<8;i++){
                for(int j = 0;j<8;j++){
                    if(true == reverse[i][j])
                        board[i][j]='x';
                }
            }
            turn = true;
        } else {// white
            if(mode == 2){
                Serial.println("White, it's your turn:");
                while (!Serial.available() > 0);  
                Serial.readBytesUntil('\n', rcvBuffer, 2);
                row = rcvBuffer[0]-'0';
                col = rcvBuffer[1]-'0';
                while(!isMoveLegal(board,row,col,'o','x')){
                    Serial.println("You cannot move here.\nEnter again:");
                    while (Serial.available() <= 0);  
                    Serial.readBytesUntil('\n', rcvBuffer, 2);
                    row = rcvBuffer[0]-'0';
                    col = rcvBuffer[1]-'0';
                }
                Serial.print("OK! You move to ");
                Serial.print(row);
                Serial.println(col);
                board[row][col]='o';
                for(int i = 0;i<8;i++){
                    for(int j = 0;j<8;j++){
                        if(true == reverse[i][j])
                            board[i][j]='o';
                    }
                }
                turn = false;
            }else {
                struct plot bestMove = MinMax(0,'o','x');
                row = bestMove.x;
                col = bestMove.y;
                isMoveLegal(board,row,col,'o','x');
                board[row][col]='o';
                for(int i = 0;i<8;i++){
                    for(int j = 0;j<8;j++){
                        if(true == reverse[i][j])
                            board[i][j]='o';
                    }
                }
                clean(root->firstChild);
                turn = false;
                Serial.print("AI moves to ");
                Serial.print(row);
                Serial.print(" ");
                Serial.println(col);
            }
        }
    }
    if(3 == mode){
        if(false == turn){
            Serial.println("Black :");
            struct plot bestMove = MinMax(0,'x','o');
            row = bestMove.x;
            col = bestMove.y;
            isMoveLegal(board,row,col,'x','o');
            board[row][col]='x';
            for(int i = 0;i<8;i++){
                for(int j = 0;j<8;j++){
                    if(true == reverse[i][j])
                        board[i][j]='x';
                }
            }
            clean(root->firstChild);
            turn = true;
        }else{
            Serial.println("White :");
            struct plot bestMove = MinMax(0,'o','x');
            row = bestMove.x;
            col = bestMove.y;
            isMoveLegal(board,row,col,'o','x');
            board[row][col]='o';
            for(int i = 0;i<8;i++){
                for(int j = 0;j<8;j++){
                    if(true == reverse[i][j])
                        board[i][j]='o';
                }
            }
            clean(root->firstChild);
            turn = false;
        }
    }

    root->firstChild = NULL;
    root->nextSibling = NULL;
    root->mov.x = -1;
    root->mov.y = -1;
    show();
    return 0;
}

void show(void){
    Serial.println("\n  0 1 2 3 4 5 6 7");
    Serial.println("  ----------------");
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(0==j){
                Serial.print(i);
                Serial.print("|");
            }
            Serial.print(board[i][j]);
            Serial.print(" ");
            if(7==j)
                Serial.println("|");

        }
    }
    Serial.println("  ----------------");
}

int canMove(char board[8][8], const char player, const char opponent){
    int movePlot = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(board[i][j]=='.'){
                if(isMoveLegal(board,i,j,player,opponent))
                    movePlot++;
            }
        }
    }
    if(0==movePlot)
        return -1;
    else
        return 0;
}

int gameOver(void){
    int mov = 0;
    int black = 0;
    int white = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if('.'==board[i][j])
                if(isMoveLegal(board,i,j,'o','x')||isMoveLegal(board,i,j,'x','o'))
                    mov++;
            if('x'==board[i][j]) black++;
            if('o'==board[i][j]) white++;
        }
    }
    if(64==(black+white)||0==mov){
        if(black>white){
            Serial.println("Black Win!");
        }else if(black<white){
            Serial.println("White Win!");
        }else if(black==white){
            Serial.println("Tie!");
        }
        return 1;
    }
    return 0;
}

void findPossibleMoves(struct node *parent, const char player, const char opponent){
    struct node *current=NULL;
    //if(parent->mov.x != -1)printf("findPossibleMoves %d %d %c\n",parent->mov.x,parent->mov.y,parent->graph[parent->mov.x][parent->mov.y]);
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(isMoveLegal(parent->graph,i,j,player,opponent)){
                    current = insertTree(i,j,parent,current,player,opponent);
            }
        }
    }
}

struct plot MinMax(int depth, const char player, const char opponent){
    struct plot bestMove;
    int bestValue = -infinity;
    int value = 0;
    int alpha = -infinity;
    int beta = infinity;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            root->graph[i][j]=board[i][j];
        }
    }
    findPossibleMoves(root,player,opponent);
    if(root->firstChild!=NULL){
        bestMove.x = root->firstChild->mov.x;
        bestMove.y = root->firstChild->mov.y;
        struct node *tmp=root->firstChild;
        value = mini(root->firstChild,depth,player,opponent,alpha,beta);
        if(value>bestValue){
            bestValue = value;
            bestMove = root->firstChild->mov;
        }
        while(tmp->nextSibling!=NULL){
            value = mini(tmp->nextSibling,depth,player,opponent,alpha,beta);
            if(value>bestValue){
                bestValue = value;
                bestMove = tmp->nextSibling->mov;
            }
            tmp=tmp->nextSibling;
        }
    }
    //printf("best value : %d\n",bestValue);
    return bestMove;
}

int mini(struct node *root, int depth, const char player, const char opponent, int alpha, int beta){
    //printf("mini depth%d\n",depth);
    //Serial.println("mini !!!!!!!!!!!!!!!!!!!!!!!!");
    int mini = infinity;
    int value = 0;
    if(depth<=0) return evaluate(root,opponent,player);
    if(-1==canMove(root->graph,opponent,player)){
        if(-1==canMove(root->graph,player,opponent)){
            return evaluate(root,opponent,player);
        }
        return maxi(root,depth,player,opponent,alpha,beta);
    }
    findPossibleMoves(root,opponent,player);

    //debug
    //printf("2Min %d %d %c\n",root->mov.x,root->mov.y,root->graph[root->mov.x][root->mov.y]);

    if(root->firstChild!=NULL){
        struct node *tmp=root->firstChild;
        value = maxi(root->firstChild,depth-1,player,opponent,alpha,beta);
        if(value<mini){
            mini = value;
        }
        if(mini<beta) beta = mini;
        if(beta<=alpha) return mini;
        while(tmp->nextSibling!=NULL){
            value = maxi(tmp->nextSibling,depth-1,player,opponent,alpha,beta);
            if(value<mini){
                mini = value;
            }
            if(mini<beta) beta = mini;
            if(beta<=alpha) return mini;
            tmp=tmp->nextSibling;
        }
    }
    return mini;
}

int maxi(struct node *root, int depth, const char player, const char opponent, int alpha, int beta){
    //printf("maxi depth%d\n",depth);
    int maxi = -infinity;
    int value = 0;
    if(depth<=0) return evaluate(root,player,opponent);
    if(-1==canMove(root->graph,player,opponent)){
        if(-1==canMove(root->graph,opponent,player)){
            return evaluate(root,player,opponent);
        }
        return mini(root,depth,player,opponent,alpha,beta);
    }
    findPossibleMoves(root,player,opponent);

    //debug
    //printf("maxi %d %d\n",root->mov.x,root->mov.y);

    if(root->firstChild!=NULL){
        struct node *tmp=root->firstChild;
        value = mini(root->firstChild,depth-1,player,opponent,alpha,beta);
        if(value>maxi){
            maxi = value;
        }
        if(maxi>alpha) alpha = maxi;
        if(beta<=alpha) return maxi;
        while(tmp->nextSibling!=NULL){
            value = mini(tmp->nextSibling,depth-1,player,opponent,alpha,beta);
            if(value>maxi){
                maxi = value;
            }
            if(maxi>alpha) alpha = maxi;
            if(beta<=alpha) return maxi;
            tmp=tmp->nextSibling;
        }
    }
    return maxi;
}

struct node *insertTree(const int row, const int col, struct node *parent, struct node *preSibling, const char player, const char opponent){
    struct node *child =  (struct node*)malloc(sizeof(struct node));
    for(int m = 0;m<8;m++){// copy
        for(int n = 0;n<8;n++){
            child->graph[m][n]=parent->graph[m][n];
            if(true == reverse[m][n])
                child->graph[m][n]=player;
        }
    }
    child->graph[row][col]=player;
    child->mov.x=row;
    child->mov.y=col;
    //printf("insertTree %d %d\n",row,col);

    if(parent!=NULL) parent->firstChild = child;
    if(preSibling!=NULL) child->nextSibling = preSibling;
    else child->nextSibling = NULL;

    child->firstChild = NULL;
    return child;
}

int evaluate(struct node *current, const char player, const char opponent){
    int mobility = 0;
    int stability = 0;
    int value = 0;
    int score = 0;
    int row = current->mov.x;
    int col = current->mov.y;
    for(int m = 0;m<8;m++){
        for(int n = 0;n<8;n++){
            if(isMoveLegal(current->graph,m,n,player,opponent)){
                mobility++;
            }
        }
    }
    if((current->graph[row-1][col]=='x'||current->graph[row-1][col]=='o')&&(current->graph[row+1][col]=='x'||current->graph[row+1][col]=='o')
        &&(current->graph[row][col-1]=='x'||current->graph[row][col-1]=='o')&&(current->graph[row][col+1]=='x'||current->graph[row][col+1]=='o')
        &&(current->graph[row-1][col-1]=='x'||current->graph[row-1][col-1]=='o')&&(current->graph[row+1][col+1]=='x'||current->graph[row+1][col+1]=='o')
        &&(current->graph[row+1][col-1]=='x'||current->graph[row+1][col-1]=='o')&&(current->graph[row-1][col+1]=='x'||current->graph[row-1][col+1]=='o'))
        stability++;
    if((row==0&&col==0)||(row==7&&col==0)||(row==7&&col==7)||(row==0&&col==7)) value = 10;
    if((row==0&&(col>=2&&col<=5))||(row==7&&(col>=2&&col<=5))||((row>=2&&row<=5)&&col==0)||((row>=2&&row<=5)&&col==7)) value = 5;

    score = mobility+stability*3+value;
    return score;
}

void clean(struct node *parent){
    if(parent->firstChild!=NULL)
        clean(parent->firstChild);
    if(parent->nextSibling!=NULL){
        clean(parent->nextSibling);
    }
    free(parent);
}


bool isMoveLegal(char board[8][8], const int row, const int col, const char player, const char opponent){
    int xNext = 0;
    int yNext = 0;
    int legal[8];
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            legal[i] = 0;
            reverse[i][j] = false;
        }
    }

    if(board[row][col]=='x'||board[row][col]=='o')
        return false;
    // 1.left
    for(int i = 1;i<8;i++){
        xNext = row;
        yNext = col - i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row][col - 1]){
                legal[0] = 1;
            }
            if(player==board[xNext][yNext]&&legal[0]){
                legal[0] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row;
                    y = col - i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 2.right
    for(int i = 1;i<8;i++){
        xNext = row;
        yNext = col + i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row][col + 1]){
                legal[1] = 1;
            }
            if(player==board[xNext][yNext]&&legal[1]){
                legal[1] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row;
                    y = col + i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 3.above
    for(int i = 1;i<8;i++){
        xNext = row - i;
        yNext = col;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row - 1][col]){
                legal[2] = 1;
            }
            if(player==board[xNext][yNext]&&legal[2]){
                legal[2] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row - i;
                    y = col;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 4.below
    for(int i = 1;i<8;i++){
        xNext = row + i;
        yNext = col;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row + 1][col]){
                legal[3] = 1;
            }
            if(player==board[xNext][yNext]&&legal[3]){
                legal[3] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row + i;
                    y = col;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }

    // 5.upper-left
    for(int i = 1;i<8;i++){
        xNext = row - i;
        yNext = col - i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row -1][col -1]){
                legal[4] = 1;
            }
            if(player==board[xNext][yNext]&&legal[4]){
                legal[4] = 2;

                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row - i;
                    y = col - i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }

                break;
            }
        }
    }
    // 6.upper-right
    for(int i = 1;i<8;i++){
        xNext = row - i;
        yNext = col + i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row -1][col +1]){
                legal[5] = 1;
            }
            if(player==board[xNext][yNext]&&legal[5]){
                legal[5] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row - i;
                    y = col + i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 7.lower-left
    for(int i = 1;i<8;i++){
        xNext = row + i;
        yNext = col - i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row + 1][col - 1]){
                legal[6] = 1;
            }
            if(player==board[xNext][yNext]&&legal[6]){
                legal[6] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row + i;
                    y = col - i;
                    if( x>=0&&x<=7 && y>=0&&y<=7){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 8.lower-right
    for(int i = 1;i<8;i++){
        xNext = row + i;
        yNext = col + i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==board[xNext][yNext])
                break;
            if (opponent==board[row + 1][col + 1]){
                legal[7] = 1;
            }
            if(player==board[xNext][yNext]&&legal[7]){
                legal[7] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = row + i;
                    y = col + i;
                    if( x>=0&&y<=7 && x>=0&&y<=7 ){
                        if ('.'==board[x][y]||player==board[x][y])
                            break;
                        if (opponent==board[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }

    // judge
    for(int j = 0;j<8;j++){
        if(2 == legal[j]){
            return true;
        }
    }
    return false;
}

