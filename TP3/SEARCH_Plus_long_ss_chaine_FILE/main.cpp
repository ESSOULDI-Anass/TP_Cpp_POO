#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

// ---------------------------------------------------------
// Fonction : calcule la plus longue sous-chaine commune
// (sous-chaine = caracteres CONSECUTIFS) entre deux mots.
// Utilise un tableau statique pour eviter les allocations dynamiques.
// ---------------------------------------------------------
string sousChaineCommune(const string &a, const string &b)
{
    int n = a.length();
    int m = b.length();

    // Table dp statique (ajustez la taille max si vos mots depassent 100 caracteres)
    const int MAX_LEN = 100;
    static int dp[MAX_LEN + 1][MAX_LEN + 1];

    if (n > MAX_LEN || m > MAX_LEN) {
        // Fallback de securite si un mot depasse MAX_LEN
        // (vous pouvez augmenter MAX_LEN si necessaire)
        return "";
    }

    int longueurMax = 0;
    int finIndexDansA = 0;

    for (int i = 0; i <= n; i++) {
        dp[i][0] = 0;
    }
    for (int j = 0; j <= m; j++) {
        dp[0][j] = 0;
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (a[i - 1] == b[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;

                if (dp[i][j] > longueurMax)
                {
                    longueurMax = dp[i][j];
                    finIndexDansA = i;
                }
            }
            else
            {
                dp[i][j] = 0;
            }
        }
    }

    if (longueurMax == 0)
        return "";

    return a.substr(finIndexDansA - longueurMax, longueurMax);
}

// ---------------------------------------------------------
// Fonction : lit un fichier texte et retourne un vector<string>
// contenant tous les mots du fichier
// ---------------------------------------------------------
vector<string> lireMotsDuFichier(const string &nomFichier)
{
    vector<string> mots;
    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier \"" << nomFichier << "\"" << endl;
        return mots;
    }

    string mot;
    while (fichier >> mot)
    {
        mots.push_back(mot);
    }

    fichier.close();
    return mots;
}

// ---------------------------------------------------------
// Programme principal
// ---------------------------------------------------------
int main(int argc, char *argv[])
{
    string nomFichier;

    if (argc >= 2)
    {
        nomFichier = argv[1];
    }
    else
    {
        cout << "Entrez le nom du fichier a analyser : ";
        cin >> nomFichier;
    }

    // --- Etape 1 : lecture / tokenisation ---
    vector<string> mots = lireMotsDuFichier(nomFichier);

    if (mots.empty())
    {
        cerr << "Aucun mot trouve dans le fichier. Fin du programme." << endl;
        return 1;
    }

    cout << "Nombre de mots lus dans le fichier : " << mots.size() << endl;
    cout << "Demarrage de la recherche..." << endl;

    // --- Etape 2 : benchmarking - debut du chronometre ---
    auto debut = high_resolution_clock::now();

    string meilleureSousChaine = "";
    string meilleurMot1 = "";
    string meilleurMot2 = "";
    long long nbComparaisons = 0; // compte le nombre de paires de mots comparees

    int n = mots.size();

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            nbComparaisons++; // on compte chaque paire (mots[i], mots[j]) comparee

            string candidate = sousChaineCommune(mots[i], mots[j]);

            if (candidate.length() > meilleureSousChaine.length())
            {
                meilleureSousChaine = candidate;
                meilleurMot1 = mots[i];
                meilleurMot2 = mots[j];
            }
        }
    }

    // --- Fin du chronometre ---
    auto fin = high_resolution_clock::now();
    auto duree = duration_cast<microseconds>(fin - debut);

    // --- Etape 3 : affichage des resultats ---
    cout << "\n====== RESULTAT ======" << endl;

    if (meilleureSousChaine.empty())
    {
        cout << "Aucune sous-chaine commune trouvee entre les mots du fichier." << endl;
    }
    else
    {
        cout << "Mot 1                 : " << meilleurMot1 << endl;
        cout << "Mot 2                 : " << meilleurMot2 << endl;
        cout << "Plus longue sous-chaine commune : \"" << meilleureSousChaine << "\"" << endl;
        cout << "Longueur              : " << meilleureSousChaine.length() << " caracteres" << endl;
    }

    cout << "\n====== BENCHMARK ======" << endl;
    cout << "Nombre total de mots            : " << n << endl;
    cout << "Nombre de comparaisons effectuees : " << nbComparaisons << endl;
    cout << "Temps de recherche               : " << duree.count() << " microsecondes"
         << " (" << duree.count() / 1000.0 << " ms)" << endl;

    return 0;
}
