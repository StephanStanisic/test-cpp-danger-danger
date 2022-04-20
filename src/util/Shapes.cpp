#include "Shapes.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>
#include <sstream>

using std::cout, std::endl, std::vector, std::istringstream;

void Shape::draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) {}
Shape::Shape(Color c) : color(c) {}
Shape::~Shape() {}
void Shape::setColor(Color c) {
    this->color = c;
}
Color Shape::getColor() {
    return this->color;
}

Line::Line(Vector2D s, Vector2D e, Color c) : Shape(c), start(s), end(e) {}
Line::~Line() {}

void Line::draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, this->color.r, this->color.g, this->color.b, SDL_ALPHA_OPAQUE);

    // Apply viewport transform
    Vector2D new_start = start * transform + (translate),
             new_end = end * transform + (translate);

    SDL_RenderDrawLine(renderer, new_start.x, new_start.y, new_end.x, new_end.y);
}

void Circle::updateLines() {
    for(auto shape: this->shapes) {
        delete shape;
    }
    shapes = vector<Shape*>();

    float angle = this->_da;
    Vector2D start, end;

    end.x = this->radius;
    end.y = 0.0f;
    end = end + this->center;
    for (int i=0; i!=this->sides; i++) {
        start = end;
        end.x = cos(angle) * this->radius;
        end.y = sin(angle) * this->radius;
        end = end + center;
        angle += this->_da;

        shapes.push_back(new Line(start, end, this->color));
    }
}

Circle::Circle(Vector2D c, double r, int s, Color co) : Shape(co), center(c), radius(r), sides(s), _da(M_PI * 2 / s)  {
    this->updateLines();
}
Circle::Circle(Vector2D c, double r, Color co) : Circle(c, r, 25, co) {}
Circle::~Circle() {
    for(auto shape: this->shapes) {
        delete shape;
    }
}

void Circle::setColor(Color c) {
    for(auto shape: this->shapes) {
        shape->setColor(c);
    }
}
void Circle::draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) {
    for(auto shape: shapes){
        shape->draw(transform, translate, renderer);
    }
}
Circle& Circle::setCenter(Vector2D center) {
    this->center = center;
    this->updateLines();
    return *this;
}
Circle& Circle::setRadius(double r) {
    this->radius = r;
    this->updateLines();
    return *this;
}
Circle& Circle::setSides(int s) {
    this->sides = s;
    this->updateLines();
    return *this;
}

Sprite::Sprite(SDL_Texture** t, SDL_Rect s, Vector2D pos, double a) : Shape({0,0,0}), texture(t), SrcR(s), position(pos), angle(a) {}
void Sprite::draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) {
    Vector2D new_pos = position * transform + (translate);
    SDL_Rect DestR { new_pos.x, new_pos.y, SrcR.w, SrcR.h };
    SDL_RenderCopyEx(renderer, *texture, &SrcR, &DestR, (double)(((int)(angle * 180.0 / M_PI) - 90) % 360), NULL, SDL_FLIP_NONE);
}
void Sprite::setPosition(Vector2D pos) {
    this->position = pos;
}
void Sprite::setAngle(double a) {
    this->angle = a;
}

Text::Text(const char* message, TTF_Font** font, Color color, Vector2D position) : Shape(color), font(font), position(position) {
    this->messageLock.lock();
    this->message = string(message);
    this->messageLock.unlock();
}

void Text::draw(Vector2D transform, Vector2D translate, SDL_Renderer* renderer) {
    this->messageLock.lock();
    this->rects.clear();
    Vector2D new_pos = position * transform + (translate);
    istringstream f(this->message);
    string s;    
    int width, height;
    while (getline(f, s)) {
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(*this->font, s.c_str(), {this->color.r, this->color.g, this->color.b}); 
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        TTF_SizeText(*this->font, s.c_str(), &width, &height);

        this->rects.push_back({ new_pos.x, new_pos.y, width, height });
        SDL_RenderCopy(renderer, Message, NULL, &(this->rects[this->rects.size() - 1]));
        new_pos.y += height;

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
    }
    this->messageLock.unlock();
}

void Text::setPosition(Vector2D pos) {
    this->position = pos;
}

void Text::setText(const char *text) {
    this->messageLock.lock();
    this->message = string(text);
    this->messageLock.unlock();
}
