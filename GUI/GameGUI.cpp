// idocohen963@gmail.com
#include "GameGUI.hpp"
#include <iostream>
#include <random>
#include <map>
#include <algorithm>
#include <chrono> // For sf::Clock
#include "PLAYER/PlayerFactory.hpp" 

namespace coup {

// === Visual Design Constants - DESIGN IMPROVEMENT ===
// Modern color palette for enhanced UI
namespace VisualStyle {
    // Primary colors
    const sf::Color PRIMARY_DARK = sf::Color(26, 32, 44);        // Dark blue-gray
    const sf::Color PRIMARY_MEDIUM = sf::Color(45, 55, 72);      // Medium blue-gray
    const sf::Color PRIMARY_LIGHT = sf::Color(74, 85, 104);      // Light blue-gray
    
    // Accent colors
    const sf::Color ACCENT_BLUE = sf::Color(66, 153, 225);       // Modern blue
    const sf::Color ACCENT_GREEN = sf::Color(72, 187, 120);      // Modern green
    const sf::Color ACCENT_RED = sf::Color(245, 101, 101);       // Modern red
    const sf::Color ACCENT_YELLOW = sf::Color(237, 203, 67);     // Modern yellow
    const sf::Color ACCENT_PURPLE = sf::Color(159, 122, 234);    // Modern purple
    
    // Text colors
    const sf::Color TEXT_PRIMARY = sf::Color(255, 255, 255);     // White
    const sf::Color TEXT_SECONDARY = sf::Color(160, 174, 192);   // Light gray
    const sf::Color TEXT_ACCENT = sf::Color(237, 203, 67);       // Yellow accent
    
    // Background gradients (simulated with solid colors)
    const sf::Color BG_GRADIENT_TOP = sf::Color(45, 55, 72);
    const sf::Color BG_GRADIENT_BOTTOM = sf::Color(26, 32, 44);
    
    // Button styles
    const sf::Color BUTTON_PRIMARY = sf::Color(66, 153, 225);
    const sf::Color BUTTON_SUCCESS = sf::Color(72, 187, 120);
    const sf::Color BUTTON_DANGER = sf::Color(245, 101, 101);
    const sf::Color BUTTON_WARNING = sf::Color(237, 203, 67);
    const sf::Color BUTTON_HOVER = sf::Color(90, 170, 240);      // Lighter blue for hover effect
}

// Helper function to create rounded rectangle shape - DESIGN IMPROVEMENT
sf::RectangleShape createRoundedButton(sf::Vector2f size, sf::Vector2f position, sf::Color fillColor) {
    sf::RectangleShape button(size);
    button.setPosition(position);
    button.setFillColor(fillColor);
    // Note: SFML doesn't support rounded corners natively, but we can simulate with outline
    button.setOutlineThickness(2.f);
    button.setOutlineColor(sf::Color(fillColor.r + 30, fillColor.g + 30, fillColor.b + 30, 180));
    return button;
}

// Helper function to create shadow effect - DESIGN IMPROVEMENT
sf::RectangleShape createShadow(sf::Vector2f size, sf::Vector2f position, float offset = 4.f) {
    sf::RectangleShape shadow(size);
    shadow.setPosition(position.x + offset, position.y + offset);
    shadow.setFillColor(sf::Color(0, 0, 0, 80)); // Semi-transparent black
    return shadow;
}

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

GameGUI::GameGUI(Game& gameRef) : window(sf::VideoMode(900, 700), "Coup Game - Modern Edition"), game(gameRef) {
    // DESIGN IMPROVEMENT: Larger window size for better layout and modern styling
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
    // DESIGN IMPROVEMENT: Modern gradient background instead of single color
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(static_cast<float>(window.getSize().x) / bgTexture.getSize().x, static_cast<float>(window.getSize().y) / bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // DESIGN IMPROVEMENT: Modern gradient-like background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(VisualStyle::PRIMARY_DARK); // Modern dark background
    }

    // DESIGN IMPROVEMENT: Enhanced title with modern typography
    sf::Text title("COUP", font, 64);
    title.setFillColor(VisualStyle::TEXT_ACCENT);
    title.setStyle(sf::Text::Bold);
    title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2.f, 120.f);
    
    // DESIGN IMPROVEMENT: Subtitle for better visual hierarchy
    sf::Text subtitle("Strategic Card Game", font, 24);
    subtitle.setFillColor(VisualStyle::TEXT_SECONDARY);
    subtitle.setPosition((window.getSize().x - subtitle.getLocalBounds().width) / 2.f, 200.f);

    // DESIGN IMPROVEMENT: Modern button with shadow and rounded appearance
    sf::Vector2f buttonSize(240, 70);
    sf::Vector2f buttonPos((window.getSize().x - buttonSize.x) / 2.f, 320.f);
    
    // Create shadow first (drawn behind button)
    sf::RectangleShape buttonShadow = createShadow(buttonSize, buttonPos, 6.f);
    
    // Create main button with modern styling
    sf::RectangleShape startButton = createRoundedButton(buttonSize, buttonPos, VisualStyle::BUTTON_PRIMARY);

    // DESIGN IMPROVEMENT: Enhanced button text styling
    sf::Text startText("START GAME", font, 28);
    startText.setFillColor(VisualStyle::TEXT_PRIMARY);
    startText.setStyle(sf::Text::Bold);
    startText.setPosition(buttonPos.x + (buttonSize.x - startText.getLocalBounds().width) / 2.f, buttonPos.y + 18.f);

    // DESIGN IMPROVEMENT: Welcome message with modern styling
    sf::Text welcomeMsg("Welcome to the world of political intrigue", font, 18);
    welcomeMsg.setFillColor(VisualStyle::TEXT_SECONDARY);
    welcomeMsg.setPosition((window.getSize().x - welcomeMsg.getLocalBounds().width) / 2.f, 480.f);

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
        
        // DESIGN IMPROVEMENT: Enhanced drawing order for proper layering
        window.clear(VisualStyle::PRIMARY_DARK);
        if (hasTexture) {
            window.draw(background);
        } else {
            window.draw(colorBackground);
        }
        
        // Draw shadow first, then button
        window.draw(buttonShadow);
        window.draw(startButton);
        
        // Draw all text elements
        window.draw(title);
        window.draw(subtitle);
        window.draw(startText);
        window.draw(welcomeMsg);
        
        window.display();
    }
}

void GameGUI::showPlayerInputScreen() {
    std::vector<std::string> playerNames;
    std::string currentInput;
    std::string errorMessage;
    sf::Clock errorClock;
    
    // DESIGN IMPROVEMENT: Modern background styling
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("wood_background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(static_cast<float>(window.getSize().x) / bgTexture.getSize().x, static_cast<float>(window.getSize().y) / bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // DESIGN IMPROVEMENT: Modern gradient background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(VisualStyle::PRIMARY_MEDIUM);
    }

    // DESIGN IMPROVEMENT: Enhanced header with modern typography
    sf::Text headerTitle("Setup Players", font, 36);
    headerTitle.setFillColor(VisualStyle::TEXT_ACCENT);
    headerTitle.setStyle(sf::Text::Bold);
    headerTitle.setPosition((window.getSize().x - headerTitle.getLocalBounds().width) / 2.f, 30.f);

    // DESIGN IMPROVEMENT: More descriptive instruction with better styling
    sf::Text instruction("Enter player names (2-6 players) and press Enter:", font, 20);
    instruction.setFillColor(VisualStyle::TEXT_SECONDARY);
    instruction.setPosition((window.getSize().x - instruction.getLocalBounds().width) / 2.f, 80.f);

    // DESIGN IMPROVEMENT: Input field with modern styling and background
    sf::RectangleShape inputBox(sf::Vector2f(400, 40));
    inputBox.setPosition((window.getSize().x - inputBox.getSize().x) / 2.f, 120.f);
    inputBox.setFillColor(VisualStyle::PRIMARY_LIGHT);
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(VisualStyle::ACCENT_BLUE);

    sf::Text inputText("", font, 20);
    inputText.setFillColor(VisualStyle::TEXT_ACCENT);
    inputText.setPosition(inputBox.getPosition().x + 10.f, inputBox.getPosition().y + 8.f);

    // DESIGN IMPROVEMENT: Player list with card-like styling
    sf::RectangleShape playerListBox(sf::Vector2f(350, 200));
    playerListBox.setPosition((window.getSize().x - playerListBox.getSize().x) / 2.f, 180.f);
    playerListBox.setFillColor(VisualStyle::PRIMARY_DARK);
    playerListBox.setOutlineThickness(1.f);
    playerListBox.setOutlineColor(VisualStyle::PRIMARY_LIGHT);

    sf::Text playerListTitle("Players Added:", font, 18);
    playerListTitle.setFillColor(VisualStyle::TEXT_SECONDARY);
    playerListTitle.setPosition(playerListBox.getPosition().x + 10.f, playerListBox.getPosition().y + 10.f);

    sf::Text playerListText("", font, 16);
    playerListText.setFillColor(VisualStyle::TEXT_PRIMARY);
    playerListText.setPosition(playerListBox.getPosition().x + 10.f, playerListBox.getPosition().y + 35.f);

    // DESIGN IMPROVEMENT: Modern error message styling
    sf::Text errorText("", font, 16);
    errorText.setFillColor(VisualStyle::ACCENT_RED);
    errorText.setPosition((window.getSize().x - 400) / 2.f, 400.f);

    // DESIGN IMPROVEMENT: Enhanced start button with shadow
    sf::Vector2f startButtonSize(220, 60);
    sf::Vector2f startButtonPos((window.getSize().x - startButtonSize.x) / 2.f, 450.f);
    
    sf::RectangleShape startButtonShadow = createShadow(startButtonSize, startButtonPos, 4.f);
    sf::RectangleShape startGameButton = createRoundedButton(startButtonSize, startButtonPos, VisualStyle::BUTTON_SUCCESS);

    sf::Text startGameText("START GAME", font, 22);
    startGameText.setFillColor(VisualStyle::TEXT_PRIMARY);
    startGameText.setStyle(sf::Text::Bold);
    startGameText.setPosition(startButtonPos.x + (startButtonSize.x - startGameText.getLocalBounds().width) / 2.f, startButtonPos.y + 16.f);

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

        // Update display strings
        inputText.setString("> " + currentInput);
        std::string allPlayersStr;
        for (size_t i = 0; i < playerNames.size(); ++i) {
            allPlayersStr += std::to_string(i + 1) + ". " + playerNames[i] + "\n";
        }
        playerListText.setString(allPlayersStr);

        if (!errorMessage.empty() && errorClock.getElapsedTime().asSeconds() > 3.0f) {
            errorMessage.clear();
        }
        errorText.setString(errorMessage);

        // DESIGN IMPROVEMENT: Enhanced drawing with proper layering
        window.clear(VisualStyle::PRIMARY_DARK);
        if (hasTexture) {
            window.draw(background);
        } else {
            window.draw(colorBackground);
        }
        
        // Draw UI elements with shadows
        window.draw(startButtonShadow);
        
        // Draw main elements
        window.draw(headerTitle);
        window.draw(instruction);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(playerListBox);
        window.draw(playerListTitle);
        window.draw(playerListText);
        
        if (!errorMessage.empty()) window.draw(errorText);
        
        window.draw(startGameButton);
        window.draw(startGameText);
        window.display();
    }
}

void GameGUI::showRoleRevealScreen(const std::vector<std::string>& playerNames) {
    // DESIGN IMPROVEMENT: Modern background with enhanced styling
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(window.getSize().x / (float)bgTexture.getSize().x, window.getSize().y / (float)bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // DESIGN IMPROVEMENT: Rich gradient-like background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(VisualStyle::PRIMARY_DARK);
    }

    std::vector<std::string> roles = {"General", "Governor", "Judge", "Merchant", "Baron", "Spy"};
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(roles.begin(), roles.end(), rng);

    for (size_t i = 0; i < playerNames.size(); ++i) {
        if (!window.isOpen()) return;

        bool revealed = false;
        std::string assignedRole = roles[i % roles.size()];

        // DESIGN IMPROVEMENT: Enhanced player introduction with modern styling
        sf::Text playerText(playerNames[i] + ", prepare for your role...", font, 28);
        playerText.setFillColor(VisualStyle::TEXT_PRIMARY);
        playerText.setStyle(sf::Text::Bold);
        playerText.setPosition((window.getSize().x - playerText.getLocalBounds().width) / 2.f, 150.f);

        // DESIGN IMPROVEMENT: Instruction text with better styling
        sf::Text instructionText("Click the button when ready to reveal", font, 18);
        instructionText.setFillColor(VisualStyle::TEXT_SECONDARY);
        instructionText.setPosition((window.getSize().x - instructionText.getLocalBounds().width) / 2.f, 190.f);

        // DESIGN IMPROVEMENT: Modern reveal button with shadow and enhanced styling
        sf::Vector2f revealButtonSize(250, 70);
        sf::Vector2f revealButtonPos((window.getSize().x - revealButtonSize.x) / 2.f, 350.f);
        
        sf::RectangleShape revealButtonShadow = createShadow(revealButtonSize, revealButtonPos, 5.f);
        sf::RectangleShape revealButton = createRoundedButton(revealButtonSize, revealButtonPos, VisualStyle::BUTTON_PRIMARY);

        sf::Text revealText("REVEAL ROLE", font, 24);
        revealText.setFillColor(VisualStyle::TEXT_PRIMARY);
        revealText.setStyle(sf::Text::Bold);
        revealText.setPosition(revealButtonPos.x + (revealButtonSize.x - revealText.getLocalBounds().width) / 2.f, revealButtonPos.y + 20.f);
        
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

            window.clear(VisualStyle::PRIMARY_DARK);
            if (hasTexture) {
                window.draw(background);
            } else {
                window.draw(colorBackground);
            }

            if (waiting) {
                // DESIGN IMPROVEMENT: Role reveal with dramatic styling and card-like appearance
                sf::RectangleShape roleCard(sf::Vector2f(400, 150));
                roleCard.setPosition((window.getSize().x - roleCard.getSize().x) / 2.f, 200.f);
                roleCard.setFillColor(VisualStyle::PRIMARY_MEDIUM);
                roleCard.setOutlineThickness(3.f);
                roleCard.setOutlineColor(VisualStyle::ACCENT_YELLOW);
                
                sf::RectangleShape roleCardShadow = createShadow(sf::Vector2f(400, 150), sf::Vector2f((window.getSize().x - 400) / 2.f, 200.f), 8.f);
                
                sf::Text roleText("Your Role: " + assignedRole, font, 32);
                roleText.setFillColor(VisualStyle::TEXT_ACCENT);
                roleText.setStyle(sf::Text::Bold);
                roleText.setPosition((window.getSize().x - roleText.getLocalBounds().width) / 2.f, 240.f);
                
                playerText.setString(playerNames[i] + ", this is your role:");
                playerText.setPosition((window.getSize().x - playerText.getLocalBounds().width) / 2.f, 100.f);
                
                // DESIGN IMPROVEMENT: Remember and strategize text
                sf::Text strategyText("Remember your abilities and plan your strategy!", font, 16);
                strategyText.setFillColor(VisualStyle::TEXT_SECONDARY);
                strategyText.setPosition((window.getSize().x - strategyText.getLocalBounds().width) / 2.f, 300.f);
                
                window.draw(roleCardShadow);
                window.draw(roleCard);
                window.draw(playerText);
                window.draw(roleText);
                window.draw(strategyText);
                
                if (revealTimer.getElapsedTime().asSeconds() > 2.5f) {
                    revealed = true;
                }
            } else {
                window.draw(revealButtonShadow);
                window.draw(revealButton);
                window.draw(playerText);
                window.draw(instructionText);
                window.draw(revealText);
            }
            
            window.display();
        }
    }
    game.startGame();
}

void GameGUI::runGameScreen() {
    // DESIGN IMPROVEMENT: Enhanced game screen background
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("gametable.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(static_cast<float>(window.getSize().x) / bgTexture.getSize().x, static_cast<float>(window.getSize().y) / bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // DESIGN IMPROVEMENT: Rich game table appearance
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(VisualStyle::PRIMARY_DARK);
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

        // DESIGN IMPROVEMENT: Enhanced left panel with card-like player info
        float leftMargin = 30.f;
        float startY = 30.f;
        
        // Player info card background
        sf::RectangleShape playerInfoCard(sf::Vector2f(350, 280));
        playerInfoCard.setPosition(leftMargin, startY);
        playerInfoCard.setFillColor(VisualStyle::PRIMARY_MEDIUM);
        playerInfoCard.setOutlineThickness(2.f);
        playerInfoCard.setOutlineColor(VisualStyle::ACCENT_BLUE);
        
        sf::RectangleShape playerInfoShadow = createShadow(sf::Vector2f(350, 280), sf::Vector2f(leftMargin, startY), 5.f);

        // DESIGN IMPROVEMENT: Enhanced text styling with hierarchy
        sf::Text turnText(currentPlayer->getName() + "'s Turn", font, 26);
        turnText.setFillColor(VisualStyle::TEXT_ACCENT);
        turnText.setStyle(sf::Text::Bold);
        turnText.setPosition(leftMargin + 20.f, startY + 20.f);

        sf::Text roleText("Role: " + roleStr, font, 20);
        roleText.setFillColor(VisualStyle::TEXT_PRIMARY);
        roleText.setPosition(leftMargin + 20.f, startY + 60.f);

        sf::Text coinsText("Coins: " + std::to_string(currentPlayer->getCoins()), font, 20);
        coinsText.setFillColor(VisualStyle::ACCENT_YELLOW);
        coinsText.setStyle(sf::Text::Bold);
        coinsText.setPosition(leftMargin + 20.f, startY + 90.f);

        sf::Text statusText(std::string("Sanctioned: ") + (currentPlayer->isSanctioned() ? "Yes" : "No"), font, 18);
        statusText.setFillColor(currentPlayer->isSanctioned() ? VisualStyle::ACCENT_RED : VisualStyle::ACCENT_GREEN);
        statusText.setPosition(leftMargin + 20.f, startY + 120.f);

        // DESIGN IMPROVEMENT: Active players list with better formatting
        std::string alivePlayersStr = "Active Players:\n";
        int playerCount = 0;
        for (const auto& p : game.getPlayers()) {
            if (p->isActive()) {
                alivePlayersStr += "• " + p->getName() + " (" + std::to_string(p->getCoins()) + " coins)\n";
                playerCount++;
            }
        }
        sf::Text alivePlayersText(alivePlayersStr, font, 16);
        alivePlayersText.setFillColor(VisualStyle::TEXT_SECONDARY);
        alivePlayersText.setPosition(leftMargin + 20.f, startY + 160.f);
        
        // DESIGN IMPROVEMENT: Enhanced action buttons panel
        std::vector<ActionType> availableActions = currentPlayer->getAvailableActions();
        std::vector<sf::RectangleShape> actionButtons;
        std::vector<sf::RectangleShape> actionShadows;
        std::vector<sf::Text> actionTexts;
        
        float rightMargin = window.getSize().x - 280.f;
        float buttonHeight = 50.f;
        float buttonSpacing = 15.f;
        float actionStartY = startY + 20.f;

        // Action panel background
        sf::RectangleShape actionPanel(sf::Vector2f(250, static_cast<float>(availableActions.size() * (buttonHeight + buttonSpacing) + 40)));
        actionPanel.setPosition(rightMargin, startY);
        actionPanel.setFillColor(VisualStyle::PRIMARY_MEDIUM);
        actionPanel.setOutlineThickness(2.f);
        actionPanel.setOutlineColor(VisualStyle::ACCENT_PURPLE);
        
        sf::RectangleShape actionPanelShadow = createShadow(actionPanel.getSize(), actionPanel.getPosition(), 5.f);

        sf::Text actionTitle("Available Actions", font, 18);
        actionTitle.setFillColor(VisualStyle::TEXT_ACCENT);
        actionTitle.setStyle(sf::Text::Bold);
        actionTitle.setPosition(rightMargin + 10.f, startY + 10.f);

        size_t button_idx = 0;
        for (ActionType action : availableActions) {
            if (action == ActionType::cancel) continue; // "cancel" is not a player-initiated action button
            
            sf::Vector2f buttonSize(220, buttonHeight);
            sf::Vector2f buttonPos(rightMargin + 15.f, actionStartY + 40.f + button_idx * (buttonHeight + buttonSpacing));
            
            // DESIGN IMPROVEMENT: Color-coded action buttons
            sf::Color buttonColor = VisualStyle::BUTTON_PRIMARY;
            if (action == ActionType::Coup) buttonColor = VisualStyle::BUTTON_DANGER;
            else if (action == ActionType::Gather || action == ActionType::Tax) buttonColor = VisualStyle::BUTTON_SUCCESS;
            else if (action == ActionType::Bribe) buttonColor = VisualStyle::BUTTON_WARNING;
            
            sf::RectangleShape buttonShadow = createShadow(buttonSize, buttonPos, 3.f);
            sf::RectangleShape button = createRoundedButton(buttonSize, buttonPos, buttonColor);
            
            actionShadows.push_back(buttonShadow);
            actionButtons.push_back(button);

            std::string actionStr = actionTypeToString(action);
            sf::Text text(actionStr, font, 18);
            text.setFillColor(VisualStyle::TEXT_PRIMARY);
            text.setStyle(sf::Text::Bold);
            text.setPosition(buttonPos.x + (buttonSize.x - text.getLocalBounds().width) / 2.f, buttonPos.y + 14.f);
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

            // DESIGN IMPROVEMENT: Enhanced drawing with proper layering and modern styling
            window.clear(VisualStyle::PRIMARY_DARK);
            if (hasTexture) {
                window.draw(background);
            } else {
                window.draw(colorBackground);
            }
            
            // Draw shadows first
            window.draw(playerInfoShadow);
            window.draw(actionPanelShadow);
            for (const auto& shadow : actionShadows) {
                window.draw(shadow);
            }
            
            // Draw main elements
            window.draw(playerInfoCard);
            window.draw(actionPanel);
            
            // Draw text content
            window.draw(turnText);
            window.draw(roleText);
            window.draw(coinsText);
            window.draw(statusText);
            window.draw(alivePlayersText);
            window.draw(actionTitle);

            // Draw action buttons and their text
            for (size_t i = 0; i < actionButtons.size(); ++i) {
                window.draw(actionButtons[i]);
                window.draw(actionTexts[i]);
            }
            
            window.display();
        }
    }
}

std::string GameGUI::askForTargetPlayerName() {
    // DESIGN IMPROVEMENT: Modern modal window with enhanced styling
    sf::RenderWindow inputWindow(sf::VideoMode(450, 250), "Select Target Player", sf::Style::Titlebar | sf::Style::Close);
    
    // Background with modern colors
    sf::RectangleShape modalBackground(sf::Vector2f(450, 250));
    modalBackground.setFillColor(VisualStyle::PRIMARY_MEDIUM);
    
    // DESIGN IMPROVEMENT: Enhanced prompt styling
    sf::Text prompt("Enter target player name:", font, 22);
    prompt.setFillColor(VisualStyle::TEXT_PRIMARY);
    prompt.setStyle(sf::Text::Bold);
    prompt.setPosition(30, 40);

    // DESIGN IMPROVEMENT: Input field with modern styling
    sf::RectangleShape inputBox(sf::Vector2f(390, 40));
    inputBox.setPosition(30, 80);
    inputBox.setFillColor(VisualStyle::PRIMARY_LIGHT);
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(VisualStyle::ACCENT_BLUE);

    std::string inputText;
    sf::Text inputDisplay("", font, 18);
    inputDisplay.setFillColor(VisualStyle::TEXT_ACCENT);
    inputDisplay.setPosition(40, 88);

    // DESIGN IMPROVEMENT: Modern cancel and confirm buttons
    sf::Vector2f buttonSize(100, 35);
    sf::RectangleShape cancelButton = createRoundedButton(buttonSize, sf::Vector2f(240, 170), VisualStyle::BUTTON_DANGER);
    sf::RectangleShape confirmButton = createRoundedButton(buttonSize, sf::Vector2f(350, 170), VisualStyle::BUTTON_SUCCESS);
    
    sf::Text cancelText("Cancel", font, 16);
    cancelText.setFillColor(VisualStyle::TEXT_PRIMARY);
    cancelText.setPosition(265, 180);
    
    sf::Text confirmText("Confirm", font, 16);
    confirmText.setFillColor(VisualStyle::TEXT_PRIMARY);
    confirmText.setPosition(370, 180);

    // Instruction text
    sf::Text instructionText("Press Enter to confirm or click Cancel", font, 14);
    instructionText.setFillColor(VisualStyle::TEXT_SECONDARY);
    instructionText.setPosition(30, 140);

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
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(inputWindow);
                if (cancelButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    inputWindow.close();
                    return "";
                }
                if (confirmButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) && !inputText.empty()) {
                    inputWindow.close();
                    return inputText;
                }
            }
        }
        
        inputWindow.clear(VisualStyle::PRIMARY_DARK);
        inputWindow.draw(modalBackground);
        inputWindow.draw(prompt);
        inputWindow.draw(inputBox);
        inputWindow.draw(inputDisplay);
        inputWindow.draw(instructionText);
        inputWindow.draw(cancelButton);
        inputWindow.draw(confirmButton);
        inputWindow.draw(cancelText);
        inputWindow.draw(confirmText);
        inputWindow.display();
    }
    return inputText;
}

void GameGUI::showWinnerScreen(const std::string& winnerName) {
    // DESIGN IMPROVEMENT: Dramatic winner screen with enhanced styling
    sf::Texture bgTexture;
    sf::Sprite background;
    sf::RectangleShape colorBackground;
    bool hasTexture = false;
    
    if (bgTexture.loadFromFile("background.jpg")) {
        background.setTexture(bgTexture);
        background.setScale(window.getSize().x / (float)bgTexture.getSize().x, window.getSize().y / (float)bgTexture.getSize().y);
        hasTexture = true;
    } else {
        // DESIGN IMPROVEMENT: Celebration gradient background
        colorBackground.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        colorBackground.setFillColor(VisualStyle::PRIMARY_DARK);
    }

    // DESIGN IMPROVEMENT: Dramatic "GAME OVER" with enhanced styling
    sf::Text gameOverText("GAME OVER", font, 72);
    gameOverText.setFillColor(VisualStyle::ACCENT_RED);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition((window.getSize().x - gameOverText.getLocalBounds().width) / 2.f, 80.f);
    
    // Add outline effect for drama
    sf::Text gameOverOutline("GAME OVER", font, 72);
    gameOverOutline.setFillColor(sf::Color::Transparent);
    gameOverOutline.setOutlineColor(sf::Color(100, 0, 0));
    gameOverOutline.setOutlineThickness(3.f);
    gameOverOutline.setStyle(sf::Text::Bold);
    gameOverOutline.setPosition((window.getSize().x - gameOverOutline.getLocalBounds().width) / 2.f, 80.f);

    // DESIGN IMPROVEMENT: Winner announcement with celebration styling
    sf::RectangleShape winnerCard(sf::Vector2f(500, 120));
    winnerCard.setPosition((window.getSize().x - winnerCard.getSize().x) / 2.f, 200.f);
    winnerCard.setFillColor(VisualStyle::PRIMARY_MEDIUM);
    winnerCard.setOutlineThickness(4.f);
    winnerCard.setOutlineColor(VisualStyle::ACCENT_YELLOW);
    
    sf::RectangleShape winnerCardShadow = createShadow(sf::Vector2f(500, 120), sf::Vector2f((window.getSize().x - 500) / 2.f, 200.f), 8.f);

    sf::Text winLabel("🏆 WINNER 🏆", font, 28);
    winLabel.setFillColor(VisualStyle::ACCENT_YELLOW);
    winLabel.setStyle(sf::Text::Bold);
    winLabel.setPosition((window.getSize().x - winLabel.getLocalBounds().width) / 2.f, 220.f);

    sf::Text winText(winnerName, font, 48);
    winText.setFillColor(VisualStyle::TEXT_ACCENT);
    winText.setStyle(sf::Text::Bold);
    winText.setPosition((window.getSize().x - winText.getLocalBounds().width) / 2.f, 260.f);

    // DESIGN IMPROVEMENT: Celebration message
    sf::Text celebrationText("Congratulations! You have mastered the art of political intrigue!", font, 18);
    celebrationText.setFillColor(VisualStyle::TEXT_SECONDARY);
    celebrationText.setPosition((window.getSize().x - celebrationText.getLocalBounds().width) / 2.f, 380.f);

    // DESIGN IMPROVEMENT: Enhanced exit button with modern styling
    sf::Vector2f exitButtonSize(200, 60);
    sf::Vector2f exitButtonPos((window.getSize().x - exitButtonSize.x) / 2.f, 480.f);
    
    sf::RectangleShape exitButtonShadow = createShadow(exitButtonSize, exitButtonPos, 6.f);
    sf::RectangleShape exitButton = createRoundedButton(exitButtonSize, exitButtonPos, VisualStyle::BUTTON_DANGER);

    sf::Text buttonText("EXIT GAME", font, 24);
    buttonText.setFillColor(VisualStyle::TEXT_PRIMARY);
    buttonText.setStyle(sf::Text::Bold);
    buttonText.setPosition(exitButtonPos.x + (exitButtonSize.x - buttonText.getLocalBounds().width) / 2.f, exitButtonPos.y + 16.f);

    // DESIGN IMPROVEMENT: Play again suggestion
    sf::Text playAgainText("Thanks for playing! Click Exit to close the game.", font, 16);
    playAgainText.setFillColor(VisualStyle::TEXT_SECONDARY);
    playAgainText.setPosition((window.getSize().x - playAgainText.getLocalBounds().width) / 2.f, 580.f);

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
            // DESIGN IMPROVEMENT: Allow Escape key to exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        window.clear(VisualStyle::PRIMARY_DARK);
        if (hasTexture) {
            window.draw(background);
        } else {
            window.draw(colorBackground);
        }
        
        // Draw shadows first
        window.draw(winnerCardShadow);
        window.draw(exitButtonShadow);
        
        // Draw outline first, then main text
        window.draw(gameOverOutline);
        window.draw(gameOverText);
        
        // Draw winner celebration
        window.draw(winnerCard);
        window.draw(winLabel);
        window.draw(winText);
        window.draw(celebrationText);
        
        // Draw exit button
        window.draw(exitButton);
        window.draw(buttonText);
        window.draw(playAgainText);
        
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

    // DESIGN IMPROVEMENT: Enhanced spy report window with modern styling
    sf::RenderWindow popupWindow(sf::VideoMode(450, 280), "🕵️ Spy Report", sf::Style::Titlebar | sf::Style::Close);
    
    // Background
    sf::RectangleShape modalBackground(sf::Vector2f(450, 280));
    modalBackground.setFillColor(VisualStyle::PRIMARY_MEDIUM);
    
    // DESIGN IMPROVEMENT: Spy-themed header
    sf::Text headerText("SPY INTELLIGENCE REPORT", font, 20);
    headerText.setFillColor(VisualStyle::ACCENT_PURPLE);
    headerText.setStyle(sf::Text::Bold);
    headerText.setPosition((popupWindow.getSize().x - headerText.getLocalBounds().width) / 2.f, 30.f);
    
    // DESIGN IMPROVEMENT: Target info card
    sf::RectangleShape infoCard(sf::Vector2f(380, 100));
    infoCard.setPosition(35, 80);
    infoCard.setFillColor(VisualStyle::PRIMARY_DARK);
    infoCard.setOutlineThickness(2.f);
    infoCard.setOutlineColor(VisualStyle::ACCENT_YELLOW);
    
    sf::Text targetLabel("Target Player:", font, 16);
    targetLabel.setFillColor(VisualStyle::TEXT_SECONDARY);
    targetLabel.setPosition(50, 100);
    
    sf::Text targetText(targetName, font, 24);
    targetText.setFillColor(VisualStyle::TEXT_PRIMARY);
    targetText.setStyle(sf::Text::Bold);
    targetText.setPosition(50, 125);
    
    sf::Text coinsLabel("Current Wealth:", font, 16);
    coinsLabel.setFillColor(VisualStyle::TEXT_SECONDARY);
    coinsLabel.setPosition(50, 155);
    
    sf::Text infoText(std::to_string(target->getCoins()) + " coins", font, 24);
    infoText.setFillColor(VisualStyle::ACCENT_YELLOW);
    infoText.setStyle(sf::Text::Bold);
    infoText.setPosition(200, 155);

    // DESIGN IMPROVEMENT: Modern OK button
    sf::Vector2f okButtonSize(120, 40);
    sf::Vector2f okButtonPos((popupWindow.getSize().x - okButtonSize.x) / 2.f, 210.f);
    
    sf::RectangleShape okButtonShadow = createShadow(okButtonSize, okButtonPos, 3.f);
    sf::RectangleShape okButton = createRoundedButton(okButtonSize, okButtonPos, VisualStyle::BUTTON_SUCCESS);

    sf::Text okText("GOT IT", font, 18);
    okText.setFillColor(VisualStyle::TEXT_PRIMARY);
    okText.setStyle(sf::Text::Bold);
    okText.setPosition(okButtonPos.x + (okButtonSize.x - okText.getLocalBounds().width) / 2.f, okButtonPos.y + 10.f);

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
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                popupWindow.close();
            }
        }

        popupWindow.clear(VisualStyle::PRIMARY_DARK);
        popupWindow.draw(modalBackground);
        popupWindow.draw(headerText);
        popupWindow.draw(infoCard);
        popupWindow.draw(targetLabel);
        popupWindow.draw(targetText);
        popupWindow.draw(coinsLabel);
        popupWindow.draw(infoText);
        popupWindow.draw(okButtonShadow);
        popupWindow.draw(okButton);
        popupWindow.draw(okText);
        popupWindow.display();
    }
}

std::string GameGUI::showCancelConfirmation(const std::string& playerName) {
    // DESIGN IMPROVEMENT: Enhanced cancel confirmation with dramatic styling
    sf::RenderWindow popupWindow(sf::VideoMode(500, 300), "🚫 Cancel Action?", sf::Style::Titlebar);

    // Background
    sf::RectangleShape modalBackground(sf::Vector2f(500, 300));
    modalBackground.setFillColor(VisualStyle::PRIMARY_MEDIUM);
    
    // DESIGN IMPROVEMENT: Attention-grabbing header
    sf::Text headerText("ACTION CANCELLATION", font, 18);
    headerText.setFillColor(VisualStyle::ACCENT_RED);
    headerText.setStyle(sf::Text::Bold);
    headerText.setPosition((popupWindow.getSize().x - headerText.getLocalBounds().width) / 2.f, 30.f);

    // DESIGN IMPROVEMENT: Player name with emphasis
    sf::Text playerText(playerName, font, 28);
    playerText.setFillColor(VisualStyle::TEXT_ACCENT);
    playerText.setStyle(sf::Text::Bold);
    playerText.setPosition((popupWindow.getSize().x - playerText.getLocalBounds().width) / 2.f, 70.f);
    
    sf::Text questionText("Do you want to cancel the current action?", font, 20);
    questionText.setFillColor(VisualStyle::TEXT_PRIMARY);
    questionText.setPosition((popupWindow.getSize().x - questionText.getLocalBounds().width) / 2.f, 110.f);
    
    // DESIGN IMPROVEMENT: Warning message
    sf::Text warningText("⚠️ This decision is final and cannot be undone", font, 16);
    warningText.setFillColor(VisualStyle::TEXT_SECONDARY);
    warningText.setPosition((popupWindow.getSize().x - warningText.getLocalBounds().width) / 2.f, 150.f);

    // DESIGN IMPROVEMENT: Enhanced Yes/No buttons with proper styling
    sf::Vector2f buttonSize(140, 50);
    sf::Vector2f yesButtonPos(80.f, 200.f);
    sf::Vector2f noButtonPos(280.f, 200.f);
    
    sf::RectangleShape yesButtonShadow = createShadow(buttonSize, yesButtonPos, 4.f);
    sf::RectangleShape noButtonShadow = createShadow(buttonSize, noButtonPos, 4.f);
    
    sf::RectangleShape yesButton = createRoundedButton(buttonSize, yesButtonPos, VisualStyle::BUTTON_DANGER);
    sf::RectangleShape noButton = createRoundedButton(buttonSize, noButtonPos, VisualStyle::BUTTON_SUCCESS);
    
    sf::Text yesText("YES, CANCEL", font, 16);
    yesText.setFillColor(VisualStyle::TEXT_PRIMARY);
    yesText.setStyle(sf::Text::Bold);
    yesText.setPosition(yesButtonPos.x + (buttonSize.x - yesText.getLocalBounds().width) / 2.f, yesButtonPos.y + 16.f);

    sf::Text noText("NO, CONTINUE", font, 16);
    noText.setFillColor(VisualStyle::TEXT_PRIMARY);
    noText.setStyle(sf::Text::Bold);
    noText.setPosition(noButtonPos.x + (buttonSize.x - noText.getLocalBounds().width) / 2.f, noButtonPos.y + 16.f);

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
            // DESIGN IMPROVEMENT: Keyboard shortcuts
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Y) {
                    popupWindow.close();
                    return "yes";
                }
                if (event.key.code == sf::Keyboard::N || event.key.code == sf::Keyboard::Escape) {
                    popupWindow.close();
                    return "no";
                }
            }
        }

        popupWindow.clear(VisualStyle::PRIMARY_DARK);
        popupWindow.draw(modalBackground);
        popupWindow.draw(headerText);
        popupWindow.draw(playerText);
        popupWindow.draw(questionText);
        popupWindow.draw(warningText);
        
        // Draw shadows first
        popupWindow.draw(yesButtonShadow);
        popupWindow.draw(noButtonShadow);
        
        // Draw buttons and text
        popupWindow.draw(yesButton);
        popupWindow.draw(noButton);
        popupWindow.draw(yesText);
        popupWindow.draw(noText);
        
        popupWindow.display();
    }
    return "no";
}

void GameGUI::showErrorPopup(const std::string& errorMessage) {
    // DESIGN IMPROVEMENT: Enhanced error popup with modern styling
    sf::RenderWindow popupWindow(sf::VideoMode(550, 250), "⚠️ Error", sf::Style::Titlebar | sf::Style::Close);

    // Background
    sf::RectangleShape modalBackground(sf::Vector2f(550, 250));
    modalBackground.setFillColor(VisualStyle::PRIMARY_MEDIUM);
    
    // DESIGN IMPROVEMENT: Error header with warning styling
    sf::Text headerText("ERROR OCCURRED", font, 20);
    headerText.setFillColor(VisualStyle::ACCENT_RED);
    headerText.setStyle(sf::Text::Bold);
    headerText.setPosition((popupWindow.getSize().x - headerText.getLocalBounds().width) / 2.f, 30.f);
    
    // DESIGN IMPROVEMENT: Error message card
    sf::RectangleShape errorCard(sf::Vector2f(480, 80));
    errorCard.setPosition(35, 70);
    errorCard.setFillColor(VisualStyle::PRIMARY_DARK);
    errorCard.setOutlineThickness(2.f);
    errorCard.setOutlineColor(VisualStyle::ACCENT_RED);

    // DESIGN IMPROVEMENT: Better text wrapping and styling
    sf::Text message(errorMessage, font, 18);
    message.setFillColor(VisualStyle::TEXT_PRIMARY);
    message.setPosition(50, 95);
    
    // Simple text wrapping for long messages
    std::string wrappedMessage = errorMessage;
    if (wrappedMessage.length() > 50) {
        size_t pos = wrappedMessage.find(' ', 40);
        if (pos != std::string::npos && pos < wrappedMessage.length() - 10) {
            wrappedMessage.insert(pos, "\n");
        }
    }
    message.setString(wrappedMessage);

    // DESIGN IMPROVEMENT: Modern OK button
    sf::Vector2f okButtonSize(140, 40);
    sf::Vector2f okButtonPos((popupWindow.getSize().x - okButtonSize.x) / 2.f, 180.f);
    
    sf::RectangleShape okButtonShadow = createShadow(okButtonSize, okButtonPos, 3.f);
    sf::RectangleShape okButton = createRoundedButton(okButtonSize, okButtonPos, VisualStyle::BUTTON_DANGER);

    sf::Text okText("UNDERSTOOD", font, 16);
    okText.setFillColor(VisualStyle::TEXT_PRIMARY);
    okText.setStyle(sf::Text::Bold);
    okText.setPosition(okButtonPos.x + (okButtonSize.x - okText.getLocalBounds().width) / 2.f, okButtonPos.y + 11.f);

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
            // DESIGN IMPROVEMENT: Allow Enter key to close
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                popupWindow.close();
            }
        }
        
        popupWindow.clear(VisualStyle::PRIMARY_DARK);
        popupWindow.draw(modalBackground);
        popupWindow.draw(headerText);
        popupWindow.draw(errorCard);
        popupWindow.draw(message);
        popupWindow.draw(okButtonShadow);
        popupWindow.draw(okButton);
        popupWindow.draw(okText);
        popupWindow.display();
    }
}

} // namespace coup