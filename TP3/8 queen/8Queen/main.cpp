
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

int N;
vector<int> position;

long long testsCorrects = 0;
long long testsIncorrects = 0;
long long totalTests = 0;
long long nombreSolutions = 0;


// Verifier si une reine peut etre placee
bool estSure(int ligne, int colonne) {

    for (int i = 0; i < ligne; i++) {

        // Chaque comparaison constitue un test
        totalTests++;

        // Meme colonne ou meme diagonale
        if (position[i] == colonne ||
            abs(position[i] - colonne) == abs(i - ligne)) {

            testsIncorrects++;
            return false;
        }
    }

    // Aucun conflit detecte : position valide
    testsCorrects++;
    return true;
}

// Afficher une solution
void afficherSolution() {
    nombreSolutions++;

    cout << "\nSolution " << nombreSolutions << " :\n";

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << (position[i] == j ? "Q " : ". ");
        }
        cout << endl;
    }
}

// Backtracking
void resoudre(int ligne) {

    if (ligne == N) {
        //afficherSolution();
        return;
    }

    for (int colonne = 0; colonne < N; colonne++) {

        if (estSure(ligne, colonne)) {
            position[ligne] = colonne;
            resoudre(ligne + 1);
        }
    }
}

int main() {

    cout << "Entrez N : ";
    cin >> N;

    if (N <= 0) {
        cout << "N doit etre positif.\n";
        return 1;
    }

    position.resize(N);

    auto debut = high_resolution_clock::now();

    resoudre(0);

    auto fin = high_resolution_clock::now();

    double tempsMs =
        duration<double, milli>(fin - debut).count();

    cout << "\n========== BENCHMARKING ==========\n";
    cout << "Nombre de reines : " << N << endl;
    cout << "Nombre de solutions : " << nombreSolutions << endl;
    cout << "Tests corrects : " << testsCorrects << endl;
    cout << "Tests incorrects : " << testsIncorrects << endl;
    cout << "Total des tests : " << totalTests << endl;
    cout << "Temps d'execution : " << tempsMs << " us" << endl;

    cout << "==================================\n";

    return 0;
}

