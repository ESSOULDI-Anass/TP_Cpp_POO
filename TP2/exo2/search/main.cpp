#include <iostream>
#include <string>
#include <chrono>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Utilisation : "
             << argv[0]
             << " \"chaine recherchee\" < fichier.txt"
             << endl;

        return 1;
    }

    // Construire la phrase recherchee
    string recherche;

    for (int i = 1; i < argc; i++)
    {
        if (i > 1)
            recherche += " ";

        recherche += argv[i];
    }

    string ligne;

    bool trouve = false;

    // =====================================
    // DEBUT DU BENCHMARK
    // =====================================

    auto debut = chrono::high_resolution_clock::now();

    while (getline(cin, ligne))
    {
        if (ligne.find(recherche) != string::npos)
        {
            trouve = true;
        }
    }

    // =====================================
    // FIN DU BENCHMARK
    // =====================================

    auto fin = chrono::high_resolution_clock::now();

    auto duree = chrono::duration_cast<chrono::microseconds>(
        fin - debut
    );

    // Affichage APRES la mesure
    if (trouve)
    {
        cout << "Chaine trouvee." << endl;
    }
    else
    {
        cout << "Chaine non trouvee." << endl;
    }

    cout << "Chaine recherchee : "
         << recherche << endl;

    cout << "Temps de recherche : "
         << duree.count()
         << " microsecondes"
         << endl;

    return 0;
}
