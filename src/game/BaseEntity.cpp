#include "BaseEntity.h"
#include <SDL.h>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "../util/VectorMath.hpp"

using std::cout, std::endl, std::invalid_argument, std::string;

BaseEntity::BaseEntity(string n, Vector2D p, World* w) : BaseEntity(n, p, w, {255, 255, 255}, 8, true) {}
BaseEntity::BaseEntity(string n, Vector2D p, World* w, Color c, double r, bool s) : position(p), world(w), color(c), radius(r), solid(s), name(n) {}

// Rule of three
BaseEntity::BaseEntity(BaseEntity& b) : BaseEntity(b.name, b.position, b.world, b.color, b.radius, b.solid) {}
BaseEntity& BaseEntity::operator=(const BaseEntity& b) {
	this->position = b.position;
	this->color = b.color;
	this->radius = b.radius;
	this->solid = b.solid;

	if(&(this->world) != &(b.world)) {
		// This is the end, this is the end.
		throw invalid_argument("Cannot use assignment operator on Entities from different worlds. Deallocate and use the copy constructor instead.");
	}

	return *this;
}
BaseEntity::~BaseEntity() {
	for(auto shape: this->shapes) {
		delete shape;
		shape = nullptr;
	}
};

void BaseEntity::update(float delta) {}

const vector<Shape*> BaseEntity::render() {
	return this->shapes;
}

void BaseEntity::setPosition(int x, int y) {
	this->position.x = x;
	this->position.y = y;

	dynamic_cast<Circle*>(this->shapes[0])->setCenter(this->position);
}

void BaseEntity::setPosition(const Vector2D pos) {
	return setPosition(pos.x, pos.y);
}

Vector2D& BaseEntity::getPosition() {
	return this->position;
}

void BaseEntity::setColor(Color c) {
	this->color = c;
	dynamic_cast<Circle*>(this->shapes[0])->setColor(this->color);
}

World* BaseEntity::getWorld() {
	return this->world;
}

double BaseEntity::getRadius() {
	return this->radius;
}

bool BaseEntity::isSolid() {
	return this->solid;
}

string BaseEntity::getName() {
	return this->name;
}

const vector<LocalizedEntity> BaseEntity::getLocalEntities() {
	auto local_entities = vector<LocalizedEntity>();

	for(auto entity : this->world->getEntities()) {
		local_entities.push_back(LocalizedEntity {
			entity->getPosition() - this->position,
			entity
		});
	}

	return local_entities;
}

void BaseEntity::addShape(Shape *s) {
	cout << "Shapes total: " << shapes.size() << endl;
	this->shapes.push_back(s);
	cout << "Added shape " << s << endl;
}

void BaseEntity::removeShape(Shape *s) {
	this->shapes.erase(std::remove(this->shapes.begin(), this->shapes.end(), s), this->shapes.end());
}

PointerEntity::PointerEntity(Vector2D p, World* w) : BaseEntity("POINTER", p, w, {255, 255, 0}, 8, false) {
	this->shapes.push_back(new Circle(this->position, this->radius, this->color));
}
WallEntity::WallEntity(string n, Vector2D p, World* w): BaseEntity(n, p, w, {0, 255, 0}, 48, true) {}
