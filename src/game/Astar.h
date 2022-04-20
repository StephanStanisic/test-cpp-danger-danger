#pragma once

#include <vector>
#include "../util/Vector2D.h"
#include "../util/Shapes.h"

using std::vector;

class Vertex;

struct Edge {
	Vertex& toVertex;
	int weight;
};

class Vertex {
	private:
		vector<Edge> edges;
		Vector2D position;
		double heuristic;
		double distance;
		bool visited;
		Vertex* from;

	public:
		Vertex(Vector2D position, double heuristic);

		void addEdge(Edge edge);
		Vector2D getPosition();
		vector<Edge> getNeighbours();
		void print();
		double getDistance();
		void setDistance(double d);
		double getHeuristic();
		bool isVisited();
		void setVisited(bool v);
		Vertex* getFrom();
		void setFrom(Vertex* v);
};

class Graph {
	private:
		vector<Vertex*> vertices;
		vector<Shape*> shapes;

	public:
		Graph();
		~Graph();

		void addVertex(Vertex* vertex);

		Vertex* findClosest(Vector2D pos);
		Vertex* vertexAt(Vector2D pos);
		vector<Vector2D> shortestPathPoints(Vector2D from, Vector2D to);
		vector<Vertex*> shortestPath(Vertex* from, Vertex* to);
        const vector<Shape*> render();
		void addShape(Shape* shape);
};

