#pragma once
#include "../util/Color.h"
#include "../util/Shapes.h"
#include "../util/Vector2D.h"
#include "World.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>

using std::vector, std::shared_ptr;

struct LocalizedEntity {
    Vector2D local_position;
    shared_ptr<BaseEntity> entity;
};

class BaseEntity {

    protected:
        Vector2D position;
        World* world;
        double radius;
        Color color;
        bool solid;
        string name;
        vector<Shape*> shapes;

    public:
        BaseEntity(string name, Vector2D p, World* w);
        BaseEntity(string name, Vector2D p, World* w, Color c, double d, bool s);

        // Rule of 3
        BaseEntity(BaseEntity& b);
        BaseEntity& operator=(const BaseEntity& b);
        ~BaseEntity();

        virtual void update(float delta);
        virtual const vector<Shape*> render();

        void setPosition(int x, int y);
        void setPosition(const Vector2D pos);
        Vector2D& getPosition();

        void setColor(Color c);
        World* getWorld();

        virtual const vector<LocalizedEntity> getLocalEntities();
        double getRadius();
        bool isSolid();
        string getName();
        void addShape(Shape* s);
        void removeShape(Shape* s);

};

class PointerEntity : public BaseEntity {
    public:
        PointerEntity(Vector2D p, World* w);
};

class WallEntity : public BaseEntity {
    public:
        WallEntity(string n, Vector2D p, World* w);
};
