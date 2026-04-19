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
#include <list>
#include <vector>
#include <numeric>
#include <algorithm>
#include <initializer_list>

using namespace std;

//-------------------------------------------------------------------------------------------------
// 1. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati priemernu hodnotu prvkov vstupneho inicializacneho zoznamu.
    Ak inicializacny zoznam neobsahuje ziadne prvky, tak vyhodi vynimku typu 'std::invalid_argument',
    ktorej metoda 'what' vrati textovy retazec: "inicializacny zoznam je prazdny".

    Na vypocet sumy cisiel, v implementacii pouzite 'std::accumulate'.

    PARAMETER:
        [in] data - zoznam prvkov

    NAVRATOVA HODNOTA:
        Priemerna hodnota prvkov vstupneho inicializacneho zoznamu

    VYNIMKA:
        Ak je inicializacny zoznam prazdny, tak funkcia vyhodi vynimku typu 'std::invalid_argument',
        ktorej metoda 'what' vrati vysvetlujuci popis: "inicializacny zoznam je prazdny" (dodrzte presne tento textovy retazec! (ctrl+c, ctrl+v))
*/

double average(const initializer_list<int> & data) {
    if(data.size() <= 0) throw invalid_argument("inicializacny zoznam je prazdny");
    double sum = accumulate(data.begin(), data.end(), 0.0);
    return sum/(double)data.size();
}

//-------------------------------------------------------------------------------------------------
// 2. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati iterator na 1. prvok zoznamu 'data' obsahujuci hodnotu 'value'.
    Pozor na pripad, ked 'data' obsahuje viac prvkov s hodnotou 'value'.

    V implementacii pouzite 'std::find'.

    PARAMETRE:
        [in] data - zoznam, v ktorom funkcia hlada 1. vyskyt hodnoty 'value'
        [in] value - hodnota hladaneho prvku

    NAVRATOVA HODNOTA:
        Ak 'data' obsahuje prvok s hodnotou 'value', tak vrati konstantny iterator na 1. takyto prvok.
        Inak vrati iterator za posledny prvok 'data' (iterator 'data.cend()')
*/

list<int>::const_iterator findValue(const list<int> & data, int value) noexcept {
    return find(data.begin(), data.end(), value);
}

//-------------------------------------------------------------------------------------------------
// 3. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati iterator na 1. prvok, ktoreho hodnota je v rozsahu <200, 400> (vratane 200 a 400).

    V implementacii pouzite funkciu 'std::find_if'.
    Pri jej volani pouzite ako jeden z argumentov funkciu 'isInRange200to400'.

    PARAMETER:
        [in] data - zoznam, v ktorom sa hlada 1. vyskyt hodnoty v rozsahu <200, 400>

    NAVRATOVA HODNOTA:
        Ak 'data' obsahuje prvok/prvky v rozsahu <200,400>, tak vrati iterator na 1. takyto prvok.
        Inak vrati iterator za posledny prvok (iterator 'data.cend()').
*/

bool isInRange200to400(const int element) noexcept {
    return element >= 200 && element <= 400;
}

list<int>::const_iterator findInRange200to400(const list<int> & data) noexcept {
    return find_if(data.begin(), data.end(), isInRange200to400);
}

//-------------------------------------------------------------------------------------------------
// 4. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia v zozname 'data' nahradi vsetky hodnoty v rozsahu <200, 400> (vratane 200 a 400), hodnotou 5.

    V implementacii pouzite funkciu 'std::replace_if'.
    Pri jej volani pouzite ako jeden z argumentov funkciu 'isInRange200to400' (definovanu v zadani predchadzajucej ulohy).

    PARAMETER:
        [in, out] data - zoznam, v ktorom funkcia nahradi vsetky hodnoty v rozsahu <200, 400>, hodnotou 5
*/

void replace200to400by5(list<int> & data) noexcept {
    replace_if(data.begin(), data.end(), isInRange200to400, 5);
}

//-------------------------------------------------------------------------------------------------
// 5. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia zvysi hodnotu kazdeho prvku vstupneho zoznamu o 1 (ku kazdej hodnote pripocita 1).

    V implementacii pouzite funkciu 'std::transform'.
    Pri jej volani pouzite ako jeden z argumentov funkciu 'increment'.

    PARAMETER:
        [in, out] data - zoznam, v ktorom funkcia zvysi hodnotu kazdeho prvku o 1
*/

int increment(int element) noexcept {
    return element + 1;
}

void incrementAll(list<int> & data) noexcept {
    transform(data.begin(), data.end(), data.begin(), increment);
}

//-------------------------------------------------------------------------------------------------
// 6. ULOHA (0.4 bodu)
//----------------------------------------------------------------------------------------------
/*
    Vstupny zoznam obsahuje celkovy pocet bodov z predmetu, pre kazdeho studenta.
    Ak je pocet bodov teste pod hranicou na lepsiu znamku (do lepsej znamky chyba 1 bod),
    tak funkcia zvysi pocet bodov o 1.
    Inak funkcia nezmeni pocet bodov.

    V implementacii pouzite funkciu 'std::transform'.
    Pri jej volani pouzite ako jeden z argumentov  funkciu 'incrementPointsIfItHelps'.

    PARAMETER:
        [in, out] points - celkovy pocet bodov z predmetu pre kazdeho studenta
                           (niektore mozu byt po vykonani funkcie inkrementovane)

    PRIKLAD:
        vstup:  { 100, 90, 91, 92, 93, 53, 54, 55, 56, 57, 91, 92 }
        vystup: { 100, 90, 92, 92, 93, 53, 54, 56, 56, 57, 92, 92 }
                            ^                   ^           ^
*/

int incrementPointsIfItHelps(int points) noexcept {
    switch (points) {
        case 91 :
            return 92;
        case 82:
            return 83;
        case 73:
            return 74;
        case 64:
            return 65;
        case 55:
            return 56;
        default:
            return points;
    }
}

void helpAfterExam1(list<int> & points) noexcept {
    transform(points.begin(), points.end(), points.begin(), incrementPointsIfItHelps); 
}

//-------------------------------------------------------------------------------------------------
// 7. ULOHA (0.4 bodu)
//----------------------------------------------------------------------------------------------
/*
    Implementujte rovnaku funkcionalitu ako v predchadzajucej ulohe,
    ale namiesto 'std::transform' pouzite "range-based for" cyklus, v ktorom budete volat 'incrementPointsIfItHelps'.

    POZNAMKA:
    Aby obsah zoznamu mohol byt zmeneny, premenna v cykle nemoze obsahovat kopiu prvku zo zoznamu, ale ... (vyrieste).
*/

void helpAfterExam2(list<int> & points) noexcept {
    for(int point : points)
        replace(points.begin(), points.end(), point, incrementPointsIfItHelps(point));
}

//-------------------------------------------------------------------------------------------------
// 8. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati novy zoznam, v ktorom hodnota kazdeho prvku je dana suctom prvkov v 'data1' a 'data2' s rovnakou poziciou.

    V implementacii pouzite 'std::transform'.
    Pri jej volani pouzite ako jeden z argumentov 'std::plus<int>()'.

    PARAMETRE:
        [in] data1 - obsahuje scitance
        [in] data2 - obsahuje scitance

    NAVRATOVA HODNOTA:
        Zoznam, v ktorom hodnota kazdeho prvku je suctom prvkov v 'data1' a 'data2' s rovnakou poziciou.

    VSTUPNA PODMIENKA:
        'data1' a 'data2' maju rovnaky pocet prvkov

    VYSTUPNA PODMENKA:
        Nech 'output' je vystupny zoznam.
        Pocet prvkov 'output' je rovnaky ako pocet prvkov 'data1' a 'data2'.
        Pre kazde i take, ze i >= 0 && i < data1.size() plati: output[i] = data1[i] + data2[i].

    PRIKLAD:
        vstup: data1 = { 2, 1, 1, 4, 5 }
               data2 = { 1, 0, 4, 6, 2 }
        vystup:        { 3, 1, 5,10, 7 }
*/

list<int> add(const list<int> & data1, const list<int> & data2) noexcept {
    list<int> data(data1.size());
    transform(data1.begin(), data1.end(), data2.begin(), data.begin(), plus<int>());
    return data;
}

//-------------------------------------------------------------------------------------------------
// 9. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia usporiada prvky vektora od najvacsieho po najmensi.

    V implementacii pouzite 'std::sort' alebo 'std::stable_sort' s parametrom 'std::greater<int>()'.

    PARAMETER:
        [in, out] data - vektor, ktory funkcia usporiada
*/

void sort1(vector<int> & data) noexcept {
    sort(data.begin(), data.end(), greater<int>());
}

//-------------------------------------------------------------------------------------------------
// 10. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia usporiada prvky vektora od najvacsieho po najmensi.

    V implementacii pouzite 'std::sort' alebo 'std::stable_sort' s reverznymi iteratormi.

    PARAMETER:
        [in, out] data - vektor, ktory funkcia usporiada
*/

void sort2(vector<int> & data) noexcept {
    sort(data.rbegin(), data.rend());
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

// tu mozete doplnit pomocne testovacie funkcie a datove typy

int main() {

    // Test 1: average
    cout << "Test 1a (average non-empty) ";
    double avg = average({1, 2, 3, 4});
    if (avg == 2.5) {
        cout << "✅\n";
    } else {
        cout << "❌ (got " << avg << ")\n";
    }

    cout << "Test 1b (average throws on empty) ";
    bool threw = false;
    try {
        average({});
    } catch (...) {
        threw = true;
    }
    if (threw) {
        cout << "✅\n";
    } else {
        cout << "❌ (no exception)\n";
    }

    // Test 2: findValue
    cout << "Test 2a (findValue first occurrence) ";
    list<int> l2 = {5, 7, 7, 9};
    auto it2 = findValue(l2, 7);
    auto it2_expected = l2.cbegin();
    ++it2_expected; // points to first 7
    if (it2 == it2_expected && it2 != l2.cend() && *it2 == 7) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    cout << "Test 2b (findValue not found) ";
    auto it2b = findValue(l2, 42);
    if (it2b == l2.cend()) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 3: findInRange200to400
    cout << "Test 3a (findInRange200to400 first in range) ";
    list<int> l3 = {100, 199, 200, 250, 400, 401};
    auto it3 = findInRange200to400(l3);
    auto it3_expected = l3.cbegin();
    ++it3_expected; // 199
    ++it3_expected; // 200 (first in range)
    if (it3 == it3_expected && it3 != l3.cend() && *it3 == 200) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    cout << "Test 3b (findInRange200to400 none in range) ";
    list<int> l3b = {0, 199, 401};
    auto it3b = findInRange200to400(l3b);
    if (it3b == l3b.cend()) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 4: replace200to400by5
    cout << "Test 4 (replace200to400by5) ";
    list<int> l4 = {199, 200, 250, 400, 401};
    replace200to400by5(l4);
    list<int> l4_expected = {199, 5, 5, 5, 401};
    if (l4 == l4_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 5: incrementAll
    cout << "Test 5 (incrementAll) ";
    list<int> l5 = {0, -1, 10};
    incrementAll(l5);
    list<int> l5_expected = {1, 0, 11};
    if (l5 == l5_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 6: helpAfterExam1
    cout << "Test 6 (helpAfterExam1) ";
    list<int> p1 = {100, 90, 91, 92, 93, 53, 54, 55, 56, 57, 91, 92};
    helpAfterExam1(p1);
    list<int> p1_expected = {100, 90, 92, 92, 93, 53, 54, 56, 56, 57, 92, 92};
    if (p1 == p1_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 7: helpAfterExam2
    cout << "Test 7 (helpAfterExam2) ";
    list<int> p2 = {100, 90, 91, 92, 93, 53, 54, 55, 56, 57, 91, 92};
    helpAfterExam2(p2);
    list<int> p2_expected = {100, 90, 92, 92, 93, 53, 54, 56, 56, 57, 92, 92};
    if (p2 == p2_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 8: add
    cout << "Test 8 (add) ";
    list<int> a = {2, 1, 1, 4, 5};
    list<int> b = {1, 0, 4, 6, 2};
    list<int> sum = add(a, b);
    list<int> sum_expected = {3, 1, 5, 10, 7};
    if (sum == sum_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 9: sort1
    cout << "Test 9 (sort1) ";
    vector<int> v1 = {3, 1, 5, 2};
    sort1(v1);
    vector<int> v1_expected = {5, 3, 2, 1};
    if (v1 == v1_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    // Test 10: sort2
    cout << "Test 10 (sort2) ";
    vector<int> v2 = {4, 7, 1, 7, 0};
    sort2(v2);
    vector<int> v2_expected = {7, 7, 4, 1, 0};
    if (v2 == v2_expected) {
        cout << "✅\n";
    } else {
        cout << "❌\n";
    }

    return 0;
}

