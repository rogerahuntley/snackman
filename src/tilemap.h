#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "ecpps.h"

using std::string;
using std::vector;

using namespace ecpps;

// move this out later
class Renderer {
    public:
        virtual void render(SDL_Renderer* renderer)=0;
};

// holds data for map
class TileMapData{
    private:
        // holds 2d vector of ints to represent the tilemap
        vector<vector<int>> mapData;
        // number of tiles tall
        unsigned int height;
        // number of tiles wide
        unsigned int width;
    public:
        TileMapData();
        TileMapData(string& fileName);
        void loadMap(string& fileName);
        unsigned int getTile(int x, int y);
};

struct TileMapRenderComponent: public RenderComponent {
        // holds texture for road (lol)
        SDL_Texture* roadTexture;
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
        void init();
};

class TileMapDataSystem: public System {
    public:
        void update(ECSManager* manager) override;
        void loadMap(TileMapDataComponent component);
};

class TileMapRenderer: public Renderer {
    private:
        // holds texture for road (lol)
        SDL_Texture* roadTexture;
    public:
        TileMapRenderer();
        TileMapRenderer(SDL_Texture* roadTexture);
        void render(SDL_Renderer* renderer);
        void setRoadTexture(SDL_Texture *roadTexture);
};

class TileMap{
    private:
        // renders the map
        TileMapRenderer mapRenderer;
        // holds the map data
        TileMapData mapData;
    public:
        TileMap();
        TileMap(string fileName, SDL_Texture *roadTexture);
        void render(SDL_Renderer* renderer);
        void loadMap(string fileName);
        void setRoadTexture(SDL_Texture *roadTexture);
};

#endif // MAP_H