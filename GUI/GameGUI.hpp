#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "GAME/game.hpp"       // ודא שהנתיב ל-game.hpp נכון
#include "PLAYER/player.hpp" // ודא שהנתיב ל-player.hpp נכון

namespace coup {

/**
 * @class GameGUI
 * @brief Manages the graphical user interface for the Coup game.
 * 
 * This class is responsible for displaying various screens such as the welcome screen, 
 * player input, role reveal, main game screen, winner screen, and error popups.
 * It interacts with the Game singleton to get game state and player information,
 * and uses the SFML library for rendering.
 */
class GameGUI {
public:
    /**
     * @brief GameGUI constructor.
     * Initializes the main graphical window and loads necessary resources like fonts.
     */
    GameGUI(Game& game);

    /**
     * @brief Runs the main GUI loop.
     * Starts with the welcome screen, proceeds to player input, and then runs the game screen.
     */
    void run();

    /**
     * @brief Default destructor.
     */
    ~GameGUI() = default;

private:
    sf::RenderWindow window; // The main SFML window
    sf::Font font;           // The font used for all text rendering
    Game& game;              // Reference to the game instance

    // === Private Helper Functions ===

    // Screen display functions
    void showWelcomeScreen();
    void showPlayerInputScreen();
    void showRoleRevealScreen(const std::vector<std::string>& playerNames);
    void runGameScreen();
    void showWinnerScreen(const std::string& winnerName);

    // User interaction popups
    std::string askForTargetPlayerName();
    std::string showCancelConfirmation(const std::string& playerName);
    void showErrorPopup(const std::string& errorMessage);
    void viewPlayerCoinsPopup(const std::string& targetName);

    // Utility and conversion functions
    std::string roleToString(Role role) const;
    std::string actionTypeToString(ActionType action) const;
    ActionType stringToActionType(const std::string& actionStr) const;
};

} // namespace coup