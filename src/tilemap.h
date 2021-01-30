#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

#include <string>
#include <vector>

#include "sdlecs.h"
#include "ecpps/ecpps.h"

// all stuff relating to tilemaps
// these are tile-based drawable grids of images

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
        string tmxFile;
        // holds 2d vector of ints to represent the tilemap
        tmx_map* mapData;
        // number of tiles tall
        unsigned int height;
        // number of tiles wide
        unsigned int width;
};

class TileMapRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(ECSManager* manager) override;
    private:
        void drawAllLayers(SDLRendererComponent& ren, tmx_map *map, tmx_layer *layer);
        void draw_layer(SDLRendererComponent& ren, tmx_map *map, tmx_layer *layer);
        void draw_tile(SDLRendererComponent& ren, SDL_Texture* image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags);

};

struct TileMapRenderComponent: public RenderComponent {
        // holds texture for road (lol)
        SDL_Texture* roadTexture;
};

#endif // MAP_H