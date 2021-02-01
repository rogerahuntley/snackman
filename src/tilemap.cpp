#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <tmx.h>

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
#include "sdlecs.h"
#include "ecpps/ecpps.h"

using namespace ecpps;

const string COLLISION_LAYER("collision");

typedef TileMapRenderSystem TMRenderSystem;
typedef TileMapDataSystem TMDataSystem;
typedef TileMapRenderComponent TMRenderComp;
typedef TileMapDataComponent TMDataComp;

// ------- TileMapEntity ------- //

void TileMapEntity::init(){
    // instantiate tilemap data component
    TileMapDataComponent mapData;

    // instantiate tilemap renderer component
    TileMapRenderComponent mapRenderer;

    // add components to tilemap
    addComponent(mapRenderer);
    addComponent(mapData);
}

void TileMapEntity::init(const char* tmxFile){
    init();
    TMDataComp& mapData = manager.getComponent<TMDataComp>(entityID);
    mapData.tmxFile = tmxFile;
}

// ------- TileMapDataSystem ------- //

// a good example
// init loop for tilemapdata
void TMDataSystem::init(ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getNewComponentEntities<TMDataComp>();
    // for each entity in set
    for(ID entityID : entities){
        // get relevant component
        TMDataComp& tileMap = manager->getComponent<TMDataComp>(entityID);
        // load map
        loadMap(tileMap);
        // print map
        printLayer(tileMap.mapData, tileMap.collisionLayer);
    }
    // release init components into regular pools
    manager->groupEntities<TMDataComp>();
}

// loads tile data from a file
void TMDataSystem::loadMap(TMDataComp& component){
    // load data using tmx lib
    component.mapData = tmx_load(component.tmxFile);
    // get collision layer
    tmx_layer* layers = component.mapData->ly_head;
    // find collision layer
    while(layers){
        // if layer is the same as designated collision layer
        if(layers->name == COLLISION_LAYER){
            // save to component
            component.collisionLayer = layers;
            // return to stop while loop
            //return;
        }
        // next iteration
        layers = layers->next;
    }
}

// print to console
void TMDataSystem::printLayer(tmx_map* map, tmx_layer* layer){
    // if layer is set
    if(layer == nullptr){
    return;
    }
    // string for outputting to console (remove later)
    string cout = "collision map:";

    // initalize vars
    unsigned int x, y, gid;

    // loop through all map tiles
    for (x=0; x<map->height; x++) {
        for (y=0; y<map->width; y++) {
            // set gid
            gid = (layer->content.gids[(x*map->width)+y]) & TMX_FLIP_BITS_REMOVAL;
            // add to cout
            if(y == 0){ cout += "\n"; }
            if(gid != 0){  cout += "##"; } else { cout += "  "; }
        }
    }
    // print cout
    std::cout << cout << std::endl;
}

// ------- TileMapRenderSystem ------- //

void TMRenderSystem::init(ECSManager* manager){
    // tell manager that these components are ready
    manager->groupEntities<TMRenderComp>();
}

void TMRenderSystem::render(ECSManager* manager){
    // get renderer
    SDLRendererComponent& renderer = manager->getComponent<SDLRendererComponent>();
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<TMRenderComp>();
    // for each entity in set
    for(ID entityID : entities){
        // get relevant components
        TMRenderComp& mapRender = manager->getComponent<TMRenderComp>(entityID);
        TMDataComp& mapData = manager->getComponent<TMDataComp>(entityID);
        // render
        drawAllLayers(renderer, mapData, mapRender, mapData.mapData->ly_head);
    }
}

void TMRenderSystem::drawAllLayers(SDLRendererComponent& ren, TMDataComp& mapData, TMRenderComp& mapRender, tmx_layer *layers) {
    // for every later
    while (layers) {
        if (layers->visible) {
            // draw collection of layers (recursive)
            if (layers->type == L_GROUP) {
                drawAllLayers(ren, mapData, mapRender, layers->content.group_head);
            }
            else if (layers->type == L_OBJGR) {
                //draw_objects(layers->content.objgr);
            }
            else if (layers->type == L_IMAGE) {
                //draw_image_layer(layers->content.image);
            }
            else if (layers->type == L_LAYER) {
                drawLayer(ren, mapData, mapRender, layers);
            }
        }
        // pass to next later
        layers = layers->next;
    }
}

void TMRenderSystem::drawLayer(SDLRendererComponent& ren, TMDataComp& mapData, TMRenderComp& mapRender, tmx_layer* layer){
    SDL_Texture* texture;
    SDL_Renderer* renderer = ren.getRenderer();
    // check if entry exists
    if(mapRender.baseLayers.find(layer->id) != mapRender.baseLayers.end()){
        //if so, pull from entry
        texture = mapRender.baseLayers.at(layer->id);
    } else {
        // else, create entry
        // start with texture
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 480, 272);
        // send blend mode for transparent
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        // set alpha
        SDL_SetTextureAlphaMod(texture, 255);
        // set proper target
        SDL_SetRenderTarget(renderer, texture);
        // clear
        SDL_RenderClear(renderer);
        // draw
        drawLayerBase(ren, mapData, mapRender, layer);
        // set regular target
        SDL_SetRenderTarget(renderer, NULL);
        // add to map
        mapRender.baseLayers.insert({layer->id, texture});
    }
    // draw texture
    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

// returns tile at specific position on map
void TMRenderSystem::drawLayerBase(SDLRendererComponent& ren, TMDataComp& mapData, TMRenderComp& mapRender, tmx_layer* layer) {
    // ayy load map data
    tmx_map* map = mapData.mapData;
    // get vars ready
    unsigned int x, y;
    unsigned int gid, sx, sy, w, h, flags;
    float op;
    tmx_tileset* tileset;
    SDL_Texture* image;
    op = layer->opacity;

    // loop through all map tiles
    for (x=0; x<map->height; x++) {
        for (y=0; y<map->width; y++) {
            // get tile gid
            gid = (layer->content.gids[(x*map->width)+y]) & TMX_FLIP_BITS_REMOVAL;
            // if tile is valid
            if (map->tiles[gid] != NULL) {

                // pull data for tile
                tileset = map->tiles[gid]->tileset;
                image = ren.loadTexture(tileset->image->source);
                sx  = map->tiles[gid]->ul_x;
                sy  = map->tiles[gid]->ul_y;
                w  = tileset->tile_width;
                h  = tileset->tile_height;

                // draw tile
                drawTile(ren, image, sx, sy, w, h, y*tileset->tile_width, x*tileset->tile_height, op, gid);
            }
        }
    }
    SDL_Delay(100);
}

void TMRenderSystem::drawTile(SDLRendererComponent& ren, SDL_Texture* image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               unsigned int dx, unsigned int dy, float opacity, unsigned int flags) {
	SDL_Rect src_rect, dest_rect;
	src_rect.x = sx;
	src_rect.y = sy;
	src_rect.w = dest_rect.w = sw;
	src_rect.h = dest_rect.h = sh;
	dest_rect.x = dx;
	dest_rect.y = dy;
	SDL_RenderCopy(ren.getRenderer(), image, &src_rect, &dest_rect);
}