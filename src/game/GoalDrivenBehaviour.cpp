#include "GoalDrivenBehaviour.h"
#include "../util/instanceof.hpp"
#include "../Fuzzy/FuzzyVariable.h"
#include "../Fuzzy/FuzzyRule.h"
#include "../Fuzzy/FuzzyRuleSet.h"
#include "../Fuzzy/FuzzySet.h"

/********** Goal **********/

Goal::Goal(string name) : name(name) {}

/********** AtomicGoal **********/

AtomicGoal::AtomicGoal(string name, MovingEntity& entity) : Goal(name), entity(entity) {}
string AtomicGoal::getName() {
	return this->name;
}

/********** CompositeGoal **********/

CompositeGoal::CompositeGoal(string name) : Goal(name) {}
CompositeGoal::~CompositeGoal() {
	this->subgoalsLock.lock();
	for(auto goal : this->subgoals) {
		delete goal;
	}
	this->subgoalsLock.unlock();
}

void CompositeGoal::Activate() {
	this->subgoalsLock.lock();
	if(this->subgoals.size() > 0) {
		this->subgoals.back()->Activate();
	}
	this->subgoalsLock.unlock();
}

int CompositeGoal::Process() {
	this->subgoalsLock.lock();
	if(this->subgoals.size() == 0) {
		// We're empty, commit sudoku
		this->subgoalsLock.unlock();
		return 1;
	}

	auto goal = this->subgoals.back();
	auto result = goal->Process();

	if(result > 0) {
		goal->Terminate();
		delete goal;
		this->subgoals.pop_back();

		if(this->subgoals.size() > 0) {
			this->subgoals.back()->Activate();
		}
	}

	this->subgoalsLock.unlock();
	return 0;
}

void CompositeGoal::Terminate() {
	this->subgoalsLock.lock();
	if(this->subgoals.size() > 0) {
		this->subgoals.back()->Terminate();
	}
	this->subgoalsLock.unlock();
}

void CompositeGoal::AddSubGoal(Goal* g) {
	this->subgoalsLock.lock();
	this->subgoals.push_back(g);
	this->subgoalsLock.unlock();
}

string CompositeGoal::getName() {
	this->subgoalsLock.lock();
	string name = this->name + "\n";
	if(this->subgoals.size() > 0)
		name += this->subgoals.back()->getName();
	this->subgoalsLock.unlock();
	return name;
}

/********** SeekGoal **********/

SeekGoal::SeekGoal(MovingEntity& entity, Vector2D seek_pos) : AtomicGoal("Seek", entity), seek_pos(seek_pos) {}

void SeekGoal::Activate() {
	this->entity.pushSteeringBehaviour(new SeekBehaviour(this->entity, this->seek_pos));
	this->entity.pushSteeringBehaviour(new ArriveBehaviour(this->entity, this->seek_pos));
}

int SeekGoal::Process() {
	if((this->seek_pos - this->entity.getPosition()).length() < 10) {
		// We're done here.
		return 1;
	}
	return 0;
}

void SeekGoal::Terminate() {
	this->entity.clearSteeringBehaviours();
}


/************* FlockGoal **************/

FlockGoal::FlockGoal(MovingEntity& entity) : AtomicGoal("Flock", entity) {}

void FlockGoal::Activate() {
	this->entity.pushSteeringBehaviour(new FlockingBehaviour(this->entity, 100));
}

int FlockGoal::Process() {
	return 0;
}

void FlockGoal::Terminate() {
	this->entity.clearSteeringBehaviours();
}

/************* PatrolGoal *************/

RedPatrolGoal::RedPatrolGoal(MovingEntity& entity) : CompositeGoal("Red Patrol") {
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-50, 50)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-140, 120)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-120, 280)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-320, 220)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-360, 0)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-220, -100)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-320, -220)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(-100, -220)));
}
BluePatrolGoal::BluePatrolGoal(MovingEntity& entity) : CompositeGoal("Blue Patrol") {
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(90, -210)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(290, -220)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(320, -25)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(320, 250)));
	this->AddSubGoal(new ShortestPathGoal(entity, Vector2D(180, 210)));
}

/************* FollowPathGoal *************/

FollowPathGoal::FollowPathGoal(MovingEntity& entity, vector<Vector2D> points) : CompositeGoal("Follow Path") {
	for(auto vec : points) {
		this->AddSubGoal(new SeekGoal(entity, vec));
	}
}


/************** ShortestPathGoal ************/

ShortestPathGoal::ShortestPathGoal(MovingEntity& entity, Vector2D to) : AtomicGoal("Shortest Path", entity), to(to), followPathGoal(nullptr) {}
ShortestPathGoal::~ShortestPathGoal() {
	this->followPathGoalLock.lock();
	if(this->followPathGoal != nullptr)
		delete this->followPathGoal;
	this->followPathGoalLock.unlock();
}

void ShortestPathGoal::Activate() {
	this->followPathGoalLock.lock();
	if(this->followPathGoal != nullptr)
		delete this->followPathGoal;

    auto shortest_path = entity.getWorld()->shortestPath(entity.getPosition(), to);
	this->followPathGoal = new FollowPathGoal(entity, shortest_path);
	this->followPathGoal->Activate();
	this->followPathGoalLock.unlock();
}
int ShortestPathGoal::Process() {
	this->followPathGoalLock.lock();
	auto result = this->followPathGoal->Process();
	this->followPathGoalLock.unlock();
	return result;
}
void ShortestPathGoal::Terminate() {
	this->followPathGoalLock.lock();
	this->followPathGoal->Terminate();
	this->followPathGoalLock.unlock();
}

string ShortestPathGoal::getName() {
	this->followPathGoalLock.lock();
	string a = "Shortest Path\n" + this->followPathGoal->getName();
	this->followPathGoalLock.unlock();
	return a;
}

/******** Think *************/

RedThink::RedThink(MovingEntity& entity) : AtomicGoal("Red Think", entity), goal(nullptr) {}
RedThink::~RedThink() {
	if(this->goal != nullptr)
		delete this->goal;
}

void RedThink::Activate() {
	if(this->goal != nullptr)
		delete this->goal;

	this->goal = new RedPatrolGoal(entity);
	this->goal->Activate();
}

int RedThink::Process() {
	auto result = this->goal->Process();
	if(result == 0)
		return 0;

	this->goal->Terminate();
	delete this->goal;
	int rrr = rand() % 2;
	cout << rrr << endl;
	if(rrr == 1) {
		this->goal = new ShootyShootyPewPew(entity);
	} else {
		this->goal = new RedPatrolGoal(entity);
	}
	this->goal->Activate();

	return 0;
}

void RedThink::Terminate() {
	
}

string RedThink::getName() {
	return "Red Think\n" + this->goal->getName(); 
}

BlueThink::BlueThink(MovingEntity& entity) : AtomicGoal("Blue Think", entity), goal(nullptr) {}
BlueThink::~BlueThink() {
	if(this->goal != nullptr)
		delete this->goal;
}

void BlueThink::Activate() {
	if(this->goal != nullptr)
		delete this->goal;

	this->goal = new BluePatrolGoal(entity);
	this->goal->Activate();
}

int BlueThink::Process() {
	auto result = this->goal->Process();
	if(result == 0)
		return 0;

	this->goal->Terminate();
	delete this->goal;
	int rrr = rand() % 2;
	cout << rrr << endl;
	if(rrr == 1) {
		this->goal = new ShootyShootyPewPew(entity);
	} else {
		this->goal = new BluePatrolGoal(entity);
	}
	this->goal->Activate();

	return 0;
}

void BlueThink::Terminate() {
	
}

string BlueThink::getName() {
	return "Blue Think\n" + this->goal->getName(); 
}

/******** Pew **********/

ShootyShootyPewPew::ShootyShootyPewPew(MovingEntity& entity) : CompositeGoal("ShootyShooty") {
	// Find first enemy in the list
	
	MovingEntity* enemy = nullptr;
	for(auto tmp_e : entity.getWorld()->getEntities()) {
		
		if(!instanceof<MovingEntity>(tmp_e.get())) 
			continue;

		MovingEntity* e = dynamic_cast<MovingEntity*>(tmp_e.get());

		if(e->getTeam() == entity.getTeam())
			continue;

		enemy = e;
		break;
	}

	if(enemy != nullptr) {

		// Path over there
		this->AddSubGoal(new ShortestPathGoal(entity, enemy->getPosition()));
		// Try to take a shot
		this->AddSubGoal(new TakeShotGoal(entity, *enemy));

	}
}

/************* TakeShotGoal ***************/

TakeShotGoal::TakeShotGoal(MovingEntity& entity, MovingEntity& enemy) : AtomicGoal("TakeShotGoal", entity), enemy(enemy), ticks(0), lazor(nullptr) {
	
}

void TakeShotGoal::Activate() {
	// Fuzzy fuzz
	double distance = (enemy.getPosition() - entity.getPosition()).length();
	double rocketLauncherAmmo = entity.getRocketLauncherAmmo();
	double handGumAmmo = entity.getHandGunAmmo();
	
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
	FuzzyRule rocketLauncher_rule1 = FuzzyRule(&DistanceFar, &AmmoLoads, &Desirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule2 = FuzzyRule(&DistanceFar, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule3 = FuzzyRule(&DistanceFar, &AmmoLow, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule4 = FuzzyRule(&DistanceMedium, &AmmoLoads, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule5 = FuzzyRule(&DistanceMedium, &AmmoOkay, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule6 = FuzzyRule(&DistanceMedium, &AmmoLow, &Desirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule7 = FuzzyRule(&DistanceClose, &AmmoLoads, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule8 = FuzzyRule(&DistanceClose, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule rocketLauncher_rule9 = FuzzyRule(&DistanceClose, &AmmoLow, &Undesirable, FuzzyOperator::AND);

	FuzzyRuleSet rocketLauncherSet;
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule1);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule2);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule3);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule4);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule5);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule6);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule7);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule8);
	rocketLauncherSet.addFuzzyRule(&rocketLauncher_rule9);

	auto matrix = rocketLauncherSet.Calculate(distance, rocketLauncherAmmo);
	auto resultSet = FuzzySet(100);
	for (auto result : matrix) {
		resultSet.Add(result.Concequent, result.DOM);
	}


	/*aanmaken van Fuzzyrules*/
	FuzzyRule handGun_rule1 = FuzzyRule(&DistanceFar, &AmmoLoads, &Desirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule2 = FuzzyRule(&DistanceFar, &AmmoOkay, &Undesirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule3 = FuzzyRule(&DistanceFar, &AmmoLow, &Undesirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule4 = FuzzyRule(&DistanceMedium, &AmmoLoads, &Desirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule5 = FuzzyRule(&DistanceMedium, &AmmoOkay, &Desirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule6 = FuzzyRule(&DistanceMedium, &AmmoLow, &Undesirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule7 = FuzzyRule(&DistanceClose, &AmmoLoads, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule8 = FuzzyRule(&DistanceClose, &AmmoOkay, &Verydesirable, FuzzyOperator::AND);
	FuzzyRule handGun_rule9 = FuzzyRule(&DistanceClose, &AmmoLow, &Desirable, FuzzyOperator::AND);

	FuzzyRuleSet handGunSet;
	handGunSet.addFuzzyRule(&handGun_rule1);
	handGunSet.addFuzzyRule(&handGun_rule2);
	handGunSet.addFuzzyRule(&handGun_rule3);
	handGunSet.addFuzzyRule(&handGun_rule4);
	handGunSet.addFuzzyRule(&handGun_rule5);
	handGunSet.addFuzzyRule(&handGun_rule6);
	handGunSet.addFuzzyRule(&handGun_rule7);
	handGunSet.addFuzzyRule(&handGun_rule8);
	handGunSet.addFuzzyRule(&handGun_rule9);

	auto handGunMatrix = handGunSet.Calculate(distance, handGumAmmo);
	auto handGunResultSet = FuzzySet(100);
	for (auto result : handGunMatrix) {
		handGunResultSet.Add(result.Concequent, result.DOM);
	}


	auto handGun_result = handGunResultSet.getMaxAV();
	auto rocketLauncher_result = resultSet.getMaxAV();

	cout << "Hand gun: " << handGun_result << ", rocketLauncher: " << rocketLauncher_result << endl;

	if(handGun_result > rocketLauncher_result) {
		// use handgun
		// add lazor line
		this->lazor = new Line(entity.getPosition(), enemy.getPosition(), {255, 0, 0});
		// remove ammo
		this->enemy.setHandGunAmmo(handGumAmmo - 1);
		// remove health
		this->enemy.takeDamage(1);
	} else {
		// use rockets
		// add lazor line
		this->lazor = new Line(entity.getPosition(), enemy.getPosition(), {255, 255, 255});
		// remove ammo
		this->enemy.setRocketLauncherAmmo(rocketLauncherAmmo - 1);
		// remove health
		this->enemy.takeDamage(2);
	}
	
	cout << this->lazor << endl;
}

int TakeShotGoal::Process() {
	// Update lazor line
	if(this->lazor != nullptr) {
		this->lazor->start = this->entity.getPosition();
		this->lazor->end = this->enemy.getPosition();
	}

	if(this->ticks == 1) {
		entity.addShape(this->lazor);
	}

	// return 1 after n frames
	return this->ticks++ > 100;
}

void TakeShotGoal::Terminate() {
	// remove lazor line
	entity.removeShape(this->lazor);
	//delete this->lazor;
	//this->lazor = nullptr;
}

string TakeShotGoal::getName() {
	return this->name;
}
