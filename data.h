#include <vector>
#include <random>

using namespace std;

class Item {
	private:
		double value;
		double weight;
		int index;
	public:
		Item(double, double, int);
		double getValue()const ;
		double getWeight()const ;
		int getIndex()const ;

};

class Class {
	private:
		vector<Item*> items;
	public:
		Class(vector<double>, vector<double>);
		Class(vector<Item*>);
		Item* operator[](int);
		int getNbItems() const;
		void sortItemsWeight();
		Class upperBound();
};

class Dataset {
	private:
		vector<Class*> classes;
	public:
		Dataset(int, int, lognormal_distribution<double>, lognormal_distribution<double>, default_random_engine);
		Class* operator[](int);
		int getNbClasses() const;
};
