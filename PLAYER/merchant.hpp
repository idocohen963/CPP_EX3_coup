
#ifndef MERCHANT_HPP
#define MERCHANT_HPP
#include "player.hpp"

/**
 * @file merchant.hpp
 * @brief Definition of the Merchant class in the Coup game.
 * 
 * This file contains the definition of the Merchant class, which inherits from the Player class
 * and represents the Merchant role with its specific behavior in arrest and economic actions.
 */

namespace coup
{

/**
 * @class Merchant
 * @brief Class representing the Merchant role in the game.
 * 
 * The Merchant is a role that has special behavior when arrested:
 * - When arrested, loses 2 coins to the treasury (not to the arresting player)
 * - Gets bonus coins when having 3 or more coins in certain game variants
 */
class Merchant : public Player{
friend Player* createPlayer(std::string name , std::string roleStr); // The only way to create a player is through the factory

private:
    /**
     * @brief Private constructor for the Merchant class
     * 
     * The constructor is private to enforce object creation only through the Factory.
     * 
     * @param name The Merchant's name
     */
    Merchant(const std::string &name);

public:
    /**
     * @brief Destructor for the Merchant class
     */
    ~Merchant();
    
    /**
     * @brief Returns the role type of the player
     * 
     * @return Role::Merchant - the Merchant role
     */
    Role getRoleType() const override { // Returns the role type of the Merchant player.
        return Role::Merchant;
    }
    
};  
} // namespace coup
#endif