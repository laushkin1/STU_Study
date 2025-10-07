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
// DATOVE TYPY
//-------------------------------------------------------------------------------------------------

// Reprezentacia uzla zretazeneho zoznamu
struct Node {
    int data; // hodnota uzla
    Node *next; // adresa nasledujuceho uzla zoznamu
};

// Reprezentacia zretazeneho zoznamu
struct List {
    Node *first; // adresa prveho uzla zoznamu
};

// Pomocna struktura pre ulohu 2
struct ListData {
    int *data; // pole hodnot uzlov
    size_t len; // dlzka pola 'data'
};

//-------------------------------------------------------------------------------------------------
// 1. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia prida novy uzol s hodnotou 'val' na koniec zretazeneho zoznamu 'list'.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list' vzdy existuje (t.j. 'list' je rozny od 'nullptr') a moze
           mat lubovolny pocet uzlov (0 a viac).

    PARAMETRE:
        [in, out] list - adresa zretazeneho zoznamu, do ktoreho ideme pridavat uzol
        [in] val       - hodnota uzla, ktory sa ma pridat na koniec zoznamu 'list'

    NAVRATOVA HODNOTA:
        Ziadna.

    PRIKLADY:
        prazdny zoznam, 5 ---> (5)
        (1), 1 ---> (1, 1)
        (8, 2, 6, 6), 14 ---> (8, 2, 6, 6, 14)
        (6, 100, 33), -85 ---> (6, 100, 33, -85)
        (1, 2, 3, 4, 5), 3 ---> (1, 2, 3, 4, 5, 3)
*/

void appendNode(List *list, const int val) {
    Node *last_el = new Node;
    last_el->data = val;
    last_el->next = nullptr;
    if(list->first == nullptr){
        list->first = last_el;
        return;
    }

    Node *el = list->first;
    while(el->next != nullptr) el = el->next;
    el->next = last_el;
}

//-------------------------------------------------------------------------------------------------
// 2. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori novy zretazeny zoznam, ktory bude skonstruovany pomocou udajov vo vstupnej
    strukture 'listData'. Struktura 'listData' obsahuje polozku 'data', ktora reprezentuje pole
    hodnot pre jednotlive uzly zoznamu. Struktura 'listData' taktiez obsahuje polozku 'len', ktora
    hovori, aku ma dlzku pole 'data'. Vytvoreny zoznam musi obsahovat uzly, ktore zodpovedaju
    svojimi hodnotami a poradim prvkom ulozenym v poli 'data' (dlzka zoznamu bude 'len').
    Funkcia nakoniec vrati adresu takto vytvoreneho zoznamu.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'listData' vzdy existuje (t.j. 'listData' je rozny od 'nullptr').
        2) Pre polozku 'len' plati, ze a nikdy nebude mat vacsiu hodnotu ako je realny pocet prvkov v poli 'data'.

    VYSTUPNA PODMIENKA:
        1) Ak je polozka 'len' rovna 0, vtedy sa vytvori a vrati prazdny zretazeny zoznam
           (t.j. platny zoznam, ktory ma 0 uzlov).

    PARAMETRE:
        [in] listData - struktura, pomocou ktorej sa vytvori zoznam

    NAVRATOVA HODNOTA:
        Adresa zretazeneho zoznamu, ktory bol vytvoreny pomocou struktury 'listData'.
        Vid pokyny vyssie.

    PRIKLADY:
        polozka 'data' = (1,2,3)
        polozka 'len' = 0
        vystupny zoznam bude mat 0 uzlov, t.j. ()

        polozka 'data' = (1,2,3)
        polozka 'len' = 2
        vystupny zoznam bude (1,2)

        polozka 'data' = (1,2,3)
        polozka 'len' = 3
        vystupny zoznam bude (1,2,3)

        polozka 'data' = (-9,412,64,72,100,100,6)
        polozka 'len' = 4
        vystupny zoznam bude (-9,412,64,72)
*/

List *createList(const ListData *listData) {
    List *myList = new List;
    myList->first = nullptr;
    if(listData->len <= 0) return myList;

    Node *el;
    Node *temp;
    Node *head;

    el = new Node;
    el->data = listData->data[0];
    temp = el;
    head = el;

    int indx = 1;
    int mylen = (listData->len)-1;

    while(indx <= mylen){ 
        el = new Node;
        el->data = listData->data[indx];
        temp->next = el;
        temp = temp->next;
        indx++;
    }
    temp->next = nullptr;
    

    // List *myList = new List;
    myList->first = head;

    return myList;
}

//-------------------------------------------------------------------------------------------------
// 3. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia prida novy uzol s hodnotou 'val' do vzostupne zotriedeneho zoznamu 'sortedList'.
    Pridanie musi prebehnut takym sposobom, aby bol zoznam 'sortedList' po pridani noveho uzla
    stale vzostupne zotriedeny.

    POZNAMKA: Na poradi duplikatov v zozname 'sortedList' nezalezi.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'sortedList' vzdy existuje (t.j. 'sortedList' je rozny od 'nullptr').
        2) Vstupny zoznam 'sortedList' moze mat lubovolny pocet uzlov (0 a viac).
        3) Vstupny zoznam 'sortedList' je vzostupne zotriedeny.

    PARAMETRE:
        [in, out] sortedList   - adresa vstupneho zretazeneho zoznamu, ktory je zotriedeny
                                 vzostupne (z angl. ascending)
        [in] val               - hodnota uzla, ktory sa ma pridat do zoznamu 'sortedList'

    NAVRATOVA HODNOTA:
        Ziadna.

    PRIKLADY:
        (), 0 ---> (0}
        (), 156 ---> (156)
        (8), 4 ---> (4, 8)
        (7), 10 ---> (7, 10)
        (55), 55 ---> (55, 55) ... poznamka: na poradi duplikatov nezalezi
        (1, 8, 236, 888), 70 ---> (1, 8, 70, 236, 888)
        (5, 5, 14, 82, 82, 100), 5 ---> (5, 5, 5, 14, 82, 82, 100)
*/

void insertNode(List *sortedList, const int val) {
    if(sortedList->first == nullptr || val <= sortedList->first->data){
        Node *current = new Node;
        current->data = val;
        current->next = sortedList->first;
        sortedList->first = current;
        return;
    }

    Node *el = sortedList->first;
    while (el->next != nullptr && el->next->data < val) el = el->next;

    Node *current = new Node;
    current->data = val;
    current->next = el->next;
    el->next = current;

}

//-------------------------------------------------------------------------------------------------
// 4. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori novy zoznam spojenim zoznamov 'list1' a 'list2'. Funkcia vrati adresu
    novo vytvoreneho zoznamu. V spojenom zozname idu najprv uzly zoznamu 'list1' (v nezmenenom poradi)
    a potom nasleduju uzly zoznamu 'list2' (v nezmenenom poradi). Pri vytvarani noveho zoznamu mozete
    pouzit hlboku alebo plytku kopiu existujucich uzlov (obe riesenia budu akceptovane).

    POZNAMKA:
        Prazdny zoznam je taky zoznam, ktory je platny, ale obsahuje 0 uzlov
        (t.j. polozka 'first' je nastavena na 'nullptr').

    VSTUPNE PODMIENKY:
        1) Zoznamy 'list1' a 'list2' mozu byt lubovolne dlhe (0 a viac uzlov).

    PARAMETRE:
        [in, out] list1 - adresa prveho zoznamu
        [in, out] list2 - adresa druheho zoznamu

    NAVRATOVA HODNOTA:
        Adresa zoznamu, ktory vznikol spojenim zoznamov 'list1' a 'list2'.

    PRIKLADY:
        () + () ---> ()
        (5, 9) + () ---> (5, 9)
        () + (2, 2, 2) ---> (2, 2, 2)
        (1) + (2) ---> (1, 2)
        (7) + (3, 9, 16) ---> (7, 3, 9, 16)
        (50, -47, 23, 1) + (0, 0, 9, -8) ---> (50, -47, 23, 1, 0, 0, 9, -8)
*/

List *joinLists(List *list1, List *list2) {
    if(list1->first == nullptr && list2->first == nullptr) return list1;
    if(list1->first == nullptr) return list2; 

    Node *el = list1->first;
    while(el->next != nullptr) el = el->next;
    el->next = list2->first;
    return list1;
}

//-------------------------------------------------------------------------------------------------
// 5. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia odstrani posledny uzol zo zoznamu 'list'. Ak je zoznam 'list' prazdny, funkcia nic
    nevykona. Pamat uzla, ktory funkcia vymaze musi byt korektne dealokovana.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list' vzdy existuje (t.j. 'list' je rozny od 'nullptr').
        2) Zoznam 'list' moze byt lubovolne dlhy (0 a viac uzlov).

    PARAMETRE:
        [in, out] list - adresa vstupneho zoznamu

    NAVRATOVA HODNOTA:
        Ziadna.

    PRIKLADY:
        () ---> ()
        (1) ---> ()
        (1, 2) ---> (1)
        (1, 2, 3) ---> (1, 2)
*/

void removeLastNode(List *list) {
    if(list->first == nullptr) return;
    if(list->first->next == nullptr){
        delete list->first;
        list->first = nullptr;
        return;
    };

    
    Node *prev = list->first;
    Node *el = list->first->next;
    while(el->next != nullptr){
        prev = el;
        el = el->next;
    }
    delete el;
    prev->next = nullptr;
}

//-------------------------------------------------------------------------------------------------
// 6. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia overi, ci je vstupny zoznam 'list' palindrom. Prazdny zoznam budeme povazovat za
    palindrom.

    Poznamka: Palindrom predstavuje taku postupnost hodnot, ktora sa cita rovnako v oboch smeroch.
    URL: https://sk.wikipedia.org/wiki/Palindr%C3%B3m
    Priklad palindromu: 1212121 alebo 4953594

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list' vzdy existuje (t.j. 'list' je rozny od 'nullptr').
        2) Zoznam 'list' moze byt lubovolne dlhy (0 a viac uzlov).

    PARAMETRE:
        [in] list - adresa vstupneho zoznamu

    NAVRATOVA HODNOTA:
        Pravdivostna hodnota 'true'/'false' podla toho, ci vstupny zoznam je/nie je palindrom.

    PRIKLADY:
        () ---> true
        (1) ---> true
        (2, 2, 2, 2) ---> true
        (1, 2, 3, 4, 5) ---> false
        (1, 2, 2, 1) ---> true
        (8, 4, 8, 4, 8) ---> true
        (9, 842, 613, 2, 613, 842, 9) ---> true
        (7, 9, 1, 2) ---> false
        (0, 0, 0, 7, 0, 0) ---> false
*/

bool isPalindrome(const List *list) {
    if(list->first == nullptr) return true;
    Node *slow = list->first;
    Node *fast = list->first;

    while(fast != nullptr && fast->next != nullptr){
        fast = fast->next->next;
        slow = slow->next;
    }

    if(fast != nullptr) slow = slow->next;

    Node* prev = nullptr;
    Node* cur = slow;
    while(cur != nullptr){
        Node* nxt = cur->next; // ?
        cur->next = prev; // ?
        prev = cur; // ?
        cur = nxt; // ?
    }
    Node* revHead = prev; // ?

    Node* l1 = list->first;
    Node* l2 = revHead;
    while(l2 != nullptr){
        if(l1->data != l2->data) return false;
        l1 = l1->next;
        l2 = l2->next;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
// 7. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati sucet hodnot prvych 'n' uzlov v zozname 'list'.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list' vzdy existuje (t.j. 'list' je rozny od 'nullptr').
        2) Zoznam 'list' moze byt lubovolne dlhy (0 a viac uzlov).
        3) Parameter 'n' nebude mat nikdy vyssiu hodnotu ako je realny pocet uzlov v zozname.

    PARAMETRE:
        [in] list - vstupny zretazeny zoznam
        [in] n - pocet uzlov od zaciatku zoznamu 'list', ktorych sucet sa vrati

    NAVRATOVA HODNOTA:
        Sucet hodnot prvych 'n' uzlov zoznamu 'list'. Ak 'n'=0, vtedy funkcia vrati hodnotu 0.

    PRIKLADY:
        (), 0 ---> 0
        (1,2,3), 0 ---> 0
        (6), 1 ---> 6
        (5,6,7), 2 ---> 11
        (4,4,4,4,4,4), 6 ---> 24
        (-8,41,-314,21), 3 ---> -281
*/

int sumNodes(const List *list, const size_t n) {
    if(list->first == nullptr) return 0;
    int sum = 0;
    Node *current = list->first;
    for(size_t i = 0; i<n; i++){
        sum = sum + current->data;
        current = current->next;
    }
    return sum;
}

//-------------------------------------------------------------------------------------------------
// 8. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia overi, ci zoznam 'list1' obsahuje vsetky hodnoty uzlov zoznamu 'list2'.
    Na poradi nezalezi.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list1' a 'list2' vzdy existuju (t.j. su rozne od 'nullptr').
        2) Zoznamy 'list1' a 'list2' mozu byt lubovolne dlhe (0 a viac uzlov).
        3) Zoznam 'list2' nebude obsahovat duplicitne uzly.

    PARAMETRE:
        [in] list1 - prvy zoznam
        [in] list2 - druhy zoznam

    NAVRATOVA HODNOTA:
        Pravdivostna hodnota 'true'/'false' podla toho, ci zoznam 'list1' obsahuje/neobsahuje
        vsetky hodnoty uzlov zoznamu 'list2'.

    PRIKLADY:
        (), () ---> true
        (), (1, 2, 3) ---> false
        (1, 2, 3), () ---> true
        (5, 6, 7), (5, 6, 8) ---> false
        (8, 4, 3), (3, 8, 4) ---> true
        (7, 6, 1, 9, 8), (1, 6, 8) ---> true
        (3, 3, 3, 3, 3), (2, 3) ---> false
        (8, 6, 1, 2, 5, 0, 7, 7), (7) ---> true
        (9, 5, 10), (41, 10, 5) ---> false
*/

bool contains(const List *list1, const List *list2) {
    if(list1->first == nullptr && list2->first == nullptr) return true;
    if(list1->first == nullptr) return false;
    if(list2->first == nullptr) return true;


    Node* list2_ptr = list2->first;
    while(list2_ptr != nullptr){
        Node* list1_ptr = list1->first;
        while (list1_ptr != nullptr){
            if(list1_ptr->data == list2_ptr->data) break;
            list1_ptr = list1_ptr->next;
        }
        if(list1_ptr == nullptr) return false;
        list2_ptr = list2_ptr->next;
    }
    return true;
}

//-------------------------------------------------------------------------------------------------
// 9. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori a vrati HLBOKU kopiu zoznamu 'list'. Vo vytvorenej kopii musia byt zachovane
    hodnoty a poradie uzlov z povodneho zoznamu.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list' vzdy existuje (t.j. je rozny od 'nullptr').
        2) Zoznam 'list' moze byt lubovolne dlhy (0 a viac uzlov).

    VYSTUPNA PODMIENKA
        Adresy uzlov na rovnakych poziciach v oboch zoznamoch sa musia lisit (pretoze je to hlboka kopia)

    PARAMETRE:
        [in] list - adresa zoznamu, ktoreho hlboku kopiu treba vytvorit

    NAVRATOVA HODNOTA:
        Adresa zoznamu, ktory vznikol ako hlboka kopia vstupneho zoznamu 'list'.
        Kopiou prazdneho zoznamu je prazdny zoznam.

    PRIKLADY:
        Vstup = ()
        Vystup = ()

        Vstup = (1, 2, 3)
        Vystup = (1, 2, 3)
*/

List *deepCopyList(const List *list) {
    List *myList = new List;
    myList->first = nullptr;
    if(list->first == nullptr) return myList;

    Node *el;
    Node *temp;
    Node *head;

    Node *old_el = list->first;

    el = new Node;
    el->data = old_el->data;
    temp = el;
    head = el;
    old_el = old_el->next;

    while(old_el != nullptr){ 
        el = new Node;
        el->data = old_el->data;
        temp->next = el;
        temp = temp->next;
        old_el = old_el->next;
    }
    temp->next = nullptr;
    

    myList->first = head;

    return myList;
}

//-------------------------------------------------------------------------------------------------
// 10. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia najde posledny vyskyt uzla s hodnotou 'val' v zozname 'list' a vrati jeho adresu.
    Ak sa v zozname 'list' takyto uzol nenachadza, funkcia vrati 'nullptr'.

    VSTUPNE PODMIENKY:
        1) Plati, ze 'list' vzdy existuje (t.j. je rozny od 'nullptr').
        2) Zoznam 'list' moze byt lubovolne dlhy (0 a viac uzlov).

    PARAMETRE:
        [in] list - vstupny zretazeny zoznam
        [in] val - hladana hodnota uzla

    NAVRATOVA HODNOTA:
        Adresa posledneho vyskytu uzla s hodnotou 'val' v zozname 'list'.

    PRIKLADY (treba otestovat pomocou debuggera):
        (), 2 ---> nullptr
        (5, 4, 3, 2), 1 ---> nullptr
        (-5, 4, -9, 66), 4 ---> adresa 2. uzla
        (1, 1, 1, 5, 6, 41), 1 ---> adresa 3. uzla
        (8, 6, 7, 3, 2, 6), 6 ---> adresa 6. uzla
*/

Node *findLastNodeOccurrence(const List *list, const int val) {
    if(list->first == nullptr) return nullptr;
    Node* el = list->first;
    Node* result = nullptr;
    
    while (el != nullptr) {
        if(el->data == val) result = el;
        el = el->next;
    }
    return result;
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

// tu mozete doplnit pomocne testovacie funkcie a struktury

void deleteList(List *myList){
    Node *current = myList->first;
    Node *prev_el;
    while(current->next != nullptr){ 
        prev_el = current;
        current = current->next;
        delete prev_el;
    }
}

void printList(const List *myList){
    Node *current = myList->first;
    while (current != nullptr) {
        cout << current->data << "->";
        current = current->next;
    }
    cout << "\n";
}
    

int main() {

    return 0;
}
