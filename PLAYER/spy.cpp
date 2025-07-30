// idocohen963@gmail.com
#include "spy.hpp"
#include <iostream>
#include "GAME/game.hpp"

/**
 * @file spy.cpp
 * @brief Implementation of the Spy class in the Coup game.
 * 
 * This file contains the implementations of the Spy class methods.
 * The Spy can see the number of coins of another player and prevent arrest actions.
 */

namespace coup {
    
    /**
     * @brief Constructor for the Spy class
     * 
     * Creates a new player with the Spy role.
     * 
     * @param name The Spy's name
     */
    Spy::Spy(const std::string &name) : Player(name) { 
        role = Role::Spy; // Set the player's role to Spy
    }

    /**
     * @brief Default destructor for the Spy class
     */
    Spy::~Spy() = default;
    
    /**
     * @brief Spy on another player
     * 
     * A unique action for the Spy that reveals the target's coins and prevents
     * them from using arrest in their next turn. This action consumes a turn.
     * 
     * @param target The player to spy on
     * @throws std::runtime_error if trying to spy on themselves or if players are not active
     */
    void Spy::spyOn(Player& target) {
        this->checkTurn();      // Check that it's the Spy's turn
        this->checkMustCoup();  // Check that the player is not required to perform coup
        this->checkActive();    // Check that the Spy is active
        this->checkSanctioned(); // Check that the player is not under sanction
        
        if(target.getName() == this->getName()) { 
            throw std::runtime_error("cannot spy on themselves"); // Cannot spy on themselves
        }
        
        target.checkActive();   // Check that the target player is active
        
        // 1. View coins (information gathering)
        std::cout << target.getName() << " has " << target.getCoins() << " coins.\n";
        
        // 2. Prevent arrest ability for the next turn
        if (target.isCanArrest()) {
            target.setCanArrest(false);  // Prevent arrest ability from the target player
        } else {
            throw std::runtime_error("Player already spyOn(cannot arrest).");  // Player already cannot arrest
        }
        
        // Advance turn
        if (Game::getInstance().getLastStep() != ActionType::Bribe) {
            Game::getInstance().nextTurn();
            this->canArrest = true;     // Reset the ability to arrest for the next turn
            this->sanctioned = false;    // Reset sanction status for the next turn
        }
        
        Game::getInstance().setLastStep(ActionType::SpyOn);  // Update the last action to SpyOn
    }

    /**
     * @brief Returns the list of available actions for the Spy
     * 
     * Adds the spy on action to the basic player's action list.
     * 
     * @return Vector containing all action types the Spy can perform
     */
    std::vector<ActionType> Spy::getAvailableActions() const {
        std::vector<ActionType> action = Player::getAvailableActions();
        action.push_back(ActionType::SpyOn);  // Add spy on action
        return action;
    }
    
}