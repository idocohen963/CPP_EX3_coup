// idocohen963@gmail.com
#include "baron.hpp"
#include "GAME/game.hpp"

/**
 * @file baron.cpp
 * @brief Implementation of the Baron class in the Coup game.
 * 
 * This file contains the implementations of the Baron class methods.
 * The Baron can invest 3 coins and get 6 coins in return,
 * and receives a compensation coin when sanctioned.
 */

namespace coup {

/**
 * @brief Constructor for the Baron class
 * 
 * Creates a new player with the Baron role.
 * 
 * @param name The Baron's name
 */
Baron::Baron(const std::string &name) : Player(name) {
    role = Role::Baron;
}

/**
 * @brief Default destructor for the Baron class
 */
Baron::~Baron() = default;

/**
 * @brief Returns the list of available actions for the Baron
 * 
 * Adds the investment action (invest) to the basic player's action list.
 * 
 * @return Vector containing all action types the Baron can perform
 */
std::vector<ActionType> Baron::getAvailableActions() const {
    std::vector<ActionType> actions = Player::getAvailableActions();
    actions.push_back(ActionType::Invest);
    return actions;
}

/**
 * @brief Investment action unique to the Baron
 * 
 * Allows the Baron to gain 3 coins as a special ability.
 * The Baron needs to have at least 3 coins to perform this action, but doesn't spend them.
 * This provides a net gain of 3 coins.
 * 
 * @throws std::runtime_error if the Baron doesn't have at least 3 coins to perform this action
 * or if the player is not active, under sanction, or if it's not their turn
 */
void Baron::invest() {
    this->checkActive(); // Check that the player is active
    this->checkSanctioned(); // Check that the player is not under sanction
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    
    // Check that the Baron has at least 3 coins to perform this action
    if (this->playerCoins < 3) {
        throw std::runtime_error("Baron does not have enough coins to invest");
    }
    
    // Gain 3 coins as the Baron's special ability
    this->playerCoins += 3;
    
    
    // Advance turn if the last action was not a bribe
    if (Game::getInstance().getLastStep() != ActionType::Bribe) {
        Game::getInstance().nextTurn();
        this->canArrest = true; // Reset the ability to arrest for the next turn
        this->sanctioned = false; // Reset sanction status for the next turn
    }
    
    Game::getInstance().setLastStep(ActionType::Invest); // Update the last action to Invest
}

} // namespace coup
