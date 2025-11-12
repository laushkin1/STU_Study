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

using namespace std;

//-------------------------------------------------------------------------------------------------
// 1. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia prida novy prvok do heapu.
    Verzia heap-u je Min-heap (hodnota kazdeho uzla je mensia alebo rovna, ako hodnoty vsetkych jeho nasledovnikov).
    Pouzite algoritmus sift up.

    PARAMETRE:
        [in, out] data - pole, ktoreho prva cast tvori heap, do ktoreho funkcia prida novy prvok
        [in] addIndex - index prvku v 'data', ktory funkcia prida do heap-u (preusporiadanim prvkov)

    VSTUPNE PODMIENKY:
        'data' ukazuje na platne pole s dlzkou minimalne 'addIndex+1'
        'addIndex' moze mat lubovolnu hodnotu
        Prvky data[0]...data[addIndex-1] (vratane) tvoria heap

    VYSTUPNE PODMIENKY:
        Prvky data[0]...data[addIndex] (vratane) tvoria heap
        Funkcia preusporiadala prvky data[0]...data[addIndex] tak, aby tvorili heap

    PRIKLADY:
        vstup:  data = (2, 4, 10, 7, 1, 2, 5, 0, 3, -1, 11, 12, 1), addIndex = 4
        vystup: data = (1, 2, 10, 7, 4, 2, 5, 0, 3, -1, 11, 12, 1)

        vstup:  data = (3, 4, 10, 5, 5, 11, 15, 7, 8, 9, 10, 14,  8, 1, 2), addIndex = 12
        vystup: data = (3, 4,  8, 5, 5, 10, 15, 7, 8, 9, 10, 14, 11, 1, 2)

        vstup:  data = (3, 4, 10, 5), addIndex = 0
        vystup: data = (3, 4, 10, 5)
*/
void siftUp(int data[], const size_t addIndex)
{
    if(addIndex == 0) return;

    size_t i = addIndex;    
    while(i > 0){
        size_t p = (i-1)/2;
        if (data[p] <= data[i]) break;
        int t = data[p]; data[p] = data[i]; data[i] = t;
        i=p;
    }
}

//-------------------------------------------------------------------------------------------------
// 2. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori heap na poli 'data' preusporiadanim prvkov.
    Verzia heap-u je Min-heap.
    Pouzite algoritmus sift up.

    PARAMETRE:
        [in, out] data - pole, ktore funkcia preusporiada, aby bolo heap-om
        [in] length - pocet prvkov pola

    VSTUPNE PODMIENKY:
        'data' ukazuje na platne pole, ak 'length' > 0
        'length' moze mat lubovolnu hodnotu

    VYSTUPNE PODMIENKY:
        'data' je heap-om

    PRIKLAD:
        vstup:  data = (7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6), length = 11
        vystup: data = (1, 2, 2, 2, 2,  5, 3, 7, 4, 8, 6)
*/
void buildHeapSiftUp(int data[], const size_t length)
{
    if (length <= 0) return;
    for(size_t i = 0; i<length; i++)
        siftUp(data, i);
}

//-------------------------------------------------------------------------------------------------
// 3. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia opravi cast heap-u (podstrom, ktoreho koren ma index 'topIndex').
    Verzia heap-u je Min-heap.
    Pouzite algoritmus sift down.

    PARAMETRE:
        [in, out] data - pole, v ktorom funkcia opravi cast heapu preusporiadanim prvkov
        [in] topIndex - index korena podstromu (casti heapu), ktory sa ma opravit
        [in] length - pocet prvkov pola

    VSTUPNE PODMIENKY:
        Podstromy prvku s indexom 'topIndex' splnaju podmienky heap (podstromy, ktorych korene su priamy nasledovnici uzla s indexom 'topIndex').
        'data' ukazuje na platne pole
        'topIndex' moze mat lubovolnu hodnotu
        'length' moze mat lubovolnu hodnotu

    VYSTUPNE PODMIENKY:
        Podstrom, ktoreho koren ma index 'topIndex', splna podmienku heap.

    PRIKLADY:
        vstup:  data = (55, 20, 10, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140), topIndex = 0, length = 15
        vystup: data = (10, 20, 50, 30, 40, 55, 60, 70, 80, 90, 100, 110, 120, 130, 140)

        vstup:  data = (100, 8, 2, 1, 0, 5, 6, 7, 4, 2, 3, 11, 12, 13, 14, 15, 16, 17), topIndex = 1, length = 18
        vystup: data = (100, 0, 2, 1, 2, 5, 6, 7, 4, 8, 3, 11, 12, 13, 14, 15, 16, 17)
*/
void siftDown(int data[], const size_t topIndex, const size_t length)
{
    int smallest = topIndex;
    int l = 2 * topIndex + 1;
    int r = 2 * topIndex + 2;

    if (l < (int)length && data[l] < data[smallest])
        smallest = l;

    if (r < (int)length && data[r] < data[smallest])
        smallest = r;

    if (smallest != (int)topIndex) {
        int t = data[topIndex]; data[topIndex] = data[smallest]; data[smallest] = t;
        siftDown(data, smallest, length);
    }
}

//-------------------------------------------------------------------------------------------------
// 4. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori heap na poli 'data' preusporiadanim prvkov.
    Verzia heap-u je Min-heap.
    Pouzite algoritmus sift down.

    PARAMETRE:
        [in, out] data - pole, ktore funkcia preusporiada, aby bolo heap-om
        [in] length - pocet prvkov pola

    VSTUPNE PODMIENKY:
        'data' ukazuje na platne pole, ak 'length' > 0
        'length' moze mat lubovolnu hodnotu

    VYSTUPNE PODMIENKY:
        'data' je heap-om

    PRIKLAD:
        vstup:  data = (7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6), length = 11
        vystup: data = (1, 2, 3, 2, 2, 5, 7, 4, 2, 8, 6)
*/
void buildHeapSiftDown(int data[], const size_t length)
{
    if (length < 2) return;
    for (size_t i = length/2; i-- > 0; )
        siftDown(data, i, length);
}

//-------------------------------------------------------------------------------------------------
// 5. ULOHA (0.8 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia preusporiada pole 'data' od najvacsieho prvku po najmensi.
    Pouzite algoritmus heap sort.
    Pre vytvorenie heapu mozete pouzit sift down alebo sift up. Lepsie je pouzit sift down, pretoze je efektivnejsi.

    PARAMETRE:
        [in,out] data - pole, ktore funkcia usporiada
        [in] length - dlzka pola

    VSTUPNE PODMIENKY:
        'data' ukazuje na platne pole, ak 'length' > 0
        'length' moze mat lubovolnu hodnotu

    VYSTUPNE PODMIENKY:
        Pole 'data' je usporiadane

    PRIKLAD:
        vstup:  data = (7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6), length = 11
        vystup: data = (8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 1)
*/
static void siftDownMin(int a[], size_t n, size_t i) {
    while (true) {
        size_t smallest = i;
        size_t l = 2*i + 1;
        size_t r = 2*i + 2;
        if (l < n && a[l] < a[smallest]) smallest = l;
        if (r < n && a[r] < a[smallest]) smallest = r;
        if (smallest == i) break;
        swap(a[i], a[smallest]);
        i = smallest;
    }
}

void heapSort(int data[], const size_t length) {
    if (length < 2) return;

    for (size_t i = length/2; i-- > 0; )
        siftDownMin(data, length, i);

    for (size_t heap = length; heap > 1; ) {
        swap(data[0], data[heap-1]);
        siftDownMin(data, --heap, 0);
    }
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

// tu mozete doplnit pomocne testovacie funkcie a struktury
void printArr(int *date, size_t length){
    for(size_t i = 0; i<length; i++){
        cout << '\'' << date[i] << '\'' << ' ';
    }
    cout << endl;
}

int main() {
/*  1.
    PRIKLADY:
        vstup:  data = (2, 4, 10, 7, 1, 2, 5, 0, 3, -1, 11, 12, 1), addIndex = 4
        vystup: data = (1, 2, 10, 7, 4, 2, 5, 0, 3, -1, 11, 12, 1)

        vstup:  data = (3, 4, 10, 5, 5, 11, 15, 7, 8, 9, 10, 14,  8, 1, 2), addIndex = 12
        vystup: data = (3, 4,  8, 5, 5, 10, 15, 7, 8, 9, 10, 14, 11, 1, 2)

        vstup:  data = (3, 4, 10, 5), addIndex = 0
        vystup: data = (3, 4, 10, 5)
*/
    // size_t length = 15;
    // int data[] = {3, 4, 10, 5, 5, 11, 15, 7, 8, 9, 10, 14,  8, 1, 2};
    // printArr(data, length);
    // siftUp(data, 12);
    // printArr(data, length);

/* 2.
    PRIKLAD:
        vstup:  data = (7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6), length = 11
        vystup: data = (1, 2, 2, 2, 2, 5, 3, 7, 4, 8, 6)
*/
    // size_t length = 11;
    // int data[] = {7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6};
    // printArr(data, length);
    // buildHeapSiftUp(data, length);
    // printArr(data, length);

/* 3.
    PRIKLADY:
        vstup:  data = (55, 20, 10, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140), topIndex = 0, length = 15
        vystup: data = (10, 20, 50, 30, 40, 55, 60, 70, 80, 90, 100, 110, 120, 130, 140)

        vstup:  data = (100, 8, 2, 1, 0, 5, 6, 7, 4, 2, 3, 11, 12, 13, 14, 15, 16, 17), topIndex = 1, length = 18
        vystup: data = (100, 0, 2, 1, 2, 5, 6, 7, 4, 8, 3, 11, 12, 13, 14, 15, 16, 17)
*/
    // size_t length = 15;
    // int data[] = {55, 20, 10, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140};
    // printArr(data, length);
    // siftDown(data, 0, length);
    // printArr(data, length);

/* 4.
    PRIKLAD:
        vstup:  data = (7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6), length = 11
        vystup: data = (1, 2, 3, 2, 2, 5, 7, 4, 2, 8, 6)
*/
    // size_t length = 11;
    // int data[] = {7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6};
    // printArr(data, length);
    // buildHeapSiftDown(data, length);
    // printArr(data, length);

/* 5.
    PRIKLAD:
        vstup:  data = (7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6), length = 11
        vystup: data = (8, 7, 6, 5, 4, 3, 2, 2, 2, 2, 1)
*/
    // size_t length = 11;
    // int data[] = {7, 2, 1, 2, 8, 5, 3, 4, 2, 2, 6};
    // printArr(data, length);
    // heapSort(data, length);
    // printArr(data, length);

    return 0;
}
