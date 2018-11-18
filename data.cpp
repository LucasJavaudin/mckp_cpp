#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

#include "data.h"

using namespace std;

int Item::createdItems=0;

Item::Item(double p, double w, int i) : value(p), weight(w), index(i)
{
    ++createdItems;
    index = createdItems;
}

Item::~Item(){}

double Item::getValue() const{ return value; }
double Item::getWeight() const{ return weight; }
int Item::getIndex()const { return index; }
int Item::getCreatedItems() { return createdItems; }
bool Item::operator=(const Item item){ return index==item.getIndex(); }
void Item::affiche() const { cout << "Item " << index << ", poids " << weight << ", value " << value <<endl;}

// Si les poids sont égaux, renvoie true si la valeur du premier est supérieure à celle du deuxième
struct compareWeight{
bool operator ()(const Item * it1,const Item * it2) const // à revérifier
{
    if (it1 != NULL && it2 != NULL){
        if(it1->getWeight()!=it2->getWeight()) return it1->getWeight() < it2->getWeight();
        else return it1->getValue() > it2->getValue();
    }else{
        return false;
    }
}
};

// La fonction renvoie true si @it1 domine @it2, false sinon
bool domine(Item* it1, Item* it2){
    return(it1->getWeight()<=it2->getWeight() && it1->getValue()>=it2->getValue());
}

// La fonction renvoie true si @it1 et @it3 LP-dominent @it2
bool domineLP(Item* it1, Item* it2, Item* it3){
    if (it1->getWeight()<it2->getWeight() && it2->getWeight()<it3->getWeight() &&
        it1->getValue()<it2->getValue() && it2->getValue()<it3->getValue()){
        // les deux premières conditions sont w1 < w2 < w3 (poids)
        // la deuxième : p1 < p2 < p3 (valeurs)
        double terme1 = ( it3->getValue() - it2->getValue() ) / ( it3->getWeight() - it2->getWeight() );
        double terme2 = ( it2->getValue() - it1->getValue() ) / ( it2->getWeight() - it1->getWeight() );
        return(terme1>=terme2);
    }else return false;
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

Class::~Class() {
    for(Item* it : items){
        delete it; // supprime l'item pointé par it, libère la mémoire
    }
}

Item* Class::operator[](int i) { return items[i]; }
int Class::getNbItems() const { return items.size(); }
vector<Item*> Class::getItems() const { return items; }
void Class::sortItemsWeight() { sort (items.begin(), items.end(), compareWeight()); }
void Class::affiche() const { for(Item* it : items) it->affiche(); }

Class Class::upperBound(){
    // construit l'enveloppe convexe de la classe
    sortItemsWeight(); // on commence par trier par ordre croissant les items selon leur poids
    vector<Item*> res;
    res.push_back(items[0]); // je suppose que weight[0] est strictement plus petit pour l'instant, à voir ensuite

    for (int k=1; k<getNbItems(); k++){
        bool estDomine = false;
        int j=0;
        while(!estDomine && j<k){
            estDomine = domine(items[j],items[k]);

            if(k<getNbItems()-1){
                int l=k+1;
                while(!estDomine && l<getNbItems()){
                    estDomine = domineLP(items[j],items[k],items[l]);
                    l++;
                }
            }
            j++;
        }
        if(!estDomine) res.push_back(items[k]);
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

Dataset::~Dataset() {
    for(Class* c : classes){
        delete c;
    }
}

Class* Dataset::operator[](int i) { return classes[i]; }
int Dataset::getNbClasses() const{ return classes.size(); }
void Dataset::affiche() const {
    for(unsigned int i=0; i < classes.size() ;i++) {
        cout << "Classe " <<i+1 << " :" << endl;
        classes[i]->affiche();
        cout << endl ;
    }
}
vector<Class*> Dataset::getClasses() const { return classes; }
