#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>


using std::string;
using std::vector;
using std::ifstream;

#include "tilemap.h"
#include "ecpps.h"

using ecpps::ECSManager;
using ecpps::Component;

// ------- RenderSystem ------- //

void TileMapRenderSystem::init(){
}


void TileMapDataSystem::update(ECSManager* manager){
    // // get component vector from manager
    vector<TileMapDataComponent>& components = manager->getComponentVector<TileMapDataComponent>();
    // loop all components
    for(TileMapDataComponent& component : components){
        // really really bad, fix later
        if(check != nullptr){
            std::cout << check->fileName << std::endl;
        }
    // }
}

// method to load map from file after init
void TileMapData::loadMap(string& fileName){
    ifstream mapFile("res/" + fileName);
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
        this -> mapData = mapData;
        this -> height = height;
        this -> width = width;
    }
}

// ------- DataSystem ------- //

void TileMapDataSystem::loadMap(TileMapDataComponent component){
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

// ------- TileMap ------- //

// empty constructor
TileMap::TileMap(){
    
}

// constructor with instant load
TileMap::TileMap(string fileName, SDL_Texture *roadTexture){
    this -> loadMap(fileName);
    this -> setRoadTexture(roadTexture);
}

void TileMap::render(SDL_Renderer* renderer){
    this -> mapRenderer.render(renderer);
}

// method to load map from file after init
void TileMap::loadMap(string fileName){
    this -> mapData.loadMap(fileName);
}

// method to set road texture after init
void TileMap::setRoadTexture(SDL_Texture *roadTexture){
    this -> mapRenderer.setRoadTexture(roadTexture);
}

// ------- MapData ------- //

// empty constructor
TileMapData::TileMapData(){
}

// constructor with instant load
TileMapData::TileMapData(string& fileName){
    this -> loadMap(fileName);
}

// returns tile at specific position on map
unsigned int TileMapData::getTile(int x, int y){
    try{
        return mapData.at(y).at(x);
    } catch (...) {
        std::cout << "no vector data found at: " << x << ", " << y << std::endl;
        return -1;
    }
}

// ------- MapRenderer ------- //

// empty constructor
TileMapRenderer::TileMapRenderer(){
}

TileMapRenderer::TileMapRenderer(SDL_Texture* roadTexture){
    setRoadTexture(roadTexture);
}

void TileMapRenderer::render(SDL_Renderer* renderer){
    SDL_RenderCopy( renderer, roadTexture, NULL, NULL);
}

void TileMapRenderer::setRoadTexture(SDL_Texture *roadTexture){
    roadTexture = roadTexture;
}
