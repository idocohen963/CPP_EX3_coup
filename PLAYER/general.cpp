// idocohen963@gmail.com
#include "general.hpp"
#include "GAME/game.hpp"

/**
 * @file general.cpp
 * @brief Implementation of the General class in the Coup game.
 * 
 * This file contains the implementations of the General class methods.
 * The General can cancel coup actions performed against another player.
 */

namespace coup {

/**
 * @brief Constructor for the General class
 * 
 * Creates a new player with the General role.
 * 
 * @param name The General's name
 */
General::General(const std::string &name) : Player(name) {
    role = Role::General; // Set the player's role to General
}

/**
 * @brief Default destructor for the General class
 */
General::~General() = default;

/**
 * @brief Cancel coup action against another player
 * 
 * A unique action for the General that allows canceling a coup action performed against another player.
 * This action costs 5 coins and restores the target player to the game.
 * 
 * @param target The player whose coup action we want to cancel
 * @throws std::runtime_error if the last action was not a coup,
 *                           if the General doesn't have enough coins,
 *                           or if the General is not active
 */
void General::cancel(Player& target) {
    if(Game::getInstance().getLastStep() != ActionType::Coup) { // Check if the last action was a coup
        throw std::runtime_error("General can cancel only coup."); // If not, throw an error
    }
    if(target.getName()== this->getName()) { // Check if the target is the player themselves
        throw std::runtime_error("cannot undo their own Action"); // Throw error if trying to cancel their own action
    }

    if(this->playerCoins < 5) { // Check that the General has at least 5 coins
        throw std::runtime_error("Not enough coins to prevent coup."); // If not, throw error
    }
    
    this->playerCoins -= 5; // Deduct 5 coins from the General
    Game::getInstance().setNumPlayers(Game::getInstance().getNumPlayers() + 1); // Increase the number of players in the game
    target.setActive(true); // Reactivate the target player

    Game::getInstance().setLastStep(ActionType::cancel);  // Update the last action to cancel
}


/**
 * @brief Returns the list of available actions for the General
 * 
 * Adds the cancel action to the basic player's action list.
 * 
 * @return Vector containing all action types the General can perform
 */
std::vector<ActionType> General::getAvailableActions() const{
    std::vector<ActionType> action = Player::getAvailableActions();
    action.push_back(ActionType::cancel); // Add cancel action
    return action;
}
}