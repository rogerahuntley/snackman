
// sdl helper class
// provides and interface between ECS and SDL

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sdlecs.h"

SDLRendererComponent& Scene::getRenderer(){
    return renderer;
};