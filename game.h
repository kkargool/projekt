#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "player.h"

class Game
{
public:
    Game();
    void run();
private:
    enum class State { CharacterSelect, Playing };

    void processEvents();
    void update(sf::Time dt);
    void render();
    void handleAttacks();
    void characterSelect();

    sf::RenderWindow window;
    Player player1;
    Player player2;
    //Map map;
    //HUD hud;

    sf::Clock clock;
    State gameState;
    int selected1, selected2;
};




#endif // GAME_H
