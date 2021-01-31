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

// ------- TileMapDataSystem ------- //

typedef TileMapRenderSystem TMRenderSystem;
typedef TileMapDataSystem TMDataSystem;
typedef TileMapRenderComponent TMRenderComp;
typedef TileMapDataComponent TMDataComp;


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
  }
  // release init components into regular pools
  manager->groupEntities<TMDataSystem>();
}

// loads tile data from a file
void TMDataSystem::loadMap(TMDataComp& component){
  // load data using tmx lib
	component.mapData = tmx_load(component.tmxFile.c_str());
}

// ------- TileMapRenderSystem ------- //

void TMRenderSystem::init(ECSManager* manager){
  // tell manager that these components are ready
  manager->groupEntities<TMRenderComp>();
}

void TMRenderSystem::render(ECSManager* manager){
  // get set of entities
  set<ID>& entities = manager->getComponentEntities<TMRenderComp>();
  // get renderer
  SDLRendererComponent& renderer = manager->getComponent<SDLRendererComponent>();
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
  // check if entry exists
  if(mapRender.baseLayers.find(layer->id) != mapRender.baseLayers.end()){
    //if so, pull from entry
    texture = mapRender.baseLayers.at(layer->id);
  } else {
    // else, create entry
    // start with texture
    texture = SDL_CreateTexture(ren.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 480, 272);
    // send blend mode for transparent
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    // set alpha
    SDL_SetTextureAlphaMod(texture, 255);
    // set proper target
    SDL_SetRenderTarget(ren.renderer, texture);
    // set color to white
	  //SDL_SetRenderDrawColor(ren.renderer, 255, 255, 255, 255);
    // clear
    SDL_RenderClear(ren.renderer);
    // draw
    drawLayerBase(ren, mapData, mapRender, layer);
    // set regular target
    SDL_SetRenderTarget(ren.renderer, NULL);
    // add to map
    mapRender.baseLayers.insert({layer->id, texture});
  }
  // draw texture
	SDL_RenderCopy(ren.renderer, texture, NULL, NULL);
}

// returns tile at specific position on map
void TMRenderSystem::drawLayerBase(SDLRendererComponent& ren, TMDataComp& mapData, TMRenderComp& mapRender, tmx_layer* layer) {
  // string for outputting to console (remove later)
  string cout;
  // ayy
  tmx_map* map = mapData.mapData;
  
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
        // get title id for printing
        unsigned id = map->tiles[gid]->id;
        if(y == 0){ cout += "\n"; } // remove later
        if(id == 0){  cout += "##"; } else { cout += "  "; } // remove later

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
  std::cout << cout << std::endl;
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
	SDL_RenderCopy(ren.renderer, image, &src_rect, &dest_rect);
}