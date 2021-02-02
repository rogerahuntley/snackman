#ifndef CHARA_H
#define CHARA_H

#include "ecpps/ecpps.h"
#include "sdlecs.h"
#include "gcomp.h"

// this is stuff relating to the characters
// i.e., pacman and the ghosts

enum DIRECTIONS {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

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
    float speed = 1.0f;
    int directions[4]; // use DIRECTION as index
};

// ------- Systems ------- //

class CharacterControlSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void update(ECSManager* manager) override;
    private:
        void moveCharacter(ECSManager* manager, ID entityID);
};

class PlayerControlSystem: public CharacterControlSystem {

};

class AIControlSystem: public CharacterControlSystem {

};

class CharacterRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(ECSManager* manager) override;
    private:
        void drawCharacter(ECSManager* manager, ID entityID);
};

#endif // CHARA_H