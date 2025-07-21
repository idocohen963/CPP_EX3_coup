#ifndef PLAYER_HPP
#define PLAYER_HPP
#include <string>
#include <stdexcept>
#include <vector>

/**
 * @file player.hpp
 * @brief Definition of base Player class and helper structures for the Coup game.
 * 
 * This file contains the definition of the base class for a player in the game, including enumerations
 * for roles and action types, as well as declarations for all functions shared by all
 * player types.
 */

namespace coup {

class Game; // Forward declaration of Game class

/**
 * @enum Role
 * @brief representing the different roles in the game.
 * 
 * Each player receives a specific role with special abilities accordingly.
 */
enum class Role{
    Spy,       ///< Spy - can see another player's coins and block arrest actions
    Merchant,  ///< Merchant - gets an extra coin when having 3 or more, loses 2 coins when arrested
    General,   ///< General - can prevent coup against another player, gets a coin back when arrested  
    Governor,  ///< Governor - gets 3 coins in tax action, can cancel tax actions of others
    Judge,     ///< Judge - can cancel bribe actions of other players
    Baron,     ///< Baron - can gain 3 coins through special invest action, receives compensation coin when sanctioned
};

/**
 * @enum ActionType
 * @brief  representing the possible action types in the game.
 * 
 * Every action in the game is performed using one of these options, some are shared by all players
 * and some are unique to certain roles.
 */
enum class ActionType {
    Gather,     ///< Gather - player gets one coin from the treasury
    Tax,        ///< Tax - player gets two coins from the treasury
    Bribe,      ///< Bribe - pay 4 coins to perform an additional action in the turn
    Arrest,     ///< Arrest - take a coin from another player
    Coup,       ///< Coup - remove another player from the game
    Sanction,   ///< Sanction - prevent economic actions from another player
    Invest,     ///< Investment - unique action for Baron (gain 3 coins with prerequisite of having 3 coins)
    SpyOn,      ///< Spy on another player - view coins and prevent arrest ability (unique to Spy)
    cancel      ///< Cancel - unique action for Judge (bribe), Governor (tax), and General (coup)
};

/**
 * @class Player
 * @brief Base class representing a player in the game.
 * 
 * This class defines the basic interface and functionality shared by all player types in the game.
 * Each specific role inherits from this class and adds or modifies behavior according to game rules.
 * This is a semi-abstract class, with one pure virtual method (getRoleType).
 */
class Player{
    protected:
        std::string name;       ///< Player's name
        Role role;              ///< Player's role
        int playerCoins;        ///< Number of coins owned by the player
        bool active;            ///< Whether the player is active in the game (not removed)
        bool sanctioned;        ///< Whether the player is under sanction
        bool lastArrested;      ///< Whether the player was arrested in the previous turn
        bool canArrest;         ///< Whether the player is allowed to arrest in this turn
        bool isBribed;          ///< Whether the player performed a bribe in the last turn

    public:
        /**
         * @brief Constructor for the Player class
         * @param name Player's name
         */
        Player(std::string name);
        
        /**
         * @brief Virtual destructor
         */
        virtual ~Player(); //destructor

        /**
         * @brief Gather action - player gets one coin from the treasury
         * 
         * Checks that the player is active, not sanctioned and it's their turn, then adds one coin.
         * Advances turn if the previous action was not a bribe.
         */
        virtual void gather();
        
        /**
         * @brief Tax action - player gets two coins from the treasury
         * 
         * Checks that the player is active, not sanctioned and it's their turn, then adds two coins.
         * Advances turn if the previous action was not a bribe.
         */
        virtual void tax();
        
        /**
         * @brief Bribe action - allows player to perform an additional action
         * 
         * The player pays 4 coins and marks themselves as "bribed" to perform an additional action.
         * This action does not advance the turn to the next player.
         * 
         * @throws std::runtime_error if the player doesn't have enough coins
         */
        virtual void bribe();
        
        /**
         * @brief Arrest action - taking a coin from another player
         * 
         * Checks that the player is active, not sanctioned and it's their turn, then takes a coin from the other player.
         * Behavior varies according to the role of the arresting player and the arrested player.
         * 
         * @param other The player to be arrested
         * @throws std::runtime_error if the player tries to arrest themselves or if the other player has no coins
         */
        virtual void arrest(Player& other);
        
        /**
         * @brief Sanction action - imposes a sanction on another player
         * 
         * Checks that the player is active and it's their turn, then imposes a sanction on the other player.
         * Sanction prevents the player from performing economic actions (gather, tax) in their next turn.
         * 
         * @param other The player on to impose the sanction
         * @throws std::runtime_error if the player doesn't have enough coins or tries to sanction themselves
         */
        virtual void sanction(Player& other);
        
        /**
         * @brief Coup action - removes another player from the game
         * 
         * Checks that the player is active, not sanctioned and it's their turn, then removes the other player.
         * This action costs 7 coins. The turn advances to the next player only if there are at least 2 
         * players remaining and the last action was not a bribe. If only 1 player remains, the game ends.
         * 
         * @param other The player to be removed
         * @throws std::runtime_error if the player doesn't have enough coins or tries to coup themselves
         */
        virtual void coup(Player& other);
        
        /**
         * @brief Returns the list of available actions for the player
         * 
         * Derived classes can add actions to the basic list.
         * 
         * @return Vector of action types that the player can perform
         */
        virtual std::vector<ActionType> getAvailableActions() const;

        /**
         * @brief Returns the role type of the player
         * 
         * This is a pure virtual function that every derived class must implement.
         * Because of this, it's not possible to create a direct object of type Player, only from inheriting classes.
         * 
         * @return The role type of the player (of type Role)
         */
        virtual Role getRoleType() const = 0;
    
        /** @name Access functions (Getters and Setters)
         * Functions for accessing and updating class variables
         */
        ///@{
        
        /**
         * @brief Returns the player's name
         * @return Player's name as a string
         */
        std::string getName() const { return name; }
        
        /**
         * @brief Returns the number of coins the player has
         * @return Number of coins
         */
        int getCoins() const { return playerCoins; }
        
        /**
         * @brief Checks if the player is active in the game
         * @return true if the player is active, otherwise false
         */
        bool isActive() const { return active; }
        
        /**
         * @brief Updates the player's activity status
         * @param active The new status value
         */
        void setActive(bool active) { this->active = active; }
        
        /**
         * @brief Checks if the player is under sanction
         * @return true if under sanction, otherwise false
         */
        bool isSanctioned() const { return sanctioned; }
        
        /**
         * @brief Updates the player's sanction status
         * @param sanctioned The new status value
         */
        void setSanctioned(bool sanctioned) { this->sanctioned = sanctioned; }
        
        /**
         * @brief Checks if the player was arrested in the last turn
         * @return true if arrested in the last turn, otherwise false
         */
        bool isLastArrested() const { return lastArrested; }
        
        /**
         * @brief Checks if the player can arrest other players
         * @return true if can arrest, otherwise false
         */
        bool isCanArrest() const { return canArrest; }
        
        /**
         * @brief Updates the player's ability to arrest
         * @param canArrest The new status value
         */
        void setCanArrest(bool canArrest) { this->canArrest = canArrest; }
        
        /**
         * @brief Checks if the player performed a bribe
         * @return true if performed a bribe, otherwise false
         */
        bool getIsBribed() const { return isBribed; }
        
        /**
         * @brief Updates the player's bribe status
         * @param isBribed The new status value
         */
        void setIsBribed(bool isBribed) { this->isBribed = isBribed; }
        
        /**
         * @brief Updates the player's number of coins
         * @param coins The new number of coins
         */
        void setCoins(int coins) { this->playerCoins = coins; }
        
        /**
         * @brief Returns the player's role
         * @return The player's role (of type Role)
         */
        Role getRole() const { return role; }
        ///@}
        
        /**
         * @brief Checks if it's the player's turn
         * 
         * @throws std::runtime_error if it's not the player's turn
         */
        void checkTurn() const;
        
        /**
         * @brief Checks if the player is under sanction
         * 
         * @throws std::runtime_error if the player is under sanction
         */
        void checkSanctioned() const;
        
        /**
         * @brief Checks if the player is active in the game
         * 
         * @throws std::runtime_error if the player is not active
         */
        void checkActive() const;
        
       // ...existing code...
        /**
         * @brief Checks if the player must perform coup (10+ coins)
         * 
         * @throws std::runtime_error if the player must perform coup
         */
        void checkMustCoup() const; 

        /** @name Actions unique to certain roles
         * Actions that will be redefined in inheriting classes for specific roles
         */
        ///@{
        
        /**
         * @brief Investment action (unique action for Baron)
         * 
         * This method will be redefined in the Baron class.
         * 
         * @throws std::runtime_error when called from the base class
         */
        virtual void invest(){ 
            throw std::runtime_error("invest not implemented for base Player.");
        }

        /**
         * @brief Spy on another player (unique action for Spy)
         * 
         * This method will be redefined in the Spy class.
         * 
         * @param target The player to spy on
         * @throws std::runtime_error when called from the base class
         */
        virtual void spyOn(Player& target) { 
            (void)target; // Avoid unused parameter warning
            throw std::runtime_error("spyOn not implemented for base Player.");
        }

        /**
         * @brief Cancel - unique action for Judge (bribe) and General (coup)
         * 
         * This method will be redefined in the Judge class.
         * 
         * @param target The player whose action we want to cancel
         * @throws std::runtime_error when called from the base class
         */
        virtual void cancel(Player& target){
            (void)target; // Avoid unused parameter warning
            throw std::runtime_error("cancel not implemented for base Player.");
        }
        
        /**
         * @brief Checks if the player can cancel a specific action
         * 
         * This method will be redefined in the relevant inheriting classes.
         * 
         * @param action The action to be cancelled
         * @return true if the player can cancel the action, otherwise false
         */
        virtual bool canCancel(ActionType action) const {
            (void)action; // Avoid unused parameter warning
            return false; // Default implementation, will be overridden in inheritance
        }
        ///@}
        

};
}
#endif