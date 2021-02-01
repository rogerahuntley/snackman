#ifndef CHARA_H
#define CHARA_H

#include "ecpps/ecpps.h"
#include "sdlecs.h"
#include "gcomp.h"

// this is stuff relating to the characters
// i.e., pacman and the ghosts

using namespace ecpps;

// ------- Entities ------- //

class PacmanEntity: public Entity {
    private:
        void init() override;
    public:
        // constructor
        PacmanEntity(ID entityID, ECSManager* manager): Entity(entityID, manager) { init(); };
};

class GhostEntity: public Entity {
    private:
        void init() override;
    public:
        // constructor
        GhostEntity(ID entityID, ECSManager* manager): Entity(entityID, manager) { init(); };
};

// ------- Components ------- //

struct CharacterComponent: public Component {

};

// ------- Systems ------- //

class CharacterRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(ECSManager* manager) override;
    private:
        void drawCharacter(SDLRendererComponent& renderer, PositionComponent& pos, ScaleComponent& scale, RotationComponent& rot);
};

class CharacterControlSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void update(ECSManager* manager) override;
    private:
        void moveCharacter(SDLEventComponent& event, PositionComponent& pos, ScaleComponent& scale, RotationComponent& rot);
};

#endif // CHARA_H