#include "../include/Player.hpp"
#include <iostream>

namespace ShadowsEdge {

    // Checks if two boxes overlap mathematically
    bool CheckCollision(AABB a, AABB b) {
        return (a.x < b.x + b.width &&
                a.x + a.width > b.x &&
                a.y < b.y + b.height &&
                a.y + a.height > b.y);
    }

    Player::Player() {
        xPosition = 0.0f;
        yPosition = 0.0f; // Start high in the air
        movementSpeed = 5.0f;
        velocityX = 0.0f;
        velocityY = 0.0f;
        isOnGround = false;
    }

    void Player::MoveRight() { xPosition += movementSpeed; }
    void Player::MoveLeft()  { xPosition -= movementSpeed; }
    
    float Player::GetXPosition() { return xPosition; }
    float Player::GetYPosition() { return yPosition; }

    // --- LIVE INPUT MECHANICS ---
    void Player::HandleInput(char inputKey) {
        float moveAcceleration = 15.0f;

        if (inputKey == 'd' || inputKey == 'D') {
            velocityX = moveAcceleration; // Move right momentum
        } 
        else if (inputKey == 'a' || inputKey == 'A') {
            velocityX = -moveAcceleration; // Move left momentum
        }
    }

    // --- KINEMATICS & GRAVITY CALCULATIONS ---
    void Player::UpdatePhysics(float deltaTime, AABB environmentPlatform) {
        float gravityForce = 50.0f;

        // 1. If airborne, gravity constantly pulls down over time
        if (!isOnGround) {
            velocityY += gravityForce * deltaTime;
        }

        // 2. Apply velocities to coordinates using Delta Time
        xPosition += velocityX * deltaTime;
        yPosition += velocityY * deltaTime;

        // 3. Friction decay: slows down horizontal speed naturally
        velocityX *= 0.8f;

        // 4. Ground Collision Check
        if (CheckCollision(GetHitbox(), environmentPlatform)) {
            yPosition = environmentPlatform.y - 64.0f; // Snap to top of floor (64 is player height)
            velocityY = 0.0f;
            isOnGround = true;
        } else {
            isOnGround = false;
        }
    }

    // Matches lowercase 'b' blueprint perfectly
    AABB Player::GetHitbox() {
        return { xPosition, yPosition, 32.0f, 64.0f };
    }
}