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
		double getValue();
		double getWeight();
		int getIndex();
};

class Class {
	private:
		vector<Item*> items;
	public:
		Class(vector<double>, vector<double>);
		Item* operator[](int);
		int getNbItems();
};

class Dataset {
	private:
		vector<Class*> classes;
	public:
		Dataset(int, int, lognormal_distribution<double>, lognormal_distribution<double>, default_random_engine);
		Class* operator[](int);
		int getNbClasses();
};
