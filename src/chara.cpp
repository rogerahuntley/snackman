#include "chara.h"
#include "sdlecs.h"
#include "ecpps/ecpps.h"
#include "gcomp.h"

using namespace ecpps;

// // ------- PacmanEntity ------- //

void PacmanEntity::init(){
    // instantiate position
    PositionComponent position(16, 16);

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

    switch (character.direction)
    {
    case UP:
        pos.y -= moveDist;
        break;
    case RIGHT:
        pos.x += moveDist;
        break;
    case DOWN:
        pos.y += moveDist;
        break;
    case LEFT:
        pos.x -= moveDist;
        break;
    case NONE:
        /* code */
        break;
    };
    // 1 is true, 0 if false
    //pos.x += state[SDL_SCANCODE_D]* + -state[SDL_SCANCODE_A]*character.speed*deltaTime.getDeltaTimeS();
    //pos.y += state[SDL_SCANCODE_S]*character.speed*deltaTime.getDeltaTimeS() + -state[SDL_SCANCODE_W]*character.speed*deltaTime.getDeltaTimeS();
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
