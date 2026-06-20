#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <optional>

// =========================================================================
// 🔗 UNITY DLL EXPORT BRIDGE
// =========================================================================

extern "C" {
    __declspec(dllexport) float CalculateJumpVelocity(float jumpForce);
    __declspec(dllexport) int calculate_damage(int baseAttack, int criticalMultiplier, int targetType);
}


extern "C" {
    // Your jump function is already here...
    __declspec(dllexport) float CalculateJumpVelocity(float jumpForce) {
    return jumpForce * 0.05f;
    }


    // Expanded Damage Logic & Added EnemyBoss. 
    __declspec(dllexport) int calculate_damage(int baseAttack, int criticalMultiplier, int targetType) {
        int finalDamage = baseAttack * criticalMultiplier;

        switch(targetType) {
            case 0: 
                break;
            case 2: 
                finalDamage = static_cast<int>(finalDamage * 0.6f);
                break;
            
            default:
                break;    
        }
        if (finalDamage <= 0) finalDamage = 1;
        return finalDamage;
    }
}

// =========================================================================
// 🎮 STANDALONE SFML 3.0 GAME ENGINE LAYERS
// =========================================================================
enum PlayerState { IDLE, WALKING, JUMPING, ATTACKING, DEAD };
enum EnemyState { ENEMY_WALKING, ENEMY_ATTACKING };

struct Enemy {
    std::optional<sf::Sprite> sprite; // Safe wrapper matching SFML 3.0 structures
    float x;
    float y;
    float speed;
    int currentFrame;
    sf::Clock animClock;
    sf::Clock attackCooldownClock; 
    EnemyState state;
    EnemyState previousState; 
    int health;       
    int maxHealth;    
    bool isAlive;
};

int main() {
    // 1. CREATE THE GAME WINDOW (SFML 3.0 explicit Vector2u requirement)
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280, 720)), "Shadows Edge");
    window.setFramerateLimit(60);

    // 2. LOAD BACKGROUND
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../Assets/Background_img.jpg")) {
        std::cout << "Error: Could not find Assets/Background_img.jpg!" << std::endl;
        return -1;
    }
    
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale({
        1280.0f / backgroundSprite.getLocalBounds().size.x,
        720.0f / backgroundSprite.getLocalBounds().size.y
    });

    // 3. SETUP PLAYER (HERO)
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../Assets/heroimg.png")) {
        std::cout << "Error: Could not find Assets/heroimg.png!" << std::endl;
        return -1;
    }
    
    sf::Sprite player(playerTexture);

    const int HERO_FRAME_WIDTH = 67;
    const int HERO_FRAME_HEIGHT = 93;
    player.setTextureRect(sf::IntRect({0, 0}, {HERO_FRAME_WIDTH, HERO_FRAME_HEIGHT}));
    player.setOrigin({HERO_FRAME_WIDTH / 2.0f, HERO_FRAME_HEIGHT / 2.0f});

    float playerX = 350.0f;
    float playerY = 545.0f + (HERO_FRAME_HEIGHT / 2.0f); 
    float playerSpeed = 5.0f;
    
    int playerMaxHealth = 100;
    int playerHealth = 100;
    
    float velocityY = 0.0f;
    const float gravity = 0.6f;
    bool isGrounded = true;
    const float groundLevel = 545.0f + (HERO_FRAME_HEIGHT / 2.0f);

    PlayerState currentState = IDLE;
    PlayerState previousState = IDLE;
    sf::Clock animationClock;
    int currentFrame = 0;
    float frameDuration = 0.09f; 

    // --- SETUP HERO HEALTH BAR UI ---
    sf::RectangleShape heroHealthBarBg(sf::Vector2f({60.0f, 6.0f}));
    heroHealthBarBg.setFillColor(sf::Color(50, 50, 50)); 
    
    sf::RectangleShape heroHealthBarFg(sf::Vector2f({60.0f, 6.0f}));
    heroHealthBarFg.setFillColor(sf::Color(50, 220, 100)); 

    // 4. SETUP ENEMY
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("../Assets/EnemyImg.png")) {
        std::cout << "Error: Could not find Assets/EnemyImg.png!" << std::endl;
        return -1;
    }

    const int ENEMY_FRAME_WIDTH = 67;
    const int ENEMY_FRAME_HEIGHT = 93;

    Enemy targetEnemy;
    targetEnemy.sprite.emplace(enemyTexture); 
    targetEnemy.sprite->setTextureRect(sf::IntRect({0, 0}, {ENEMY_FRAME_WIDTH, ENEMY_FRAME_HEIGHT}));
    targetEnemy.sprite->setOrigin({ENEMY_FRAME_WIDTH / 2.0f, ENEMY_FRAME_HEIGHT / 2.0f});
    
    targetEnemy.x = 900.0f; 
    targetEnemy.y = 545.0f + (ENEMY_FRAME_HEIGHT / 2.0f); 
    targetEnemy.speed = 2.0f;
    targetEnemy.currentFrame = 0;
    targetEnemy.state = ENEMY_WALKING;
    targetEnemy.previousState = ENEMY_WALKING;
    targetEnemy.maxHealth = 60; 
    targetEnemy.health = 60;
    targetEnemy.isAlive = true;

    // --- SETUP ENEMY HEALTH BAR UI ---
    sf::RectangleShape enemyHealthBarBg(sf::Vector2f({60.0f, 6.0f}));
    enemyHealthBarBg.setFillColor(sf::Color(50, 50, 50));
    
    sf::RectangleShape enemyHealthBarFg(sf::Vector2f({60.0f, 6.0f}));
    enemyHealthBarFg.setFillColor(sf::Color(220, 50, 50)); 

    // 5. MAIN GAME LOOP
    while (window.isOpen()) {
        bool weaponKeyJustPressed = false;
        bool respawnKeyPressed = false;

        // --- SFML 3.0 POLL EVENT LOOP ---
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::F && currentState != DEAD) {
                    weaponKeyJustPressed = true;
                }
                if (keyPressed->code == sf::Keyboard::Key::R && currentState == DEAD) {
                    respawnKeyPressed = true;
                }
            }
        }

        // --- RESPAWN HANDLER ---
        if (respawnKeyPressed) {
            playerHealth = playerMaxHealth;
            playerX = 350.0f;
            playerY = groundLevel;
            velocityY = 0.0f;
            isGrounded = true;
            currentState = IDLE;
            currentFrame = 0;

            targetEnemy.health = targetEnemy.maxHealth;
            targetEnemy.x = 900.0f;
            targetEnemy.state = ENEMY_WALKING;
            targetEnemy.currentFrame = 0;
            targetEnemy.isAlive = true;

            std::cout << "Respawn Success! Fight restarted." << std::endl;
            animationClock.restart();
            continue;
        }

        // TRIGGER DEATH STATE
        if (playerHealth <= 0 && currentState != DEAD) {
            currentState = DEAD;
            currentFrame = 0;
            animationClock.restart();
            std::cout << "Hero Defeated. Press 'R' to Respawn." << std::endl;
        }

        // --- HERO INPUT HANDLING & STATE LOGIC ---
        previousState = currentState;

        if (currentState != ATTACKING && currentState != DEAD) {
            if (isGrounded) {
                currentState = IDLE;
            } else {
                currentState = JUMPING;
            }

            if (weaponKeyJustPressed) {
                currentState = ATTACKING;
                currentFrame = 0; 
                animationClock.restart();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                playerX -= playerSpeed;
                if (isGrounded) currentState = WALKING;
                player.setScale({-1.0f, 1.0f}); 
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                playerX += playerSpeed;
                if (isGrounded) currentState = WALKING;
                player.setScale({1.0f, 1.0f});      
            }

            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) && isGrounded) {
                float baseJumpForce = 13.0f;
                velocityY = -CalculateJumpVelocity(baseJumpForce);
                
                isGrounded = false;
                currentState = JUMPING;
            }
        }

        // --- HERO JUMP PHYSICS ---
        if (currentState != DEAD) {
            if (!isGrounded) {
                velocityY += gravity;
            } else {
                velocityY = 0.0f;
            }
            playerY += velocityY;

            if (playerY >= groundLevel) {
                playerY = groundLevel;
                isGrounded = true;
            }

            if (playerX < (HERO_FRAME_WIDTH / 2)) playerX = HERO_FRAME_WIDTH / 2;
            if (playerX > 1280 - (HERO_FRAME_WIDTH / 2)) playerX = 1280 - (HERO_FRAME_WIDTH / 2);
        }

        // --- HERO ANIMATION TICKER ---
        if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
            currentFrame++;
            
            if (currentState == IDLE && currentFrame >= 5) currentFrame = 0;
            if (currentState == WALKING && currentFrame >= 10) currentFrame = 0;
            if (currentState == JUMPING && currentFrame >= 7) currentFrame = 0;
            if (currentState == ATTACKING && currentFrame >= 8) {
                currentState = IDLE;
                currentFrame = 0;
            }
            
            if (currentState == DEAD && currentFrame >= 4) {
                currentFrame = 4; 
            }
            animationClock.restart();
        }

        int textureY = currentState * HERO_FRAME_HEIGHT;
        if (currentState == DEAD) {
            textureY = 2 * HERO_FRAME_HEIGHT; 
        }

        player.setTextureRect(sf::IntRect({currentFrame * HERO_FRAME_WIDTH, textureY}, {HERO_FRAME_WIDTH, HERO_FRAME_HEIGHT}));
        player.setPosition({playerX, playerY}); // FIXED: Added SFML 3.0 Braces

        // POSITION HEALTH BAR ABOVE PLAYER
        float barXOffset = playerX - 30.0f; 
        float barYOffset = playerY - (HERO_FRAME_HEIGHT / 2.0f) - 15.0f;
        heroHealthBarBg.setPosition({barXOffset, barYOffset}); // FIXED: Added SFML 3.0 Braces
        heroHealthBarFg.setPosition({barXOffset, barYOffset}); // FIXED: Added SFML 3.0 Braces
        
        float playerHpRatio = (float)playerHealth / playerMaxHealth;
        if (playerHpRatio < 0) playerHpRatio = 0;
        heroHealthBarFg.setSize(sf::Vector2f({60.0f * playerHpRatio, 6.0f}));

        // --- ENEMY LOGIC & ATTACK SYSTEM ---
        if (targetEnemy.isAlive && targetEnemy.sprite.has_value()) {
            float distanceToPlayer = std::abs(playerX - targetEnemy.x);
            targetEnemy.previousState = targetEnemy.state;
            
            if (currentState != DEAD) {
                if (distanceToPlayer <= 55.0f) {
                    targetEnemy.state = ENEMY_ATTACKING;
                } else {
                    targetEnemy.state = ENEMY_WALKING;
                }
            } else {
                targetEnemy.state = ENEMY_WALKING;
            }

            if (targetEnemy.state != targetEnemy.previousState) {
                targetEnemy.currentFrame = 0;
                targetEnemy.animClock.restart();
            }

            if (targetEnemy.state == ENEMY_WALKING && currentState != DEAD) {
                if (targetEnemy.x > playerX + 30) {
                    targetEnemy.x -= targetEnemy.speed;
                    targetEnemy.sprite->setScale({1.0f, 1.0f}); // FIXED: Added SFML 3.0 Braces
                } else if (targetEnemy.x < playerX - 30) {
                    targetEnemy.x += targetEnemy.speed;
                    targetEnemy.sprite->setScale({-1.0f, 1.0f}); // FIXED: Added SFML 3.0 Braces
                }
            }

            int enemyMaxFrames = 10; 
            int enemyTextureY = 1 * ENEMY_FRAME_HEIGHT; 

            if (targetEnemy.state == ENEMY_ATTACKING && currentState != DEAD) {
                enemyMaxFrames = 6;                     
                enemyTextureY = 3 * ENEMY_FRAME_HEIGHT; 

                if (targetEnemy.attackCooldownClock.getElapsedTime().asSeconds() >= 1.0f) {
                    playerHealth -= 15;
                    targetEnemy.attackCooldownClock.restart();
                }
            }

            if (targetEnemy.animClock.getElapsedTime().asSeconds() >= 0.12f) {
                if (currentState != DEAD) {
                    targetEnemy.currentFrame++;
                    if (targetEnemy.state == ENEMY_WALKING && targetEnemy.currentFrame >= 10) {
                        targetEnemy.currentFrame = 0;
                    }
                    if (targetEnemy.state == ENEMY_ATTACKING && targetEnemy.currentFrame >= enemyMaxFrames) {
                        targetEnemy.currentFrame = 0; 
                    }
                } else {
                    targetEnemy.currentFrame = 0;
                }
                targetEnemy.animClock.restart();
            }
            
            targetEnemy.sprite->setTextureRect(sf::IntRect({targetEnemy.currentFrame * ENEMY_FRAME_WIDTH, enemyTextureY}, {ENEMY_FRAME_WIDTH, ENEMY_FRAME_HEIGHT}));
            targetEnemy.sprite->setPosition({targetEnemy.x, targetEnemy.y}); // FIXED: Added SFML 3.0 Braces

            // POSITION ENEMY HP BAR
            float enemyBarXOffset = targetEnemy.x - 30.0f;
            float enemyBarYOffset = targetEnemy.y - (ENEMY_FRAME_HEIGHT / 2.0f) - 15.0f;
            enemyHealthBarBg.setPosition({enemyBarXOffset, enemyBarYOffset}); // FIXED: Added SFML 3.0 Braces
            enemyHealthBarFg.setPosition({enemyBarXOffset, enemyBarYOffset}); // FIXED: Added SFML 3.0 Braces

            float enemyHpRatio = (float)targetEnemy.health / targetEnemy.maxHealth;
            if (enemyHpRatio < 0) enemyHpRatio = 0;
            enemyHealthBarFg.setSize(sf::Vector2f({60.0f * enemyHpRatio, 6.0f}));

            if (weaponKeyJustPressed && distanceToPlayer < 75.0f && currentState != DEAD) { 
               int baseDamage = 20;
               float criticalMultiplier = 1.25f;

               int finalDamage = calculate_damage(baseDamage, criticalMultiplier, 0);

               targetEnemy.health -= finalDamage;
               std::cout << "Hero hits Enemy! Dealt " << finalDamage << " damage. Enemy HP: " 
               << targetEnemy.health << std::endl;
               
               if (targetEnemy.health <= 0) {
                targetEnemy.isAlive = false;
                std::cout << "Target Neutralized!" << std::endl;
               }
            }
        }

        // --- RENDER PIPELINE ---
        window.clear();
        window.draw(backgroundSprite);
        
        if (targetEnemy.isAlive && targetEnemy.sprite.has_value()) {
            window.draw(targetEnemy.sprite.value()); 
            window.draw(enemyHealthBarBg);
            window.draw(enemyHealthBarFg);
        }
        
        window.draw(player);
        
        if (currentState != DEAD) {
            window.draw(heroHealthBarBg);
            window.draw(heroHealthBarFg);
        }
        
        window.display();
    }

    return 0;
}