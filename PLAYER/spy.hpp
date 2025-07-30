// idocohen963@gmail.com
#ifndef SPY_HPP
#define SPY_HPP
#include "player.hpp"

/**
 * @file spy.hpp
 * @brief Definition of the Spy class in the Coup game.
 * 
 * This file contains the definition of the Spy class, which inherits from the Player class
 * and adds unique abilities for the Spy role, such as viewing coins of another player
 * and preventing arrest actions from other players in their next turn.
 */

namespace coup {

/**
 * @class Spy
 * @brief Class representing the Spy role in the game.
 * 
 * The Spy is a unique role that can spy on another player to view their coins
 * and prevent them from using arrest action in their next turn.
 * This action costs a turn but does not cost coins.
 */
class Spy: public Player {
friend Player* createPlayer(std::string name, std::string roleStr); // The only way to create a player is through the factory

private:
    /**
     * @brief Private constructor for the Spy class
     * 
     * The constructor is private to enforce object creation only through the Factory.
     * 
     * @param name The Spy's name
     */
    Spy(const std::string &name);

public:
    /**
     * @brief Destructor for the Spy class
     */
    ~Spy();
    
    /**
     * @brief Spy on another player
     * 
     * A unique action for the Spy that allows seeing the number of coins of another player
     * and prevents them from using arrest action in their next turn.
     * This action costs a turn but does not cost coins.
     * 
     * @param target The player to spy on
     * @throws std::runtime_error if trying to spy on themselves or if players are not active
     */
    void spyOn(Player& target) override;
    
    /**
     * @brief Returns the list of available actions for the Spy
     * 
     * Adds the spy on action to the basic player's action list.
     * 
     * @return Vector containing all action types the Spy can perform
     */
    std::vector<ActionType> getAvailableActions() const override;
    
    /**
     * @brief Returns the role type of the player
     * 
     * @return Role::Spy - the Spy role
     */
    Role getRoleType() const override {
        return Role::Spy;
    }
};

} // namespace coup
#endif // SPY_HPP