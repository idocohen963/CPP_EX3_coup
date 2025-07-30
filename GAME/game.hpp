// idocohen963@gmail.com
#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <vector>
#include "PLAYER/player.hpp"


/**
 * @file game.hpp
 * @brief Definition of the Game class for managing a Coup game session.
 *
 * This file contains the declaration of the Game class, which implements the Singleton pattern
 * and is responsible for managing the game state, player turns, player list, and game actions.
 * The Game class provides the main interface for adding players, starting the game, progressing turns,
 * and determining the winner.
 */

namespace coup {

class Player; // Forward declaration

/**
 * @class Game
 * @brief Main class for managing a Coup game session.
 *
 * Implements the Singleton pattern to ensure only one game instance exists.
 * Manages the list of players, turn order, game state, and last action performed.
 * Provides methods for adding players, starting the game, progressing turns, and determining the winner.
 */
class Game {
private:
    std::vector<Player*> _players;       ///< List of all players in the game
    int _currentPlayerIndex;             ///< Index of the player whose turn it is
    bool _gameActive;                    ///< Flag indicating if the game is currently active
    int _numPlayers;                     ///< Number of active players in the game
    ActionType _lastStep;                ///< Last action performed in the game

    /**
     * @brief Private constructor for Singleton pattern.
     * Initializes the game in its starting state, with no players and inactive.
     */
    Game() : _players(), _currentPlayerIndex(0), _gameActive(false), _numPlayers(0) {
        _players.reserve(6); // Reserve space for up to 6 players
    }

    // Prevent copy and assignment (Singleton enforcement)
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    /**
     * @brief Returns the singleton instance of the game.
     * @return Reference to the single Game instance.
     */
    static Game& getInstance();

    /**
     * @brief Destructor. Cleans up all dynamically allocated players.
     */
    ~Game();

    /**
     * @brief Adds a new player to the game.
     * @param name Name of the new player.
     * @param role Role of the new player (as string).
     * @return Pointer to the newly created player.
     * @throws std::runtime_error if the game has already started, if there are already 6 players, or if a player with the same name exists.
     */
    Player* addPlayer(const std::string& name, const std::string& role);

    /**
     * @brief Starts the game. Sets the game as active.
     * @throws std::runtime_error if there are fewer than 2 or more than 6 players.
     */
    void startGame();

    /**
     * @brief Prints the name of the player whose turn it is.
     * @throws std::runtime_error if there are no players in the game.
     */
    void turn() const;

    /**
     * @brief Returns the name of the winner (the last active player).
     * @return Name of the winning player.
     * @throws std::runtime_error if the game is not over yet.
                std::runtime_error if no active player is found despite game state indicating one player remains (internal consistency error).
     */
    std::string winner() const;

    /**
     * @brief Advances the turn to the next active player.
     * Handles special logic for Merchant role and bribed status.
     * @throws std::runtime_error if there are not enough active players to continue (less than 2).
     */
    void nextTurn();

    /**
     * @brief Returns the last action performed in the game.
     * @return Last action type.
     */
    ActionType getLastStep() const {
        return _lastStep;
    }

    /**
     * @brief Sets the last action performed in the game.
     * @param action The new last action.
     */
    void setLastStep(ActionType action) {
        _lastStep = action;
    }

    /**
     * @brief Returns the number of active players in the game.
     * @return Number of active players.
     */
    int getNumPlayers() const {
        return _numPlayers;
    }

    /**
     * @brief Sets the number of active players in the game.
     * @param num New number of active players.
     */
    void setNumPlayers(int num) {
        _numPlayers = num;
    }

    /**
     * @brief Returns a reference to the list of all players in the game.
     * @return Constant reference to the vector of players.
     */
    const std::vector<Player*>& getPlayers() const {
        return _players;
    }

    /**
     * @brief Returns the names of all active players in the game.
     * @return Vector of strings representing the names of active players.
     * @throws std::runtime_error if there are no players in the game.
     */
    std::vector<std::string> getActivePlayersName() const;

    /**
     * @brief Returns the index of the current player.
     * @return Index of the current player.
     */
    int getCurrentPlayerIndex() const {
        return _currentPlayerIndex;
    }

    /**
     * @brief Returns a pointer to the current player.
     * @return Pointer to the current player.
     */
    Player* getCurrentPlayer() const { return _players[_currentPlayerIndex]; }

    /**
     * @brief Resets the game to initial state for testing purposes.
     * Clears all players and resets game state.
     */
    void reset();

};

}
#endif