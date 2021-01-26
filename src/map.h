#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
using std::string;
using std::vector;

// holds data for map
class MapData{
    private:
        vector<vector<int>> mapData;
        int height;
        int width;
    public:
        MapData();
        MapData(string fileName);
        void loadMap(string fileName);
        int getTile(int x, int y);
};

class Map{
    public:
        Map();
        Map(string fileName);
        void loadMap(string fileName);
    private:
        MapData mapData;
};

class MapRenderer{


};

#endif // MAP_H