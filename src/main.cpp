#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "tilemap.h"
#include "ecpps.h"

using namespace ecpps;

SDL_Texture* loadTexture(SDL_Renderer* renderer, string fileName){
    SDL_Texture* texture = IMG_LoadTexture(renderer, ("res/" + fileName).c_str());
        if( texture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        printf(("res/" + fileName).c_str());
        printf( IMG_GetError());
    }
    return texture;
}

int main(int argv, char** args)
{
    Scene mainScene;

    Entity& tileMap = mainScene.createEntity();

    TileMapDataComponent mapData;
    mapData.fileName = "map.dat";

    TileMapRenderComponent mapRenderer;
    
    tileMap.addComponent(mapData);
    tileMap.addComponent(mapRenderer);

    mainScene.registerSystem<TileMapDataSystem>();
    mainScene.registerSystem<TileMapRenderSystem>();

    SDL_Window* window = SDL_CreateWindow("Hello SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture* roadTexture = loadTexture(renderer, "roadsheet.png");

    TileMap mainMap("map.dat", roadTexture);

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

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

        // UPDATE
        mainScene.update();

        SDL_RenderClear(renderer);

        // RENDER
        mainScene.render();

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}