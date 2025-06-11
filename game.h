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
    void initStartButton();
    void initMap();
    sf::RectangleShape startButton;
    sf::RectangleShape mapRect;
    void resolvePlayerCollision();
    void initObstacles();

    sf::RenderWindow window;
    Player player1;
    Player player2;
    //Map map;
    //HUD hud;

    sf::Clock clock;
    State gameState;
    int selected1, selected2;
    std::vector<sf::RectangleShape> obstacles;
};




#endif // GAME_H
