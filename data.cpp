#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

#include "data.h"

using namespace std;

int Item::createdItems=0;

Item::Item(double p, double w) : value(p), weight(w)
{
    ++createdItems;
    index = createdItems;
}

Item::~Item(){}

double Item::getValue() const{ return value; }
double Item::getWeight() const{ return weight; }
int Item::getIndex()const { return index; }
int Item::getCreatedItems() { return createdItems; }
bool Item::operator==(const Item item){ return index==item.getIndex(); }
bool Item::operator!=(const Item item){ return index!=item.getIndex(); }
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
		items.push_back( new Item(values[j], weights[j]) );
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

double Class::getMinWeight()const {
	double m = items[0]->getWeight();
	int nbItems = items.size();
	if ( nbItems > 1 ) {
		for (int j = 1; j<nbItems; j++) {
			if ( items[j]->getWeight() < m ) {
				m = items[j]->getWeight();
			}
		}
	}
	return m;
}

double Class::getMaxWeight()const {
	double m = items[0]->getWeight();
	int nbItems = items.size();
	if ( nbItems > 1 ) {
		for (int j = 1; j<nbItems; j++) {
			if ( items[j]->getWeight() > m ) {
				m = items[j]->getWeight();
			}
		}
	}
	return m;
}

Class Class::eliminateDominatedItems() {
    sortItemsWeight();
    vector<Item*> res;
    res.push_back(items[0]);

    for (int k=1; k<getNbItems(); k++){
        bool estDomine = false;
        int j=0;
        while(!estDomine && j<k){
            estDomine = domine(items[j],items[k]);
            j++;
        }
        if(!estDomine) res.push_back(items[k]);

    }
    Class c(res);
    return c;
}

void Class::deleteItem(int ind) {
    items.erase(items.begin() + ind);
}

//Peut-être à reprendre, voir comment choisir si deux efficacité sont les mêmes
// returns an Item of the Class, which is the most efficient replacer, i.e argmax { [value(i)-value(it)] / [weight(i)-weight(it)])
// where i is in the vector of Items of Class, and i is different from it
pair<Item*,double> Class::mostEfficientReplacer(const Item* it) const{
    Item* res = nullptr;
    double maxEfficiency = 0; // just for the initialization, no importance of the value
    for(Item* i : getItems()){
        if((*i)!=(*it)){

            double newEfficiency = (i->getValue() - it->getValue())/(i->getWeight() - it->getWeight());
            if(res!=nullptr){ // if res already points toward an Item of the class
                if(maxEfficiency<newEfficiency){
                    maxEfficiency = newEfficiency ;
                    res=i;
                }
            }else{
                res = i;
                maxEfficiency = newEfficiency ;
            }

        }
    }
    return make_pair(res,maxEfficiency); // if res equals nullptr, then @it is alone in the class
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

Dataset::Dataset(vector< vector< vector<double> > > datasetValues) {
	// Create a dataset from a three-dimension vector.
	// For each class, there are two vectors: a vector with the values and a vector with the weights.
	int nbClasses = datasetValues.size();
	for (int i = 0; i<nbClasses; i++) {
		classes.push_back( new Class(datasetValues[i][0], datasetValues[i][1]) );
	}
}

Dataset::Dataset(vector<Class*> c) : classes(c) {}

Dataset::~Dataset() {
    for(Class* c : classes){
        delete c;
    }
}

double Dataset::getMinWeight() const{
	double sum = 0;
	for (int i = 0; i<classes.size(); i++) {
		Class* currentClass = classes[i];
		sum += currentClass->getMinWeight();
	}
	return sum;
}

double Dataset::getMaxWeight() const{
	double sum = 0;
	for (int i = 0; i<classes.size(); i++) {
		Class* currentClass = classes[i];
		sum += currentClass->getMaxWeight();
	}
	return sum;
}

Class* Dataset::operator[](int i) { return classes[i]; }
int Dataset::getNbClasses() const{ return classes.size(); }
vector<int> Dataset::getNbItemsByClass() const{ 
	vector<int> nbItemsByClass;
	for (unsigned int i = 0; i < classes.size(); i++) {
		Class* currentClass = classes[i];
		nbItemsByClass.push_back( currentClass->getNbItems() );
	}
	return nbItemsByClass;
}
void Dataset::affiche() const {
    for(unsigned int i=0; i < classes.size() ;i++) {
        cout << "Class " <<i+1 << " :" << endl;
        classes[i]->affiche();
        cout << endl ;
    }
}
vector<Class*> Dataset::getClasses() const { return classes; }

Pair::Pair(int i, Item* j, Item* k) {
	classIndex = i;
	v0 = j->getValue();
	v1 = k->getValue();
	w0 = j->getWeight();
	w1 = k->getWeight();
	dominated = false;
	swapped = false;
	if ( domine(j, k) ) {
		// j dominates k.
		dominated = true;
	} else if ( domine(k, j) ) {
		// k dominates j.
		dominated = true;
		swapped = true;
	} else if ( w0 > w1 ) {
		// Item j has more weight, reverse the pair.
		swapped = true;
	}
	// Slope is 0 if it is not computed yet.
	slope = 0;
}

bool Pair::hasDominance() const{
	return dominated;
}

bool Pair::hasSwapped() const{
	return swapped;
}

double Pair::getSlope() {
	if ( slope == 0 ) {
		// Compute slope.
		slope = (v1 - v0) / (w1 - w0);
	}
	return slope;
}

int Pair::getClassIndex() const{
	return classIndex;
}

Allocation::Allocation(vector<Item*> items) : items(items) {}

Item* Allocation::operator[](int i) {
	return items[i];
}

void Allocation::changeItem(int classIndex, Item* newItem) {
	items[classIndex] = newItem;
}

void Allocation::affiche() const{
	for(unsigned int i=0; i < items.size() ;i++) {
		cout << "Class " <<i+1 << " :" << endl;
		items[i]->affiche();
	}
}

double Allocation::getWeight() const{
	double w = 0;
	for(unsigned int i=0; i < items.size() ;i++) {
		w += items[i]->getWeight();
	}
	return w;
}

double Allocation::getValue() const{
	double v = 0;
	for(unsigned int i=0; i < items.size() ;i++) {
		v += items[i]->getValue();
	}
	return v;
}

WeightedAllocation::WeightedAllocation(vector<Item*>items,vector<double>proportions):Allocation(items),proportions(proportions){}

void WeightedAllocation::affiche() const{
	for(unsigned int i=0; i < items.size() ;i++) {
        cout << "Class " <<i+1 << " :" << endl;

        if(proportions[i]==1){
            items[i]->affiche();
        }else { // in this case, the last element of the list belongs to the same class than the current item
            if(i!=items.size()){ // allows us not to show the class of the items twice
                cout << "Item " << items[i]->getIndex() << ", poids " << items[i]->getWeight() << ", value " << items[i]->getValue() << " et proportion " << proportions[i] << endl;
                cout << "et" << endl;
                cout << "Item " << items[i]->getIndex() << ", poids " << items.back()->getWeight() << ", value " << items.back()->getValue() << " et proportion " << proportions.back() << endl;
            }
        }


	}
}

double WeightedAllocation::getWeight() const{
	double w = 0;
	for(unsigned int i=0; i < items.size() ;i++) {
		w += items[i]->getWeight() * proportions[i];
	}
	return w;
}

double WeightedAllocation::getValue() const{
	double v = 0;
	for(unsigned int i=0; i < items.size() ;i++) {
		v += items[i]->getValue() * proportions[i];
	}
	return v;
}
