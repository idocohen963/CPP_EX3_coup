// idocohen963@gmail.com
#include "GameGUI.hpp"
#include "GAME/game.hpp"
#include <iostream>

int main() {
    try {
        coup::Game& game = coup::Game::getInstance();
        coup::GameGUI gui(game);
        gui.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
