#include <vector>
#include <random>

using namespace std;

class Item {
	private:
		double value;
		double weight;
		int index;
		static int createdItems;
	public:
		Item(double, double, int);
		~Item();
		double getValue()const ;
		double getWeight()const ;
		int getIndex()const ;
		static int getCreatedItems() ;
		bool operator==(const Item item);
		bool operator!=(const Item item);
        	void affiche() const;
};

class Class {
	private:
		vector<Item*> items;
	public:
		Class(vector<double>, vector<double>);
		Class(vector<Item*>);
		~Class();
		Item* operator[](int);
		int getNbItems() const;
		vector<Item*> getItems() const ;
		void sortItemsWeight();
		void affiche() const;
		Class upperBound();
		double getMinWeight();
		double getMaxWeight();
		pair<Item*,double> mostEfficientReplacer(const Item*) const;
};

class Dataset {
	private:
		vector<Class*> classes;
	public:
		Dataset(int, int, lognormal_distribution<double>, lognormal_distribution<double>, default_random_engine);
		Dataset(vector< vector< vector<double> > >);
		Dataset(vector<Class*>);
		~Dataset();
		Class* operator[](int);
		int getNbClasses() const;
		vector<Class*> getClasses() const;
		void affiche() const;
		double getMinWeight();
		double getMaxWeight();
};

class Pair {
	private:
		int classIndex;
		double v0;
		double v1;
		double w0;
		double w1;
		double slope;
		bool dominated;
		bool swapped;
	public:
		Pair(int, Item*, Item*);
		bool hasDominance();
		bool hasSwapped();
		double getSlope();
		int getClassIndex();
};

class Allocation {
	private:
		vector<Item*> items;
	public:
		Allocation(vector<Item*>);
		Item* operator[](int);
		void changeItem(int, Item*);
		void affiche();
		double getWeight();
		double getValue();
};
