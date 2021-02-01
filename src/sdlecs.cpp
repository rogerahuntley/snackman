
// sdl helper class
// provides and interface between ECS and SDL

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

using std::string;

#include "sdlecs.h"

// ------- SDLRenderComponent ------- //

SDL_Texture* SDLRendererComponent::loadTexture(string fileName){
    // get texture pointer ready
    SDL_Texture* texture;
    // set path up
    string path("res/" + fileName);
    // check if texture entry exists
    if(textures.find(fileName) != textures.end()){
        // if so, pull from list
        texture = getTexture(fileName);
    } else {
        // else, load texture
        texture = IMG_LoadTexture(renderer, path.c_str());
            if( texture == NULL )
        {
            std::cout << "Failed to load texture image!" << std::endl;
            std::cout << path.c_str() << std::endl;
            std::cout << IMG_GetError() << std::endl;
        }
        // create new entry
        textures.insert({fileName, texture});
        // note we don't have to return a reference to the textures entry bc it's already a pointer to something else
    }
    return texture;
}

SDL_Texture* SDLRendererComponent::getTexture(string fileName){
    // get texture reference
    SDL_Texture* texture = textures.at(fileName);
    // return
    return texture;
}

void SDLRendererComponent::destroyTexture(string& fileName){
    // get texture reference
    SDL_Texture* texture = textures.at(fileName);
    // remove from sdl
    SDL_DestroyTexture(texture);
    // remove reference from map
    textures.erase(fileName);
}

SDL_Renderer* SDLRendererComponent::getRenderer(){
    return renderer;
}

// ------- Scene ------- //

Scene::Scene(SDL_Renderer* renderer){
    // initalize render component
    SDLRendererComponent renderComp(renderer);
    // initalize even component
    

    addComponent(renderComp);
};


SDLRendererComponent& Scene::getRenderer(){
    return getComponent<SDLRendererComponent>();
};

void Scene::render(){
    // get renderer
    SDL_Renderer* renderer = getComponent<SDLRendererComponent>().getRenderer();
    // clear renderer
    SDL_RenderClear(renderer);
    // render all render systems
    ECSManager::render();
    // send it boii
    SDL_RenderPresent(renderer);
}