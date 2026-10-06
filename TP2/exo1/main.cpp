#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

using namespace std;

int comparer(const string& s1, const string& s2)
{
    if (s1 == s2)
        return 0;

    if (s1.find(s2) != string::npos)
        return 1;

    if (s2.find(s1) != string::npos)
        return 2;

    return -1;
}

// Mesurer une seule execution
long long mesurer(const string& s1, const string& s2)
{
    auto debut = chrono::high_resolution_clock::now();

    volatile int resultat = comparer(s1, s2);

    auto fin = chrono::high_resolution_clock::now();

    auto duree = chrono::duration_cast<chrono::nanoseconds>(
        fin - debut
    );

    return duree.count();
}

int main()
{
    int tailles[] = {10, 100, 1000, 10000, 100000};

    cout << "================ BENCHMARK ================\n\n";

    cout << left
         << setw(12) << "Taille"
         << setw(20) << "Cas"
         << setw(15) << "Temps (ns)"
         << endl;

    cout << "-----------------------------------------------\n";

    for (int taille : tailles)
    {
        // Chaine principale
        string s1(taille, 'a');

        // =====================================
        // 1. EGALITE
        // =====================================

        string s2 = s1;

        long long temps = mesurer(s1, s2);

        cout << left
             << setw(12) << taille
             << setw(20) << "Egalite"
             << setw(15) << temps
             << endl;


        // =====================================
        // 2. RECHERCHE AU DEBUT
        // =====================================

        s2 = string(10, 'a');

        temps = mesurer(s1, s2);

        cout << left
             << setw(12) << taille
             << setw(20) << "Debut"
             << setw(15) << temps
             << endl;


        // =====================================
        // 3. RECHERCHE AU MILIEU
        // =====================================

        s1[taille / 2] = 'b';

        s2 = "ab";

        temps = mesurer(s1, s2);

        cout << left
             << setw(12) << taille
             << setw(20) << "Milieu"
             << setw(15) << temps
             << endl;


        // =====================================
        // 4. RECHERCHE A LA FIN
        // =====================================

        s1 = string(taille, 'a');

        if (taille >= 2)
        {
            s1[taille - 2] = 'b';
            s1[taille - 1] = 'c';
        }

        s2 = "bc";

        temps = mesurer(s1, s2);

        cout << left
             << setw(12) << taille
             << setw(20) << "Fin"
             << setw(15) << temps
             << endl;


        // =====================================
        // 5. ELEMENT ABSENT
        // =====================================

        s1 = string(taille, 'a');
        s2 = "xyz";

        temps = mesurer(s1, s2);

        cout << left
             << setw(12) << taille
             << setw(20) << "Absent"
             << setw(15) << temps
             << endl;

        cout << "-----------------------------------------------\n";
    }

    return 0;
}
