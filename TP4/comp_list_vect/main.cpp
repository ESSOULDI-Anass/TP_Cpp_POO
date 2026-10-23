#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <chrono>
#include <string>
#include <iomanip>

using namespace std;
using namespace std::chrono;

int main()
{
    vector<string> mots;
    list<string> mots2;

    string file = "file3.txt";
    ifstream fichier(file);

    if (!fichier.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir le fichier \""
             << file << "\"" << endl;
        return 1;
    }

    string mot;

    // Chargement initial des mots
    while (fichier >> mot)
    {
        mots.push_back(mot);
        mots2.push_back(mot);
    }

    fichier.close();

    if (mots.empty())
    {
        cout << "Le fichier est vide." << endl;
        return 0;
    }

    cout << "Nombre de mots : " << mots.size() << endl;

    // Variables pour stocker les durées
    long long v_ins_debut = 0, l_ins_debut = 0;
    long long v_ins_milieu = 0, l_ins_milieu = 0;
    long long v_ins_fin = 0, l_ins_fin = 0;
    long long v_supp_debut = 0, l_supp_debut = 0;
    long long v_supp_milieu = 0, l_supp_milieu = 0;
    long long v_supp_fin = 0, l_supp_fin = 0;

    auto debut = high_resolution_clock::now();
    auto fin = high_resolution_clock::now();

    // ---------------------------------------------------------
    // 1. INSERTION AU DEBUT
    // ---------------------------------------------------------
    debut = high_resolution_clock::now();
    mots.insert(mots.begin(), "NOUVEAU");
    fin = high_resolution_clock::now();
    v_ins_debut = duration_cast<microseconds>(fin - debut).count();

    debut = high_resolution_clock::now();
    mots2.push_front("NOUVEAU");
    fin = high_resolution_clock::now();
    l_ins_debut = duration_cast<microseconds>(fin - debut).count();


    // ---------------------------------------------------------
    // 2. INSERTION AU MILIEU
    // ---------------------------------------------------------
    size_t midVector = mots.size() / 2;
    debut = high_resolution_clock::now();
    mots.insert(mots.begin() + midVector, "NOUVEAU");
    fin = high_resolution_clock::now();
    v_ins_milieu = duration_cast<microseconds>(fin - debut).count();

    debut = high_resolution_clock::now();
    auto it2 = mots2.begin();
    advance(it2, mots2.size() / 2);
    mots2.insert(it2, "NOUVEAU");
    fin = high_resolution_clock::now();
    l_ins_milieu = duration_cast<microseconds>(fin - debut).count();


    // ---------------------------------------------------------
    // 3. INSERTION A LA FIN
    // ---------------------------------------------------------
    debut = high_resolution_clock::now();
    mots.push_back("NOUVEAU");
    fin = high_resolution_clock::now();
    v_ins_fin = duration_cast<microseconds>(fin - debut).count();

    debut = high_resolution_clock::now();
    mots2.push_back("NOUVEAU");
    fin = high_resolution_clock::now();
    l_ins_fin = duration_cast<microseconds>(fin - debut).count();


    // ---------------------------------------------------------
    // 4. SUPPRESSION AU DEBUT
    // ---------------------------------------------------------
    debut = high_resolution_clock::now();
    mots.erase(mots.begin());
    fin = high_resolution_clock::now();
    v_supp_debut = duration_cast<microseconds>(fin - debut).count();

    debut = high_resolution_clock::now();
    mots2.pop_front();
    fin = high_resolution_clock::now();
    l_supp_debut = duration_cast<microseconds>(fin - debut).count();


    // ---------------------------------------------------------
    // 5. SUPPRESSION AU MILIEU
    // ---------------------------------------------------------
    midVector = mots.size() / 2;
    debut = high_resolution_clock::now();
    mots.erase(mots.begin() + midVector);
    fin = high_resolution_clock::now();
    v_supp_milieu = duration_cast<microseconds>(fin - debut).count();

    debut = high_resolution_clock::now();
    it2 = mots2.begin();
    advance(it2, mots2.size() / 2);
    mots2.erase(it2);
    fin = high_resolution_clock::now();
    l_supp_milieu = duration_cast<microseconds>(fin - debut).count();


    // ---------------------------------------------------------
    // 6. SUPPRESSION A LA FIN
    // ---------------------------------------------------------
    debut = high_resolution_clock::now();
    mots.pop_back();
    fin = high_resolution_clock::now();
    v_supp_fin = duration_cast<microseconds>(fin - debut).count();

    debut = high_resolution_clock::now();
    mots2.pop_back();
    fin = high_resolution_clock::now();
    l_supp_fin = duration_cast<microseconds>(fin - debut).count();


    // ---------------------------------------------------------
    // AFFICHAGE DES RESULTATS SOUS FORME DE TABLEAU
    // ---------------------------------------------------------
    cout << "\n========================================================" << endl;
    cout << left << setw(30) << "Operation"
         << right << setw(12) << "Vector (us)"
         << setw(12) << "List (us)" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << left << setw(30) << "Insertion au debut"     << right << setw(12) << v_ins_debut   << setw(12) << l_ins_debut   << endl;
    cout << left << setw(30) << "Insertion au milieu"    << right << setw(12) << v_ins_milieu  << setw(12) << l_ins_milieu  << endl;
    cout << left << setw(30) << "Insertion a la fin"     << right << setw(12) << v_ins_fin     << setw(12) << l_ins_fin     << endl;
    cout << left << setw(30) << "Suppression au debut"   << right << setw(12) << v_supp_debut  << setw(12) << l_supp_debut  << endl;
    cout << left << setw(30) << "Suppression au milieu"  << right << setw(12) << v_supp_milieu << setw(12) << l_supp_milieu << endl;
    cout << left << setw(30) << "Suppression a la fin"     << right << setw(12) << v_supp_fin    << setw(12) << l_supp_fin    << endl;
    cout << "========================================================" << endl;

    return 0;
}
