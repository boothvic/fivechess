/*
 * Author: Vic Lin   File Name: fivechess.c
 * Created Time: 2017年07月09日 星期日 13时31分29秒
 *
 * C source code - replace this with a description
 * of the code and write the code below this text
 *
 */


#include<cstdio>
#include<cstdlib>
#include<cstring>           // UPDATE: period refresh (show POS) | AI |
#include<termio.h>
                        // no: regret
#define CHESS    5
#define CELLSLEN 9
#define BOARD (CELLSLEN*2+1)

void envir();
void print();
int Icon();
int checkchess();
int getch();        /// LINUX

const char* board[BOARD][BOARD]={0};
int check[CELLSLEN][CELLSLEN]={0};
int turn=-1;                    /// -1==white  1==black
int row=1, col=1,  con;

const char * BLACK="○",  * WHITE ="●",   * POS  ="¤";
const char * LT   ="┌",  * TOP   ="┬",   * RT   ="┐";
const char * LEFT ="├",  * CENTER="┼",   * RIGHT="┤";
const char * LF   ="└",  * FOOT  ="┴",   * RF   ="┘";
const char * SPACE=" ";
//#include <stdio.h>

int main(void)
{
    int s;
    envir();
    s=Icon();

    if(s){
        if(turn==1) printf("\n-----White Won-----\n");
        else        printf("\n-----Black Won-----\n");
    }
    else printf("\n-----Tied-----\n");

    return 0;
}

void envir()
{
    for(int j=0;j<BOARD;j++)
        for(int k=0;k<BOARD;k++)
            board[j][k]=SPACE;

    board[0][0]=LT; board[0][BOARD-1]=RT; board[BOARD-1][0]=LF; board[BOARD-1][BOARD-1]=RF;
    for(int i=2;i<BOARD-2;i+=2){
        board[0][i]=TOP;
        board[BOARD-1][i]=FOOT;
        board[i][0]=LEFT;
        board[i][BOARD-1]=RIGHT;
    }
    for(int j=2;j<BOARD-2;j+=2)
        for(int k=2;k<BOARD-2;k+=2){
            board[j][k]=CENTER;
        }
}


void print()
{
    system("clear");                                        /// BUG 2 boards   no"reset"
    for(int j=0;j<BOARD;j++){               /// columns
        for(int k=0;k<BOARD;k++)            /// rows
            //puts(board[j][k]);
            printf("%s ",board[j][k]);
        putchar('\n');
    }
    if(turn==1) printf("  %s  Black turn\n",BLACK);
    else     printf("  %s  White turn\n",WHITE);
    printf("    POS(%d,%d)\n",(row+1)/2,(col+1)/2);
}

#define NOTCHESS board[row][col]!=BLACK&&board[row][col]!=WHITE
int Icon()
{
    int count=0;
    board[row][col]=POS;
    print();
    while((con=getch())!=0x1b)
    {
        if(con=='w'||con=='s'||con=='a'||con=='d'||con==' '){
            if(con!=' ' && board[row][col]==POS && NOTCHESS) board[row][col]=SPACE;
            switch(con){
                case 'w': if(row>1)       row-=2; if(NOTCHESS) board[row][col]=POS; break;
                case 's': if(row<BOARD-2) row+=2; if(NOTCHESS) board[row][col]=POS; break;
                case 'a': if(col>1)       col-=2; if(NOTCHESS) board[row][col]=POS; break;
                case 'd': if(col<BOARD-2) col+=2; if(NOTCHESS) board[row][col]=POS; break;
                case ' ':{if(board[row][col]==BLACK||board[row][col]==WHITE) break;
                          else {if(turn==-1)     {board[row][col]=WHITE; check[(row-1)/2][(col-1)/2]=-1; }
                                else if(turn==1) {board[row][col]=BLACK; check[(row-1)/2][(col-1)/2]=1 ; }
                                turn=-turn;
                                count++;
                               }    break;
                         }
                default : break;
            }
        }
        print();
        if(count==CELLSLEN*CELLSLEN) return 0;
        if(checkchess()) return 1;
    }
}


int checkchess()
{
    bool judge;
    int  pres;
    for(int j=0;j<CELLSLEN;j++)        /// check rows || columns
        for(int k=0;k<CELLSLEN;k++)
            if(check[j][k]!=0){
                pres=check[j][k];
                judge=true;
                if(j<=CELLSLEN-CHESS && k<=CELLSLEN-CHESS){             /// should contain '='
                    for(int l=1;l<CHESS;l++)
                        if(check[j+l][k+l]!=pres) judge=false;          ///oblique right
                    if(judge==true) return 1;
                }
                judge=true;
                if(j<=CELLSLEN-CHESS && k>=CHESS-1){
                    for(int l=1;l<CHESS;l++)
                        if(check[j+l][k-l]!=pres) judge=false;          ///oblique left
                    if(judge==true) return 1;
                }
                judge=true;
                if(k<=CELLSLEN-CHESS){
                    for(int l=1;l<CHESS;l++)
                        if(check[j][k+l]!=pres) judge=false;            ///horizontal
                    if(judge==true) return 1;
                }
                judge=true;
                if(j<=CELLSLEN-CHESS){
                    for(int l=1;l<CHESS;l++)
                        if(check[j+l][k]!=pres) judge=false;            ///vertical
                    if(judge==true) return 1;
                }
            }
    return 0;
}

int getch()                             ///LINUX special demand
{
    struct termios tm,tm_old;
    int fd=0,ch;
    if(tcgetattr(fd,&tm)<0) return -1;
    tm_old=tm;
    cfmakeraw(&tm);
    if(tcsetattr(fd,TCSANOW,&tm)<0) return -1;
    ch=getchar();
    if(tcsetattr(fd,TCSANOW,&tm_old)<0) return -1;
    return ch;
}
