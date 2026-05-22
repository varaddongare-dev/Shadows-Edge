#include "../include/Player.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // SFML 2 classic constructor syntax
    sf::RenderWindow window(sf::VideoMode(800, 600), "Shadow's Edge - Pure C++ Physics");
    window.setFramerateLimit(60); 

    ShadowsEdge::Player testPlayer;
    ShadowsEdge::AABB floorPlatform = { 0.0f, 500.0f, 800.0f, 40.0f };

    sf::RectangleShape visualPlayer(sf::Vector2f(32.0f, 64.0f));
    visualPlayer.setFillColor(sf::Color::Cyan); 

    sf::RectangleShape visualFloor(sf::Vector2f(800.0f, 40.0f));
    visualFloor.setFillColor(sf::Color::Green); 
    visualFloor.setPosition(floorPlatform.x, floorPlatform.y);

    float deltaTime = 0.016f; 

    std::cout << " Firing up visual SFML window loop..." << std::endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            testPlayer.HandleInput('d');
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            testPlayer.HandleInput('a');
        }

        testPlayer.UpdatePhysics(deltaTime, floorPlatform);
        visualPlayer.setPosition(testPlayer.GetXPosition(), testPlayer.GetYPosition());

        window.clear(sf::Color::Black); 
        window.draw(visualFloor);       
        window.draw(visualPlayer);      
        window.display();               
    }

    return 0;
}