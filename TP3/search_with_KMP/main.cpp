#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

// ---------------------------------------------------------
// Construction du tableau LPS (Longest Prefix Suffix)
// lps[i] = longueur du plus long prefixe du motif qui est
// aussi un suffixe de motif[0..i]
// ---------------------------------------------------------
vector<int> calculerLPS(const string &motif, long long &nbComparaisonsLPS)
{
    int m = motif.length();
    vector<int> lps(m, 0);

    int longueur = 0; // longueur du prefixe/suffixe courant
    int i = 1;

    while (i < m)
    {
        nbComparaisonsLPS++; // on compte chaque comparaison de caracteres

        if (motif[i] == motif[longueur])
        {
            longueur++;
            lps[i] = longueur;
            i++;
        }
        else
        {
            if (longueur != 0)
            {
                longueur = lps[longueur - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}

// ---------------------------------------------------------
// Recherche KMP : trouve toutes les occurrences du motif
// dans le texte, et retourne leurs positions (vector<int>)
// ---------------------------------------------------------
vector<int> rechercheKMP(const string &texte, const string &motif, long long &nbComparaisons)
{
    vector<int> positionsTrouvees;

    int n = texte.length();
    int m = motif.length();

    if (m == 0)
        return positionsTrouvees;

    vector<int> lps = calculerLPS(motif, nbComparaisons);

    int i = 0; // index dans le texte
    int j = 0; // index dans le motif

    while (i < n)
    {
        nbComparaisons++; // comparaison texte[i] vs motif[j]

        if (texte[i] == motif[j])
        {
            i++;
            j++;

            if (j == m)
            {
                positionsTrouvees.push_back(i - j);
                j = lps[j - 1];
            }
        }
        else
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i++;
            }
        }
    }

    return positionsTrouvees;
}

// ---------------------------------------------------------
// Lecture complete d'un fichier texte dans une seule chaine
// ---------------------------------------------------------
string lireFichier(const string &nomFichier)
{
    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier \"" << nomFichier << "\"" << endl;
        return "";
    }

    stringstream buffer;
    buffer << fichier.rdbuf();
    fichier.close();

    return buffer.str();
}

// ---------------------------------------------------------
// Programme principal
// ---------------------------------------------------------
int main(int argc, char *argv[])
{
    string nomFichier;
    string motif;

    if (argc >= 3)
    {
        nomFichier = argv[1];
        motif = argv[2];
    }
    else
    {
        cout << "Entrez le nom du fichier a analyser : ";
        cin >> nomFichier;
        cout << "Entrez le motif (mot) a rechercher : ";
        cin >> motif;
    }

    // --- Lecture du fichier ---
    string texte = lireFichier(nomFichier);

    if (texte.empty())
    {
        cerr << "Fichier vide ou introuvable. Fin du programme." << endl;
        return 1;
    }

    cout << "Taille du texte  : " << texte.length() << " caracteres" << endl;
    cout << "Motif recherche  : \"" << motif << "\" (" << motif.length() << " caracteres)" << endl;
    cout << "Demarrage de la recherche KMP..." << endl;

    // --- Benchmarking : debut du chronometre ---
    auto debut = high_resolution_clock::now();

    long long nbComparaisons = 0;
    vector<int> positions = rechercheKMP(texte, motif, nbComparaisons);

    // --- Fin du chronometre ---
    auto fin = high_resolution_clock::now();
    auto duree = duration_cast<microseconds>(fin - debut);

    // --- Affichage des resultats ---
    cout << "\n====== RESULTAT ======" << endl;

    if (positions.empty())
    {
        cout << "Le motif \"" << motif << "\" n'a pas ete trouve dans le texte." << endl;
    }
    else
    {
        cout << "Le motif \"" << motif << "\" a ete trouve " << positions.size() << " fois,   " << endl;

//        for (size_t k = 0; k < positions.size(); k++)
//        {
//            cout << "  - position " << positions[k];
//
//            // Affiche un petit extrait du texte autour de l'occurrence
//            int debutExtrait = max(0, positions[k] - 5);
//            int finExtrait = min((int)texte.length(), positions[k] + (int)motif.length() + 5);
//            string extrait = texte.substr(debutExtrait, finExtrait - debutExtrait);
//
//            cout << "   (extrait : \"..." << extrait << "...\")" << endl;
//        }
    }

    cout << "\n====== BENCHMARK ======" << endl;
    cout << "Taille du texte (n)              : " << texte.length() << endl;
    cout << "Taille du motif (m)              : " << motif.length() << endl;
    cout << "Nombre de comparaisons effectuees : " << nbComparaisons << " (complexite O(n+m))" << endl;
    cout << "Temps de recherche               : " << duree.count() << " microsecondes"
         << " (" << duree.count() / 1000.0 << " ms)" << endl;

    return 0;
}
