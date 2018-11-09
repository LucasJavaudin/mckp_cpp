#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

#include "data.h"

using namespace std;

Item::Item(double p, double w, int i) : value(p), weight(w), index(i) {}

double Item::getValue() const{ return value; }
double Item::getWeight() const{ return weight; }
int Item::getIndex() const{ return index; }

struct compareWeight{
bool operator ()(const Item * it1,const Item * it2) const
{
    if (it1 != NULL && it2 != NULL)
        return it1->getWeight() < it2->getWeight();
    else return false;
}
};

// La fonction renvoie true si @it1 domine @it2, false sinon
bool domine(Item* it1, Item* it2){
    return(it1->getWeight()<=it2->getWeight() && it1->getValue()>it2->getValue());
}

// La fonction renvoie true si @it1 et @it3 LP-dominent @it2
bool domineLP(Item* it1, Item* it2, Item* it3){
    double terme1 = ( it3->getValue() - it2->getValue() ) / ( it3->getValue() - it2->getValue() );
    double terme2 = ( it2->getValue() - it1->getValue() ) / ( it2->getValue() - it1->getValue() );
    return(terme1>=terme2);
}

Class::Class(vector<double> values, vector<double> weights) {
	// Create a class of items from the vector of values and weigths of the items.
	int nbItems = min( values.size(), weights.size() );
	for ( int j=0; j<nbItems; j++ ) {
		items.push_back( new Item(values[j], weights[j], j) );
	}
}

Class::Class(vector<Item*> items):items(items) {
	// Create a class of items from a vector of items.
}

Item* Class::operator[](int i) { return items[i]; }
int Class::getNbItems() const { return items.size(); }
void Class::sortItemsWeight() { sort (items.begin(), items.end(), compareWeight()); }
Class Class::upperBound(){
    // construit l'enveloppe convexe de la classe
    this->sortItemsWeight(); // on commence par trier par ordre croissant les items selon leur poids
    vector<Item*> res;
    res.push_back(items[0]); // je suppose que weight[0] est strictement plus petit pour l'instant, à voir ensuite
    for (int k=1; k<getNbItems(); k++){
        bool estDomine = false;
        int j=0;
        while(!estDomine && j<k){
            estDomine = domine(items[j],items[k]);
            if(k<getNbItems()-1){
                int l=j+1;
                while(!estDomine && l<getNbItems()){
                    estDomine = domineLP(items[j],items[k],items[l]);
                }
            }
        }
        if(estDomine) res.push_back(items[k]);
    }
    Class upperB(res);
    return upperB;
}

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
int Dataset::getNbClasses() const{ return classes.size(); }
