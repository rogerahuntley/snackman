#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

#include <string>
#include <vector>
#include <map>

#include "sdlecs.h"
#include "ecpps/ecpps.h"

// all stuff relating to tilemaps
// these are tile-based drawable grids of images

using std::string;
using std::vector;
using std::map;

using namespace ecpps;

// ------- Entities ------- //

class TileMapEntity: public Entity {
    private:
        void init() override;
        void init(const char* tmxFile);
    public:
        // constructor
        TileMapEntity(ID entityID, ECSManager* manager): Entity(entityID, manager) { init(); };
        TileMapEntity(ID entityID, ECSManager* manager, const char* tmxFile): Entity(entityID, manager) { init(tmxFile); };
};

// ------- Component ------- //

struct TileMapDataComponent: public Component {
    // holds the name of the file from which to load tilemap data
    const char* tmxFile;
    // holds 2d vector of ints to represent the tilemap
    tmx_map* mapData;
    // collision layer
    tmx_layer* collisionLayer;
    // number of tiles tall
    unsigned int height;
    // number of tiles wide
    unsigned int width;
    // get collision at
    bool getTileEmpty(int x, int y){ return collisionLayer->content.gids[(y*mapData->width)+x]==0; };
};

struct TileMapRenderComponent: public RenderTextureComponent {
    map<unsigned, SDL_Texture*> baseLayers;
};

// ------- Systems ------- //

class TileMapRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(ECSManager* manager) override;
    private:
        void drawAllLayers(SDLRendererComponent& ren, TileMapDataComponent& mapData, TileMapRenderComponent& mapRender, tmx_layer *layer);
        void drawLayer(SDLRendererComponent& ren, TileMapDataComponent& mapData, TileMapRenderComponent& mapRender, tmx_layer *layer);
        void drawLayerBase(SDLRendererComponent& ren, TileMapDataComponent& mapData, TileMapRenderComponent& mapRender, tmx_layer *layer);
        void drawTile(SDLRendererComponent& ren, SDL_Texture* image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, unsigned int flags);
};

class TileMapDataSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void loadMap(TileMapDataComponent& component);
        int getTile(TileMapDataComponent& component, int x, int y);
        void printLayer(tmx_map* map, tmx_layer* layer);
};

#endif // MAP_H