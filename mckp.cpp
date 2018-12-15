#include <random>
#include <chrono>
#include <iostream>
#include <vector>

#include "data.h"
#include "GreedyAlgorithm.h"
#include "DyerZemel.h"
#include "DiscreteAlgorithm.h"

using namespace std;

int main() {
	// Create a generator for the random numbers.
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	// Distribution for the alternatives' value.
	lognormal_distribution<double> value_distribution(0.0, 1.0);
	// Distribution for the alternatives' weight.
	lognormal_distribution<double> weight_distribution(0.0, 1.0);

	// Create dataset (1000 classes with 10 items).
	Dataset data(1000, 10, value_distribution, weight_distribution, generator);

	/*
	// Create a dataset with the values of Fig. 11.6.
	// Class N1.
	int values1[] = {76, 49, 79, 41, 25, 78, 31, 55, 63, 19, 89};
	int weights1[] = {26, 62, 84, 35, 7, 54, 82, 12, 23, 50, 65};
	vector<double> valueClass1;
	valueClass1.assign(values1, values1+11);
	vector<double> weightClass1;
	weightClass1.assign(weights1, weights1+11);
	vector< vector<double> > class1;
	class1.push_back(valueClass1);
	class1.push_back(weightClass1);
	// Class N2.
	int values2[] = {95, 17, 36, 47, 29, 51, 84, 73};
	int weights2[] = {36, 25, 62, 12, 91, 28, 73, 54};
	vector<double> valueClass2;
	valueClass2.assign(values2, values2+8);
	vector<double> weightClass2;
	weightClass2.assign(weights2, weights2+8);
	vector< vector<double> > class2;
	class2.push_back(valueClass2);
	class2.push_back(weightClass2);
	// Class N3.
	int values3[] = {35, 39, 20, 51, 24, 71, 59, 80, 14};
	int weights3[] = {8, 85, 61, 62, 30, 40, 24, 85, 11};
	vector<double> valueClass3;
	valueClass3.assign(values3, values3+9);
	vector<double> weightClass3;
	weightClass3.assign(weights3, weights3+9);
	vector< vector<double> > class3;
	class3.push_back(valueClass3);
	class3.push_back(weightClass3);
	// Dataset.
	vector< vector< vector<double> > > datasetValues;
	datasetValues.push_back(class1);
	datasetValues.push_back(class2);
	datasetValues.push_back(class3);
	Dataset data(datasetValues);
	*/

	/*
	// Test de la fonction upperBound() qui rend l'enveloppe convexe d'une classe
	Class c(vector<double>({1,3,2.3,4,1,0.9,1.9,1.5,4.5}),vector<double>({1,2,2.5,3,3.5,4,4.2,4.5,4.7}));
	Class c2 = c.upperBound();
	c2.affiche();
	pair<double,double> a = MCKP_Greedy_Algorithm(data,100);
	cout << "poids vide " << a.first << " valeur totale " << a.second;
	// data.getClasses()[0]->getItems()[0]->affiche();
	*/

	// Test Dyer-Zemel.
	data.affiche();
	double minWeight = data.getMinWeight();
	double maxWeight = data.getMaxWeight();
	double capacity = (maxWeight + minWeight) / 2;
	if ( capacity > minWeight && capacity < maxWeight ) {
		pair <double, Allocation> resultPair = DyerZemelAlgorithm(&data, capacity);
		double optimalSlope = resultPair.first;
		Allocation optimalAllocation = resultPair.second;
		cout << "Optimal slope is " + to_string(optimalSlope) << endl;
		//cout << "Optimal allocation is:" << endl;
		//optimalAllocation.affiche();
	} else {
		cout << "Capacity is not compatible with dataset." << endl;
	}
}
