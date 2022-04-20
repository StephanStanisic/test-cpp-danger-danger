#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "Color.h"
#include "Vector2D.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <mutex>

using std::cout, std::endl, std::vector, std::mutex;

class Shape {
    protected:
        Color color;
        
    public:
        Shape(Color co);
        virtual void draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer);
        virtual ~Shape();

        virtual void setColor(Color co);
        Color getColor();
};

class Line : public Shape {
    public:
        Vector2D start, end;

        Line(Vector2D s, Vector2D e, Color c);
        ~Line();

        void draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) override;
};

class Circle : public Shape {
    private:
        double _da;
        vector<Shape*> shapes;
        double radius;
        int sides;
        Vector2D center;

        void updateLines();

    public:
        Circle(Vector2D c, double r, int s, Color co);
        Circle(Vector2D c, double r, Color co);
        ~Circle();

        void draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) override;
        Circle& setCenter(Vector2D center);
        Circle& setRadius(double r);
        Circle& setSides(int s);
        void setColor(Color co) override;
};

class Sprite : public Shape {
    private:
        SDL_Rect SrcR;
        SDL_Texture** texture;
        Vector2D position;
        double angle;

    public:
        Sprite(SDL_Texture** t, SDL_Rect s, Vector2D pos, double angle);
        void draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) override;
        void setPosition(Vector2D pos);
        void setAngle(double a);
};

class Text : public Shape {
    private:
        string message;
        mutex messageLock;
        TTF_Font** font;
        Vector2D position;
        vector<SDL_Rect> rects;

    public:
        Text(const char* message, TTF_Font** font, Color color, Vector2D position);
        void draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) override;
        void setPosition(Vector2D pos);
        void setText(const char* text);
};
