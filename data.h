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
		bool operator=(const Item item);
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
};

class Dataset {
	private:
		vector<Class*> classes;
	public:
		Dataset(int, int, lognormal_distribution<double>, lognormal_distribution<double>, default_random_engine);
		~Dataset();
		Class* operator[](int);
		int getNbClasses() const;
		vector<Class*> getClasses() const;
		void affiche() const;
};

pair<double,double> MCKP_Greedy_Algorithm(Dataset d, double max_Weight);
