#include <iostream>
#include <map>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

struct Noeud {
    int v;
    Noeud *g, *d;
    Noeud(int x) : v(x), g(nullptr), d(nullptr) {}
};

void insererABR(Noeud*& r, int x) {
    if (!r) { r = new Noeud(x); return; }
    Noeud* p = r;
    while (true) {
        if (x < p->v) {
            if (!p->g) { p->g = new Noeud(x); return; }
            p = p->g;
        } else if (x > p->v) {
            if (!p->d) { p->d = new Noeud(x); return; }
            p = p->d;
        } else return;
    }
}

bool chercherABR(Noeud* r, int x) {
    while (r) {
        if (x == r->v) return true;
        r = (x < r->v) ? r->g : r->d;
    }
    return false;
}

bool supprimerABR(Noeud*& r, int x) {
    Noeud** lien = &r;
    while (*lien && (*lien)->v != x)
        lien = (x < (*lien)->v) ? &((*lien)->g) : &((*lien)->d);
    if (!*lien) return false;

    Noeud* cible = *lien;
    if (!cible->g) {
        *lien = cible->d;
        delete cible;
    } else if (!cible->d) {
        *lien = cible->g;
        delete cible;
    } else {
        Noeud** ls = &(cible->d);
        while ((*ls)->g) ls = &((*ls)->g);
        Noeud* s = *ls;
        cible->v = s->v;
        *ls = s->d;
        delete s;
    }
    return true;
}

// Liberation iterative, adaptee meme a un ABR tres desequilibre.
void libererABR(Noeud*& r) {
    while (r) {
        if (r->g) {
            Noeud* g = r->g;
            r->g = g->d;
            g->d = r;
            r = g;
        } else {
            Noeud* suivant = r->d;
            delete r;
            r = suivant;
        }
    }
}

void melanger(int t[], int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int x = t[i]; t[i] = t[j]; t[j] = x;
    }
}

double insertionABR(const int t[], int n) {
    Noeud* r = nullptr;
    auto a = steady_clock::now();
    for (int i = 0; i < n; ++i) insererABR(r, t[i]);
    auto b = steady_clock::now();
    double us = duration<double, micro>(b-a).count();
    libererABR(r);
    return us;
}

double insertionMap(const int t[], int n) {
    map<int,int> m;
    auto a = steady_clock::now();
    for (int i = 0; i < n; ++i) m.insert({t[i], t[i]});
    auto b = steady_clock::now();
    return duration<double, micro>(b-a).count();
}

double rechercheABR(Noeud* r, const int t[], int n, int& ok) {
    ok = 0;
    auto a = steady_clock::now();
    for (int i = 0; i < n; ++i) if (chercherABR(r,t[i])) ++ok;
    auto b = steady_clock::now();
    return duration<double, micro>(b-a).count();
}

double rechercheMap(map<int,int>& m, const int t[], int n, int& ok) {
    ok = 0;
    auto a = steady_clock::now();
    for (int i = 0; i < n; ++i) if (m.find(t[i]) != m.end()) ++ok;
    auto b = steady_clock::now();
    return duration<double, micro>(b-a).count();
}

double suppressionABR(Noeud*& r, const int t[], int n) {
    auto a = steady_clock::now();
    for (int i = 0; i < n; ++i) supprimerABR(r,t[i]);
    auto b = steady_clock::now();
    return duration<double, micro>(b-a).count();
}

double suppressionMap(map<int,int>& m, const int t[], int n) {
    auto a = steady_clock::now();
    for (int i = 0; i < n; ++i) m.erase(t[i]);
    auto b = steady_clock::now();
    return duration<double, micro>(b-a).count();
}

void ligne(const char* op, int n, double a, double m) {
    cout << left << setw(15) << op << right << setw(9) << n
         << setw(17) << fixed << setprecision(2) << a
         << setw(17) << m << '\n';
}

void tester(const char* type, const int donnees[], int n) {
    Noeud* r = nullptr;
    map<int,int> m;
    int* ordreRecherche = new int[n];
    int* ordreSuppression = new int[n];

    for (int i=0; i<n; ++i) {
        ordreRecherche[i] = donnees[i];
        ordreSuppression[i] = donnees[i];
    }
    // Meme ordre de recherche/suppression pour les deux structures.
    melanger(ordreRecherche,n);
    melanger(ordreSuppression,n);

    cout << "\n========== " << type << " | N = " << n << " ==========\n";
    cout << left << setw(15) << "Operation" << right << setw(9) << "N"
         << setw(17) << "ABR (us)" << setw(17) << "map (us)" << '\n';

    // Mesure de l'insertion sur des structures vides.
    ligne("Insertion",n,insertionABR(donnees,n),insertionMap(donnees,n));

    // Construction hors chronometrage pour recherche et suppression.
    for (int i=0; i<n; ++i) {
        insererABR(r,donnees[i]);
        m.insert({donnees[i],donnees[i]});
    }

    int okA, okM;
    double trA = rechercheABR(r,ordreRecherche,n,okA);
    double trM = rechercheMap(m,ordreRecherche,n,okM);
    ligne("Recherche",n,trA,trM);
    cout << "Recherches trouvees : ABR " << okA << "/" << n
         << " | map " << okM << "/" << n << '\n';

    double tsA = suppressionABR(r,ordreSuppression,n);
    double tsM = suppressionMap(m,ordreSuppression,n);
    ligne("Suppression",n,tsA,tsM);
    cout << "Elements restants : ABR " << (r ? "non nul" : "0")
         << " | map " << m.size() << '\n';

    libererABR(r);
    delete[] ordreRecherche;
    delete[] ordreSuppression;
}

int main() {
    srand((unsigned)time(nullptr));
    const int tailles[] = {1000,5000,10000,20000};
    for (int n : tailles) {
        int* triees = new int[n];
        int* aleatoires = new int[n];
        for (int i=0; i<n; ++i) triees[i] = aleatoires[i] = i+1;
        melanger(aleatoires,n);

        tester("INSERTION TRIEE",triees,n);
        tester("INSERTION ALEATOIRE",aleatoires,n);

        delete[] triees;
        delete[] aleatoires;
    }
    return 0;
}
