#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // 1. Create the game window (1280x720 matching standard asset layouts)
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Shadows Edge");
    window.setFramerateLimit(60); // Keeps movement silky smooth at 60 FPS

    // ==========================================
    // 2. LOAD YOUR BACKGROUND IMAGE
    // ==========================================
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../Assets/Background_img.jpg")) {
        std::cout << "Error: Could not find Assets/Background_img.jpg!" << std::endl;
        return -1;
    }
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Automatically scales your image to perfectly stretch across the 1280x720 window
    backgroundSprite.setScale(
        static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
        static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
    );

    // ==========================================
    // 3. SETUP THE CHARACTER (BUBBA)
    // ==========================================
    // For testing right now, we create a simple dark silhouette block 
    // so you can see movement immediately without loading a second image file.
    sf::RectangleShape player(sf::Vector2f(40.f, 70.f));
    player.setFillColor(sf::Color(15, 20, 25)); // Dark shadow body
    player.setOutlineColor(sf::Color(0, 255, 204)); // Neon Edge trim highlight
    player.setOutlineThickness(2.f);

    // Coordinates to place the character on your ground platform
    float playerX = 350.0f;
    float playerY = 575.0f; // Adjust this number up or down to align with your background floor
    float playerSpeed = 7.0f;

    // ==========================================
    // 4. MAIN GAME LOOP
    // ==========================================
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Real-time keyboard input checking
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            playerX -= playerSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            playerX += playerSpeed;
        }

        // Screen boundary locks (Stops the character from leaving the window)
        if (playerX < 0) playerX = 0;
        if (playerX > 1280 - 40) playerX = 1280 - 40;

        // Apply position updates to the player entity
        player.setPosition(playerX, playerY);

        // ==========================================
        // 5. RENDERING LAYER (Order Matters!)
        // ==========================================
        window.clear();

        window.draw(backgroundSprite); // 1. Draws your image completely in the back
        window.draw(player);           // 2. Draws your character right over it

        window.display();
    }

    return 0;
}