#include <random>
#include <chrono>

#include "data.h"

using namespace std;

int main() {
	// Create a generator for the random numbers.
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	// Distribution for the alternatives' value.
	lognormal_distribution<double> value_distribution(0.0, 1.0);
	// Distribution for the alternatives' weight.
	lognormal_distribution<double> weight_distribution(0.0, 1.0);

	// Create dataset.
	Dataset data(1000, 10, value_distribution, weight_distribution, generator);
}
