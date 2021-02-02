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

// ------- Components ------- //

struct RenderTextureComponent : public RenderComponent {
    SDL_Texture* renderTexture;
};

struct SDLRendererComponent: public RenderComponent {
    private:
        SDL_Texture* getTexture(string fileName);
        SDL_Renderer* renderer;
        map <string, SDL_Texture*> textures;
    public:
        SDLRendererComponent(SDL_Renderer* renderer): renderer(renderer) {};
        SDL_Texture* loadTexture(string fileName);
        void destroyTexture(string& fileName);
        SDL_Renderer* getRenderer();
};

struct SDLEventComponent: public Component {
    private:
        const SDL_Event* event;
        const Uint8* state = SDL_GetKeyboardState(NULL);
    public:
        SDLEventComponent() {};
        const SDL_Event* getEvent();
        const Uint8* getState();
};

struct SDLDeltaTimeComponent: public Component {
    private:
        double currentTime;
        double lastTime;
        double deltaTime;
    public:
        SDLDeltaTimeComponent();
        double getDeltaTime();
        void updateTime();
};

// ------- Managers ------- //

// most often used ECSManager type, usually near the root of the program
class Scene: public ECSManager {
    private:
    public:
        Scene(SDL_Renderer* renderer);
        SDLRendererComponent& getRenderer();
        void render() override;
        void update() override;
};

#endif // SDLECS_H