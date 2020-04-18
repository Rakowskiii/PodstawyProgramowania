/* Nazwa: Kompresja RLE
 * Autor: Maciej Rak
 * Opis: Program CLI kompresujący plik metodą RLE
 *
 *Instrukcja: Przy wywołaniu programu należy podać 2 argumenty [compress/decompress]
 *              decydujący o tym czy plik będzie kompresowany czy dekompresowany,
 *              oraz nazwę pliku na którym planujemy przeprowadzić operację.
 *              plik po kompresji zachowuje nazwę, na koncu zostaje dodane .RLE.
 *              Plik po dekompresji traci rozszerzenie .RLE, wracając do poprzedniego rozszerzenia
 *
 *
 *Przykład działania: plik.plik: {aaaaaaaaabbbbccdeeeeeeeeeeeeaaa} -> plik.plik.RLE: {9a4b2c1d12e3a}
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compress(char* argv) {
    printf("Rozpoczynanie kompresji pliku %s\n",argv);
    FILE* f = fopen(argv,"r");
    if ( f == NULL){
        printf("Nie ma takiego pliku.\n");
        return;
    }

    fseek(f,0, SEEK_END);
    long length = ftell(f);
    fseek(f,0, SEEK_SET);
    char* buff = (char*)malloc(length*sizeof(char));
    fread(buff,length,sizeof(char),f);

    char* compressed = (char*)malloc(2*length*sizeof(char));
    memset(compressed,'\0',2*length);


    long ptr = 0;
    long  clength = 0;
    char temp = buff[0];
    char licznik = 1;

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
    char buffer[100];
    strcpy(buffer,argv);
    strcat(buffer,".RLE");
    f = fopen(buffer,"w");
    for (long i = 0; i < clength; i++){
        fputc(compressed[i],f);
    }
    fclose(f);
    printf("Kompresja pliku %s zakonczona powodzeniem, otrzymany plik to %s .\n",argv,buffer);
}

void decompress(char* argv){
    printf("Dekompresja pliku %s.\n",argv);
    FILE* f = fopen(argv,"r");
    if ( f == NULL){
        printf("Nie ma takiego pliku.\n");
        return;
    }

    fseek(f,0, SEEK_END);
    long length = ftell(f);
    fseek(f,0, SEEK_SET);

    char* buff = (char*)malloc(length*sizeof(char));
    fread(buff,length,sizeof(char),f);
    fclose(f);

    char buffer[100];
    strcpy(buffer,argv);
    buffer[strlen(argv)-4]='\0';
    f = fopen(buffer,"w");
    for (long i = 0; i < length; i+=2){
        for(int j = 0; j < buff[i]; j++){
            fputc(buff[i+1],f);
        }
    }

    fclose(f);
    printf("Zakonczono dekompresje pliku %s do pliku %s .\n",argv,buffer);
}

int main(int arc, char** argv){
    if(!strcmp(argv[1],"compress")) compress(argv[2]);
    else if(!strcmp(argv[1],"decompress")) decompress(argv[2]);
    else printf("Bledna opcja.\n");
    return 0;
}

