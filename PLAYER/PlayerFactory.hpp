// idocohen963@gmail.com
#pragma once
#include "GAME/game.hpp"
#include "player.hpp"

/**
 * @file PlayerFactory.hpp
 * @brief Definition of Factory functions for creating players in the Coup game.
 * 
 * This file contains the definition of functions used as Factory for creating
 * players of different roles in the game.
 */

namespace coup {

/**
 * @brief Creates a new player with a specific role
 * 
 * Factory function that receives a name and role and creates a player from the appropriate class.
 * This is the recommended way to create new players in the game.
 * 
 * @param name The new player's name
 * @param role The player's role
 * @return Pointer to the newly created player
 */
    Player* createPlayer(std::string name, std::string roleStr);


}