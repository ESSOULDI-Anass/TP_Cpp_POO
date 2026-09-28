#include <iostream>
#include <string>
#include <chrono>
#include "PileDLL.h"

using namespace std;
using namespace chrono;

// ============================================================
// Benchmark de verifier()
// ============================================================
double benchmarkVerifier(int n, int repetitions)
{
    double total = 0;

    // Création d'une expression valide de taille n
    string expression;

    for (int i = 0; i < n / 2; i++)
        expression += "()";

    for (int r = 0; r < repetitions; r++)
    {
        auto debut = high_resolution_clock::now();

        bool resultat = verifier(expression);

        auto fin = high_resolution_clock::now();

        // Évite que le compilateur considère le résultat inutile
        if (!resultat)
            cerr << "";

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}


// ============================================================
// Benchmark de infixeVersPrefixe()
// ============================================================
double benchmarkInfixeVersPrefixe(int n, int repetitions)
{
    double total = 0;

    // Exemple : A+A+A+A+A...
    string expression = "A";

    for (int i = 1; i < n; i++)
        expression += "+A";

    for (int r = 0; r < repetitions; r++)
    {
        auto debut = high_resolution_clock::now();

        string resultat = infixeVersPrefixe(expression);

        auto fin = high_resolution_clock::now();

        if (resultat.empty())
            cerr << "";

        total += duration<double, milli>(fin - debut).count();
    }

    return total / repetitions;
}


// ============================================================
// Programme principal
// ============================================================
int main()
{
    cout << "==============================================" << endl;
    cout << "     BENCHMARK APRES INTEGRATION DLL" << endl;
    cout << "==============================================" << endl;

    const int repetitions = 10;

    int tailles[] = {1000, 10000, 100000, 1000000};

    cout << endl;

    // --------------------------------------------------------
    // Test de verifier()
    // --------------------------------------------------------
    cout << "Benchmark de verifier()" << endl;
    cout << "----------------------------------------------" << endl;

    cout << "Taille\t\tTemps moyen (ms)" << endl;

    for (int n : tailles)
    {
        double temps = benchmarkVerifier(n, repetitions);

        cout << n << "\t\t"
             << temps
             << endl;
    }

    // --------------------------------------------------------
    // Test de infixeVersPrefixe()
    // --------------------------------------------------------
    cout << endl;
    cout << "Benchmark de infixeVersPrefixe()" << endl;
    cout << "----------------------------------------------" << endl;

    cout << "Taille\t\tTemps moyen (ms)" << endl;

    for (int n : tailles)
    {
        double temps = benchmarkInfixeVersPrefixe(n, repetitions);

        cout << n << "\t\t"
             << temps
             << endl;
    }

    return 0;
}
