#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Car{
    int year;
    char * brand;
    int hp;                         // horsepower
    int seats;                      // num of seats
};

typedef struct tagListElement {     // element listy
    struct tagListElement *next;
    struct tagListElement *prev;
    struct Car * car;
} ListElement;

typedef struct tagList {            // lista
    ListElement *head;
    ListElement *tail;
    int size;
} List;

void init(List*list) {              // inicjalizacja poczatkowa listy
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void push_back(List *list, struct Car* car) {                           // dopisanie elementu na koniec listy
    ListElement *element = (ListElement*)malloc(sizeof(ListElement));
    element->car=car;
    element->next=0;
    if(list->tail){                        // jesli lista ma element koncowy to  
        element->prev = list->tail;        // wskaznik na poprzedni element jest ustawiony na poprzedni ogon
        list->tail->next=element;          // i dopisywany element jest ogonem
    }
    else element->prev = 0;

    list->tail=element;
    if(!list->head) list->head=list->tail;          // jesli lista nie ma glowy (pierwsze dopisanie) to wskaznik na glowe listy jest jednoczesnie wskaznikiem na ogon
    list->size++;
}

void push_front(List*list, struct Car* car) {
    ListElement* element = (ListElement*)malloc(sizeof(ListElement));
    element->next = list->head;
    element->car = car;
    if(list->head != NULL) {
        list->head=element;
    } else {
        list->head = element;
        list->tail = element;
    }
    list->size++;
}

void delete_front(List *list){                  // usuwanie pierwszego elementu listy
    ListElement* toDelete;      
    if(list->head == 0) return;
    toDelete = list->head;
    list->head = list->head->next;
    if(list->head == 0) list->tail = 0;
    free(toDelete);
    list->size--;
}

void freeList(List *list) {                     // zwalnianie pamieci
    while(list->head) {
        delete_front(list);                     // z wykorzystaniem funkcji delete_front
    }
}

void dumpList(const List *list) {               // wypisanie elementow listy
    ListElement *curr;
    for(curr=list->head; curr!=0; curr=curr->next){         // iterujemy na wskaznikach zamiast liczbach jak by to bylo zwykle
        printf("%d,",curr->car->year);
        printf("%s,",curr->car->brand);
        printf("%d,",curr->car->hp);
        printf("%d\n",curr->car->seats);
    }
}

char* read_string()                             // funkcja czytająca znaki z klawiatury
{
    char * bufor = (char*)malloc(20*sizeof(char)); 
    fgets(bufor, 20, stdin); 
    bufor[strcspn(bufor, "\n")] = 0;
    return bufor;        
}

/* void insertion_sort(List * list){
    ListElement *current = (ListElement*)malloc(sizeof(ListElement));
    ListElement *key = (ListElement*)malloc(sizeof(ListElement));
    ListElement *previous = (ListElement*)malloc(sizeof(ListElement));
    for(current = list->head->next; current != 0; current = current->next){
        key = current;
        previous = current->prev;
        while(previous != 0 && previous->car->year > key->car->year){
            previous->next = previous;
            previous = previous->prev;
        }
        previous->next = key;
    }
} */

void insertion_sort(List *list) {           // sortowanie
    if (list->size <= 1)                    // jesli lista ma jeden lub zero elementow to jest juz posortowana.
        return;

    ListElement *current = list->head->next;
    while (current != NULL) {
        ListElement *key = current;
        int year = key->car->year;
        ListElement *previous = key->prev;

        while (previous != NULL && previous->car->year > year) {            // przesuwamy w lewo elementy wieksze od klucza            
            previous->next = key->next;
            if (previous->prev != NULL)
                previous->prev->next = key;
            if (key->next != NULL)
                key->next->prev = previous;

            key->prev = previous->prev;
            key->next = previous;
            previous->prev = key;

            if (list->head == previous)                         // Jesli previous był głowa to aktualizujemy wskaznik na głowe
                list->head = key;

            previous = key->prev;
        }

        // przechodzimy do nastepnego elementu
        current = current->next;
    }
}

struct Car * read(){                // funkcja wczytujaca kolejne rekordy samochodow
    struct Car * p = (struct Car *)malloc(sizeof(struct Car));
    printf("Podaj rok produkcji auta: ");
    scanf("%d", &p->year);
    getchar();
    printf("Podaj marke: ");
    p->brand = read_string();
    printf("Podaj liczbe koni mechanicznych: ");
    scanf("%d", &p->hp);
    printf("Podaj liczbe siedzen: ");
    scanf("%d", &p->seats);
    return p;
}

int main(){
    List list;
    init(&list);
    for(int i=0; i<2; i++){
        push_back(&list, read());
    }
    insertion_sort(&list);
    dumpList(&list);
    freeList(&list);
    return 0;
}

