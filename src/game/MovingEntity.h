#pragma once
#include "../util/Shapes.h"
#include "../util/Vector2D.h"
#include "BaseEntity.h"
#include "SteeringBehaviour.h"
#include "GoalDrivenBehaviour.h"
#include <mutex>

enum class Team {
	Red, Blue
};

class MovingEntity : public BaseEntity {
	protected:
		Vector2D velocity;
		double mass;
		double maxSpeed;

		vector<SteeringBehaviour*> sbs;
		mutex steeringBehavioursLock;
		virtual void updateLines();
		double getAngle();

		Goal* goal;
		Goal* resetGoal;
		mutex goalLock;

		Team team;
		int rocketLauncherAmmo, handGunAmmo;
		int health;

	public:
		MovingEntity(string n, Vector2D p, World* w, Vector2D v, double m, double ms, Team team);
		~MovingEntity();
		void update(float delta) override;

		double getMaxSpeed();
		Vector2D getVelocity();

		const vector<LocalizedEntity> getLocalEntities() override;
		Vector2D toLocalSpace(Vector2D v);
		Vector2D toWorldSpace(Vector2D v);

		void setGoal(Goal* goal);

		void pushSteeringBehaviour(SteeringBehaviour* sb);
		void clearSteeringBehaviours();
		Team getTeam();
		int getRocketLauncherAmmo();
		void setRocketLauncherAmmo(int a);
		int getHandGunAmmo();
		void setHandGunAmmo(int a);
		void takeDamage(int a);
};

class Triangle : public MovingEntity {
	public:
		Triangle(string n, Vector2D p, World* w, Vector2D v, double m, double ms);
		void updateLines() override;
};

class Soldier : public MovingEntity {
	private:
		SDL_Texture** texture;

	public:
		Soldier(SDL_Texture**, Vector2D p, World* w, Team team);
		void updateLines() override;
};
class Commander : public MovingEntity {
	private:
		SDL_Texture** texture;

	public:
		Commander(SDL_Texture**, Vector2D p, World* w, Team team);
		void updateLines() override;
};
