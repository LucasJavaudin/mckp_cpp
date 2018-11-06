#include <cmath>
#include <vector>
#include <random>

#include "data.h"

using namespace std;

Item::Item(double p, double w, int i) : value(p), weight(w), index(i) {}

double Item::getValue() { return value; }
double Item::getWeight() { return weight; }
int Item::getIndex() { return index; }


Class::Class(vector<double> values, vector<double> weights) {
	// Create a class of items from the vector of values and weigths of the items.
	int nbItems = min( values.size(), weights.size() );
	for ( int j=0; j<nbItems; j++ ) {
		items.push_back( new Item(values[j], weights[j], j) );
	}
}

Item* Class::operator[](int i) { return items[i]; }
int Class::getNbItems() { return items.size(); }


Dataset::Dataset(int nbClasses, int nbItems, lognormal_distribution<double> value_distribution, lognormal_distribution<double> weight_distribution, default_random_engine generator) {
	for ( int i=0; i<nbClasses; i++ ) {
		// For each class, generate a random vector of values and a random vector of weights.
		vector<double> values;
		vector<double> weights;
		for ( int j=0; j<nbItems; j++ ) {
			// For each item in the class, generate the value and the weight of the item.
			double v = value_distribution(generator);
			values.push_back(v);
			double w = weight_distribution(generator);
			weights.push_back(w);
		}
		// Create a class from the vectors and add it to the classes vector.
		classes.push_back( new Class(values, weights) );
	}
}

Class* Dataset::operator[](int i) { return classes[i]; }
int Dataset::getNbClasses() { return classes.size(); }
