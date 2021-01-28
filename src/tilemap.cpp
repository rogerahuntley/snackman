#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <set>

using std::string;
using std::vector;
using std::set;
using std::ifstream;

#include "tilemap.h"
#include "ecpps.h"

using namespace ecpps;

// ------- TileMapDataSystem ------- //

// a good example
// init loop for tilemapdata
void TileMapDataSystem::init(ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getNewComponentEntities<TileMapDataComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // get relevant component
        TileMapDataComponent& tileMap = manager->getComponent<TileMapDataComponent>(entityID);
        // load map
        loadMap(tileMap);
    }
    // release init components into regular pools
    manager->groupEntities<TileMapDataSystem>();
}

// loads tile data from a file
void TileMapDataSystem::loadMap(TileMapDataComponent& component){
    ifstream mapFile("res/" + component.fileName);
    if(mapFile.is_open()){
        string line;
        int width, height;

        vector<vector<int>> mapData;
        mapData.reserve(height);

        // stream into width/height
        mapFile >> width >> height;

        // for each line
        while(mapFile >> line){

            // setup 1d vector
            vector<int> mapLine;
            mapLine.reserve(width);

            // loop through chars, add to vector as ints
            for(char& c : line){
                mapLine.push_back(1);
            }

            // add 1d vector to 2d vector
            mapData.push_back(mapLine);
        }

        // save data to object
        component.mapData = mapData;
        component.height = height;
        component.width = width;
    }
}

// returns tile at specific position on map
int TileMapDataSystem::getTile(TileMapDataComponent& component, int x, int y){
    try{
        return component.mapData.at(y).at(x);
    } catch (...) {
        std::cout << "no vector data found at: " << x << ", " << y << std::endl;
        return -1;
    }
}

// ------- MapRenderer ------- //

void TileMapRenderSystem::init(ECSManager* manager){
    manager->groupEntities<TileMapRenderComponent>();
}

void TileMapRenderSystem::render(SDL_Renderer* renderer, ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<TileMapRenderComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // get relevant component
        TileMapRenderComponent& roadRender = manager->getComponent<TileMapRenderComponent>(entityID);
        // render
        SDL_RenderCopy( renderer, roadRender.roadTexture, NULL, NULL);
    }
}
