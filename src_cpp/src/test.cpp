#include <SFML/Graphics.hpp>
#include <iostream>

// Track our player's action states
enum PlayerState { IDLE, WALKING, JUMPING, ATTACKING };

int main() {
    // 1. CREATE THE GAME WINDOW
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Shadows Edge");
    window.setFramerateLimit(60); // Keeps movement silky smooth at 60 FPS

    // 2. LOAD YOUR BACKGROUND IMAGE
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../Assets/Background_img.jpg")) {
        std::cout << "Error: Could not find Assets/Background_img.jpg!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    // Scales image to perfectly stretch across the 1280x720 window
    backgroundSprite.setScale(
        1280.0f / backgroundSprite.getLocalBounds().width,
        720.0f / backgroundSprite.getLocalBounds().height
    );

    // 3. SETUP THE CHARACTER SPRITE SHEET
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../Assets/heroimg.png")) {
        std::cout << "Error: Could not find Assets/heroimg.png!" << std::endl;
        return -1;
    }
    sf::Sprite player;
    player.setTexture(playerTexture);

    // Grid frame sizing configurations calculated from your 669x373 image dimensions
    const int FRAME_WIDTH = 67;
    const int FRAME_HEIGHT = 93;
    player.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));

    // Movement & Physics Coordinates
    float playerX = 350.0f;
    float playerY = 545.0f;       // Adjusted height base for the 93px tall sprite frame
    float playerSpeed = 5.0f;
    
    float velocityY = 0.0f;
    const float gravity = 0.6f;
    bool isGrounded = true;
    const float groundLevel = 545.0f; // Must perfectly match playerY coordinate base

    // Animation Timers & States
    PlayerState currentState = IDLE;
    sf::Clock animationClock;
    int currentFrame = 0;
    float frameDuration = 0.1f; // Speed of frame switches (0.1 seconds)

    // 4. MAIN GAME LOOP
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // --- GAME LOGIC & INPUT HANDLING ---
        
        // Reset state to default ground position states
        if (isGrounded) {
            currentState = IDLE;
        } else {
            currentState = JUMPING;
        }

        // Input Checklist: Attacking, Left, Right, Jumping
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
            currentState = ATTACKING;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            playerX -= playerSpeed;
            if (isGrounded) currentState = WALKING;
            player.setScale(-1.0f, 1.0f);     // Flip sprite mirror-left
            player.setOrigin(FRAME_WIDTH, 0); // Correct rotation pivot anchor point
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerX += playerSpeed;
            if (isGrounded) currentState = WALKING;
            player.setScale(1.0f, 1.0f);      // Normal sprite orientation right
            player.setOrigin(0, 0);
        }

        // Jump Execution Input
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && isGrounded) {
            velocityY = -13.0f; // Initial vertical upward force impulse vector
            isGrounded = false;
        }

        // Apply Real-Time Gravity Physics Calculations
        if (!isGrounded) {
            velocityY += gravity;
        } else {
            velocityY = 0.0f;
        }
        playerY += velocityY;

        // Sidewalk Collision Bounds Management
        if (playerY >= groundLevel) {
            playerY = groundLevel;
            isGrounded = true;
        }

        // Keep player bounded horizontally inside the frame edges
        if (playerX < 0) playerX = 0;
        float rightBound = 1280.0f - FRAME_WIDTH;
        if (playerX > rightBound) playerX = rightBound;

        // Run the Animation Frame Clock Iteration Cycles
        if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
            currentFrame++;
            
            // Loop boundaries matching your specific sheet row column counts
            if (currentState == IDLE && currentFrame >= 5) currentFrame = 0;
            if (currentState == WALKING && currentFrame >= 10) currentFrame = 0;
            if (currentState == JUMPING && currentFrame >= 7) currentFrame = 0;
            if (currentState == ATTACKING && currentFrame >= 8) currentFrame = 0;
            
            animationClock.restart();
        }

        // Map Active Animation Sequence Row to Image Coordinates
        int textureY = 0;
        if (currentState == IDLE)      textureY = 0 * FRAME_HEIGHT;
        if (currentState == WALKING)   textureY = 1 * FRAME_HEIGHT;
        if (currentState == JUMPING)   textureY = 2 * FRAME_HEIGHT;
        if (currentState == ATTACKING) textureY = 3 * FRAME_HEIGHT;

        // Slice the exact frame and set coordinates
        player.setTextureRect(sf::IntRect(currentFrame * FRAME_WIDTH, textureY, FRAME_WIDTH, FRAME_HEIGHT));
        player.setPosition(playerX, playerY);

        // 5. RENDER PIPELINE OVERLAYS
        window.clear();
        window.draw(backgroundSprite);
        window.draw(player);
        window.display();
    }

    return 0;
}