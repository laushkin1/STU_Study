/*
Meno a priezvisko: Ivan Laushkin

POKYNY:
(1)  Implementujte funkcie tak, aby splnali popis pri ich deklaraciach.
(2)  Cela implementacia musi byt v tomto jednom subore.
(3)  Odovzdajte len tento zdrojovy subor (s vypracovanymi rieseniami).
(4)  Program musi byt kompilovatelny.
(5)  Globalne a staticke premenne su zakazane.
(6)  V ziadnom pripade nemente deklaracie funkcii, ktore mate za ulohu naprogramovat
     (nemente nazvy, navratove hodnoty, ani typ a pocet parametrov v zadanych funkciach).
     Nemente implementacie zadanych datovych typov, ani implementacie hotovych pomocnych funkcii
     (ak nie je v zadani ulohy uvedene inak).
(7)  V pripade potreby mozete kod doplnit o dalsie pomocne funkcie alebo datove typy.
(8)  Vase riesenie otestujte (vo funkcii 'main' a pomocou doplnenych pomocnych funkcii alebo datovych typov).
     Testovaci kod ale nebude hodnoteny.
(9)  Funkcia 'main' musi byt v zdrojovom kode posledna.
*/

#include <iostream>
#include <cstring>

using namespace std;

//-------------------------------------------------------------------------------------------------
// DATOVE TYPY
//-------------------------------------------------------------------------------------------------

// Uzol zretazeneho zoznamu
struct Node {
    int data; // hodnota uzla
    Node* next; // smernik na dalsi uzol zoznamu
};

// Zretazeny zoznam
struct List {
    Node* first; // smernik na prvy uzol zoznamu
};

struct ListData {
    int *data; // pole hodnot uzlov
    size_t len; // dlzka pola 'data'
};

//-------------------------------------------------------------------------------------------------
// 1. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia usporiada pole 'data' od najvacsieho prvku po najmensi prvok.
    Pouzite algoritmus insertion sort.

    PARAMETRE:
        [in, out] data - pole, ktore funkcia usporiada
        [in] length    - pocet prvkov pola

    VSTUPNE PODMIENKY:
        'length' moze mat lubovolnu hodnotu
        'data' ukazuje na platne pole

    PRIKLADY:
        (1, 3, 2) -> (3, 2, 1)
        (1, 2, 2, 1) -> (2, 2, 1, 1)
        (5) -> (5)
        () -> ()
*/
void insertionSort(int *data, const size_t length) {
    for(size_t i = 0; i<length; i++){
        int key = data[i];
        int j = i-1;

        while(j >= 0 && data[j] < key){
            data[j+1] = data[j];
            j--;
        }
        data[j+1] = key;
    }
}

//-------------------------------------------------------------------------------------------------
// 2. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia usporiada textove retazce v poli 'data' od najvacsieho prvku po najmensi (lexikograficky).
    Preusporiadajte smerniky v poli.
    Pouzite algoritmus insertion sort.

    PARAMETRE:
        [in, out] data - pole, ktore funkcia usporiada.
                Pole obsahuje smerniky na textove retazce.
                Kazdy textovy retazec je ukonceny '\0'.
                Posledny smernik ma hodnotu 'nullptr'. Podla toho urcite pocet prvkov pola (pocet textovych retazcov).

    VSTUPNE PODMIENKY:
        'data' obsahuje minimalne jeden smernik.
        Posledny smernik ma hodnotu 'nullptr'.

    PRIKLADY:
        ("Juraj", "Peter", "Andrej", nullptr) -> ("Peter", "Juraj", "Andrej", nullptr)
        ("Juraj", "Anabela", "Peter", "Andrej", nullptr) -> ("Peter", "Juraj", "Andrej", "Anabela", nullptr)
        ("Andrej", "Juraj", "Andrej", nullptr) -> ("Juraj", "Andrej", "Andrej", nullptr)
        ("Andrej", nullptr) -> ("Andrej", nullptr)
        (nullptr) -> (nullptr)

    POZNAMKY:
        Pri testovani mozete jednoducho pole vytvorit nasledovnym sposobom:
        const char *mena[] = {"Juraj", "Peter", "Andrej", nullptr};

        Na porovnanie obsahu textovych retazcov vyuzite prislusnu funkciu zo standardnej kniznice.
*/
void insertionSort(const char *data[]) {
    for(int i = 0; data[i] != nullptr; i++){
        const char* key = data[i];
        int j = i-1;

        while(j >= 0 && strcmp(data[j], key) < 0){
            data[j+1] = data[j];
            j--;
        }
        data[j+1] = key;
    }
}

//-------------------------------------------------------------------------------------------------
// 3. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia usporiada zretazeny zoznam 'list' od najvacsieho prvku po najmensi.
    Preusporiadajte uzly v zozname (nekopirujte hodnoty v uzloch).
    Pouzite algoritmus .insertion sort

    PARAMETRE:
        [in, out] list - zretazeny zoznam, ktory funkcia usporiada

    VSTUPNE PODMIENKY:
        'list' obsahuje lubovolny pocet uzlov (moze byt prazdny)
        'list' nie je 'nullptr'

    PRIKLADY:
        vstup: 2->1->3,        vystup: 3->2->1
        vstup: 1->2->2->1,     vystup: 2->2->1->1
        vstup: prazdny zoznam, vystup: prazdny zoznam
*/
Node* insertSortList(Node* first, Node* newNode){
    if(!first || (int)newNode->data > (int)first->data){
        newNode->next = first;
        return newNode;
    }
    Node *current = first;
    while(current->next && (int)current->next->data > (int)newNode->data)
        current = current->next;
    newNode->next = current->next;
    current->next = newNode;
    return first;
}

void insertionSort(List *list) {
    if(list->first == nullptr) return;
    Node *sorted = nullptr;
    Node *current = list->first;
    while (current != nullptr) {
        Node* next = current->next;
        sorted = insertSortList(sorted, current);
        current = next;
    }
    list->first = sorted;
}

//-------------------------------------------------------------------------------------------------
// 4. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vykona algoritmus merge (cast algoritmu merge sort), ktory ma linearnu vypoctovu zlozitost.
    Kombinuje dve susedne, usporiadane casti v poli 'input', do jednej usporiadanej casti v poli 'output'.
    Usporiadanie je od najvacsieho prvku po najmensi prvok!

    PARAMETRE:
        [out] output - vystupne pole, ktoreho cast output[low]...output[high-1] bude po vykonani funkcie usporiadana
        [in]  input  - vstupne pole, ktoreho casti input[low]...input[middle-1] a input[middle]...input[high-1]
                         musia byt pri volani funkcie usporiadane od najvacsieho prvku po najmensi
        [in]  low    - index 1. prvku lavej usporiadanej casti pola 'input'
        [in]  middle - index 1. prvku pravej usporiadanej casti pola 'input'
        [in]  high   - index za poslednym prvkom pravej usporiadanej casti pola 'input'

    VYSTUPNE PODMIENKY:
        Obsah 'input' je nezmeneny.
        output[low] ... output[high-1] obsahuje usporiadane prvky z input[low] ... input[high-1].
        Prvky s indexami mensimi ako 'low' sa nemenia (ani v jednom poli).
        Prvky s indexami vacsimi alebo rovnymi ako 'high' sa nemenia (ani v jednom poli).

    PRIKLAD:
        low: 4                                          low            middle           high
        middle: 8                                        |               |               |
        hight: 12                                        V               V               V
        input:                         (10, 10, 10, 10,  7,  5,  2,  0,  8,  4,  2,  1, 10, 10, 10, 10)
        output pred vykonanim funkcie: (20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20)
        output po vykonani funkcie:    (20, 20, 20, 20,  8,  7,  5,  4,  2,  2,  1,  0, 20, 20, 20, 20)
*/

void mergeNeighbours(int *output, const int *input, const size_t low, const size_t middle, const size_t high) {
    size_t i = low;
    size_t j = middle;
    size_t k = low;

    while(i < middle && j < high){
        if(input[i] >= input[j]){
            output[k++] = input[i++];
        }
        else{
            output[k++] = input[j++];
        }
    }

    while(i < middle) output[k++] = input[i++];
    while(j < high)   output[k++] = input[j++];
}

//-------------------------------------------------------------------------------------------------
// 5. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia usporiada prvky v poli 'data' od najvacsieho prvku po najmensi.
    Pouzite algoritmus merge sort.
    Vytvorte jedno pomocne pole. Pre alokaciu a dealokaciu pomocneho pola pouzite new[] a delete[].

    PARAMETRE:
        [in, out] data - pole, ktore funkcia usporiada
        [in] length    - pocet prvkov pola

    VSTUPNE PODMIENKY:
        'data' ukazuje na platne pole

    PRIKLADY:
        (1, 3, 2) -> (3, 2, 1)
        (1, 2, 2, 1) -> (2, 2, 1, 1)
        (5) -> (5)
        () -> ()

    POZNAMKA:
        Ak pouzijete pristup top-down, tak
        - v tejto funkcii zabezpecte vytvorenie a kopirovanie dat do pomocneho pola,
        - vytvorte a zavolajte rekurzivnu funkciu, v ktorej implementujete hlavnu cast algoritmu merge sort.
*/

static void mergesort_td(const int* src, int* dst, size_t left, size_t right) {
    if (left == right) { dst[left] = src[left]; return; }
    size_t mid = left + (right - left) / 2;

    
    mergesort_td(dst, const_cast<int*>(src), left, mid);
    mergesort_td(dst, const_cast<int*>(src), mid + 1, right);


    size_t i = left, j = mid + 1, k = left;
    while (i <= mid && j <= right) {
        if (src[i] >= src[j]) dst[k++] = src[i++];
        else                  dst[k++] = src[j++];
    }
    while (i <= mid)  dst[k++] = src[i++];
    while (j <= right) dst[k++] = src[j++];
}

void mergeSort(int *data, const size_t length) {
    if(length <= 1) return;
    int* aux = new int[length];
    for(size_t i = 0; i<length; i++) aux[i] = data[i];
    mergesort_td(aux, data, 0, length-1);
    delete[] aux;
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

// tu mozete doplnit pomocne funkcie a struktury
void printArray(int arr[], size_t length)
{
    for (size_t i = 0; i < length; ++i)
        cout << arr[i] << " ";
    cout << endl;
}

int main() {
    int arr[] = {1, 9, 2, 8, 3, 7, 4, 6, 5, 0};
    size_t lenght = 10;
    printArray(arr, lenght);
    mergeSort(arr, lenght);
    printArray(arr, lenght);
    return 0;
}
