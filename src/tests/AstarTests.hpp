#include <cassert>
#include <iostream>
#include <cmath>
#include "../game/Astar.h"

using std::cout, std::endl;

void addEdge(Vertex* v1, Vertex* v2, int weight) {
    v1->addEdge(Edge { *v2, weight });
    v2->addEdge(Edge { *v1, weight });
}

void run_Astar_tests() {
    Graph* graph = new Graph();

    Vertex* v1 = new Vertex(Vector2D(0, 0), 400);
    Vertex* v2 = new Vertex(Vector2D(200, -100), 300);
    Vertex* v3 = new Vertex(Vector2D(-300, -200), 900);
    Vertex* v4 = new Vertex(Vector2D(-200, 100), 600);
    Vertex* v5 = new Vertex(Vector2D(100, 200), 300);
    Vertex* v6 = new Vertex(Vector2D(300, 100), 0);
    Vertex* v7 = new Vertex(Vector2D(100, -200), 500);
    Vertex* v8 = new Vertex(Vector2D(-100, -100), 600);
    
    graph->addVertex(v1);
    graph->addVertex(v2);
    graph->addVertex(v3);
    graph->addVertex(v4);
    graph->addVertex(v5);
    graph->addVertex(v6);
    graph->addVertex(v7);
    graph->addVertex(v8);

    addEdge(v3, v7, 400);
    addEdge(v7, v1, 223);
    addEdge(v7, v2, 141);
    addEdge(v3, v8, 223);
    addEdge(v3, v4, 316);
    addEdge(v4, v8, 223);
    addEdge(v8, v1, 141);
    addEdge(v1, v2, 223);
    addEdge(v2, v6, 223);
    addEdge(v6, v5, 223);
    addEdge(v5, v1, 223);
    addEdge(v5, v4, 316);
    addEdge(v4, v1, 223);

    Vertex* end = graph->findClosest(Vector2D(300, 100));
    Vertex* start = graph->findClosest(Vector2D(-300, -200));

    cout << "Finding the path from " << start->getPosition() << " to " << end->getPosition() << endl;

    auto result = graph->shortestPath(start, end);
    for(auto vertex : result) {
        cout << "Vertex with position " << vertex->getPosition() << endl;
    }

    delete graph;
}
