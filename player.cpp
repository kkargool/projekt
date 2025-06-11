#include "player.h"

Player::Player(bool isP1, int characterId) : isPlayerOne(isP1), health(100), attacking(false) {
    shape.setSize({50.f, 50.f});

    switch (characterId) {
    case 0:
        shape.setFillColor(sf::Color::Green);
        break;
    case 1:
        shape.setFillColor(sf::Color::Magenta);
        break;
    case 2:
        shape.setFillColor(sf::Color::Cyan);
        break;
    default:
        shape.setFillColor(sf::Color::White);
    }

    shape.setPosition(isP1 ? 100.f : 600.f, 500.f);
}

void Player::update(sf::Time dt) {
    float speed = 200.f;
    sf::Vector2f movement(0.f, 0.f);
    attacking = false;

    if (isPlayerOne) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && attackCooldown.getElapsedTime().asSeconds() > 0.5f) {
            attacking = true;
            attackCooldown.restart();
        }
    } else {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) movement.x -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movement.x += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) movement.y -= speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) movement.y += speed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && attackCooldown.getElapsedTime().asSeconds() > 0.5f) {
            attacking = true;
            attackCooldown.restart();
        }
    }

    shape.move(movement * dt.asSeconds());
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

int Player::getHealth() const {
    return health;
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

bool Player::isAttacking() const {
    return attacking;
}
