#ifndef CHARA_H
#define CHARA_H

#include "sdlecs.h"
#include "gcomp.h"

#include "ecpps/ecpps.h"
#include "physicpps/physicpps.h"

#include <utility>

using std::pair;

// this is stuff relating to the characters
// i.e., pacman and the ghosts

enum DIRECTION {
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE
};

using namespace ecpps;
using namespace physicpps;

// ------- Entities ------- //

class PacmanEntity: public Entity {
    private:
        void init(float x, float y);
    public:
        // constructor
        PacmanEntity(ID entityID, ECSManager* manager, float x = 0, float y = 0): Entity(entityID, manager) { init(x, y); };
};

class GhostEntity: public Entity {
    private:
        void init(float x, float y);
    public:
        // constructor
        GhostEntity(ID entityID, ECSManager* manager, float x = 0, float y = 0): Entity(entityID, manager) { init(x, y); };
};

// ------- Components ------- //

struct CharacterComponent: public Component {
    float speed = 16.0f;
    float width = 16;
    float height = 16;
    DIRECTION direction = NONE; // use DIRECTION as index
    DIRECTION newDirection = NONE; // use DIRECTION as index
    pair<int, int> mapCoord;
};

struct PlayerControlComponent: public Component {
};

struct AIControlComponent: public Component {
};

// ------- Systems ------- //

class CharacterControlSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void update(ECSManager* manager) override;
    private:
        void moveCharacter(ECSManager* manager, ID entityID);
        void applyVector(ECSManager* manager, ID entityID, Vector2& vector);
        pair<int, int> getTile(ECSManager* manager, ID entityID);
};

class CharacterControllerSystem: public System {
    protected:
};

class PlayerControllerSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void update(ECSManager* manager) override;
    private:
        void updateInput(ECSManager* manager, ID entityID);
};

class AIControllerSystem: public System {
    public:
        void init(ECSManager* manager) override;
        void update(ECSManager* manager) override;
    private:
        void updateInput(ECSManager* manager, ID entityID);
};

class CharacterRenderSystem: public RenderSystem {
    public:
        void init(ECSManager* manager) override;
        void render(ECSManager* manager) override;
    private:
        void drawCharacter(ECSManager* manager, ID entityID);
};

#endif // CHARA_H