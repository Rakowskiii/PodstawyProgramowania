/* Nazwa: Matrix
 * Autor: Maciej Rak
 * Opis: Program pozwala sprawdzić czy macierz zawarta w pliku '.cvs' ./macierz.csv jest macierzą trójkątną.
 * Instrukcja: Należy w pliku ./macierz.csv umieścić macierz, zapisując w kolejnych wierszach pliku 
 * kolejne wiersze macierzy. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define buff_size 255


int main() {
    FILE* file = fopen("macierz.csv","r");
    char* buff = (char*)malloc(sizeof(char)*buff_size);
    char* pch;
    int h = 1;
    int w = -1;

    fgets(buff,255,file);
    pch = strtok (buff,";");
    while (pch != NULL){     //Zliczenie kolumn
        w++;
        pch = strtok (NULL, ";");
    }

    while(fgets(buff,255,file)){      //Zliczenie wierszy
        h++;
    }

    fseek(file,0,0);     //Reset kursora
    int i;
    i = 0;
    short dolna,gorna; // zmienne posłużą do sprawdzenia stanu logicznego górnej i dolnej części macierzy (zero jeśli nie zawiera elementów niezerowych)
    dolna = 0;
    gorna = 0;
    if (w==h){   //sprawdzenie czy jest kwadratowa
        while(fgets(buff,255,file)!=0){
            pch = strtok (buff,";\n");
            //while (pch != NULL)
            for(int j = 0; j < i; j++){ //sumowanie logiczne (istnieją/nie istnieją) niezerowych elementów poniżej przekątnej
                gorna += (strlen(pch)==1 && (int)pch[0]==48)?0:1;
                pch = strtok (NULL, ";\n");
            }
            pch = strtok (NULL, ";\n");
            for(int j = i+1; j < w; j++){ //sumowanie logiczne (istnieją/nie istnieją) niezerowych elementów powyżej przekątnej
                dolna += (strlen(pch)==1 && (int)pch[0]==48)?0:1;
                pch = strtok (NULL, ";\n");
            }
            i++;
        }
    } else {
        dolna = 1;
        gorna = 1;
    }

    printf("Macierz zawarta w pliku %s macierzą trojkatną dolną%s %s macierzą trojkątną gorną%s",dolna?"nie jest":"jest",dolna==gorna?" i":",lecz",gorna?"nie jest":"jest",w!=h?", ponieważ macierz nie jest kwadratowa.\n":".\n");
    return 0;
}

