#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "tilemap.h"
#include "sdlecs.h"

// main file ayy
// this is a lib / framework for learning c++ and making games
// all code (except linked libraries) was all written by:

// --------- ROGER HUNTLEY II --------- //
// ----- roger.a.huntley@gmail.com ---- //
// ----------- @rogerthat52 ----------- //
// -------------- A.K.A. -------------- //
// ----------- @YoungsDitch ----------- //

int main(int argv, char** args)
{
    double currentTime = SDL_GetTicks();
    double lastTime = currentTime;
    double deltaTime;

    // init SDL
    SDL_Window* window = SDL_CreateWindow("Snackman early dev", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 272, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // instantiate main scene
    Scene mainScene;

    // instantiate render component
    SDLRendererComponent renderComponent;
    renderComponent.renderer = renderer;

    // add render component to main scene
    mainScene.addComponent(renderComponent);

    // create new tilemap entity
    Entity& tileMap = mainScene.createEntity();

    // instantiate tilemap data component
    TileMapDataComponent mapData;
    mapData.tmxFile = "res/map1.tmx";

    // instantiate tilemap renderer component
    TileMapRenderComponent mapRenderer;
    //mapRenderer.roadTexture = roadTexture;

    tileMap.addComponent(mapRenderer);
    tileMap.addComponent(mapData);

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    
    // register systems
    mainScene.registerSystem<TileMapDataSystem>();
    mainScene.registerSystem<TileMapRenderSystem>();

    bool isRunning = true;
    SDL_Event event;

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
            }
        }

        // INIT NEW COMPONENTS
        mainScene.init();

        // UPDATE COMPONENTS
        mainScene.update();

        SDL_RenderClear(renderer);

        // RENDER COMPONENTS
        mainScene.render();

        SDL_RenderPresent(renderer);

        // get delta time
        double currentTime = SDL_GetTicks();
        double deltaTime = (currentTime - lastTime) / 1000.0f;
        double lastTime = currentTime;
    }

    // quit
    //SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}