
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <iomanip>
#include <cstdint>
#include <algorithm>

using namespace std;
using namespace chrono;

// ==========================================
// 1. MAX HEAP FROM SCRATCH
// ==========================================

class MaxHeap {
private:
    vector<int> tab;

    // Remonter un element
    void monter(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;

            if (tab[i] > tab[parent]) {
                swap(tab[i], tab[parent]);
                i = parent;
            } else {
                break;
            }
        }
    }

    // Descendre un element
    void descendre(int i) {
        int n = tab.size();

        while (true) {
            int gauche = 2 * i + 1;
            int droite = 2 * i + 2;
            int maxIndex = i;

            if (gauche < n && tab[gauche] > tab[maxIndex])
                maxIndex = gauche;

            if (droite < n && tab[droite] > tab[maxIndex])
                maxIndex = droite;

            if (maxIndex == i)
                break;

            swap(tab[i], tab[maxIndex]);
            i = maxIndex;
        }
    }

public:

    // Insertion
    void inserer(int valeur) {
        tab.push_back(valeur);
        monter(tab.size() - 1);
    }

    // Consulter le maximum
    int maximum() const {
        if (tab.empty())
            throw out_of_range("Heap vide");

        return tab[0];
    }

    // Extraction du maximum (equivalent a pop)
    int extraireMax() {
        if (tab.empty())
            throw out_of_range("Heap vide");

        int maxVal = tab[0];

        tab[0] = tab.back();
        tab.pop_back();

        if (!tab.empty())
            descendre(0);

        return maxVal;
    }

    bool vide() const {
        return tab.empty();
    }

    int taille() const {
        return tab.size();
    }
};


// ==========================================
// 2. GENERATION DES DONNEES ALEATOIRES
// ==========================================

vector<int> genererDonnees(int n) {
    vector<int> donnees;
    donnees.reserve(n);

    static mt19937 gen(42);
    uniform_int_distribution<int> dist(1, 10000000);

    for (int i = 0; i < n; i++) {
        donnees.push_back(dist(gen));
    }

    return donnees;
}


// ==========================================
// 3. BENCHMARKING
// ==========================================

const int REPETITIONS = 5;
const int NB_RECHERCHES = 1000000;

double benchmarkInsertionScratch(const vector<int>& donnees) {
    double total = 0;

    for (int r = 0; r < REPETITIONS; r++) {
        MaxHeap heap;

        auto debut = steady_clock::now();

        for (int x : donnees)
            heap.inserer(x);

        auto fin = steady_clock::now();

        total += duration<double, milli>(fin - debut).count();
    }

    return total / REPETITIONS;
}

double benchmarkInsertionSTL(const vector<int>& donnees) {
    double total = 0;

    for (int r = 0; r < REPETITIONS; r++) {
        priority_queue<int> heap;

        auto debut = steady_clock::now();

        for (int x : donnees)
            heap.push(x);

        auto fin = steady_clock::now();

        total += duration<double, milli>(fin - debut).count();
    }

    return total / REPETITIONS;
}


// ==========================================
// EXTRACTION
// ==========================================

double benchmarkExtractionScratch(const vector<int>& donnees) {
    double total = 0;
    uint64_t somme = 0;
    int resultat;

    for (int r = 0; r < REPETITIONS; r++) {
        MaxHeap heap;

        for (int x : donnees)
            heap.inserer(x);

        auto debut = steady_clock::now();

       /* while (!heap.vide())
            somme +=heap.extraireMax();*/

            resultat = heap.extraireMax();
        auto fin = steady_clock::now();

        total += duration<double, micro>(fin - debut).count();
    }
   //cout << "Max Scratch : " << resultat << endl;

    return total / REPETITIONS;
}

double benchmarkExtractionSTL(const vector<int>& donnees) {
    double total = 0;
    uint64_t somme = 0;
    int resultat;

    for (int r = 0; r < REPETITIONS; r++) {
        priority_queue<int> heap;

        for (int x : donnees)
            heap.push(x);

        auto debut = steady_clock::now();

        /*while (!heap.empty()) {
            somme += heap.top();
            heap.pop();
        }*/
            resultat = heap.top();
            heap.pop();

        auto fin = steady_clock::now();

        total += duration<double, micro>(fin - debut).count();
    }
    //cout << "Max STL : " << resultat << endl;

    return total / REPETITIONS;
}


// ==========================================
// RECHERCHE / CONSULTATION DU MAXIMUM
// ==========================================

double benchmarkMaximumScratch(const vector<int>& donnees) {
    MaxHeap heap;

    for (int x : donnees)
        heap.inserer(x);

    uint64_t somme = 0;

    auto debut = steady_clock::now();

    for (int i = 0; i < NB_RECHERCHES; i++)
        somme += heap.maximum();

    auto fin = steady_clock::now();

    // Utiliser le resultat pour conserver le calcul
    if (somme == 0)
        cout << "";

    return duration<double, milli>(fin - debut).count();
}

double benchmarkMaximumSTL(const vector<int>& donnees) {
    priority_queue<int> heap;

    for (int x : donnees)
        heap.push(x);

    uint64_t somme = 0;

    auto debut = steady_clock::now();

    for (int i = 0; i < NB_RECHERCHES; i++)
        somme += heap.top();

    auto fin = steady_clock::now();

    if (somme == 0)
        cout << "";

    return duration<double, milli>(fin - debut).count();
}


// ==========================================
// 4. PROGRAMME PRINCIPAL
// ==========================================

int main() {

    vector<int> tailles = {
        1000,
        10000,
        100000,
        500000,
        1000000
    };

    cout << fixed << setprecision(3);

    cout << "\n========== INSERTION (ms) ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(20) << "From Scratch"
         << "STL" << endl;

    for (int n : tailles) {
        vector<int> donnees = genererDonnees(n);

        double t1 = benchmarkInsertionScratch(donnees);
        double t2 = benchmarkInsertionSTL(donnees);

        cout << left << setw(12) << n
             << setw(20) << t1
             << t2 << endl;
    }

    cout << "\n========== EXTRACTION (us) ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(20) << "From Scratch"
         << "STL" << endl;

    for (int n : tailles) {
        vector<int> donnees = genererDonnees(n);

        double t1 = benchmarkExtractionScratch(donnees);
        double t2 = benchmarkExtractionSTL(donnees);

        cout << left << setw(12) << n
             << setw(20) << t1
             << t2 << endl;
    }

    cout << "\n========== MAXIMUM (1M consultations) ==========\n";
    cout << left << setw(12) << "Taille"
         << setw(20) << "From Scratch"
         << "STL" << endl;

    for (int n : tailles) {
        vector<int> donnees = genererDonnees(n);

        double t1 = benchmarkMaximumScratch(donnees);
        double t2 = benchmarkMaximumSTL(donnees);

        cout << left << setw(12) << n
             << setw(20) << t1
             << t2 << endl;
    }

    return 0;
}
