#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "tilemap.h"
#include "sdlecs.h"

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
    // init SDL
    SDL_Window* window = SDL_CreateWindow("Snackman early dev", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture* roadTexture = loadTexture(renderer, "roadsheet.png");

    // instantiate main scene
    Scene mainScene;
    
    // register systems
    mainScene.registerSystem<TileMapDataSystem>();
    mainScene.registerSystem<TileMapRenderSystem>();

    // instantiate render component
    SDLRendererComponent renderComponent;
    renderComponent.renderer = renderer;

    // add render component to main scene
    mainScene.addComponent(renderComponent);

    // create new tilemap entity
    Entity& tileMap = mainScene.createEntity();

    // instantiate tilemap data component
    TileMapDataComponent mapData;
    mapData.fileName = "map.dat";

    // instantiate tilemap renderer component
    TileMapRenderComponent mapRenderer;
    mapRenderer.roadTexture = roadTexture;

    // add components to entity
    tileMap.addComponent(mapData);
    tileMap.addComponent(mapRenderer);

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

        // INIT NEW COMPONENTS
        mainScene.init();

        // UPDATE COMPONENTS
        mainScene.update();

        SDL_RenderClear(renderer);

        // RENDER COMPONENTS
        mainScene.render();

        SDL_RenderPresent(renderer);
    }

    // quit
    SDL_DestroyTexture(roadTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}