#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <list>
using namespace std;
using namespace std::chrono;

// 1. Structure d'un noeud
struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droit;
};

// 2. Creer un nouveau noeud
Noeud* creerNoeud(int valeur) {
    Noeud* nouveau = new Noeud;

    nouveau->valeur = valeur;
    nouveau->gauche = nullptr;
    nouveau->droit = nullptr;

    return nouveau;
}

// 3. Inserer une valeur dans l'ABR
Noeud* inserer(Noeud* racine, int valeur) {

    // Si l'arbre est vide
    if (racine == nullptr) {
        return creerNoeud(valeur);
    }

    // Si la valeur est inferieure
    if (valeur < racine->valeur) {
        racine->gauche = inserer(racine->gauche, valeur);
    }

    // Si la valeur est superieure
    else if (valeur > racine->valeur) {
        racine->droit = inserer(racine->droit, valeur);
    }

    // Les doublons sont ignores
    return racine;
}

// 4. Rechercher une valeur
bool rechercher(Noeud* racine, int valeur) {

    if (racine == nullptr) {
        return false;
    }

    if (racine->valeur == valeur) {
        return true;
    }

    if (valeur < racine->valeur) {
        return rechercher(racine->gauche, valeur);
    }

    return rechercher(racine->droit, valeur);
}

// 5. Parcours infixe : afficher les valeurs triees
void afficherInfixe(Noeud* racine) {

    if (racine != nullptr) {
        afficherInfixe(racine->gauche);
        cout << racine->valeur << " ";
        afficherInfixe(racine->droit);
    }
}

// 6. Compter les noeuds
int compterNoeuds(Noeud* racine) {

    if (racine == nullptr) {
        return 0;
    }

    return 1 + compterNoeuds(racine->gauche)
             + compterNoeuds(racine->droit);
}

// 7. Liberer la memoire
void libererArbre(Noeud* racine) {

    if (racine != nullptr) {
        libererArbre(racine->gauche);
        libererArbre(racine->droit);
        delete racine;
    }
}

void comparerRecherche(Noeud* racine, list<int>& liste,
                       int valeur, int repetitions) {

    // Benchmark de l'ABR
    int compteurABR = 0;

    auto debutABR = high_resolution_clock::now();

    for (int i = 0; i < repetitions; i++) {
        if (rechercher(racine, valeur)) {
            compteurABR++;
        }
    }

    auto finABR = high_resolution_clock::now();

    // Benchmark de la liste
    int compteurListe = 0;

    auto debutListe = high_resolution_clock::now();

    for (int i = 0; i < repetitions; i++) {
        for (int element : liste) {
            if (element == valeur) {
                compteurListe++;
                break;
            }
        }
    }

    auto finListe = high_resolution_clock::now();

    // Calcul des durees en microsecondes
    auto tempsABR =
        duration_cast<microseconds>(finABR - debutABR).count();

    auto tempsListe =
        duration_cast<microseconds>(finListe - debutListe).count();

    cout << "\n===== RESULTATS DU BENCHMARK =====" << endl;

    cout << "Nombre recherche : " << repetitions << endl;

    cout << "\nABR :" << endl;
    cout << "Temps total : " << tempsABR << " microsecondes" << endl;
    cout << "Nombre de recherches reussies : "
         << compteurABR << endl;

    cout << "\nListe chainee :" << endl;
    cout << "Temps total : " << tempsListe << " microsecondes" << endl;
    cout << "Nombre de recherches reussies : "
         << compteurListe << endl;
}

// 8. Programme principal
int main() {

    int tab[]={1000,10000,10000,100000};
    int j=0;
    while (j<4){

    srand(time(nullptr));

    Noeud* racine = nullptr;
    list<int> liste;



    // Generer et inserer les memes valeurs
    for (int i = 0; i < tab[j]; i++) {

        int valeur = rand() % 1000000 + 1;

        racine = inserer(racine, valeur);
        liste.push_back(valeur);
    }

    // Valeur a rechercher
    int valeurRecherchee = liste.back();

    // Comparer les temps de recherche
    comparerRecherche(racine, liste, valeurRecherchee, tab[j]);

    // Liberer la memoire de l'ABR
    libererArbre(racine);
    cout<<"======================================"<<endl;
        j++;
    }

    return 0;
}
