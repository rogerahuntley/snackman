#include "chara.h"
#include "sdlecs.h"
#include "ecpps/ecpps.h"
#include "gcomp.h"
#include "tilemap.h"

#include <math.h>

using namespace ecpps;

// // ------- PacmanEntity ------- //

void PacmanEntity::init(){
    // instantiate position
    PositionComponent position(16, 64);

    // instantiate scale
    ScaleComponent scale(16, 16);

    // instantiate rotation
    RotationComponent rotation;

    // instantiate chracter component
    CharacterComponent character;
    character.speed = 64.0f;

    // add components to tilemap
    addComponent(position);
    addComponent(scale);
    addComponent(rotation);
    addComponent(character);

};

// ------- CharacterControlSystem ------- //

void CharacterControlSystem::update(ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<CharacterComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // update
        moveCharacter(manager, entityID);
    }
};

void CharacterControlSystem::moveCharacter(ECSManager* manager, ID entityID){
    // get event components
    SDLDeltaTimeComponent& deltaTime = manager->getComponent<SDLDeltaTimeComponent>();
    SDLEventComponent& event = manager->getComponent<SDLEventComponent>();
    CharacterComponent& character = manager->getComponent<CharacterComponent>(entityID);
    PositionComponent& pos = manager->getComponent<PositionComponent>(entityID);
    ScaleComponent& scale = manager->getComponent<ScaleComponent>(entityID);
    RotationComponent& rot = manager->getComponent<RotationComponent>(entityID);

    double moveDist = character.speed * (double)deltaTime.getDeltaTimeS();

    
    PositionComponent changeVector;

    switch (character.direction)
    {
    case UP:
        changeVector.y -= moveDist;
        break;
    case RIGHT:
        changeVector.x += moveDist;
        break;
    case DOWN:
        changeVector.y += moveDist;
        break;
    case LEFT:
        changeVector.x -= moveDist;
        break;
    }
    applyVector(manager, entityID, changeVector);
};

bool CharacterControlSystem::applyVector(ECSManager* manager, ID entityID, PositionComponent& vector){

};

bool checkTileCollision(ECSManager* manager, ID entityID, PositionComponent position){
    
};

void CharacterControlSystem::init(ECSManager* manager){
    // tell manager that these components are ready
    manager->groupEntities<CharacterComponent>();
    manager->groupEntities<PositionComponent>();
    manager->groupEntities<ScaleComponent>();
    manager->groupEntities<RotationComponent>();
};

// ------- ChracterControllerSystem ------- //

bool CharacterControllerSystem::updateDirection(ECSManager* manager, ID entityID, DIRECTION direction){
    // normally direction would be a Vector2 and would be normalized
    // but with pacman it's gonna be something different
    // we'll check in the controller system if pacman is in a position to be able to change his direction
    // and if so, we change that mf direction
    // note: we also have to consider his distance when changing direction
    // we want him aligned to the tile edge parallel to his direction
    CharacterComponent& character = manager->getComponent<CharacterComponent>(entityID);

    // check if changed
    bool isChanged = false;
    if(character.direction != direction){
        // if changed, set return val
        isChanged = true;
        // set direction
        character.direction = direction;
    }
    // return val
    return isChanged;
};

set<DIRECTION> CharacterControllerSystem::getPossibleDirections(ECSManager* manager, ID entityID){
    // assume first tilemap is only tilemap - can be fixed later if needed, not in this proj
    ID tilemapEntity = manager->getSpecialEntity("tilemap");
    TileMapDataComponent mapData = manager->getComponent<TileMapDataComponent>(tilemapEntity);

    // weird but we can use component just as regular objects too
    PositionComponent mapPosition = getCurrentTile(manager, entityID);

    bool upPosition = mapData.getTileEmpty(mapPosition.x, mapPosition.y);
    bool rightPosition = mapData.getTileEmpty(mapPosition.x+1, mapPosition.y);
    bool downPosition = mapData.getTileEmpty(mapPosition.x, mapPosition.y+1);
    bool leftPosition = mapData.getTileEmpty(mapPosition.x, mapPosition.y);

    set<DIRECTION> possibleDirections;

    if(upPosition) { possibleDirections.insert(UP); };
    if(rightPosition) { possibleDirections.insert(RIGHT); };
    if(downPosition) { possibleDirections.insert(DOWN); };
    if(leftPosition) { possibleDirections.insert(LEFT); };

    return possibleDirections;
}

PositionComponent CharacterControllerSystem::getCurrentTile(ECSManager* manager, ID entityID){
    // defined outside, make dynamic later
    const float widthheight = 16.0;
    // get regular chracter posistion
    PositionComponent& characterPos = manager->getComponent<PositionComponent>(entityID);
    ScaleComponent& characterScale = manager->getComponent<ScaleComponent>(entityID);

    PositionComponent tilemapPos = { (int)floor(characterPos.x / widthheight), (int)floor(characterPos.y / widthheight) };

    std::cout << "x: " << tilemapPos.x << ", y: " << tilemapPos.y << std::endl;

    return tilemapPos;
}

// ------- PlayerControllerSystem ------- //

void PlayerControllerSystem::update(ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<CharacterComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // update
        updateInput(manager, entityID);
    }
};

void PlayerControllerSystem::updateInput(ECSManager* manager, ID entityID){
    // get components
    SDLDeltaTimeComponent deltaTime = manager->getComponent<SDLDeltaTimeComponent>();
    SDLEventComponent event = manager->getComponent<SDLEventComponent>();

    if(event.isDown(SDL_SCANCODE_W)){
        updateDirection(manager, entityID, UP);
    }
    if(event.isDown(SDL_SCANCODE_D)){
        updateDirection(manager, entityID, RIGHT);
    }
    if(event.isDown(SDL_SCANCODE_S)){
        updateDirection(manager, entityID, DOWN);
    }
    if(event.isDown(SDL_SCANCODE_A)){
        updateDirection(manager, entityID, LEFT);
    }

    CharacterComponent& character = manager->getComponent<CharacterComponent>(entityID);

    set<DIRECTION> directions = getPossibleDirections(manager, entityID);
    // if direction is not possible
    if(directions.find(character.direction) == directions.end()){
        updateDirection(manager, entityID, NONE);
    }
};

// ------- CharacterRenderSystem ------- //

void CharacterRenderSystem::render(ECSManager* manager){
    // get renderer
    SDLRendererComponent& renderer = manager->getComponent<SDLRendererComponent>();
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<CharacterComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // render
        drawCharacter(manager, entityID);
    }
};

void CharacterRenderSystem::drawCharacter(ECSManager* manager, ID entityID){
    // get renderer
    SDLRendererComponent& renderer = manager->getComponent<SDLRendererComponent>();
    // get relevant components
    PositionComponent& pos = manager->getComponent<PositionComponent>(entityID);
    ScaleComponent& scale = manager->getComponent<ScaleComponent>(entityID);
    RotationComponent& rot = manager->getComponent<RotationComponent>(entityID);
    // draw square
    SDL_Rect fillRect = { (int)pos.x, (int)pos.y, (int)scale.x, (int)scale.y };
    SDL_SetRenderDrawColor( renderer.getRenderer(), 0xFF, 0x00, 0x00, 0xFF );        
    SDL_RenderFillRect( renderer.getRenderer(), &fillRect );
}

void CharacterRenderSystem::init(ECSManager* manager){
};
