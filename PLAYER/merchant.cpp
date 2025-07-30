// idocohen963@gmail.com

#include "merchant.hpp"

/**
 * @file merchant.cpp
 * @brief Implementation of the Merchant class in the Coup game.
 * 
 * This file contains the implementations of the Merchant class methods.
 * The Merchant has special behavior when arrested, losing 2 coins to the treasury.
 */

namespace coup
{
    
/**
 * @brief Constructor for the Merchant class
 * 
 * Creates a new player with the Merchant role.
 * 
 * @param name The Merchant's name
 */
Merchant::Merchant(const std::string &name) : Player(name){
    role = Role::Merchant;
}

/**
 * @brief Default destructor for the Merchant class
 */
Merchant::~Merchant() = default;

} // namespace coup



