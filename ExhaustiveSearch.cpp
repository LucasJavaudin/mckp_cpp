#include <vector>
#include <algorithm>
#include <iostream>
#include <float.h>

#include "data.h"

#define VERBOSE
// #define ULTRA_VERBOSE

using namespace std;

Allocation ExhaustiveSearch(Dataset* data, double capacity) {
#ifdef VERBOSE
	cout << "Initialization..." << endl;
#endif
	vector<int> nbItemsByClass = data->getNbItemsByClass();
	IndexAllocation allocation(0, data);
	int j;

	double bestValue = -DBL_MAX;
	double bestWeight = 0;
	IndexAllocation bestAllocation = allocation;

	double w = allocation.getWeight();
	if ( w <= capacity ) {
		// First allocation is a valid allocation.
		bestValue = allocation.getValue();
		bestWeight = w;
	}

	// Cycle through all allocations.
#ifdef VERBOSE
	cout << "Starting main loop..." << endl;
#endif
	do {
#ifdef ULTRA_VERBOSE
	cout << "Looking for next allocation..." << endl;
#endif
		j = 0;
		// Find a new allocation.
		while ( true ) {
			// Increase index of class j by 1.
			allocation.increment(j);
			if ( allocation[j] == nbItemsByClass[j] ) {
				// Reached last item in class j.
				allocation.reset(j);
				j++;
			} else {
				break;
			}
		}
#ifdef ULTRA_VERBOSE
	cout << "New allocation is" << endl;
	allocation.affiche();
#endif

		// Check if allocation is the best allocation.
		double w = allocation.getWeight();
		if ( w <= capacity ) {
			double v = allocation.getValue();
			if ( v > bestValue ) {
				// New best allocation.
#ifdef ULTRA_VERBOSE
	cout << "Found new best allocation" << endl;
#endif
				bestValue = v;
				bestWeight = w;
				bestAllocation = allocation;
			}
		}

	} while ( j < data->getNbClasses() );

	return Allocation(&bestAllocation);
}
