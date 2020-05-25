/* Nazwa: Rescue
 * Autor: Maciej Rak
 * Opis: Program pozwala symulować ruch pieszych w ewakuowanym budynku. Trasa ewakuacji jest ustalana 
 * poprzez algorytm szukania najkrótszej trasy A*.
 * Instrukcja: Należy edytować tablice 'map', tak aby odpowiadała budynkowi który chcemy symulować.
 * Do oznaczenia ścian można użyć dowolnego znaku ASCII oprócz znaków zarezerwowanych:
 *  'o' - oznaczające człowieka, których umieszczamy dowolną ilość na mapie, jednak w przypadku gdy jest ich więcej niż 50 
 *  należy zmienić wartość MAX_PPL na odpowiednią.
 *  ' ' - oznaczające przestrzeć wolną (lub przejście) którą można użyć do przemieszczania się.
 *  'X' - oznaczające wyjście, (a także po wykonaniu programu wskazuje osoby, z których połozenia nie istniała ścieżka do wyjścia.
 * Do poprawnego działania programu należy po edycji mapy, zmienić wartości H i W na kolejno wysokość i szerokość pomieszczenia.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define H 15
#define W 40
#define EXIT 'X'
#define MAX_PPL 100


struct tNode{
    int x;
    int y;
    int g;
    int h;
    int f;
    struct tNode* parent;
    struct tNode* next;
};
typedef struct tNode Node;

typedef struct{
    Node* path;
    int x;
    int y;
    short finished;
} Entity;

void move (char map[H][W],Entity* n) {
    map[n->y][n->x] = ' ';
    n->y = n->path->y;
    n->x = n->path->x;
    map[n->y][n->x] = 'o';
}

void draw(char map[H][W]){
    printf("\n\n\n\n\n\n");
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            printf("%c",map[i][j]);
        }
        printf("\n");
    }
}

Node* newNode(int x, int y){
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->x = x;
    temp->y = y;
    temp->next = NULL;
    return temp;
}

Node* append(Node* head, Node* elem){
    Node* temp = (Node*)malloc(sizeof(Node));
    memcpy(temp,elem,sizeof(Node));
    temp->next = head;
    return temp;
}

Node* delete(Node* head, Node* elem){
    Node* temp = head;
    Node* res = NULL;
    while (temp != NULL) {
        if(memcmp(temp,elem,sizeof(int)*4)) res = append(res,temp);
        temp = temp->next;
    }
    return res;
}

void printLL(Node* head){
    Node* temp = head;
    while (temp != NULL) {
        printf("%d %d\n",temp->x,temp->y);
        temp = temp->next;
    }
}

int abs(int a){
    return a<0?-a:a;
}

Node* makeWay(Node* head,Node* stop){
    Node* ptr = head;
    Node* res= stop;
    while(ptr != NULL){
        res = append(res,ptr);
        ptr = ptr->parent;
    }
    return res;
}

Node* lowest_f(Node* head){
    Node* lowest_f=head;
    Node* temp;
    temp = head;
    while(temp != NULL){
        if ( temp->f < lowest_f->f) lowest_f=temp;
        temp = temp->next;
    }
    return lowest_f;
}

short has_better(Node* head, int x, int y, int f){
    if(head == NULL) return 0;
    if (head->next == NULL){
        if (head->x == x && head->y == y && head->f <= f) return 1;
    } else {
        Node* temp = head;
        while(temp->next != NULL){
            if(temp->x == x && temp->y == y && temp->f <= f ) return 1;
            temp = temp->next;
        }
    }
    return 0;
}
short inbound(Node* n, int x, int y){
    short x_inbound = (n->x+x < W && n->x+x > 0);
    short y_inbound = (n->y+y < H && n->y+y > 0);
    return x_inbound && y_inbound;
}
Node* A_star(char map[H][W], Node* start, Node* stop){
    int added;
    Node* open = NULL;
    Node* close = NULL;
    start->g = 0;
    open = append(open,start);
    Node* q;
    while(open != NULL){
        q = lowest_f(open);
        open = delete(open,q);
        for (int i = -1; i < 2; i++){
            for (int j = -1; j < 2; j++){
                if (i | j && inbound(q,i,j)) {
                    //check for end
                    if(map[q->y+j][q->x+i]==EXIT) return q;
                    if(map[q->y+j][q->x+i]==' '||map[q->y+j][q->x+i]=='o'){
                        Node* temp =  (Node*)malloc(sizeof(Node));
                        temp->x = q->x+i;
                        temp->y = q->y+j;
                        temp->next = NULL;
                        temp->parent = q;
                        added = (abs(i)+abs(j))==2?14:10;
                        temp->g = q->g + added;
                        temp->h = (int)sqrt((temp->x-stop->x)*(temp->x-stop->x)+(temp->y-stop->y)*(temp->y-stop->y));
                        temp->h *= 10;
                        temp->f = temp->g+temp->h;
                        if (has_better(open,temp->x, temp->y, temp->f)) continue;
                        if (has_better(close,temp->x, temp->y, temp->f)) continue;
                        open = append(open,temp);
                    }
                }
            }
        }
        close = append(close,q);
    }
    return NULL;
}

Entity* getPeopleList(char map[H][W], int* size, Node* stp){
    Entity* temp = (Entity*)malloc(sizeof(Entity)*MAX_PPL);
    int ptr = 0;
    Node* stop;
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            if (map[i][j]=='o'){
                temp[ptr].x = j;
                temp[ptr].y = i;
                temp[ptr].finished =0;
                ptr++;
            }
            if (map[i][j]==EXIT){
                stop = newNode(i,j);
            }
        }
    }
    for (int i = 0; i < ptr; i++){
        temp[i].path = makeWay(A_star(map,newNode(temp[i].x,temp[i].y),stop),stop);
    }
    Entity* res = (Entity*)malloc(sizeof(Entity)*(ptr+1));
    memcpy(res,temp,sizeof(Entity)*(ptr+1));
    memcpy(stp,stop,sizeof(Entity));
    *size = ptr;
    free(temp);
    return res;
}

int main() {
char map[H][W] = {
            "########################################",
            "#o #                          #       o#",
            "#  #   #  #         #         #   o    #",
            "#  #   #  #         #         #  o     #",
            "#  #   #  #         #  o      #    o   #",
            "#      #o #         #              o   #",
            "##### ##### ### ######### ##############",
            "#o        #  o      #   #  oo #     o  #",
            "# o       #         #   ###ooo#  o     #",
            "#         #      o  #   #######    o   #",
            "#      o  # o      o#oooo     # o      #",
            "############## ############# ####### ###",
            "#                            #o   o  o #",
            "#                   ####     #  o   o  #",
            "###################EXIT#################",
    };
    int rescued,nrescued;
    nrescued = 0;
    rescued = 0;
    int size;
    Node stop;
    Entity* people = getPeopleList(map,&size,&stop);
    short ongoing = 1;
    for(int i = 0; i < size; i++){people[i].path=people[i].path->next;}//brzydki sposób na naprawę kolizji, brak czasu na poprawę
    while(ongoing){
        ongoing = 0;
        for(int i = 0; i < size; i++){
            if(people[i].path!=NULL){
               if(map[people[i].path->y][people[i].path->x] != 'o'){
                    map[people[i].y][people[i].x] = ' ';
                    people[i].y = people[i].path->y;
                    people[i].x = people[i].path->x;
                    map[people[i].y][people[i].x] = 'o';
                    people[i].path = people[i].path->next;
                }
                ongoing = 1;
            } else {
                if(!people[i].finished){
                    if(people[i].x==stop.x && people[i].y==stop.y){
                        rescued++;
                    } else nrescued++;
                    people[i].finished=1;
                    map[people[i].y][people[i].x]='X';
                }
            }
        }
        usleep(430000);
        draw(map);
        printf("Jak na razie udalo sie ewakuowac %d/%d osob.\n",rescued,size);
    }

    printf("\nEwakuacja zakonczona sukscesem, uratowano %d/%d osob.\n Drogi do wyjscia nie znalazlo %d osob.\n",rescued,size,nrescued);
}

