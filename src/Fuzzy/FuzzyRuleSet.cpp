#pragma once
#include <iostream>
#include <iomanip>
#include "FuzzyRuleSet.h"
//#include "MakeVariables.hpp"


using std::cout, std::endl;

// aan / uitzetten debugging in commandline 
bool DEBUGGING = true;

/*****************************************************/
// standaard constructor + destructor
FuzzyRuleSet::FuzzyRuleSet()
{

}

FuzzyRuleSet::~FuzzyRuleSet()
{

}


/*****************************************************/
// functies
void FuzzyRuleSet::addFuzzyRule(FuzzyRule* fuzzyRule) {
	// Toevoegen van een fuzzyRule.
	this->Rules.push_back(fuzzyRule);
}



vector<rulesetResult> FuzzyRuleSet::Calculate(double a, double b)
{
	vector<rulesetResult> result;

	for (auto rule : this->Rules) {
		if (DEBUGGING == true) {
			
			cout << "Rule " << std::left << std::setw(15) << rule->getAntecedent1()->getName()  << "(" << std::setprecision(5) << a << ", " << std::setprecision(5) << std::setw(7) << rule->getAntecedent1()->Fuzzify(a) << ")" << " \tAND "
							<< std::left << std::setw(15) << rule->getAntecedent2()->getName()  << "(" << std::setprecision(5) << b << ", " << std::setprecision(2) << std::setw(4) << rule->getAntecedent2()->Fuzzify(b) << ")" << " \t= "
							<< std::setw(5) << std::setprecision(2) << rule->Calculate(a, b) << " " << rule->getConcequent()->getName() << endl;
		}
		result.push_back(rulesetResult {
			rule->Calculate(a, b),
			rule->getConcequent(),
			rule->getAntecedent1(),
			rule->getAntecedent2()
		});
	}
		

	return result;
}
