
#include "judge.hpp"
#include <iostream>
#include "GAME/game.hpp"

/**
 * @file judge.cpp
 * @brief Implementation of the Judge class in the Coup game.
 * 
 * This file contains the implementations of the Judge class methods, including
 * the unique cancel action for canceling bribe actions.
 */

namespace coup{
    
    /**
     * @brief Constructor for the Judge class
     * 
     * Creates a new player with the Judge role.
     * 
     * @param name The Judge's name
     */
    Judge::Judge(const std::string &name) : Player(name) {    
        role = Role::Judge;
    }  

    /**
     * @brief Default destructor for the Judge class
     */
    Judge::~Judge() = default;

    /**
     * @brief Cancel bribe action of another player
     * 
     * A unique action for the Judge that allows canceling a bribe action performed as the last action.
     * Advances turn after cancellation and updates the last action to cancel.
     * 
     * @param target The player whose bribe action we want to cancel
     * @throws std::runtime_error if the last action was not a bribe
     */
    void Judge::cancel(Player &target) {
        if (Game::getInstance().getLastStep() != ActionType::Bribe)
        {
            throw std::runtime_error("judge can cancel only bribe");
        }
        target.setIsBribed(false);
        Game::getInstance().nextTurn();
        Game::getInstance().setLastStep(ActionType::cancel);
        return;
    }

    /**
     * @brief Returns the list of available actions for the Judge
     * 
     * Adds the cancel action to the basic player's action list.
     * 
     * @return Vector containing all action types the Judge can perform
     */
    std::vector<ActionType> Judge::getAvailableActions() const{
        std::vector<ActionType> action = Player::getAvailableActions();
        action.push_back(ActionType::cancel);
        return action;
    }

}