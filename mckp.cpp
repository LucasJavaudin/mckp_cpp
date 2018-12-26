#include <random>
#include <chrono>
#include <iostream>
#include <vector>

#include "data.h"
#include "ExhaustiveSearch.h"
#include "GreedyAlgorithm.h"
#include "DyerZemel.h"
#include "DiscreteAlgorithm.h"

using namespace std;

Dataset random_data(int nbClasses, int nbItems) {
	// Create a generator for the random numbers.
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	// Distribution for the alternatives' value.
	lognormal_distribution<double> value_distribution(0.0, 1.0);
	// Distribution for the alternatives' weight.
	lognormal_distribution<double> weight_distribution(0.0, 1.0);

	// Create dataset (nbClasses classes with nbItems items).
	Dataset data(nbClasses, nbItems, value_distribution, weight_distribution, generator);

	return data;
}

Dataset example1() {
	// Create a dataset with the values of Fig. 11.6 in Kellerer, Pferschy and Pisinger (2004).
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
	return data;
}

Dataset example2() {
	// Create a dataset with the values of Example 15 in Araldo, de Palma and Javaudin.
	// Class N1.
	int values1[] = {1, 5, 7, 8};
	int weights1[] = {0, 1, 2, 3};
	vector<double> valueClass1;
	valueClass1.assign(values1, values1+4);
	vector<double> weightClass1;
	weightClass1.assign(weights1, weights1+4);
	vector< vector<double> > class1;
	class1.push_back(valueClass1);
	class1.push_back(weightClass1);
	// Class N2.
	int values2[] = {1, 6, 7, 11};
	int weights2[] = {0, 1, 2, 3};
	vector<double> valueClass2;
	valueClass2.assign(values2, values2+4);
	vector<double> weightClass2;
	weightClass2.assign(weights2, weights2+4);
	vector< vector<double> > class2;
	class2.push_back(valueClass2);
	class2.push_back(weightClass2);
	// Class N3.
	int values3[] = {1, 2, 8, 11};
	int weights3[] = {0, 1, 2, 3};
	vector<double> valueClass3;
	valueClass3.assign(values3, values3+4);
	vector<double> weightClass3;
	weightClass3.assign(weights3, weights3+4);
	vector< vector<double> > class3;
	class3.push_back(valueClass3);
	class3.push_back(weightClass3);
	// Dataset.
	vector< vector< vector<double> > > datasetValues;
	datasetValues.push_back(class1);
	datasetValues.push_back(class2);
	datasetValues.push_back(class3);
	Dataset data(datasetValues);
	return data;
}

int main() {

	Dataset data = example2();

	cout << "===== Dataset =====" << endl;
	data.affiche();

	double capacity = 6;
	// Test Exhaustive Search.
	cout << "===== Exhaustive Search =====" << endl;
	Allocation optimalAllocation0 = ExhaustiveSearch(&data, capacity);
	cout << "Optimal value is: " << optimalAllocation0.getValue() << endl;
	// Test Dyer-Zemel.
	cout << "===== Dyer-Zemel =====" << endl;
	pair <double, Allocation> resultPair = DyerZemelAlgorithm(&data, capacity);
	Allocation optimalAllocation1 = resultPair.second;
	cout << "Optimal value is: " << optimalAllocation1.getValue() << endl;
	// Test Greedy.
	cout << "===== Greedy =====" << endl;
	pair < Allocation, vector<double> > resultPair2 = MCKP_Greedy_Algorithm(&data, capacity);
	Allocation optimalAllocation2 = resultPair2.first;
	cout << "Optimal value is: " << optimalAllocation2.getValue() << endl;
	// Test Discrete.
	cout << "===== Discrete =====" << endl;
	Allocation optimalAllocation3 = MCKP_Discrete_Algorithm(&data, capacity);
	cout << "Optimal value is: " << optimalAllocation3.getValue() << endl;
}
