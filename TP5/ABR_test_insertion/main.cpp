
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace std::chrono;

// ==================== STRUCTURE ABR ====================

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droit;
};

Noeud* creerNoeud(int valeur) {
    Noeud* n = new Noeud;
    n->valeur = valeur;
    n->gauche = nullptr;
    n->droit = nullptr;
    return n;
}

// ==================== INSERTION ====================

Noeud* inserer(Noeud* racine, int valeur) {
    if (racine == nullptr)
        return creerNoeud(valeur);

    Noeud* courant = racine;

    while (true) {
        if (valeur < courant->valeur) {
            if (courant->gauche == nullptr) {
                courant->gauche = creerNoeud(valeur);
                break;
            }
            courant = courant->gauche;
        }
        else if (valeur > courant->valeur) {
            if (courant->droit == nullptr) {
                courant->droit = creerNoeud(valeur);
                break;
            }
            courant = courant->droit;
        }
        else {
            break; // Doublon ignore
        }
    }

    return racine;
}

// ==================== RECHERCHE ====================

bool rechercher(Noeud* racine, int valeur) {
    while (racine != nullptr) {
        if (racine->valeur == valeur)
            return true;

        if (valeur < racine->valeur)
            racine = racine->gauche;
        else
            racine = racine->droit;
    }

    return false;
}

// ==================== SUPPRESSION ====================

Noeud* supprimer(Noeud* racine, int valeur) {
    if (racine == nullptr)
        return nullptr;

    if (valeur < racine->valeur) {
        racine->gauche = supprimer(racine->gauche, valeur);
    }
    else if (valeur > racine->valeur) {
        racine->droit = supprimer(racine->droit, valeur);
    }
    else {
        if (racine->gauche == nullptr) {
            Noeud* temp = racine->droit;
            delete racine;
            return temp;
        }

        if (racine->droit == nullptr) {
            Noeud* temp = racine->gauche;
            delete racine;
            return temp;
        }

        Noeud* successeur = racine->droit;

        while (successeur->gauche != nullptr)
            successeur = successeur->gauche;

        racine->valeur = successeur->valeur;
        racine->droit = supprimer(
            racine->droit, successeur->valeur
        );
    }

    return racine;
}

// ==================== AFFICHAGE INfixe ====================

void afficherInfixe(Noeud* racine) {
    if (racine == nullptr)
        return;

    afficherInfixe(racine->gauche);
    cout << racine->valeur << " ";
    afficherInfixe(racine->droit);
}

// Parcours sans affichage console
long long compterParcours(Noeud* racine) {
    if (racine == nullptr)
        return 0;

    return 1 + compterParcours(racine->gauche)
             + compterParcours(racine->droit);
}

// ==================== LIBERATION MEMOIRE ====================

void libererArbre(Noeud* racine) {
    while (racine != nullptr) {
        if (racine->gauche != nullptr) {
            Noeud* gauche = racine->gauche;
            racine->gauche = gauche->droit;
            gauche->droit = racine;
            racine = gauche;
        }
        else {
            Noeud* temp = racine->droit;
            delete racine;
            racine = temp;
        }
    }
}

// ==================== AFFICHAGE DU TEMPS ====================

void afficherTemps(double tempsUs) {
    if (tempsUs < 1.0) {
        cout << fixed << setprecision(2)
             << tempsUs * 1000.0 << " ns";
    }
    else {
        cout << fixed << setprecision(3)
             << tempsUs << " us";
    }
}

// ==================== BENCHMARK INSERTION ====================

void testInsertion(int n) {
    Noeud* trie = nullptr;
    Noeud* aleatoire = nullptr;

    auto debut = steady_clock::now();

    for (int i = 1; i <= n; i++)
        trie = inserer(trie, i);

    auto fin = steady_clock::now();

    double tempsTrie =
        duration<double, micro>(fin - debut).count();

    debut = steady_clock::now();

    for (int i = 1; i <= n; i++)
        aleatoire = inserer(
            aleatoire, rand() % (n * 10) + 1
        );

    fin = steady_clock::now();

    double tempsAleatoire =
        duration<double, micro>(fin - debut).count();

    cout << left << setw(12) << n;
    afficherTemps(tempsTrie);
    cout << "\t\t";
    afficherTemps(tempsAleatoire);
    cout << '\n';

    libererArbre(trie);
    libererArbre(aleatoire);
}

// ==================== BENCHMARK RECHERCHE ====================

void testRecherche(int n) {
    Noeud* racine = nullptr;

    for (int i = 1; i <= n; i++)
        racine = inserer(
            racine, rand() % (n * 10) + 1
        );

    int tests[] = {
        racine->valeur,
        1,
        n * 10,
        -1
    };

    const char* noms[] = {
        "Racine", "Valeur 1", "Valeur max", "Absente"
    };

    for (int i = 0; i < 4; i++) {
        auto debut = steady_clock::now();

        bool trouve = rechercher(racine, tests[i]);

        auto fin = steady_clock::now();

        double temps =
            duration<double, micro>(fin - debut).count();

        cout << left << setw(12) << n
             << setw(18) << noms[i];

        afficherTemps(temps);
        cout << '\n';

        (void)trouve;
    }

    libererArbre(racine);
}

// ==================== BENCHMARK SUPPRESSION ====================

void testSuppression(int n) {
    Noeud* racine = nullptr;

    for (int i = 1; i <= n; i++)
        racine = inserer(
            racine, rand() % (n * 10) + 1
        );

    int cible = racine->valeur;

    auto debut = steady_clock::now();

    racine = supprimer(racine, cible);

    auto fin = steady_clock::now();

    double temps =
        duration<double, micro>(fin - debut).count();

    cout << left << setw(12) << n
         << setw(22) << "Suppression racine";

    afficherTemps(temps);
    cout << '\n';

    libererArbre(racine);
}

// ==================== BENCHMARK PARCOURS ====================

void testAffichage(int n) {
    Noeud* racine = nullptr;

    for (int i = 1; i <= n; i++)
        racine = inserer(
            racine, rand() % (n * 10) + 1
        );

    auto debut = steady_clock::now();

    long long visites = compterParcours(racine);

    auto fin = steady_clock::now();

    double temps =
        duration<double, micro>(fin - debut).count();

    cout << left << setw(12) << n;

    afficherTemps(temps);

    cout << "\tNoeuds visites : " << visites << '\n';

    libererArbre(racine);
}

// ==================== PROGRAMME PRINCIPAL ====================

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    int tailles[] = {1000, 5000, 10000, 20000};
    int nombreTailles = 4;

    cout << "\n========== INSERTION ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(22) << "Triee"
         << "Aleatoire\n";

    for (int i = 0; i < nombreTailles; i++)
        testInsertion(tailles[i]);

    cout << "\n========== RECHERCHE ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(18) << "Cible"
         << "Temps mesure\n";

    for (int i = 0; i < nombreTailles; i++)
        testRecherche(tailles[i]);

    cout << "\n========== SUPPRESSION ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(22) << "Operation"
         << "Temps mesure\n";

    for (int i = 0; i < nombreTailles; i++)
        testSuppression(tailles[i]);

    cout << "\n========== PARCOURS INfixe ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(22) << "Temps mesure"
         << "Noeuds visites\n";

    for (int i = 0; i < nombreTailles; i++)
        testAffichage(tailles[i]);

    return 0;
}

