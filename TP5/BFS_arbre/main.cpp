#include <iostream>
#include <queue>

using namespace std;

struct Noeud {
    int valeur;
    Noeud* gauche;
    Noeud* droite;
};

// Créer un nouveau noeud
Noeud* creerNoeud(int valeur) {
    return new Noeud{valeur, nullptr, nullptr};
}

// Insérer à gauche
void insererGauche(Noeud* parent, int valeur) {
    if (parent != nullptr && parent->gauche == nullptr) {
        parent->gauche = creerNoeud(valeur);
    }
}

// Insérer à droite
void insererDroite(Noeud* parent, int valeur) {
    if (parent != nullptr && parent->droite == nullptr) {
        parent->droite = creerNoeud(valeur);
    }
}

// Récupérer le fils gauche
Noeud* getGauche(Noeud* parent) {
    return (parent != nullptr) ? parent->gauche : nullptr;
}

// Récupérer le fils droit
Noeud* getDroite(Noeud* parent) {
    return (parent != nullptr) ? parent->droite : nullptr;
}

// Parcours en largeur (BFS)
void parcoursLargeur(Noeud* racine) {
    if (racine == nullptr)
        return;

    queue<Noeud*> q;
    q.push(racine);

    while (!q.empty()) {
        Noeud* courant = q.front();
        q.pop();

        cout << courant->valeur << " ";

        if (courant->gauche != nullptr)
            q.push(courant->gauche);

        if (courant->droite != nullptr)
            q.push(courant->droite);
    }
}

int main() {
    Noeud* racine = creerNoeud(7);

    insererGauche(racine, 9);
    insererDroite(racine->gauche, 28);
    insererGauche(racine->gauche, 1);
    insererDroite(racine->gauche->droite, 2);
    insererGauche(racine->gauche->droite, 29);

    insererDroite(racine, 67);
    insererDroite(racine->droite, 10);
    insererGauche(racine->droite, 8);
    insererDroite(racine->droite->droite, 6);
    insererGauche(racine->droite->gauche, 57);
    insererDroite(racine->droite->gauche, 40);

    cout << "Parcours en largeur : ";
    parcoursLargeur(racine);

    cout << endl;

    return 0;
}
