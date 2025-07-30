// idocohen963@gmail.com
#include "GAME/game.hpp"
#include "PLAYER/PlayerFactory.hpp"
#include "PLAYER/player.hpp"
#include <iostream>

/**
 * @file game.cpp
 * @brief Implementation of the Game class for managing a Coup game session.
 *
 * This file contains the implementation of the Game class, which manages the game state,
 * player turns, player list, and game actions. The Game class provides the main interface
 * for adding players, starting the game, progressing turns, and determining the winner.
 */

namespace coup {

/**
 * @brief Returns the singleton instance of the game.
 * Implements the Singleton pattern to ensure only one game instance exists.
 */
Game& Game::getInstance() {
    static Game instance;
    return instance;
}

/**
 * @brief Destructor. Cleans up all dynamically allocated players.
 */
Game::~Game() {
    for (Player* player : _players) {
        delete player;
    }
    _players.clear();
}


/**
 * @brief Adds a new player to the game.
 * Checks for game state, maximum player count, and duplicate names.
 * Uses the PlayerFactory to create the player.
 * @param name Name of the new player.
 * @param role Role of the new player (as string).
 * @return Pointer to the newly created player.
 * @throws std::runtime_error if the game has already started, if there are already 6 players, or if a player with the same name exists.
 */
Player* Game::addPlayer(const std::string& name, const std::string& role) {
    // Prevent adding players after the game has started
    if (_gameActive)
        throw std::runtime_error("Cannot add players after the game has started");
    // Enforce maximum player count
    if (_numPlayers >= 6)
        throw std::runtime_error("Maximum players is 6");
    // Check for duplicate player names
    for (const auto& existingPlayer : _players) {
        if (existingPlayer->getName() == name) {
            throw std::runtime_error("Player with this name already exists");
        }
    }
    // Create new player using the factory
    Player* newPlayer = createPlayer(name, role);
    // Add the new player to the list
    _players.push_back(newPlayer);
    _numPlayers++;
    return newPlayer;
}

/**
 * @brief Starts the game. Sets the game as active.
 * @throws std::runtime_error if there are fewer than 2 or more than 6 players.
 */
void Game::startGame() {
    // Game must have between 2 and 6 players to start
    if (_numPlayers < 2 || _numPlayers > 6) {
        throw std::runtime_error("Illegal number of players to start the game");
    }
    _gameActive = true;
}



/**
 * @brief Prints the name of the player whose turn it is.
 * @throws std::runtime_error if there are no players in the game.
 */
void Game::turn() const {
    // Check if there are any players in the game
    if (_numPlayers == 0) {
        throw std::runtime_error("No players in the game");
    }
    // Print the name of the current player whose turn it is
    std::cout << "It's " << _players[_currentPlayerIndex]->getName() << "'s turn." << std::endl;
}


/**
 * @brief Advances the turn to the next active player.
 * Handles special logic for Merchant role and bribed status.
 * @throws std::runtime_error if there are not enough active players to continue (less than 2).
 */
void Game::nextTurn() {
    // Count active players dynamically to ensure accurate game state
    int activePlayerCount = 0;
    for (const auto& player : _players) {
        if (player->isActive()) {
            activePlayerCount++;
        }
    }
    
    // There must be at least two active players to continue the game
    if (activePlayerCount <= 1) {
        throw std::runtime_error("Not enough players to continue the game");
    }

    // Reset bribed flag for the current player if it was set in the previous turn
    if (_players[_currentPlayerIndex]->getIsBribed()) {
        _players[_currentPlayerIndex]->setIsBribed(false);
    }

    // If the current player is a Merchant and has at least 3 coins, grant a bonus coin
    if (_players[_currentPlayerIndex]->getRole() == Role::Merchant && _players[_currentPlayerIndex]->getCoins() >= 3) {
        _players[_currentPlayerIndex]->setCoins(_players[_currentPlayerIndex]->getCoins() + 1);
        std::cout << _players[_currentPlayerIndex]->getName() << " received an extra coin for being a Merchant." << std::endl;
    }

    // Advance to the next active player in circular order
    do {
        _currentPlayerIndex = (_currentPlayerIndex + 1) % _players.size();
    } while (!_players[_currentPlayerIndex]->isActive());

    // Announce the next player's turn
    turn();
}


/**
 * @brief Returns the names of all active players in the game.
 * @return Vector of strings representing the names of active players.
 * @throws std::runtime_error if there are no players in the game.
 */
std::vector<std::string> Game::getActivePlayersName() const {
    // If there are no players, throw an error
    if (_numPlayers == 0) {
        throw std::runtime_error("No players in the game");
    }
    std::vector<std::string> names;
    // Collect names of all active players
    for (const auto& player : _players) {
        if (player->isActive()) {
            names.push_back(player->getName());
        }
    }
    return names;
}



/**
 * @brief Returns the name of the winner (the last active player).
 * @return Name of the winning player.
 * @throws std::runtime_error if the game is not over yet.
 *         std::runtime_error if no active player is found despite game state indicating one player remains (internal consistency error).

 */
std::string Game::winner() const {
    // Count active players dynamically
    int activePlayers = 0;
    Player* lastActivePlayer = nullptr;
    
    for (const auto& player : _players) {
        if (player->isActive()) {
            activePlayers++;
            lastActivePlayer = player;
        }
    }
    
    // The game is considered over only if one active player remains
    if (activePlayers != 1) {
        throw std::runtime_error("Game is not over yet");
    }
    
    // Return the name of the last active player
    if (lastActivePlayer) {
        return lastActivePlayer->getName();
    }
    
    // Should never reach here if game state is consistent
    throw std::runtime_error("No active player found");
}

/**
 * @brief Resets the game to initial state for testing purposes.
 * Clears all players and resets game state.
 */
void coup::Game::reset() {
    // Clean up existing players
    for (Player* player : _players) {
        delete player;
    }
    _players.clear();
    
    // Reset game state
    _currentPlayerIndex = 0;
    _gameActive = false;
    _numPlayers = 0;
}
}
