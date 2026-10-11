
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Fonction qui remplace toutes les occurrences
int recherche(string &mot, const string &sous_chaine,
              const string &nouveau)
{
    if (sous_chaine.empty())
        return 0;

    int compteur = 0;
    size_t pos = 0;

    while ((pos = mot.find(sous_chaine, pos)) != string::npos)
    {
        mot.replace(pos, sous_chaine.size(), nouveau);

        // Avancer apres le remplacement
        pos += nouveau.size();

        compteur++;
    }

    return compteur;
}

int main()
{
    string nomFichier;
    string sous_chaine;
    string nouveau;
    string contenu;
    string ligne;

    // 1. Demander le nom du fichier
    cout << "Entrer le nom du fichier TXT : ";
    getline(cin, nomFichier);

    // 2. Ouvrir le fichier en lecture
    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cout << "Erreur : impossible d'ouvrir le fichier."
             << endl;
        cout << "Verifiez le nom et le chemin du fichier."
             << endl;
        return 1;
    }

    // Lire tout le contenu en conservant les lignes
    {
        ostringstream buffer;
        buffer << fichier.rdbuf();
        contenu = buffer.str();
    }

    fichier.close();

    // 3. Demander les chaines
    cout << "Entrer la chaine a remplacer : ";
    getline(cin, sous_chaine);

    if (sous_chaine.empty())
    {
        cout << "Erreur : la chaine a remplacer est vide."
             << endl;
        return 1;
    }

    cout << "Entrer la nouvelle chaine : ";
    getline(cin, nouveau);

    // 4. Effectuer les remplacements et mesurer le temps
    auto debutTotal = high_resolution_clock::now();

    int nombre = recherche(contenu, sous_chaine, nouveau);

    auto finTotal = high_resolution_clock::now();

    auto duree = duration_cast<microseconds>(
        finTotal - debutTotal
    );

    // 5. Enregistrer le resultat dans le fichier original
    ofstream sortie(nomFichier, ios::out | ios::trunc);

    if (!sortie.is_open())
    {
        cout << "Erreur : impossible d'enregistrer les modifications."
             << endl;
        return 1;
    }

    sortie << contenu;
    sortie.close();

    // 6. Afficher les resultats
    cout << "\n========== RESULTAT ==========" << endl;

    if (nombre > 0)
    {
        cout << "Remplacement effectue avec succes !" << endl;
        cout << "Ancienne chaine : " << sous_chaine << endl;
        cout << "Nouvelle chaine : " << nouveau << endl;
        cout << "Nombre de remplacements : " << nombre << endl;
    }
    else
    {
        cout << "Aucune occurrence trouvee." << endl;
        cout << "Le contenu du fichier reste inchange." << endl;
    }

    cout << "Temps d'execution : "
         << duree.count() << " microsecondes" << endl;

    cout << "==============================" << endl;

    return 0;
}
