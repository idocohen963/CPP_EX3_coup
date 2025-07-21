
#ifndef BARON_HPP
#define BARON_HPP
#include "player.hpp"

/**
 * @file baron.hpp
 * @brief Definition of the Baron class in the Coup game.
 * 
 * This file contains the definition of the Baron class, which inherits from the Player class
 * and adds unique abilities for the Baron role, such as coin investment
 * and receiving a compensation coin when sanctioned.
 */

namespace coup{

/**
 * @class Baron
 * @brief Class representing the Baron role in the game.
 * 
 * The Baron is a unique role that can gain 3 coins through a special "invest" action,
 * requiring at least 3 coins to perform but not spending them.
 * Additionally, if attacked via sanction, the Baron receives one coin as compensation.
 */
class Baron : public Player{
friend Player* createPlayer(std::string name, std::string roleStr); // The only way to create a player is through the factory

private:
    /**
     * @brief Private constructor for the Baron class
     * 
     * The constructor is private to enforce object creation only through the Factory.
     * 
     * @param name The Baron's name
     */
    Baron(const std::string &name);

public:
    /**
     * @brief Destructor for the Baron class
     */
    ~Baron();

    /**
     * @brief Returns the list of available actions for the Baron
     * 
     * Adds the investment action (invest) to the basic player's action list.
     * 
     * @return Vector containing all action types the Baron can perform
     */
    std::vector<ActionType> getAvailableActions() const override;

    /**
     * @brief Investment action unique to the Baron
     * 
     * Allows the Baron to gain 3 coins as a special ability.
     * The Baron needs to have at least 3 coins to perform this action, but doesn't spend them.
     * This provides a net gain of 3 coins.
     * 
     * @throws std::runtime_error if the Baron doesn't have at least 3 coins to perform this action
     */
    void invest() override;
   
    /**
     * @brief Returns the role type of the player
     * 
     * @return Role::Baron - the Baron role
     */
    Role getRoleType() const override { 
        return Role::Baron;
    }
};

} // namespace coup
#endif // BARON_HPP