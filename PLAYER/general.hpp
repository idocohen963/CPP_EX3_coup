#ifndef GENERAL_HPP
#define GENERAL_HPP
#include "player.hpp"

/**
 * @file general.hpp
 * @brief Definition of the General class in the Coup game.
 * 
 * This file contains the definition of the General class, which inherits from the Player class
 * and adds unique abilities for the General role, such as canceling coup actions.
 */

namespace coup {

/**
 * @class General
 * @brief Class representing the General role in the game.
 * 
 * The General is a unique role that can prevent coup actions against another player,
 * at a cost of 5 coins.
 */
class General: public Player {
friend Player* createPlayer(std::string name, std::string roleStr); // The only way to create a player is through the factory

private:
    /**
     * @brief Private constructor for the General class
     * 
     * The constructor is private to enforce object creation only through the Factory.
     * 
     * @param name The General's name
     */
    General(const std::string &name);

public:
    /**
     * @brief Destructor for the General class
     */
    ~General();

    /**
     * @brief Cancel coup action against another player
     * 
     * A unique action for the General that allows canceling a coup action performed against another player.
     * This action costs 5 coins.
     * 
     * @param target The player whose coup action we want to cancel
     * @throws std::runtime_error if the last action was not a coup,
     *                           if the General doesn't have enough coins, 
     *                           or if the General is not active
     */
    void cancel(Player& target) override;
    
    /**
     * @brief Returns the list of available actions for the General
     * 
     * Adds the cancel action to the basic player's action list.
     * 
     * @return Vector containing all action types the General can perform
     */
    std::vector<ActionType> getAvailableActions() const override;
    
    /**
     * @brief Returns the role type of the player
     * 
     * @return Role::General - the General role
     */
    Role getRoleType() const override {
        return Role::General;
    }
    
    /**
     * @brief Checks if the General can cancel a specific action
     * 
     * The General can only cancel coup actions.
     * 
     * @param action The action to be cancelled
     * @return true if the action is a coup, otherwise false
     */
    bool canCancel(ActionType action) const override {
        return action == ActionType::Coup;
    }
};
}
#endif