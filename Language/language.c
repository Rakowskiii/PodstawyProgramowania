/* Nazwa: Wykrywanie języka tekstu
 * Autor: Maciej Rak
 * Opis: Program rozpoznający tekstu umieszonego w pliku ./tekst.txt
 *        na podstawie analizy częstotliwośći występowania liter.
 *        Dla lepszej dokładności zaleca się zamienić znaki specyficzne językowo
 *        na ich podstawowe odpowiedniki. (np. ę -> e, ó -> o)
 *        Nie jest to jednak wymagane.
 *
 *Instrukcja: Należy wkleić interesujący nas tekst do pliku ./tekst.txt
 *            Program porówna je ze statystykami ze znanych mu języków.
 *            Statystyki odpowiednich języków umieszoczne są w plikach:
 *                  ./lang-[nazwa-języka].txt
 *Pomocny link: https://miniwebtool.com/remove-accent/
 * ^ jest to narzędzie online służące do usuwania znaków specjalnych, zgodnie z ^
 * ^ zaleceniem ^
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EN 0
#define PL 1
#define FR 2
#define DE 3
//Parsuje % wystąpien w danym jezyku z pliku języka
void parseStats(FILE* f, float* tab){
    char a;
    char buff[10];
    float stat;
    for(int i = 0; i < 26; i++) {
        a = (char) fgetc(f);
        fgets(buff, 20, f);
        tab[(int) a - 97] = strtof(buff, NULL);
    }
}

void Analiza(float* tekst, float* jezyk,char* s){
    printf("\nLitera    |    %% tekstu    |    %% tekstu w jezyku %s\n",s);
    for (int i = 0; i< 26; i++){
        printf("     %c    |    %.2f%%     |     %.2f%%\n", (char)(i+97),jezyk[i],tekst[i]);
    }
}

//Zwraca index najmniejszej wartosci
int minIndex(float* tab, int size){
    int index = 0;
    for(int i = 1; i < size; i++){
        if (tab[i] < tab[index]) index = i;
    }
    return index;
}


void displayINT(int* tab) {
    for (int i = 0; i < 26; i++) {
        printf("%c : %d\n", (char) (97 + i), tab[i]);
    }
}
void displayFLOAT(float* tab) {
    for (int i = 0; i < 26; i++) {
        printf("%c : %f\n", (char) (97 + i), tab[i]);
    }
}

//wartość bezwzględna z liczby zmiennoprzecinkowej
float absf(float a){
    if (a<0) a *=-1;
    return a;
}

int main() {
    //Otawrcie pliku
    FILE* f;
    f = fopen("./tekst.txt","r");
    if (f != NULL){
        printf("Plik z danymi otworzyony pomyslnie.\n");
    } else {
        printf("Nie udalo sie otworzyc pliku z danymi.\n");
    }


    //Zliczenie liter w tekscie
    int counter[26] = {0};
    char currChar = (char)fgetc(f);
    int num = 0;
    do {
        //DEBUG printf("%c",currChar)
        if ((int)currChar > 64 && (int)currChar < 91) currChar += 32;
        if(currChar < 123 && currChar > 96){
            counter[currChar-97]++;
            num++;
        }
        currChar = (char)fgetc(f);
    } while (currChar != EOF);
    fclose(f);

    //Utworzenie tablicy % wystąpień liter z tablicy ilości liter
    float stat[26];
    for (int i = 0; i < 26; i++) {
        stat[i] = ((float) counter[i] / (float) num)*100;
    }

    //Wczytanie statystyk jezykowych "lang-en.txt","lang-pl.txt","lang-fr.txt","lang-de.txt"
    char langs[4][20] = {
            "lang-en.txt",
            "lang-pl.txt",
            "lang-fr.txt",
            "lang-de.txt",
    };

    FILE* buff;
    float langStats[4][26] = {0};
    for (int i = 0; i<4;i++){
        buff = fopen(langs[i],"r");
        buff == NULL?printf("Nie udalo sie otworzyc pliku z pakietem jezykowym: %s.\n",langs[i]):printf("Plik z pakietem jezykowym: %s otworzyony pomyslnie.\n",langs[i]);
        parseStats(buff,langStats[i]);
        fclose(buff);
    }

   //Porownanie statystyk tekstu z pakietami
   float sum[4] = {0};
   for (int i = 0; i < 4; i++){
       for(int j = 0; j < 26; j++) {
           sum[i] += absf(stat[j] - langStats[i][j]);
       }
   }
   printf("\n\n");
   switch(minIndex(sum,4)){
       char jezyk[20];
       case DE:
           strcpy(jezyk, "niemieckim");
           printf("Analiza wystapien liter wskazuje, ze tekst jest najprawdopodobniej w jezyku %s.\n",jezyk);
           Analiza(stat,langStats[DE],jezyk);
           break;
       case PL:
           strcpy(jezyk, "polskim");
           printf("Analiza wystapien liter wskazuje, ze tekst jest najprawdopodobniej w jezyku %s.\n",jezyk);
           Analiza(stat,langStats[PL],jezyk);
           break;
       case FR:
           strcpy(jezyk, "francuskim");
           printf("Analiza wystapien liter wskazuje, ze tekst jest najprawdopodobniej w jezyku %s.\n",jezyk);
           Analiza(stat,langStats[FR],jezyk);
           break;
       case EN:
           strcpy(jezyk, "angielskim");
           printf("Analiza wystapien liter wskazuje, ze tekst jest najprawdopodobniej w jezyku %s.\n",jezyk);
           Analiza(stat,langStats[EN],jezyk);
           break;
   }

    return 0;
}
