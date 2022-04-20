#include <cassert>
#include <iostream>
#include <iomanip>
#include "FuzzyVariable.h"
#include "FuzzyRule.h"
#include "FuzzyRuleSet.h"
#include "FuzzySet.h"

using std::cout, std::endl;

// globale Bool voor aan/ uitzetten van de cout's in het project.
//static bool DEBUGGING = true;


void MakeVariables() {
	// In deze functie worden de variabelen aangemaakt.


// Aanmaken van de FuzzyVariables 
	FuzzyVariable DistanceClose = FuzzyVariable("DistanceClose", 0.00001, (1 - 0.00025), -1.0 / 125, 1.2, FuzzyVariableShape::LeftShoulder);
	FuzzyVariable DistanceMedium = FuzzyVariable("DistanceMedium", 1.2 / 150, -0.2, -1.0 / 150, 2, FuzzyVariableShape::Triangle);
	FuzzyVariable DistanceFar = FuzzyVariable("DistanceFar", 1.0 / 150, -1, 0, 1, FuzzyVariableShape::RightShoulder);
	FuzzyVariable AmmoLow = FuzzyVariable("AmmoLow", 0, 1.00001, -0.1, 1, FuzzyVariableShape::LeftShoulder);
	FuzzyVariable AmmoOkay = FuzzyVariable("AmmoOkay", 0.1, 0, -0.05, 1.5, FuzzyVariableShape::Triangle);
	FuzzyVariable AmmoLoads = FuzzyVariable("AmmoLoads", 0.05, -0.5, 0, 1, FuzzyVariableShape::RightShoulder);
	FuzzyVariable Undesirable = FuzzyVariable("Undesirable", 0.00001, (1 - 0.00025), -0.04, 2, FuzzyVariableShape::LeftShoulder);
	FuzzyVariable Desirable = FuzzyVariable("Desirable", 0.04, -1, -0.04, 3, FuzzyVariableShape::Triangle);
	FuzzyVariable Verydesirable = FuzzyVariable("Verydesirable", 0.04, -2, 0, 1, FuzzyVariableShape::RightShoulder);

	// Aanmaken van de Fuzzyrules
	FuzzyRule rule1 = FuzzyRule(&DistanceFar, &AmmoLoads, &Desirable, FuzzyOperator::AND);
	FuzzyRule rule2 = FuzzyRule(&DistanceFar, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule3 = FuzzyRule(&DistanceFar, &AmmoLow, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule4 = FuzzyRule(&DistanceMedium, &AmmoLoads, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule rule5 = FuzzyRule(&DistanceMedium, &AmmoOkay, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule rule6 = FuzzyRule(&DistanceMedium, &AmmoLow, &Desirable, FuzzyOperator::AND);
	FuzzyRule rule7 = FuzzyRule(&DistanceClose, &AmmoLoads, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule8 = FuzzyRule(&DistanceClose, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule9 = FuzzyRule(&DistanceClose, &AmmoLow, &Undesirable, FuzzyOperator::AND);

	// Aanmaken van de FuzzyruleSet door alle Fuzzyrules toe te voegen.
	FuzzyRuleSet set;
	set.addFuzzyRule(&rule1);
	set.addFuzzyRule(&rule2);
	set.addFuzzyRule(&rule3);
	set.addFuzzyRule(&rule4);
	set.addFuzzyRule(&rule5);
	set.addFuzzyRule(&rule6);
	set.addFuzzyRule(&rule7);
	set.addFuzzyRule(&rule8);
	set.addFuzzyRule(&rule9);

	// Uitrekenen van de FuzzyruleSet
	auto CalcResult = set.Calculate(200, 8);		// set.Calculate(distance, ammo )
	auto resultSet = FuzzySet(100);					// FuzzySet(max witdh);
	for (auto result : CalcResult) {
		resultSet.Add(result.Concequent, result.DOM);
		//if (DEBUGGING = true) {
		//	cout << result.Concequent->getName() << ": " << result.Concequent->getCenter(100) << endl;
		//}
	}

	double result = resultSet.getMaxAV();
	cout <<  "Result getMaxAv : " << result << endl;
}


