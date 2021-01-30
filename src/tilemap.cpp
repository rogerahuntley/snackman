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
  
	component.mapData = tmx_load(component.tmxFile.c_str());
}

// ------- MapRenderer ------- //

void TileMapRenderSystem::init(ECSManager* manager){
    manager->groupEntities<TileMapRenderComponent>();
}

void TileMapRenderSystem::render(ECSManager* manager){
    // get set of entities
    set<ID>& oentities = manager->getNewComponentEntities<TileMapRenderComponent>();
    set<ID>& entities = manager->getComponentEntities<TileMapRenderComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // get relevant component
        TileMapRenderComponent& mapRender = manager->getComponent<TileMapRenderComponent>(entityID);
        TileMapDataComponent& mapData = manager->getComponent<TileMapDataComponent>(entityID);
        // get renderer
        SDLRendererComponent& renderer = manager->getComponent<SDLRendererComponent>();
        // render
        drawAllLayers(renderer, mapData.mapData, mapData.mapData->ly_head);
    }
}

void TileMapRenderSystem::drawAllLayers(SDLRendererComponent& ren, tmx_map *map, tmx_layer *layers) {
  while (layers) {
    if (layers->visible) {

      if (layers->type == L_GROUP) {
        drawAllLayers(ren, map, layers->content.group_head); // recursive call
      }
      else if (layers->type == L_OBJGR) {
        //draw_objects(layers->content.objgr); // Function to be implemented
      }
      else if (layers->type == L_IMAGE) {
        //draw_image_layer(layers->content.image); // Function to be implemented
      }
      else if (layers->type == L_LAYER) {
        draw_layer(ren, map, layers); // Function to be implemented
      }
    }
    layers = layers->next;
  }

}

// returns tile at specific position on map
void TileMapRenderSystem::draw_layer(SDLRendererComponent& ren, tmx_map *map, tmx_layer *layer) {
  string cout;
  unsigned long i, j;
  unsigned int gid, x, y, w, h, flags;
  float op;
  tmx_tileset *ts;
  tmx_image *im;
  void* image;
  op = layer->opacity;
  for (i=0; i<map->height; i++) {
    for (j=0; j<map->width; j++) {
      gid = (layer->content.gids[(i*map->width)+j]) & TMX_FLIP_BITS_REMOVAL;
      if (map->tiles[gid] != NULL) {
        unsigned id = map->tiles[gid]->id;
        if(j == 0){
          
          cout += "\n";
        }
        if(id == 0){
          cout += "##";
        } else {
          cout += "  ";
        }
				ts = map->tiles[gid]->tileset;
				im = map->tiles[gid]->image;
				x  = map->tiles[gid]->ul_x;
				y  = map->tiles[gid]->ul_y;
				w  = ts->tile_width;
				h  = ts->tile_height;
        SDL_Texture* image2 = ren.loadTexture(ts->image->source);
				if (im) {
					image = im->resource_image;
				}
				else {
					image = ts->image->resource_image;
				}
        flags = (layer->content.gids[(i*map->width)+j]) & ~TMX_FLIP_BITS_REMOVAL;
        draw_tile(ren, image2, x, y, w, h, j*ts->tile_width, i*ts->tile_height, op, flags);
      }
    }
  }
  std::cout << cout << std::endl;
}

void TileMapRenderSystem::draw_tile(SDLRendererComponent& ren, SDL_Texture* image, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
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