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

struct SDLRendererComponent: public RenderComponent {
    private:
        SDL_Texture* getTexture(string fileName);
        SDL_Renderer* renderer;
    public:
        SDLRendererComponent(SDL_Renderer* renderer): renderer(renderer) {};
        SDL_Texture* loadTexture(string fileName);
        void destroyTexture(string& fileName);
        map <string, SDL_Texture*> textures;
        SDL_Renderer* getRenderer();
};

struct SDLEventComponent: public Component {

};

// most often used ECSManager type, usually near the root of the program
class Scene: public ECSManager {
    private:
    public:
        Scene(SDL_Renderer* renderer);
        SDLRendererComponent& getRenderer();
        void render() override;
};

#endif // SDLECS_H