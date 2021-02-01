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
    // get event component
    SDLEventComponent& event = manager->getComponent<SDLEventComponent>();
    // for each entity in set
    for(ID entityID : entities){
        // get relevant components
        PositionComponent& pos = manager->getComponent<PositionComponent>(entityID);
        ScaleComponent& scale = manager->getComponent<ScaleComponent>(entityID);
        RotationComponent& rot = manager->getComponent<RotationComponent>(entityID);
        // render
        moveCharacter(event, pos, scale, rot);
    }
};

void CharacterControlSystem::moveCharacter(SDLEventComponent event, PositionComponent& pos, ScaleComponent& scale, RotationComponent& rot){
    pos.x += .01;
    pos.y += .001;
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
        // get relevant components
        PositionComponent& pos = manager->getComponent<PositionComponent>(entityID);
        ScaleComponent& scale = manager->getComponent<ScaleComponent>(entityID);
        RotationComponent& rot = manager->getComponent<RotationComponent>(entityID);
        // render
        drawCharacter(renderer, pos, scale, rot);
    }
};

void CharacterRenderSystem::drawCharacter(SDLRendererComponent& renderer, PositionComponent& pos, ScaleComponent& scale, RotationComponent& rot){
    // draw square
    SDL_Rect fillRect = { (int)pos.x, (int)pos.y, (int)scale.x, (int)scale.y };
    SDL_SetRenderDrawColor( renderer.getRenderer(), 0xFF, 0x00, 0x00, 0xFF );        
    SDL_RenderFillRect( renderer.getRenderer(), &fillRect );
}

void CharacterRenderSystem::init(ECSManager* manager){
};
