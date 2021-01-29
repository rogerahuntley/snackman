
// sdl helper class
// provides and interface between ECS and SDL

#ifndef SDLECS_H
#define SDLECSH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ecpps/ecpps.h"

using namespace ecpps;

struct RenderTextureComponent : public RenderComponent {
    SDL_Texture* renderTexture;
};

struct SDLRendererComponent : public RenderComponent {
    SDL_Renderer* renderer;
};

// most often used ECSManager type, usually near the root of the program
class Scene: public ECSManager {
    private:
        SDLRendererComponent renderer;
    public:
        SDLRendererComponent& getRenderer();
};

#endif // SDLECS_H