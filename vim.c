/* Nazwa: Szyfrator Cezara i Vigenere'a
 * Autor: Maciej Rak
 * Opis: Program służący do szyfrowania wiadomości popularnymi szyframi
 *
 * Instrukcja: Postępując zgodnie z instrukcjami podawanymi przez program, można zaszyfrować dowolny tekst.
 *              Zmiana partości parametru LEN pozwala na dłuższe wiadomości.
 */

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEN 200
#define flush(); while ((getchar()) != '\n');


bool isLetter(char c){
    int a = (int)c;
    if(a >96 && a < 123) return true;
    if(a > 64 && a < 91) return true;
    return false;
}
void prnt(char* tekst){
    int i = -1;
    while(i ++< LEN-1) printf("%c",*(tekst+i));
    printf("\n");
}
void toUpper(char* tekst){
    int i = -1;
    while(i++ < LEN-1){
        if(*(tekst+i)!= ' ' && *(tekst+i)!= 0) {
            if ((int)*(tekst+i) > 96) *(tekst+i) = (char)((int)*(tekst+i)-32);
        }
    }
}

void vigenere(char* tekst){
    char **tab = (char**)malloc(sizeof(char*)*26*26);
    for(int i = 0; i < 26; i++){
        *(tab+i) = (char*)malloc(sizeof(char)*26);
        for (int j =0; j < 26; j++){
            *(*(tab+i)+j) = (char)(65+(j+i)%26);
        }
    }
    int i = -1;
    int wskKlucza = 0;
    printf("Podaj klucz(maximum %d znakow, krotszy klucz zostanie zapetlony): ",LEN);
    char klucz[LEN]= {0};
    fgets(klucz, LEN, stdin);
    klucz[strcspn(klucz, "\n")] = 0;
    prnt(klucz);
    toUpper(klucz);
    toUpper(tekst);
    while (i ++< LEN-1 && tekst[i]!=0){
        if(klucz[wskKlucza] == 0) wskKlucza = 0;
        while(klucz[wskKlucza] == ' ') wskKlucza++;
        if (*(tekst+i) != 0 && *(tekst+i) != ' ') {
            *(tekst+i) = *(*(tab+(int)klucz[wskKlucza++]-65)+(int)tekst[i]-65);
        }
    }
    prnt(tekst);
}

void cezar(char* tekst){
    printf("Podaj wartosc przesuniecia: ");
    int p;
    scanf("%d",&p);
    int i = -1;
    while (i ++< LEN-1) if (*(tekst+i) != 0 && *(tekst+i) != ' ' && isLetter(*(tekst+i)))
            *(tekst+i) = (char)((((int)*(tekst+i)-((int)*(tekst+i)<97?65:97))+p%26)%26+((int)*(tekst+i)<97?65:97));
    printf("Szyfrogram to: ");
    prnt(tekst);
}

int main() {
    printf("Podaj wiadomosc do zaszyfrowania(max 200 znakow): ");
    char tekst[LEN] = {0};
    fgets(tekst, LEN, stdin);
    tekst[strcspn(tekst, "\n")] = 0;
    char tryb;
    printf("Wybierz tryb szyfrowania: (v - Vigenerea/ c - Cezara) : ");
    scanf("%c",&tryb);
    flush();
    switch(tryb){
        case 'c':
            cezar(tekst);
            break;
        case 'v':
            vigenere(tekst);
            break;
        default:
            printf("Bledny wybor.\n");
    }
    return 0;
}
