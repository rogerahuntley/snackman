
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

// ------- SDLEventComponent ------- //

void SDLEventComponent::flushEvents(){
    // move from keydown to keypressed
    keyPressed.insert(keyDown.begin(), keyDown.end());
    // clear up/down events
    keyDown.clear();
    keyUp.clear();

    // for every event
    while(SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_KEYDOWN:
                    keyDown.emplace(event.key.keysym.scancode);
                break;
            case SDL_KEYUP:
                    keyUp.emplace(event.key.keysym.scancode);
                break;
        }
    }
    
    // check keyup and remove from keypressed
    for(SDL_Scancode key : keyUp){
        keyPressed.erase(key);
    }
}



// ------- SDLDeltaTimeComponent ------- //

double SDLDeltaTimeComponent::getDeltaTimeS(){
    return deltaTime * 0.001;
}

double SDLDeltaTimeComponent::getDeltaTimeMS(){
    return deltaTime;
}

void SDLDeltaTimeComponent::updateTime(){
    // set delta time
    currentTime = SDL_GetPerformanceCounter();
    deltaTime = (currentTime - lastTime) * 1000 / (double)SDL_GetPerformanceFrequency();
    lastTime = currentTime;
}

// ------- Scene ------- //

Scene::Scene(SDL_Renderer* renderer){
    // initalize render component
    SDLRendererComponent render(renderer);
    // initalize event component
    SDLEventComponent event;
    // initalize deltaTime component
    SDLDeltaTimeComponent deltaTime;

    addComponent(render);
    addComponent(event);
    addComponent(deltaTime);

    groupEntities<SDLRendererComponent>();
    groupEntities<SDLEventComponent>();
    groupEntities<SDLDeltaTimeComponent>();
};


SDLRendererComponent& Scene::getRenderer(){
    return getComponent<SDLRendererComponent>();
};

void Scene::update(){
    // update delta time
    getComponent<SDLDeltaTimeComponent>().updateTime();
    // update events
    getComponent<SDLEventComponent>().flushEvents();
    // render all render systems
    ECSManager::update();
}

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