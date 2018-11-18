#include <random>
#include <chrono>
#include <iostream>

#include "data.h"

using namespace std;

int main() {
	// Create a generator for the random numbers.
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	// Distribution for the alternatives' value.
	lognormal_distribution<double> value_distribution(0.0, 1.0);
	// Distribution for the alternatives' weight.
	lognormal_distribution<double> weight_distribution(0.0, 1.0);

	// Create dataset.
	Dataset data(100, 100, value_distribution, weight_distribution, generator);

	// Test de la fonction upperBound() qui rend l'enveloppe convexe d'une classe
	Class c(vector<double>({1,3,2.3,4,1,0.9,1.9,1.5,4.5}),vector<double>({1,2,2.5,3,3.5,4,4.2,4.5,4.7}));
    Class c2 = c.upperBound();
    c2.affiche();
	pair<double,double> a = MCKP_Greedy_Algorithm(data,100);
	cout << "poids vide " << a.first << " valeur totale " << a.second;
	// data.getClasses()[0]->getItems()[0]->affiche();
	return 0;
}
