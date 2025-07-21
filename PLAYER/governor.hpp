#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP
#include "player.hpp"

/**
 * @file governor.hpp
 * @brief Definition of the Governor class in the Coup game.
 * 
 * This file contains the definition of the Governor class, which inherits from the Player class
 * and adds unique abilities for the Governor role, such as taking 3 coins in tax action
 * and canceling tax actions of other players.
 */

namespace coup {

/**
 * @class Governor
 * @brief Class representing the Governor role in the game.
 * 
 * The Governor is a unique role that takes 3 coins from the treasury in tax action instead of 2,
 * and can cancel tax actions of other players.
 */
class Governor : public Player {
friend Player* createPlayer(std::string name, std::string roleStr); // The only way to create a player is through the factory

private:
    /**
     * @brief Private constructor for the Governor class
     * 
     * The constructor is private to enforce object creation only through the Factory.
     * 
     * @param name The Governor's name
     */
    Governor(const std::string &name);

public:
    /**
     * @brief Destructor for the Governor class
     */
    ~Governor();
    
    /**
     * @brief Tax action adapted for the Governor
     * 
     * Adds 3 coins to the Governor instead of 2 like regular players.
     */
    void tax() override;
    
    /**
     * @brief Cancel tax action of another player
     * 
     * A unique action for the Governor that allows canceling a tax action performed as the last action.
     * 
     * @param target The player whose tax action we want to cancel
     * @throws std::runtime_error if the last action was not a tax
     */
    void cancel(Player &target) override;
    
    /**
     * @brief Returns the list of available actions for the Governor
     * 
     * Adds the cancel action to the basic player's action list.
     * 
     * @return Vector containing all action types the Governor can perform
     */
    std::vector<ActionType> getAvailableActions() const override;
    
    /**
     * @brief Returns the role type of the player
     * 
     * @return Role::Governor - the Governor role
     */
    Role getRoleType() const override { 
        return Role::Governor;
    }
    
    /**
     * @brief Checks if the Governor can cancel a specific action
     * 
     * The Governor can only cancel tax actions.
     * 
     * @param action The action to be cancelled
     * @return true if the action is a tax, otherwise false
     */
    bool canCancel(ActionType action) const override { 
        return action == ActionType::Tax;
    }   
};

} // namespace coup
#endif // GOVERNOR_HPP