#ifndef PILEDLL_H
#define PILEDLL_H

#include <string>

using namespace std;

__declspec(dllexport) bool verifier(string expression);

__declspec(dllexport) string infixeVersPrefixe(string expression);

__declspec(dllexport) string prefixeVersInfixe(string expression);

#endif
