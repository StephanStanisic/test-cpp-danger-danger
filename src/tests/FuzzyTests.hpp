#include <cassert>
#include <iostream>
#include "../Fuzzy/FuzzyVariable.h"
#include "../Fuzzy/FuzzyRule.h"
#include "../Fuzzy/FuzzyRuleSet.h"
#include "../Fuzzy/FuzzySet.h"

using std::cout, std::endl;

void run_Fuzzy_tests() {


	//startA = (1.2/150);
	//startb = -0.2;

	//endA = -(1 / 150);
	//endB = 2;

	FuzzyVariable testMedium = FuzzyVariable("testMedium", 1.2 / 150, -0.2, -1.0 / 150.0, 2, FuzzyVariableShape::Triangle); // medium
	FuzzyVariable testAmmoStatOk = FuzzyVariable("testAmmoStatOk", 0.1, 0, -0.05, 1.5, FuzzyVariableShape::Triangle);	//okay
	FuzzyVariable testdesireability = FuzzyVariable("testdesireability", 0.02, -2, 0, 1, FuzzyVariableShape::RightShoulder);	// very

	/*aanmaken van de FuzzyVariables */
	FuzzyVariable DistanceClose		= FuzzyVariable("DistanceClose", 0.00001, (1 - 0.00025), -1.0 / 125, 1.2, FuzzyVariableShape::LeftShoulder);
	FuzzyVariable DistanceMedium	= FuzzyVariable("DistanceMedium", 1.2/150, -0.2, -1.0 / 150, 2, FuzzyVariableShape::Triangle);
	FuzzyVariable DistanceFar		= FuzzyVariable("DistanceFar", 1.0/150, -1, 0, 1, FuzzyVariableShape::RightShoulder);

	FuzzyVariable AmmoLow	= FuzzyVariable("AmmoLow", 0, 1.00001, -0.1, 1, FuzzyVariableShape::LeftShoulder);
	FuzzyVariable AmmoOkay = FuzzyVariable("AmmoOkay", 0.1, 0, -0.05, 1.5, FuzzyVariableShape::Triangle);
	FuzzyVariable AmmoLoads = FuzzyVariable("AmmoLoads", 0.05, -0.5, 0, 1, FuzzyVariableShape::RightShoulder);

	FuzzyVariable Undesirable = FuzzyVariable("Undesirable", 0.00001, (1 - 0.00025), -0.04, 2, FuzzyVariableShape::LeftShoulder);
	FuzzyVariable Desirable		= FuzzyVariable("Desirable", 0.04, -1, -0.04, 3, FuzzyVariableShape::Triangle);
	FuzzyVariable Verydesirable	= FuzzyVariable("Verydesirable", 0.04, -2, 0, 1, FuzzyVariableShape::RightShoulder);


	/*aanmaken van Fuzzyrules*/
	FuzzyRule rule1 = FuzzyRule(&DistanceFar, &AmmoLoads, &Desirable, FuzzyOperator::AND);
	FuzzyRule rule2 = FuzzyRule(&DistanceFar, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule3 = FuzzyRule(&DistanceFar, &AmmoLow, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule4 = FuzzyRule(&DistanceMedium, &AmmoLoads, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule rule5 = FuzzyRule(&DistanceMedium, &AmmoOkay, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule rule6 = FuzzyRule(&DistanceMedium, &AmmoLow, &Desirable, FuzzyOperator::AND);
	FuzzyRule rule7 = FuzzyRule(&DistanceClose, &AmmoLoads, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule8 = FuzzyRule(&DistanceClose, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rule9 = FuzzyRule(&DistanceClose, &AmmoLow, &Undesirable, FuzzyOperator::AND);

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

	auto aa = set.Calculate(200, 8);
	auto resultSet = FuzzySet(100);
	for (auto result : aa) {
		resultSet.Add(result.Concequent, result.DOM);
		cout << result.Concequent->getName() << ": " << result.Concequent->getCenter(100) << endl;
	}

	auto result = resultSet.getMaxAV();
	cout << "???? " << result << endl;

	auto defuzzy1 = testMedium.DeFuzzify(0.5);
	cout << defuzzy1[0] << "  " << defuzzy1[1] << endl;
	cout << testMedium.Fuzzify(100) << endl;

	cout << testMedium.Fuzzify(250) << endl;
	auto defuzzy2 = testMedium.DeFuzzify(0.7);
	cout << defuzzy2[0] << "  " << defuzzy2[1] << endl;


	FuzzyRule ruletest = FuzzyRule(&testMedium, &testAmmoStatOk, &testdesireability, FuzzyOperator::AND);

	cout << "fuzzyfymedium (100)" << testMedium.Fuzzify(100) << endl;
	cout << "testammostat ok (15)" << testAmmoStatOk.Fuzzify(15) << endl;
	cout << ruletest.Calculate(100, 15) << endl;



}





