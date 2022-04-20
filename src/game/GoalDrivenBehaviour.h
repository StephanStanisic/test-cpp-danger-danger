#pragma once
#include <vector>
#include <string>
#include "../util/Vector2D.h"
#include "../util/Shapes.h"
#include <mutex>
class MovingEntity;

using std::vector, std::string, std::mutex;

class Goal {
	protected:
		string name;
		Goal(string name);

	public:
		virtual void Activate() = 0;
		virtual int Process() = 0;
		virtual void Terminate() = 0;
		virtual string getName() = 0;
};

class AtomicGoal : public Goal {
	protected:
		MovingEntity& entity;
		AtomicGoal(string name, MovingEntity& entity);

	public:
		virtual void Activate() = 0;
		virtual int Process() = 0;
		virtual void Terminate() = 0;
		virtual string getName();
};

class CompositeGoal : public Goal {
	protected:
		CompositeGoal(string name);
		~CompositeGoal();
		vector<Goal*> subgoals;
		mutex subgoalsLock;

		void AddSubGoal(Goal* g);

	public:
		void Activate() override;
		int Process() override;
		void Terminate() override;
		string getName() override;
};

class SeekGoal : public AtomicGoal {
	protected:
		Vector2D seek_pos;

	public:
		SeekGoal(MovingEntity& entity, Vector2D seek_pos);
		void Activate() override;
		int Process() override;
		void Terminate() override;
};

class FlockGoal : public AtomicGoal {
	public:
		FlockGoal(MovingEntity& entity);
		void Activate() override;
		int Process() override;
		void Terminate() override;
};

class RedPatrolGoal : public CompositeGoal {
	public:
		RedPatrolGoal(MovingEntity& entity);
};
class BluePatrolGoal : public CompositeGoal {
	public:
		BluePatrolGoal(MovingEntity& entity);
};

class FollowPathGoal : public CompositeGoal {
	public:
		FollowPathGoal(MovingEntity& entity, vector<Vector2D> points);
};

class ShortestPathGoal : public AtomicGoal {
	private:
		Vector2D to;
		FollowPathGoal* followPathGoal;
		mutex followPathGoalLock;

	public:
		ShortestPathGoal(MovingEntity& entity, Vector2D pos);
		~ShortestPathGoal();
		void Activate() override;
		int Process() override;
		void Terminate() override;
		string getName() override;
};

class RedThink : public AtomicGoal {
	private:
		Goal* goal;

	public:
		RedThink(MovingEntity& entity);
		~RedThink();
		void Activate() override;
		int Process() override;
		void Terminate() override;
		string getName() override;
};

class BlueThink : public AtomicGoal {
	private:
		Goal* goal;

	public:
		BlueThink(MovingEntity& entity);
		~BlueThink();
		void Activate() override;
		int Process() override;
		void Terminate() override;
		string getName() override;
};

class ShootyShootyPewPew : public CompositeGoal {
	public:
		ShootyShootyPewPew(MovingEntity& entity);
};

class TakeShotGoal : public AtomicGoal {
	private:
		MovingEntity& enemy;
		Line* lazor;
		int ticks;

	public:
		TakeShotGoal(MovingEntity& entity, MovingEntity& enemy);
		void Activate() override;
		int Process() override;
		void Terminate() override;
		string getName() override;
};

#include "MovingEntity.h"
