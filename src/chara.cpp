#include "chara.h"
#include "sdlecs.h"
#include "gcomp.h"
#include "tilemap.h"

#include "ecpps/ecpps.h"
#include "physicpps/physicpps.h"

#include <math.h>
#include <utility>

using std::pair;

using namespace ecpps;
using namespace physicpps;

// // ------- PacmanEntity ------- //

void PacmanEntity::init(float x, float y){
    // instantiate position
    PositionComponent position(x, y);

    // instantiate scale
    ScaleComponent scale;

    // instantiate rotation
    RotationComponent rotation;

    // instantiate chracter component
    CharacterComponent character;
    character.speed = 64.0f;

    // instantiate player controller
    PlayerControlComponent playerControl;

    // add components to tilemap
    addComponent(position);
    addComponent(scale);
    addComponent(rotation);
    addComponent(character);
    addComponent(playerControl);
};

// // ------- GhostEntity ------- //

void GhostEntity::init(float x, float y){
    // instantiate position
    PositionComponent position(x, y);

    // instantiate scale
    ScaleComponent scale;

    // instantiate rotation
    RotationComponent rotation;

    // instantiate chracter component
    CharacterComponent character;
    character.speed = 64.0f;

    // instantiate ai controller
    AIControlComponent aiControl;

    // add components to tilemap
    addComponent(position);
    addComponent(scale);
    addComponent(rotation);
    addComponent(character);
    addComponent(aiControl);
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
    CharacterComponent& character = manager->getComponent<CharacterComponent>(entityID);
    PositionComponent& position = manager->getComponent<PositionComponent>(entityID);

    double moveDist = character.speed * (double)deltaTime.getDeltaTimeS();

    pair<int, int> mapCoord = getTile(manager, entityID);
    if(mapCoord != character.mapCoord || character.direction == NONE){
        if(character.direction != character.newDirection){
            position.x = mapCoord.first * 16;
            position.y = mapCoord.second * 16;
            character.direction = character.newDirection;
        }
        character.mapCoord = mapCoord;
    }


    Vector2 changeVector;
    switch (character.direction)
    {
    case UP:
        changeVector.y = -moveDist;
        break;
    case RIGHT:
        changeVector.x = moveDist;
        break;
    case DOWN:
        changeVector.y = moveDist;
        break;
    case LEFT:
        changeVector.x = -moveDist;
        break;
    }
    applyVector(manager, entityID, changeVector);
};

pair<int, int> CharacterControlSystem::getTile(ECSManager* manager, ID entityID){
    // get map data
    TileMapDataComponent mapData = manager->getComponent<TileMapDataComponent>(manager->getSpecialEntity("tilemap"));
    int height = mapData.mapData->tile_height;
    int width = mapData.mapData->tile_width;

    PositionComponent& position = manager->getComponent<PositionComponent>(entityID);
    int x = floor(position.x / width) * width;
    int y = floor(position.y / height) * height;
    pair<int, int> toReturn(x, y);
    return toReturn;
}

bool getCollisionAtTile(ECSManager* manager, ID entityID, pair<int, int> tile){
    return false;
}

void CharacterControlSystem::applyVector(ECSManager* manager, ID entityID, Vector2& vector){
    
    PositionComponent& pos = manager->getComponent<PositionComponent>(entityID);
    pos.x += vector.x;
    pos.y += vector.y;
};

bool checkTileCollision(ECSManager* manager, ID entityID, PositionComponent position){
    return false;
};

void CharacterControlSystem::init(ECSManager* manager){
    // tell manager that these components are ready
    manager->groupEntities<CharacterComponent>();
    manager->groupEntities<PositionComponent>();
    manager->groupEntities<ScaleComponent>();
    manager->groupEntities<RotationComponent>();
};

// ------- CharacterComponent ------- //

// ------- PlayerControllerSystem ------- //

void PlayerControllerSystem::init(ECSManager* manager){
    // tell manager that these components are ready
    manager->groupEntities<PlayerControlComponent>();
}

void PlayerControllerSystem::update(ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<PlayerControlComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // update
        updateInput(manager, entityID);
    }
};

void PlayerControllerSystem::updateInput(ECSManager* manager, ID entityID){
    // get components
    SDLDeltaTimeComponent& deltaTime = manager->getComponent<SDLDeltaTimeComponent>();
    SDLEventComponent& event = manager->getComponent<SDLEventComponent>();
    CharacterComponent& character = manager ->getComponent<CharacterComponent>();

    if(event.isDown(SDL_SCANCODE_W)){
        character.newDirection = UP;
    }
    if(event.isDown(SDL_SCANCODE_D)){
        character.newDirection = RIGHT;
    }
    if(event.isDown(SDL_SCANCODE_S)){
        character.newDirection = DOWN;
    }
    if(event.isDown(SDL_SCANCODE_A)){
        character.newDirection = LEFT;
    }

    
    PositionComponent& pos = manager ->getComponent<PositionComponent>();
    std::cout << "x: " << pos.x << ", y: " << pos.y << std::endl;
};

// ------- AIControllerSystem ------- //

void AIControllerSystem::init(ECSManager* manager){
    // tell manager that these components are ready
    manager->groupEntities<AIControlComponent>();
}

void AIControllerSystem::update(ECSManager* manager){
    // get set of entities
    set<ID>& entities = manager->getComponentEntities<AIControlComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // update
        updateInput(manager, entityID);
    }
};

void AIControllerSystem::updateInput(ECSManager* manager, ID entityID){
    // get components
    SDLDeltaTimeComponent& deltaTime = manager->getComponent<SDLDeltaTimeComponent>();
    SDLEventComponent& event = manager->getComponent<SDLEventComponent>();
    CharacterComponent& character = manager ->getComponent<CharacterComponent>();

    if(event.isDown(SDL_SCANCODE_W)){
        character.newDirection = UP;
    }
    if(event.isDown(SDL_SCANCODE_S)){
        character.newDirection = DOWN;
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
    CharacterComponent& character = manager->getComponent<CharacterComponent>(entityID);
    // draw square
    SDL_Rect fillRect = { (int)pos.x, (int)pos.y, (int)character.width, (int)character.height };
    SDL_SetRenderDrawColor( renderer.getRenderer(), 0xFF, 0x00, 0x00, 0xFF );        
    SDL_RenderFillRect( renderer.getRenderer(), &fillRect );
}

void CharacterRenderSystem::init(ECSManager* manager){
};
