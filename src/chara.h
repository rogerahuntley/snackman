#ifndef CHARA_H
#define CHARA_H

#include "ecpps/ecpps.h"
#include "sdlecs.h"
#include "gcomp.h"

// this is stuff relating to the characters
// i.e., pacman and the ghosts

enum DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
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
    float speed = 16.0f;
    DIRECTION direction = NONE; // use DIRECTION as index
};

// ------- Systems ------- //

class CharacterControlSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void update(ECSManager* manager) override;
    private:
        void moveCharacter(ECSManager* manager, ID entityID);
};

class CharacterControllerSystem: public System {
    protected:
        virtual void updateInput(ECSManager* manager, ID entityID) {};
        // returns true if direction has changed
        bool updateDirection(ECSManager* manager, ID entityID, DIRECTION direction);
};

class PlayerControllerSystem: public CharacterControllerSystem {
    public:
        void update(ECSManager* manager) override;
        double inputTimer = 0.0;
    private:
        void updateInput(ECSManager* manager, ID entityID) override;
};

class AIControllerSystem: public CharacterControllerSystem {
    public:
    private:
};

class CharacterRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(ECSManager* manager) override;
    private:
        void drawCharacter(ECSManager* manager, ID entityID);
};

#endif // CHARA_H