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
    character.speed = 4.1f;

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
        // get relevant components
        // render
        moveCharacter(manager, entityID);
    }
};

void CharacterControlSystem::moveCharacter(ECSManager* manager, ID entityID){
    // get event components
    SDLEventComponent& event = manager->getComponent<SDLEventComponent>();
    CharacterComponent& character = manager->getComponent<CharacterComponent>(entityID);
    PositionComponent& pos = manager->getComponent<PositionComponent>(entityID);
    ScaleComponent& scale = manager->getComponent<ScaleComponent>(entityID);
    RotationComponent& rot = manager->getComponent<RotationComponent>(entityID);
    // get keyboard state
    const Uint8* state(event.getState());

    // 1 is true, 0 if false
    pos.x += state[SDL_SCANCODE_D]*character.speed*.01 + -state[SDL_SCANCODE_A]*character.speed*.01;
    pos.y += state[SDL_SCANCODE_S]*character.speed*.01 + -state[SDL_SCANCODE_W]*character.speed*.01;
};

void CharacterControlSystem::init(ECSManager* manager){
    // tell manager that these components are ready
    manager->groupEntities<CharacterComponent>();
    manager->groupEntities<PositionComponent>();
    manager->groupEntities<ScaleComponent>();
    manager->groupEntities<RotationComponent>();
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
