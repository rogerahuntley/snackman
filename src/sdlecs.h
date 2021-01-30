#ifndef SDLECS_H
#define SDLECS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <map>

#include "ecpps/ecpps.h"

// sdl helper class
// provides and interface between ECS and SDL

using std::string;
using std::map;

using namespace ecpps;

struct RenderTextureComponent : public RenderComponent {
    SDL_Texture* renderTexture;
};

struct SDLRendererComponent : public RenderComponent {
    private:
        SDL_Texture* getTexture(string fileName);
    public:
        SDL_Texture* loadTexture(string fileName);
        void destroyTexture(string& fileName);
        SDL_Renderer* renderer;
        map <string, SDL_Texture*> textures;

};

// most often used ECSManager type, usually near the root of the program
class Scene: public ECSManager {
    private:
        SDLRendererComponent renderer;
    public:
        SDLRendererComponent& getRenderer();
};

#endif // SDLECS_H