// idocohen963@gmail.com
#include "PlayerFactory.hpp"
#include "governor.hpp"
#include "spy.hpp"
#include "baron.hpp"
#include "general.hpp"
#include "judge.hpp"
#include "merchant.hpp"

/**
 * @file PlayerFactory.cpp
 * @brief Implementation of Factory functions for creating players in the Coup game.
 * 
 * This file contains the implementation of the factory function that creates
 * players of different roles based on a string parameter.
 */

namespace coup{

    /**
     * @brief Creates a new player with a specific role
     * 
     * Factory function that receives a name and role string and creates a player from the appropriate class.
     * This is the only way to create players in the game, enforcing the factory pattern.
     * 
     * @param name The new player's name
     * @param roleStr String representation of the player's role 
     *                ("Spy", "Merchant", "General", "Governor", "Judge", "Baron")
     * @return Pointer to the newly created player
     * @throws std::invalid_argument if roleStr doesn't match any valid role
     */
    Player* createPlayer(std::string name, std::string roleStr){
    if (roleStr == "Spy") return new Spy(name);
    if (roleStr == "Merchant") return new Merchant(name);
    if (roleStr == "General") return new General(name);
    if (roleStr == "Governor") return new Governor(name);
    if (roleStr == "Judge") return new Judge(name);
    if (roleStr == "Baron") return new Baron(name);
    throw std::invalid_argument("Invalid role string");    // Throw exception for invalid role strings
    };

}