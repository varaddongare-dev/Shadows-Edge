#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace ShadowsEdge {

    struct AABB {
        float x;
        float y;
        float width;
        float height;
    };

    bool CheckCollision(AABB a, AABB b);

    class Player {
    private:
        float xPosition;
        float yPosition;
        float movementSpeed;
        
        // --- ADD THESE PHYSICS FIELDS ---
        float velocityX;
        float velocityY;
        bool isOnGround;
    
    public:
        Player();
        void MoveRight();
        void MoveLeft();
        float GetXPosition();
        float GetYPosition();

        // --- ADD THESE TWO ENGINE FUNCTION BLUEPRINTS ---
        void HandleInput(char inputKey);
        void UpdatePhysics(float deltaTime, AABB environmentPlatform);

        // Note: Make sure the 'B' in Hitbox matches your Player.cpp case exactly (GetHitbox)
        AABB GetHitbox();
    };

}
#endif