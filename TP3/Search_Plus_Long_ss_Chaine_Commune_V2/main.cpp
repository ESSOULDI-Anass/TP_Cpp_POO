#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <chrono>
#include <cctype>

using namespace std;
using namespace std::chrono;

// ---------------------------------------------------------
// Lecture et tokenisation du fichier
// ---------------------------------------------------------
vector<string> lireMotsDuFichier(const string &nomFichier)
{
    vector<string> mots;
    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cerr << "Impossible d'ouvrir le fichier : " << nomFichier << endl;
        return mots;
    }

    string mot;
    while (fichier >> mot)
    {
        // Nettoyage basique : on enleve la ponctuation en debut/fin de mot
        string motPropre;
        for (char c : mot)
        {
            if (isalpha(static_cast<unsigned char>(c)))
            {
                motPropre += static_cast<char>(tolower(static_cast<unsigned char>(c)));
            }
        }
        if (!motPropre.empty())
        {
            mots.push_back(motPropre);
        }
    }

    fichier.close();
    return mots;
}

// ---------------------------------------------------------
// NOUVEAU : construction du conteneur de mots uniques
// (ne garde que la premiere occurrence de chaque mot)
// ---------------------------------------------------------
vector<string> extraireMotsUniques(const vector<string> &mots)
{
    vector<string> motsUniques;
    unordered_set<string> dejaVus; // "memoire" des mots deja rencontres

    motsUniques.reserve(mots.size()); // optimisation memoire, taille reelle <= mots.size()

    for (const string &mot : mots)
    {
        // insert() renvoie une paire <iterator, bool>
        // bool == true seulement si le mot n'etait PAS deja dans le set
        if (dejaVus.insert(mot).second)
        {
            motsUniques.push_back(mot);
        }
        // sinon : mot deja vu -> on l'ignore, aucune comparaison future ne le concernera
    }

    return motsUniques;
}

// ---------------------------------------------------------
// Plus longue sous-chaine commune entre deux mots (programmation dynamique)
// ---------------------------------------------------------
string sousChaineCommune(const string &A, const string &B)
{
    int n = A.size();
    int m = B.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    int longueurMax = 0;
    int finIndex = 0;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            if (A[i - 1] == B[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > longueurMax)
                {
                    longueurMax = dp[i][j];
                    finIndex = i;
                }
            }
            else
            {
                dp[i][j] = 0;
            }
        }
    }

    return A.substr(finIndex - longueurMax, longueurMax);
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

    cout << "Nombre de mots lus dans le fichier    : " << mots.size() << endl;

    // --- Etape 2 (NOUVEAU) : deduplication avant la recherche ---
    // On isole ce calcul du chronometre de recherche, car ce n'est pas
    // la phase qu'on veut comparer a la version naive (comme demande dans le TP,
    // seule la phase de recherche est chronometree). On peut neanmoins
    // afficher son cout separement pour etre transparent dans le rapport.
    auto debutDedup = high_resolution_clock::now();
    vector<string> motsUniques = extraireMotsUniques(mots);
    auto finDedup = high_resolution_clock::now();
    auto dureeDedup = duration_cast<microseconds>(finDedup - debutDedup);

    cout << "Nombre de mots uniques (dedoublonnes) : " << motsUniques.size() << endl;
    cout << "Mots repetes ignores                  : "
         << (mots.size() - motsUniques.size()) << endl;
    cout << "Temps de deduplication                : " << dureeDedup.count()
         << " microsecondes" << endl;
    cout << "Demarrage de la recherche..." << endl;

    // --- Etape 3 : benchmarking - debut du chronometre (phase de recherche uniquement) ---
    auto debut = high_resolution_clock::now();

    string meilleureSousChaine = "";
    string meilleurMot1 = "";
    string meilleurMot2 = "";
    long long nbComparaisons = 0; // compte le nombre de paires de mots comparees

    int n = motsUniques.size(); // <-- on boucle sur les mots UNIQUES, plus sur "mots"

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            nbComparaisons++; // on compte chaque paire (motsUniques[i], motsUniques[j]) comparee

            string candidate = sousChaineCommune(motsUniques[i], motsUniques[j]);

            if (candidate.length() > meilleureSousChaine.length())
            {
                meilleureSousChaine = candidate;
                meilleurMot1 = motsUniques[i];
                meilleurMot2 = motsUniques[j];
            }
        }
    }

    // --- Fin du chronometre ---
    auto fin = high_resolution_clock::now();
    auto duree = duration_cast<microseconds>(fin - debut);

    // --- Etape 4 : affichage des resultats ---
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
    cout << "Nombre total de mots (bruts)       : " << mots.size() << endl;
    cout << "Nombre de mots uniques compares    : " << n << endl;
    cout << "Nombre de comparaisons effectuees  : " << nbComparaisons << endl;
    cout << "Temps de recherche                 : " << duree.count() << " microsecondes"
         << " (" << duree.count() / 1000.0 << " ms)" << endl;

    return 0;
}
