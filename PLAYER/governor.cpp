// idocohen963@gmail.com

#include "governor.hpp"
#include "GAME/game.hpp"

/**
 * @file governor.cpp
 * @brief Implementation of the Governor class in the Coup game.
 * 
 * This file contains the implementations of the Governor class methods.
 * The Governor gets 3 coins in tax action and can cancel tax actions of other players.
 */

namespace coup{

/**
 * @brief Constructor for the Governor class
 * 
 * Creates a new player with the Governor role.
 * 
 * @param name The Governor's name
 */
Governor::Governor(const std::string &name) : Player(name){
    role = Role::Governor;
}

/**
 * @brief Default destructor for the Governor class
 */
Governor::~Governor() = default;

/**
 * @brief Tax action implementation adapted for the Governor
 * 
 * The Governor gets 3 coins in tax action instead of 2 like regular players.
 * Advances turn if the previous action was not a bribe.
 */
void Governor::tax() {
    this->checkActive(); // Check that the player is active
    this->checkSanctioned(); // Check that the player is not under sanction
    this->checkTurn(); // Check that it's the player's turn
    this->checkMustCoup(); // Check that the player is not required to perform coup
    playerCoins += 3; // The Governor gets 3 coins (instead of 2 like regular players)
    if (Game::getInstance().getLastStep() != ActionType::Bribe) { // If the last action was not a bribe, advance to the next turn
        Game::getInstance().nextTurn();
        this->canArrest = true; // Reset the ability to arrest for the next turn
        this->sanctioned = false; // Reset sanction status for the next turn
    }
    Game::getInstance().setLastStep(ActionType::Tax); // Update the last action to Tax
}

/**
 * @brief Cancel tax action of another player
 * 
 * A unique action for the Governor that allows canceling a tax action performed as the last action.
 * When canceling the action, the player loses the coins they received (2 or 3 depending on their role).
 * 
 * @param target The player whose tax action we want to cancel
 * @throws std::runtime_error if the last action was not a tax or if trying to cancel their own action
 */
void Governor::cancel(Player &target) {
    if (Game::getInstance().getLastStep() != ActionType::Tax)
    {
        throw std::runtime_error("governor can cancel only tax");
    }
    if(target.getName()== this->getName()) { // Check if the target is the Governor themselves
        throw std::runtime_error("cannot undo their own Action"); // Throw error if trying to cancel their own action
    }
    this->checkActive(); // Check that the Governor is active
    target.checkActive(); // Check that the target player is active
    
    // Reduce the number of coins according to the player's role
    if(target.getRole()==Role::Governor) {
        target.setCoins(target.getCoins()-3);  // Another Governor loses 3 coins
    }else{ // Other roles
        target.setCoins(target.getCoins()-2);  // Regular player loses 2 coins
    }
    Game::getInstance().setLastStep(ActionType::cancel);
    return;
}

/**
 * @brief Returns the list of available actions for the Governor
 * 
 * Adds the cancel action to the basic player's action list.
 * 
 * @return Vector containing all action types the Governor can perform
 */
std::vector<ActionType> Governor::getAvailableActions() const {
    std::vector<ActionType> action = Player::getAvailableActions();
    action.push_back(ActionType::cancel);
    return action;
}


}