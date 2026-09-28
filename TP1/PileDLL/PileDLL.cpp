#include "PileDLL.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

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

__declspec(dllexport)
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


__declspec(dllexport)
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

__declspec(dllexport)
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



