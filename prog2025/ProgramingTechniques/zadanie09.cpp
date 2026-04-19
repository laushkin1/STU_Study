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
(8)  V prvych osmich prikladoch mozete pouzit iterativnu alebo rekurzivnu implementaciu. <- SPECIALNE INSTRUKCIE PRE TOTO ZADANIE
     V pripade implementacie rekurziou, moze byt hlavna cast implemetovana v pridanej rekurzivnej funkcii. <- SPECIALNE INSTRUKCIE PRE TOTO ZADANIE
(9)  Vase riesenie otestujte (vo funkcii 'main' a pomocou doplnenych pomocnych funkcii alebo datovych typov).
     Testovaci kod ale nebude hodnoteny.
(10) Funkcia 'main' musi byt v zdrojovom kode posledna.
*/


#include <iostream>
#include <list>
#include <vector>
#include <map>
#include <exception>
#include <set>

using namespace std;

//-------------------------------------------------------------------------------------------------
// DATOVE TYPY
//-------------------------------------------------------------------------------------------------

// Uzol binarneho vyhladavacieho stromu
struct Node {
    int value;     // hodnota uzla
    Node *smaller; // uzol 'smaller' a jeho nasledovnici maju hodnotu mensiu ako 'value'
    Node *greater; // uzol 'greater' a jeho nasledovnici maju hodnotu vacsiu ako 'value

    explicit Node(int value = 0, Node *smaller = nullptr, Node *greater = nullptr)
    : value(value)
    , smaller(smaller)
    , greater(greater)
    {
    }
};

// Binarny vyhladavaci strom
struct BinarySearchTree {
    Node *root; // koren stromu

    explicit BinarySearchTree(Node *root = nullptr)
    : root(root)
    {
    }
};

// Vynimka oznacujuca, ze uzol so zadanou hodnotou v strome neexistuje
class ValueNotExistsException : public std::exception {
};

//-------------------------------------------------------------------------------------------------
// 1. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati najmensiu hodnotu v strome.
    V pripade neexistencie tejto hodnoty vyhodi vynimku.

    Riesenie (tejto, aj dalsich uloh) musi vyuzivat vlastnosti binarneho vyhladavacieho stromu.
    Napriklad, pri hladani minima treba prejst len cez potrebne uzly. Netreba prehladavat vsetky uzly (okrem specialnych pripadov).

    VSTUPNY PARAMETER:
        [in] tree - strom, v ktorom funkcia hlada minimum

    NAVRATOVA HODNOTA:
        najmensia hodnota v strome

    VYNIMKA:
        ValueNotExistsException - ak je strom prazdny
*/

int min(const BinarySearchTree *tree) {
    if(tree == nullptr || tree->root == nullptr)
        throw ValueNotExistsException();
    Node *current = tree->root;
    while(current->smaller != nullptr)
        current = current->smaller;
    return current->value;
}

//-------------------------------------------------------------------------------------------------
// 2. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati hlbku uzla s hodnotou 'value' v strome 'tree'.
    Hlbka korena je 0.
    Ak hladany uzol v strome nie je, tak funkcia vyhodi vynimku.

    VSTUPNE PARAMETRE:
        [in] tree - strom, v ktorom funkcia hlada uzol
        [in] value - hodnota hladaneho uzla

    NAVRATOVA HODNOTA:
        Hlbka uzla s hodnotou 'value'.

    VYNIMKA:
        ValueNotExistsException - ak sa uzol s hodnotou 'value' v strome nenachdza
*/

size_t depth(const BinarySearchTree *tree, int value) {
    if(tree == nullptr || tree->root == nullptr)
        throw ValueNotExistsException();

    Node *current = tree->root;
    int deep = 0;
    while (current != nullptr){
        if(current->value > value) current = current->smaller;
        else if(current->value < value) current = current->greater;
        else return deep;
        deep++;
    }
    
    throw ValueNotExistsException();
}

//-------------------------------------------------------------------------------------------------
// 3. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati zoznam hodnot uzlov, ktore musi prejst pri hladani uzla s hodnotou 'value'.
    Prva hodnota vystupneho zoznamu je hodnota korena, druha hodnota zoznamu je lavy alebo pravy nasledovnik korena, atd.
    Poslednou hodnotou vo vystupnom zozname je hodnota uzla, na ktorom sa hladanie zastavilo.
    Ak 'tree' obsahuje uzol s hodnotou 'value', tak poslednou hodnotou vystupeho zoznamu je 'value'.

    VSTUPNE PARAMETRE:
        [in] tree - strom, v ktorom funkcia hlada uzol s hodnotou 'value'
        [in] value - hodnota hladaneho uzla

    NAVRATOVA HODNOTA:
        Zoznam hodnot uzlov, ktorych hodnoty musela funkcia pri hladani porovnavat.
        Poradie hodnot vo vystupnom zozname musi zodpovedat poradiu prechadzania uzlov stromu.

    PRIKLADY:
        V prikladoch predpokladajme strom:
                  40
                 /  \
               20    50
              /  \    \
            10   30    60

        1. priklad
            value: 30 -> vystup: (40, 20, 30)
        2. priklad
            value: 20 -> vystup: (40, 20)
        3. priklad
            value: 45 -> vystup: (40, 50)
*/

list<int> path(const BinarySearchTree *tree, int value) noexcept {
    Node *current = tree->root;
    list<int> my_list;
    while (current != nullptr){
        my_list.push_back(current->value);
        if(current->value > value) current = current->smaller;
        else if(current->value < value) current = current->greater;
        else break;
    }
    
    return my_list;
}

//-------------------------------------------------------------------------------------------------
// 4. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati pocet uzlov stromu.

    VSTUPNY PARAMETER:
        [in] tree - strom, ktoreho pocet uzlov funkcia zistuje

    NAVRATOVA HODNOTA:
        pocet uzlov stromu
*/
size_t countNodes(Node *n){
    if(n == nullptr) return 0;
    return 1 + countNodes(n->greater) + countNodes(n->smaller);
}

size_t count(const BinarySearchTree *tree) noexcept {
    if(tree == nullptr || tree->root == nullptr)
        return 0;
    return countNodes(tree->root);
}

//-------------------------------------------------------------------------------------------------
// 5. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati zoznam hodnot vsetkych uzlov v strome 'tree'.
    Zoznam musi byt usporiadany od najmensieho prvku po najvacsi.

    Usporiadanie dosiahnite vhodnou postupnostou prechadzania uzlov stromu!

    VSTUPNY PARAMETER:
        [in] tree - strom, ktoreho hodnoty funkcia vrati

    NAVRATOVA HODNOTA:
        hodnoty uzlov, v poradi od najmensej po najvacsiu
*/

void inorder(const Node *node, list<int> &out) {
    if (node == nullptr) return;

    inorder(node->smaller, out);
    out.push_back(node->value);
    inorder(node->greater, out);
}


list<int> all(const BinarySearchTree *tree) noexcept {
    list<int> result;
    if(tree == nullptr || tree->root == nullptr)
        return result;
    
    inorder(tree->root, result);
    return result;
}

//-------------------------------------------------------------------------------------------------
// 6. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati pocet uzlov stromu 'tree', ktorych hodnota je vacsia ako 'value'.

    VSTUPNE PARAMETRE:
        [in] tree - strom, v ktorom funkcia pocita pocet uzlov splnajich podmienku
        [in] value - hodnota, podla ktorej sa vyberaju uzly

    NAVRATOVA HODNOTA:
        pocet uzlov s hodnotou vacsou ako 'value'
*/
size_t countGreaterNode(Node* n, int value){
    if(n == nullptr) return 0;
    if(n->value > value)
        return 1 + countGreaterNode(n->greater, value) + countGreaterNode(n->smaller, value);
    else
        return countGreaterNode(n->greater, value);

}

size_t countGreater(const BinarySearchTree *tree, int value) noexcept {
    if(tree == nullptr || tree->root == nullptr)
        return 0;
    return countGreaterNode(tree->root, value);
}

//-------------------------------------------------------------------------------------------------
// 7. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia odstrani vsetky uzly stromu a dealokuje ich.

    Na dealokaciu pouzite operator 'delete'!

    VSTUPNY PARAMETER:
        [in, out] tree - strom, ktoreho uzly funkcia odstrani

    VYSTUPNE PODMIENKY:
        vsetky uzly su dealokovane
        'tree->root' je nulovy smernik
*/

void clearNode(const Node *node) {
    if (node == nullptr) return;

    clearNode(node->smaller);
    clearNode(node->greater);
    delete node;
}

void clear(BinarySearchTree *tree) noexcept {
    if(tree == nullptr || tree->root == nullptr)
        return;
    clearNode(tree->root);
    tree->root = nullptr;
}

//-------------------------------------------------------------------------------------------------
// 8. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia hlada hodnotu 'value' vo vektore 'data'.
    Vrati pocet prvkov v 'data', ktore pri hladni porovnala s 'value'.
    Vektor 'data' obsahuje usporiadane hodnoty.
    Casova narocnost algoritmu je O(log(n)), kde n je pocet prvkov vektora.

    PARAMETRE:
        [in] data - hodnoty usporiadane od najmensieho prvku po najvacsi
        [in] value - hodnota hladana v 'data'

    NAVRATOVA HODNOTA:
        Pocet prvkov 'data', ktore funkcia pri hladani porovnala s 'value'.
        (bez ohladu na to, ci 'data' obsahuje hodnotu 'value').

    VSTUPNA PODMIENKA:
        Hodnoty v 'data' su usporiadane od najmensieho prvku po najvacsi.

    PRIKLADY:
        data = (100, 102, 104, 106, 108, 110, 112), value = 106 -> vystup: 1
        data = (100, 102, 104, 106, 108, 110, 112), value = 110 -> vystup: 2
        data = (100, 102, 104, 106, 108, 110, 112), value = 108 -> vystup: 3
        data = (100, 102, 104, 106, 108, 110), value = 104 -> vystup: 1 alebo 3
        data = (100, 102, 104, 106, 108, 110), value = 107 -> vystup: 3
        data = (100), value = 100 -> vystup: 1
        data = (100), value = 200 -> vystup: 1
        data = (), value = 100 -> vystup: 0
*/

size_t containsNode(const vector<int> & data, int low, int high, int value){
    if (low > high) return 0;
    int middle = low + (high-low)/2;
    if(data[middle] == value) return 1;
    if(data[middle] > value)
        return 1 + containsNode(data, low, middle-1, value);
    if(data[middle] < value)
        return 1 + containsNode(data, middle+1, high, value);
    return 0;
}

size_t contains(const vector<int> & data, int value) noexcept {
    if(data.empty()) return 0;
    return containsNode(data, 0, data.size()-1, value);
}

//-------------------------------------------------------------------------------------------------
// 9. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori a vrati histogram slov v 'data' (zisti, kolko krat sa ktore slovo nachadza v 'data').

    PARAMETER:
        [in] data - vektor slov

    NAVRATOVA HODNOTA:
        histogram slov (pre kazde slovo z 'data' obsahuje pocet jeho vyskytov)

    VSTUPNE PODMIENKY:
        'data' moze obsahovat lubobolny pocet slov
        'data' moze obsahovat opakujuce sa slova
        slova mozu byt prazdne retazce

    VYSTUPNE PODMIENKY:
        Histogram obsahuje iba slova, ktore sa v 'data' nachadzaju

    PRIKLADY:
        vstup: data = ("pocitac", "lietadlo", luk", "pocitac", "pocitac", "okno", "luk")
        vystup: {"pocitac" -> 3, "lietadlo" -> 1, "luk" -> 2, "okno" -> 1}

        vstup: data = ("pocitac", "")
        vystup: {"pocitac" -> 1, "" -> 1}

        vstup: data = ()
        vystup: {}
*/

map<string, size_t> histogram(const vector<string> & data) noexcept {
    map<string, size_t> result;
    for(string word : data)
        result[word]++;
    return result;
}

//-------------------------------------------------------------------------------------------------
// 10. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vytvori a vrati index slov v 'data' (pre kazde slovo mnozinu indexov jeho vyskytu).

    PARAMETER:
        [in] data - vektor slov

    NAVRATOVA HODNOTA:
        index slov (pre kazde slovo z 'data' obsahuje indexy, na ktorych sa slovo v 'data' nachadza)

    VSTUPNE PODMIENKY:
        'data' moze obsahovat lubobolny pocet slov
        'data' moze obsahovat opakujuce sa slova
        slova mozu byt prazdne retazce

    VYSTUPNE PODMIENKY:
        Index obsahuje zoznamy vyskytov len pre slova, ktore sa nachadzaju v 'data'

    PRIKLADY:
        vstup: data = ("pocitac", "lietadlo", luk", "pocitac", "pocitac", "okno", "luk")
        vystup: {"pocitac" -> {0, 3, 4}, "lietadlo" -> {1}, "luk" -> {2, 6}, "okno" -> {5}}

        vstup: data = ("pocitac", "")
        vystup: {"pocitac" -> {0}, "" -> {1}}

        vstup: data = ()
        vystup: {}
*/

map<string, set<size_t>> index(const vector<string> & data) noexcept {
    map<string, set<size_t>> result;
    int indx = 0;
    for(string word : data){
        result[word].insert(indx);
        indx++;
    }
    return result;
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

// tu mozete doplnit pomocne funkcie a struktury

int main() {

    // Nepouzivam ziadne extra kniznice/func, iba cout/cin a struktury z ulohy.
    using std::cout;
    using std::endl;

    //----------------------------------------------------------------------------
    // Priprava stromu z prikladu v zadani:
    //           40
    //          /  \
    //        20    50
    //       /  \    \ 
    //     10   30    60
    //----------------------------------------------------------------------------
    BinarySearchTree tree;
    Node *n40 = new Node(40);
    Node *n20 = new Node(20);
    Node *n50 = new Node(50);
    Node *n10 = new Node(10);
    Node *n30 = new Node(30);
    Node *n60 = new Node(60);

    n40->smaller = n20;
    n40->greater = n50;
    n20->smaller = n10;
    n20->greater = n30;
    n50->greater = n60;
    tree.root = n40;

    BinarySearchTree emptyTree;

    //----------------------------------------------------------------------------
    // TEST 1: min()
    //----------------------------------------------------------------------------
    cout << "=== TEST 1: min() ===" << endl;

    // 1a) prazdny strom -> vynimka
    {
        bool ok = false;
        try {
            (void)min(&emptyTree); // nemalo by sa sem dostat
        }
        catch (const ValueNotExistsException &) {
            ok = true;
        }
        cout << (ok ? "✅ " : "❌ ")
             << "min(emptyTree) ma vyhodit ValueNotExistsException" << endl;
    }

    // 1b) strom z prikladu
    {
        bool ok = false;
        try {
            int m = min(&tree);
            ok = (m == 10);
            cout << (ok ? "✅ " : "❌ ")
                 << "min(tree): ocakavane 10, dostal " << m << endl;
        }
        catch (...) {
            cout << "❌ min(tree) vyhodilo vynimku" << endl;
        }
    }

    //----------------------------------------------------------------------------
    // TEST 2: depth()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 2: depth() ===" << endl;

    {
        bool ok = true;

        try {
            size_t d40 = depth(&tree, 40);
            bool t1 = (d40 == 0);
            cout << (t1 ? "✅ " : "❌ ")
                 << "depth(tree, 40): ocakavane 0, dostal " << d40 << endl;
            ok = ok && t1;

            size_t d20 = depth(&tree, 20);
            bool t2 = (d20 == 1);
            cout << (t2 ? "✅ " : "❌ ")
                 << "depth(tree, 20): ocakavane 1, dostal " << d20 << endl;
            ok = ok && t2;

            size_t d60 = depth(&tree, 60);
            bool t3 = (d60 == 2);
            cout << (t3 ? "✅ " : "❌ ")
                 << "depth(tree, 60): ocakavane 2, dostal " << d60 << endl;
            ok = ok && t3;
        }
        catch (...) {
            cout << "❌ depth(tree, X) vyhodilo vynimku pre existujuci uzol" << endl;
            ok = false;
        }

        // neexistujuca hodnota
        bool okExc = false;
        try {
            (void)depth(&tree, 99);
        }
        catch (const ValueNotExistsException &) {
            okExc = true;
        }
        cout << (okExc ? "✅ " : "❌ ")
             << "depth(tree, 99) ma vyhodit ValueNotExistsException" << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 3: path()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 3: path() ===" << endl;

    {
        list<int> p30 = path(&tree, 30);
        list<int> exp30 = {40, 20, 30};
        bool t1 = (p30 == exp30);
        cout << (t1 ? "✅ " : "❌ ")
             << "path(tree, 30) == {40, 20, 30}" << endl;

        list<int> p20 = path(&tree, 20);
        list<int> exp20 = {40, 20};
        bool t2 = (p20 == exp20);
        cout << (t2 ? "✅ " : "❌ ")
             << "path(tree, 20) == {40, 20}" << endl;

        list<int> p45 = path(&tree, 45);
        list<int> exp45 = {40, 50};
        bool t3 = (p45 == exp45);
        cout << (t3 ? "✅ " : "❌ ")
             << "path(tree, 45) == {40, 50}" << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 4: count()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 4: count() ===" << endl;

    {
        size_t cTree = count(&tree);
        bool t1 = (cTree == 6);
        cout << (t1 ? "✅ " : "❌ ")
             << "count(tree): ocakavane 6, dostal " << cTree << endl;

        size_t cEmpty = count(&emptyTree);
        bool t2 = (cEmpty == 0);
        cout << (t2 ? "✅ " : "❌ ")
             << "count(emptyTree): ocakavane 0, dostal " << cEmpty << endl;

        BinarySearchTree oneNodeTree(new Node(5));
        size_t cOne = count(&oneNodeTree);
        bool t3 = (cOne == 1);
        cout << (t3 ? "✅ " : "❌ ")
             << "count(oneNodeTree): ocakavane 1, dostal " << cOne << endl;

        // uvolnenie jednoprvkoveho stromu pre istotu
        clear(&oneNodeTree);
    }

    //----------------------------------------------------------------------------
    // TEST 5: all()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 5: all() ===" << endl;

    {
        list<int> aTree = all(&tree);
        list<int> expAll = {10, 20, 30, 40, 50, 60};
        bool t1 = (aTree == expAll);
        cout << (t1 ? "✅ " : "❌ ")
             << "all(tree) == {10, 20, 30, 40, 50, 60}" << endl;

        list<int> aEmpty = all(&emptyTree);
        bool t2 = aEmpty.empty();
        cout << (t2 ? "✅ " : "❌ ")
             << "all(emptyTree) je prazdny" << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 6: countGreater()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 6: countGreater() ===" << endl;

    {
        size_t cg45 = countGreater(&tree, 45); // 50,60
        bool t1 = (cg45 == 2);
        cout << (t1 ? "✅ " : "❌ ")
             << "countGreater(tree, 45): ocakavane 2, dostal " << cg45 << endl;

        size_t cg20 = countGreater(&tree, 20); // 30,40,50,60
        bool t2 = (cg20 == 4);
        cout << (t2 ? "✅ " : "❌ ")
             << "countGreater(tree, 20): ocakavane 4, dostal " << cg20 << endl;

        size_t cg60 = countGreater(&tree, 60); // nic
        bool t3 = (cg60 == 0);
        cout << (t3 ? "✅ " : "❌ ")
             << "countGreater(tree, 60): ocakavane 0, dostal " << cg60 << endl;

        size_t cgEmpty = countGreater(&emptyTree, 10);
        bool t4 = (cgEmpty == 0);
        cout << (t4 ? "✅ " : "❌ ")
             << "countGreater(emptyTree, 10): ocakavane 0, dostal " << cgEmpty << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 7: clear()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 7: clear() ===" << endl;

    {
        clear(&tree);
        bool t1 = (tree.root == nullptr);
        cout << (t1 ? "✅ " : "❌ ")
             << "Po clear(&tree): tree.root == nullptr" << endl;

        // Opakovane clear na uz prazdny strom (ak sa program nezrutil, berieme ako OK)
        clear(&tree);
        cout << "✅ Opakovane clear(&tree) na prazdny strom (program pokracuje)" << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 8: contains()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 8: contains() ===" << endl;

    {
        vector<int> v1 = {100, 102, 104, 106, 108, 110, 112};

        size_t cc106 = contains(v1, 106);
        bool t1 = (cc106 == 1);
        cout << (t1 ? "✅ " : "❌ ")
             << "contains((100..112), 106): ocakavane 1, dostal " << cc106 << endl;

        size_t cc110 = contains(v1, 110);
        bool t2 = (cc110 == 2);
        cout << (t2 ? "✅ " : "❌ ")
             << "contains((100..112), 110): ocakavane 2, dostal " << cc110 << endl;

        size_t cc108 = contains(v1, 108);
        bool t3 = (cc108 == 3);
        cout << (t3 ? "✅ " : "❌ ")
             << "contains((100..112), 108): ocakavane 3, dostal " << cc108 << endl;

        vector<int> v2 = {100, 102, 104, 106, 108, 110};
        size_t cc107 = contains(v2, 107);
        bool t4 = (cc107 == 3);
        cout << (t4 ? "✅ " : "❌ ")
             << "contains((100..110), 107): ocakavane 3, dostal " << cc107 << endl;

        vector<int> vSingle = {100};
        size_t cc100 = contains(vSingle, 100);
        bool t5 = (cc100 == 1);
        cout << (t5 ? "✅ " : "❌ ")
             << "contains({100}, 100): ocakavane 1, dostal " << cc100 << endl;

        size_t cc200 = contains(vSingle, 200);
        bool t6 = (cc200 == 1);
        cout << (t6 ? "✅ " : "❌ ")
             << "contains({100}, 200): ocakavane 1, dostal " << cc200 << endl;

        vector<int> vEmpty;
        size_t ccEmpty = contains(vEmpty, 100);
        bool t7 = (ccEmpty == 0);
        cout << (t7 ? "✅ " : "❌ ")
             << "contains({}, 100): ocakavane 0, dostal " << ccEmpty << endl;

        // specialny priklad: pre parny pocet prvkov moze byt 1 alebo 3
        size_t cc104 = contains(v2, 104);
        bool t8 = (cc104 == 1 || cc104 == 3);
        cout << (t8 ? "✅ " : "❌ ")
             << "contains((100..110), 104): ocakavane 1 alebo 3, dostal " << cc104 << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 9: histogram()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 9: histogram() ===" << endl;

    {
        vector<string> words1 = {"pocitac", "lietadlo", "luk", "pocitac", "pocitac", "okno", "luk"};
        map<string, size_t> h1 = histogram(words1);

        bool tSize = (h1.size() == 4);
        cout << (tSize ? "✅ " : "❌ ")
             << "histogram(words1) ma 4 rozne slova" << endl;

        bool tPoc = (h1["pocitac"] == 3);
        cout << (tPoc ? "✅ " : "❌ ")
             << "histogram(words1)[\"pocitac\"]: ocakavane 3, dostal " << h1["pocitac"] << endl;

        bool tLiet = (h1["lietadlo"] == 1);
        cout << (tLiet ? "✅ " : "❌ ")
             << "histogram(words1)[\"lietadlo\"]: ocakavane 1, dostal " << h1["lietadlo"] << endl;

        bool tLuk = (h1["luk"] == 2);
        cout << (tLuk ? "✅ " : "❌ ")
             << "histogram(words1)[\"luk\"]: ocakavane 2, dostal " << h1["luk"] << endl;

        bool tOkno = (h1["okno"] == 1);
        cout << (tOkno ? "✅ " : "❌ ")
             << "histogram(words1)[\"okno\"]: ocakavane 1, dostal " << h1["okno"] << endl;

        vector<string> words2 = {"pocitac", ""};
        map<string, size_t> h2 = histogram(words2);
        bool tP2 = (h2["pocitac"] == 1);
        cout << (tP2 ? "✅ " : "❌ ")
             << "histogram(words2)[\"pocitac\"]: ocakavane 1, dostal " << h2["pocitac"] << endl;

        bool tEmptyStr = (h2[""] == 1);
        cout << (tEmptyStr ? "✅ " : "❌ ")
             << "histogram(words2)[\"\"]: ocakavane 1, dostal " << h2[""] << endl;

        vector<string> wordsEmpty;
        map<string, size_t> hEmpty = histogram(wordsEmpty);
        bool tEmptyMap = hEmpty.empty();
        cout << (tEmptyMap ? "✅ " : "❌ ")
             << "histogram({}) je prazdna mapa" << endl;
    }

    //----------------------------------------------------------------------------
    // TEST 10: index()
    //----------------------------------------------------------------------------
    cout << "\n=== TEST 10: index() ===" << endl;

    {
        vector<string> words1 = {"pocitac", "lietadlo", "luk", "pocitac", "pocitac", "okno", "luk"};
        map<string, set<size_t>> idx1 = index(words1);

        map<string, set<size_t>> idx1exp;
        idx1exp["pocitac"] = set<size_t>({0, 3, 4});
        idx1exp["lietadlo"] = set<size_t>({1});
        idx1exp["luk"]      = set<size_t>({2, 6});
        idx1exp["okno"]     = set<size_t>({5});

        bool tSize = (idx1.size() == idx1exp.size());
        cout << (tSize ? "✅ " : "❌ ")
             << "index(words1) ma 4 rozne slova" << endl;

        bool tPoc = (idx1["pocitac"] == idx1exp["pocitac"]);
        cout << (tPoc ? "✅ " : "❌ ")
             << "index(words1)[\"pocitac\"] == {0,3,4}" << endl;

        bool tLiet = (idx1["lietadlo"] == idx1exp["lietadlo"]);
        cout << (tLiet ? "✅ " : "❌ ")
             << "index(words1)[\"lietadlo\"] == {1}" << endl;

        bool tLuk = (idx1["luk"] == idx1exp["luk"]);
        cout << (tLuk ? "✅ " : "❌ ")
             << "index(words1)[\"luk\"] == {2,6}" << endl;

        bool tOkno = (idx1["okno"] == idx1exp["okno"]);
        cout << (tOkno ? "✅ " : "❌ ")
             << "index(words1)[\"okno\"] == {5}" << endl;

        vector<string> words2 = {"pocitac", ""};
        map<string, set<size_t>> idx2 = index(words2);
        bool tP2 = (idx2["pocitac"] == set<size_t>({0}));
        cout << (tP2 ? "✅ " : "❌ ")
             << "index(words2)[\"pocitac\"] == {0}" << endl;

        bool tEmptyStr = (idx2[""] == set<size_t>({1}));
        cout << (tEmptyStr ? "✅ " : "❌ ")
             << "index(words2)[\"\"] == {1}" << endl;

        vector<string> wordsEmpty;
        map<string, set<size_t>> idxEmpty = index(wordsEmpty);
        bool tEmptyMap = idxEmpty.empty();
        cout << (tEmptyMap ? "✅ " : "❌ ")
             << "index({}) je prazdna mapa" << endl;
    }

    cout << "\n=== KONIEC TESTOV ===" << endl;
    return 0;
}

