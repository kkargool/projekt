#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "player.h"

class Game
{
public:
    Game();
    void run();
private:
    enum class State { MainMenu, Playing };

    void processEvents();
    void update(sf::Time dt);
    void render();
    void handleAttacks();
    void mainMenu();
    void resolveCollisions(Player& player);
    void resolvePlayerCollision();
    void initStartButton();
    void initMap();

    sf::RenderWindow window;
    Player player1;
    Player player2;
    //Map map;
    //HUD hud;

    sf::Clock clock;
    State gameState;
    std::vector<sf::RectangleShape> obstacles;
    sf::RectangleShape startButton;
    sf::RectangleShape mapRect;
};




#endif // GAME_H
