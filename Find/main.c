/* Nazwa: Wyszukiwanie słowa w pliku
 * Autor: Maciej Rak
 * Opis: Program przeszukujący plik ./tekst.txt w poszukiwaniu słowa podanego na standardowym wejściu.
 *       Program zamienia wszystkie litery na małe, oraz usuwa znaki nie będące literami, aby
 *       na pewno znaleźć wszystkie słowa zgodne z podanym, bez względu na zapis.
 *
 *Instrukcja: Należy umieścić interesujący nas plik w folderze z programem pod nazwą tekst.txt,
 *            na wejściu należy podać szukane słowo.
 *
 * Przykłady: Szukane słowo: "Ty" -> fraza szukana przez program: ty
 *            Tekst: "nie spodziewałem się że to będziesz TY! Chyba że to nie Ty, to kto?"
 *            Wynik: Slowo znaleziono : 2 razy w tekscie: tekst.txt
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

//Srpawdzanie czy znak jest malą literą ascii
int isLetter(char a){
    return (a < 123 && a > 96)?1:0;
}

//Modyfikacja łancucha znaków: usuwanie zbędnych znaków, zamiana liter na małe
void strip(char* buff){
    int ptr = 0;
    int temp;
    while(buff[ptr]!='\0'){
        buff[ptr] = (char)tolower(buff[ptr]);
        if (!isLetter(buff[ptr])){
            temp = ptr;
            while (buff[ptr]!='\0'){
                buff[ptr]=buff[ptr+1];
                ptr++;
            }
            ptr = temp-1;
        }
        ptr++;
    }
}
int main() {
    //Otwarcie pliku
    char plik[] = "tekst.txt";
    FILE* f = fopen(plik,"r");

    //Utworzenie bufera na słowa, zakładamy że nie ma słów dłuższych niż 1024 litery
    char word[1024];
    //Wczytujemy słowo ze standardowego wejscia
    fgets(word,1024,stdin);
    strip(word);


    //Zczytujemy kolejne słowa z pliku, stripujemy i porównujemy ze słowem ze standardowego wejścia
    char x[1024];
    int counter = 0;
    while (fscanf(f, " %1024s", x) == 1) {
        strip(x);
        if(strcmp(x,word)==0) counter++;
    }
    printf("Slowo \"%s\" znaleziono : %d razy w tekscie: %s",word,counter,plik);

    return 0;
}
