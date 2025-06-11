#include "game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Bombardini Combatini"),
    player1(true), player2(false), selected1(0), selected2(0), gameState(State::CharacterSelect) {}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        sf::Time dt = clock.restart();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time dt) {
    if (gameState == State::CharacterSelect) {
        characterSelect();
    } else if (gameState == State::Playing) {
        player1.update(dt);
        player2.update(dt);
        handleAttacks();
    }
}

void Game::render() {
    window.clear();

    if (gameState == State::CharacterSelect) {
        sf::Font font;
        //font.loadFromFile("assets/font.ttf");
        sf::Text text("Press 1-3 for P1, Num1-Num3 for P2", font, 24);
        text.setPosition(100, 250);
        window.draw(text);
    } else {
        //map.draw(window);
        player1.draw(window);
        player2.draw(window);
        //hud.draw(window, player1.getHealth(), player2.getHealth());
    }

    window.display();
}

void Game::handleAttacks() {
    if (player1.isAttacking() && player1.getBounds().intersects(player2.getBounds())) {
        player2.takeDamage(10);
    }
    if (player2.isAttacking() && player2.getBounds().intersects(player1.getBounds())) {
        player1.takeDamage(10);
    }
}

void Game::characterSelect() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) selected2 = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) selected2 = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) selected2 = 2;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) selected1 = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) selected1 = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) selected1 = 2;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        player1 = Player(true, selected1);
        player2 = Player(false, selected2);
        gameState = State::Playing;
    }
}
