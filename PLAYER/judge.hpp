// idocohen963@gmail.com

#ifndef JUDGE_HPP
#define JUDGE_HPP
#include "player.hpp"

/**
 * @file judge.hpp
 * @brief Definition of the Judge class in the Coup game.
 * 
 * This file contains the definition of the Judge class, which inherits from the Player class
 * and adds unique abilities for the Judge role, such as canceling bribe actions.
 */

namespace coup{

/**
 * @class Judge
 * @brief Class representing the Judge role in the game.
 * 
 * The Judge is a unique role that can cancel bribe actions of other players.
 * Additionally, if attacked via sanction, the player who imposed the sanction pays an extra coin to the treasury.
 */
class Judge: public Player {
friend Player* createPlayer(std::string name, std::string roleStr); // The only way to create a player is through the factory

private:
    /**
     * @brief Private constructor for the Judge class
     * 
     * The constructor is private to enforce object creation only through the Factory.
     * 
     * @param name The player's name
     */
    Judge(const std::string &name);

public:
    /**
     * @brief Destructor for the Judge class
     */
    ~Judge();
    
    /**
     * @brief Cancel bribe action of another player
     * 
     * This action cancels a bribe action of another player, causing them to lose the 4 coins they paid.
     * Can only be activated if the last action in the game was a bribe.
     * 
     * @param target The player whose bribe action we want to cancel
     * @throws std::runtime_error if the last action was not a bribe
     */
    void cancel(Player &target) override;
    
    /**
     * @brief Returns the list of available actions for the Judge
     * 
     * Adds the cancel action to the basic player's action list.
     * 
     * @return Vector of action types the Judge can perform
     */
    std::vector<ActionType> getAvailableActions() const override;
    
    /**
     * @brief Returns the role type of the player
     * 
     * @return Role::Judge - the Judge role
     */
    Role getRoleType() const override { 
        return Role::Judge;
    }
    
    /**
     * @brief Checks if the Judge can cancel a specific action
     * 
     * The Judge can only cancel bribe actions.
     * 
     * @param action The action to be cancelled
     * @return true if the action is a bribe, otherwise false
     */
    bool canCancel(ActionType action) const override { 
        return action == ActionType::Bribe;
    }   
};
}
#endif
