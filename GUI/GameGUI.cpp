#include "GameGUI.hpp"
#include <iostream>
#include <random>
#include <map>
#include <algorithm>
#include <chrono> // For sf::Clock
#include "PLAYER/PlayerFactory.hpp" 

namespace coup {

// === Conversion Helper Functions ===

std::string GameGUI::roleToString(Role role) const {
    static const std::map<Role, std::string> roleMap = {
        {Role::Governor, "Governor"}, {Role::Spy, "Spy"},
        {Role::Baron, "Baron"}, {Role::General, "General"},
        {Role::Judge, "Judge"}, {Role::Merchant, "Merchant"}
    };
    auto it = roleMap.find(role);
    return (it != roleMap.end()) ? it->second : "Unknown";
}

std::string GameGUI::actionTypeToString(ActionType action) const {
    static const std::map<ActionType, std::string> actionMap = {
        {ActionType::Gather, "Gather"}, {ActionType::Tax, "Tax"},
        {ActionType::Bribe, "Bribe"}, {ActionType::Arrest, "Arrest"},
        {ActionType::Coup, "Coup"}, {ActionType::Sanction, "Sanction"},
        {ActionType::Invest, "Invest"}, {ActionType::SpyOn, "Spy On"},
        {ActionType::cancel, "Cancel"}
    };
    auto it = actionMap.find(action);
    return (it != actionMap.end()) ? it->second : "Unknown";
}

ActionType GameGUI::stringToActionType(const std::string& actionStr) const {
    static const std::map<std::string, ActionType> actionMap = {
        {"Gather", ActionType::Gather}, {"Tax", ActionType::Tax},
        {"Bribe", ActionType::Bribe}, {"Arrest", ActionType::Arrest},
        {"Coup", ActionType::Coup}, {"Sanction", ActionType::Sanction},
        {"Invest", ActionType::Invest}, {"Spy On", ActionType::SpyOn},
        {"Cancel", ActionType::cancel}
    };
    auto it = actionMap.find(actionStr);
    if (it != actionMap.end()) {
        return it->second;
    }
    // This should ideally not happen if UI buttons are generated correctly
    throw std::runtime_error("Invalid action string provided: " + actionStr);
}

// === GameGUI Class Implementation ===

GameGUI::GameGUI(Game& gameRef) : window(sf::VideoMode(800, 600), "Coup Game by IDO"), game(gameRef) {
    window.setFramerateLimit(60);
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        throw std::runtime_error("Fatal Error: Failed to load font 'assets/fonts/arial.ttf'. Make sure it's in the execution directory.");
    }
}

void GameGUI::run() {
    showWelcomeScreen();
    showPlayerInputScreen();
    runGameScreen();
}

void GameGUI::showWelcomeScreen() {
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(static_cast<float>(window.getSize().x) / bgTexture.getSize().x, static_cast<float>(window.getSize().y) / bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // Fallback to colored background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(sf::Color(50, 50, 100)); // Dark blue background
    }

    sf::Text title("Welcome to Coup", font, 40);
    title.setFillColor(sf::Color::White);
    title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2.f, 150.f);

    sf::RectangleShape startButton(sf::Vector2f(200, 60));
    startButton.setFillColor(sf::Color(100, 100, 250));
    startButton.setPosition((window.getSize().x - startButton.getSize().x) / 2.f, 300.f);

    sf::Text startText("Start Game", font, 24);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(startButton.getPosition().x + (startButton.getSize().x - startText.getLocalBounds().width) / 2.f, startButton.getPosition().y + 15.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return; // Exit application
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    return; // Proceed to the next screen
                }
            }
        }
        window.clear();
        if (hasTexture) {
            window.draw(background);
        } else {
            window.draw(colorBackground);
        }
        window.draw(title);
        window.draw(startButton);
        window.draw(startText);
        window.display();
    }
}

void GameGUI::showPlayerInputScreen() {
    std::vector<std::string> playerNames;
    std::string currentInput;
    std::string errorMessage;
    sf::Clock errorClock;
    
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("wood_background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(static_cast<float>(window.getSize().x) / bgTexture.getSize().x, static_cast<float>(window.getSize().y) / bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // Fallback to colored background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(sf::Color(100, 50, 50)); // Dark red background
    }

    sf::Text instruction("Enter player name (2-6 players) and press Enter:", font, 24);
    instruction.setFillColor(sf::Color::White);
    instruction.setPosition((window.getSize().x - instruction.getLocalBounds().width) / 2.f, 50.f);

    sf::Text inputText("", font, 24); // Declare inputText properly
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setPosition(200.f, 100.f);

    sf::Text playerListText("", font, 20); // Ensure playerListText is also declared properly
    playerListText.setFillColor(sf::Color::White);
    playerListText.setPosition(200.f, 160.f);

    sf::Text errorText("", font, 20);
    errorText.setFillColor(sf::Color::Red);
    errorText.setPosition(200.f, 130.f);

    sf::RectangleShape startGameButton(sf::Vector2f(200, 50));
    startGameButton.setFillColor(sf::Color(0, 150, 0));
    startGameButton.setPosition(300.f, 500.f);

    sf::Text startGameText("Start Game", font, 24);
    startGameText.setFillColor(sf::Color::White);
    startGameText.setPosition(startGameButton.getPosition().x + (startGameButton.getSize().x - startGameText.getLocalBounds().width) / 2.f, startGameButton.getPosition().y + 10.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8 && !currentInput.empty()) { // Backspace
                    currentInput.pop_back();
                } else if (event.text.unicode == 13 || event.text.unicode == 10) { // Enter
                    if (!currentInput.empty()) {
                        bool nameExists = std::any_of(playerNames.begin(), playerNames.end(), [&](const std::string& name) { return name == currentInput; });

                        if (nameExists) {
                            errorMessage = "Name already exists. Please enter a unique name.";
                            errorClock.restart();
                        } else if (playerNames.size() == 6) {
                            errorMessage = "Maximum of 6 players reached. Please start the game.";
                            errorClock.restart();
                        } else {
                            playerNames.push_back(currentInput);
                            currentInput.clear();
                            errorMessage.clear();
                        }
                    }
                } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    currentInput += static_cast<char>(event.text.unicode);
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (startGameButton.getGlobalBounds().contains(mousePos)) {
                    if (playerNames.size() >= 2) {
                        showRoleRevealScreen(playerNames);
                        return;
                    } else {
                        errorMessage = "You need at least 2 players to start.";
                        errorClock.restart();
                    }
                }
            }
        }

        inputText.setString("> " + currentInput);
        std::string allPlayersStr;
        for (const auto& name : playerNames) {
            allPlayersStr += name + "\n";
        }
        playerListText.setString(allPlayersStr);

        if (!errorMessage.empty() && errorClock.getElapsedTime().asSeconds() > 3.0f) {
            errorMessage.clear();
        }
        errorText.setString(errorMessage);

        window.clear();
        if (hasTexture) {
            window.draw(background);
        } else {
            window.draw(colorBackground);
        }
        window.draw(instruction);
        window.draw(inputText);
        window.draw(playerListText);
        if (!errorMessage.empty()) window.draw(errorText);
        window.draw(startGameButton);
        window.draw(startGameText);
        window.display();
    }
}

void GameGUI::showRoleRevealScreen(const std::vector<std::string>& playerNames) {
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(window.getSize().x / (float)bgTexture.getSize().x, window.getSize().y / (float)bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // Fallback to colored background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(sf::Color(50, 100, 50)); // Dark green background
    }

    std::vector<std::string> roles = {"General", "Governor", "Judge", "Merchant", "Baron", "Spy"};
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(roles.begin(), roles.end(), rng);

    for (size_t i = 0; i < playerNames.size(); ++i) {
        if (!window.isOpen()) return;

        bool revealed = false;
        std::string assignedRole = roles[i % roles.size()];

        sf::Text playerText(playerNames[i] + ", get ready...", font, 30);
        playerText.setFillColor(sf::Color::White);
        playerText.setPosition((window.getSize().x - playerText.getLocalBounds().width) / 2.f, 150.f);

        sf::RectangleShape revealButton(sf::Vector2f(200, 60));
        revealButton.setFillColor(sf::Color(100, 100, 250));
        revealButton.setPosition(300.f, 400.f);

        sf::Text revealText("Reveal Role", font, 24);
        revealText.setFillColor(sf::Color::White);
        revealText.setPosition(revealButton.getPosition().x + (revealButton.getSize().x - revealText.getLocalBounds().width) / 2.f, revealButton.getPosition().y + 15.f);
        
        sf::Clock revealTimer;
        bool waiting = false;
        
        while (window.isOpen() && !revealed) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !waiting) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (revealButton.getGlobalBounds().contains(mousePos)) {
                        game.addPlayer(playerNames[i], assignedRole);
                        waiting = true;
                        revealTimer.restart();
                    }
                }
            }

            window.clear();
            if (hasTexture) {
                window.draw(background);
            } else {
                window.draw(colorBackground);
            }

            if (waiting) {
                sf::Text roleText("Your Role: " + assignedRole, font, 40);
                roleText.setFillColor(sf::Color::Yellow);
                roleText.setPosition((window.getSize().x - roleText.getLocalBounds().width) / 2.f, 200.f);
                
                playerText.setString(playerNames[i] + ", this is your role:");
                playerText.setPosition((window.getSize().x - playerText.getLocalBounds().width) / 2.f, 100.f);
                window.draw(playerText);
                window.draw(roleText);
                
                if (revealTimer.getElapsedTime().asSeconds() > 2.0f) {
                    revealed = true;
                }
            } else {
                window.draw(playerText);
                window.draw(revealButton);
                window.draw(revealText);
            }
            
            window.display();
        }
    }
    game.startGame();
}

void GameGUI::runGameScreen() {
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("gametable.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(static_cast<float>(window.getSize().x) / bgTexture.getSize().x, static_cast<float>(window.getSize().y) / bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // Fallback to colored background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(sf::Color(80, 80, 80)); // Dark gray background
    }
    
    while (window.isOpen()) {
        // Count active players directly
        int activePlayers = 0;
        for (const auto& p : game.getPlayers()) {
            if (p->isActive()) {
                activePlayers++;
            }
        }
        
        std::cout << "Debug: getNumPlayers()=" << game.getNumPlayers() << ", actual active players=" << activePlayers << std::endl;
        
        if (activePlayers <= 1) {
            try {
                showWinnerScreen(game.winner());
            } catch (const std::runtime_error&) {
                 showWinnerScreen("No one"); // In case of an empty game or other errors
            }
            return;
        }

        Player* currentPlayer = game.getCurrentPlayer();
        
        // Check if current player is active, if not try to advance to next active player
        if (!currentPlayer->isActive()) {
            // Count active players first to see if game should end
            int activePlayerCount = 0;
            for (const auto& player : game.getPlayers()) {
                if (player->isActive()) {
                    activePlayerCount++;
                }
            }
            
            // If only one or no active players remain, the game is over
            if (activePlayerCount <= 1) {
                if (activePlayerCount == 1) {
                    // Find the last active player and declare winner
                    for (const auto& player : game.getPlayers()) {
                        if (player->isActive()) {
                            showWinnerScreen(player->getName());
                            return;
                        }
                    }
                }
                showWinnerScreen("No one");
                return;
            }
            
            // Safe to advance turn since there are enough active players
            try {
                game.nextTurn();
                currentPlayer = game.getCurrentPlayer();
            } catch (const std::runtime_error& e) {
                std::cout << "Error advancing turn: " << e.what() << std::endl;
                break;
            }
        }
        std::string roleStr = roleToString(currentPlayer->getRoleType());

        // Left side: Player Info
        float leftMargin = 50.f;
        float startY = 50.f;
        float lineSpacing = 40.f;
        sf::Text turnText(currentPlayer->getName() + "'s turn", font, 30);
        turnText.setFillColor(sf::Color::White);
        turnText.setPosition(leftMargin, startY);

        sf::Text roleText("Role: " + roleStr, font, 20);
        roleText.setFillColor(sf::Color::White);
        roleText.setPosition(leftMargin, startY + lineSpacing);

        sf::Text coinsText("Coins: " + std::to_string(currentPlayer->getCoins()), font, 20);
        coinsText.setFillColor(sf::Color::White);
        coinsText.setPosition(leftMargin, startY + 2 * lineSpacing);

        sf::Text statusText(std::string("Sanctioned: ") + (currentPlayer->isSanctioned() ? "Yes" : "No"), font, 20);
        statusText.setFillColor(sf::Color::Red);
        statusText.setPosition(leftMargin, startY + 3 * lineSpacing);

        std::string alivePlayersStr = "Alive Players: ";
        for (const auto& p : game.getPlayers()) {
            if (p->isActive()) {
                alivePlayersStr += p->getName() + " ";
            }
        }
        sf::Text alivePlayersText(alivePlayersStr, font, 18);
        alivePlayersText.setFillColor(sf::Color::Cyan);
        alivePlayersText.setPosition(leftMargin, startY + 5 * lineSpacing);
        
        // Right side: Action Buttons
        std::vector<ActionType> availableActions = currentPlayer->getAvailableActions();
        std::vector<sf::RectangleShape> actionButtons;
        std::vector<sf::Text> actionTexts;
        float rightMargin = window.getSize().x - 250.f;
        float buttonHeight = 45.f;
        float buttonSpacing = 15.f;

        size_t button_idx = 0;
        for (ActionType action : availableActions) {
            if (action == ActionType::cancel) continue; // "cancel" is not a player-initiated action button
            
            sf::RectangleShape button(sf::Vector2f(200, buttonHeight));
            button.setFillColor(sf::Color(150, 100, 100));
            button.setPosition(rightMargin, startY + button_idx * (buttonHeight + buttonSpacing));
            actionButtons.push_back(button);

            std::string actionStr = actionTypeToString(action);
            sf::Text text(actionStr, font, 20);
            text.setFillColor(sf::Color::White);
            text.setPosition(button.getPosition().x + (button.getSize().x - text.getLocalBounds().width) / 2.f, button.getPosition().y + 10.f);
            actionTexts.push_back(text);
            button_idx++;
        }

        bool turnEnded = false;
        while (window.isOpen() && !turnEnded) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return;
                }
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    for (size_t i = 0; i < actionButtons.size(); ++i) {
                        if (actionButtons[i].getGlobalBounds().contains(mousePos)) {
                            ActionType action = stringToActionType(actionTexts[i].getString());
                            Player* targetPlayer = nullptr;

                            try {
                                if (action == ActionType::Coup || action == ActionType::Arrest || action == ActionType::Sanction || action == ActionType::SpyOn) {
                                    std::string targetName = askForTargetPlayerName();
                                    if (targetName.empty()) continue; // User cancelled target selection
                                    
                                    bool found = false;
                                    for (Player* p : game.getPlayers()) {
                                        if (p->getName() == targetName) {
                                            targetPlayer = p;
                                            found = true;
                                            break;
                                        }
                                    }
                                    if (!found) {
                                        showErrorPopup("Player '" + targetName + "' not found.");
                                        continue;
                                    }
                                }

                                ActionType executedAction = action; // Store the action before potential modification

                                switch (action) {
                                    case ActionType::Gather: currentPlayer->gather(); break;
                                    case ActionType::Tax: currentPlayer->tax(); break;
                                    case ActionType::Bribe: currentPlayer->bribe(); break;
                                    case ActionType::Invest: currentPlayer->invest(); break;
                                    case ActionType::Coup: currentPlayer->coup(*targetPlayer); break;
                                    case ActionType::Arrest: currentPlayer->arrest(*targetPlayer); break;
                                    case ActionType::Sanction: currentPlayer->sanction(*targetPlayer); break;
                                    case ActionType::SpyOn: 
                                        currentPlayer->spyOn(*targetPlayer);
                                        viewPlayerCoinsPopup(targetPlayer->getName());
                                        break;
                                    default: break;
                                }

                                turnEnded = true;

                                // Check for cancellation possibilities after the action
                                for (Player* p : game.getPlayers()) {
                                    // A player can cancel if they are active, not the current player, and their role allows it.
                                    if (p->isActive() && p != currentPlayer && p->canCancel(executedAction)) {
                                        std::string answer = showCancelConfirmation(p->getName());
                                        if (answer == "yes") {
                                            try {
                                                // The target of the cancel action depends on the original action
                                                Player& cancelTarget = (executedAction == ActionType::Coup) ? *targetPlayer : *currentPlayer;
                                                p->cancel(cancelTarget);
                                            } catch (const std::runtime_error& cancel_e) {
                                                showErrorPopup(cancel_e.what());
                                            }
                                            break; // Only one player can cancel
                                        }
                                    }
                                }

                                // Note: All actions advance turn automatically, so we don't need to call nextTurn() here

                            } catch (const std::runtime_error& e) {
                                showErrorPopup(e.what());
                                // Turn does not end on error, allowing player to choose another action
                            }
                        }
                    }
                }
            }

            // Drawing the screen
            window.clear();
            if (hasTexture) {
                window.draw(background);
            } else {
                window.draw(colorBackground);
            }
            window.draw(turnText);
            window.draw(roleText);
            window.draw(coinsText);
            window.draw(statusText);
            window.draw(alivePlayersText);

            for (size_t i = 0; i < actionButtons.size(); ++i) {
                window.draw(actionButtons[i]);
                window.draw(actionTexts[i]);
            }
            window.display();
        }
    }
}

std::string GameGUI::askForTargetPlayerName() {
    sf::RenderWindow inputWindow(sf::VideoMode(400, 200), "Select Target", sf::Style::Titlebar | sf::Style::Close);
    
    sf::Text prompt("Enter target player name:", font, 20);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(20, 30);

    std::string inputText;
    sf::Text inputDisplay("", font, 20);
    inputDisplay.setFillColor(sf::Color::Yellow);
    inputDisplay.setPosition(20, 70);

    while (inputWindow.isOpen()) {
        sf::Event event;
        while (inputWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                inputWindow.close();
                return ""; // Window closed by user
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !inputText.empty()) { // Backspace
                    inputText.pop_back();
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') { // Enter
                    inputWindow.close();
                    return inputText;
                } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    inputText += static_cast<char>(event.text.unicode);
                }
                inputDisplay.setString("> " + inputText);
            }
        }
        inputWindow.clear(sf::Color(30, 30, 30));
        inputWindow.draw(prompt);
        inputWindow.draw(inputDisplay);
        inputWindow.display();
    }
    return inputText;
}

void GameGUI::showWinnerScreen(const std::string& winnerName) {
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(window.getSize().x / (float)bgTexture.getSize().x, window.getSize().y / (float)bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // Fallback to colored background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(sf::Color(100, 100, 0)); // Dark yellow background
    }

    sf::Text gameOverText("GAME OVER", font, 60);
    gameOverText.setFillColor(sf::Color(255, 50, 50));
    gameOverText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2.f, 100.f);

    sf::Text winText("Winner: " + winnerName, font, 40);
    winText.setFillColor(sf::Color(255, 215, 0));
    winText.setPosition((window.getSize().x - winText.getLocalBounds().width) / 2.f, 250.f);

    sf::RectangleShape exitButton(sf::Vector2f(200, 60));
    exitButton.setPosition((window.getSize().x - exitButton.getSize().x) / 2.f, 400.f);
    exitButton.setFillColor(sf::Color(70, 130, 180));

    sf::Text buttonText("Exit Game", font, 26);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(exitButton.getPosition().x + (exitButton.getSize().x - buttonText.getLocalBounds().width) / 2.f, exitButton.getPosition().y + 15.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                if (exitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                }
            }
        }
        window.clear();
        if (hasTexture) {
            window.draw(background);
        } else {
            window.draw(colorBackground);
        }
        window.draw(gameOverText);
        window.draw(winText);
        window.draw(exitButton);
        window.draw(buttonText);
        window.display();
    }
}

void GameGUI::viewPlayerCoinsPopup(const std::string& targetName) {
    Player* target = nullptr;
    for(Player* p : game.getPlayers()) {
        if (p->getName() == targetName) {
            target = p;
            break;
        }
    }
    if (!target) return; // Should not happen if logic is correct

    sf::RenderWindow popupWindow(sf::VideoMode(400, 200), "Spy Report", sf::Style::Titlebar | sf::Style::Close);
    
    sf::Text infoText(targetName + " has " + std::to_string(target->getCoins()) + " coins.", font, 24);
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition((popupWindow.getSize().x - infoText.getLocalBounds().width) / 2.f, 60.f);

    sf::RectangleShape okButton(sf::Vector2f(100, 40));
    okButton.setFillColor(sf::Color(100, 200, 250));
    okButton.setPosition((popupWindow.getSize().x - okButton.getSize().x) / 2.f, 130.f);

    sf::Text okText("OK", font, 20);
    okText.setFillColor(sf::Color::Black);
    okText.setPosition(okButton.getPosition().x + (okButton.getSize().x - okText.getLocalBounds().width) / 2.f, okButton.getPosition().y + 7.f);

    while (popupWindow.isOpen()) {
        sf::Event event;
        while (popupWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) popupWindow.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse = sf::Mouse::getPosition(popupWindow);
                if (okButton.getGlobalBounds().contains(static_cast<float>(mouse.x), static_cast<float>(mouse.y))) {
                    popupWindow.close();
                }
            }
        }

        popupWindow.clear(sf::Color(30, 30, 30));
        popupWindow.draw(infoText);
        popupWindow.draw(okButton);
        popupWindow.draw(okText);
        popupWindow.display();
    }
}

std::string GameGUI::showCancelConfirmation(const std::string& playerName) {
    sf::RenderWindow popupWindow(sf::VideoMode(460, 250), "Cancel Action?", sf::Style::Titlebar);

    sf::Text questionText(playerName + ", do you want to cancel?", font, 22);
    questionText.setFillColor(sf::Color::White);
    questionText.setPosition((popupWindow.getSize().x - questionText.getLocalBounds().width) / 2.f, 40.f);

    sf::RectangleShape yesButton(sf::Vector2f(120, 50));
    yesButton.setFillColor(sf::Color(70, 200, 70));
    yesButton.setPosition(60.f, 150.f);
    sf::Text yesText("Yes", font, 22);
    yesText.setFillColor(sf::Color::Black);
    yesText.setPosition(yesButton.getPosition().x + (yesButton.getSize().x - yesText.getLocalBounds().width) / 2.f, yesButton.getPosition().y + 10.f);

    sf::RectangleShape noButton(sf::Vector2f(120, 50));
    noButton.setFillColor(sf::Color(200, 70, 70));
    noButton.setPosition(280.f, 150.f);
    sf::Text noText("No", font, 22);
    noText.setFillColor(sf::Color::Black);
    noText.setPosition(noButton.getPosition().x + (noButton.getSize().x - noText.getLocalBounds().width) / 2.f, noButton.getPosition().y + 10.f);

    while (popupWindow.isOpen()) {
        sf::Event event;
        while (popupWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                popupWindow.close();
                return "no";
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(popupWindow);
                if (yesButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    popupWindow.close();
                    return "yes";
                }
                if (noButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    popupWindow.close();
                    return "no";
                }
            }
        }

        popupWindow.clear(sf::Color(50, 50, 50));
        popupWindow.draw(questionText);
        popupWindow.draw(yesButton);
        popupWindow.draw(yesText);
        popupWindow.draw(noButton);
        popupWindow.draw(noText);
        popupWindow.display();
    }
    return "no";
}

void GameGUI::showErrorPopup(const std::string& errorMessage) {
    sf::RenderWindow popupWindow(sf::VideoMode(500, 200), "Error", sf::Style::Titlebar | sf::Style::Close);

    sf::Text message(errorMessage, font, 20);
    message.setFillColor(sf::Color(255, 80, 80));
    message.setPosition((popupWindow.getSize().x - message.getLocalBounds().width) / 2.f, 50.f);

    sf::RectangleShape okButton(sf::Vector2f(120, 40));
    okButton.setFillColor(sf::Color(200, 100, 100));
    okButton.setPosition((popupWindow.getSize().x - okButton.getSize().x) / 2.f, 130.f);

    sf::Text okText("OK", font, 20);
    okText.setFillColor(sf::Color::White);
    okText.setPosition(okButton.getPosition().x + (okButton.getSize().x - okText.getLocalBounds().width) / 2.f, okButton.getPosition().y + 5.f);

    while (popupWindow.isOpen()) {
        sf::Event event;
        while (popupWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                popupWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(popupWindow);
                if (okButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    popupWindow.close();
                }
            }
        }
        popupWindow.clear(sf::Color(50, 20, 20));
        popupWindow.draw(message);
        popupWindow.draw(okButton);
        popupWindow.draw(okText);
        popupWindow.display();
    }
}

} // namespace coup