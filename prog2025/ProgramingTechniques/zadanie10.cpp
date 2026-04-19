/*
Meno a priezvisko: Ivan Laushkin

POKYNY:
(1)  Implementujte funkcie tak, aby splnali popis pri ich deklaraciach.
(2)  Cela implementacia musi byt v tomto jednom subore.
(3)  Odovzdajte len tento zdrojovy subor (dopleny o riesenia).
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
#include <iomanip>
#include <cassert>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <deque>
#include <algorithm>
#include <limits>

#include <sstream>

using namespace std;

//-------------------------------------------------------------------------------------------------
// 1. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Upravte definiciu metody 'T::met()' tak, aby informovala, ze metoda nemeni stav objektu.

    Pre kontrolu odkomentujte riadok v testovacej funkcii. Odkomentovany riadok musi byt kompilovatelny.
*/

class T {
public:
    void met() const {}
};

void testUloha1() {
    cout << "----- 1. uloha (metoda nemeni stav objektu) ------------------------------------" << endl;
    const T o;
    o.met(); // odkomentujte
}

//-------------------------------------------------------------------------------------------------
// 2. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Upravte definiciiu konstruktora tak, aby bol konverznym konstruktorom.

    Zakomentovane riadky v testovacom kode musia byt kompilovatelne.
*/

class Number {
private:
    int number;
public:
    Number(int value) : number(value){
        this->number = value;
    }
    int getValue() const {
        return number;
    }
};

void fun(Number n) {
    cout << n.getValue() << endl;
}

void testUloha2() {
    cout << "----- 2. uloha (konverzny konstruktor) -----------------------------------------" << endl;

   Number a = 10; // ak existuje zodpovedajuci konverzny konstruktor, tak je tento riadok kompilovatelny
   cout << a.getValue() << endl;

   fun(20); // ak existuje zodpovedajuci konverzny konstruktor, tak je tento riadok kompilovatelny
}

//-------------------------------------------------------------------------------------------------
// 3. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Vytvorte triedu 'B', ktora je podtriedou triedy 'A'.
    Upravte definiciu metody 'A::met()' a definujte metodu 'B::met()' tak,
    aby bol testovaci kod po odkomentovani uspesne vykonany (aby bola vzdy volana spravna metoda).
*/

class A {
public:
    virtual char met() {
        return 'a';
    }
};

class B : public A {
public:
    char met() override {
        return 'b';
    }
};

void testUloha3() {
    cout << "----- 3. uloha (volanie spravnej metody) ---------------------------------------" << endl;

   A a;
   B b;

   A *pa  = &a;
   A *pab = &b;
   B* pb  = &b;

   assert(pa->met()  == 'a'); // volanie A::met()
   assert(pab->met() == 'b'); // volanie B::met()
   assert(pb->met()  == 'b'); // volanie B::met()
}

//-------------------------------------------------------------------------------------------------
// 4. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Upravte definicie destruktorov tried 'C' a 'D' tak, aby sa v testovacej funkcii zavolal spravny destruktor.
    Testovacia funkcia musi namiesto:
        C::~C()
    po uprave vypisat:
        D::~D()
        C::~C()
*/

class C {
public:
    virtual ~C() {
        cout << "C::~C()" << endl;
    }
};

class D : public C {
public:
    ~D() override {
        cout << "D::~D()" << endl;
    }
};

void testUloha4() {
    cout << "----- 4. uloha (volanie spravneho destruktora) ---------------------------------" << endl;
    C *obj = new D;
    delete obj; // aby sa zavolal spravny detruktor
}

//-------------------------------------------------------------------------------------------------
// 5. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Vytvorte operator +=, ktory bude sluzit pre pripocitanie druheho komplexneho cisla ku prvemu.
    Operator definujte ako clena triedy.

    Pre kontrolu odkomentujte riadky v testovacom kode, kde je priklad pouzitia operatora.
*/

class Complex {
private:
    int real;
    int imaginary;
public:
    Complex(int realPart, int imaginaryPart)
            : real(realPart)
            , imaginary(imaginaryPart) {
    }
    int getRealPart() const {
        return real;
    }
    int getImaginaryPart() const {
        return imaginary;
    }
    // TODO tu pridajte operator +=
    Complex& operator += (const Complex& other) {
        this->real += other.getRealPart();
        this->imaginary += other.getImaginaryPart();
        return *this;
    }
};

void testUloha5() {
    cout << "----- 5. uloha (operator +=) ---------------------------------------------------" << endl;

    Complex a(1,2);
    Complex b(10, 20);
    Complex c(100, 200);

    c += b += a;

   assert(a.getRealPart() == 1);
   assert(a.getImaginaryPart() == 2);
   assert(b.getRealPart() == 11);
   assert(b.getImaginaryPart() == 22);
   assert(c.getRealPart() == 111);
   assert(c.getImaginaryPart() == 222);
}

//-------------------------------------------------------------------------------------------------
// 6. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Vytvorte operator <<, ktory zapise hodnotu objektu triedy 'Complex' (z predchadzajuceho prikladu)
    do textoveho prudu dat.
    Poznamka: Operator musi byt definovany ako globalny.

    Pre kontrolu odkomentujte riadky v testovacom kode.
*/

// TODO tu definujte operator <<
ostream& operator<<(ostream& os, const Complex& c) {
    os << c.getRealPart();

    int img = c.getImaginaryPart();
    if (img >= 0)
        os << "+" << img << "i";
    else
        os << img << "i";      // img already contains '-' sign

    return os;
}

void testUloha6() {
    cout << "----- 6. uloha (operator << ) --------------------------------------------------" << endl;

    Complex a( 1, 2);
    Complex b(-3,-4);
   cout << "a = " << a << ", b = " << b << endl;

   ostringstream stream;
   stream << a << " " << b;
   assert(stream.str() == "1+2i -3-4i");
}

//=================================================================================================
// STROM - definicia pre dalsie ulohy
//=================================================================================================

// Uzol stromu
struct Node {
    char value; // hodnota uzla
    list<Node*> children; // zoznam nasledovnikov

    explicit Node(char value) : value(value) {}
};

// Strom
struct Tree {
    Node * root; // koren stromu

    explicit Tree(Node *root = nullptr) : root(root) {}
};

//-------------------------------------------------------------------------------------------------
// 7. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati zoznam vsetkych hodnot uzlov v strome 'tree', ktorych hodnotou je velke pismeno.
    Pouzite algoritmus hladania DO HLBKY.
    Pouzite iterativnu implementaciu.
    Doporucenie: pre identifikaciu velkych pismen pouzite funkciu 'std::isupper'

    VSTUPNA HODNOTA:
        [in] tree - prehladavany strom

    NAVRATOVA HODNOTA:
        zoznam velkych pismen, ktore su hodnotami uzlov v strome 'tree'

    VYSTUPNA PODMIENKA:
        Poradie vo vystupnom zozname musi zodpovedat postupnosti prehladavania algoritmom DO HLBKY.

    PRIKLAD:
        na obrazku
*/

list<char> depthFirstSearchUpperCases(const Tree *tree) {
    list<char> result;

    if (!tree || !tree->root)
        return result;

    stack<Node*> st;
    st.push(tree->root);

    while (!st.empty()) {
        Node* node = st.top();
        st.pop();

        if (std::isupper(static_cast<unsigned char>(node->value)))
            result.push_back(node->value);

        for (auto it = node->children.begin(); it != node->children.end(); ++it) {
            if (*it)
                st.push(*it);
        }
    }

    return result;
}






//-------------------------------------------------------------------------------------------------
// 8. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati zoznam vsetkych hodnot uzlov v strome 'tree', ktorych hodnotou je velke pismeno.
    Pouzite algoritmus hladania DO SIRKY.
    Pouzite iterativnu implementaciu.
    Doporucenie: pre identifikaciu velkych pismen pouzite funkciu 'std::isupper'

    VSTUPNA HODNOTA:
        [in] tree - prehladavany strom

    NAVRATOVA HODNOTA:
        zoznam velkych pismen, ktore su hodnotami uzlov v strome 'tree'

    VYSTUPNA PODMIENKA:
        Poradie vo vystupnom zozname musi zodpovedat postupnosti prehladavania algoritmom DO SIRKY.

    PRIKLAD:
        obrazok na webe
*/

list<char> breadthFirstSearchUpperCases(const Tree *tree) {
    list<char> result;
    if (tree == nullptr || tree->root == nullptr)
        return result;

    queue<Node*> q;
    q.push(tree->root);

    while (!q.empty()) {
        Node *cur = q.front();
        q.pop();

        if (std::isupper(static_cast<unsigned char>(cur->value)))
            result.push_back(cur->value);

        for (Node *child : cur->children)
            q.push(child);
    }

    return result;
}

//=================================================================================================
// GRAF - definicia pre dalsie ulohy
//=================================================================================================
// Graf reprezentuje cestnu siet medzi obcami na planete.
// Uzol grafu reprezentuje obec.
// Hrana grafu reprezentuje "priame" cestne spojenie z jednej obce do druhej.
// Graf je orientovany -> Hrana grafu reprezentuje moznost jazdy len jednym smerom.
//  Preto je moznost "priamej" jazdy medzi obcami oboma smermy reprezentovana dvoma hranami grafu.
// Graf je ohodnoteny -> Mapa cestnej siete eviduje dlzky "priamych" cestnych spojeni medzi obcami.
// Obce nemusia byt prepojene cestnou sietou, napriklad ak sa nachadzaju na roznych pevninach.

// Forward deklaracie su potrebne, pretoze:
//  - definicie 'City' a 'RoadTo' su cyklicky zavisle:
//      - v definicii 'City' je pouzite 'RoadTo'
//      - a v definicii 'RoadTo' je pouzite 'City'
//  - definicie 'City' a 'SearchData' su cyklicky zavisle:
//      - v definicii 'City' je pouzite 'SearchData'
//      - a v definicii 'SearchData' je pouzite 'City'
struct RoadTo;
struct City;

// Udaje pouzite v algoritmoch hladania
struct SearchData {
    // Mozete si doplnit dalsie atributy a metody, ktore pouzijete v algoritmoch hladania
    bool discovered;
    unsigned distance;
    const City *previous;
    bool foundShortestPath;

    void clear() { // nastavi na implicitne hodnoty (tuto metodu mozete upravit (ale nie jej deklaracnu cast - nedoplnajte parametre))
        discovered = false;
        distance = numeric_limits<unsigned>::max();
        previous = nullptr;
        foundShortestPath = false;
    }
};

// Obec (uzol grafu)
struct City {
    string name; // nazov obce
    list<RoadTo> roads; // zoznam "priamych" jednosmernych cestnych spojeni do dalsich obci (zoznam vystupnych hran z tohto uzla grafu)

    SearchData searchData; // udaje pouzite v algoritmoch hladania

    // Mozete doplnit dalsi konstruktor alebo metody, ale tento konstruktor nemente
    explicit City(string name) : name(std::move(name)) {
        searchData.clear();
    };
};

// Jednosmerne "priame" cestne spojenie do dalsej obce (orientovana, ohodnotena hrana grafu)
struct RoadTo {
    City *city; // obec, do ktorej je toto "priame" cestne spojenie
    unsigned length; // dlzka tohto "priameho" spojenia

    // Mozete doplnit dalsi konstruktor alebo metody, ale tento konstruktor nemente
    RoadTo(City *city, unsigned length)
            : city(city)
            , length(length) {
    }
};

// Cestna mapa planety (orientovany, ohodnoteny graf)
struct Planet {
    list<City> cities; // zoznam obci na planete (zoznam vrcholov grafu)

    void clearSearchData() { // inicializuje atributy pouzite v algoritme hladania
        for(City &c : cities) {
            c.searchData.clear();
        }
    }
};

// Vyminka v pripade neexistencie obce so zadanym nazvom
class CityNotExistsException : exception {
    string cityName; // nazov obce
public:
    explicit CityNotExistsException(string name)
            : cityName(std::move(name)) {
    }
    const char * what() const noexcept override { // vrati nazov neexistujucej obce
        return cityName.c_str();
    }
};

//-------------------------------------------------------------------------------------------------
// 9. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia vrati zoznam nazvov vsetkych obci na planete 'planet',
    ktore su z obce s nazvom 'startCity' dosiahnutelne pomocou cestnej siete.
    Pouzite algoritmus hladania DO SIRKY.

    VSTUPNA HODNOTA:
        [in] planet - cestna siet na planete
        [in] startCity - nazov obce

    NAVRATOVA HODNOTA:
        zoznam nazvov vsetkych obci, dosiahnutelnych z obce 'startCity'

    VYNIMKA:
        CityNotExistsException - ak na planete 'planet' neexistuje obec s nazvom 'startCity'.
        Metoda 'CityNotExistsException::what()' vrati nazov neexistujucej obce (hodnota 'startCity').

    VYSTUPNE PODMIENKY:
        Vystupny zoznam obsahuje aj 'startCity'.
        Poradie vo vystupnom zozname musi zodpovedat postupnosti prehladavania algoritmom DO SIRKY.

    PRIKLAD:
        obrazok na webe

    POZNAMKA:
        Ak v implementacii pouziteje 'City::searchData',
        tak bude pravdepodobne potrebne na zaciatku zavolat 'planet->clearSearchData()',
        aj aby bolo osetrene viacnasobne volanie algoritmu s tym istym objektom 'planet'.
*/

list<string> breadthFirstSearchReachable(Planet * planet, const string & startCity) {
    list<string> result;
    if (planet == nullptr)
        return result;

    planet->clearSearchData();

    City *start = nullptr;
    for (City &c : planet->cities) {
        if (c.name == startCity) {
            start = &c;
            break;
        }
    }

    if (!start)
        throw CityNotExistsException(startCity);

    queue<City*> q;
    start->searchData.discovered = true;
    q.push(start);

    while (!q.empty()) {
        City *cur = q.front();
        q.pop();

        result.push_back(cur->name);

        for (RoadTo &r : cur->roads) {
            City *next = r.city;
            if (!next->searchData.discovered) {
                next->searchData.discovered = true;
                q.push(next);
            }
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
// 10. ULOHA (0.4 bodu)
//-------------------------------------------------------------------------------------------------
/*
    Funkcia najte dlzky najkratsich ciest z obce 'startCity' do vsetkych dosiahnutelnych obci.
    Pouzite Dijkstrov algoritmus.

    VSTUPNE HODNOTY:
        [in] planet - cestna siet na planete
        [in] startCity - nazov obce

    NAVRATOVA HODNOTA:
        Pre kazdu obec dosiahnutelnu zo 'startCity' (na planete 'planet') obsahuje dlzku najkratsej cesty zo 'startCity'.

    VYNIMKA:
        CityNotExistsException - ak na planete 'planet' neexistuje obec s nazvom 'startCity'.
        Metoda 'CityNotExistsException::what()' vrati nazov neexistujucej obce (hodnota 'startCity').

    VYSTUPNA PODMIENKA:
        Navratova hodnota obsahuje nazvy len tych miest, ktore su dosiahnutelne zo 'startCity'.
        Navratova hodnota obsahuje aj vzdialenost do 'startCity' (nula).

    PRIKLAD:
        obrazok na webe

    POZNAMKA:
        Ak v implementacii pouziteje 'City::searchData',
        tak bude pravdepodobne potrebne na zaciatku zavolat 'planet->clearSearchData()',
        aj aby bolo osetrene viacnasobne volanie algoritmu s tym istym objektom 'planet'.
*/

map<string, unsigned> dijkstra(Planet * planet, const string & startCity) {
    map<string, unsigned> result;
    if (planet == nullptr)
        return result;

    planet->clearSearchData();

    City *start = nullptr;
    for (City &c : planet->cities) {
        if (c.name == startCity) {
            start = &c;
            break;
        }
    }

    if (!start)
        throw CityNotExistsException(startCity);

    using PQItem = pair<unsigned, City*>;
    priority_queue<PQItem, vector<PQItem>, greater<PQItem>> pq;

    start->searchData.distance = 0;
    pq.push({0u, start});

    while (!pq.empty()) {
        PQItem top = pq.top();
        pq.pop();

        unsigned dist = top.first;
        City *city = top.second;

        if (city->searchData.foundShortestPath) continue;
        city->searchData.foundShortestPath = true;

        for (RoadTo &r : city->roads) {
            City *next = r.city;
            unsigned newDist = dist + r.length;
            if (newDist < next->searchData.distance) {
                next->searchData.distance = newDist;
                next->searchData.previous = city;
                pq.push(PQItem(newDist, next));
            }
        }
    }

    for (City &c : planet->cities)
        if (c.searchData.distance != numeric_limits<unsigned>::max())
            result[c.name] = c.searchData.distance;

    return result;
}

//-------------------------------------------------------------------------------------------------
// TESTOVANIE
//-------------------------------------------------------------------------------------------------

// tu mozete doplnit pomocne funkcie a struktury

int main() {
    auto printResult = [](const string &name, bool ok) {
        cout << (ok ? "✅ " : "❌ ") << name << endl;
    };

    // 1. uloha – T::met() const
    {
        const T o;
        o.met(); // if this compiles, it's OK
        printResult("1. uloha (T::met() je const)", true);
    }

    // 2. uloha – konverzny konstruktor Number(int)
    {
        Number a = 10;
        bool ok = (a.getValue() == 10);

        fun(20); // must compile

        printResult("2. uloha (konverzny konstruktor Number)", ok);
    }

    // 3. uloha – virtualne volanie met()
    {
        A a;
        B b;

        A *pa  = &a;
        A *pab = &b;
        B *pb  = &b;

        bool ok =
            pa->met()  == 'a' &&
            pab->met() == 'b' &&
            pb->met()  == 'b';

        printResult("3. uloha (virtualne met() v A/B)", ok);
    }

    // 4. uloha – virtualny destruktor
    {
        ostringstream oss;
        streambuf *oldBuf = cout.rdbuf(oss.rdbuf());

        {
            C *obj = new D;
            delete obj;
        }

        cout.rdbuf(oldBuf);

        string out = oss.str();
        bool ok = (out == string("D::~D()\nC::~C()\n"));

        printResult("4. uloha (virtualne destruktory C/D)", ok);
    }

    // 5. uloha – Complex::operator+=
    {
        Complex a(1, 2);
        Complex b(10, 20);
        Complex c(100, 200);

        c += b += a;

        bool ok =
            a.getRealPart() == 1   &&
            a.getImaginaryPart() == 2 &&
            b.getRealPart() == 11  &&
            b.getImaginaryPart() == 22 &&
            c.getRealPart() == 111 &&
            c.getImaginaryPart() == 222;

        printResult("5. uloha (Complex::operator+=)", ok);
    }

    // 6. uloha – operator<< pre Complex
    {
        Complex a(1, 2);
        Complex b(-3, -4);

        ostringstream oss;
        oss << a << " " << b;

        bool ok = (oss.str() == "1+2i -3-4i");

        printResult("6. uloha (operator<< pre Complex)", ok);
    }

    // 7. uloha – DFS na strome (pre-order)
    {
        /*
            Tree:
                   A
                 /   \
                B     C
               /
              D
        */
        // DFS pre-order (all caps): A B D C
        Node *nA = new Node('A');
        Node *nB = new Node('B');
        Node *nC = new Node('C');
        Node *nD = new Node('D');

        nA->children.push_back(nB);
        nA->children.push_back(nC);
        nB->children.push_back(nD);

        Tree tree(nA);

        list<char> dfs = depthFirstSearchUpperCases(&tree);
        string dfsStr;
        for (char ch : dfs) dfsStr.push_back(ch);

        bool ok = (dfsStr == "ACBD");

        printResult("7. uloha (DFS pre-order na strome)", ok);

        delete nD;
        delete nB;
        delete nC;
        delete nA;
    }

    // 8. uloha – BFS na strome
    {
        /*
            Same tree:
                   A
                 /   \
                B     C
               /
              D
        */
        // BFS order (all caps): A B C D
        Node *nA = new Node('A');
        Node *nB = new Node('B');
        Node *nC = new Node('C');
        Node *nD = new Node('D');

        nA->children.push_back(nB);
        nA->children.push_back(nC);
        nB->children.push_back(nD);

        Tree tree(nA);

        list<char> bfs = breadthFirstSearchUpperCases(&tree);
        string bfsStr;
        for (char ch : bfs) bfsStr.push_back(ch);

        bool ok = (bfsStr == "ABCD");

        printResult("8. uloha (BFS na strome)", ok);

        delete nD;
        delete nB;
        delete nC;
        delete nA;
    }

    // pomocna lambda na najdenie mesta v Planet
    auto findCity = [](Planet &planet, const string &name) -> City* {
        for (City &c : planet.cities) {
            if (c.name == name) return &c;
        }
        return nullptr;
    };

    // vytvorime jednu testovaciu planetu pre ulohy 9 a 10
    Planet planet;
    planet.cities.emplace_back("A");
    planet.cities.emplace_back("B");
    planet.cities.emplace_back("C");
    planet.cities.emplace_back("D");
    planet.cities.emplace_back("E"); // izolovane mesto

    City *cA = findCity(planet, "A");
    City *cB = findCity(planet, "B");
    City *cC = findCity(planet, "C");
    City *cD = findCity(planet, "D");
    City *cE = findCity(planet, "E");

    // cesty:
    // A -> B (1)
    // A -> C (5)
    // B -> C (2)
    // B -> D (4)
    // C -> D (1)
    if (cA && cB && cC && cD && cE) {
        cA->roads.emplace_back(cB, 1);
        cA->roads.emplace_back(cC, 5);
        cB->roads.emplace_back(cC, 2);
        cB->roads.emplace_back(cD, 4);
        cC->roads.emplace_back(cD, 1);
        // E bez ciest
    }

    // 9. uloha – BFS reachable
    {
        bool ok = true;

        try {
            list<string> reach = breadthFirstSearchReachable(&planet, "A");
            vector<string> v(reach.begin(), reach.end());

            vector<string> expected = {"A", "B", "C", "D"};
            ok = (v == expected);
        } catch (...) {
            ok = false;
        }

        // test vynimky
        bool exceptionOk = false;
        try {
            breadthFirstSearchReachable(&planet, "X");
        } catch (const CityNotExistsException &ex) {
            exceptionOk = (string(ex.what()) == "X");
        } catch (...) {
            exceptionOk = false;
        }
        ok = ok && exceptionOk;

        printResult("9. uloha (BFS reachable v grafe)", ok);
    }

    // 10. uloha – Dijkstra
    {
        bool ok = true;

        try {
            auto dist = dijkstra(&planet, "A");

            // ocakavane vzdialenosti:
            // A: 0
            // B: 1
            // C: 3  (A->B->C)
            // D: 4  (A->B->C->D)
            // E: nedosiahnutelne
            ok = (dist.size() == 4);
            ok = ok && (dist["A"] == 0);
            ok = ok && (dist["B"] == 1);
            ok = ok && (dist["C"] == 3);
            ok = ok && (dist["D"] == 4);
            ok = ok && (dist.find("E") == dist.end());
        } catch (...) {
            ok = false;
        }

        // test vynimky
        bool exceptionOk = false;
        try {
            dijkstra(&planet, "X");
        } catch (const CityNotExistsException &ex) {
            exceptionOk = (string(ex.what()) == "X");
        } catch (...) {
            exceptionOk = false;
        }
        ok = ok && exceptionOk;

        printResult("10. uloha (Dijkstra na grafe)", ok);
    }

    return 0;
}
