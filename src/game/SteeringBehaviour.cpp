#include "../util/Vector2D.h"
#include "../util/debug.hpp"
#include "../util/instanceof.hpp"
#include "SteeringBehaviour.h"
#include <cmath>
#include <iostream>
#include <limits>

using std::cout, std::endl, std::numeric_limits;

SteeringBehaviour::SteeringBehaviour(MovingEntity& me) : entity(me) {}
Vector2D SteeringBehaviour::calculate() {
	return Vector2D();
}

SeekBehaviour::SeekBehaviour(MovingEntity& me, Vector2D& seek_pos) : SteeringBehaviour(me), seek_pos(seek_pos) {}
Vector2D SeekBehaviour::calculate() {
	Vector2D pos = entity.getPosition();
	Vector2D force = (this->seek_pos - pos);

	return force - entity.getVelocity();
}

ArriveBehaviour::ArriveBehaviour(MovingEntity& me, Vector2D& seek_pos) : SteeringBehaviour(me), seek_pos(seek_pos) {}
Vector2D ArriveBehaviour::calculate() {
	Vector2D pos = entity.getPosition();
	double length = (this->seek_pos - pos).length();
	Vector2D force = (this->seek_pos - pos) * (1 - length / 200) * -1 / 2;

	return force - entity.getVelocity();
}

ObstacleAvoidanceBehaviour::ObstacleAvoidanceBehaviour(MovingEntity& me, double detection_radius) : SteeringBehaviour(me), detection_radius(detection_radius) {}
Vector2D ObstacleAvoidanceBehaviour::calculate() {
	Vector2D pos = entity.getPosition();
	auto entities = entity.getLocalEntities();
	shared_ptr<BaseEntity> closest;
	double closest_intersection = numeric_limits<double>::max();
	Vector2D closest_position = Vector2D(); 

	for(auto e : entities) {
		// Filter out other stuffs
		if(instanceof<PointerEntity>(e.entity.get())) 
			continue;
		if(instanceof<Soldier>(e.entity.get())) 
			continue;

		e.entity->setColor({80, 80, 80});
		// If it's outside the radius, ignore it
		if(e.local_position.length() > this->detection_radius) 
			continue;

		// If it's behind us, ignore it
		if(e.local_position.y < 0) 
			continue;

		// If it's outside the bounding area, ignore it.
		if((e.local_position.x - e.entity->getRadius() >  entity.getRadius() || 
			//e.local_position.x - __OBSTACLEAVOIDANCE_DETECTION < -__OBSTACLEAVOIDANCE_BOUNDING) &&
			//(e.local_position.x + __OBSTACLEAVOIDANCE_DETECTION > __OBSTACLEAVOIDANCE_BOUNDING || 
			e.local_position.x + e.entity->getRadius() < -entity.getRadius())) 
			continue;

		// Calculate intersections with the y axis
		double expandedRadius = e.entity->getRadius() + entity.getRadius(); 
		double squarePart = sqrt(expandedRadius*expandedRadius - e.local_position.x*e.local_position.x);
		double intersection = e.local_position.y - squarePart;
		if(intersection <= 0) {
			intersection = e.local_position.y + squarePart;
		}

		if(intersection < closest_intersection) {
			closest_intersection = intersection;
			closest = e.entity;
			closest_position = e.local_position;
		}

		e.entity->setColor({150, 150, 255});

	}

	// Nothing to do! Don't change the current force.
	if(!closest) {
		return Vector2D();
	}

	closest->setColor({150, 0, 0});
	Vector2D steering_force = Vector2D();

	double multiplier = (1.0 + (this->detection_radius - closest_position.y) / this->detection_radius) * 1;

	steering_force.x = (closest->getRadius() - closest_position.x) * multiplier;
	steering_force.y = (closest->getRadius() - closest_position.x) * 0.2;

	return entity.toWorldSpace(steering_force);
}

FlockingBehaviour::FlockingBehaviour(MovingEntity& me, double radius) : SteeringBehaviour(me), radius(radius) {}

Vector2D FlockingBehaviour::calculate() {
	// Find neighbours
	vector<MovingEntity*> neighbours;
	
	//iterate through all entities checking for range
	for(auto tmp_e : this->entity.getWorld()->getEntities()) {
		if(!instanceof<MovingEntity>(tmp_e.get())) 
			continue;

		MovingEntity* e = dynamic_cast<MovingEntity*>(tmp_e.get());

		if(e->getTeam() != this->entity.getTeam())
			continue;

		Vector2D to = e->getPosition() - this->entity.getPosition();
		//the bounding radius of the other is taken into account by adding it
		//to the range
		double range = radius + e->getRadius();
		//if entity within range, tag for further consideration. (working in
		//distance-squared space to avoid sqrts)
		if ( (e != &this->entity) && (to.length() < range))
		{
			neighbours.push_back(e);
		}
	}//next entity	

	// Seperation
	
	Vector2D SteeringForce;
	for (int a=0; a<neighbours.size(); ++a)
	{
	//make sure this agent isn't included in the calculations and that
	//the agent being examined is close enough.
	if((neighbours[a] != &this->entity))
	{
	Vector2D ToAgent = this->entity.getPosition() - neighbours[a]->getPosition();
	//scale the force inversely proportional to the agent's distance
	//from its neighbor.
	SteeringForce = SteeringForce + (ToAgent.normalize())/ToAgent.length() * 100;
	}
	}

	// Alignment
	
	//used to record the average heading of the neighbours
	Vector2D AverageHeading;
	//used to count the number of vehicles in the neighborhood
	int NeighborCount = 0;
	//iterate through all the tagged vehicles and sum their heading vectors
	for (int a=0; a<neighbours.size(); ++a)
	{
	//make sure *this* agent isn't included in the calculations and that
	//the agent being examined is close enough
	if((neighbours[a] != &this->entity))
	{
	AverageHeading = AverageHeading + neighbours[a]->getVelocity().normalize();
	++NeighborCount;
	}
	}
	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0)
	{
	AverageHeading = AverageHeading / (double)NeighborCount;
	AverageHeading = AverageHeading - this->entity.getVelocity().normalize();
	}

	SteeringForce = SteeringForce + AverageHeading;

	// Cohesion
	
	//first find the center of mass of all the agents
	Vector2D CenterOfMass;
	NeighborCount = 0;
	//iterate through the neighbours and sum up all the position vectors
	for (int a=0; a<neighbours.size(); ++a)
	{
	//make sure *this* agent isn't included in the calculations and that
	//the agent being examined is a neighbor
	if((neighbours[a] != &this->entity))
	{
	CenterOfMass = CenterOfMass + neighbours[a]->getPosition();
	++NeighborCount;
	}
	}
	if (NeighborCount > 0)
	{
	//the center of mass is the average of the sum of positions
	CenterOfMass = CenterOfMass / (double)NeighborCount;
	//now seek toward that position
	Vector2D pos = this->entity.getPosition();
	Vector2D force = (CenterOfMass - pos);

	SteeringForce = SteeringForce + force - this->entity.getVelocity();
	}

	if(SteeringForce.length() < 0.5) {
		return Vector2D();
	}

	return SteeringForce;
}
