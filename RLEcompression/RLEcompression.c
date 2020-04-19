/* Nazwa: Kompresja RLE
 * Autor: Maciej Rak
 * Opis: Program CLI kompresujący plik metodą RLE
 *
 *Instrukcja: Przy wywołaniu programu należy podać 2 argumenty [compress/decompress]
 *              decydujący o tym czy plik będzie kompresowany czy dekompresowany,
 *              oraz nazwę pliku na którym planujemy przeprowadzić operację.
 *              plik po kompresji zachowuje nazwę, na koncu zostaje dodane .RLE.
 *              Plik po dekompresji traci rozszerzenie .RLE, wracając do poprzedniego rozszerzenia
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Kompresja pliku o nazwia argv
void compress(char* argv) {
    printf("Rozpoczynanie kompresji pliku %s\n",argv);
    
    //Otwarcie pliku wejscia
    FILE* f = fopen(argv, "r");
    if (f == NULL){
        printf("Nie ma takiego pliku.\n");
        return;
    }

    //szukanie długości pliku
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    //przepisanie pliku do bufera
    char* buff = (char*)malloc(length*sizeof(char));
    fread(buff, length, sizeof(char),f);

    //zamkciecie pliku
    fclose(f);

    //stworzenie bufera na skompresowany plik i ustawienie wszystkich elementów na null
    char* compressed = (char*)malloc(2*length*sizeof(char));
    memset(compressed, '\0', 2*length);

    //inicjalizacja kompresji
    long ptr = 0;
    long  clength = 0;
    char temp = buff[0];
    char licznik = 1;

    //kompresja
    for (long i = 1; i < length; i++){
        if (temp == buff[i] && licznik != 126){
            licznik++;
        } else {
            compressed[ptr++] = licznik;
            compressed[ptr++] = temp;
            clength += 2;
            licznik = 1;
            temp = buff[i];
        }
    }

    //Utworzenie nazwy pliku wyjscia
    char buffer[100];
    strcpy(buffer, argv);
    strcat(buffer,".RLE");

    //Utworzenie pliku wyjscia
    f = fopen(buffer, "w");
    for (long i = 0; i < clength; i++){
        fputc(compressed[i], f);
    }
    //zamkciecie pliku
    fclose(f);
    printf("Kompresja pliku %s zakonczona powodzeniem, otrzymany plik to %s .\n", argv, buffer);
}

//Dekompresja pliku o nazwie argv
void decompress(char* argv){
    printf("Dekompresja pliku %s.\n", argv);
    
    //Otwarcie pliku wejscia
    FILE* f = fopen(argv, "r");
    if (f == NULL){
        printf("Nie ma takiego pliku.\n");
        return;
    }
    
    //Znalezienie długości pliku
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    //Wczytanie pliku do bufera
    char* buff = (char*)malloc(length*sizeof(char));
    fread(buff, length, sizeof(char), f);
    
    //Zamknięcie pliku
    fclose(f);

    //Utworzenie nazwy pliku wyjscia
    char buffer[100];
    strcpy(buffer,argv);
    buffer[strlen(argv)-4]='\0';
    
    //Otwarcie/utworzenie pliku wyjscia
    f = fopen(buffer, "w");
    
    //Dekompresja buffera to pliku wyjscia
    for (long i = 0; i < length; i+=2){
        for(int j = 0; j < buff[i]; j++){
            fputc(buff[i+1], f);
        }
    }
    //Zamknięcie pliku
    fclose(f);
    
    printf("Zakonczono dekompresje pliku %s do pliku %s .\n", argv, buffer);
}

int main(int arc, char** argv){
    if(!strcmp(argv[1],"compress")) compress(argv[2]);
    else if(!strcmp(argv[1], "decompress")) decompress(argv[2]);
    else printf("Bledna opcja.\n");
    return 0;
}

