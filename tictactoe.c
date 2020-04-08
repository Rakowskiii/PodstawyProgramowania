#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

void game();
void drawBoard();
void botMove(char gameBoard[5][5], char sign){
    //okreslanie wartosci wierszy worth[(0-2)] i kolumn[(3-5)] i skosów worth[6-7]
    char nsign;
    int worth[8]= {0,0,0,0,0,0,0,0};
    int block[8] = {0,0,0,0,0,0,0,0};
    int trans[3] = {0,2,4};
    int retrans[5] = {0,0,1,0,2};
    if (sign == 'o') {
        nsign = 'x';
    } else {
        nsign = 'o';
    }
    for(int y = 0; y < 5; y+=2){
        for (int x = 0 ; x < 5; x+=2){
            if(gameBoard[y][x] == sign){
                worth[retrans[y]]++;
                block[retrans[y]]-=10;
            }
            if(gameBoard[y][x] == nsign){
                worth[retrans[y]]-=10;
                block[retrans[y]]++;
            }
        }
    }
    for(int x = 0; x < 5; x+=2){
        for (int y = 0 ; y < 5; y+=2){
            if(gameBoard[y][x] == sign){
                worth[3+retrans[x]]++;
                block[3+retrans[x]]-=10;
            }
            if(gameBoard[y][x] == nsign){
                worth[3+retrans[x]]-=10;
                block[3+retrans[x]]++;
            }
        }
    }
    int k =3;
    while(k -- > 0){
        if(gameBoard[trans[k]][trans[k]] == sign){
            worth[6]++;
            block[6]-=10;
        }
        if(gameBoard[trans[k]][trans[k]] == nsign){
            worth[6]-=10;
            block[6]++;
        }
        if(gameBoard[trans[k]][trans[2-k]] == sign){
            worth[7]++;
            block[7]-=10;
        }
        if(gameBoard[trans[k]][trans[2-k]] == nsign){
            worth[7]-=10;
            block[7]++;
        }
    }
    int max = 0;
    bool blocking = false;
    k=8;
    while (k-->0) if (block[k] == 2){
        max = k;
        blocking = true;
    }
    k=8;
    if (!blocking){
        while(k-->0){
            if (worth[k] > worth[max]) max = k;
        }
    }
    k=8;while(k-->0)printf("%d\n",worth[k]);printf("%d\n\n",max);
    k=3;
    if (max < 3){ //wiersz najlepszy
        while(k-->0){
            if(gameBoard[trans[max]][trans[2-k]] == ' ') {
                gameBoard[trans[max]][trans[2 - k]] = sign;
                break;
            }
        }
    } else if (max < 6){ //kolumna najleszy
        while(k-->0){
            if(gameBoard[trans[2-k]][trans[max-3]] == ' '){
                gameBoard[trans[2-k]][trans[max-3]] = sign;
                break;
            }
        }
    } else { //skos najlepszy
        if (max == 6){
            while(k-->0){
                if(gameBoard[trans[2-k]][trans[2-k]] == ' '){
                    gameBoard[trans[2-k]][trans[2-k]] = sign;
                    break;
                }
            }
        } else {
            while(k-->0){
                if(gameBoard[trans[2-k]][trans[k]] == ' '){
                    gameBoard[trans[2-k]][trans[k]] = sign;
                    break;
                }
            }
        }
    }
}
void makeMove(char gameBoard[5][5], char sign){
    int x,y;
    int dict[3] = {0,2,4};
    printf("Podaj wspolrzedna x: ");
    scanf("%1d",&x);
    printf("Podaj wspolrzedna y: ");
    scanf("%1d",&y);
    while(x < 1 || x > 3 || y < 1 || y > 3){
        printf("Bledne dane. Sprobuj ponownie. \n");
        printf("Podaj wsporzedna x: ");
        scanf("%1d",&x);
        printf("Podaj wsporzedna y: ");
        scanf("%1d",&y);
    }
    while (gameBoard[dict[y-1]][dict[x-1]] != ' '){
        printf("To miejsce jest juz zajete.\n");
        printf("Podaj wsporzedna x: ");
        scanf("%1d",&x);
        printf("Podaj wsporzedna y: ");
        scanf("%1d",&y);
    }
    gameBoard[dict[y-1]][dict[x-1]] = sign;
}

void drawBoard(char gameBoard[5][5]){
    printf("   1    2    3\n");
    int l = 1;
    for (int i = 0; i < 5; i++){
        (i%2) ? printf(" ") : printf("%d",l++);
        for (int j = 0; j < 5; j++){
            printf(" %c ",gameBoard[i][j]);
        }
        printf("\n");
    }
}
bool check(char gameBoard[5][5],char sign){
    if (gameBoard[2][2] == sign && gameBoard[2][2] == gameBoard[0][4] && gameBoard[2][2] == gameBoard[4][0]) return true;
    if (gameBoard[2][2] == sign && gameBoard[2][2] == gameBoard[4][4] && gameBoard[2][2] == gameBoard[0][0]) return true;
    for (int i = 0; i < 5; i += 2){
        if (gameBoard[i][0] == sign && gameBoard[i][0] == gameBoard[i][2] && gameBoard[i][2] == gameBoard[i][4]) return true;
        if (gameBoard[0][i] == sign && gameBoard[0][i] == gameBoard[2][i] && gameBoard[2][i] == gameBoard[4][i]) return true;
    }
    return false;
}
void gameLoop(char gameBoard[5][5],char playerName[2][30]){
    char signOfChoice;
    // Wybór znaku jakim oznaczany będzie ruch pierwszego gracza ze zbioru {o,x},gdy w grze nie ma S.I
    if(strcmp(playerName[0],"Bot") && strcmp(playerName[1],"Bot")){
        printf("Gracz %s wybiera swoj znak (x/o) : ",playerName[0]);
        scanf("%1s",&signOfChoice);
        while(!(signOfChoice == 'o' || signOfChoice == 'x')){
            printf("Bledny znak, wybierz nowy: \n");
            scanf("%1s",&signOfChoice);
        }
    } else {
        //ustawienie znaku S.I na o
        if(!strcmp(playerName[0],"Bot")) signOfChoice='o';
        else signOfChoice='x';
    }



    char sign;
    //Główna pętla gry, narysowanie planszy, wykonanie ruchu, warunek zakończenia, ewentualna możliwośc zagrania jeszcze raz
    for (int i = 0; i < 8; i++){
        //ustawienie znaku w grze
        sign = (i+(int)(signOfChoice=='x'))%2 ? 'x':'o';
        //system("clear");
        drawBoard(gameBoard);
        printf("Tura gracza: %s. (znak to %c)\n",playerName[i%2],sign);
        !strcmp(playerName[i%2],"Bot")?botMove(gameBoard,sign):makeMove(gameBoard,sign);
        //system("clear");
        //drawBoard(gameBoard);
        if ((i>3 && check(gameBoard,sign)) || i==7){
            drawBoard(gameBoard);
            i==7?printf("Gra zakonczona remisem.\n"):printf("Wygral gracz %s!\n",playerName[i%2]);
            break;
        }
    }
    printf("Chcesz zagrać ponownie? (t/n) :");
    char playAgain;
    scanf("%1s",&playAgain);
    while(!(playAgain == 't' || playAgain == 'n')){
        printf("Bledna opcja! \n");
        scanf("%1s",&playAgain);
    }
    playAgain == 'n' ? printf("Dziekuje za gre.\n"): game();
}
//Funkcja uruchamiająca grę.
void game(){
    //system("clear");

    char gameBoard[5][5] = {{' ','|',' ','|',' '},
                            {'-','+','-','+','-'},
                            {' ','|',' ','|',' '},
                            {'-','+','-','+','-'},
                            {' ','|',' ','|',' '}};
    char playerName[2][30];
    char winner[30];
    printf("Podaj imie pierwszego gracza: ");
    scanf("%30s",playerName[0]);
    printf("Podaj imie drugiego gracze: ");
    scanf("%30s",playerName[1]);
    strcmp(playerName[0],playerName[1])?gameLoop(gameBoard,playerName):printf("Nie moze byc dwoch takich samych graczy :( ");
}


int main(){
    game();
}
