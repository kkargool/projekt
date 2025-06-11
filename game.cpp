#include "game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Bombardini Combatini"),
    player1(true), player2(false), selected1(0), selected2(0), gameState(State::MainMenu) {
    initObstacles();
}

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
    if (gameState == State::MainMenu) {
        mainMenu();
    } else if (gameState == State::CharacterSelect) {
        characterSelect();
    } else if (gameState == State::Playing) {
        player1.update(dt);
        player2.update(dt);
        resolveCollisions(player1);
        resolveCollisions(player2);
        resolvePlayerCollision();
        handleAttacks();
    }
}

void Game::render() {
    window.clear();

    if (gameState == State::MainMenu) {
        sf::Font font;
        //font.loadFromFile("assets/font.ttf");
        sf::Text title("Bombardini Combatini", font, 36);
        title.setPosition(150, 200);
        sf::Text prompt("Press Enter to Start", font, 24);
        prompt.setPosition(220, 300);
        window.draw(title);
        window.draw(prompt);
    } else if (gameState == State::CharacterSelect) {
        sf::Font font;
        //font.loadFromFile("assets/font.ttf");
        sf::Text text("Press 1-3 for P1, Num1-Num3 for P2", font, 24);
        text.setPosition(100, 250);
        window.draw(text);
    } else {
        for (auto& o : obstacles)
            window.draw(o);
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

void Game::mainMenu() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        gameState = State::CharacterSelect;
    }
}

void Game::initObstacles() {
    sf::RectangleShape wall;
    wall.setSize({120.f, 40.f});
    wall.setFillColor(sf::Color(100, 100, 100));
    wall.setPosition(340.f, 280.f);
    obstacles.push_back(wall);
}

void Game::resolveCollisions(Player& player) {
    auto size = window.getSize();
    sf::FloatRect b = player.getBounds();

    if (b.left < 0)
        player.move({-b.left, 0});
    if (b.left + b.width > size.x)
        player.move({size.x - (b.left + b.width), 0});
    if (b.top < 0)
        player.move({0, -b.top});
    if (b.top + b.height > size.y)
        player.move({0, size.y - (b.top + b.height)});

    for (auto& obs : obstacles) {
        if (player.getBounds().intersects(obs.getGlobalBounds())) {
            sf::FloatRect pb = player.getBounds();
            sf::FloatRect ob = obs.getGlobalBounds();
            float dx = (pb.left + pb.width / 2.f) - (ob.left + ob.width / 2.f);
            float dy = (pb.top + pb.height / 2.f) - (ob.top + ob.height / 2.f);
            float ox = pb.width / 2.f + ob.width / 2.f - std::abs(dx);
            float oy = pb.height / 2.f + ob.height / 2.f - std::abs(dy);
            if (ox < oy) {
                player.move({dx > 0 ? ox : -ox, 0});
            } else {
                player.move({0, dy > 0 ? oy : -oy});
            }
        }
    }
}

void Game::resolvePlayerCollision() {
    if (player1.getBounds().intersects(player2.getBounds())) {
        sf::FloatRect b1 = player1.getBounds();
        sf::FloatRect b2 = player2.getBounds();
        float dx = (b1.left + b1.width / 2.f) - (b2.left + b2.width / 2.f);
        float dy = (b1.top + b1.height / 2.f) - (b2.top + b2.height / 2.f);
        float ox = b1.width / 2.f + b2.width / 2.f - std::abs(dx);
        float oy = b1.height / 2.f + b2.height / 2.f - std::abs(dy);
        if (ox < oy) {
            float move = ox / 2.f;
            player1.move({dx > 0 ? move : -move, 0});
            player2.move({dx > 0 ? -move : move, 0});
        } else {
            float move = oy / 2.f;
            player1.move({0, dy > 0 ? move : -move});
            player2.move({0, dy > 0 ? -move : move});
        }
    }
}
