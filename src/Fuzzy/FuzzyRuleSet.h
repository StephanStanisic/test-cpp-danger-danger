#pragma once
#include <vector>
#include "FuzzyRule.h"


using std::vector;

struct rulesetResult{

	double DOM;						// degree of membership
	FuzzyVariable* Concequent;
	FuzzyVariable* Antecedent1;
	FuzzyVariable* Antecedent2;
};

class FuzzyRuleSet
{
private:
	vector<FuzzyRule*> Rules;

public:
	//standaard constructor + destructor
	FuzzyRuleSet();
	~FuzzyRuleSet();

	void addFuzzyRule(FuzzyRule* fuzzyRule);

	// functies
	vector<rulesetResult> Calculate(double a, double b);
};
