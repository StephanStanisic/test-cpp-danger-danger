#pragma once

#include <vector>
#include "FuzzyVariable.h"
#include "FuzzyRule.h"

using std::vector;

class FuzzySet
{
private:
	double maxWidth;

public:
	vector<FuzzyVariable*> Variables;
	vector<double> MaxValues;

	//standaard constructor + destructor
	FuzzySet(double maxWidth);
	~FuzzySet();

	// functies
	void Add(FuzzyVariable* a, double value);
	double getMaxAV();
};
