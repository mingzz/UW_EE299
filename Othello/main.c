#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define infinity 100

char map[8][8];

typedef struct plot{
    int x;
    int y;
};

typedef struct node{
  char graph[8][8];
  struct plot mov;
  struct node *firstChild;
  struct node *nextSibling;
};

struct node *root;
bool reverse[8][8];

void init();
int move(const int mode);
bool isMoveLegal(char map[8][8], const int raw, const int col, const char player, const char opponent);
void display(void);
int gameOver(void);
int canMove(char map[8][8], const char player, const char opponent);
struct plot MinMax(int depth, const char player, const char opponent);
void findPossebleMoves(struct node *parent, const char player, const char opponent);
struct node *insertTree(const int row, const int col, struct node *parent, struct node *preSibling, const char player, const char opponent);
int Min(struct node *root, int depth, const char player, const char opponent);
int Max(struct node *root, int depth, const char player, const char opponent);
int evaluate(char map[8][8]);


int main()
{
    int status = 0;
    int mode;
    printf("input format:row line\n");
    init();
    display();
    printf("1.VS Computer 2.VS User\nChoose mode:");
    scanf("%d",&mode);
    while(0==status){
        status = move(mode);
    }
    return 0;
}

void init(){
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if((3==i && 4==j)||(4==i && 3==j))
               map[i][j] = 'x';// black
            else if((3==i && 3==j)||(4==i && 4==j))
                map[i][j] = 'o';// white
            else
                map[i][j] = '.';
        }
    }
    root=(struct node*)malloc(sizeof(struct node));
    root->firstChild = NULL;
    root->nextSibling = NULL;
    root->mov.x = -1;
    root->mov.y = -1;
}

int move(const int mode){
    static bool round = 0;
    int raw = 0;
    int col = 0;
    static int roundCount = 0;
    roundCount++;
    if(roundCount>40){
        if(gameOver()){
            return -1;
        }
    }

    if(false==round){// black
        printf("Black, it's your turn:");
        scanf("%d %d",&raw,&col);
        while(getchar()!='\n');
        while(!isMoveLegal(map,raw,col,'x','o')){
            printf("You cannot move here.\nEnter again:");
            scanf("%d %d",&raw,&col);
            while(getchar()!='\n');
        }
        printf("OK! You move to (%d,%d). \n",raw,col);
        map[raw][col]='x';
        for(int i = 0;i<8;i++){
            for(int j = 0;j<8;j++){
                if(true == reverse[i][j])
                    map[i][j]='x';
            }
        }
        round = true;
    } else {// white
        if(mode == 2){
            printf("White, it's your turn:");
            scanf("%d %d",&raw,&col);
            while(getchar()!='\n');
            while(!isMoveLegal(map,raw,col,'o','x')){
                printf("You cannot move here.\nEnter again:");
                scanf("%d %d",&raw,&col);
                while(getchar()!='\n');
            }
            printf("OK! You move to (%d,%d). \n",raw,col);
            map[raw][col]='o';
            for(int i = 0;i<8;i++){
                for(int j = 0;j<8;j++){
                    if(true == reverse[i][j])
                        map[i][j]='o';
                }
            }
            round = false;
        }else {
            struct plot bestMove = MinMax(2,'o','x');
            raw = bestMove.x;
            col = bestMove.y;
            map[raw][col]='o';
            isMoveLegal(map,raw,col,'o','x');
            for(int i = 0;i<8;i++){
                for(int j = 0;j<8;j++){
                    if(true == reverse[i][j])
                        map[i][j]='o';
                }
            }
            round = false;
        }
    }
    display();
    return 0;
}

bool isMoveLegal(char map[8][8], const int raw, const int col, const char player, const char opponent){
    int xNext = 0;
    int yNext = 0;
    int legal[8];
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            legal[i] = 0;
            reverse[i][j] = false;
        }
    }

    // 1.above
    for(int i = 1;i<8;i++){
        xNext = raw;
        yNext = col - i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[0] = 1;
            }
            if(player==map[xNext][yNext]&&legal[0]){
                legal[0] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw;
                    y = col - i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 2.below
    for(int i = 1;i<8;i++){
        xNext = raw;
        yNext = col + i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[1] = 1;
            }
            if(player==map[xNext][yNext]&&legal[1]){
                legal[1] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw;
                    y = col + i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 3.left
    for(int i = 1;i<8;i++){
        xNext = raw - i;
        yNext = col;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[2] = 1;

            }
            if(player==map[xNext][yNext]&&legal[2]){
                legal[2] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw - i;
                    y = col;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 4.right
    for(int i = 1;i<8;i++){
        xNext = raw + i;
        yNext = col;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[3] = 1;
            }
            if(player==map[xNext][yNext]&&legal[3]){
                legal[3] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw + i;
                    y = col;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
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
        xNext = raw - i;
        yNext = col - i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[4] = 1;
            }
            if(player==map[xNext][yNext]&&legal[4]){
                legal[4] = 2;

                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw - i;
                    y = col - i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }

                break;
            }
        }
    }
    // 6.lower-left
    for(int i = 1;i<8;i++){
        xNext = raw - i;
        yNext = col + i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[5] = 1;
            }
            if(player==map[xNext][yNext]&&legal[5]){
                legal[5] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw - i;
                    y = col + i;
                    if( x>=0&&x<=7 && y>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
                            reverse[x][y] = true;
                        }
                    }
                }
                break;
            }
        }
    }
    // 7.upper-right
    for(int i = 1;i<8;i++){
        xNext = raw + i;
        yNext = col - i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[6] = 1;
            }
            if(player==map[xNext][yNext]&&legal[6]){
                legal[6] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw + i;
                    y = col - i;
                    if( x>=0&&x<=7 && y>=0&&y<=7){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
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
        xNext = raw + i;
        yNext = col + i;
        if( xNext>=0&&xNext<=7 && yNext>=0&&yNext<=7 ){
            if ('.'==map[xNext][yNext])
                break;
            if (opponent==map[xNext][yNext]){
                legal[7] = 1;
            }
            if(player==map[xNext][yNext]&&legal[7]){
                legal[7] = 2;
                int x;
                int y;
                for(int i = 1;i<8;i++){
                    x = raw + i;
                    y = col + i;
                    if( x>=0&&y<=7 && x>=0&&y<=7 ){
                        if ('.'==map[x][y]||player==map[x][y])
                            break;
                        if (opponent==map[x][y]){
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

void display(void){
    printf("\n   0 1 2 3 4 5 6 7\n");
    printf("  ----------------\n");
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(0==j)
                printf("%d|",i);
            printf(" %c", map[i][j]);
            if(7==j)
                printf("|\n");

        }
    }
    printf("  ----------------\n");
}

int canMove(char map[8][8], const char player, const char opponent){
    int blackCount = 0;
    int whiteCount = 0;
    int movePlot = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if(map[i][j]=='.'){
                if(isMoveLegal(map,i,j,player,opponent))
                    movePlot++;
            } else if('x'==map[i][j]){
                blackCount++;
            } else{
                whiteCount++;
            }
        }
    }
    if(0==movePlot)
        return -1;
    else
        return 0;
}

int gameOver(void){
    int move = 0;
    int black = 0;
    int white = 0;
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            if('.'==map[i][j])
                if(isMoveLegal(map,i,j,'o','x')||isMoveLegal(map,i,j,'x','o'))
                    move++;
            if('x'==map[i][j]) black++;
            if('o'==map[i][j]) white++;
        }
    }
    if(64==(black+white)||0==move){
        if(black>white){
            printf("Black Win!\n");
        }else if(black<white){
            printf("White Win!\n");
        }else if(black==white){
            printf("Tie!\n");
        }
        return 1;
    }
    return 0;
}

void findPossebleMoves(struct node *parent, const char player, const char opponent){
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
    for(int i = 0;i<8;i++){
        for(int j = 0;j<8;j++){
            root->graph[i][j]=map[i][j];
        }
    }
    findPossebleMoves(root,player,opponent);
    struct node *tmp=root->firstChild;
    if(root->firstChild!=NULL)
        value = Min(root->firstChild,depth,player,opponent);
    if(value>bestValue){
        bestValue = value;
        bestMove = root->firstChild->mov;
    }
    while(tmp->nextSibling!=NULL){
        value = Min(tmp->nextSibling,depth,player,opponent);
        if(value>bestValue){
            bestValue = value;
            bestMove = tmp->nextSibling->mov;
        }
        tmp=tmp->nextSibling;
    }
    return bestMove;
}

int Min(struct node *root, int depth, const char player, const char opponent){
    //printf("Min depth%d\n",depth);
    int min = infinity;
    int value = 0;
    if(depth<=0) return evaluate(root->graph);
    if(-1==canMove(root->graph,opponent,player)){
        if(-1==canMove(root->graph,player,opponent)){
            return evaluate(root->graph);
        }
        return Max(root,depth,player,opponent);
    }
    findPossebleMoves(root,opponent,player);

    //debug
    //printf("Min %d %d %c\n",root->mov.x,root->mov.y,root->graph[root->mov.x][root->mov.y]);

    struct node *tmp=root->firstChild;
    if(root->firstChild!=NULL)
        value = Max(root->firstChild,depth-1,player,opponent);
    if(value<min){
        min = value;
    }
    while(tmp->nextSibling!=NULL){
        value = Max(tmp->nextSibling,depth-1,player,opponent);
        if(value<min){
            min = value;
        }
        tmp=tmp->nextSibling;
    }
    return min;
}

int Max(struct node *root, int depth, const char player, const char opponent){
    //printf("Max depth%d\n",depth);
    int max = -infinity;
    int value = 0;
    if(depth<=0) return evaluate(root->graph);
    if(-1==canMove(root->graph,opponent,player)){
        if(-1==canMove(root->graph,player,opponent)){
            return evaluate(root->graph);
        }
        return Min(root,depth,player,opponent);
    }
    findPossebleMoves(root,player,opponent);

    //debug
    //printf("Max %d %d\n",root->mov.x,root->mov.y);

    struct node *tmp=root->firstChild;
    if(root->firstChild!=NULL)
        value = Min(root->firstChild,depth-1,player,opponent);
    if(value>max){
        max = value;
    }
    while(tmp->nextSibling!=NULL){
        value = Min(tmp->nextSibling,depth-1,player,opponent);
        if(value>max){
            max = value;
        }
        tmp=tmp->nextSibling;
    }
    return max;
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

int evaluate(char map[8][8]){
    return 1;
}
