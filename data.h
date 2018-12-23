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
		Item(double, double);
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
		vector<Item*> upperBound();
		double getMinWeight() const;
		double getMaxWeight() const;
		pair<Item*,double> mostEfficientReplacer(const Item*) const;
		Class eliminateDominatedItems();
		void deleteItem(int);
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
		vector<int> getNbItemsByClass() const;
		vector<Class*> getClasses() const;
		void affiche() const;
		double getMinWeight() const;
		double getMaxWeight() const;
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
		bool hasDominance() const;
		bool hasSwapped() const;
		double getSlope() ;
		int getClassIndex() const;
};

class IndexAllocation {
	private:
		vector<int> itemIndexes;
		Dataset* data;
	public:
		IndexAllocation(vector<int>, Dataset*);
		IndexAllocation(int, Dataset*);
		int operator[](int);
		double getWeight();
		double getValue();
		int getSize();
		Class* getClass(int);
		void increment(int);
		void reset(int);
		void affiche();
};

class Allocation {
	protected:
		vector<Item*> items;
	public:
		Allocation(vector<Item*>);
		Allocation(IndexAllocation*);
		Item* operator[](int);
		void changeItem(int, Item*);
		void affiche() const;
		double getWeight() const;
		double getValue() const;
};

class WeightedAllocation :public Allocation{
    private :
        vector<double> proportions;
	public:
		WeightedAllocation(vector<Item*>,vector<double>);
		void affiche() const;
		double getWeight() const;
		double getValue() const;
};
