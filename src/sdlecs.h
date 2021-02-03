#ifndef SDLECS_H
#define SDLECS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <map>
#include <set>

#include "ecpps/ecpps.h"

// sdl helper class
// provides and interface between ECS and SDL

using std::string;
using std::map;
using std::set;

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
        SDL_Event event;
        set<SDL_Scancode> keyUp;
        set<SDL_Scancode> keyPressed;
        set<SDL_Scancode> keyDown;
    public:
        SDLEventComponent() {};
        bool isUp(SDL_Scancode key) { return keyUp.find(key) != keyUp.end(); };
        bool isPressed(SDL_Scancode key) { return keyPressed.find(key) != keyPressed.end(); };
        bool isDown(SDL_Scancode key) { return keyDown.find(key) != keyDown.end(); };
        void flushEvents();
};

struct SDLDeltaTimeComponent: public Component {
    private:
        Uint64 currentTime;
        Uint64 lastTime = 0;
        double deltaTime = 0.0;
    public:
        SDLDeltaTimeComponent(): currentTime(SDL_GetPerformanceCounter()),lastTime(SDL_GetPerformanceCounter()) {};
        double getDeltaTimeS();
        double getDeltaTimeMS();
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