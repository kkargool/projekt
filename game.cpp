#include "game.h"

Game::Game()
    : window(sf::VideoMode(800, 600), "Bombardini Combatini"),
    player1(true), player2(false), gameState(State::MainMenu) {
    initStartButton();
    initMap();
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
        title.setPosition(150, 150);
        sf::Text text("Start", font, 24);
        sf::FloatRect bounds = startButton.getGlobalBounds();
        text.setPosition(bounds.left + 50, bounds.top + 15);
        window.draw(title);
        window.draw(startButton);
        window.draw(text);
    } else {
        window.draw(mapRect);
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

void Game::mainMenu() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i m = sf::Mouse::getPosition(window);
        if (startButton.getGlobalBounds().contains(static_cast<float>(m.x), static_cast<float>(m.y))) {
            player1 = Player(true);
            player2 = Player(false);
            gameState = State::Playing;
        }
    }
}

void Game::initStartButton() {
    startButton.setSize({200.f, 50.f});
    startButton.setFillColor(sf::Color(150, 150, 250));
    startButton.setPosition(300.f, 280.f);
}

void Game::initMap() {
    mapRect.setSize({800.f, 100.f});
    mapRect.setFillColor(sf::Color(50, 150, 50));
    mapRect.setPosition(0.f, 500.f);
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
