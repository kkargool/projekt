#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player(bool isPlayerOne, int characterId = 0);
    void update(sf::Time dt);
    void draw(sf::RenderWindow& window);
    void takeDamage(int amount);
    int getHealth() const;
    sf::FloatRect getBounds() const;
    bool isAttacking() const;

    // movement helpers used for collision resolution
    void move(sf::Vector2f offset);
    void setPosition(sf::Vector2f pos);

private:
    sf::CircleShape shape;
    int health;
    bool isPlayerOne;
    bool attacking;
    sf::Clock attackCooldown;



};

#endif // PLAYER_H
