#pragma once
#include "../util/Vector2D.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <vector>
#include <mutex>
#include "Astar.h"
class BaseEntity;
class Commander;

using std::vector, std::shared_ptr, std::mutex;

enum WorldEvent { leftMouseClick, rightMouseClick };

class World {

    private:
        int width, height;
        vector<shared_ptr<BaseEntity>> entities;
        BaseEntity& seek_pos;
        Commander* redCommander;
        Commander* blueCommander;

        Vector2D transform, translate;
        SDL_Texture* redSoldierSprite;
        SDL_Texture* blueSoldierSprite;
        Graph* graph;
        TTF_Font* font;
        mutex graphLock;

    public:
        World(int w, int h);
        ~World();
        void update(float delta);
        void render(SDL_Renderer* renderer);
        void event(WorldEvent e, Vector2D pos);

        Vector2D& getSeekPosition();
        const vector<shared_ptr<BaseEntity>> getEntities();

        vector<Vector2D> shortestPath(Vector2D start, Vector2D end);
        TTF_Font** getFont();
};

