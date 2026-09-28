#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <vector>
#include <iomanip>

using namespace std;
using namespace chrono;

///===========================pile normal=====================================

class pile{
private:

    typedef struct Noeud {
        char valeur;
        Noeud* suivant;
    }PPile;

   PPile*sommet;

public:

    pile() {
        sommet = nullptr;
    }

    void empiler(char c) {
        PPile* nouveau = new PPile;

        nouveau->valeur = c;
        nouveau->suivant = sommet;

        sommet = nouveau;
    }

    char depiler() {
        if (estvide()) {
            return '\0';
        }

        char valeur = sommet->valeur;

        PPile* temp = sommet;
        sommet = sommet->suivant;

        delete temp;

        return valeur;
    }

    bool estvide() {
        return sommet == nullptr;
    }
};

///=================================pile generique(liste chainnee)==========================================

template <typename T>

class Gpile{

private:

    typedef struct Noeud {
        T valeur;
        Noeud* suivant;
    }PPile;

   PPile*sommet;

public:

    Gpile() {
        sommet = nullptr;
    }

    void empiler(T c) {
        PPile* nouveau = new PPile;

        nouveau->valeur = c;
        nouveau->suivant = sommet;

        sommet = nouveau;
    }

    T depiler() {
        if (estvide()) {
            throw runtime_error("Pile vide");
        }

        T valeur = sommet->valeur;

        PPile* temp = sommet;
        sommet = sommet->suivant;

        delete temp;

        return valeur;
    }

    bool estvide() {
        return sommet == nullptr;
    }
    T top()
    {
    if (estvide())
        return T{};

    return sommet->valeur;
    }

};

///=================================pile generique(tableau dynamiue)==========================================

template <typename T>
class GTpile {

private:

    T* tab;
    int sommet;
    int capacite;

public:

    GTpile(int taille) {

        tab = new T[taille];

        capacite = taille;
        sommet = -1;
    }

    void empiler(T c) {

        if (sommet == capacite - 1) {
            throw runtime_error("Pile pleine");
        }

        sommet++;

        tab[sommet] = c;
    }

    T depiler() {

        if (estvide()) {
            throw runtime_error("Pile vide");
        }

        T valeur = tab[sommet];

        sommet--;

        return valeur;
    }

    bool estvide() {
        return sommet == -1;
    }

    ~GTpile() {
        delete[] tab;
    }
};

///================================verification l expression math ====================================
bool verifier(string expression)
{
    Gpile<char> p;

    for (char c : expression)
    {
        if (c == '(' || c == '[' || c == '{')
        {
            p.empiler(c);
        }

        else if (c == ')' || c == ']' || c == '}')
        {
            if (p.estvide())
            {
                return false;
            }

            char sommet = p.depiler();

            if (c == ')' && sommet != '(')
                return false;

            if (c == ']' && sommet != '[')
                return false;

            if (c == '}' && sommet != '{')
                return false;
        }
    }

    return p.estvide();
}
///================================infixe -> prefixe ====================================
int priorite(char c)
{
    if (c == '+' || c == '-')
        return 1;

    if (c == '*' || c == '/')
        return 2;

    return 0;
}


string infixeVersPrefixe(string expression)
{
    // Inverser
    reverse(expression.begin(), expression.end());

    // Échanger ( et )
    for (char &c : expression)
    {
        if (c == '(')
            c = ')';

        else if (c == ')')
            c = '(';
    }

    Gpile<char> p;
    string resultat = "";

    for (char c : expression)
    {
        // Opérande
        if (isalnum(c))
        {
            resultat += c;
        }

        // '('
        else if (c == '(')
        {
            p.empiler(c);
        }

        // ')'
        else if (c == ')')
        {
            while (!p.estvide() && p.top() != '(')
            {
                resultat += p.depiler();
            }

            if (!p.estvide())
                p.depiler(); // enlever '('
        }

        // Opérateur
        else if (c == '+' || c == '-' ||
                 c == '*' || c == '/')
        {
            while (!p.estvide() &&
                   p.top() != '(' &&
                   priorite(p.top()) > priorite(c))
            {
                resultat += p.depiler();
            }

            p.empiler(c);
        }
    }

    // Vider la pile
    while (!p.estvide())
    {
        resultat += p.depiler();
    }

    // Inverser le résultat
    reverse(resultat.begin(), resultat.end());

    return resultat;
}
///================================prefixe -> infixe ====================================

string prefixeVersInfixe(string expression)
{
    Gpile<string> p;

    // Parcourir de droite vers la gauche
    for (int i = expression.length() - 1; i >= 0; i--)
    {
        char c = expression[i];

        // Si c'est un opérande
        if (isalnum(c))
        {
            string s(1, c);
            p.empiler(s);
        }

        // Si c'est un opérateur
        else if (c == '+' || c == '-' ||
                 c == '*' || c == '/')
        {
            string gauche = p.depiler();
            string droite = p.depiler();

            string resultat = "(" + gauche + c + droite + ")";

            p.empiler(resultat);
        }
    }

    return p.depiler();
}

// ============================================================
// Benchmark Gpile
// ============================================================

double benchmarkGpile(int n, int repetitions)
{
    double total = 0;

    volatile long long verification = 0;

    for (int r = 0; r < repetitions; r++)
    {
        Gpile<int> p;

        auto debut = high_resolution_clock::now();

        for (int i = 0; i < n; i++)
        {
            p.empiler(i);
        }

        while (!p.estvide())
        {
            verification += p.depiler();
        }

        auto fin = high_resolution_clock::now();

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}

// ============================================================
// Benchmark GTpile
// ============================================================

double benchmarkGTpile(int n, int repetitions)
{
    double total = 0;

    volatile long long verification = 0;

    for (int r = 0; r < repetitions; r++)
    {
        GTpile<int> p(n);

        auto debut = high_resolution_clock::now();

        for (int i = 0; i < n; i++)
        {
            p.empiler(i);
        }

        while (!p.estvide())
        {
            verification += p.depiler();
        }

        auto fin = high_resolution_clock::now();

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}

// ============================================================
// Benchmark verifier()
// ============================================================

double benchmarkVerifier(int n, int repetitions)
{
    double total = 0;

    // Création d'une expression avec n caractères
    string expression;

    for (int i = 0; i < n / 2; i++)
    {
        expression += "()";
    }

    for (int r = 0; r < repetitions; r++)
    {
        auto debut = high_resolution_clock::now();

        bool resultat = verifier(expression);

        auto fin = high_resolution_clock::now();

        // Éviter que le résultat soit considéré comme inutile
        volatile bool verification = resultat;
        (void)verification;

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}


// ============================================================
// Benchmark infixeVersPrefixe()
// ============================================================

double benchmarkInfixeVersPrefixe(int n, int repetitions)
{
    double total = 0;

    // Exemple : A+A+A+A+A...
    string expression = "A";

    for (int i = 1; i < n; i++)
    {
        expression += "+A";
    }

    for (int r = 0; r < repetitions; r++)
    {
        auto debut = high_resolution_clock::now();

        string resultat = infixeVersPrefixe(expression);

        auto fin = high_resolution_clock::now();

        volatile size_t verification = resultat.size();
        (void)verification;

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}


// ============================================================
// Benchmark prefixeVersInfixe()
// ============================================================

double benchmarkPrefixeVersInfixe(int n, int repetitions)
{
    double total = 0;

    // Exemple de préfixe :
    // + + + A B C D
    //
    // Pour n opérandes, il faut n-1 opérateurs.
    string expression;

    for (int i = 0; i < n - 1; i++)
    {
        expression += "+";
    }

    for (int i = 0; i < n; i++)
    {
        expression += char('A' + (i % 26));
    }

    for (int r = 0; r < repetitions; r++)
    {
        auto debut = high_resolution_clock::now();

        string resultat = prefixeVersInfixe(expression);

        auto fin = high_resolution_clock::now();

        volatile size_t verification = resultat.size();
        (void)verification;

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}

// ============================================================
// MAIN
// ============================================================

int main()
{
     vector<int> tailles =
    {
        1000,
        10000,
        100000,
        1000000
    };

    int repetitions = 10;

    cout << fixed << setprecision(3);

    // ========================================================
    // ETUDE 1 : Gpile vs GTpile
    // ========================================================

    cout << "==============================================" << endl;
    cout << " BENCHMARK : Gpile vs GTpile" << endl;
    cout << "==============================================" << endl;

    cout << "Nombre de repetitions : "
         << repetitions << endl << endl;

    cout << left
         << setw(15) << "Taille"
         << setw(20) << "Gpile (ms)"
         << setw(20) << "GTpile (ms)"
         << endl;

    cout << "-------------------------------------------------------"
         << endl;

    for (int n : tailles)
    {
        double tempsGpile = benchmarkGpile(n, repetitions);

        double tempsGTpile = benchmarkGTpile(n, repetitions);

        cout << left
             << setw(15) << n
             << setw(20) << tempsGpile
             << setw(20) << tempsGTpile
             << endl;
    }


    // ========================================================
    // ETUDE 2 : Mesure des fonctions
    // ========================================================

    cout << endl;
    cout << "==============================================" << endl;
    cout << " BENCHMARK DES FONCTIONS" << endl;
    cout << "==============================================" << endl;

    cout << "Nombre de repetitions : "
         << repetitions << endl << endl;

    cout << left
         << setw(15) << "Taille"
         << setw(20) << "Verifier (ms)"
         << setw(25) << "Infixe->Prefixe (ms)"
         << setw(25) << "Prefixe->Infixe (ms)"
         << endl;

    cout << "----------------------------------------------------------------------------"
         << endl;

    // Pour commencer, on utilise des tailles raisonnables
    vector<int> taillesFonctions =
    {
        100,
        1000,
        10000,
        50000
    };

    for (int n : taillesFonctions)
    {
        double tempsVerifier =
            benchmarkVerifier(n, repetitions);

        double tempsInfixe =
            benchmarkInfixeVersPrefixe(n, repetitions);

        double tempsPrefixe =
            benchmarkPrefixeVersInfixe(n, repetitions);

        cout << left
             << setw(15) << n
             << setw(20) << tempsVerifier
             << setw(25) << tempsInfixe
             << setw(25) << tempsPrefixe
             << endl;
    }

    return 0;
}
