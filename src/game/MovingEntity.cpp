#include "MovingEntity.h"
#include "SteeringBehaviour.h"
#include <cmath>
#include <iostream>
#include <string>

using std::cout, std::endl, std::string;

MovingEntity::MovingEntity(string n, Vector2D p, World* w, Vector2D v, double m, double ms, Team team) : BaseEntity(n, p, w, {255, 255, 255}, 8, false), velocity(v), mass(m), maxSpeed(ms), goal(nullptr), resetGoal(nullptr), team(team), rocketLauncherAmmo(5), handGunAmmo(5), health(10) {}

MovingEntity::~MovingEntity() {
	this->clearSteeringBehaviours();
	for(auto shape : this->shapes) {
		delete shape;
		shape = nullptr;
	}
	this->shapes.clear();
}

double MovingEntity::getAngle() {
	Vector2D angle = this->velocity.normalize();
	//double h = asin(angle.y) + M_PI / 2;
	//if(angle.x < 0) 
	//	h = 2 * M_PI - h;
	
	double h = atan2(angle.x, angle.y);
	return h;
}

void MovingEntity::updateLines() {}

void MovingEntity::update(float delta) {
	// Update tick for goals
	this->goalLock.lock();
	if(this->goal != nullptr) {
		auto result = this->goal->Process();

		if(result > 0) {
			this->goal->Terminate();
			if(this->goal != this->resetGoal) {
				delete this->goal;
				this->goal = nullptr;
			}
			this->goal = this->resetGoal;
			if(this->goal != nullptr) {
				this->goal->Activate();
			}
		}
	}
	this->goalLock.unlock();

	// Update tick for steering
	Vector2D force = Vector2D();

	this->steeringBehavioursLock.lock();
	for(auto sb: this->sbs) {
		auto result = sb->calculate();
		force = force + result;
	}
	this->steeringBehavioursLock.unlock();


	force = this->velocity + force / this->mass * delta;
	force = force.truncate(this->getMaxSpeed()) * 0.9;
	this->position = this->position + force;
	this->velocity = force;
	this->updateLines();
}

double MovingEntity::getMaxSpeed() { 
	return this->maxSpeed;
}

Vector2D MovingEntity::getVelocity() {
	return Vector2D(this->velocity);
}
const vector<LocalizedEntity> MovingEntity::getLocalEntities() {
	auto local_entities = vector<LocalizedEntity>();
	
	for(auto entity : this->world->getEntities()) {
		local_entities.push_back(LocalizedEntity {
			this->toLocalSpace(entity->getPosition()),
			entity
		});
	}

	return local_entities;
}

Vector2D MovingEntity::toLocalSpace(Vector2D v) {
	double a = this->getAngle();
	Vector2D new_pos = v - this->position;
	return new_pos.rotate(a);
}

Vector2D MovingEntity::toWorldSpace(Vector2D v) {
	double a = this->getAngle();
	Vector2D new_pos = v.rotate(-a) + this->position;
	return new_pos;
}

void MovingEntity::setGoal(Goal* goal) {
	this->goalLock.lock();
	if(this->goal != nullptr) {
		this->goal->Terminate();
		//delete this->goal;
		this->goal = nullptr;
	}
	this->goal = goal;
	this->goal->Activate();
	this->goalLock.unlock();
}
void MovingEntity::pushSteeringBehaviour(SteeringBehaviour* sb) {
	this->steeringBehavioursLock.lock();
	this->sbs.push_back(sb);
	this->steeringBehavioursLock.unlock();
}
void MovingEntity::clearSteeringBehaviours() {
	this->steeringBehavioursLock.lock();
	for(auto sb : this->sbs) {
		delete sb;
		sb = nullptr;
	}
	this->sbs.clear();
	this->steeringBehavioursLock.unlock();
}
int MovingEntity::getRocketLauncherAmmo() {
	return this->rocketLauncherAmmo;
}
void MovingEntity::setRocketLauncherAmmo(int a) {
	this->rocketLauncherAmmo = a;
}
int MovingEntity::getHandGunAmmo() {
	return this->handGunAmmo;
}
void MovingEntity::setHandGunAmmo(int a) {
	this->handGunAmmo = a;
}
void MovingEntity::takeDamage(int a) {
	this->health -= a;
}

Team MovingEntity::getTeam() {
	return this->team;
}

Triangle::Triangle(string n, Vector2D p, World* w, Vector2D v, double m, double ms) : MovingEntity(n, p, w, v, m, ms, Team::Red) {
	this->shapes.pop_back();

	this->shapes.push_back(new Line(Vector2D(), Vector2D(), this->color));
	this->shapes.push_back(new Line(Vector2D(), Vector2D(), this->color));
	this->shapes.push_back(new Line(Vector2D(), Vector2D(), this->color));
	this->updateLines();
}

void Triangle::updateLines() {
	double h = this->getAngle();

	Vector2D p1 = Vector2D(0, -10).rotate(h) + position;
	Vector2D p2 = Vector2D(4, 10).rotate(h) + position;
	Vector2D p3 = Vector2D(-4, 10).rotate(h) + position;

	dynamic_cast<Line*>(this->shapes[0])->start = p1;
	dynamic_cast<Line*>(this->shapes[0])->end = p2;

	dynamic_cast<Line*>(this->shapes[1])->start = p2;
	dynamic_cast<Line*>(this->shapes[1])->end = p3;
	
	dynamic_cast<Line*>(this->shapes[2])->start = p3;
	dynamic_cast<Line*>(this->shapes[2])->end = p1;
}

Soldier::Soldier(SDL_Texture** t, Vector2D p, World* w, Team team) : MovingEntity("Soldier", p, w, Vector2D(), 50, 2, team), texture(t) {
	//this->sbs.push_back(new ObstacleAvoidanceBehaviour(*this, 100));
	this->resetGoal = new FlockGoal(*this);
	this->setGoal(this->resetGoal);

	this->shapes.push_back(new Sprite(this->texture, {0,0,20,15}, this->position, this->getAngle()));
}
void Soldier::updateLines() {
	dynamic_cast<Sprite*>(this->shapes[0])->setPosition(this->position);
	dynamic_cast<Sprite*>(this->shapes[0])->setAngle(this->getAngle());
}

Commander::Commander(SDL_Texture** t, Vector2D p, World* w, Team team) : MovingEntity("Commander", p, w, Vector2D(), 50, 1.2, team), texture(t) {
	//this->sbs.push_back(new ObstacleAvoidanceBehaviour(*this, 100));
	if(team == Team::Blue)
		this->resetGoal = new BlueThink(*this);
	else
		this->resetGoal = new RedThink(*this);

	this->setGoal(this->resetGoal);

	this->shapes.push_back(new Sprite(this->texture, {0,0,20,15}, this->position, this->getAngle()));
	this->goalLock.lock();
	this->shapes.push_back(new Text(this->goal->getName().c_str(), w->getFont(), {255, 255, 255}, this->position));
	this->goalLock.unlock();
}
void Commander::updateLines() {
	dynamic_cast<Sprite*>(this->shapes[0])->setPosition(this->position);
	dynamic_cast<Sprite*>(this->shapes[0])->setAngle(this->getAngle());
	dynamic_cast<Text*>(this->shapes[1])->setPosition(this->position + Vector2D(15, -15));
	this->goalLock.lock();
	if(this->goal != nullptr)
		dynamic_cast<Text*>(this->shapes[1])->setText(this->goal->getName().c_str());
	this->goalLock.unlock();
}

