#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "ecpps.h"

using std::string;
using std::vector;

using namespace ecpps;

class TileMapDataComponent;

class TileMapDataSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void loadMap(TileMapDataComponent& component);
        int getTile(TileMapDataComponent& component, int x, int y);
};

struct TileMapDataComponent: public Component {
        // holds the name of the file from which to load tilemap data
        string fileName;
        // holds 2d vector of ints to represent the tilemap
        vector<vector<int>> mapData;
        // number of tiles tall
        unsigned int height;
        // number of tiles wide
        unsigned int width;
};

class TileMapRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(SDL_Renderer* renderer, ECSManager* manager) override;
};

struct TileMapRenderComponent: public RenderComponent {
        // holds texture for road (lol)
        SDL_Texture* roadTexture;
};

#endif // MAP_H