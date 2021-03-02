#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "tilemap.h"
#include "sdlecs.h"
#include "chara.h"

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
    // init SDL
    SDL_Window* window = SDL_CreateWindow("Snackman early dev", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480, 272, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // instantiate main scene
    Scene mainScene(renderer);

    // create new tilemap entity
    Entity& tileMap = mainScene.createEntity<TileMapEntity>("res/map1.tmx");
    mainScene.setSpecialEntity("tilemap", tileMap);

    // create pacman entity
    Entity& pacman = mainScene.createEntity<PacmanEntity>(16, 64);

    // create ghost entities
    Entity& ghost1 = mainScene.createEntity<GhostEntity>(64, 32);
    Entity& ghost2 = mainScene.createEntity<GhostEntity>(32, 64);
    Entity& ghost3 = mainScene.createEntity<GhostEntity>(128, 128);
    Entity& ghost4 = mainScene.createEntity<GhostEntity>(128, 64);

    // register systems
    mainScene.registerSystem<TileMapDataSystem>();
    mainScene.registerSystem<TileMapRenderSystem>();

    // chararacter stuff
    mainScene.registerSystem<PlayerControllerSystem>();
    mainScene.registerSystem<AIControllerSystem>();
    mainScene.registerSystem<CharacterControlSystem>();
    mainScene.registerSystem<CharacterRenderSystem>();

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDLEventComponent& event = mainScene.getComponent<SDLEventComponent>();

    while (!event.quit)
    {
        // INIT NEW COMPONENTS
        mainScene.init();

        // UPDATE COMPONENTS
        mainScene.update();

        // RENDER COMPONENTS
        mainScene.render();
    }

    // quit
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}