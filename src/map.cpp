#include "map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::ifstream;

// ------- Map ------- //

// empty constructor
Map::Map(){
}

Map::Map(string fileName){
    this -> mapData.loadMap(fileName);
}

void Map::loadMap(string fileName){
    this -> mapData.loadMap(fileName);
}

// ------- MapData ------- //

// empty constructor
MapData::MapData(){
}

// constructor with load
MapData::MapData(string fileName){
    this -> loadMap(fileName);
}

// load map from filename
void MapData::loadMap(string fileName){
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

// returns tile at specific position on map
int MapData::getTile(int x, int y){
    try{
        return mapData.at(y).at(x);
    } catch (...) {
        std::cout << "no vector data found at: " << x << ", " << y << std::endl;
        return -1;
    }
}

// ------- MapRenderer ------- //

