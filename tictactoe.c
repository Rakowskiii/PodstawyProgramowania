#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void game();

int makeMove(char gameBoard[5][5], char sign){
    int x,y;
    int dict[3] = {0,2,4};
    printf("Podaj wsporzedna x: ");
    scanf("%1d",&x);
    printf("Podaj wsporzedna y: ");
    scanf("%1d",&y);
    while(x < 1 || x > 3 || y < 1 || y > 3){
        printf("Błędne dane. Spróbuj ponownie. \n");
        printf("Podaj wsporzedna x: ");
        scanf("%1d",&x);
        printf("Podaj wsporzedna y: ");
        scanf("%1d",&y);
    }
    while (gameBoard[dict[y-1]][dict[x-1]] != ' '){
        printf("To miejsce jest juz zajęte.\n");
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
    printf("Gracz %s wybiera swoj znak (x/o) : ",playerName[0]);
    char signOfChoice;
    scanf("%1s",&signOfChoice);
    while (!(signOfChoice=='o' || signOfChoice=='x')){
        printf("Błędny znak, spróbuj ponownie: ");
        scanf("%1s",&signOfChoice);
    }
    char sign;
    for (int i = 0; i < 8; i++){
        sign = (i+(int)(signOfChoice=='x'))%2 ? 'x':'o';
        system("clear");
        drawBoard(gameBoard);
        printf("Tura gracza: %s. (znak to %c)\n",playerName[i%2],sign);
        makeMove(gameBoard,sign);
        system("clear");
        drawBoard(gameBoard);
        if (i>3 && check(gameBoard,sign)){
            printf("Wygrał gracz %s!\n",playerName[i%2]);
            break;
        }
        if (i == 7) printf("Gra zakończona remisem.\n");
    }
    printf("Chcesz zagrać ponownie? (t/n) :");
    char playAgain;
    scanf("%1s",&playAgain);
    while(!(playAgain == 't' || playAgain == 'n')){
        printf("Błędna opcja! \n");
        scanf("%1s",&playAgain);
    }
    playAgain == 'n' ? printf("Dziękuje za gre.\n"): game();
}

void game(){
    system("clear");
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
    gameLoop(gameBoard,playerName);
}


int main(){
    game();
}