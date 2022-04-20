#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "game/World.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <random>
#include "tests/Vector2DTests.hpp"
#include "tests/AstarTests.hpp"
#include "tests/FuzzyTests.hpp"
#include <ctime>
#include <cstdlib>
#include "Fuzzy/MakeVariables.hpp"

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#define path(x) x
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#define path(x) "gp-aai/" x
#endif

using std::thread, std::cout, std::endl, std::string;

//bool DEBUGGING = true;


string get_current_dir() {
    char buff[FILENAME_MAX]; //create string buffer to hold path
    GetCurrentDir(buff, FILENAME_MAX);
    string current_working_dir(buff);
    return current_working_dir;
}

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

void drawTile(SDL_Renderer* renderer, SDL_Texture* texture, int u, int v, int x, int y, int z) {
    SDL_Rect SrcR { 8 * u * z, 8 * v * z, 8 * z, 8 * z };
    SDL_Rect DestR { 16 * x, 16 * y, 16, 16 };
    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);
}

void drawHouse(SDL_Renderer* renderer, SDL_Texture* Background_Tx2, int sx, int sy) {
    for(int x=3; x<=7; x++) {
        for(int y=0; y<=1; y++) drawTile(renderer, Background_Tx2, x, y, sx + x, sy + y, 2);
        for(int y=4; y<=5; y++) drawTile(renderer, Background_Tx2, x, y, sx + x, sy - 2 + y, 2);
    }

    drawTile(renderer, Background_Tx2, 0, 7, sx + 3 + 0, sy + 4, 2);
    drawTile(renderer, Background_Tx2, 0, 7, sx + 3 + 0, sy + 5, 2);
    drawTile(renderer, Background_Tx2, 0, 8, sx + 3 + 0, sy + 6, 2);

    drawTile(renderer, Background_Tx2, 1, 7, sx + 3 + 1, sy + 4, 2);
    drawTile(renderer, Background_Tx2, 1, 7, sx + 3 + 1, sy + 5, 2);
    drawTile(renderer, Background_Tx2, 1, 8, sx + 3 + 1, sy + 6, 2);

    drawTile(renderer, Background_Tx2, 1, 7, sx + 3 + 2, sy + 4, 2);
    drawTile(renderer, Background_Tx2, 1, 7, sx + 3 + 2, sy + 5, 2);
    drawTile(renderer, Background_Tx2, 1, 8, sx + 3 + 2, sy + 6, 2);

    drawTile(renderer, Background_Tx2, 1, 7, sx + 3 + 3, sy + 4, 2);
    drawTile(renderer, Background_Tx2, 1, 7, sx + 3 + 3, sy + 5, 2);
    drawTile(renderer, Background_Tx2, 1, 8, sx + 3 + 3, sy + 6, 2);

    drawTile(renderer, Background_Tx2, 2, 7, sx + 3 + 4, sy + 4, 2);
    drawTile(renderer, Background_Tx2, 2, 7, sx + 3 + 4, sy + 5, 2);
    drawTile(renderer, Background_Tx2, 2, 8, sx + 3 + 4, sy + 6, 2);

    drawTile(renderer, Background_Tx2, 8, 12, sx + 3 + 2, sy + 5, 2);
    drawTile(renderer, Background_Tx2, 8, 13, sx + 3 + 2, sy + 6, 2);
}

void display_loop(World* world) {
    if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;
        SDL_Surface* Loading_Surf;
        SDL_Texture* Background_Tx;
        SDL_Texture* Background_Tx2;

        //this opens a font style and sets a size

        // this is the color in rgb format,
        // maxing out all would give you the color white,
        // and it will be your text's color
        SDL_Color White = {255, 255, 255};

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0,100);

        int arr[WINDOW_WIDTH/16 * (WINDOW_HEIGHT*16+1)];
        for(int i=0; i<WINDOW_WIDTH/16 * (WINDOW_HEIGHT*16+1); i++) {
            arr[i] = dist6(rng);
        }

        if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            Loading_Surf = IMG_Load(path("background.bmp"));
            Background_Tx = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
            SDL_FreeSurface(Loading_Surf); 

            Loading_Surf = IMG_Load(path("Set_E_Small_Village1.png"));
            Background_Tx2 = SDL_CreateTextureFromSurface(renderer, Loading_Surf);
            SDL_FreeSurface(Loading_Surf); 


            while (!done) {
                SDL_Event event;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                    
                for(int i=0; i<WINDOW_WIDTH/16; i++) {
                    for(int o=0; o<WINDOW_HEIGHT/16+1; o++) {

                        switch(arr[o*WINDOW_HEIGHT/16+i]) {
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                drawTile(renderer, Background_Tx, 6, 4, i, o, 1);
                                break;
                            case 7:
                            case 8:
                            case 9:
                                drawTile(renderer, Background_Tx, 6, 3, i, o, 1);
                                break;
                            case 10:
                            case 11:
                            case 12:
                                drawTile(renderer, Background_Tx, 6, 5, i, o, 1);
                                break;
                            case 13:
                                drawTile(renderer, Background_Tx, 4, 3, i, o, 1);
                                break;
                            default:
                                drawTile(renderer, Background_Tx, 0, 0, i, o, 1);

                        }
                    }
                }

                world->render(renderer);

                // Houses on top of the world so players can walk behind
                drawHouse(renderer, Background_Tx2, 5, 12);
                drawHouse(renderer, Background_Tx2, 30, 29);
                drawHouse(renderer, Background_Tx2, 28, 10);
                drawHouse(renderer, Background_Tx2, 20, 2);
                

                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    } else if(event.type == SDL_MOUSEBUTTONDOWN) {
                        SDL_MouseButtonEvent ev = event.button;
                        if(ev.button == SDL_BUTTON_LEFT)
                            world->event(WorldEvent::leftMouseClick, Vector2D(ev.x, ev.y));
                        else
                            world->event(WorldEvent::rightMouseClick, Vector2D(ev.x, ev.y));
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
}

void logic_loop(World* world, bool* running) {
    while(*running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
        world->update(1);
    }
}

int main(int argc, char* argv[])  {
    cout << get_current_dir() << endl;

    // Run tests first
    run_Vector2D_tests();
    run_Fuzzy_tests();
    run_Astar_tests();

    srand(time(NULL));
    MakeVariables();

    World* world = new World(WINDOW_WIDTH, WINDOW_HEIGHT);
    bool running = true;

    // We're handing out references since this all isn't threadsafe anyways
    thread display_thread = thread(display_loop, world);
    thread logic_thread = thread(logic_loop, world, &running);

    // Wait for display thread to finish
    display_thread.join();
    running = false;
    logic_thread.join();
    delete world;
    return 0;
}
