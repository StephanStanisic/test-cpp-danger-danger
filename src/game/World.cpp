#include "../util/Shapes.h"
#include "../util/VectorMath.hpp"
#include "Astar.h"
#include "BaseEntity.h"
#include "MovingEntity.h"
#include "World.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <random>
#include <vector>

#include <csignal>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define path(x) x
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define path(x) "gp-aai/" x
#endif

#define WORLD_GRAPH_DENSITY 24

#define breakpoint std::raise(SIGINT)
using std::cout, std::endl, std::shared_ptr;

void addHouseWalls(vector<shared_ptr<BaseEntity>>& entities, int w, int h, int sx, int sy, World* world) {
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 4), h/2 - 16 * (sy  + 6)), world)));
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 5), h/2 - 16 * (sy  + 6)), world)));
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 6), h/2 - 16 * (sy  + 6)), world)));
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 7), h/2 - 16 * (sy  + 6)), world)));

    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 4), h/2 - 16 * (sy  + 7)), world)));
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 5), h/2 - 16 * (sy  + 7)), world)));
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 6), h/2 - 16 * (sy  + 7)), world)));
    entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Wall", Vector2D(-w/2 + 16 * (sx + 7), h/2 - 16 * (sy  + 7)), world)));
}

World::World(int w, int h) : font(nullptr), width(w), height(h), seek_pos(*new PointerEntity(Vector2D(-w/2, -h/2), this)), redSoldierSprite(nullptr), blueSoldierSprite(nullptr), blueCommander(nullptr), redCommander(nullptr) {

    this->entities.push_back(shared_ptr<BaseEntity>(&this->seek_pos));

    addHouseWalls(this->entities, this->width, this->height, 5, 12, this);
    addHouseWalls(this->entities, this->width, this->height, 30, 29, this);
    addHouseWalls(this->entities, this->width, this->height, 28, 10, this);
    addHouseWalls(this->entities, this->width, this->height, 20, 2, this);


    this->transform = Vector2D(1, -1);
    this->translate = Vector2D(this->width/2, this->height/2);

    this->redCommander = new Commander(&redSoldierSprite, Vector2D(), this, Team::Red);
    this->entities.push_back(shared_ptr<BaseEntity>(this->redCommander));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&redSoldierSprite, Vector2D(-10, -10), this, Team::Red)));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&redSoldierSprite, Vector2D(-10, 10), this, Team::Red)));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&redSoldierSprite, Vector2D(10, -10), this, Team::Red)));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&redSoldierSprite, Vector2D(11, 12), this, Team::Red)));

    this->blueCommander = new Commander(&blueSoldierSprite, Vector2D(), this, Team::Blue);
    this->entities.push_back(shared_ptr<BaseEntity>(this->blueCommander));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&blueSoldierSprite, Vector2D(12, 12), this, Team::Blue)));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&blueSoldierSprite, Vector2D(13, 13), this, Team::Blue)));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&blueSoldierSprite, Vector2D(14, 12), this, Team::Blue)));
    this->entities.push_back(shared_ptr<BaseEntity>(new Soldier(&blueSoldierSprite, Vector2D(37, 26), this, Team::Blue)));

    //std::random_device dev;
    //std::mt19937 rng(dev());
    //std::uniform_int_distribution<std::mt19937::result_type> dist6(0,200);

    //for(int i=0; i<30; i++) {
    //    this->entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Tree", Vector2D(dist6(rng), dist6(rng)), *this)));
    //}

    //for(int i=-this->width/2; i<this->width/2; i+=WORLD_GRAPH_DENSITY) {
    //    for(int o=-this->height/2; o<this->height/2; o+=WORLD_GRAPH_DENSITY) {
    //        this->entities.push_back(shared_ptr<BaseEntity>(new WallEntity("Tree", Vector2D(i, o), *this)));
    //    }
    //}


    //this->generateGraph();

}

void World::update(float delta) {
    for(auto entity: this->entities) {
        entity->update(delta);
    }
}

void World::render(SDL_Renderer* renderer){
    if(this->redSoldierSprite == nullptr){
        SDL_Surface* Loading_Surf = IMG_Load(path("RedPlayerShooting.png"));
        this->redSoldierSprite = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
        SDL_FreeSurface(Loading_Surf); 
        Loading_Surf = IMG_Load(path("BluePlayerShooting.png"));
        this->blueSoldierSprite = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
        SDL_FreeSurface(Loading_Surf); 
    }

    if(this->font == nullptr) {
        this->font = TTF_OpenFont(path("Arial.ttf"), 9);
    }

    vector<Shape*> shapes;

    this->graphLock.lock();

    if(this->graph != nullptr) {
        shapes += this->graph->render();
    }

    for(auto entity: this->entities) {
        shapes += entity->render();
    }

    //breakpoint;
    // Create viewport transformation vector

    for(auto shape: shapes) {
        shape->draw(this->transform, this->translate, renderer);
    }

    this->graphLock.unlock();
}

void World::event(WorldEvent e, Vector2D pos) {
    pos = pos * this->transform - (this->transform * this->translate);
    this->seek_pos.setPosition(pos);
    if(e == WorldEvent::leftMouseClick)
        this->redCommander->setGoal(new ShortestPathGoal(*this->redCommander, pos));
    else
        this->blueCommander->setGoal(new ShortestPathGoal(*this->blueCommander, pos));
}

Vector2D& World::getSeekPosition() {
    return this->seek_pos.getPosition();
}

const vector<shared_ptr<BaseEntity>> World::getEntities() {
    return this->entities;
}

vector<Vector2D> World::shortestPath(Vector2D start, Vector2D end) {
    this->graphLock.lock();
    if(this->graph != nullptr) {
        delete this->graph;
        this->graph = nullptr;
    }

    this->graph = new Graph();

    for(int x=-this->width/2; x<=this->width/2; x+=WORLD_GRAPH_DENSITY) {
        for(int y=-this->height/2; y<=this->height/2; y+=WORLD_GRAPH_DENSITY) {
            Vertex* v = new Vertex(Vector2D(x, y), (start - Vector2D(x, y)).length());

            // Test if spot if free
            for(auto entity : this->entities) {
                if(!entity->isSolid()) continue;

                double distance = (entity->getPosition() - Vector2D(x, y)).length();
                if(distance < entity->getRadius()) {
                    // Taken
                    goto continue_to_next;
                }
            }

            if(x >= -this->width/2 + WORLD_GRAPH_DENSITY) {
                Vertex* vp = graph->vertexAt(Vector2D(x - WORLD_GRAPH_DENSITY, y));
                if(vp != nullptr) {
                    v->addEdge(Edge { *vp, WORLD_GRAPH_DENSITY });
                    vp->addEdge(Edge { *v, WORLD_GRAPH_DENSITY });
                    graph->addShape(new Line(v->getPosition(), vp->getPosition(), {58, 83, 57}));
                }
            }

            if(y >= -this->height/2 + WORLD_GRAPH_DENSITY) {
                Vertex* vp = graph->vertexAt(Vector2D(x, y - WORLD_GRAPH_DENSITY));
                if(vp != nullptr) {
                    v->addEdge(Edge { *vp, WORLD_GRAPH_DENSITY });
                    vp->addEdge(Edge { *v, WORLD_GRAPH_DENSITY });
                    graph->addShape(new Line(v->getPosition(), vp->getPosition(), {58, 83, 57}));
                }
            }

            graph->addVertex(v);
continue_to_next:;
        }
    }

    Vertex* v_end = graph->findClosest(start);
    Vertex* v_start = graph->findClosest(end);

    auto result = graph->shortestPath(v_start, v_end);
    vector<Vector2D> result_return;
    Vector2D last_point = result[0]->getPosition();
    for(auto vertex : result) {
        result_return.push_back(vertex->getPosition());
        graph->addShape(new Line(vertex->getPosition(), last_point, {255, 255, 255}));
        last_point = vertex->getPosition();
    }


    this->graphLock.unlock();

    return result_return;
}

World::~World() {
    delete this->graph;
}

TTF_Font** World::getFont() {
    return &this->font;
}
